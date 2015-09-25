//
//  PlayerRankListView.cpp
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#include "PlayerRankListView.h"
#include "AllianceRankView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "UseCDToolView.h"
#include "FunBuildController.h"
#include "ChangeNickNameView.h"
#include "WorldMapView.h"
#include "ChangePicView.h"
#include "PlayerInfoBtnPopUpView.h"
#include "RoleInfoView.h"
#include "AllRankCommand.h"
#include "TipsView.h"

PlayerRankListView* PlayerRankListView::create(int type){
    PlayerRankListView* ret = new PlayerRankListView(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool PlayerRankListView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    auto tmpCCB = CCBLoadFile("AllianceRank",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    
    m_data = CCArray::create();
    m_isgetAllCommand = false;
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    int BGcount = (newBgHeight-80)/100+1-1;
    for (int i=0; i<BGcount; i++) {
        auto pic = CCLoadSprite::createSprite("technology_09.png");
        m_totalNode->addChild(pic);
        pic->setPositionY(219-(i+1)*100);
    }
    
    m_textTitle1->setString(_lang("115825"));
   

    m_rankTitle->setString(_lang_1("108104",""));
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);

    getRankData();
    
    return true;
}

void PlayerRankListView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PlayerRankListView::refreashData), PLAYER_RANK, NULL);
    string name ="";
    string title2 = "";
    if(m_type==1){
        name =_lang("108101")+" "+ _lang("108103");
        title2 = _lang("102163");
    }else if(m_type==2){
        name =_lang("108101")+" "+ _lang("108108");
        title2 = _lang("108109");
    }else if(m_type==3){
        name =_lang("108101")+" "+ _lang("108110");
        title2 = _lang("108634");
    }else if(m_type==4){
        name =_lang("108101")+" "+ _lang("108112");
        title2 = _lang("108634");
    }
     m_textTitle2->setString(title2);
    setTitleName(name.c_str());
}

void PlayerRankListView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PLAYER_RANK);
    
    CCNode::onExit();
}

SEL_CCControlHandler PlayerRankListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", PlayerRankListView::onTipBtnClick);
    
    return NULL;
}

bool PlayerRankListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankText", CCLabelIF*, this->m_rankText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTitle", CCLabelIF*, this->m_rankTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textTitle1", CCLabelIF*, this->m_textTitle1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textTitle2", CCLabelIF*, this->m_textTitle2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    
    return false;
}
void PlayerRankListView::getRankData(){
    GameController::getInstance()->showWaitInterface();
    if(m_type==1){
        PlayerRankListCommand* cmd = new PlayerRankListCommand();
        cmd->sendAndRelease();
    }else if(m_type==2){
        PlayerKillRankListCommand* cmd = new PlayerKillRankListCommand();
        cmd->sendAndRelease();
    }else if(m_type==3){
        PlayerBuildingRankListCommand* cmd = new PlayerBuildingRankListCommand();
        cmd->sendAndRelease();
    }else if(m_type==4){
        PlayerLevelRankListCommand *cmd = new PlayerLevelRankListCommand();
        cmd->sendAndRelease();
    }
}

void PlayerRankListView::updateInfo()
{
    m_rankText->setString(CC_ITOA(GlobalData::shared()->rankListInfo.playerRank));
    m_data->removeAllObjects();
    m_allianceList = GlobalData::shared()->rankListInfo.playerRankList;
    vector<PlayerRankInfo*>::iterator it;
    for(it = m_allianceList.begin(); it != m_allianceList.end(); it++){
        m_data->addObject(*it);
    }
    
    m_tabView->reloadData();
}

void PlayerRankListView::refreashData(CCObject* obj)
{
    updateInfo();
}


CCSize PlayerRankListView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_allianceList.size()){
        return CCSizeZero;
    }
    
    return CCSize(625, 120);
    
}
CCSize PlayerRankListView::cellSizeForTable(CCTableView *table)
{
    return CCSize(625, 120);
}
CCTableViewCell* PlayerRankListView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    PlayerRankListCell* cell = (PlayerRankListCell*)table->dequeueCell();
    if(idx < m_data->count()){
        PlayerRankInfo* info = dynamic_cast<PlayerRankInfo*>(m_data->objectAtIndex(idx));
        if(cell){
            cell->setData(info,idx,m_type);
        }else{
            cell = PlayerRankListCell::create(info,idx,m_type);
        }
    }
    return cell;
}
ssize_t PlayerRankListView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1);
    return num;
}
void PlayerRankListView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void PlayerRankListView::onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    string title  =_lang("108101")+" "+ _lang("108103");
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("108106")));
}

PlayerRankListCell* PlayerRankListCell::create(PlayerRankInfo* info,int index,int type)
{
    auto ret = new PlayerRankListCell(info,index,type);
    if (ret && ret->init(info,index)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PlayerRankListCell::init(PlayerRankInfo* info,int index)
{
    bool ret = true;
    CCBLoadFile("PlayerRankCell",this,this);
    
    m_headImgNode = HFHeadImgNode::create();
    setData(info,index,m_type);
    return ret;
}

void PlayerRankListCell::setData(PlayerRankInfo* info,int index,int type)
{
    m_info = info;
    m_index = index;
    m_type = type;
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
    if (CCCommonUtils::isUseCustomPic(m_info->picVer))
    {
        m_headImgNode->initHeadImgUrl2(m_headNode, CCCommonUtils::getCustomPicUrl(m_info->uid, m_info->picVer), 1.0f, 60, true);
    }

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
    }else{
        m_selfheadSpr->setVisible(false);
        m_otherheadSpr->setVisible(true);
        m_sprBG4->setVisible(false);
    }

    string allname ;
    string name = "";
    if(m_info->alliancename==""){
        allname = "";
        name = m_info->name;
    }else{
        allname =_lang_1("108596", m_info->alliancename.c_str());
        name = "("+m_info->abbr+")"+m_info->name;
    }
    m_text2->setString(allname);
    if(m_type==1){
        m_text3->setString(CC_CMDITOAL(m_info->power));
    }else if(m_type==2){
        m_text3->setString(CC_CMDITOAL(m_info->armyKill));
    }else if(m_type==3){
        m_text3->setString(CC_CMDITOAL(m_info->level));
    }else if(m_type==4){
        m_text3->setString(CC_CMDITOAL(m_info->level));
    }
    m_text1->setString(name.c_str());
}

void PlayerRankListCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
}

void PlayerRankListCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void PlayerRankListCell::refreashData(CCObject* obj)
{
    
    
}

SEL_CCControlHandler PlayerRankListCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", AllianceDonateRankCell::onClickUseBtn);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", AllianceDonateRankCell::onClickBuyBtn);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoreUseBtn", AllianceDonateRankCell::onClickStoreUseBtn);
    return NULL;
}


bool PlayerRankListCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(isTouchInside(this->m_hintBGNode, pTouch)){
        m_startPoint = pTouch->getLocation();
        return true;
    }
    return false;
}

void PlayerRankListCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(fabs(pTouch->getLocation().y - m_startPoint.y)>20||fabs(pTouch->getLocation().x - m_startPoint.x)>20){
        return;
    }
  if (isTouchInside(m_hintBGNode,pTouch)) {
        if(m_info->uid==GlobalData::shared()->playerInfo.uid){
            PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(&GlobalData::shared()->playerInfo,1));
            return;
        }
        PlayerInfoBtnPopUpView* pop =PlayerInfoBtnPopUpView::create();
        pop->InitCommonBtns(m_info->name, m_info->uid);
        PopupViewController::getInstance()->addPopupView(pop, false);
   }
}



bool PlayerRankListCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText", CCLabelIF*, m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text1", Label*, m_text1);
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