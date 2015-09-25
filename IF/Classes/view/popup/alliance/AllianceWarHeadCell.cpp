//
//  AllianceWarHeadCell.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-22.
//
//

#include "AllianceWarHeadCell.h"

AllianceWarHeadCell *AllianceWarHeadCell::create(std::string icon,int index,string uid,int picVer,int mType){
    AllianceWarHeadCell *ret = new AllianceWarHeadCell(icon,index,uid,picVer,mType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceWarHeadCell::init(){
    auto ccbNode = CCBLoadFile("AllianceWarHeadCell", this, this);
    this->setContentSize(ccbNode->getContentSize());
    m_iconNode->removeAllChildrenWithCleanup(true);
    CCSprite* head = CCLoadSprite::createSprite(m_icon.c_str());
    if (m_icon == "scws.png") {
         head  = CCLoadSprite::createSprite(m_icon.c_str(),true,CCLoadSpriteType_MONSTERLAYERLITTLE);
    }
    if (m_icon!="icon_hospital.png") {
        CCCommonUtils::setSpriteMaxSize(head, 61);
    }
    head->setScale(m_icon=="icon_hospital.png"?1:0.45);
    m_iconNode->addChild(head);
    if (CCCommonUtils::isUseCustomPic(m_picVer)) {
        m_headImgNode = HFHeadImgNode::create();
        m_headImgNode->initHeadImgUrl2(m_iconNode, CCCommonUtils::getCustomPicUrl(m_uid, m_picVer), 1.0f, 61, false);
    }
    
    if (m_mType>0 && m_icon=="tile_pop_icon21.png") {
        int color = 0;
        int tempType = m_mType;
        if(m_mType==3){
            color = 0;
            
        }else if (m_mType==2){
            tempType = 3;
            color = 1;
        }else{
            color = 3;
        }
        auto preBg = CCLoadSprite::createSprite(CCString::createWithFormat("Activty_icon_BG%d.png",tempType)->getCString());
        CCCommonUtils::setSpriteMaxSize(preBg, 63);
        m_iconNode->addChild(preBg);
        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 62,true);
        m_iconNode->addChild(iconBg,-100);
    }
    
    //    if(m_index==3){
    //        head->setOpacity(160);
    //        m_bg->setOpacity(160);
    //    }
    //    if(m_index==4){
    //        head->setOpacity(80);
    //        m_bg->setOpacity(80);
    //    }
    return true;
}

void AllianceWarHeadCell::onEnter(){
    CCNode::onEnter();
}

void AllianceWarHeadCell::onExit(){
    CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERBUST);
    CCNode::onExit();
}

SEL_CCControlHandler AllianceWarHeadCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

bool AllianceWarHeadCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCNode*, this->m_bg);
    return false;
}