//
//  AllianceFunView.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-6.
//
//

#include "AllianceFunView.h"
#include "PopupViewController.h"
#include "AllianceManager.h"
#include "AllianceWarDetailView.h"
#include "AllianceWarView.h"
#include "WorldController.h"
#include "AllianceFlagPar.h"
#include "UpdateAllianceInfoView.h"
#include "ChangeAllianceFlagView.h"
#include "CheckAllianceInfoView.h"
#include "JoinAllianceView.h"
#include "AllianceDonateRankView.h"
#include "AllianceCheckView.h"
#include "UIComponent.h"
#include "CountryChatCommand.h"
#include "LeaveAllianceCommand.h"
#include "DismissAllianceCommand.h"
#include "ChatController.h"
#include "ParticleFireAni.h"
#include "MailController.h"
#include "AllianceCommentView.h"
#include "AllianceUnlockView.h"
#include "ChatView.h"
#include "SoundController.h"
#include "ChatServiceCocos2dx.h"
#include "AllianceTerritoryShowCommand.h"
#include "ActivityController.h"

static const char* icons[10] = {"changeAllianceName.png","alliance_changeFlag.png","alliance_search.png","alliance_view.png","alliance_paihang.png","icon_comment.png","icon_remove_comment.png","icon_alliance_quit.png","alliance_view.png","alliance_view.png"};

static const char* langs[10] = {"115250","115256","115211","115210","115823","115929","115924","115039","140043",""};

enum AllianceFunBtnType{
    A_NAME_MODIFY = 0
    ,A_FLAG_MODIFY
    ,A_ALLIANCE_SEARCH
    ,A_ALLIANCE_VIEW
    ,A_ALLIANCE_PAI_HANG
    ,A_ALLIANCE_COMMENT
    ,A_ALLIANCE_BLOCK
    ,A_QUIT
    ,A_ALLIANCE_MANAGE_BATTLE
};

AllianceFunView *AllianceFunView::create(){
    AllianceFunView *ret = new AllianceFunView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceFunView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("115258"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceFunView::refreshFlag), MSG_CHANGE_ALLIANCE_ICON, NULL);
}

void AllianceFunView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_ALLIANCE_ICON);
    PopupBaseView::onExit();
}

void AllianceFunView::refreshFlag(CCObject* obj){
    m_icon->removeAllChildren();
    AllianceFlagPar* flag = AllianceFlagPar::create(GlobalData::shared()->playerInfo.allianceInfo.getAllianceIcon().c_str());
    m_icon->addChild(flag);
}

bool AllianceFunView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//        int curHight = -500;
//        while (curHight<maxHight) {
//            auto bg = CCLoadSprite::createSprite("technology_09.png");
//            bg->setAnchorPoint(ccp(0, 1));
//            bg->setPosition(ccp(0, curHight));
//            curHight += bg->getContentSize().height;
//            tBatchNode->addChild(bg);
//        }
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            tBatchNode->setScaleX(1536.0 / 640.0);
//        }
//        this->addChild(tBatchNode);
        
        auto node = CCBLoadFile("AlliancefunView", this, this);
        this->setContentSize(node->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        if (CCCommonUtils::isIosAndroidPad()) {
            add = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - add);
        m_funList->setContentSize(CCSize(m_funList->getContentSize().width,m_funList->getContentSize().height+add));
        m_funList->setPositionY(m_funList->getPositionY() - add);
        m_viewBg->setVisible(false);
        
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        
        m_icon->removeAllChildren();
        AllianceFlagPar* flag = AllianceFlagPar::create(GlobalData::shared()->playerInfo.allianceInfo.getAllianceIcon().c_str());
        m_icon->addChild(flag);
        
        if (AllianceManager::getInstance()->checkRight(SHOW_ALLIANCE_LANGUAGE, GlobalData::shared()->playerInfo.allianceInfo.rank)) {
            m_buttonFuns.push_back(A_NAME_MODIFY);
        }
        m_buttonFuns.push_back(A_FLAG_MODIFY);
        m_buttonFuns.push_back(A_ALLIANCE_SEARCH);
        m_buttonFuns.push_back(A_ALLIANCE_VIEW);
        m_buttonFuns.push_back(A_ALLIANCE_PAI_HANG);
        //m_buttonFuns.push_back(A_ALLIANCE_COMMENT);
        if (GlobalData::shared()->playerInfo.allianceInfo.rank>=4) {
            m_buttonFuns.push_back(A_ALLIANCE_BLOCK);
        }
        if (GlobalData::shared()->playerInfo.allianceInfo.rank==5 && ActivityController::getInstance()->checkOpenDragonBattle()) {
            m_buttonFuns.push_back(A_ALLIANCE_MANAGE_BATTLE);
        }
        m_buttonFuns.push_back(A_QUIT);
        m_cellNum = m_buttonFuns.size();
        
        m_tabView = CCMultiColTableView::create(this, m_funList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(Touch_Popup);
        m_funList->addChild(m_tabView);
        
        m_index = 0;

        ret = true;
        m_tabView->reloadData();
    }
    return ret;
}

bool AllianceFunView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modifySpr", CCSprite*, this->m_modifySpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funList", CCNode*, this->m_funList);
    return false;
}

bool AllianceFunView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void AllianceFunView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

}

void AllianceFunView::onClickConfirm(CCObject *pSender, CCControlEvent event){

}

void AllianceFunView::onClickCancel(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler AllianceFunView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

void AllianceFunView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize AllianceFunView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(452, 306);
    }
    return CCSize(190, 150);
}

CCTableViewCell* AllianceFunView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_cellNum){
        return NULL;
    }
    AllianceFunBtnCell* cell = (AllianceFunBtnCell*)table->dequeueGrid();
    if(idx < m_cellNum){
        if(cell){
            cell->setData(m_buttonFuns[idx]);
        }else{
            cell = AllianceFunBtnCell::create(m_buttonFuns[idx],m_funList);
        }
    }
    return cell;
}

ssize_t AllianceFunView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return (m_cellNum/3 + (m_cellNum%3==0?0:1));
}

unsigned int AllianceFunView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 3;
}

AllianceFunBtnCell *AllianceFunBtnCell::create(int index,CCNode* showArea){
    AllianceFunBtnCell *ret = new AllianceFunBtnCell(index,showArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceFunBtnCell::setData(int index){
    m_index = index;
    m_btnNode1->removeAllChildrenWithCleanup(true);
    string temp = _lang(langs[m_index]);
    if(GlobalData::shared()->playerInfo.allianceInfo.currentNum<=1 && GlobalData::shared()->playerInfo.allianceInfo.rank==5 && m_index==7){
        temp = _lang("115037");
    }
    m_nameText1->setString(temp);
    if (!CCCommonUtils::isIosAndroidPad())
    {
        m_nameText1->setMaxScaleXByWidth(245);
    }
    else {
        float sc = m_nameText1->getScale();
        float fw = m_nameText1->getContentSize().width;
        if (fw>315) {//防爆框
            setScaleX(315.0/fw * sc);
        }
    }
    
    m_btnNode1->removeAllChildrenWithCleanup(true);
    auto pic = CCLoadSprite::createSprite(icons[m_index]);
    m_btnNode1->addChild(pic);
}

void AllianceFunBtnCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceFunBtnCell::checkIfStorage), CHECKIFSTORAGE_DATA_BACK, NULL);
}

void AllianceFunBtnCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHECKIFSTORAGE_DATA_BACK);
    CCNode::onExit();
}

bool AllianceFunBtnCell::init(){
    auto bg = CCBLoadFile("AllianceFunBtnCell", this, this);
    this->setContentSize(bg->getContentSize());
    
    setData(m_index);
    return true;
}

bool AllianceFunBtnCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPos = pTouch->getLocation();
    if(isTouchInside(m_nodeClick1, pTouch) && isTouchInside(m_showArea, pTouch)){
        m_btnNode1->setScale(1.05);
        return true;
    }
    return true;
}

void AllianceFunBtnCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    m_btnNode1->setScale(1.0);
    CCPoint pos = pTouch->getLocation();
    if(isTouchInside(m_nodeClick1, pTouch) && isTouchInside(m_showArea, pTouch) && fabs(pos.y - m_startPos.y)<=30){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        switch (m_index) {
            case A_NAME_MODIFY:
                PopupViewController::getInstance()->addPopupInView(UpdateAllianceInfoView::create(-1));
                break;
            case A_FLAG_MODIFY:
                PopupViewController::getInstance()->addPopupView(AllianceCheckView::create(),true,true);
                break;
            case A_ALLIANCE_SEARCH:
                PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
                break;
            case A_ALLIANCE_VIEW:
                PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
                break;
            case A_ALLIANCE_PAI_HANG:
                PopupViewController::getInstance()->addPopupInView(AllianceDonateRankView::create(0));
                break;
            case A_ALLIANCE_COMMENT:
            {
                PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COMMENT,GlobalData::shared()->playerInfo.allianceInfo.uid),false);
            }
                break;
            case A_ALLIANCE_BLOCK:
            {
                
                if(GlobalData::shared()->playerInfo.allianceInfo.rank>=4)
                {
                    PopupViewController::getInstance()->addPopupInView(AllianceUnlockView::create());
                }
            }
                break;
            case A_ALLIANCE_MANAGE_BATTLE:
            {
                
            }
                break;
            case A_QUIT:
            {
                if(GlobalData::shared()->isCrossService){
                    YesNoDialog::show(_lang("138064").c_str(), NULL);
                    return ;
                }
                if(GlobalData::shared()->playerInfo.allianceInfo.rank==5 && GlobalData::shared()->playerInfo.allianceInfo.currentNum>1){
                    CCCommonUtils::flyHint("", "", _lang("115249"));
                }else if(GlobalData::shared()->playerInfo.allianceInfo.currentNum<=1 && GlobalData::shared()->playerInfo.allianceInfo.rank==5){
                    YesNoDialog::showVariableTitle(_lang("115040").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceFunBtnCell::jieXianAlliance)),_lang("confirm").c_str());
                }else {
                    YesNoDialog::showVariableTitle(_lang("115042").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceFunBtnCell::secondConfirm)),_lang("confirm").c_str());
                }
            }
                break;
            default:
                break;
        }
    }
}

void AllianceFunBtnCell::checkIfStorage(cocos2d::CCObject *param){
    if (m_index == A_QUIT) {
        auto dic = _dict(param);
        if(dic==NULL || dic->objectForKey("errorCode")) return;
        if (dic->objectForKey("ret")) {
            int ret = dic->valueForKey("ret")->intValue();
            if (ret == 1) {
                YesNoDialog::showVariableTitle(_lang("115461").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceFunBtnCell::sendChatFirst)),_lang("confirm").c_str());
            }
            else if (ret == 0) {
                sendChatFirst();
            }
        }
    }
}

void AllianceFunBtnCell::secondConfirm(){
    AllianceTerritoryCheckIfStorageCommand* cmd = new AllianceTerritoryCheckIfStorageCommand(GlobalData::shared()->playerInfo.uid);
    cmd->sendAndRelease();
}

void AllianceFunBtnCell::sendChatFirst(){
   // CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,_lang("115186").append("  (").append(_lang("115181")).append(")").c_str(),2);
   // cmd->sendAndRelease();
   exitAlliance(0.0f);
   // this->scheduleOnce(schedule_selector(AllianceFunBtnCell::exitAlliance),0.3);
}
void AllianceFunBtnCell::exitAlliance(float _time){
    GameController::getInstance()->showWaitInterface();
    LeaveAllianceCommand * command = new LeaveAllianceCommand(_lang("115186").append("  (").append(_lang("115181")).append(")").c_str(),2);
    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceFunBtnCell::leavelSuccess), NULL));
    command->sendAndRelease();
}

void AllianceFunBtnCell::leavelSuccess(CCObject* obj){
    UIComponent::getInstance()->changeChatChannel(CHAT_COUNTRY);
    PopupViewController::getInstance()->removeAllPopupView();
    ChatController::getInstance()->m_chatAlliancePool.clear();
    ChatController::getInstance()->m_latestAllianceMsg.sequenceId=0;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::resetPlayerIsInAlliance();
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    ChatServiceCocos2dx::resetPlayerIsInAlliance();
#endif
    CCUserDefault::sharedUserDefault()->setStringForKey(ALLIANCE_MEMBER_BTN_STATUS, "");
}

void AllianceFunBtnCell::jieXianAlliance(){
    DismissAllianceCommand * command = new DismissAllianceCommand();
    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceFunBtnCell::leavelSuccess), NULL));
    command->sendAndRelease();
}

void AllianceFunBtnCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void AllianceFunBtnCell::changePic(){
    
}

void AllianceFunBtnCell::clickHead(CCObject* data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        std::string clickPic = str->getCString();
    }
}

void AllianceFunBtnCell::changeSuccess(){

}

bool AllianceFunBtnCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick1", CCNode*, this->m_nodeClick1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode1", CCNode*, this->m_btnNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText1", CCLabelIF*, this->m_nameText1);
    return false;
}

SEL_CCControlHandler AllianceFunBtnCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}