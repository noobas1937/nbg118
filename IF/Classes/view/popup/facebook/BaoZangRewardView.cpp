//
//  BaoZangRewardView.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-19.
//
//

#include "BaoZangRewardView.h"
#include "SocialController.h"
#include "PopupViewController.h"
#include "ToolController.h"

BaoZangRewardView* BaoZangRewardView::create(CCDictionary* info){
    BaoZangRewardView* ret = new BaoZangRewardView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BaoZangRewardView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTitle", CCLabelIF*, this->m_rewardTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_reward1", CCLabelIF*, this->m_reward1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_reward2", CCLabelIF*, this->m_reward2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_thankBtn", CCControlButton*, this->m_thankBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNode", CCNode*, this->m_goldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon2", CCNode*, this->m_icon2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickArea", CCNode*, this->m_clickArea);
    return true;
}

SEL_CCControlHandler BaoZangRewardView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onThankClick", BaoZangRewardView::onThankClick);
    return NULL;
}

void BaoZangRewardView::onThankClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    FBGetGlodMineCommand* cmd = new FBGetGlodMineCommand();
//    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(BaoZangRewardView::collectCallback), NULL));
//    cmd->sendAndRelease();
    this->closeSelf();
}


bool BaoZangRewardView::init(){
    bool ret = false;
    if(PopupBaseView::init())
    {
        setIsHDPanel(true);
        setCleanFunction([](){
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        });
        auto tmpCCB = CCBLoadFile("BaoZangRewardView", this, this);
        this->setContentSize(tmpCCB->getContentSize());
        CCCommonUtils::setButtonTitle(m_thankBtn, _lang("107060").c_str());
        m_rewardTitle->setString(_lang("107059"));
        m_goldNode->removeAllChildrenWithCleanup(true);
        m_icon2->removeAllChildrenWithCleanup(true);
        if(m_rewardInfo){
            if(m_rewardInfo->objectForKey("goods")){
                auto goods = _dict(m_rewardInfo->objectForKey("goods"));
                string itemId = goods->valueForKey("itemId")->getCString();
                int add = goods->valueForKey("add")->intValue();
                int count = goods->valueForKey("count")->intValue();
                string uuid = goods->valueForKey("uuid")->getCString();
                
                string itemName = CCCommonUtils::getNameById(itemId);
                itemName += "X ";
                itemName += CC_ITOA(add);
                m_reward2->setString(itemName);
                string icon = CCCommonUtils::getIcon(itemId);
                
                auto itemIcon = CCLoadSprite::createSprite(icon.c_str(),true,CCLoadSpriteType_GOODS);
                m_icon2->addChild(itemIcon);
                CCCommonUtils::setSpriteMaxSize(itemIcon, 100);
                ToolController::getInstance()->addTool(atoi(itemId.c_str()),count,uuid);
            }
            if(m_rewardInfo->objectForKey("gold")){
                auto gold = _dict(m_rewardInfo->objectForKey("gold"));
                string add = gold->valueForKey("add")->getCString();
                int multiple = gold->valueForKey("multiple")->intValue();
                auto spr = CCLoadSprite::createSprite(CCString::createWithFormat("scrifice_hint_%d.png",multiple)->getCString());
                if(spr && multiple>1){
                    spr->setScale(0.7);
                    m_goldNode->addChild(spr);
                }
                m_reward1->setString(add);
                if(gold->objectForKey("gold")){
                    int totalGold = gold->valueForKey("gold")->intValue();
                    GlobalData::shared()->playerInfo.gold = totalGold;
                }
            }
        }
        ret = true;
    }
    return ret;
    
}

bool BaoZangRewardView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void BaoZangRewardView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (!isTouchInside(m_clickArea,pTouch)) {
        PopupViewController::getInstance()->removePopupView(this);
    }else if(isTouchInside(m_clickArea,pTouch)){
        
    }
}

void BaoZangRewardView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
}

void BaoZangRewardView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}

void BaoZangRewardView::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
}