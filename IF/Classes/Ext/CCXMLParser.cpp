//
//  CCXMLParser.cpp
//  DragonEmpire
//
//  Created by zhangjm on 13-7-22.
//
//

#include "cocos2d.h"
#include "CCXMLParser.h"

USING_NS_CC;

CCXMLParser::CCXMLParser()
:m_isJumpHeadData(false),m_pDictGroup(NULL),m_pDictItem(NULL)
{
    m_pDictItem = new CCDictionary();
    m_pDictGroup = new CCDictionary();
}

CCXMLParser::~CCXMLParser()
{
    CC_SAFE_RELEASE_NULL(this->m_pDictItem);
    CC_SAFE_RELEASE_NULL(this->m_pDictGroup);
}

CCXMLParser* CCXMLParser::parseWithFile(const char *xmlFileName)
{
    CCXMLParser *pXMLParser = new CCXMLParser();
    if(!pXMLParser->initWithFile(xmlFileName) ) {
        CC_SAFE_RELEASE_NULL(pXMLParser);
    }
    return pXMLParser;
}

bool CCXMLParser::initWithFile(const char *xmlFileName)
{
    SAXParser _parser;
    _parser.setDelegator(this);
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(xmlFileName);
    return _parser.parse(fullPath.c_str());
}

CCXMLParser* CCXMLParser::parseWithString(const char *content)
{
    CCXMLParser *pXMLParser = new CCXMLParser();
    if(!pXMLParser->initWithString(content)) {
        CC_SAFE_RELEASE_NULL(pXMLParser);
    }
    return pXMLParser;
}

bool CCXMLParser::initWithString(const char *content)
{
    SAXParser _parse;
    _parse.setDelegator(this);
    return _parse.parse(content, strlen(content) );
}


//  elementNode处理
void CCXMLParser::startElement(void *ctx, const char *name, const char **atts)
{
    CC_UNUSED_PARAM(ctx);
    
    m_startXMLElement = (char*)name;
    if(!m_isJumpHeadData){//跳过数据头Y
        CCLog("------跳过root name");
        m_isJumpHeadData=true;
        m_root_name=m_startXMLElement;
        return;
    }
    
    //  属性值处理
    std::string elementName = (char*)name;
    //  如果是分组节点，创建组数据字典；
    std::string strGroupID= "";
    if(strcmp(elementName.c_str(),"Group") == 0)
    {
        CCDictionary * pDictGroupItem = CCDictionary::create();
        std::string key = (char*)atts[0];
        std::string value = (char*)atts[1];
        m_groupName = value;
        m_pDictGroup->setObject(pDictGroupItem, value.c_str());
        return;
    }
    
    CCDictionary * pDictGroupItem = getGroupByKey(m_groupName);
    if(!pDictGroupItem)
    {
        pDictGroupItem = CCDictionary::create();
        m_pDictGroup->setObject(pDictGroupItem, m_groupName);
    }
    
    //  处理节点属性值；
    if(atts && atts[0])
    {
        CCDictionary * pDictTmp = CCDictionary::create();
        for(int i = 0; atts[i]; i += 2)
        {
            std::string key = (char*)atts[i];
            std::string value = (char*)atts[i+1];
            
            pDictTmp->setObject(CCString::create(value), key);
            
//            CCLog("-----key:%s, value:%s",key.c_str(),value.c_str());
        }
        //  按照id建立索引表
        CCString   * pID = (CCString   * )pDictTmp->objectForKey("id");
        m_pDictItem->setObject(pDictTmp , pID->_string);
        //  按照组id建立索引表
        pDictGroupItem->setObject(pDictTmp , pID->_string);
        
    }
    
//    CCLog("-startElement----%s",m_startXMLElement.c_str());
    
}

void CCXMLParser::endElement(void *ctx, const char *name)
{
    CC_UNUSED_PARAM(ctx);
    
    m_endXMLElement = (char*)name;
    if(m_endXMLElement==m_root_name)
    {//数据尾
        CCLog("读取xml结束");
        m_isJumpHeadData=false;
        m_root_name="";
        return;
    }
    
//    CCLog("-endElement----%s",m_endXMLElement.c_str());
}

//  TextNode处理
void CCXMLParser::textHandler(void *ctx, const char *s, int len)
{
    
    CC_UNUSED_PARAM(ctx);
    m_currString=std::string((char*)s,0,len);
 
    
    CCLog("-textHandler----%s",m_currString.c_str());
    
}


CCDictionary * CCXMLParser::getObjectByKey(std::string key)
{
    //  按照id建立索引表
    auto ret = (CCDictionary *)m_pDictItem->objectForKey(key);
    if (ret == NULL) {
        ret = CCDictionary::create();
    }
    return ret;
//    return (CCDictionary *) m_pDictItem->objectForKey(key);
}

CCDictionary * CCXMLParser::getGroupByKey(std::string key)
{
    //  按照GroupID建立索引表
    auto ret = (CCDictionary *)m_pDictGroup->objectForKey(key);
    if (ret == NULL) {
        ret = CCDictionary::create();
    }
    return ret;
//    return (CCDictionary *) m_pDictGroup->objectForKey(key);
}
