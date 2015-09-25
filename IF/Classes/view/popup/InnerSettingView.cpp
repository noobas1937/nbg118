//
//  InnerSettingView.cpp
//  IF
//
//  Created by zhangjun on 15-4-13.
//
//

#include "InnerSettingView.h"
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
#include "GetUserInfoCommand.h"

InnerSettingView* InnerSettingView::create(){
    InnerSettingView* ret = new InnerSettingView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}

void InnerSettingView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("101343"));//_lang("101217")
}

void InnerSettingView::onExit(){
    PopupBaseView::onExit();
}

InnerSettingView::~InnerSettingView()
{
    
}

bool InnerSettingView::init(){
    
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
bool InnerSettingView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupBG", CCScale9Sprite*, this->m_popupBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    return true;
}

SEL_CCControlHandler InnerSettingView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}
void InnerSettingView::setDataList(){
    m_data->removeAllObjects();

    m_data->addObject(CCString::create(CC_ITOA(ISetting_Sound)));
    m_data->addObject(CCString::create(CC_ITOA(ISetting_TaskTips)));
    m_data->addObject(CCString::create(CC_ITOA(ISetting_Equip)));
    m_data->addObject(CCString::create(CC_ITOA(ISetting_DaltonStyle)));
    if(GlobalData::shared()->analyticID == "cn1"){
        m_data->addObject(CCString::create(CC_ITOA(ISetting_Download)));
    }
    m_tabView->reloadData();
}

void InnerSettingView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}
ssize_t InnerSettingView::numberOfCellsInTableView(CCTableView *table)
{
    int num = m_data->count();
    return num;
}
CCSize InnerSettingView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    int size = m_data->count();
    if(idx >= size){
        return CCSizeZero;
    }
    int height = 240;
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(640, 470);
    }
    return CCSize(640, height);
}

CCSize InnerSettingView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 24);
}

CCTableViewCell* InnerSettingView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    int size = m_data->count();
    if(idx >= size){
        return NULL;
    }
    InnerCell* cell = (InnerCell*)table->dequeueCell();
    string type = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    if(cell){
        cell->setData(atoi(type.c_str()));
    }else{
        cell = InnerCell::create(atoi(type.c_str()));
    }
    return cell;
}


    
//-----------
InnerCell *InnerCell::create(int index){
    InnerCell *ret = new InnerCell(index);
    if(ret && ret->init(index)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
    
void InnerCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item,false);
}

void InnerCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool InnerCell::init(int index){
    touchIndex="0";
    auto bg = CCBLoadFile("InnerSettingCell", this, this);
    this->setContentSize(bg->getContentSize());
    m_index = index;
    setData(index);
    return true;
}

void InnerCell::setData(int index){
    m_index = index;
    m_info=InnerSettingInfo::create();
    if(m_index==ISetting_Sound){
        m_info->type=ISetting_Sound;
        bool _effectMusic = CCUserDefault::sharedUserDefault()->getBoolForKey(EFFECT_MUSIC_ON, true);
        m_info->status1=_effectMusic?1:0;
        bool _BGMusic = CCUserDefault::sharedUserDefault()->getBoolForKey(BG_MUSIC_ON, true);
        m_info->status2=_BGMusic?1:0;
    }else if (m_index==ISetting_TaskTips){
        m_info->type=ISetting_TaskTips;
        bool _Tips = CCUserDefault::sharedUserDefault()->getBoolForKey("TASK_TIPS_ON", true);
        m_info->status1=_Tips?1:0;
    }else if (m_index==ISetting_Equip){
        m_info->type=ISetting_Equip;
        bool _Tips = GlobalData::shared()->playerInfo.showEquip;
        m_info->status1=_Tips?1:0;
    }else if(m_index == ISetting_Download){
        m_info->type=ISetting_Download;
        std::string _download = CCUserDefault::sharedUserDefault()->getStringForKey(IS_AUTO_DOWNLOAD_ON, "1");
        m_info->status1 = (_download == "1");
    }
    else if (m_index == ISetting_DaltonStyle){
        m_info->type=ISetting_DaltonStyle;
        bool _Tips = CCUserDefault::sharedUserDefault()->getIntegerForKey(DALTONSTYLE, 0);
        m_info->status1=_Tips?1:0;
    }
    refreshView();
}

bool InnerCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_TitleText", CCLabelIF*, this->m_TitleText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeType1", CCNode*, this->m_nodeType1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soundOnSpr", CCSprite*, this->m_soundOnSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soundOffSpr", CCSprite*, this->m_soundOffSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_11touchBtnBg", CCSprite*, this->m_11touchBtnBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_11touchBtn", CCSprite*, this->m_11touchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_11touchBtn1", CCSprite*, this->m_11touchBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_musicOnSpr", CCSprite*, this->m_musicOnSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_musicOffSpr", CCSprite*, this->m_musicOffSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_12touchBtnBg", CCSprite*, this->m_12touchBtnBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_12touchBtn", CCSprite*, this->m_12touchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_12touchBtn1", CCSprite*, this->m_12touchBtn1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeType2", CCNode*, this->m_nodeType2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_2desText", CCLabelIF*, this->m_2desText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_2touchBtnBg", CCSprite*, this->m_2touchBtnBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_2touchBtn", CCSprite*, this->m_2touchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_2touchBtn1", CCSprite*, this->m_2touchBtn1);
    
    return false;
}

SEL_CCControlHandler InnerCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

bool InnerCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (m_info->type==ISetting_Sound){
        if (isTouchInside(m_11touchBtnBg, pTouch)){
            touchIndex="11";
            return true;
        }
        if (isTouchInside(m_12touchBtnBg, pTouch)){
            touchIndex="12";
            return true;
        }
    }
    if (m_info->type==ISetting_TaskTips||m_info->type==ISetting_Equip||m_info->type==ISetting_DaltonStyle){
        if (isTouchInside(m_2touchBtnBg, pTouch)){
            touchIndex="2";
            return true;
        }
    }
    if (m_info->type==ISetting_Download){
        if (isTouchInside(m_2touchBtnBg, pTouch)){
            touchIndex="2";
            return true;
        }
    }
    
    return false;
}
void InnerCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if ((touchIndex=="11"||touchIndex=="2")&& m_info->status1==0) {
        m_info->status1 = 1;
        if (m_info->type==ISetting_Sound) {
            CCUserDefault::sharedUserDefault()->setBoolForKey(EFFECT_MUSIC_ON, true);
            CCUserDefault::sharedUserDefault()->flush();
            CCCommonUtils::flyHint("", "", _lang("101349"));//开启音效提示
        }
        if (m_info->type==ISetting_TaskTips) {//"TASK_TIPS_ON"
            CCUserDefault::sharedUserDefault()->setBoolForKey("TASK_TIPS_ON", true);
            CCUserDefault::sharedUserDefault()->flush();
            CCCommonUtils::flyHint("", "", _lang("101399"));//开启任务提示
        }
        if (m_info->type==ISetting_Equip) {//开启查看装备
//            CCUserDefault::sharedUserDefault()->setBoolForKey("SEE_EQUIP_ON", true);
//            CCUserDefault::sharedUserDefault()->flush();
//            CCCommonUtils::flyHint("", "", _lang("101399"));
            auto cmd = new SetUserEquipShowCommand(1);
            cmd->sendAndRelease();
            GlobalData::shared()->playerInfo.showEquip=true;
        }
        if (m_info->type==ISetting_DaltonStyle) {
            CCUserDefault::sharedUserDefault()->setIntegerForKey(DALTONSTYLE, 1);
            CCCommonUtils::flyHint("", "", _lang("113854"));
        }
        if (m_info->type==ISetting_Download) {//"download"
            CCUserDefault::sharedUserDefault()->setStringForKey(IS_AUTO_DOWNLOAD_ON, "1");
            CCUserDefault::sharedUserDefault()->flush();
            CCCommonUtils::flyHint("", "", "您已经成功开启了自动下载");//开启自动下载提示
        }
    }else if (touchIndex=="12"&& m_info->status2==0) {
        m_info->status2 = 1;
        CCUserDefault::sharedUserDefault()->setBoolForKey(BG_MUSIC_ON, true);
        CCUserDefault::sharedUserDefault()->flush();
        SoundController::sharedSound()->playBGMusic(SoundController::sharedSound()->getBG_NAME().c_str());
        CCCommonUtils::flyHint("", "", _lang("101348"));//开启音乐提示
    }else if ((touchIndex=="11"||touchIndex=="2")&& m_info->status1==1) {
        m_info->status1 = 0;
        if (m_info->type==ISetting_Sound) {
            CCUserDefault::sharedUserDefault()->setBoolForKey(EFFECT_MUSIC_ON, false);
            CCUserDefault::sharedUserDefault()->flush();
            CCCommonUtils::flyHint("", "", _lang("101346"));//关闭音效提示
        }
        if (m_info->type==ISetting_TaskTips) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("TASK_TIPS_ON", false);
            CCUserDefault::sharedUserDefault()->flush();
            CCCommonUtils::flyHint("", "", _lang("101347"));//关闭任务提示101347
        }
        if (m_info->type==ISetting_Equip) {//关闭查看装备
//            CCUserDefault::sharedUserDefault()->setBoolForKey("SEE_EQUIP_ON", false);
//            CCUserDefault::sharedUserDefault()->flush();
//            CCCommonUtils::flyHint("", "", _lang("101347"));
            auto cmd = new SetUserEquipShowCommand(0);
            cmd->sendAndRelease();
            GlobalData::shared()->playerInfo.showEquip=false;
        }
        if (m_info->type==ISetting_DaltonStyle) {
            CCUserDefault::sharedUserDefault()->setIntegerForKey(DALTONSTYLE, 0);
            CCCommonUtils::flyHint("", "", _lang("113855"));//
        }
        if (m_info->type==ISetting_Download) {
            CCUserDefault::sharedUserDefault()->setStringForKey(IS_AUTO_DOWNLOAD_ON, "0");
            CCUserDefault::sharedUserDefault()->flush();
            CCCommonUtils::flyHint("", "", "您已经成功关闭了自动下载");//
        }
    }else if (touchIndex=="12"&& m_info->status2==1) {
        m_info->status2 = 0;
        CCUserDefault::sharedUserDefault()->setBoolForKey(BG_MUSIC_ON, false);
        CCUserDefault::sharedUserDefault()->flush();
        SoundController::sharedSound()->stopAllMusic();
        CCCommonUtils::flyHint("", "", _lang("101345"));//关闭音乐提示
    }

    refreshView();

}

void InnerCell::refreshView()
{
    if (m_info->type==ISetting_Sound) {
        string title = "";
        title= _lang("101341");
        m_TitleText->setString(title);
        
        m_nodeType1->setVisible(true);
        m_nodeType2->setVisible(false);
        if (m_info->status1==0){//音效开关
            CCCommonUtils::setSpriteGray(m_11touchBtnBg, true);
            CCCommonUtils::setSpriteGray(m_11touchBtn1, true);
            m_11touchBtn->setPositionX(-29);
            m_11touchBtn1->setPositionX(-29);
            m_soundOnSpr->setVisible(false);
            m_soundOffSpr->setVisible(true);
        }else{
            CCCommonUtils::setSpriteGray(m_11touchBtnBg, false);
            CCCommonUtils::setSpriteGray(m_11touchBtn1, false);
            m_11touchBtn->setPositionX(29);
            m_11touchBtn1->setPositionX(29);
            m_soundOnSpr->setVisible(true);
            m_soundOffSpr->setVisible(false);
        }
        if (m_info->status2==0){//音乐开关
            CCCommonUtils::setSpriteGray(m_12touchBtnBg, true);
            CCCommonUtils::setSpriteGray(m_12touchBtn1, true);
            m_12touchBtn->setPositionX(-29);
            m_12touchBtn1->setPositionX(-29);
            m_musicOnSpr->setVisible(false);
            m_musicOffSpr->setVisible(true);
        }else{
            CCCommonUtils::setSpriteGray(m_12touchBtnBg, false);
            CCCommonUtils::setSpriteGray(m_12touchBtn1, false);
            m_12touchBtn->setPositionX(29);
            m_12touchBtn1->setPositionX(29);
            m_musicOnSpr->setVisible(true);
            m_musicOffSpr->setVisible(false);
        }
    }
    if (m_info->type==ISetting_TaskTips) {
        string title = "";
        title=_lang("101344");
        m_TitleText->setString(title);
        string des = "";
        des=_lang("101342");
        m_2desText->setString(des);
        
        m_nodeType1->setVisible(false);
        m_nodeType2->setVisible(true);
        if (m_info->status1==0){
            CCCommonUtils::setSpriteGray(m_2touchBtnBg, true);
            CCCommonUtils::setSpriteGray(m_2touchBtn1, true);
            m_2touchBtn->setPositionX(-29);
            m_2touchBtn1->setPositionX(-29);
        }else{
            CCCommonUtils::setSpriteGray(m_2touchBtnBg, false);
            CCCommonUtils::setSpriteGray(m_2touchBtn1, false);
            m_2touchBtn->setPositionX(29);
            m_2touchBtn1->setPositionX(29);
        }
    }
    if (m_info->type==ISetting_Equip) {
        string title = "";
        title=_lang("113800");
        m_TitleText->setString(title);
        string des = "";
        des=_lang("113801");
        m_2desText->setString(des);
        
        m_nodeType1->setVisible(false);
        m_nodeType2->setVisible(true);
        if (m_info->status1==0){
            CCCommonUtils::setSpriteGray(m_2touchBtnBg, true);
            CCCommonUtils::setSpriteGray(m_2touchBtn1, true);
            m_2touchBtn->setPositionX(-29);
            m_2touchBtn1->setPositionX(-29);
        }else{
            CCCommonUtils::setSpriteGray(m_2touchBtnBg, false);
            CCCommonUtils::setSpriteGray(m_2touchBtn1, false);
            m_2touchBtn->setPositionX(29);
            m_2touchBtn1->setPositionX(29);
        }
    }
    if (m_info->type==ISetting_DaltonStyle) {
        string title = "";
        title=_lang("113852");
        m_TitleText->setString(title);
        string des = "";
        des=_lang("113853");
        m_2desText->setString(des);
        
        m_nodeType1->setVisible(false);
        m_nodeType2->setVisible(true);
        if (m_info->status1==0){
            CCCommonUtils::setSpriteGray(m_2touchBtnBg, true);
            CCCommonUtils::setSpriteGray(m_2touchBtn1, true);
            m_2touchBtn->setPositionX(-29);
            m_2touchBtn1->setPositionX(-29);
        }else{
            CCCommonUtils::setSpriteGray(m_2touchBtnBg, false);
            CCCommonUtils::setSpriteGray(m_2touchBtn1, false);
            m_2touchBtn->setPositionX(29);
            m_2touchBtn1->setPositionX(29);
        }
    }
    if (m_info->type==ISetting_Download) {
        string title = "";
        title="自动下载设置";
        m_TitleText->setString(title);
        string des = "";
        des="是否关闭游戏安装包自动下载";
        m_2desText->setString(des);
        
        m_nodeType1->setVisible(false);
        m_nodeType2->setVisible(true);
        if (m_info->status1==0){
            CCCommonUtils::setSpriteGray(m_2touchBtnBg, true);
            CCCommonUtils::setSpriteGray(m_2touchBtn1, true);
            m_2touchBtn->setPositionX(-29);
            m_2touchBtn1->setPositionX(-29);
        }else{
            CCCommonUtils::setSpriteGray(m_2touchBtnBg, false);
            CCCommonUtils::setSpriteGray(m_2touchBtn1, false);
            m_2touchBtn->setPositionX(29);
            m_2touchBtn1->setPositionX(29);
        }
    }
}

void InnerCell::showActivityDetail(){
    
}
