#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import json
import os, os.path, errno
import sys
import shutil
import hashlib

main_manifest = '''{ 
    "packageUrl" : "@PACKAGE_URL",
    "remoteManifestUrl" : "@REMOTE_MANIFEST_URL", 
    "remoteVersionUrl" : "@REMOTE_VERSION_URL", 
    "version" : "@VERSION", 
    "engineVersion" : "@ENGINE_VERSION", 
    "assets" : {    
@ASSETS
    },
    "searchPaths" : [
    
    ] 
}'''

version_manifest = '''{ 
    "packageUrl" : "@PACKAGE_URL",
    "remoteManifestUrl" : "@REMOTE_MANIFEST_URL", 
    "remoteVersionUrl" : "@REMOTE_VERSION_URL", 
    "version" : "@VERSION", 
    "engineVersion" : "@ENGINE_VERSION"
}'''

replaceMap = {}
replaceMap["@PACKAGE_URL"] = ''
replaceMap["@REMOTE_MANIFEST_URL"] = ''
replaceMap["@REMOTE_VERSION_URL"] = ''
replaceMap["@VERSION"] = ''
replaceMap["@ENGINE_VERSION"] = ''
replaceMap["@ASSETS"] = ''

def getMD5(file_name):
    # Open,close, read file and calculate MD5 on its contents 
    with open(file_name) as file_to_check:
        # read contents of the file
        data = file_to_check.read()    
        # pipe contents of the file through
        md5_returned = hashlib.md5(data).hexdigest()

        return md5_returned

def getAssetsMD5(path):
    ret = ''
    for parent, dirnames, filenames in os.walk(path):
        for filename in filenames:
            fullpath = os.path.join(parent, filename)
            name = fullpath.replace(path, '')
            md5 = getMD5(fullpath)
            compressed = 'false'
            c = '''
        "%s" : {
            "md5" : "%s",
            "compressed" : %s
        },''' % (name, md5, compressed)
            ret = ret + c
            print('md5:(%s, %s)' % (md5, name))
    ret = ret + 'finalComma'
    ret = ret.replace(',finalComma', '')
    return ret


def saveFile(path, content):
    print(path)
    f = open(path, 'w')
    f.write(content)
    f.close()

def exportManifests(assetsPath, savePath, clientMainManifest, serverMainManifest, versionMainManifest):
    if os.path.exists(savePath):
        shutil.rmtree(savePath)
    os.makedirs(savePath)

    replaceMap["@ASSETS"] = getAssetsMD5(assetsPath)
    mm = main_manifest
    vm = version_manifest
    for (k, v) in replaceMap.items():
        mm = mm.replace(k, v)
        vm = vm.replace(k, v)
    saveFile(savePath + clientMainManifest, mm)
    saveFile(assetsPath + serverMainManifest, mm)
    saveFile(assetsPath + versionMainManifest, vm)
