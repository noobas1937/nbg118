#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import genManifests

import os, os.path, errno
import sys
import shutil
import plistlib
import zipfile
try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET
from datetime import datetime

platform_iOS = 'iOS'
platform_Android = 'Android'

def getCurrentTime():
    currentSecond= datetime.now().second
    currentMinute = datetime.now().minute
    currentHour = datetime.now().hour

    currentDay = datetime.now().day
    currentMonth = datetime.now().month
    currentYear = datetime.now().year

    ret = "%4d%02d%02d%02d%02d%02d" % (currentYear, currentMonth, currentDay, currentHour, currentMinute, currentSecond)
    return ret

def deleteUselessFiles(src):
    for parent, dirnames, filenames in os.walk(src):
        for filename in filenames:
            if filename.find('.DS_Store') >= 0:
                os.remove(os.path.join(parent, filename))

def gen(url, dstFolderName, platform, AndroidManifest_xml):
    srcPath = os.path.split(os.path.realpath(__file__))[0] + '/../../IF/Resources/'

    appVer = '0.0.1'
    if platform_Android == platform:
        srcPath = os.path.split(os.path.realpath(__file__))[0] + '/../../proj.android/assets/'        
        tree = ET.parse(AndroidManifest_xml)
        root = tree.getroot()
        appVer = root.attrib['{http://schemas.android.com/apk/res/android}versionName']
    else:
        infoPlist = os.path.split(os.path.realpath(__file__))[0] + '/../../IF/Resources/Info.plist'
        pl = plistlib.readPlist(infoPlist)
        appVer = pl['CFBundleShortVersionString']

    dstPath = os.path.split(os.path.realpath(__file__))[0] + '/../../static/Resources_download_contents/' + platform + '_' + dstFolderName + '/' + appVer + '/'
    
    url = url + appVer

    # client manifest
    if os.path.exists(srcPath + 'manifest/'):
        shutil.rmtree(srcPath + 'manifest/')
    os.makedirs(srcPath + 'manifest/')
    # 
    if os.path.exists(dstPath):
        shutil.rmtree(dstPath)
    shutil.copytree(srcPath, dstPath)
    deleteUselessFiles(dstPath)
    shutil.rmtree(dstPath + 'manifest')

    # ------------------------------------------------------------------------------------
    def _export(assets_path, prefix):
        if len(prefix) > 0:
            prefix = prefix + '_'
            
        server_manifest = 'server_manifest/'

        clientMainManifest = prefix + 'main.manifest'
        serverMainManifest = prefix + 'main_server.manifest'
        versionMainManifest = prefix + 'version.manifest'

        genManifests.replaceMap["@PACKAGE_URL"] = url
        genManifests.replaceMap["@REMOTE_MANIFEST_URL"] = url + '/' + server_manifest + serverMainManifest
        genManifests.replaceMap["@REMOTE_VERSION_URL"] = url + '/' + server_manifest + versionMainManifest
        genManifests.replaceMap["@VERSION"] = appVer + '.' + getCurrentTime()
        genManifests.replaceMap["@ENGINE_VERSION"] = '3.7'
        genManifests.replaceMap["@ASSETS"] = ''

        if not os.path.exists(dstPath + server_manifest):
            os.makedirs(dstPath + server_manifest)
        genManifests.exportManifests(dstPath, assets_path, srcPath + 'manifest/', clientMainManifest, server_manifest + serverMainManifest, server_manifest + versionMainManifest)

    def _zip(zipname, fullpath, filenames):
        with zipfile.ZipFile(zipname, 'w') as myzip:
            for filename in filenames:
                myzip.write(fullpath + filename, filename, zipfile.ZIP_DEFLATED)
                os.remove(fullpath + filename)
            return zipname
    # ------------------------------------------------------------------------------------
    # local/xml
    _export(dstPath + 'local/xml', 'local_xml')
    # local/text
    text_res_path = dstPath + 'local/text'
    for parent, dirnames, filenames in os.walk(text_res_path):
        for filename in filenames:
            text = os.path.join(parent, filename)
            fn, ext = os.path.splitext(filename)
            _export(text, fn)
    # common
    dict_common_res = {}
    common_res_path = dstPath + 'Common/'
    for parent, dirnames, filenames in os.walk(common_res_path):
        for filename in filenames:
            fn, ext = os.path.splitext(filename)
            fn = fn.replace('.pvr', '')
            name = fn.replace('_alpha_', '')
            name = name.replace('_alpha', '')
            if not dict_common_res.has_key(name):
                dict_common_res[name] = []
            dict_common_res[name].append(filename)
    for (k, v) in dict_common_res.items():
        _zip(common_res_path + k + '.zip', common_res_path, v)
        _export(common_res_path + k + '.zip', k)

    # ------------------------------------------------------------------------------------
    pass

if __name__ == "__main__":
    # packageUrl = url + appVer
    url = sys.argv[1]
    dstFolderName = sys.argv[2]
    platform = sys.argv[3]
    AndroidManifest_xml = sys.argv[4]
    gen(url, dstFolderName, platform, AndroidManifest_xml)
