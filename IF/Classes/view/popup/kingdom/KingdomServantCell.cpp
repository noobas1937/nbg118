//
//  KingdomServantCell.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-10.
//
//

#include "KingdomServantCell.h"
#include "ManagerKingdomView.h"
#include "SceneController.h"
#include "SoundController.h"

KingdomServantCell *KingdomServantCell::create(OfficeInfo* info,CCNode* showArea,bool manager){
    KingdomServantCell *ret = new KingdomServantCell(info,showArea,manager);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void KingdomServantCell::setData(OfficeInfo* info){
    m_info = info;
    this->refreshView();
}

void KingdomServantCell::refreshView(){
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
}

void KingdomServantCell::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(KingdomServantCell::refresh), REMOVE_APPOINT_OFFICER, NULL);
}

void KingdomServantCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, REMOVE_APPOINT_OFFICER);
    CCNode::onExit();
}

bool KingdomServantCell::init(){
    auto bg = CCBLoadFile("KingdomServantCell", this, this);
    this->setContentSize(bg->getContentSize());
    this->setData(m_info);
    return true;
}

bool KingdomServantCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPos = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch) && isTouchInside(m_showArea, pTouch)){
        m_cellBg->setColor(ccc3(128,128,128));
        return true;
    }
    return false;
}

void KingdomServantCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    m_cellBg->setColor(ccc3(255,255,255));
    if(isTouchInside(m_clickArea, pTouch) && isTouchInside(m_showArea, pTouch) && fabsf(end.x - m_startPos.x) <30 && fabsf(end.y - m_startPos.y) <30){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupView(KingdomRemoveAppointView::create(m_info,m_manager));
    }
}

void KingdomServantCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void KingdomServantCell::refresh(CCObject* param){
    CCInteger* obj = dynamic_cast<CCInteger*>(param);
    if(obj && obj->getValue()==m_info->id){
        this->setData(m_info);
    }
}

bool KingdomServantCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cellBg", CCSprite*, this->m_cellBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_officalsTxt", CCLabelIF*, this->m_officalsTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    return false;
}

SEL_CCControlHandler KingdomServantCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}