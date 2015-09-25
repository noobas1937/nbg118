//
//  CCINIFileParser.h
//  DragonEmpire
//
//  Created by zhaohf on 13-8-6.
//
//

#include "cocos2d.h"
#include "CCINIParser.h"

USING_NS_CC;

CCINIParser::CCINIParser()
{
    
}

CCINIParser::~CCINIParser()
{
//    std::map<std::string, std::string>().swap(m_Lines);
}

CCINIParser* CCINIParser::parseWithFile(const char *piniFilePath,const char* localFileName)
{
    CCINIParser *pINIFileParser = new CCINIParser();
    if(!pINIFileParser->initWithFile(piniFilePath)) {
        if (!pINIFileParser->initWithFile(localFileName)) {
            CC_SAFE_RELEASE_NULL(pINIFileParser);
        }
    }
    return pINIFileParser;
}

//解析函数
bool CCINIParser::initWithFile(const char *piniFilePath)//传入ini文件路径以及文件名
{
    bool bRet = false;
//    m_tempNil = "";
    
    unsigned char* pBuffer = NULL;
    ssize_t bufferSize = 0;
    
    pBuffer = CCFileUtils::sharedFileUtils()->getFileData(piniFilePath, "r", &bufferSize);
    
    if (pBuffer)
    {
        int nLineLength = 0;
        std::string lineStr="";//一行数据
        for(unsigned long i = 0; i < bufferSize; i ++)//遍历读进来的所有字符
        {
            if(pBuffer[i] != '\r' && pBuffer[i] !='\n' && pBuffer[i] !='\0')//如果该字符不是回车、换行、结束符,则把这个字符加入本行str变量中
            {
                lineStr += pBuffer[i];
                nLineLength ++;
            }
            else//已经读了一行
            {
                if(nLineLength > 0)//读取了一行有字符的数据
                {
                    std::string lineStrBackup(lineStr.data(),lineStr.length());
                    StringReplace(lineStrBackup, " ", "");//将该行数据中的空格全部删除(自定义函数)
                    if(lineStrBackup.length() > 0)//如果剔除空格后该行字符串中仍有数据
                    {
                        ArString lineAr;
                        auto pos1 = lineStr.find_first_of("=");
                        if(pos1!=std::string::npos){
                            lineAr.push_back(lineStr.substr(0,pos1));
                            lineAr.push_back(lineStr.substr(pos1+1));
                        }
                        //SplitString(lineStr,"=",lineAr);//按'='号分割字符串(自定义函数)
                        if(lineAr.size() >= 2)//如果字符串能分割成两个串 则是有效行
                        {
                            m_Lines[lineAr[0]] = lineAr[1];    //存入map变量中
                        }
                    }
                    
                    nLineLength = 0;
                    lineStr = "";
                }
                else
                    continue;
            }
        }
        bRet = true;
    }
    
    delete  pBuffer;   // add liuzengyou
    pBuffer = NULL;
    
    return bRet;
}

//字符串替换函数
std::string& CCINIParser::StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();
    while( (pos = strBig.find(strsrc, pos)) != std::string::npos)
    {
        strBig.replace(pos, srclen, strdst);
        pos += dstlen;
    }
    return strBig;
}

//字符串拆分函数
void CCINIParser::SplitString(const std::string& strSrc, const std::string& strFind, ArString& arSplit)
{
    int st = 0;
    int npos = strSrc.find(strFind);
    if (npos == -1)
    {
        arSplit.push_back(strSrc);
        return;
    }
    
    while(npos > -1)
    {
        arSplit.push_back(strSrc.substr(st, npos - st));
        st = npos + strFind.size();
        npos = strSrc.find(strFind, npos + 1);
        
        if (npos <= -1)
        {
            arSplit.push_back(strSrc.substr(st, npos - st));
            break;
        }
    }
}
//获取文件内容
std::string CCINIParser::getObjectByKey(std::string strKey)
{
    std::string retVal = "";
    std::map<std::string,std::string>::iterator it = m_Lines.find(strKey);
    if(it != m_Lines.end())
        retVal = it->second;
    else
        return "";
    StringReplace(retVal, "\\n", "\n");
    return retVal;
}
std::string CCINIParser::getObjectByKey(std::string strKey,const char *paramCount,...)
{
    std::string retVal = "";
    std::map<std::string,std::string>::iterator it = m_Lines.find(strKey);
    if(it != m_Lines.end())
        retVal = it->second;
    else
        return "";
    va_list args;
    va_start(args, paramCount);
    char *i = va_arg(args, char*);
    int j = 0;
    int endVal = std::atoi(paramCount);
    char strsrc[10]={0};
    while(j<endVal && i){
        sprintf(strsrc,"{%d}",j);
        
        this->StringReplace(retVal, strsrc, i);
        i = va_arg(args, char*);
        ++j;
        
    }
    va_end(args);
    StringReplace(retVal, "\\n", "\n");
    return retVal;
}
