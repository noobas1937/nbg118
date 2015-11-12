#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import genManifests

import os, os.path, errno
import sys
import shutil
import plistlib
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

def gen(url, dstFolderName, platform):
    srcPath = os.path.split(os.path.realpath(__file__))[0] + '/../../IF/Resources/'
    dstPath = os.path.split(os.path.realpath(__file__))[0] + '/../../Resources_download_contents/' + platform + '_' + dstFolderName + '/'

    appVer = '0.0.1'
    if platform_Android == platform:
        srcPath = os.path.split(os.path.realpath(__file__))[0] + '/../../proj.android/assets/'
        xml = os.path.split(os.path.realpath(__file__))[0] + '/../../Publish.android/IF_Global/AndroidManifest.xml'        
        tree = ET.parse(xml)
        root = tree.getroot()
        appVer = root.attrib['{http://schemas.android.com/apk/res/android}versionName']
    else:
        infoPlist = os.path.split(os.path.realpath(__file__))[0] + '/../../IF/Resources/Info.plist'
        pl = plistlib.readPlist(infoPlist)
        appVer = pl['CFBundleShortVersionString']
    
    url = url + appVer
    clientMainManifest = 'main.manifest'
    serverMainManifest = 'main_server.manifest'
    versionMainManifest = 'version.manifest'

    genManifests.replaceMap["@PACKAGE_URL"] = url
    genManifests.replaceMap["@REMOTE_MANIFEST_URL"] = url + '/' + serverMainManifest
    genManifests.replaceMap["@REMOTE_VERSION_URL"] = url + '/' + versionMainManifest
    genManifests.replaceMap["@VERSION"] = appVer + '.' + getCurrentTime()
    genManifests.replaceMap["@ENGINE_VERSION"] = '3.7'
    genManifests.replaceMap["@ASSETS"] = ''

    if os.path.exists(dstPath):
        shutil.rmtree(dstPath)
    shutil.copytree(srcPath, dstPath)
    genManifests.exportManifests(dstPath, srcPath + 'manifest/', clientMainManifest, serverMainManifest, versionMainManifest)

if __name__ == "__main__":
    # packageUrl = url + appVer
    url = sys.argv[1]
    dstFolderName = sys.argv[2]
    platform = sys.argv[3]
    gen(url, dstFolderName, platform)
