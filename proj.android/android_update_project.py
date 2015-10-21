# -*- coding: UTF-8 -*-

import os, sys
import subprocess
import imp 
imp.reload(sys) 
sys.setdefaultencoding('utf-8') #设置默认编码,只能是utf-8

SCRIPT_PATH = os.path.split(os.path.realpath(__file__))[0]

list_dirs = os.walk(SCRIPT_PATH + '/118/')
for root, dirs, files in list_dirs: 
    for f in files:
        if f == 'project.properties':
            print(root)
            print(subprocess.Popen("android update project --path %s" % root, shell=True, stdout=subprocess.PIPE).stdout.read())