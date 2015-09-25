#!/usr/bin/env python
import os
import os.path
import zipfile
import sys
import hashlib

version = sys.argv[1]
fileVersion = sys.argv[3]
zipfolder = "dresource"
def CalcMD5(filepath):
	with open(filepath,'rb') as f:
		md5obj = hashlib.md5()
		md5obj.update(f.read())
		hash = md5obj.hexdigest()
		return hash

xmlcontent = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
xmlcontent = xmlcontent + "<tns:database xmlns:tns=\"http://www.iw.com/sns/platform/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n"


#curpath = os.path.join(os.getcwd(),os.path.pardir)
#print curpath
os.chdir(os.path.join(os.path.join(os.getcwd(),os.path.pardir),os.path.pardir))
rootpath = os.getcwd()
print rootpath

#need to change
outputpath =  os.path.join(os.path.join(os.path.join(rootpath,os.path.pardir) ,"locale"),"lua_res")
print outputpath
#or
#outputpath =   "/Users/EmilieJiang/Documents/workspace/COK/trunk/locale/lua_res"

if os.path.exists(outputpath) == True:
	print "Android_Scale_Resource exists"
else:
	os.mkdir(outputpath)
	print "Android_Scale_Resource create"

files = sys.argv[2]

intputpath = os.path.join(os.path.join(rootpath,"Android_Resource"),"Common")
intputpathios = os.path.join(os.path.join(os.path.join(rootpath,"IF"),"Resources"),"Common")
allfile = [4,5,6,7,8,11,100,101,102,104,105,200,201,202,203,204,205,206,207,208,209,305,308,310,500,501,502,503,504,505,506,508,509]
if files == '-1':
	zipFileList = [4,5,6,7,8,11,100,101,102,104,105,200,201,202,203,204,205,206,207,208,209,305,308,310,500,501,502,503,504,505,506,508,509]
else:
	zipFileList = files.split(',')
	print zipFileList

prefix1 = "_alpha_"
prefix2 = "Common_"
suffix1 = "_alpha"
extention1 = ".pkm"
extention2 = ".plist"
extention3 = ".pvr.ccz"

idx = 1
zipsize = 1024

for scalefile in allfile:
	outzip = os.path.join(outputpath,"dr_"+prefix2+str(scalefile)+"_android.zip")
	outzipios = os.path.join(outputpath,"dr_"+prefix2+str(scalefile)+"_ios.zip")
	inList = False
	for tmp in zipFileList:
		if int(tmp) == scalefile:
			inList=True
			break

	if inList == True:
		checkFile1 = os.path.join(intputpath,prefix1+prefix2+str(scalefile)+suffix1+extention1)
		checkFile2 = os.path.join(intputpath,prefix1+prefix2+str(scalefile)+extention1)
		checkFile3 = os.path.join(intputpath,prefix2+str(scalefile)+extention2)
		if os.path.isfile(checkFile2):
			if os.path.isfile(checkFile3):
				if os.path.isfile(checkFile1):
					zipf = zipfile.ZipFile(outzip, 'w',zipfile.ZIP_DEFLATED)
					zipf.write(checkFile1,os.path.join(zipfolder,os.path.basename(checkFile1)))
					zipf.write(checkFile2,os.path.join(zipfolder,os.path.basename(checkFile2)))
					zipf.write(checkFile3,os.path.join(zipfolder,os.path.basename(checkFile3)))
					zipf.close()
				else:
					print 'file is not exist',checkFile1
			else:
				print 'file is not exist',checkFile3
		else:	
			print 'file is not exist',checkFile2

		checkFile1ios = os.path.join(intputpathios,prefix2+str(scalefile)+extention2)
		checkFile2ios = os.path.join(intputpathios,prefix2+str(scalefile)+extention3)
		if os.path.isfile(checkFile1ios):
			if os.path.isfile(checkFile2ios):
				zipfios = zipfile.ZipFile(outzipios, 'w',zipfile.ZIP_DEFLATED)
				zipfios.write(checkFile1ios,os.path.join(zipfolder,os.path.basename(checkFile1ios)))
				zipfios.write(checkFile2ios,os.path.join(zipfolder,os.path.basename(checkFile2ios)))
				zipfios.close()
			else:
				print 'file is not exist',checkFile2ios
		else:	
			print 'file is not exist',checkFile1ios

	zipmd5 = ""	
	zipmd5ios = ""
	if os.path.isfile(outzip):
		zipmd5 = CalcMD5(outzip)
	else:
		print 'file is not exist',outzip
	if os.path.isfile(outzipios):
		zipmd5ios = CalcMD5(outzipios)
	else:
		print 'file is not exist',outzipios

	specxml = "\t<ItemSpec id=\"" + str(idx) + "\" name=\"" + prefix2 + str(scalefile) + "\" checksize=\"" + str(zipsize) +"\" iosmd5=\"" + zipmd5ios +"\" androidmd5=\"" + zipmd5 + "\" version=\"" + version +"\" />" 
	xmlcontent = xmlcontent + specxml + "\n"
	idx = idx+1
	
		

sZip1 = os.path.join(outputpath,"dr_bgsound_android.zip")
zipmd51 = CalcMD5(sZip1)
sZip2 = os.path.join(outputpath,"dr_bgsound_ios.zip")
zipmd52 = CalcMD5(sZip2)
specxml = "\t<ItemSpec id=\"" + str(idx) + "\" name=\"" + "bgsound" + "\" checksize=\"" + str(zipsize) +"\" iosmd5=\""+ zipmd52 + "\" androidmd5=\"" + zipmd51 + "\" version=\"" + "1.0.1" +"\" />" 
xmlcontent = xmlcontent + specxml + "\n"

idx = idx+1

sZip3 = os.path.join(outputpath,"dr_effsound_android.zip")
zipmd53 = CalcMD5(sZip3)
sZip4 = os.path.join(outputpath,"dr_effsound_ios.zip")
zipmd54 = CalcMD5(sZip4)
specxml = "\t<ItemSpec id=\"" + str(idx) + "\" name=\"" + "effsound" + "\" checksize=\"" + str(zipsize) +"\" iosmd5=\"" + zipmd54 + "\" androidmd5=\"" + zipmd53 + "\" version=\"" + "1.0.1" +"\" />" 
xmlcontent = xmlcontent + specxml + "\n"


xmlcontent = xmlcontent + "</tns:database>"
savefile = open(os.path.join(outputpath,"drconfig" + fileVersion + ".xml"),'wb')
savefile.write(xmlcontent)
savefile.close()

print 'the end'







