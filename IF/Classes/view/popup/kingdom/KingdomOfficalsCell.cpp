//
//  KingdomOfficalsCell.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-10.
//
//

#include "KingdomOfficalsCell.h"
#include "KingdomRemoveAppointView.h"
#include "ManagerKingdomView.h"
#include "SoundController.h"

KingdomOfficalsCell *KingdomOfficalsCell::create(OfficeInfo* info,CCNode* showArea,bool manager){
    KingdomOfficalsCell *ret = new KingdomOfficalsCell(info,showArea,manager);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void KingdomOfficalsCell::setData(OfficeInfo* info){
    m_info = info;
    this->refreshView();
}

void KingdomOfficalsCell::refreshView(){
    m_officalsTxt->setString(m_info->name);
    m_iconNode->removeAllChildrenWithCleanup(true);
    string iconStr = "";
    if(m_info->uid==""){
        m_nameTxt->setString(_lang("110077").c_str());
        iconStr = m_info->icon + ".png";
        
    }else{
        string temp = "";
        if(m_info->abbr!=""){
            temp.append("(");
            temp.append(m_info->abbr);
            temp.append(")");
        }
        temp.append(m_info->playerName);
        if (temp.length()>10) {
            temp = CCCommonUtils::subStrByUtf8(temp,0,10);
            temp.append("...");
        }
        m_nameTxt->setString(temp.c_str());
        iconStr = m_info->playerIcon + ".png";
    }
    auto pic = CCLoadSprite::createSprite(iconStr.c_str());
    m_iconNode->addChild(pic);
    CCCommonUtils::setSpriteMaxSize(pic, 75);
    if (CCCommonUtils::isUseCustomPic(m_info->picVer))
    {
        m_headImgNode->initHeadImgUrl2(m_iconNode, CCCommonUtils::getCustomPicUrl(m_info->uid, m_info->picVer), 1.0f, 70.0f, true);
    }
}

void KingdomOfficalsCell::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(KingdomOfficalsCell::refresh), REMOVE_APPOINT_OFFICER, NULL);
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void KingdomOfficalsCell::onExit(){
//    setTouchEnabled(false);
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, REMOVE_APPOINT_OFFICER);
    CCNode::onExit();
}

bool KingdomOfficalsCell::init(){
    auto bg = CCBLoadFile("KingdomOfficalsCell", this, this);
    this->setContentSize(bg->getContentSize());
    m_headImgNode = HFHeadImgNode::create();
    this->setData(m_info);
    return true;
}

bool KingdomOfficalsCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPos = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch) && isTouchInside(m_showArea, pTouch)){
        m_cellBg->setColor(ccc3(128,128,128));
        return true;
    }
    return false;
}

void KingdomOfficalsCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    m_cellBg->setColor(ccc3(255,255,255));
    if(isTouchInside(m_clickArea, pTouch) && isTouchInside(m_showArea, pTouch) && fabsf(end.x - m_startPos.x) <30 && fabsf(end.y - m_startPos.y) <30){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupView(KingdomRemoveAppointView::create(m_info,m_manager));
    }
}

void KingdomOfficalsCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void KingdomOfficalsCell::refresh(CCObject* param){
    CCInteger* obj = dynamic_cast<CCInteger*>(param);
    if(obj && obj->getValue()==m_info->id){
        this->setData(m_info);
    }
}

bool KingdomOfficalsCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cellBg", CCSprite*, this->m_cellBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_officalsTxt", CCLabelIF*, this->m_officalsTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    return false;
}

SEL_CCControlHandler KingdomOfficalsCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}