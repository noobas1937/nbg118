//
//  LanguageSettingView.cpp
//  IF
//
//  Created by lifangkai on 14-9-28.
//
//

#include "LanguageSettingView.h"
#include "MailPopUpView.h"
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
#include "YesNoDialog.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"

//#define MAX_LANGUAGE_COUNT 18
//const char* langName[MAX_LANGUAGE_COUNT] = {"English","Français","Deutsch","Pусский","한국어","ไทย","日本語","Português","Español","Türkçe","Indonesia","繁體中文","简体中文","Italiano","Polski","Nederlands","العربية","Română"};
//const char* langIcon[MAX_LANGUAGE_COUNT] = {"GbUs","FR","DE","RU","KR","TH","JP","BrPr","ES","TR","ID","TW","CN","IT","PL","NL","AE","RO"};
//const char* langType[MAX_LANGUAGE_COUNT] = {"en","fr","de","ru","ko","th","ja","pt","es","tr","id","zh_TW","zh_CN","it","pl","nl","ar","ro"};
//const char* langCode[MAX_LANGUAGE_COUNT] = {"102913","102917","102919","102920","102921","102935","102928","102918","102916","102936","102941","102915","102914","102922","102931","102930","103052","115619"};
//const int langStatus[MAX_LANGUAGE_COUNT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//


#define MAX_LANGUAGE_COUNT 3
const char* langName[MAX_LANGUAGE_COUNT] = {"English","繁體中文","简体中文"};
const char* langIcon[MAX_LANGUAGE_COUNT] = {"GbUs","TW","CN"};
const char* langType[MAX_LANGUAGE_COUNT] = {"en","zh_TW","zh_CN"};
const char* langCode[MAX_LANGUAGE_COUNT] = {"102913","102915","102914"};
const int langStatus[MAX_LANGUAGE_COUNT] = {0,0,0};//

string LanguageSettingView::getLanguageIconByType(string type){
    int i=0;
    while(i<MAX_LANGUAGE_COUNT){
        if(langType[i]==type){
            return langIcon[i];
        }
        i++;
    }
    return "";
}
LanguageSettingView* LanguageSettingView::create(){
    LanguageSettingView* ret = new LanguageSettingView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}

void LanguageSettingView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("102910"));
}

void LanguageSettingView::onExit(){
    PopupBaseView::onExit();
}
void LanguageSettingView::onTimer(float dt){
    time_t t = GlobalData::shared()->getWorldTime();
    string timestr =_lang("105259")+" "+CCCommonUtils::timeStampToUTCDate(t);
    m_timeText->setString(timestr);
}
LanguageSettingView::~LanguageSettingView()
{

}

bool LanguageSettingView::init(){
    
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
bool LanguageSettingView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupBG", CCScale9Sprite*, this->m_popupBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    return true;
}

SEL_CCControlHandler LanguageSettingView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}
void LanguageSettingView::setDataList(){
    m_data->removeAllObjects();
    //排序
    m_data->addObject(CCString::create("0"));
    m_data->addObject(CCString::create("1"));
//    int count = 10;
//    for (int i=0; i<count; i++) {
//        if(langStatus[i]==0){
//            m_data->addObject(CCString::create(CC_ITOA(i)));
//        }
//    }
    m_tabView->reloadData();
}

void LanguageSettingView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}
ssize_t LanguageSettingView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}
CCSize LanguageSettingView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    int height = 332+21;
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        height = 798.0+100;
//    }
    int indeY=-1;
    string str = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    if(str=="0"){
        int count = MAX_LANGUAGE_COUNT;

        for (int i=0; i<count; i++) {
            if(langStatus[i]==0){
                indeY++;
            }
        }
        indeY/=4;
        

        if(indeY>1){
            height += (indeY-1)*150;
        }

    }else if(str=="1"){
        CCDictionary* vipGroup = LocalController::shared()->DBXMLManager()->getGroupByKey("changeLanguage");
        CCDictElement* element;
        CCDICT_FOREACH(vipGroup, element){
            CCDictionary* objDict = _dict(element->getObject());
            string open = objDict->valueForKey("open")->getCString();
            if(open!="1"){
                indeY++;
            }
        }
        indeY/=2;
  
        if(indeY>1){
            height += (indeY-1)*120+120;
        }

    }
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, height*2.4);
    }
    
    return CCSize(640, height);
}

CCSize LanguageSettingView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 360*2.4);
    }
    return CCSize(640, 360);
}

CCTableViewCell* LanguageSettingView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    int count = m_data->count();
    if(idx >= count){
        return NULL;
    }
    LanguageSettingAndTranslationCell* cell = (LanguageSettingAndTranslationCell*)table->dequeueCell();
    string str = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    if(cell){
        cell->setData(str);
    }else{
        cell = LanguageSettingAndTranslationCell::create(str);
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        cell->setScale(2.4);
    }
    return cell;
}



//setting cell
bool LanguageSettingCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    //    CCLOG("test name %s", pMemberVariableName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name", Label*, this->m_name);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBG", CCScale9Sprite*, this->m_iconBG);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_HintNum", CCLabelIF*, this->m_HintNum);
    return true;
}

SEL_CCControlHandler LanguageSettingCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}

bool LanguageSettingCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(this->m_iconBG, pTouch)) {
        return true;
    }
    return false;
}
void LanguageSettingCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (fabs(pTouch->getLocation().x-m_touchPoint.x) > 30 || fabs(pTouch->getLocation().y-m_touchPoint.y) > 30)
    {
        return;
    }
    auto _dialog = YesNoDialog::show(_lang_1("113996",_lang(m_languageCode).c_str()),
                                     CCCallFunc::create(this, callfunc_selector(LanguageSettingCell::touchEvent)),
                                     0,
                                     true,
                                     CCCallFunc::create(this, callfunc_selector(LanguageSettingCell::cancelChangeLan)));
    _dialog->hideNoButton();
//    touchEvent();
}
void LanguageSettingCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

LanguageSettingCell* LanguageSettingCell::create(string str){
    LanguageSettingCell* ret = new LanguageSettingCell(str);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void LanguageSettingCell::onEnter(){
    CCTableViewCell::onEnter();

    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, true);
    
    
}

void LanguageSettingCell::onExit(){
    
    setTouchEnabled(false);
    CCTableViewCell::onExit();
}

bool LanguageSettingCell::init(){
    if(CCTableViewCell::init()){
        auto bg = CCBLoadFile("LanguageSettingCell", this, this);
        this->setContentSize(bg->getContentSize());
        this->m_name->setSystemFontSize(20);
        setData(m_str);
        return true;
    }
    return false;
}

void LanguageSettingCell::setData(string str)
{

    m_str = str;
    int index = atoi(m_str.c_str());
    string name = langName[index];
    string icon = langIcon[index];
    m_languageType = langType[index];
    m_languageCode = langCode[index];
    if(icon=="TW" && CCCommonUtils::checkTaiWanFlag()){
        icon = "CN";
    }
    icon+=".png";
    auto pic  = CCLoadSprite::createSprite(icon.c_str());
    if(pic->getContentSize().width > 111){
        pic->setScale(0.77);
    }
    this->m_picNode->removeAllChildren();
    this->m_picNode->addChild(pic);
    this->m_name->setString(name.c_str());
    std::string language = LocalController::shared()->getLanguageFileName();
    if (language.compare(m_languageType))
    {
        m_iconBG->setVisible(false);
    }else{
        m_iconBG->setVisible(true);
    }
}

void LanguageSettingCell::touchEvent()
{
    std::string language = LocalController::shared()->getLanguageFileName();
    if (language.compare(m_languageType))
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PREFERRED_LANGUAGE, m_languageType);
        CCUserDefault::sharedUserDefault()->setStringForKey("current-version-code","");
        CCUserDefault::sharedUserDefault()->flush();

        LocalController::shared()->setLang("");
        LocalController::shared()->init();
        LogoutCommand* cmd=new LogoutCommand();
        cmd->sendAndRelease();
        PopupViewController::getInstance()->removeAllPopupView();
        
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        ChatServiceCocos2dx::notifyChangeLanguage();
        ChatServiceCocos2dx::unInstallWin();
        
#endif

    }
    else
    {
        return;
    }

}
void LanguageSettingCell::cancelChangeLan()
{
    // do nothing
}

//commoncell
//setting cell
bool LanguageSettingAndTranslationCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    //    CCLOG("test name %s", pMemberVariableName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translationTitle", CCLabelIF*, this->m_translationTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translationDes", CCLabelIF*, this->m_translationDes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBG", CCScale9Sprite*, this->m_listBG);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_HintNum", CCLabelIF*, this->m_HintNum);
    return true;
}

SEL_CCControlHandler LanguageSettingAndTranslationCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}

bool LanguageSettingAndTranslationCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
//    if (isTouchInside(this->m_iconBG, pTouch)) {
//        return true;
//    }
    return false;
}
void LanguageSettingAndTranslationCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (fabs(pTouch->getLocation().x-m_touchPoint.x) > 30 || fabs(pTouch->getLocation().y-m_touchPoint.y) > 30)
    {
        return;
    }
    touchEvent();
}
void LanguageSettingAndTranslationCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

LanguageSettingAndTranslationCell* LanguageSettingAndTranslationCell::create(string str){
    LanguageSettingAndTranslationCell* ret = new LanguageSettingAndTranslationCell(str);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void LanguageSettingAndTranslationCell::onEnter(){
    CCTableViewCell::onEnter();

    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, true);
    
    
}

void LanguageSettingAndTranslationCell::onExit(){
    
    setTouchEnabled(false);
    CCTableViewCell::onExit();
}

bool LanguageSettingAndTranslationCell::init(){
    if(CCTableViewCell::init()){
        auto bg = CCBLoadFile("LanguageTranslationCell", this, this);
        this->setContentSize(bg->getContentSize());
        m_listData= CCArray::create();
        setData(m_str);
        return true;
    }
    return false;
}

void LanguageSettingAndTranslationCell::setData(string str)
{
    
    m_str = str;
    this->m_listNode->removeAllChildren();
    m_totalNode->setPositionY(109.0);
    m_listData->removeAllObjects();
//    m_listBG->setContentSize(CCSize(624, 332));
    int indexY =0;
    int indexX = 0;
    if(m_str=="0"){
        m_translationTitle->setString(_lang("102910"));
        m_translationDes->setVisible(false);
        m_listNode->setPosition(-291, 82);
        int count = MAX_LANGUAGE_COUNT;
        for (int i=0; i<count; i++) {
            if(langStatus[i]==0){
                m_listData->addObject(CCString::create(CC_ITOA(i)));
            }
        }
        CCObject* obj=NULL;
        int j=0;
        CCARRAY_FOREACH(m_listData, obj){
            string str = dynamic_cast<CCString*>(obj)->getCString();
            indexY = j/4;
            indexX = j%4;
            LanguageSettingCell* cell = LanguageSettingCell::create(str);
            m_listNode->addChild(cell);
            cell->setPosition(indexX*153, -indexY*150);
            j++;
        }
        if(indexY>1){
            m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, 332+(indexY-1)*150));
            CCSize size = m_listBG->getContentSize();
            m_totalNode->setPositionY(109.0+(indexY-1)*150);
        }
        
    }else if(m_str=="1"){
        m_translationDes->setVisible(true);
       
        m_translationDes->setString(_lang("102912"));
        m_translationTitle->setString(_lang("102911"));
         m_listNode->setPosition(-243, m_translationDes->getPositionY()-m_translationDes->getContentSize().height-120);
        CCDictionary* vipGroup = LocalController::shared()->DBXMLManager()->getGroupByKey("changeLanguage");
        CCDictElement* element;
        CCDICT_FOREACH(vipGroup, element){
            CCDictionary* objDict = _dict(element->getObject());
            string open = objDict->valueForKey("open")->getCString();
            if(open!="1"){
                m_listData->addObject(objDict);
            }
        }
        CCObject* obj=NULL;
        int j=0;
        CCARRAY_FOREACH(m_listData, obj){
            string str = "";
            indexY = j/2;
            indexX = j%2;
            LanguageTranslationCell* cell = LanguageTranslationCell::create(dynamic_cast<CCDictionary*>(obj));
            m_listNode->addChild(cell);
            cell->setPosition(indexX*379, -indexY*120);
            j++;
        }
        if(indexY>1){
            m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, m_listBG->getContentSize().height+(indexY-1)*120+120));
            m_totalNode->setPositionY(109.0+(indexY-1)*120+120);
            CCSize size = m_listBG->getContentSize();
            int a = 0;
        }
    }
}

void LanguageSettingAndTranslationCell::touchEvent()
{
    
}

// translation setting cell
bool LanguageTranslationCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    //    CCLOG("test name %s", pMemberVariableName);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", Label*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtnOpenBg", CCSprite*, this->m_touchBtnOpenBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtnCloseBg", CCSprite*, this->m_touchBtnCloseBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtn", CCSprite*, this->m_touchBtn);
    return true;
}

SEL_CCControlHandler LanguageTranslationCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}

bool LanguageTranslationCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(this->m_touchBtnCloseBg, pTouch)) {
        return true;
    }
    return false;
}
void LanguageTranslationCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (fabs(pTouch->getLocation().x-m_touchPoint.x) > 30 || fabs(pTouch->getLocation().y-m_touchPoint.y) > 30)
    {
        return;
    }
    touchEvent();
}
void LanguageTranslationCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

LanguageTranslationCell* LanguageTranslationCell::create(CCDictionary* dict){
    LanguageTranslationCell* ret = new LanguageTranslationCell(dict);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void LanguageTranslationCell::onEnter(){
    CCTableViewCell::onEnter();
   
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, true);
    
    
}

void LanguageTranslationCell::onExit(){
    setTouchEnabled(false);
    CCTableViewCell::onExit();
}

bool LanguageTranslationCell::init(){
    if(CCTableViewCell::init()){
        auto bg = CCBLoadFile("LanguageTranslationSettingCell", this, this);
        this->setContentSize(bg->getContentSize());
        setData(m_itemDict);
        return true;
    }
    return false;
}

void LanguageTranslationCell::setData(CCDictionary* dict)
{
    
    m_itemDict = dict;
    string name = m_itemDict->valueForKey("name")->getCString();
    string type = m_itemDict->valueForKey("type")->getCString();
    m_nameText->setString(_lang(name.c_str()).c_str());
    string typeCacheKey = "LanguageTranslation.set."+type;
    string typeCache = CCUserDefault::sharedUserDefault()->getStringForKey(typeCacheKey.c_str());
    if (typeCache!="")
    {
        m_touchBtnCloseBg->setVisible(true);
        m_touchBtnOpenBg->setVisible(false);
        m_touchBtn->setPositionX(2);
    }else{
        m_touchBtnCloseBg->setVisible(false);
        m_touchBtnOpenBg->setVisible(true);
        m_touchBtn->setPositionX(62.5);
    }
}

void LanguageTranslationCell::touchEvent()
{
    string type = m_itemDict->valueForKey("type")->getCString();
    string typeCacheKey = "LanguageTranslation.set."+type;
    string typeCache = CCUserDefault::sharedUserDefault()->getStringForKey(typeCacheKey.c_str());
    if(typeCache!=""){
        CCUserDefault::sharedUserDefault()->setStringForKey(typeCacheKey.c_str(),"");
        m_touchBtnCloseBg->setVisible(false);
        m_touchBtnOpenBg->setVisible(true);
        m_touchBtn->setPositionX(62.5);
    }else{
        CCUserDefault::sharedUserDefault()->setStringForKey(typeCacheKey.c_str(),"1");
        m_touchBtnCloseBg->setVisible(true);
        m_touchBtnOpenBg->setVisible(false);
        m_touchBtn->setPositionX(2);
    }
    ChatController::getInstance()->setDisableTranslateLang();
    CCUserDefault::sharedUserDefault()->flush();
}