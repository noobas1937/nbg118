# -*- coding: UTF-8 -*-

from xml.dom import minidom             
import os, sys, re, types, shutil                  
from datetime import datetime

import imp 
imp.reload(sys) 
sys.setdefaultencoding('utf-8') #设置默认编码,只能是utf-8

uselessCCBI = [
'xxx.ccbi',
]

exportForLua = [
]
