//
//  GeneralHeadPic.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-7.
//
//

#include "GeneralHeadPic.h"


GeneralHeadPic *GeneralHeadPic::create(std::string generalId, int color,int type){
    GeneralHeadPic *ret = new GeneralHeadPic();
    if(ret && ret->init(generalId, color,type)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GeneralHeadPic::init(std::string generalId, int color,int type){
    if(type == R_GOODS){
        m_head = CCLoadSprite::createSprite(CCCommonUtils::getIcon(generalId).c_str(),true,CCLoadSpriteType_GOODS);
    }else if(type == R_GENERAL){
        m_head = CCLoadSprite::createSprite(CCCommonUtils::getIcon(generalId).c_str());
    }else{
        m_head = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(type).c_str());
    }
    
    this->setContentSize(m_head->getContentSize());
    std::string colorStr = CCCommonUtils::getPropById(generalId, "color");
    if(color != -1){
        colorStr = CC_ITOA(color);
    }
    m_bgColor = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
    m_cover = CCLoadSprite::createSprite("icon_kuang.png");
    m_bgColor->setScale((m_cover->getContentSize().width - 20) / m_bgColor->getContentSize().width);
    m_head->setScale((m_cover->getContentSize().width - 10) / m_head->getContentSize().width);
    this->addChild(m_cover);
    this->addChild(m_bgColor);
    this->addChild(m_head);

    return true;
}

void GeneralHeadPic::setGray(){
    m_head->setColor(ccGRAY);
    m_cover->setColor(ccGRAY);
}
