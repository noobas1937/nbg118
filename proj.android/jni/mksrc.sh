#!/bin/bash

echo 'LOCAL_SRC_FILES := main.cpp \' > Sources.mk
#find ../../IF/libs/LibJson -name "*.cpp"|awk '{print $1,"\\"}' >> Sources.mk
find ../../IF/Classes -name "*.cpp"|awk '{print $1,"\\"}' >> Sources.mk
#find ../../cocos2d/cocos/crashDumper -name "*.cpp"|awk '{print $1,"\\"}' >> Sources.mk
find ../../IF/libs/rapidxml -name "*.cpp"|awk '{print $1,"\\"}' >> Sources.mk
find ../../IF/libs/extensions -name "*.cpp"|awk '{print $1,"\\"}' >> Sources.mk
echo >> Sources.mk

echo 'LOCAL_C_INCLUDES := \' >Includes.mk
#find ../../DragonEmpire/libs/LibJson -type d -not -wholename '*.svn*'|awk '{printf "$(LOCAL_PATH)/%s \\\n",$1}' >> Includes.mk
find ../../IF/Classes -type d -not -wholename '*.svn*'|awk '{printf "$(LOCAL_PATH)/%s \\\n",$1}' >> Includes.mk
#find ../../IF/libs/crashDumper -type d -not -wholename '*.svn*'|awk '{printf "$(LOCAL_PATH)/%s \\\n",$1}' >> Includes.mk
find ../../IF/libs/rapidxml -type d -not -wholename '*.svn*'|awk '{printf "$(LOCAL_PATH)/%s \\\n",$1}' >> Includes.mk
find ../../IF/libs/extensions -type d -not -wholename '*.svn*'|awk '{printf "$(LOCAL_PATH)/%s \\\n",$1}' >> Includes.mk
echo >> Includes.mk
