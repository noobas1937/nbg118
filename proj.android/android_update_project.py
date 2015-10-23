# -*- coding: UTF-8 -*-

import os, sys
import subprocess
import imp 
imp.reload(sys) 
sys.setdefaultencoding('utf-8') #设置默认编码,只能是utf-8

SCRIPT_PATH = os.path.split(os.path.realpath(__file__))[0]

list_dirs = os.walk(SCRIPT_PATH + '/../')
for root, dirs, files in list_dirs: 
    for f in files:
        if f == 'project.properties':
            if os.path.isfile(root + '/proguard-project.txt'):
                os.rename(root + '/proguard-project.txt', root + '/proguard-project_2.txt')
            print(subprocess.Popen("android update project --path %s" % root, shell=True, stdout=subprocess.PIPE).stdout.read())
            if os.path.isfile(root + '/proguard-project_2.txt'):
                os.remove(root + '/proguard-project.txt')
                os.rename(root + '/proguard-project_2.txt', root + '/proguard-project.txt')