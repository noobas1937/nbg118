//
//  AllianceRankView.cpp
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

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
#include "AllianceFlagPar.h"
#include "TipsView.h"
#include "GetAllianceListCommand.h"
#include "CheckAllianceInfoView.h"

AllianceRankView* AllianceRankView::create(int type){
    AllianceRankView* ret = new AllianceRankView(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool AllianceRankView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(205, false);
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
    
//    int BGcount = (newBgHeight-80)/100+1-1;
//    for (int i=0; i<BGcount; i++) {
//        auto pic = CCLoadSprite::createSprite("technology_09.png");
//        m_totalNode->addChild(pic);
//        pic->setPositionY(219-(i+1)*100);
//    }
    
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

void AllianceRankView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceRankView::refreashData), ALLIANCE_RANK, NULL);
    string name = "";
    string tile2= "";
    if(m_type==1 ){
        name = _lang("108102")+" "+_lang("102163")+" "+_lang("108100");
        tile2 = _lang("102163");
    }else if(m_type==2){
        name = _lang("108102")+" "+_lang("108108");
        tile2 = _lang("108109");
    }
    setTitleName(name);
    m_textTitle2->setString(tile2);
}

void AllianceRankView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_RANK);
    
    CCNode::onExit();
}

SEL_CCControlHandler AllianceRankView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", AllianceRankView::onTipBtnClick);
    
    return NULL;
}

bool AllianceRankView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
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
void AllianceRankView::getRankData(){
    GameController::getInstance()->showWaitInterface();
    if(m_type==1){
        AllianceRankListCommand* cmd = new AllianceRankListCommand();
        cmd->sendAndRelease();
    }else{
        AllianceKillRankListCommand* cmd = new AllianceKillRankListCommand();
        cmd->sendAndRelease();
    }
}

void AllianceRankView::updateInfo()
{
    if(!GlobalData::shared()->playerInfo.isInAlliance()){
        m_rankText->setString("");
        m_rankTitle->setString(_lang("108105"));
        m_rankTitle->setAnchorPoint(ccp(0.5, 0.5));
        m_rankTitle->setPositionX(0);
    }else{
        m_rankText->setString(CC_ITOA(GlobalData::shared()->rankListInfo.allianceRank));
    }
    m_data->removeAllObjects();
    m_allianceList = GlobalData::shared()->rankListInfo.allianceRankList;
    vector<AllianceRankInfo*>::iterator it;
    for(it = m_allianceList.begin(); it != m_allianceList.end(); it++){
        m_data->addObject(*it);
    }

    m_tabView->reloadData();
}

void AllianceRankView::refreashData(CCObject* p)
{
    updateInfo();
}


CCSize AllianceRankView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_allianceList.size()){
        return CCSizeZero;
    }
    
    return CCSize(625, 130);
    
}
CCSize AllianceRankView::cellSizeForTable(CCTableView *table)
{
    return CCSize(625, 130);
}
CCTableViewCell* AllianceRankView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    AllianceRankListCell* cell = (AllianceRankListCell*)table->dequeueCell();
    if(idx < m_data->count()){
        AllianceRankInfo* info = dynamic_cast<AllianceRankInfo*>(m_data->objectAtIndex(idx));
        if(cell){
            cell->setData(info,idx,m_type);
        }else{
            cell = AllianceRankListCell::create(info,idx,m_type);
        }
    }
    return cell;
}
ssize_t AllianceRankView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1);
    return num;
}
void AllianceRankView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void AllianceRankView::onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    string title = _lang("108102")+" "+_lang("102163")+" "+_lang("108100");
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("108106")));
}

AllianceRankListCell* AllianceRankListCell::create(AllianceRankInfo* info,int index,int type)
{
    auto ret = new AllianceRankListCell(info,index,type);
    if (ret && ret->init(info,index)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceRankListCell::init(AllianceRankInfo* info,int index)
{
    bool ret = true;
    CCBLoadFile("AllianceRankCell",this,this);
    
    setData(info,index,m_type);
    return ret;
}

void AllianceRankListCell::setData(AllianceRankInfo* info,int index,int type)
{
    m_info = info;
    m_index = index;
    m_type = type;
    m_headNode->removeAllChildren();
    CCSprite* pic;
    if(m_info->icon==""){
        pic = CCLoadSprite::createSprite("Allance_flay.png");
        pic->setScale(0.6);
        m_headNode->addChild(pic);
    }else{
        
        string mpic = m_info->icon+".png";
         AllianceFlagPar* flag = AllianceFlagPar::create(mpic.c_str());
//        pic = CCLoadSprite::createSprite(mpic.c_str());
        flag->setScale(0.6);
        m_headNode->addChild(flag);
        
    }
//    CCCommonUtils::setSpriteMaxSize(pic, 60);
    
//    string rankstr = "Alliance_R";
//    rankstr.append(CC_ITOA(m_info->rank));
//    rankstr.append(".png");
//    auto rankpic = CCLoadSprite::createSprite(rankstr.c_str());
//    m_rankNode->addChild(rankpic);
//    rankpic->setScale(0.5);
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
    if(m_info->uid==GlobalData::shared()->playerInfo.allianceInfo.uid){
//        m_selfheadSpr->setVisible(true);
//        m_otherheadSpr->setVisible(false);
        m_sprBG4->setVisible(true);
        m_sprBG1->setVisible(false);
        m_sprBG2->setVisible(false);
        m_sprBG3->setVisible(false);
//        m_text1->setColor({250,224,143});
//        m_text2->setColor({250,224,143});
//        m_text3->setColor({250,224,143});
    }else{
//        m_selfheadSpr->setVisible(false);
//        m_otherheadSpr->setVisible(true);
        m_sprBG4->setVisible(false);
//        m_text1->setColor({201,188,149});
//        m_text2->setColor({201,188,149});
//        m_text3->setColor({201,188,149});
    }
    string name = "("+m_info->abbr+")"+m_info->alliancename;
    m_text1->setString(name.c_str());
    string leader =_lang("105403")+m_info->leader;
    m_text2->setString(leader.c_str());
    if(m_type==1){
        m_text3->setString(CC_CMDITOAL(m_info->fightpower));
    }else if(m_type==2){
        m_text3->setString(CC_CMDITOAL(m_info->armyKill));
    }
}

void AllianceRankListCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
}

void AllianceRankListCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void AllianceRankListCell::refreashData(CCObject* obj)
{
    
    
}

SEL_CCControlHandler AllianceRankListCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", AllianceDonateRankCell::onClickUseBtn);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", AllianceDonateRankCell::onClickBuyBtn);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoreUseBtn", AllianceDonateRankCell::onClickStoreUseBtn);
    return NULL;
}


bool AllianceRankListCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(isTouchInside(this->m_hintBGNode, pTouch)){
        m_startPoint = pTouch->getLocation();
        return true;
    }
    return false;
}

void AllianceRankListCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(fabs(pTouch->getLocation().y-m_startPoint.y)>20||fabs(pTouch->getLocation().x-m_startPoint.x)>20){
        return;
    }
  if (isTouchInside(m_hintBGNode,pTouch)) {

      GetAllianceListCommand* cmd = new GetAllianceListCommand(m_info->alliancename,1,1);
      cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceRankListCell::showAllianceInfo), NULL));
      cmd->sendAndRelease();
   }
}

void AllianceRankListCell::showAllianceInfo(cocos2d::CCObject *data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto params = _dict(result->getData());
    CCArray* arr =  (CCArray*)params->objectForKey("list");
    int num = arr->count();
    for (int i=0; i<num; i++) {
        CCDictionary* dicAlliance = (CCDictionary*)arr->objectAtIndex(i);

        AllianceInfo* alliance = new AllianceInfo();
        alliance->updateAllianceInfo(dicAlliance);
        if(alliance->name==m_info->alliancename){
            PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(alliance));
            alliance->release();
            break;
        }
        alliance->release();
    }
}

bool AllianceRankListCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText", CCLabelIF*, m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text1", Label*, m_text1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text2", Label*, m_text2);
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


