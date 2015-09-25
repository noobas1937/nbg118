//
//  CityCrashView.cpp
//  IF
//
//  Created by 邹 程 on 14-3-12.
//
//

#include "CityCrashView.h"
#include "RebuildCommand.h"
#include "WorldController.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "PopupViewController.h"
#include "EnemyInfoController.h"

bool CityCrashView::init() {
//    if (!PopupBaseView::init()) {
//        return false;
//    }
    this->_ignoreAnchorPointForPosition = false;
    this->setAnchorPoint(Vec2(0,0));
    auto ccb = CCBLoadFile("CityCrashView",this,this);
    auto size = CCDirector::sharedDirector()->getWinSize();
//    this->setContentSize(ccb->getContentSize());
    this->setContentSize(size);
    m_info->setString(_lang("108679"));
    if(GlobalData::shared()->isCrossService){
        m_info->setString(_lang("138112"));
    }
    m_info->setDimensions(CCSize(580, 0));
    m_buttonRebuild->setTouchPriority(0);
    CCCommonUtils::setButtonTitle(m_buttonRebuild, _lang("108680").c_str());
    m_buttonRebuild->setTouchPriority(-2);
    return true;
}

bool CityCrashView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonRebuild", CCControlButton*, m_buttonRebuild)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info", CCLabelIF*, m_info)
    return false;
}

SEL_CCControlHandler CityCrashView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickButtonRebuild", CityCrashView::onClickButtonRebuild)
    return NULL;
}

void CityCrashView::onRebuildCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        m_buttonRebuild->setEnabled(true);
        m_buttonRebuild->setColor(ccWHITE);
        CCCommonUtils::flyText("Network error , please try again!");
        return;
    }
    
    std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
    int isFirstPop = CCUserDefault::sharedUserDefault()->getIntegerForKey(isFirstPopKey.c_str(),0);
    // reset city point
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    unsigned int index = info->valueForKey("point")->intValue();
    WorldController::getInstance()->selfPoint = WorldController::getPointByIndex(index);
    GlobalData::shared()->cityDefenseVal = info->valueForKey("cityDefValue")->intValue();
    GlobalData::shared()->cityLastUpdateDefenseStamp = info->valueForKey("lastCityDefTime")->doubleValue();
    GlobalData::shared()->cityTileCountry = SPACE;
    PopupViewController::getInstance()->removeAllPopupView();
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        if(GlobalData::shared()->isCrossService && GlobalData::shared()->playerInfo.currentServerId<CROSS_SERVER_START_INDEX){
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index, SERVERFIGHT_MAP);
        }else{
            WorldController::getInstance()->playAniTargetIndex = index;
            WorldMapView::instance()->gotoTilePoint(WorldController::getInstance()->selfPoint,true);
        }
    }else{
        if (isFirstPop==0) {
            WorldController::getInstance()->firstInWorld = true;
            isFirstPop = 1;
            CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), isFirstPop);
            CCUserDefault::sharedUserDefault()->flush();
        }
        WorldController::getInstance()->playAniTargetIndex = index;
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    removeAllChildrenWithCleanup(true);
    this->getParent()->removeChild(this);
    string temp = "X:";
    temp.append(CC_ITOA(WorldController::getInstance()->selfPoint.x));
    temp.append(" Y:");
    temp.append(CC_ITOA(WorldController::getInstance()->selfPoint.y));
    
    CCCommonUtils::flyHint("", "", _lang_1("108877", temp.c_str()));
    if(GlobalData::shared()->isCrossService){
        EnemyInfoController::getInstance()->clearAllEnemy();
    }
}

void CityCrashView::onClickButtonRebuild(CCObject * pSender, Control::EventType pCCControlEvent) {
    m_buttonRebuild->setEnabled(false);
    m_buttonRebuild->setColor(ccGRAY);
    auto cmd = new RebuildCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(CityCrashView::onRebuildCallback), NULL));
    cmd->sendAndRelease();
}

bool CityCrashView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void CityCrashView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void CityCrashView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}

void CityCrashView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}