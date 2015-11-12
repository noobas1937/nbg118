#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import genManifests

import os, os.path, errno
import sys
import shutil
import plistlib

from datetime import datetime

def getCurrentTime():
   currentSecond= datetime.now().second
   currentMinute = datetime.now().minute
   currentHour = datetime.now().hour

   currentDay = datetime.now().day
   currentMonth = datetime.now().month
   currentYear = datetime.now().year

   ret = "%4d%02d%02d%02d%02d%02d" % (currentYear, currentMonth, currentDay, currentHour, currentMinute, currentSecond)
   return ret

def gen(url, dstFolderName):
    srcPath = os.path.split(os.path.realpath(__file__))[0] + '/../../IF/Resources/'
    dstPath = os.path.split(os.path.realpath(__file__))[0] + '/../../Resources_tmp/' + dstFolderName + '/'
    infoPlist = os.path.split(os.path.realpath(__file__))[0] + '/../../IF/Resources/Info.plist'

    # zipFiles.zipAllFolders(srcPath, dstPath)
    if os.path.exists(dstPath):
        shutil.rmtree(dstPath)
    # os.makedirs(dstPath)
    shutil.copytree(srcPath, dstPath)

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

    genManifests.exportManifests(dstPath, srcPath + 'manifest/', clientMainManifest, serverMainManifest, versionMainManifest)

if __name__ == "__main__":
    gen(sys.argv[1], sys.argv[2])
