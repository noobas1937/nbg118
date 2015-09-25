//
//  CCINIParser.h
//  IF
//
//  Created by zhaohf on 13-8-30.
//
//

#ifndef __IF__CCINIParser__
#define __IF__CCINIParser__

#include    "cocos2d.h"

class CCINIParser: public cocos2d::CCObject
{
public:
    static CCINIParser* parseWithFile(const char *iniFileName,const char* localFileName);
    
    CCINIParser();
    virtual ~CCINIParser();
    
    // 从本地ini文件读取
    bool initWithFile(const char *piniFilePath);
    
    //  获取某个关键字的属性数据字典
    std::string getObjectByKey(std::string strKey);
    std::string getObjectByKey(std::string strKey,const char *paramCount,...);
private:
    typedef std::vector<std::string> ArString;
   
    //key是等号左边的值  value是等号右边的值
    std::map<std::string,std::string> m_Lines;//用来记录读进内存的数据
    
    void SplitString(const std::string& strSrc, const std::string& strFind, ArString& arSplit);
    
    std::string& StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst);
};


#endif /* defined(__IF__CCINIParser__) */


