//
//  CCXMLParser.h
//  DragonEmpire
//
//  Created by zhangjm on 13-7-22.
//
//

#ifndef __DragonEmpire__CCXMLParser__
#define __DragonEmpire__CCXMLParser__

#include    "cocos2d.h"
#include    "ExtensionMacros.h"
#include    "cocosextV3.h"
USING_NS_CC;

class CCXMLParser: public cocos2d::CCObject, public CCSAXDelegator
{
public:
    static CCXMLParser* parseWithFile(const char *xmlFileName);
    
    static CCXMLParser* parseWithString(const char *content);
    
    CCXMLParser();
    virtual ~CCXMLParser();
    
    // 从本地xml文件读取
    bool initWithFile(const char *xmlFileName);
    
    // 从字符中读取，可用于读取网络中的xml数据
    bool initWithString(const char *content);
    
    /**
     *对应xml标签开始,如：<string name="alex">, name为string，atts为string的属性，如["name","alex"]
     */
    virtual void startElement(void *ctx, const char *name, const char **atts);
    
    /**
     *对应xml标签结束,如：</string>
     */
    virtual void endElement(void *ctx, const char *name);
    
    /**
     *对应xml标签文本,如：<string name="alex">Alex Zhou</string>的Alex Zhou
     */
    virtual void textHandler(void *ctx, const char *s, int len);
    
    //  获取某个关键字的属性数据字典
    CCDictionary * getObjectByKey(std::string key);
    
    //  根据分组关键字获取分组数据字典
    CCDictionary * getGroupByKey(std::string key);
    
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


#endif /* defined(__DragonEmpire__CCXMLParser__) */


