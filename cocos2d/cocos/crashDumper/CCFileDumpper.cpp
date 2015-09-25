//
//  CCFileDumpper.cpp
//  DragonEmpire
//
//  Created by Chenfei on 13-11-29.
//
//

#include "CCFileDumpper.h"

NS_CC_BEGIN

CCFileDumpper::CCFileDumpper(const char* fileName):mFileName(fileName)
{
    CC_ASSERT(mFileName && strlen(mFileName));
}


std::string CCFileDumpper::getFilePath()
{
    std::string writablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string crashLogFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(mFileName, writablePath.c_str());
    return crashLogFile;
}

int CCFileDumpper::deleteFile()
{
    std::string dumpFilePath = getFilePath();
    return remove(dumpFilePath.c_str());
}

bool CCFileDumpper::isFileExist()
{
    std::string dumpFilePath = getFilePath();
    return CCFileUtils::sharedFileUtils()->isFileExist(dumpFilePath);
}

std::string CCFileDumpper::read()
{
    std::string dumpFilePath = getFilePath();
    do
    {
        ssize_t size = -1;
        unsigned char * pData = CCFileUtils::sharedFileUtils()->getFileData(dumpFilePath.c_str(), "r", &size);
        if(pData && size > 0)
        {
            std::string forRet;
            forRet.append((char*)pData, size);
            CC_SAFE_DELETE_ARRAY(pData);
            CCLOG("CCFileDumpper::read(%s)", dumpFilePath.c_str());
            CCLOG("    Content:%s", forRet.c_str());
            return forRet;
        }
    }
    while (0);
    return "";
}

bool CCFileDumpper::write(std::string content)
{
    std::string dumpFilePath = getFilePath();
    do
    {
        FILE *fp = fopen(dumpFilePath.c_str(), "w");
        CC_BREAK_IF(!fp);
        CCLOG(" fp =%lX", (unsigned long)(fp));
        fwrite(content.c_str(), content.size(), sizeof(char), fp);
        fflush(fp);
        fclose(fp);
        CCLOG("CCFileDumpper::write(%s)", dumpFilePath.c_str());
        CCLOG("    Content:%s", content.c_str());
        return true;
    }
    while (0);
    return false;
}

NS_CC_END