# -*- coding: UTF-8 -*-

from xml.dom import minidom             
import os, sys, re, types, shutil                  
from datetime import datetime

import imp 
imp.reload(sys) 
sys.setdefaultencoding('utf-8') #设置默认编码,只能是utf-8

##############################################################################################################################
# classes 

# baseClass      String
# customClass    String
# displayName    String
# memberVarAssignmentName String
# memberVarAssignmentType integer 0/1/2 : no/root/owner

# properties Array
# children Array

class CCBNode(object):
    def __init__(self, baseClass, customClass, displayName, memberVarAssignmentName, memberVarAssignmentType):
        super(CCBNode, self).__init__()
        self.baseClass = baseClass
        self.customClass = customClass
        self.displayName = displayName
        self.memberVarAssignmentName = memberVarAssignmentName
        self.memberVarAssignmentType = memberVarAssignmentType

##############################################################################################################################
# parse utils

def getNodeText(root):
    rc = ''
    for node in root.childNodes:
        if node.nodeType == node.TEXT_NODE:
            rc = rc + node.data
    if root.nodeName == 'integer':
        rc = int(rc)
    elif root.nodeName == 'real':
        rc = float(rc)

    return rc

def isNode(node, name):
    return (node.nodeType == node.ELEMENT_NODE and node.nodeName == name)

def getNodeKeyValueStepTuple(nodes, keyIndex):
    step = 2
    key = nodes[keyIndex]
    if isNode(key, 'key'):
        value = nodes[keyIndex + step]
        if value.nodeType == value.ELEMENT_NODE:
            return (key, value, step)
        else:
            return (None, None, step)
    return (None, None, 0)

def handleNodes(nodes, handleFunc):
    ''' handleFunc(keyNode, valueNode) '''
    for i in xrange(0, nodes.length - 2):
        nodeTuple = getNodeKeyValueStepTuple(nodes, i)
        keyNode = nodeTuple[0]
        valueNode = nodeTuple[1]
        step = nodeTuple[2]
        i = i + step
        if keyNode != None and valueNode != None:
            handleFunc(keyNode, valueNode)

##############################################################################################################################
# parse

def parseProperty(properties):
    '''
    name: ccControl, tag, string
    '''
    names = []
    values = []
    def cb(keyNode, valueNode):
        key = getNodeText(keyNode)
        if key == 'name' and isNode(valueNode, 'string'):
            value = getNodeText(valueNode)
            names.append(value)
        elif key == 'value':
            values.append(valueNode)

    for child in properties.childNodes:
        if isNode(child, 'dict'):
            handleNodes(child.childNodes, cb)

    ret = {}
    for x in xrange(0, len(names)):
        n = names[x]
        v = values[x]
        if n == 'ccControl':
            for c in v.childNodes:
                if isNode(c, 'string'):
                    ret[n] = getNodeText(c)        
        elif n == 'tag':
            # print(v, v.childNodes[0].nodeValue)
            # v.childNodes[0].nodeValue = v.childNodes[0].nodeValue + '00'
            # print(v, v.childNodes[0].nodeValue)
            ret[n] = getNodeText(v)
        elif n == 'string':
            ret[n] = getNodeText(v)

    return ret

def parseNode(parent, nodeDictXML, depth = 0):
    baseProperties = {}
    properties = {}
    children = []
    propertiesNode = {}

    current = [baseProperties, properties, children, propertiesNode, parent]

    def cb(keyNode, valueNode):
        key = getNodeText(keyNode)
        if isNode(valueNode, 'array'):
            if key == 'properties':
                propertiesNode[key] = valueNode
                propertiesNode['depth'] = depth
                current[1] = parseProperty(valueNode)
            elif key == 'children':
                for child in valueNode.childNodes:
                    if isNode(child, 'dict'):
                        children.append(parseNode(current, child, depth + 1))
        else:
            value = getNodeText(valueNode)
            baseProperties[key] = value

    handleNodes(nodeDictXML.childNodes, cb)

    return current

def parseTimeline(sequences):
    ''' return names of sequences '''
    dicts = []
    for node in sequences.childNodes:
        if isNode(node, 'dict'):
            dicts.append(node)

    names = []

    def cb(keyNode, valueNode):
        key = getNodeText(keyNode)
        if key == 'name' and isNode(valueNode, 'string'):
            value = getNodeText(valueNode)
            if len(value) > 0:
                names.append(value)

    for aDict in dicts:
        handleNodes(aDict.childNodes, cb)

    return names

##############################################################################################################################

def test_exportMemVarNames(nodeGraphTuple, depth = 0):
    prefix = ''
    for x in xrange(0, depth):
        prefix = prefix + '--'
    for x in xrange(0, depth):
        prefix = prefix + '  '

    for k, v in nodeGraphTuple[0].iteritems():
        if k == 'displayName':
            print(prefix + k + ' ' + v)
    for k, v in nodeGraphTuple[1].iteritems():
        if k == 'displayName':
            print(prefix + k + ' ' + v)

    depth = depth + 1
    for x in xrange(0, len(nodeGraphTuple[2])):
        test_exportMemVarNames(nodeGraphTuple[2][x], depth)

def exportMemVarNames(nodeGraphTuple, depth = 0):
    memVars = []
    memVarTexts = []

    memberVarAssignmentName = nodeGraphTuple[0]['memberVarAssignmentName']
    if len(memberVarAssignmentName) > 0:
        baseClass = nodeGraphTuple[0]['baseClass']
        # print('%s* %s;' % (baseClass, memberVarAssignmentName))
        baseClass = baseClass.replace('CCSprite', 'Sprite')
        baseClass = baseClass.replace('CCLabelTTF', 'Label')
        baseClass = baseClass.replace('CCScale9Sprite', 'ui::Scale9Sprite')
        baseClass = baseClass.replace('CCNode', 'Node')
        baseClass = baseClass.replace('CCControlButton', 'ControlButton')
        memVars.append((baseClass, memberVarAssignmentName))

    # if nodeGraphTuple[1].has_key('tag'):
        # print('tag:', memberVarAssignmentName, nodeGraphTuple[1]['tag'])
    # if nodeGraphTuple[1].has_key('ccControl'):
        # print('ccControl:', memberVarAssignmentName, nodeGraphTuple[1]['ccControl'])
    if nodeGraphTuple[1].has_key('string'):
        memVarTexts.append((memberVarAssignmentName, nodeGraphTuple[1]['string']))
        # print('string:', memberVarAssignmentName, nodeGraphTuple[1]['string'])

    depth = depth + 1
    for x in xrange(0, len(nodeGraphTuple[2])):
        mv = exportMemVarNames(nodeGraphTuple[2][x], depth)
        if len(mv[0]) > 0:
            memVars.extend(mv[0])
        if len(mv[1]) > 0:
            memVarTexts.extend(mv[1])

    return (memVars, memVarTexts)



##############################################################################################################################

def readccb(filepath):
    xmldoc = minidom.parse(filepath)
    plist = xmldoc.documentElement
    # plist.childNodes[0] is <DOM Text node "u'\n    '">
    dict1st = plist.childNodes[1]

    ret = {}
    ret['sequences'] = None
    def cb(keyNode, valueNode):
        key = getNodeText(keyNode)
        if isNode(valueNode, 'array') and key == 'sequences':
            ret['sequences'] = valueNode
        elif isNode(valueNode, 'dict') and key == 'nodeGraph':
            ret['nodeGraph'] = valueNode

    handleNodes(dict1st.childNodes, cb)

    # f = file(dirpath + 'x' + filename, 'w')
    # import codecs
    # writer = codecs.lookup('utf-8')[3](f)
    # xmldoc.writexml(writer, encoding='utf-8')
    # writer.close()

    return (ret['nodeGraph'], ret['sequences'])

##############################################################################################################################

def exportCode(ccb_filepath, className, lua_template):
    ccb = readccb(ccb_filepath)
    nodeGraph = ccb[0]
    sequences = ccb[1]
    lua_timeline = ''
    if sequences != None:
        arr = parseTimeline(sequences)
        for a in arr:
            lua_timeline = lua_timeline + '    ccbNode[\'%s\'] = \'%s\'\n' % (a, a)
    nodes = parseNode(None, nodeGraph)
    memVarsAndTxts = exportMemVarNames(nodes)
    memVars = memVarsAndTxts[0] # 变量
    memTxts = memVarsAndTxts[1] # 文本

    isLua = lua_template.find('@@lua_timeline') > 0
    lua_className = className
    lua_memberVarSetter = ''

    # no members
    if len(memVars) <= 0:
        lua_template = lua_template.replace('@@lua_className', lua_className)
        lua_template = lua_template.replace('@@lua_memberVarSetter', lua_memberVarSetter)
        lua_template = lua_template.replace('@@lua_timeline', lua_timeline)
        return lua_template

    for mv in memVars:
        lua_memberVarSetter = '%s    ccbNode[\'%s\'] = memberTable[\'%s\'] -- %s\n' % (lua_memberVarSetter, mv[1], mv[1], mv[0])

    lua_export_text = "    ccbNode['exportTexts'] = {}\n"
    for txt in memTxts:
        if txt[0].find('export') >= 0 and len(txt[0]) > 0:
            content = txt[1].replace('\n', '"\n"').replace('\t', '"\t"')
            lua_export_text = "%s    ccbNode['exportTexts'][ ccbNode['%s'] ] = '%s'\n" % (lua_export_text, txt[0], content)

    # ----------------------------------------

    lua_template = lua_template.replace('@@lua_className', lua_className)
    lua_template = lua_template.replace('@@lua_memberVarSetter', lua_memberVarSetter)
    lua_template = lua_template.replace('@@lua_timeline', lua_timeline)
    lua_template = lua_template.replace('@@lua_export_text', lua_export_text)

    # ----------------------------------------

    return lua_template

##############################################################################################################################

def isExportedForLua(filepath):
    import ccb_cfg
    ret = False
    for f in ccb_cfg.exportForLua:
        if filepath.find(f) >= 0:
            ret = True
            break
    return ret

def create_lua_files_and_ccbi(ccbFilesPath, lua_template, codePath, ccbpublish, ccbi_path):
    '''
    ccbFilesPath : ccb 原文件目录
    lua_template : lua 模版
    codePath : lua 导出根目录
    ccbpublish : ccbi 导出命令行工具
    ccbi_path : ccbi 导出根目录
    '''
    import subprocess

    list_dirs = os.walk(ccbFilesPath)
    for root, dirs, files in list_dirs: 
        for f in files:
            if os.path.splitext(f)[1] == '.ccb':
                ccb_filepath = os.path.join(root, f)

                ccbName = f.replace('.ccb', '')

                lua_file_dir = codePath + root.replace(ccbFilesPath, '') + '/' # 代码文件细分目录
                if os.path.exists(lua_file_dir) == False:
                    os.makedirs(lua_file_dir)
                print(lua_file_dir, ccbName)

                lua_code = exportCode(ccb_filepath, ccbName, lua_template)
                lua_file = open(lua_file_dir + ccbName + '.lua', 'w')
                lua_file.write(lua_code)

                ccbiFolder = ccbi_path + root.replace(ccbFilesPath, '') + '/'
                if os.path.exists(ccbiFolder) == False:
                    os.makedirs(ccbiFolder)
                ccbi = ccbiFolder + f.replace('.ccb', '.ccbi')
                print(subprocess.Popen("%s -o %s %s" % (ccbpublish, ccbi, ccb_filepath), shell=True, stdout=subprocess.PIPE).stdout.read())

##############################################################################################################################

def create_lua_requires(res_ccbi, src_ccbi, parent_dir_name, lua_file_path):
    # ccbi
    ccbi = []
    list_dirs = os.walk(res_ccbi)
    for root, dirs, files in list_dirs: 
        for f in files:
            if os.path.splitext(f)[1] == '.ccbi':
                ccbi_name = f.replace('.ccbi', '')
                ccbi_filepath = os.path.join(root, f)
                ccbi_require = parent_dir_name + ccbi_filepath.replace(res_ccbi, '')
                require = "gt.%s.%s.filename = '%s'" % (parent_dir_name, ccbi_name, ccbi_require)
                ccbi.append(require)

    # lua
    lua = []
    list_dirs = os.walk(src_ccbi)
    for root, dirs, files in list_dirs: 
        for f in files:
            if os.path.splitext(f)[1] == '.lua':
                ccbi_name = f.replace('.lua', '')
                ccbi_filepath = os.path.join(root, ccbi_name)
                ccbi_require = parent_dir_name + ccbi_filepath.replace(src_ccbi, '')
                require = "gt.%s.%s = require '%s'" % (parent_dir_name, ccbi_name, ccbi_require)
                lua.append(require)

    c = ''
    for s in lua:
        c = c + s + '\n'
    c = c + '\n'
    for s in ccbi:
        c = c + s + '\n'

    content = '''
local gt = gt
gt.ccbi = {}

%s
''' % c

    lua_file = open(lua_file_path, 'w')
    lua_file.write(content)

##############################################################################################################################

def recreate_folder(folder_path):
    if os.path.exists(folder_path):
        shutil.rmtree(folder_path)
    if os.path.exists(folder_path) == False:
        os.makedirs(folder_path)

##############################################################################################################################

def main(ccb_path, TO_CCBI_PATH, TO_LUA_PATH):
    SCRIPT_PATH = os.path.split(os.path.realpath(__file__))[0]

    CCB_PROJECT_PATH = ccb_path
    # TO_CCBI_PATH = os.path.join(SCRIPT_PATH, '../../res/ccbi')
    # TO_LUA_PATH = os.path.join(SCRIPT_PATH, '../../src/ccbi')
    CCBPUBLISH = os.path.join(SCRIPT_PATH, 'ccbpublish')

    recreate_folder(TO_CCBI_PATH)
    recreate_folder(TO_LUA_PATH)

    # --------------------------------
    lua_template = open(os.path.join(SCRIPT_PATH, 'ccb.lua')).read()
    create_lua_files_and_ccbi(CCB_PROJECT_PATH, lua_template, TO_LUA_PATH, CCBPUBLISH, TO_CCBI_PATH)
    # --------------------------------
    create_lua_requires(TO_CCBI_PATH, TO_LUA_PATH, 'ccbi', os.path.join(TO_LUA_PATH, 'initccbi.lua'))
    # --------------------------------

if __name__ == "__main__":
    if len(sys.argv) <= 1 or sys.argv[1] == "":
        print('\n***\n   ERROR: 没有指定 ccb 原文件目录\n***')
    else:
        main(sys.argv[1], sys.argv[2], sys.argv[3])

    
