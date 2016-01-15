//
//  DragonBuildListView.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonBuildListView.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "CCCommonUtils.h"
#include "SceneController.h"
#include "ScienceView.h"
#include "YesNoDialog.h"
#include "UIComponent.h"

#include "ScienceListView.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "BuildingHospitalPopUpView.h"
#include "AllianceMarketView.h"
#include "BattleView.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "UseCDToolView.h"
#include "ProductionSoldiersView.h"
#include "GuideController.h"
#include "QuestController.h"
#include "DragonBuildingController.h"
#include "DragonScene.h"
#include "DragonBuildUpgradeView.h"

DragonBuildListView* DragonBuildListView::create(int pos){
    DragonBuildListView* ret = new DragonBuildListView();
    if(ret && ret->init(pos)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonBuildListView::init(int pos)
{
    if (!ArcPopupBaseView::init(TYPE_POS_UP)) {
        return false;
    }
    setIsHDPanel(true);
    auto size = CCDirector::sharedDirector()->getWinSize();
    setContentSize(size);
    
    CCBLoadFile("OptBuildView02",this,this);
    updateInfo(pos);
    m_openNum = 0;
    return true;
}

CCNode* DragonBuildListView::getGuideNode(string _key)
{
    if (_key=="build") {
        return m_upBtn;
    }
    else {
        if(m_initEnd) {
            gBuildId = atoi(_key.c_str());
            
            int idx = -1;
            for(int i=0; i<m_buildIds.size(); i++) {
                if (m_buildIds[i] == gBuildId) {
                    idx = i;
                    break;
                }
            }
            if (idx>0) {
                int curIdx = m_buildIds.size()-1;//m_arcScroll->m_currentIndex;
                bool up = false;
                if (idx > curIdx) {
                    up = true;
                }
                int moveCnt = abs(curIdx-idx);
                for (int i=0; i < moveCnt; i++) {
//                    m_arcScroll->moveOneCell(up);
                }
                refeash(idx);
//                return m_arcScroll->getShowAreaByPos(idx);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                       , CCString::createWithFormat("LI_%d",gBuildId));
            }
        }
    }
    return NULL;
}

void DragonBuildListView::updateInfo(int pos)
{
    int willBuild = GuideController::share()->getWillBuildItemId();
    m_pos = pos;
    if (1) {
        m_upBtnMsgLabel->setString(_lang("102277"));
        m_btnNode1->setVisible(true);
        m_btnNode1->setScale(0);
        CCActionInterval * scaleto = CCScaleTo ::create(0.3, 1);
        CCActionInterval * easeElasticIn= CCEaseElasticIn::create(scaleto);
        m_btnNode1->runAction(easeElasticIn);
    }
    
    int cellCnt = 0;
    vector<string> btnNames;
    vector<string> btnIcons;
    string tmpbuilds = DragonBuildingController::getInstance()->getBuildByPos(pos);
    tmpbuilds = DragonBuildingController::getInstance()->orderBuild(tmpbuilds);
    std::vector<std::string> buildIds;
    CCCommonUtils::splitString(tmpbuilds, ";", buildIds);
    for(int i=buildIds.size()-1;i>=0;i--)
    {
        bool _tb = true;
        int tmpItemId = atoi(buildIds[i].c_str());
        int limitNum = atoi(CCCommonUtils::getPropById(CC_ITOA(tmpItemId), "num").c_str());
        string name = CCCommonUtils::getNameById(CC_ITOA(tmpItemId));
        string pic = CCCommonUtils::getPropById(CC_ITOA(tmpItemId), "pic");
        pic = pic+"_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
        if (limitNum>0) {
            int curNum = DragonBuildingController::getInstance()->getBuildNumByType(tmpItemId);
            if (curNum >= limitNum) {
                _tb=false;
            }
        }
        
        if (_tb) {
            cellCnt++;
            btnNames.push_back(name);
            btnIcons.push_back(pic);
            m_buildIds.push_back(tmpItemId);
        }
    }
    
    int showPos = cellCnt-1;
    CCArray* arr = CCArray::create();
    for(int i=0;i<cellCnt;i++){
        
        auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_buildIds[i]));
        auto buildInfo = FunBuildInfo(dict);
        bool isLock = false;
        if (!buildInfo.isUnLock()) {
            isLock = true;
            
            string tmpBuildings = _lang("102130")+" ";
            int tmpIdx = 0;
            std::vector<std::string> lockItems;
            CCCommonUtils::splitString(buildInfo.building, "|", lockItems);
            for(int i=0; i<lockItems.size(); i++)
            {
                std::vector<std::string> tinyItems;
                CCCommonUtils::splitString(lockItems[i], ";", tinyItems);
                int tmpType = atoi(tinyItems[0].c_str());
                int tmLv = atoi(tinyItems[1].c_str());
                if(!DragonBuildingController::getInstance()->isExistBuildByTypeLv(tmpType, tmLv))
                {
                    if (tmpIdx>0) {
                        tmpBuildings +=",";
                    }
                    string tmpName = CCCommonUtils::getNameById(CC_ITOA(tmpType));
                    tmpBuildings += tmpName+" "+_lang_1("102272", CC_ITOA(tmLv));
                    tmpIdx++;
                }
            }
            m_buildLockInfos[i] = tmpBuildings;
        }
        else if (DragonBuildingController::getInstance()->bReachLimit(buildInfo.type)){
            isLock = true;
            string tmpBuildings = "main lv too low";
            m_buildLockInfos[i] = tmpBuildings;
        }
        else {
            m_buildLockInfos[i] = "";
        }
        
        if (willBuild>0 && willBuild==m_buildIds[i]) {
            showPos = i;
        }
        
        ArcInfo* info = new ArcInfo(i,btnNames[i],btnIcons[i], isLock);
        info->maxIconSize = 100;
        arr->addObject(info);
        CC_SAFE_RELEASE(info);
    }
    m_arcScroll = ArcScrollView::create(arr, 2, showPos);//arr->count()-1
    m_arcScroll->setCallback(this, callfunc_selector(DragonBuildListView::arcButtonClick));
    m_arcNode->addChild(m_arcScroll);
    
    refeash(showPos);
}

void DragonBuildListView::refeash(int idx)
{
    if (m_buildIds.size()<=0 || idx >= m_buildIds.size()) {
        return;
    }
    m_itemId = m_buildIds[idx];
    auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    layer->hideTmpBuild(m_pos);
    layer->showTmpBuild(m_pos, m_buildIds[idx]);
    
    string name = CCCommonUtils::getNameById(CC_ITOA(m_buildIds[idx]));
    string des = CCCommonUtils::getPropById(CC_ITOA(m_buildIds[idx]), "destip");
    des = _lang(des);
    m_nameLabel->setString(name);
    CCActionInterval *textScaleto = CCScaleTo ::create(0.3, 1);
    CCActionInterval *easeElasticIn= CCEaseElasticIn::create(textScaleto);
    m_titleLvNode->setScaleX(0);
    m_titleLvBG->runAction(CCFadeIn::create(0.5));
    m_nameLabel->runAction(CCFadeIn::create(0.5));
    m_titleLvNode->runAction(easeElasticIn);
    
    m_desLabel->setString(des);
    m_desLabel->runAction(CCFadeIn::create(0.5));
    
    float tmpY = m_desLabel->getPositionY()-m_desLabel->getContentSize().height*m_desLabel->getOriginScaleY();
    m_lockLabel->setPositionY(tmpY);
    m_lockLabel->setString(m_buildLockInfos[idx]);
    m_lockLabel->runAction(CCFadeIn::create(0.5));
    
    int limitNum = atoi(CCCommonUtils::getPropById(CC_ITOA(m_buildIds[idx]), "num").c_str());
    if (limitNum>1) {
        tmpY = tmpY-m_lockLabel->getContentSize().height*m_lockLabel->getOriginScaleY();
        m_numLabel->setPositionY(tmpY);
        int curNum = DragonBuildingController::getInstance()->getBuildNumByType(m_buildIds[idx]);
        m_numLabel->setString(_lang_2("102167", CC_ITOA(curNum), CC_ITOA(limitNum)));
        m_numLabel->runAction(CCFadeIn::create(0.5));
        
        if (curNum >= limitNum) {
            m_upBtn->setEnabled(false);
        }
        else {
            m_upBtn->setEnabled(true);
        }
    }
    else {
        m_numLabel->setString("");
        m_upBtn->setEnabled(true);
    }
}

void DragonBuildListView::arcButtonClick()
{
    refeash(m_arcScroll->m_currentIndex);
    if(gBuildId == m_itemId) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("LI_%d",m_itemId));
    }
}

SEL_CCControlHandler DragonBuildListView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", DragonBuildListView::onCreateOrUpClick);
    return NULL;
}

bool DragonBuildListView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockLabel", CCLabelIF*, this->m_lockLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLvNode", CCNode*, this->m_titleLvNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLvBG", CCScale9Sprite*, this->m_titleLvBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arcNode", CCNode*, this->m_arcNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode1", CCNode*, this->m_btnNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guidNode", CCNode*, this->m_guidNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    return false;
}

bool DragonBuildListView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_upNode, pTouch)) {
        m_upBtn->onTouchBegan(pTouch, pEvent);
    }
    return true;
}

void DragonBuildListView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    if (isTouchInside(m_upNode, pTouch) && m_upBtn->isEnabled()) {
        m_upBtn->onTouchEnded(pTouch, pEvent);
    }
    else if (m_arcScroll->m_isInTouch) {
    }
    else {
        onCloseView();
    }
}

void DragonBuildListView::onEnter(){
    CCNode::onEnter();
    if(m_openNum>0) {
        auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        layer->onMoveToBuildAndOpen(m_pos,TYPE_POS_UP);
    }
    m_openNum++;
    scheduleOnce(schedule_selector(DragonBuildListView::onInitEnd), 1.0f);
}

void DragonBuildListView::onExit(){
    m_initEnd = false;
    CCNode::onExit();
}

void DragonBuildListView::onInitEnd(float _time)
{
    m_initEnd = true;
    
    QuestInfo *info = QuestController::getInstance()->getRecommendQuest(false);
    if(info && (info->itemId=="2201001" || info->itemId=="2102601") && m_pos>16 && m_pos<52) {
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(DragonBuildListView::AnimationCallbackEnd));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Show");
    }
}

void DragonBuildListView::AnimationCallback()
{
    string aniName = this->getAnimationManager()->getLastCompletedSequenceName();
    if (aniName == "Show") {
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(DragonBuildListView::AnimationCallbackEnd));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Show");
    }
}

void DragonBuildListView::AnimationCallbackEnd() {
    
}

void DragonBuildListView::onCreateOrUpClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    if (!m_arcScroll->getAniState()) {
        CCLOG("fubin .....fubin .....fubin .....fubin .....fubin .....");
        return;
    }
    
    auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    layer->onMoveToBuildAndOpen(m_pos, TYPE_POS_UP);
    PopupViewController::getInstance()->addPopupInView(DragonBuildUpgradeView::create(m_itemId, m_pos));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("LI_build"));
}

void DragonBuildListView::onCloseView()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
    auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    layer->hideTmpBuild(m_pos);
    PopupViewController::getInstance()->removePopupView(this);
}