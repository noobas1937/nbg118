//
//  AllianceDonateRankView.cpp
//  IF
//
//  Created by lifangkai on 14-9-3.
//
//

#include "AllianceDonateRankView.h"

//
//  StoreView.cpp
//  IF
//
//  Created by fubin on 14-2-27.
//
//

#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "UseCDToolView.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "ChangeNickNameView.h"
#include "WorldMapView.h"
#include "ChangePicView.h"
#include "AllianceDonateRankCommand.h"
#include "PlayerInfoBtnPopUpView.h"
#include "RoleInfoView.h"
#include "SoundController.h"

const float numPerRow = 1.0;
AllianceDonateRankView* AllianceDonateRankView::create(int type){
    AllianceDonateRankView* ret = new AllianceDonateRankView(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool AllianceDonateRankView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    auto tmpCCB = CCBLoadFile("AllianceDonateRank",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    
    m_data = CCArray::create();
    m_isgetAllCommand = false;
    m_isgetWeekCommand = false;
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    if (CCCommonUtils::isIosAndroidPad())
    {
        addHeight = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
//    if (!CCCommonUtils::isIosAndroidPad())
//    {
//        int BGcount = (newBgHeight-80)/100+1-1;
//        for (int i=0; i<BGcount; i++) {
//            auto pic = CCLoadSprite::createSprite("technology_09.png");
//            m_totalNode->addChild(pic);
//            pic->setPositionY(203-(i+1)*100);
//        }
//    } else
//    {
//        float curY = 0;
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        while (curY > -2048)
//        {
//            auto pic = CCLoadSprite::createSprite("technology_09.png");
//            pic->setAnchorPoint(ccp(0, 1));
//            pic->setPosition(ccp(0, curY));
//            tBatchNode->addChild(pic);
//            curY -= pic->getContentSize().height;
//        }
//        tBatchNode->setScaleX(1536.0 / 640.0);
//        m_totalNode->addChild(tBatchNode);
//    }
    
    
    CCCommonUtils::setButtonTitle(m_weekBtn, _lang("115300").c_str());
    CCCommonUtils::setButtonTitle(m_todayBtn, _lang("115812").c_str());
    CCCommonUtils::setButtonTitle(m_historyBtn, _lang("115813").c_str());
    m_timeTitleText->setString(_lang_1("115824",""));
    m_textTitle1->setString(_lang("115825"));
    m_textTitle2->setString(_lang("115811"));
    m_textTitle3->setString(_lang("115810"));
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    

    //    m_packView->setVisible(false);
    int count = 0;
    if(count>0){
        m_type = 1;
    }else{
        m_type = 0;
    }
    m_type = 0;
    getTodayRankData();
 //   updateInfo();
    
    return true;
}
void AllianceDonateRankView::onTimer(float dt){
    if (m_type == 0) {
        int dtime = m_time - GlobalData::shared()->getWorldTime();
        if(dtime<0){
            this->unschedule(schedule_selector(AllianceDonateRankView::onTimer));
            getTodayRankData();
            
        }else{
            string timestr = getTimeStr(dtime);
            m_timeLeftText->setString(timestr.c_str());
        }
    } else if (m_type == 2) {
        int dtime = m_time - GlobalData::shared()->getWorldTime();
        if (dtime < 0) {
            this->unschedule(schedule_selector(AllianceDonateRankView::onTimer));
            getWeekRandData();
        } else {
            string timestr = getTimeStr(dtime);
            m_timeLeftText->setString(timestr.c_str());
        }
    }
}
string AllianceDonateRankView::getTimeStr(int secs){
    int day = secs/(24*3600);
    secs = secs%(24*3600);
    int hour = secs/3600;
    secs = secs%(3600);
    int min = secs/60;
    secs = secs%(60);
    int sec = secs;
    std::string dayStr = _lang("105592");
    std::string hourStr = _lang("105591");
    std::string minStr = _lang("105590");
    string timeStr = "";
    if(day > 0){
        timeStr = CC_ITOA(day)+_lang("103005")+" ";
    }
    if(hour<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(hour));
    }else{
        timeStr.append(CC_ITOA(hour));
    }
    timeStr.append(":");
    if(min<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(min));
    }else{
        timeStr.append(CC_ITOA(min));
    }
    timeStr.append(":");
    if(sec<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(sec));
    }else{
        timeStr.append(CC_ITOA(sec));
    }
    
    return timeStr;
}
void AllianceDonateRankView::getAllRankData(){
    GameController::getInstance()->showWaitInterface();
    AllianceDonateAllCommand* cmd = new AllianceDonateAllCommand();
    cmd->sendAndRelease();
}
void AllianceDonateRankView::getTodayRankData(){
    GameController::getInstance()->showWaitInterface();
    AllianceDonateTodayCommand* cmd = new AllianceDonateTodayCommand();
    cmd->sendAndRelease();
}
void AllianceDonateRankView::getWeekRandData() {
    GameController::getInstance()->showWaitInterface();
    AllianceDonateWeekCommand* cmd = new AllianceDonateWeekCommand();
    cmd->sendAndRelease();
}
void AllianceDonateRankView::updateInfo()
{
    
    if (m_type==0) {
        this->m_timeNode->setVisible(true);
        m_historyBtn->setEnabled(true);
        m_todayBtn->setEnabled(false);
        m_weekBtn->setEnabled(true);
        m_allianceDonateList = GlobalData::shared()->allianceDonateTodayList;
    }
    else if (m_type == 1) {
        this->m_timeNode->setVisible(false);
        m_historyBtn->setEnabled(false);
        m_todayBtn->setEnabled(true);
        m_weekBtn->setEnabled(true);
        if(!m_isgetAllCommand){
            m_isgetAllCommand = true;
            getAllRankData();
            return;
        }
        m_allianceDonateList = GlobalData::shared()->allianceDonateAllList;
    }
    else if (m_type == 2)
    {
        m_timeNode->setVisible(true);
        m_todayBtn->setEnabled(true);
        m_weekBtn->setEnabled(false);
        m_historyBtn->setEnabled(true);
        if (!m_isgetWeekCommand) {
            m_isgetWeekCommand = true;
            getWeekRandData();
        }
        m_allianceDonateList = GlobalData::shared()->allianceDonateWeekList;
    }
    m_data->removeAllObjects();

    vector<AllianceDonateInfo*>::iterator it;
    for(it = m_allianceDonateList.begin(); it != m_allianceDonateList.end(); it++){
        m_data->addObject(*it);
    }
    
    if(m_type==0){
        AllianceDonateInfo* info = dynamic_cast<AllianceDonateInfo*>(m_data->objectAtIndex(0));
        m_time = info->refreshTime;
        this->unschedule(schedule_selector(AllianceDonateRankView::onTimer));
        onTimer(0);
        this->schedule(schedule_selector(AllianceDonateRankView::onTimer),1.0);
    }
    else if (m_type == 2) {
        if (m_data->count()) {
            AllianceDonateInfo* info = dynamic_cast<AllianceDonateInfo*>(m_data->objectAtIndex(0));
            m_time = info->refreshTime;
            this->unschedule(schedule_selector(AllianceDonateRankView::onTimer));
            onTimer(0);
            this->schedule(schedule_selector(AllianceDonateRankView::onTimer), 1.0);
        }
    }
    
    m_tabView->reloadData();
}
void AllianceDonateRankView::refreshDataByUid(CCObject* data){
    CCString* uid = dynamic_cast<CCString*>(data);
    vector<AllianceDonateInfo*>::iterator it;
    for (it = GlobalData::shared()->allianceDonateAllList.begin(); it != GlobalData::shared()->allianceDonateAllList.end(); it++) {
        AllianceDonateInfo* tmp = *it;
        if(tmp->uid==uid->getCString()){
            CC_SAFE_RELEASE(tmp);
            GlobalData::shared()->allianceDonateAllList.erase(it);
            break;
        }
        
    }
    for (it = GlobalData::shared()->allianceDonateTodayList.begin(); it != GlobalData::shared()->allianceDonateTodayList.end(); it++) {
        AllianceDonateInfo* tmp1 = *it;
        if(tmp1->uid==uid->getCString()){
            CC_SAFE_RELEASE(tmp1);
            GlobalData::shared()->allianceDonateTodayList.erase(it);
            refreashData(NULL);
            break;
        }
        
    }
}
void AllianceDonateRankView::refreashData(CCObject* obj)
{
    updateInfo();
}
bool AllianceDonateRankView::sortByUse(int itemId1, int itemId2){
    auto& info1 = ToolController::getInstance()->getToolInfoById(itemId1);
    auto& info2 = ToolController::getInstance()->getToolInfoById(itemId2);
    return (info1.use>info2.use);
    
}
void AllianceDonateRankView::sortData(){
//    if (m_curList.size()>0) {
//        std::sort(m_curList.begin(),m_curList.end(),sortByUse);
//    }
}
CCSize AllianceDonateRankView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_allianceDonateList.size()){
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1500, 200);
    }

    return CCSize(620, 100);

}
CCSize AllianceDonateRankView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1500, 200);
    }
    return CCSize(620, 100);
}
CCTableViewCell* AllianceDonateRankView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    AllianceDonateRankCell* cell = (AllianceDonateRankCell*)table->dequeueCell();
    if(idx < m_data->count()){
        AllianceDonateInfo* info = dynamic_cast<AllianceDonateInfo*>(m_data->objectAtIndex(idx));
        if(cell){
            cell->setData(info,idx);
        }else{
            cell = AllianceDonateRankCell::create(info,idx);
        }
    }
    return cell;
}
ssize_t AllianceDonateRankView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / numPerRow);
    return num;
}
void AllianceDonateRankView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void AllianceDonateRankView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDonateRankView::refreashData), ALLIANCE_DONATE_RANK_ALL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDonateRankView::refreashData), ALLIANCE_DONATE_RANK_TODAY, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDonateRankView::refreashData), ALLIANCE_DONATE_RANK_WEEK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDonateRankView::refreshDataByUid), MSG_ALLIANCE_DATA_CHANGE, NULL);
    setTitleName(_lang("115823").c_str());
}

void AllianceDonateRankView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_DONATE_RANK_ALL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_DONATE_RANK_TODAY);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_DONATE_RANK_WEEK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_DATA_CHANGE);
    CCNode::onExit();
}

AllianceDonateRankView::~AllianceDonateRankView()
{
    //    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_REMOVE,CCString::create(CC_ITOA(PVT_store)));
}

SEL_CCControlHandler AllianceDonateRankView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHistoryBtn", AllianceDonateRankView::onClickHistoryBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTodayBtn", AllianceDonateRankView::onClickTodayBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickWeekBtn", AllianceDonateRankView::onClickWeekBtn);
    return NULL;
}

bool AllianceDonateRankView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_historyBtn", CCControlButton*, this->m_historyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_todayBtn", CCControlButton*, this->m_todayBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weekBtn", CCControlButton*, this->m_weekBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTitleText", CCLabelIF*, this->m_timeTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLeftText", CCLabelIF*, this->m_timeLeftText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textTitle1", CCLabelIF*, this->m_textTitle1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textTitle2", CCLabelIF*, this->m_textTitle2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textTitle3", CCLabelIF*, this->m_textTitle3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", CCNode*, this->m_timeNode);
    return false;
}


void AllianceDonateRankView::onClickTodayBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type != 0) {
        m_type = 0;
        updateInfo();
    }
}

void AllianceDonateRankView::onClickHistoryBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type != 1) {
        m_type = 1;
        updateInfo();
    }
}

void AllianceDonateRankView::onClickWeekBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type != 2) {
        m_type = 2;
        updateInfo();
    }
}

AllianceDonateRankCell* AllianceDonateRankCell::create(AllianceDonateInfo* info,int index)
{
    auto ret = new AllianceDonateRankCell(info,index);
    if (ret && ret->init(info,index)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceDonateRankCell::init(AllianceDonateInfo* info,int index)
{
    bool ret = true;
    CCBLoadFile("AllianceDonateRankCell",this,this);
    m_headImgNode = HFHeadImgNode::create();
    setData(info,index);
    return ret;
}

void AllianceDonateRankCell::setData(AllianceDonateInfo* info,int index)
{
    m_info = info;
    m_index = index;
    m_headNode->removeAllChildren();
    CCSprite* pic;
    if(m_info->pic==""){
        pic = CCLoadSprite::createSprite("guidePlayerIcon.png");
        pic->setScale(0.7);
    }else{
            string mpic = m_info->pic+".png";
            pic = CCLoadSprite::createSprite(mpic.c_str());
        
    
    }
    CCCommonUtils::setSpriteMaxSize(pic, 60);
    m_headNode->addChild(pic);
    if (CCCommonUtils::isUseCustomPic(info->picVer))
    {
        m_headImgNode->initHeadImgUrl2(m_headNode, CCCommonUtils::getCustomPicUrl(info->uid, info->picVer), 1.0f, 60, true);
    }
    string rankstr = "Alliance_R";
    rankstr.append(CC_ITOA(m_info->rank));
    rankstr.append(".png");
    auto rankpic = CCLoadSprite::createSprite(rankstr.c_str());
    m_rankNode->addChild(rankpic);
    rankpic->setScale(0.5);
    m_sprBG1->setVisible(false);
    m_sprBG2->setVisible(false);
    m_sprBG3->setVisible(false);
    m_numText->setVisible(false);
    m_numspr1->setVisible(false);
    m_numspr2->setVisible(false);
    m_numspr3->setVisible(false);
    if(m_index==0){
        m_sprBG1->setVisible(true);
        m_numspr1->setVisible(true);
    }else if(m_index==1){
        m_sprBG2->setVisible(true);
        m_numspr2->setVisible(true);
    }else if(m_index==2){
        m_sprBG3->setVisible(true);
        m_numspr3->setVisible(true);
    }else{
        m_numText->setVisible(true);

        m_numText->setString(CC_ITOA(m_index+1));
    }
    if(m_info->uid==GlobalData::shared()->playerInfo.uid){
        m_selfheadSpr->setVisible(true);
        m_otherheadSpr->setVisible(false);
        m_sprBG4->setVisible(true);
        m_sprBG1->setVisible(false);
        m_sprBG2->setVisible(false);
        m_sprBG3->setVisible(false);
        m_text1->setColor({250,224,143});
        m_text2->setColor({250,224,143});
        m_text3->setColor({250,224,143});
    }else{
        m_selfheadSpr->setVisible(false);
        m_otherheadSpr->setVisible(true);
        m_sprBG4->setVisible(false);
        m_text1->setColor({201,188,149});
        m_text2->setColor({201,188,149});
        m_text3->setColor({201,188,149});
    }
    m_text1->setString(m_info->name.c_str());
    m_text2->setString(CC_CMDITOA(m_info->donation));
    m_text3->setString(CC_CMDITOA(m_info->alliance_honor));
}

void AllianceDonateRankCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
   // CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDonateRankCell::refreashData), MSG_TOOL_CHANGE, NULL);
}

void AllianceDonateRankCell::onExit() {
    setTouchEnabled(false);
   // CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_CHANGE);
    CCNode::onExit();
}

void AllianceDonateRankCell::refreashData(CCObject* obj)
{

    
}

SEL_CCControlHandler AllianceDonateRankCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", AllianceDonateRankCell::onClickUseBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", AllianceDonateRankCell::onClickBuyBtn);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoreUseBtn", AllianceDonateRankCell::onClickStoreUseBtn);
    return NULL;
}


bool AllianceDonateRankCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(isTouchInside(this->m_hintBGNode, pTouch)){
        m_startPoint =pTouch->getStartLocation();
        return true;
    }
    return false;
}

void AllianceDonateRankCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(fabs(pTouch->getLocation().x-m_startPoint.x)>20||fabs(pTouch->getLocation().y-m_startPoint.y)>20){
        return;
    }
    if (isTouchInside(m_hintBGNode,pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        if(m_info->uid==GlobalData::shared()->playerInfo.uid){
            PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(&GlobalData::shared()->playerInfo,1));
            return;
        }
        PlayerInfoBtnPopUpView* pop =PlayerInfoBtnPopUpView::create();
        pop->InitBtns(m_info);
        PopupViewController::getInstance()->addPopupView(pop, false);
    }
}



bool AllianceDonateRankCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText", CCLabelIF*, m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text1", CCLabelIFTTF*, m_text1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text2", CCLabelIF*, m_text2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text3", CCLabelIF*, m_text3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numspr1", CCSprite*, m_numspr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numspr2", CCSprite*, m_numspr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numspr3", CCSprite*, m_numspr3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode", CCNode*, m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankNode", CCNode*, m_rankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintBGNode", CCNode*, m_hintBGNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG1", CCScale9Sprite*, m_sprBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG2", CCScale9Sprite*, m_sprBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG3", CCScale9Sprite*, m_sprBG3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG4", CCScale9Sprite*, m_sprBG4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_otherheadSpr", CCSprite*, m_otherheadSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selfheadSpr", CCSprite*, m_selfheadSpr);

    return false;
}


