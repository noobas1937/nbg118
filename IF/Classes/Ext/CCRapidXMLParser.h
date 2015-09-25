//
//  CCRapidXMLParser.h
//  clash of kings
//
//  Created by zhaohf on 14-5-9.
//
//

#ifndef __COK__CCRapidXMLParser__
#define __COK__CCRapidXMLParser__

#include    "cocos2d.h"
#include    "ExtensionMacros.h"
#include    "rapidxml.hpp"

using namespace rapidxml;

USING_NS_CC;

class CCRapidXMLParser: public cocos2d::CCObject
{
public:
    static CCRapidXMLParser* parseWithFile(const char *xmlFileName,const char* localFileName);
    static CCRapidXMLParser* parseWithFileAndLocalDir(const char *xmlFileName,const char* localDir);
    
    CCRapidXMLParser();
    virtual ~CCRapidXMLParser();
    
    // 从本地xml文件读取
    bool initWithFile(const char *xmlFileName);
    bool initWithDir(const char *xmlDir);
    
    /**
     *对应xml标签开始,如：<string name="alex">, name为string，atts为string的属性，如["name","alex"]
     */
    virtual void startElement(xml_node<>* dictNode);
    
    //  获取某个关键字的属性数据字典
    CCDictionary * getObjectByKey(std::string key);
    
    //  根据分组关键字获取分组数据字典
    CCDictionary * getGroupByKey(std::string key);
    
    //  以id的值为key进行存储；
    CCDictionary * getDictItem() { return m_pDictItem; }
    //  分组存储，以Group id="Animal"中id值为key;
    CCDictionary *  getDictGroup() { return m_pDictGroup; }
    
private:
    std::string m_startXMLElement;
    std::string m_endXMLElement;
    std::string m_currString;//记录每个value的值
    std::string m_root_name;
    bool m_isJumpHeadData;
    
    //  <Animal id="10002" name="Chick"  show_scale="0.5" coin_cost="10"  speed_cash="2" base_cache_key="Chick" in_store="1"
    //  以id的值为key进行存储；
    CCDictionary *   m_pDictItem;
    //  分组存储，以Group id="Animal"中id值为key;
    CCDictionary *   m_pDictGroup;
    // 分组名称
    std::string m_groupName;

};


#endif /* defined(__COK__CCRapidXMLParser__) */


