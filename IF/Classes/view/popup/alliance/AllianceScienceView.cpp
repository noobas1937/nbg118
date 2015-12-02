//
//  AllianceScienceView.cpp
//  IF
//
//  Created by fubin on 14-4-9.
//
//

#include "AllianceScienceView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "ToolInfo.h"
#include "ToolController.h"
#include "ArmyController.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "ScienceController.h"
#include "AllianceScienceDonateView.h"
#include "CCClipNode.h"
#include "ParticleController.h"
#include "UIComponent.h"
#include "UseResToolView.h"
#include "ScienceCommand.h"

#define  MSG_UPDATE_ALL_SC_DA       "msg_update_all_sc_da"
vector<int> comOpenVec;
vector<int> isLockVec;
AllianceScienceView* AllianceScienceView::create(){
    AllianceScienceView* ret = new AllianceScienceView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceScienceView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    
    CCLoadSprite::doResourceByCommonIndex(5, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(5, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    auto tbg = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -100;
    while (curHight<maxHight) {
        auto bg = CCLoadSprite::createSprite("technology_09.png");
        bg->setAnchorPoint(ccp(0, 1));
        bg->setScaleX(624*1.0/bg->getContentSize().width);
        if (CCCommonUtils::isIosAndroidPad())
        {
            bg->setScaleX(1536 * 1.0 / bg->getContentSize().width);
        }
        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
    }
    
    auto tmpCCB = CCBLoadFile("AllianceScienceView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
//    setTitleName(_lang("115159").c_str());
    m_titleTxt->setString(_lang("115159"));
    m_listNode->addChild(tBatchNode);
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_listNode->getContentSize().width;
    int oldHeight = m_listNode->getContentSize().height;
    m_listNode->setPositionY(m_listNode->getPositionY()-addHeight);
    m_listNode->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    m_tabView = CCTableView::create(this, m_listNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_listNode->addChild(m_tabView);
    
    m_contrNameLabel->setString(_lang("115800"));
    m_contrLabel->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.allianceInfo.accPoint));
    
    int totalPoint = ScienceController::getInstance()->getTotalAllScPoints();
    comOpenVec.clear();
    isLockVec.clear();
    int lockFloor = 0;
    map<int, vector<int>>::iterator it = ScienceController::getInstance()->aScienceFloorMap.begin();
    for (; it!=ScienceController::getInstance()->aScienceFloorMap.end(); it++) {
        
        auto& info = ScienceController::getInstance()->allianceScienceMap[(it->second)[0]];
        if (info.lockPoints <= totalPoint) {
            m_curList.push_back(it->first);
            comOpenVec.push_back(1);
            isLockVec.push_back(0);
            lockFloor = it->first;
        }
    }
    lockFloor += 1;
    if (ScienceController::getInstance()->aScienceFloorMap.find(lockFloor) != ScienceController::getInstance()->aScienceFloorMap.end()) {
        m_curList.push_back(lockFloor);
        comOpenVec.push_back(0);
        isLockVec.push_back(1);
    }
    
    if (GlobalData::shared()->scienceInfo.IsHasData == false) {
        ScienceDataInitCommand * cmd=new ScienceDataInitCommand();
        cmd->sendAndRelease();
    }
    
    updateInfo(NULL);
    
    return true;
}

void AllianceScienceView::updateInfo(CCObject* obj)
{
    m_tabView->reloadData();
}

void AllianceScienceView::onEnter()
{
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceScienceView::updatePoint), MSG_AL_SCIENCE_POINT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceScienceView::updateInfo), MSG_UPDATE_ALL_SC_DA, NULL);
//    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AllianceScienceView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void AllianceScienceView::onExit()
{
    comOpenVec.clear();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_AL_SCIENCE_POINT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UPDATE_ALL_SC_DA);
//    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(AllianceScienceView::onEnterFrame), this);
    CCNode::onExit();
}

void AllianceScienceView::updatePoint(CCObject* obj)
{
    m_contrLabel->setString(CC_CMDITOA(GlobalData::shared()->playerInfo.allianceInfo.accPoint));
}

void AllianceScienceView::onEnterFrame(float dt)
{
    
}

CCSize AllianceScienceView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_curList.size()){
        return CCSizeZero;
    }
    if (comOpenVec[idx]!=0) {
        int num = ScienceController::getInstance()->aScienceFloorMap[m_curList[idx]].size();
        if (CCCommonUtils::isIosAndroidPad())
        {
            return CCSize(1536, 160 + num * 310);
        }
        return CCSize(600, 90+num*175);
    }
    else {
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(1536, 160);
        }
        return CCSize(600, 90);
    }
}
CCSize AllianceScienceView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 160);
    }
    return CCSize(600, 90);
}
CCTableViewCell* AllianceScienceView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_curList.size()){
        return NULL;
    }
    
    AllScienceBigCell* cell = (AllScienceBigCell*)table->dequeueCell();
    if(idx < m_curList.size()){
        if(cell){
            cell->setData(m_curList[idx],comOpenVec[idx], isLockVec[idx]);
        }else{
            cell = AllScienceBigCell::create(m_curList[idx], comOpenVec[idx], isLockVec[idx]);
        }
    }
    return cell;
}
ssize_t AllianceScienceView::numberOfCellsInTableView(CCTableView *table)
{
    return m_curList.size();
}
void AllianceScienceView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void AllianceScienceView::onOkBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
}

void AllianceScienceView::onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45205");
}

SEL_CCControlHandler AllianceScienceView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkBtnClick", AllianceScienceView::onOkBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", AllianceScienceView::onTipBtnClick);
    return NULL;
}

bool AllianceScienceView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contrNameLabel", CCLabelIF*, this->m_contrNameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contrLabel", CCLabelIF*, this->m_contrLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    return false;
}

///////AllScienceBigCell

AllScienceBigCell* AllScienceBigCell::create(int itemId, int type, int isLock)
{
    auto ret = new AllScienceBigCell();
    if (ret && ret->init(itemId, type, isLock)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllScienceBigCell::init(int itemId, int type, int isLock)
{
    bool ret = true;
    CCBLoadFile("AllScienceTCell",this,this);
    m_floorLabel->setFntFile(getNBFont(NB_FONT_Bold_Border));
    setData(itemId, type, isLock);
    return ret;
}

void AllScienceBigCell::setData(int itemId, int type, int isLock)
{
    m_itemId = itemId;
    m_type = type;
    
    vector<int> sidVec = ScienceController::getInstance()->aScienceFloorMap[itemId];
    int sid = sidVec[0];
    auto& info = ScienceController::getInstance()->allianceScienceMap[sid];
    bool tmpLock = false;
    if (isLock==1) {
        tmpLock = true;
        int curPoint = ScienceController::getInstance()->getTotalAllScPoints();
        m_titleLabel->setString(_lang_2("115162", CC_ITOA(curPoint), CC_ITOA(info.lockPoints)));
    }
    else {
        m_titleLabel->setString("");
    }
    m_floorLabel->setString(CC_ITOA(m_itemId));
    
    m_listNode->removeAllChildren();
    int curY = 0;
    if (m_type==0) {
        m_jiantouSpr->setRotation(0);
    }
    else {
        m_jiantouSpr->setRotation(90);
        for (int i=0; i<sidVec.size(); i++) {
            auto cell = AllScienceCell::create(m_itemId, sidVec[i], tmpLock);
            cell->setPosition(ccp(0, curY));
            m_listNode->addChild(cell);
            if (CCCommonUtils::isIosAndroidPad())
            {
                curY -= 310;
            } else {
                curY -= 175;
            }
        }
    }
    m_mainNode->setPositionY(90+abs(curY));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_mainNode->setPositionY(160 + abs(curY));
    }
}

void AllScienceBigCell::onEnter() {
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCNode::onEnter();
}

void AllScienceBigCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AllScienceBigCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_myTouchNode", CCNode*, m_myTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, m_listNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jiantouSpr", CCSprite*, m_jiantouSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_floorLabel", CCLabelIF*, m_floorLabel);
    return false;
}

SEL_CCControlHandler AllScienceBigCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", ToolCell::onClickUseBtn);
    return NULL;
}

bool AllScienceBigCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_myTouchNode, pTouch)) {
        m_startY = pTouch->getLocation().y;
        return true;
    }
    return false;
}

void AllScienceBigCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int mv = pTouch->getLocation().y - m_startY;
    if (abs(mv)>20) {
        return;
    }
//    if (!m_canTouch) {
//        return;
//    }
    if (m_type==0) {
        m_type = 1;
        comOpenVec[m_itemId-1] = m_type;
        onClickOpen(90);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ALL_SC_DA);
    }
    else {
        m_type = 0;
        comOpenVec[m_itemId-1] = m_type;
        onClickOpen(0);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ALL_SC_DA);
    }
}

void AllScienceBigCell::onClickOpen(float rat)
{
    CCActionInterval * rotateto = CCRotateTo::create(0.2, rat);
    m_jiantouSpr->runAction(rotateto);
//    m_canTouch = false;
//    this->scheduleOnce(schedule_selector(AllScienceBigCell::onResetCanTouch), 0.2);
}

void AllScienceBigCell::onResetCanTouch()
{
    m_canTouch = true;
}

/////////////
AllScienceCell* AllScienceCell::create(int r_type, int itemId, bool isLock)
{
    auto ret = new AllScienceCell();
    if (ret && ret->init(r_type, itemId, isLock)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool AllScienceCell::init(int r_type, int itemId, bool isLock)
{
    bool ret = true;
    CCBLoadFile("AllScienceCell",this,this);
    
    m_type = r_type;
    m_itemId = itemId;
    m_bLock = isLock;
    
    auto& info = ScienceController::getInstance()->allianceScienceMap[m_itemId];
    m_nameLabel->setString(_lang(info.name));
    auto icon = CCLoadSprite::createSprite((info.icon+".png").c_str());
    CCCommonUtils::setSpriteMaxSize(icon, 130, true);
    if (isLock) {
        CCCommonUtils::setSpriteGray(icon, true);
    }
    m_iconNode->removeAllChildren();
    m_iconNode->addChild(icon);
    
    m_researshLabel->setString(_lang("115165"));
    m_masterLabel->setString("Master");
    m_masterLabel->setFntFile(getNBFont(NB_FONT_Gradient));
    m_bOpen  = false;
    
    m_starBar = CCStarBar::createStarBar(info.sumStars);
    m_starNode->addChild(m_starBar);
    
    m_yesTouchNode->setVisible(false);
    m_masterLabel->setVisible(false);
    
    m_clipNode = CCClipNode::create(CCSize(500,60));
    m_clipNode->setAnchorPoint(ccp(0,0));
    m_clipBarNode->addChild(m_clipNode);
    m_timeBarAnimNode = CCNode::create();
    m_timeBarNode->addChild(m_timeBarAnimNode);
    auto bar = CCLoadSprite::createSprite("Research_jingdutiao.png");
    bar->setAnchorPoint(ccp(0, 0));
    bar->setPosition(ccp(0, 0));
    m_clipNode->addChild(bar);
    auto bar1 = CCLoadSprite::createSprite("Research_jingdutiao.png");
    bar1->setFlipX(true);
    bar1->setAnchorPoint(ccp(0, 0));
    bar1->setPosition(ccp(272, 0));
    bar1->setScaleX(0.8);
    m_clipNode->addChild(bar1);
    
    onUpdateInfo();
    return ret;
}

void AllScienceCell::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllScienceCell::onRefreshData), MSG_AL_SCIENCE_PUSH, NULL);
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AllScienceCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}
void AllScienceCell::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_AL_SCIENCE_PUSH);
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(AllScienceCell::onEnterFrame), this);
    CCNode::onExit();
}
SEL_CCControlHandler AllScienceCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool AllScienceCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yesTouchNode", CCNode*, m_yesTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, m_starNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeBarNode", CCNode*, m_timeBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvNode", CCNode*, m_lvNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curLvLabel", CCLabelIF*, m_curLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLvLabel", CCLabelIF*, m_nextLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_masterLabel", CCLabelIF*, m_masterLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_researshLabel", CCLabelIF*, m_researshLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clipBarNode", CCNode*, m_clipBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeRotationBGSpr1", CCSprite*, m_timeRotationBGSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeRotationBGSpr2", CCSprite*, m_timeRotationBGSpr2);
    return false;
}

void AllScienceCell::onRefreshData(CCObject* obj)
{
    int sid = dynamic_cast<CCInteger*>(obj)->getValue();
    if (sid == m_itemId) {
        onUpdateInfo(true);
    }
}

void AllScienceCell::onUpdateInfo(bool isUpdate)
{
    auto& info = ScienceController::getInstance()->allianceScienceMap[m_itemId];
    string lvInfo = CC_ITOA(info.level);
    m_lvLabel->setString(_lang_1("102272", CC_ITOA(info.level)));//+"/"+CC_ITOA(info.maxLv)
    
    m_timeBarNode->setVisible(false);
    m_timeRotationBGSpr1->setVisible(false);
    m_timeRotationBGSpr2->setVisible(false);
    m_masterLabel->setVisible(false);
    
    m_starNode->setVisible(true);
    m_lvNode->setVisible(true);
    m_nameLabel->setVisible(true);
    
    if (info.level==info.maxLv) {//最大等级
        m_masterLabel->setVisible(true);
        m_starNode->setVisible(false);
        m_starBar->setProgress(info.curStar, info.currentPro,true);
    }
    else {
        if (info.finishTime > GlobalData::shared()->getWorldTime()) {
            //正在升级
            m_bOpen = true;
            m_curLvLabel->setString(_lang_1("102272", CC_ITOA(info.level)));
            m_nextLvLabel->setString(_lang_1("102272", CC_ITOA(info.level+1)));
            m_lvNode->setVisible(false);
            m_nameLabel->setVisible(false);
            m_starNode->setVisible(false);

           //

            string tmpStart = "ShowFire_";
            int maxP = 490/3;
            for (int i=1; i<=5; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,maxP);
                m_timeBarAnimNode->addChild(particle,100,100+i);
            }
            //onEnterFrame(0);

            if(isUpdate){
                playTouchSearchShowTimeBarAnim();
            }else{
                m_timeRotationBGSpr1->setVisible(true);
                m_timeRotationBGSpr2->setVisible(true);
                m_timeBarNode->setVisible(true);
                onEnterFrame(0);
            }
            m_starBar->setProgress(info.curStar, info.currentPro);
        }
        else {
            if (info.currentPro >= info.needPro && info.curStar == info.sumStars) {//捐满
                if (GlobalData::shared()->playerInfo.allianceInfo.rank >= 4) {//可以升级科技
                    m_yesTouchNode->setVisible(true);
                }
                m_starBar->setProgress(info.curStar, info.currentPro,true);
            }
            else {
                if (m_bLock) {//锁状态
                    int totalPoint = ScienceController::getInstance()->getTotalAllScPoints();
                    if (info.lockPoints <= totalPoint) {//解锁
                        m_bLock = false;
                        auto icon = CCLoadSprite::createSprite((info.icon+".png").c_str());
                        CCCommonUtils::setSpriteMaxSize(icon, 130, true);
                        m_iconNode->removeAllChildren();
                        m_iconNode->addChild(icon);
                    }
                }else{
                    m_starBar->setProgress(info.curStar, info.currentPro);
                }
            }
        }
    }
}

void AllScienceCell::onEnterFrame(float dt)
{
    if (m_bOpen) {
        AllianceScienceInfo* info = &(ScienceController::getInstance()->allianceScienceMap[m_itemId]);
        int curTime = GlobalData::shared()->getWorldTime();
        int lastTime = info->finishTime - curTime;
        int sumTime = info->finishTime-info->researchTime;
        if (lastTime>0) {
            m_timeLabel->setString(CC_SECTOA(lastTime));
            float pro = lastTime*1.0/sumTime;
            m_clipNode->setContentSize(CCSize(490*pro, 60));
            for (int i=1; i<=5; i++) {
                auto partanim = dynamic_cast<CCParticleSystemQuad*>(m_timeBarAnimNode->getChildByTag(100+i));

                if(partanim){
                    partanim->setPosVar(ccp(490*pro/2, 0));
                    partanim->setPosition(ccp(490*pro/2, -15));
                }
            }
        }
        else {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(AllScienceCell::onEnterFrame), this);
            m_bOpen = false;
            m_clipNode->setContentSize(CCSize(0, 60));
            m_timeBarAnimNode->removeAllChildren();
//            m_timeRotationBGSpr1->setVisible(false);
//            m_timeRotationBGSpr2->setVisible(false);
        }
    }
}

bool AllScienceCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_yesTouchNode, pTouch)) {
        m_yesTouchNode->setScale(1.2);
        m_startY = pTouch->getLocation().y;
        return true;
    }
    if (isTouchInside(m_touchNode, pTouch)) {
        m_startY = pTouch->getLocation().y;
        return true;
    }
    return false;
}

void AllScienceCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    int mv = pTouch->getLocation().y - m_startY;
    if (abs(mv)>20) {
        m_yesTouchNode->setScale(1.0);
        return;
    }
    
    if(m_bLock) {
        m_yesTouchNode->setScale(1.0);
        CCCommonUtils::flyHint("", "", _lang("115806"));
        return;
    }
    
    auto& info = ScienceController::getInstance()->allianceScienceMap[m_itemId];
    if(m_yesTouchNode->isVisible() && isTouchInside(m_yesTouchNode, pTouch)) {
        m_yesTouchNode->setScale(1.0);
        if (ScienceController::getInstance()->startResearchAllSc(m_itemId) ) {
            auto praticle1 = ParticleController::createParticle("UIGlow_1");
            m_yesTouchNode->addChild(praticle1,99,1000);
            auto praticle2 = ParticleController::createParticle("UIGlow_2");
            m_yesTouchNode->addChild(praticle2,99,1001);
            praticle1->setPosition(25, 25);
            praticle2->setPosition(25, 25);
            CCFiniteTimeAction* pActionOne  = CCScaleTo::create(0.2, 0.1);
            CCSequence* action = CCSequence::create(pActionOne,CCCallFunc::create(this, callfunc_selector(AllScienceCell::playTouchSearchAnim)),NULL);
            m_yesTouchNode->runAction(action);

        }
    }
    else {
        m_yesTouchNode->setScale(1.0);
        PopupViewController::getInstance()->addPopupView(AllianceScienceDonateView::create(m_itemId));
        if (info.lowDonateNum <= 0) {
            ScienceController::getInstance()->startDonateRefresh(m_itemId);
        }
    }
}
void AllScienceCell::playTouchSearchAnim(){
    m_yesTouchNode->setVisible(false);
    m_yesTouchNode->removeChildByTag(1000);
    m_yesTouchNode->removeChildByTag(1001);

}
void AllScienceCell::playTouchSearchShowTimeBarAnim(){
    m_timeRotationBGSpr2->setVisible(true);
    m_timeRotationBGSpr1->setVisible(true);
    m_timeRotationBGSpr1->setRotation(-90);
    
    CCRotateTo* iconRotate = CCRotateTo::create(0.3f, 0);
    
    this->m_timeRotationBGSpr1->runAction(CCSequence::create(iconRotate,NULL));
    m_timeBarNode->setVisible(true);
    CCActionInterval * fadeinAction = CCFadeIn::create(0.2);
    CCDelayTime* delay = CCDelayTime::create(0.3);
    this->m_timeBarNode->runAction(CCSequence::create(delay,fadeinAction,CCCallFunc::create(this, callfunc_selector(AllScienceCell::playTouchSearchShowTimeBarAnimFinish)),NULL));

}
void AllScienceCell::playTouchSearchShowTimeBarAnimFinish(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(AllScienceCell::onEnterFrame), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AllScienceCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}