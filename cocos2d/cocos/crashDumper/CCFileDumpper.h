//
//  CCFileDumpper.h
//  DragonEmpire
//
//  Created by Chenfei on 13-11-29.
//
//

#ifndef __DragonEmpire__CCFileDumpper__
#define __DragonEmpire__CCFileDumpper__

#include "cocos2d.h"

NS_CC_BEGIN

class CCFileDumpper
{
public:
    CCFileDumpper(const char* fileName);
    virtual ~CCFileDumpper(){};
    int deleteFile();
    std::string read();
    bool isFileExist();
    bool write(std::string);
protected:
    std::string getFilePath();
    CCFileDumpper(){};
    CC_SYNTHESIZE_READONLY(const char*, mFileName, FileName);
};

NS_CC_END

#endif /* defined(__DragonEmpire__CCFileDumpper__) */
