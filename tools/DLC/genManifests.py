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

def getAssetsMD5(resourceRootPath, assetsPath):
    ret = ''
    if os.path.isdir(assetsPath): # assetsPath is dir
        for parent, dirnames, filenames in os.walk(assetsPath):
            for filename in filenames:
                fullpath = os.path.join(parent, filename)
                name = fullpath.replace(resourceRootPath, '')
                md5 = getMD5(fullpath)
                compressed = 'true'
                if name.find('.zip') < 0:
                    compressed = 'false'
                c = '''
            "%s" : {
                "md5" : "%s",
                "compressed" : %s
            },''' % (name, md5, compressed)
                ret = ret + c
                print('md5:(%s, %s)' % (md5, name))
    else: # assetsPath is single file
        name = assetsPath.replace(resourceRootPath, '')
        md5 = getMD5(assetsPath)
        compressed = 'true'
        if name.find('.zip') < 0:
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


def saveFile(file_full_path, content):
    print(file_full_path)
    f = open(file_full_path, 'w')
    f.write(content)
    f.close()

def exportManifests(resourceRootPath, assetsPath, clientMainManifest_savePath, clientMainManifest, serverMainManifest, versionMainManifest):
    replaceMap["@ASSETS"] = getAssetsMD5(resourceRootPath, assetsPath)
    mm = main_manifest
    vm = version_manifest
    for (k, v) in replaceMap.items():
        mm = mm.replace(k, v)
        vm = vm.replace(k, v)
    saveFile(clientMainManifest_savePath + clientMainManifest,  mm)
    saveFile(resourceRootPath + serverMainManifest,  mm)
    saveFile(resourceRootPath + versionMainManifest, vm)
