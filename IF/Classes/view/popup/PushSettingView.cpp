//
//  PushSettingView.cpp
//  IF
//
//  Created by zhaohf on 14-4-4.
//
//

#include "PushSettingView.h"
#include "PushSettingCommand.h"
#include "PopupViewController.h"
#include "SuggestionView.h"
#include "SetAccountView.h"
#include "ShieldUnlockPopUpView.h"
#include "PopupViewController.h"
#include "LogoutCommand.h"
#include "PushSettingView.h"
#include "HelpView.h"
#include "BattleManager.h"
#include "SoundController.h"
#include "HowToPlayView.h"
#include "LoginServiceView.h"
#include "ChangeServerView.h"
#include "QueueController.h"


PushSettingView* PushSettingView::create(){
    PushSettingView* ret = new PushSettingView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}

void PushSettingView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("101217"));
}

void PushSettingView::onExit(){
    PopupBaseView::onExit();
}

PushSettingView::~PushSettingView()
{
    
}

bool PushSettingView::init(){
    
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto bg = CCBLoadFile("LanguageSettingView", this, this);
        this->setContentSize(bg->getContentSize());
        int preHg =m_popupBG->getContentSize().height;
        changeBGHeight(m_popupBG);
        int dh =m_popupBG->getContentSize().height-preHg;
        m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height+dh));
        changeBGHeight(m_bg);
        
        m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width,m_listContainer->getContentSize().height+dh));
        m_data = CCArray::create();
        m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_listContainer->addChild(m_tabView);
        setDataList();
        
        ret = true;
    }
    return ret;
}
bool PushSettingView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupBG", CCScale9Sprite*, this->m_popupBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    return true;
}

SEL_CCControlHandler PushSettingView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}
void PushSettingView::setDataList(){
    m_data->removeAllObjects();
    //偶先bug:Queue重复出现,未查到原因,所以这里先加一个去重的操作
    vector<int> checkList;
    vector<PushSettingInfo*>::iterator it;
    for (it = GlobalData::shared()->pushSettingVector.begin(); it!=GlobalData::shared()->pushSettingVector.end(); it++) {
        if((*it)->type==1||(*it)->type==2||(*it)->type==3||(*it)->type==5||(*it)->type==6||(*it)->type==7||(*it)->type==8||(*it)->type==9){//control pushMessage  ||(*it)->type==10
            if(find(checkList.begin(), checkList.end(), (*it)->type)!=checkList.end()){
                continue;
            }
            m_data->addObject(CCString::create(CC_ITOA((*it)->type)));
            checkList.push_back((*it)->type);
        }
    }

    m_tabView->reloadData();
}

void PushSettingView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}
ssize_t PushSettingView::numberOfCellsInTableView(CCTableView *table)
{
    int num = m_data->count();
    return num;
}
CCSize PushSettingView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    int size = m_data->count();
    if(idx >= size){
        return CCSizeZero;
    }
    int height = 240;
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(640, 380);
    }
    return CCSize(640, height);
}

CCSize PushSettingView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 24);
}

CCTableViewCell* PushSettingView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    int size = m_data->count();
    if(idx >= size){
        return NULL;
    }
    PushCell* cell = (PushCell*)table->dequeueCell();
    string type = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    if(cell){
        cell->setData(atoi(type.c_str()));
    }else{
        cell = PushCell::create(atoi(type.c_str()));
    }
    return cell;
}


    
//-----------
PushCell *PushCell::create(int index){
    PushCell *ret = new PushCell(index);
    if(ret && ret->init(index)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
    
void PushCell::onEnter(){
    CCNode::onEnter();
   
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item,false);
}

void PushCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool PushCell::init(int index){
    auto bg = CCBLoadFile("PushCell", this, this);
    this->setContentSize(bg->getContentSize());
    m_index = index;
    setData(index);
    return true;
}

void PushCell::setData(int index){
    m_index = index;
    vector<PushSettingInfo*>::iterator it;
    for (it = GlobalData::shared()->pushSettingVector.begin(); it!=GlobalData::shared()->pushSettingVector.end(); it++) {
        if((*it)->type==m_index){
            m_info = (*it);
            break;
        }
    }
    refreshView();
}

bool PushCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtnBg", CCSprite*, this->m_touchBtnBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtn1", CCSprite*, this->m_touchBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtn", CCSprite*, this->m_touchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_TitleText", CCLabelIF*, this->m_TitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desText", CCLabelIF*, this->m_desText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soundOnSpr", CCSprite*, this->m_soundOnSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soundOffSpr", CCSprite*, this->m_soundOffSpr);
    
    return false;
}

SEL_CCControlHandler PushCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

bool PushCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(m_touchBtnBg, pTouch) || (m_soundOffSpr->isVisible() && isTouchInside(m_soundOffSpr, pTouch)) || (m_soundOnSpr->isVisible() && isTouchInside(m_soundOnSpr, pTouch))) {
        return true;
    }
    return false;
}
void PushCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_soundOffSpr->isVisible() && isTouchInside(m_soundOffSpr, pTouch)  && m_info->status==1) {//打开音效 (推送通知打开的情况下才能打开)
        m_info->sound = 1;
    }else if (m_soundOnSpr->isVisible() && isTouchInside(m_soundOnSpr, pTouch) ) {//关闭音效
        m_info->sound = 0;
    }else {
        if (m_info->status==0) { // 打开
            m_info->status = 1;
            m_info->sound = 1;
        }else if (m_info->status==1) { //关闭
            m_info->status = 0;
            m_info->sound = 0;
        }else {
            return;
        }
    }
    
    refreshView();

    PushSettingCommand* cmd = new PushSettingCommand(m_index, m_info->status, m_info->sound);
    cmd->sendAndRelease();
    
    if (m_index==1) {
        QueueController::getInstance()->ResetNoticeParam(m_info->status, m_info->sound);
    }
    
//    refreshView();
}

void PushCell::refreshView()
{
    if (m_info->status==0)
    {
        CCCommonUtils::setSpriteGray(m_touchBtnBg, true);
        CCCommonUtils::setSpriteGray(m_touchBtn1, true);
//        m_touchBtnCloseBg->setVisible(true);
//        m_touchBtnOpenBg->setVisible(false);
        m_touchBtn->setPositionX(-29);
        m_touchBtn1->setPositionX(-29);
        
//        m_soundOffSpr->setVisible(true);
//        m_soundOnSpr->setVisible(false);
        
    }else{
        CCCommonUtils::setSpriteGray(m_touchBtnBg, false);
        CCCommonUtils::setSpriteGray(m_touchBtn1, false);
//        m_touchBtnCloseBg->setVisible(false);
//        m_touchBtnOpenBg->setVisible(true);
        m_touchBtn->setPositionX(29);
        m_touchBtn1->setPositionX(29);
        
        if (m_info->sound == 1) {
//            m_soundOffSpr->setVisible(false);
//            m_soundOnSpr->setVisible(true);
        }else {
//            m_soundOffSpr->setVisible(true);
//            m_soundOnSpr->setVisible(false);
        }
    }
    string title = "";
    string des = "";
    if(m_info->type==1){//建筑 科技 造兵
        title= _lang("105600");
        des= _lang("105630");
    }else if(m_info->type==2){//被侦查 被攻击
        title= _lang("105601");
        des= _lang("105631");
    }else if(m_info->type==5){//联盟相关
        title= _lang("105602");
        des= _lang("105632");
    }else if(m_info->type==6){//活动推送
        title= _lang("105609");
        des= _lang("105637");
    }else if(m_info->type==7){
        title= _lang("105604");
        des= _lang("105633");
    }else if(m_info->type==8){//聊天
        title= _lang("105605");
        des= _lang("105634");
    }else if(m_info->type==3){//邮件
        title= _lang("105606");
        des= _lang("105635");
    }else if(m_info->type==9){//礼包
        title= _lang("105607");
        des= _lang("105636");
    }else if(m_info->type==10){//web在线
        title= _lang("105608");
        des= _lang("105679");
    }
    m_TitleText->setString(title);
    m_desText->setString(des);
}

void PushCell::showActivityDetail(){
    
}
