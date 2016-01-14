//
//  MailDialogView.cpp
//  IF
//
//  Created by lifangkai on 14-9-18.
//
//

#include "MailDialogView.h"
#include "SceneController.h"
#include "PopupViewController.h"
#include "CCTypesExt.h"
#include "CCCommonUtils.h"
#include "ChatController.h"
#include "CCSafeNotificationCenter.h"
#include "CCLabelIF.h"
#include "ChatCell.h"
#include "YesNoDialog.h"
#include "ChatLockCommand.h"
#include "ChatUnLockCommand.h"
#include "MailWritePopUpView.h"
#include "GetUserInfoCommand.h"
#include "../../Ext/CCDevice.h"
#include "RoleInfoView.h"
#include "UIComponent.h"
#include "CCEditText.h"
#include "InvitesAllianceCommand.h"
#include "InputFieldMultiLine.h"
#include "JoinAllianceView.h"
#include "AlertAddAllianceView.h"
#include "CheckAllianceInfoView.h"
#include "MailController.h"
#include "MailDeleteCommand.h"
#include "MailReadCommand.h"
#include "PlayerInfoBtnPopUpView.h"
#include "Utf8Utils.h"
#include "ChatServiceCocos2dx.h"
#include "SceneContainer.h"

#define SHOW_COVER_NATIVE_INPUT          0

static const int basedh = 57;
MailDialogView* MailDialogView::create(MailInfo* info ){
    MailDialogView* ret = new MailDialogView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MailDialogView::~MailDialogView(){
    if(m_mailInfo&&m_mailInfo->dialogs->count()==0&&m_mailInfo->preSaveContent!=""){
        map<string, MailInfo*>::iterator it;
        it = GlobalData::shared()->mailList.find(m_mailInfo->uid);
        if (GlobalData::shared()->mailList.end() != it) {
            MailInfo* mail = it->second;
            CC_SAFE_RELEASE(mail);
            GlobalData::shared()->mailList.erase(it);
        }

       
    }
}
bool MailDialogView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    auto tmpCCB = CCBLoadFile("MailDialogView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    int oldBgHeight = m_buildBG->getContentSize().height;
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    tmpCCB->setContentSize(winsize);
    if (CCCommonUtils::isIosAndroidPad())
    {
        winsize = CCSize(640, 852);
    }
    m_buildBG->setContentSize(CCSize(winsize.width+2,winsize.height));
    m_bg->setContentSize(CCSize(winsize.width+2,winsize.height));
//    setContentSize(winsize);
    
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
     m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    CCCommonUtils::setButtonTitle(m_sendBtn, _lang("105302").c_str());
    //CCCommonUtils::setButtonTitleColor(m_sendBtn, ccc3(253, 187, 9));

    
    //    auto sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    m_editBox = InputFieldMultiLine::create(CCSizeMake(460,60),"UI_Alliance_TextBox.png",24);
    m_editBox->setAnchorPoint(ccp(0,0));
    //    m_editBox->setFontSize(20);
    //m_editBox->setPlaceHolder("");
   // m_editBox->setMaxChars(140);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
//	m_editBox->setShowLineNum(3);
//	m_editBox->setLineNumber(3);
//#else
	m_editBox->setLineNumber(2);
//#endif
    m_editBox->setFontColor(ccc3(0, 0, 0));

    m_editBox->setPosition(ccp(0,0));
    m_editBox->setSwallowsTouches(true);
    m_editBox->setMoveFlag(true);
    m_editBox->setTouchPriority(1);
    m_editNode->addChild(m_editBox);
    m_data = CCArray::create();
    m_contentText = CCLabelIFTTF::create();
    m_contentText->setColor(ccc3(47, 22, 0));
    m_contentText->setFontSize(24);
    m_contentText->setDimensions(CCSize(305, 0));
//    this->addChild(m_contentText, 3200);
// 
//    m_contentText->setPosition(ccp(320, 400));
//    m_contentText->setString("000000000gfgdfddfdgfjdlgfjkdlfgjdlfgjdlfgjkdlfgjkdlgjkdlgjdlgjdlgjdlgjdlgjdlgjdlgjdlgjdlgjdldjgfjdfgjdfkj哈哈觉得好久好久");
  //  m_editBox->setText(ChatController::getInstance()->m_tmpSaveMsg.c_str());
    

    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);

    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    std::string title = "";
    if(!m_mailInfo->alliance.empty()){
        title.append("("+m_mailInfo->alliance+")");
    }
    title.append(m_mailInfo->fromName);
    this->m_titleText->setString(title);
    m_titleText->setVisible(false);
    m_unSaveBtn->removeFromParent();
    m_addSaveBtn->removeFromParent();
    m_addSaveBtn->setPosition(600, -40);
    m_unSaveBtn->setPosition(600, -40);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_addSaveBtn->setPosition(600, 0);
        m_unSaveBtn->setPosition(600, 0);
    }
        refresh(NULL);

    return true;
}
void MailDialogView::refresh(CCObject* obj){
    if(m_mailInfo->save ==0){
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
    setData();
    bool ishasChild = false;
    CCPoint pos;
    if(m_tabView->getContainer()->getChildrenCount()==0){
        ishasChild = true;
    }else{
        pos = m_tabView->getContentOffset();
    }
    //    m_data->addObjectsFromArray(arr);
    m_tabView->reloadData();
//    if(ishasChild){
//
//    }else{
        CCPoint minPt = m_tabView->minContainerOffset();
        CCPoint maxPt = m_tabView->maxContainerOffset();

    if (pos.y > maxPt.y) {
        pos.y = maxPt.y;
    }
    
    if (pos.y < minPt.y) {
        pos.y = minPt.y;
    }
    if(m_editBox->getIsEditing()){
        pos.y = maxPt.y;
    }
        m_tabView->setContentOffset(pos);
  //  }
//    m_tabView->reloadData();
}
void MailDialogView::readDialogContent(){
    string content = m_editBox->getText();
    m_mailInfo->preSaveContent = content;
    if(m_mailInfo->unreadDialogNum>0){
        MailDialogReadCommand* cmd = new MailDialogReadCommand(m_mailInfo->fromUid, -1, m_mailInfo->uid);
        cmd->sendAndRelease();
        if(m_mailInfo->type == MAIL_MOD_SEND || m_mailInfo->type == MAIL_MOD_PERSONAL){
            GlobalData::shared()->mailCountObj.modR -= m_mailInfo->unreadDialogNum;
            if(GlobalData::shared()->mailCountObj.modR < 0){
                GlobalData::shared()->mailCountObj.modR = 0;
            }
        }else{
            GlobalData::shared()->mailCountObj.perR -=m_mailInfo->unreadDialogNum;
        }
        m_mailInfo->unreadDialogNum = 0;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
}
#define LANGUAGE_COUNT 13
const char* langType1[LANGUAGE_COUNT] = {"en","fr","de","ru","ko","th","ja","pt","es","tr","id","zh-Hant","zh-Hans"};
const char* langCode1[LANGUAGE_COUNT] = {"102913","102917","102919","102920","102921","102935","102928","102918","102916","102936","102941","102915","102914"};

string MailDialogView::getModLangeage(string modLangAbbreviation)
{
    for(int i=0; i<LANGUAGE_COUNT; i++)
    {
        if(langType1[i] == modLangAbbreviation)
        {
            return _lang(langCode1[i]);
        }
    }
    return _lang(langCode1[0]);
}

void MailDialogView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("105513"));
    std::string title = "";
    if(!m_mailInfo->alliance.empty()){
        title.append("("+m_mailInfo->alliance+")");
    }
    title.append(m_mailInfo->fromName);
    setTitleName(title);

    m_editBox->setText(m_mailInfo->preSaveContent);

    if((m_mailInfo->type == MAIL_MOD_PERSONAL || m_mailInfo->type == MAIL_MOD_SEND) && !GlobalData::shared()->playerInfo.isMOD()){
        std::string str = std::string("[Mod] ");
        if(!m_mailInfo->alliance.empty()){
            str.append("("+m_mailInfo->alliance+")");
        }
        str.append(m_mailInfo->fromName);
        str.append(" [");
        str.append(getModLangeage(m_mailInfo->modLanguage));
        str.append("]");
        setTitleName(str);
    }

    UIComponent::getInstance()->m_inUiTitle->addChild(m_unSaveBtn);
    UIComponent::getInstance()->m_inUiTitle->addChild(m_addSaveBtn);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_unSaveBtn->setScale(2);
        m_addSaveBtn->setScale(2);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailDialogView::refresh), MAIL_PERSON_CHAT_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailDialogView::refresh), MAIL_LIST_DELETE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailDialogView::refreshAddList), MAIL_LIST_ADD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailDialogView::refresh), INPUTFIELD_OPEN_EVENT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailDialogView::refresh), INPUTFIELD_CLOSE_EVENT, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -2, false);
	
	CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailDialogView::popupCloseEvent), MSG_POPUP_CLOSE_EVENT, NULL);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	scheduleOnce(schedule_selector(MailDialogView::showNativeTextInput), 0.01f);
	onTextChangedHandler(0.0f);
#endif
}

void MailDialogView::showNativeTextInput(float time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	ChatServiceCocos2dx::setSendMessageListener(this);
	
	CCLOG("MailDialogView::showNativeTextInput");
	ChatServiceCocos2dx::showChatInputView();
	ChatServiceCocos2dx::enableChatInputView();
#endif
}

void MailDialogView::popupCloseEvent(CCObject *params)
{
	int c = PopupViewController::getInstance()->getCurrViewCount();
	CCLOG("MailDialogView::popupCloseEvent getCurrViewCount: %d", c);
	
	if(c == 1)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
		ChatServiceCocos2dx::showChatInputView();
		ChatServiceCocos2dx::enableChatInputView();
#endif
	}
}

void MailDialogView::setHeightFromNative(int height, int usableHeightSansKeyboard)
{
	return;
}

void MailDialogView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_PERSON_CHAT_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_DELETE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_ADD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, INPUTFIELD_OPEN_EVENT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, INPUTFIELD_CLOSE_EVENT);
	setTouchEnabled(false);
	CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_CLOSE_EVENT);
	
    m_unSaveBtn->removeFromParent();
    m_addSaveBtn->removeFromParent();
	readDialogContent();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	ChatServiceCocos2dx::setSendMessageListener(NULL);
//	ChatController::getInstance()->m_tmpSaveMsg = ChatServiceCocos2dx::getChatInputText();
	
	ChatServiceCocos2dx::disableChatInputView();
	ChatServiceCocos2dx::hideChatInputView();
#endif

    CCNode::onExit();
}

bool MailDialogView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_tabView->setBounceable(true);
    if(m_isLoadMore)
        return true;
    else
        return false;
}
void MailDialogView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(!m_isLoadMore){
        return;
    }
    float offsetY = m_tabView->getContentOffset().y;
    m_currMinOffsetY = m_tabView->minContainerOffset().y;
    if(offsetY<=m_currMinOffsetY){
        float y = m_currMinOffsetY+(pTouch->getLocation().y - pTouch->getStartLocation().y)/5;
        m_tabView->setContentOffset(ccp(0,y));
    }
    int dy = m_currMinOffsetY-30;
    if(m_isLoadMore && offsetY<dy){
        auto cell = dynamic_cast<MailDialogCell*>(m_tabView->cellAtIndex(0));
        if(cell==NULL){
            return;
        }
        CCLabelIF* txt = (CCLabelIF*)cell->getChildByTag(10001);
        txt->setVisible(true);
        if(offsetY<dy)
            txt->setString(_lang("105502").c_str());
        else
            txt->setString(_lang("115189").c_str());
    }
    return;
    
}
void MailDialogView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_isLoadMore){
        m_currMinOffsetY = m_tabView->minContainerOffset().y;
        m_currOffsetY = m_tabView->getContentOffset().y;
        int dy = m_currMinOffsetY-30;
        if(m_currOffsetY <dy){
            addLoadingAni();
            m_tabView->setBounceable(false);
            this->scheduleOnce(schedule_selector(MailDialogView::sendReloadMoreMail),0.0);
        }
    }
}
void MailDialogView::sendReloadMoreMailEvent(CCObject* obj)
{
    sendReloadMoreMail(0);
}
void MailDialogView::sendReloadMoreMail(float _time){
    int count = m_mailInfo->dialogs->count();
    addLoadingAni();
    MailDialogReadBatchCommand* command = new MailDialogReadBatchCommand(m_mailInfo->fromUid,m_mailInfo->uid, count,10);
    command->sendAndRelease();
    //MailController::getInstance()->reloadMailMore(1000,realcount,20);
}
void MailDialogView::removeLoadingAni(){
    if(m_loadingIcon){
        m_infoList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
}
void MailDialogView::addLoadingAni(){
    if(m_loadingIcon){
        m_infoList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_infoList->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_infoList->addChild(m_loadingIcon,1000000);
}
void MailDialogView::closeInputRefeshList(){

}
void MailDialogView::refreshAddList(cocos2d::CCObject *obj){

    setData();
    removeLoadingAni();
    int add = dynamic_cast<CCInteger*>(obj)->getValue();
    int MinOffsetY = m_tabView->minContainerOffset().y;
    CCPoint pos = m_tabView->getContentOffset();
    int MinOffsetY1 = pos.y;
    if(add==0){
      //  m_tabView->reloadData();
       // m_tabView->setContentOffset(ccp(0,0));
        return;
    }
    m_tabView->reloadData();
    CCPoint minPt = m_tabView->minContainerOffset();
    CCPoint maxPt = m_tabView->maxContainerOffset();

    if (MinOffsetY1 > maxPt.y) {
        MinOffsetY1 = maxPt.y;
    }
    
    if (MinOffsetY1 < minPt.y) {
        MinOffsetY1 = minPt.y;
    }
    
    m_tabView->setContentOffset(ccp(0,MinOffsetY1));
   // m_tabView->setContentOffsetInDuration(ccp(0,MinOffsetY1 - 40), 0.7);
    
}
SEL_CCControlHandler MailDialogView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPostBtnClick", MailDialogView::onPostBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MailDialogView::onAddSaveClick);
    return NULL;
}

bool MailDialogView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, this->m_editNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBg", CCScale9Sprite*, this->m_editBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchEditNode", CCNode*, m_touchEditNode);
    return false;
}
void MailDialogView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_mailInfo->dialogs->count()==0)
        return;
    if(m_mailInfo->save ==1){
        MailDialogSaveCommand *cmd = new MailDialogSaveCommand(m_mailInfo->fromUid, 0, -1, m_mailInfo->uid);
        cmd->sendAndRelease();
        m_mailInfo->save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{

        MailDialogSaveCommand *cmd = new MailDialogSaveCommand(m_mailInfo->fromUid, 1, -1, m_mailInfo->uid);
        cmd->sendAndRelease();
        m_mailInfo->save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
    
}
void MailDialogView::onPostBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    sendMessage(m_editBox->getText());
	
    m_editBox->setText("");
    m_editBox->closeIME();
    m_editBox->openIME();

}

void MailDialogView::onTextChanged(string msg)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	scheduleOnce(schedule_selector(MailDialogView::onTextChangedHandler), 0.01f);
#endif
}

void MailDialogView::onTextChangedHandler(float time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	m_editBox->setText(ChatServiceCocos2dx::getChatInputText().c_str());
#endif
}

void MailDialogView::sendMessage(string msg)
{
	CCLOG("MailDialogView::sendMessage");
	string content = msg;
	string title = content;
	int len = Utf8Utils::strlen(title.c_str());
	if(len>10){
		title =Utf8Utils::utf8sub(title.c_str(),0,10);
		title.append("...");
	}
	CCLOG("title:%s", title.c_str());
    
    if (m_mailInfo->type!=CHAT_ROOM) {
        string allianceUid = "";
        string fromName = "";
        if(m_mailInfo->fromUid==GlobalData::shared()->playerInfo.uid){// all alliancemail
            allianceUid =GlobalData::shared()->playerInfo.allianceInfo.uid;
            fromName = _lang("105564");
        }else{
            fromName= m_mailInfo->fromName;
        }
        if(m_mailInfo->dialogs->count()==0){
            MailController::getInstance()->sendMail(fromName, title, content,allianceUid,m_mailInfo->uid,true,m_mailInfo->type);
        }else{
            MailController::getInstance()->sendMail(fromName, title, content,allianceUid,m_mailInfo->uid,false,m_mailInfo->type);
        }
        m_mailInfo->preSaveContent="";
    }
    else
    {
        if(m_mailInfo->crGroupId!="")
            MailController::getInstance()->sendChatRoomMsg(content, m_mailInfo->crGroupId);
    }
}

void MailDialogView::onBackPressed()
{
	CCLOG("MailDialogView::onBackPressed");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	scheduleOnce(schedule_selector(MailDialogView::onBackPressedHandler), 0.01f);
#endif
}

void MailDialogView::onBackPressedHandler(float time)
{
	CCLOG("MailDialogView::onBackPressedHandler");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	SceneContainer* sceneContainer = dynamic_cast<SceneContainer*>(CCDirector::sharedDirector()->getRunningScene());
	sceneContainer->keyBackClicked();
#endif
}

void MailDialogView::setData(){
    m_data->removeAllObjects();
    if(m_mailInfo->dialogs->count()<m_mailInfo->dialogNum){
        m_isLoadMore = true;
    }else{
        m_isLoadMore = false;
    }
    if(m_isLoadMore){
        MailDialogInfo* info = MailDialogInfo::create();
        info->type = 3;
        m_data->addObject(info);
    }
    CCObject* obj;
    time_t time =0;
    CCARRAY_FOREACH(m_mailInfo->dialogs, obj){
        MailDialogInfo* tempInfo = dynamic_cast<MailDialogInfo*>(obj);
        if(time==0){
            time=tempInfo->createTime;
            tempInfo->isShowTime = true;
        }else if(tempInfo->createTime>time+5*60){
            tempInfo->isShowTime = true;
            time =tempInfo->createTime;
        }else{
            tempInfo->isShowTime = false;
        }
        m_data->addObject(obj);
    }

}

void MailDialogView::scrollViewDidScroll(CCScrollView* view)
{

}

void MailDialogView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    ChatCell* tmpCell = dynamic_cast<ChatCell*>(cell);
    if (tmpCell) {
        //        tmpCell->cellTouchEnded(m_tabView->m_pTouchedCell);
    }
}
CCSize MailDialogView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(m_isLoadMore&&idx==0){
        return CCSize(640,50);
    }
    MailDialogInfo* dialogInfo = dynamic_cast<MailDialogInfo*>(m_data->objectAtIndex(idx));
    int height = 0;
    if(dialogInfo->isShowTime){
        height = 150;
    }else{
        height = 100;
    }
    height+=30;
    std::string showDialog = dialogInfo->contents;
    m_contentText->setString(showDialog.c_str());
    int dh = m_contentText->getContentSize().height*m_contentText->getOriginScaleY();
    if(!dialogInfo->translationMsg.empty()){
         showDialog = dialogInfo->translationMsg;
         m_contentText->setString(showDialog.c_str());
        int tmpDH = m_contentText->getContentSize().height*m_contentText->getOriginScaleY();
        dh = MAX(dh, tmpDH);
    }
    if(dialogInfo->type!=0 && dialogInfo->type!=MAIL_MOD_PERSONAL){
        dh += 50;
    }
    CCLOG("dh:%d",dh);
    CCLOG("hg:%f",m_contentText->getContentSize().height);
    CCLOG("content:%s",dialogInfo->contents.c_str());
    float bgDh = dh+24*2;
    if(bgDh>82){
        return CCSize(640,height+(bgDh-82));
    }
//    if(dh>basedh){
//        return CCSize(640,height+(dh-basedh));
//    }
    return CCSize(640,height);
}

cocos2d::CCSize MailDialogView::cellSizeForTable(CCTableView *table)
{
    return CCSize(580, 980);
}

CCTableViewCell* MailDialogView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    
//    if (type == CHAT_COUNTRY && idx >= ChatController::getInstance()->m_chatCountryPool.size()) {
//        return NULL;
//    }
//    else if ( type == CHAT_ALLIANCE && idx >= ChatController::getInstance()->m_chatAlliancePool.size()) {
//        return NULL;
//    }
    if(idx>=m_data->count()){
     return NULL;
    }
    MailDialogInfo* dialogInfo = dynamic_cast<MailDialogInfo*>(m_data->objectAtIndex(idx));
    MailDialogCell* cell = (MailDialogCell*)table->dequeueCell();
    if(cell){
        cell->setData(dialogInfo,m_mailInfo,idx);
    }else{
        cell = MailDialogCell::create(dialogInfo,m_mailInfo,idx);
    }
    return cell;
}

ssize_t MailDialogView::numberOfCellsInTableView(CCTableView *table){
    int cellNum = 0;
    cellNum = m_data->count();
//    if (type == CHAT_COUNTRY) {
//        m_cellNum = ceil(ChatController::getInstance()->m_chatCountryPool.size() / numPerRow);
//    }
//    else {
//        m_cellNum = ceil(ChatController::getInstance()->m_chatAlliancePool.size() / numPerRow);
//    }
    return cellNum;
}


MailDialogCell* MailDialogCell::create(MailDialogInfo* dialogInfo,MailInfo* mailInfo,int index){
    MailDialogCell* ret = new MailDialogCell(dialogInfo,mailInfo,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailDialogCell::init()
{
    bool ret = true;
    CCBLoadFile("MailDialogCell",this,this);
    this->setContentSize(CCSizeMake(640, 130));
    m_tipLoadingText = CCLabelIF::create();
    m_tipLoadingText->setFontSize(22);
    m_tipLoadingText->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    m_tipLoadingText->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_tipLoadingText->setColor({117,62,15});
    m_tipLoadingText->setString(_lang("105502").c_str());
    this->addChild(m_tipLoadingText,10001,10001);
    m_tipLoadingText->setPosition(290, 20);
    m_tipLoadingText->setVisible(false);;
    
    m_headImgNode = HFHeadImgNode::create();
    
    setData(m_dialogInfo,m_mailInfo, m_idx);
    return ret;
}
void MailDialogCell::setCellStatus(bool isHint){
    if(m_dialogInfo->type==0 || m_dialogInfo->type==MAIL_MOD_PERSONAL){
        if(!isHint){
            m_cellBGSprArrow1->setVisible(true);
            m_cellBGSprArrow11->setVisible(false);
            m_cellBGSpr1->setVisible(true);
            m_cellBGSpr11->setVisible(false);
        }else{
            m_cellBGSprArrow1->setVisible(false);
            m_cellBGSprArrow11->setVisible(true);
            m_cellBGSpr1->setVisible(false);
            m_cellBGSpr11->setVisible(true);
        }
    }else{
        if(!isHint){
            m_cellBGSprArrow2->setVisible(true);
            m_cellBGSprArrow21->setVisible(false);
            m_cellBGSpr2->setVisible(true);
            m_cellBGSpr21->setVisible(false);
        }else{
            m_cellBGSprArrow2->setVisible(false);
            m_cellBGSprArrow21->setVisible(true);
            m_cellBGSpr2->setVisible(false);
            m_cellBGSpr21->setVisible(true);
        }
    }
    if(m_dialogInfo->isShowTime){
        m_timeNode->setVisible(true);
    }else{
        m_timeNode->setVisible(false);
    }

}
void MailDialogCell::setData(MailDialogInfo* dialogInfo,MailInfo* mailInfo,int index)
{
    m_dialogInfo=dialogInfo;
    m_mailInfo = mailInfo;
    m_idx = index;
    m_cellnode1->setVisible(false);
    m_cellnode2->setVisible(false);
    m_timeNode->setVisible(false);
    
    if(m_dialogInfo->type==3){
        m_tipLoadingText->setVisible(true);
        return;
    }
    m_tipLoadingText->setVisible(false);
    setCellStatus(false);
    //将时间转换为day hour min
    auto dt = GlobalData::shared()->getWorldTime()-GlobalData::shared()->changeTime(m_dialogInfo->createTime);
    string timestr = "";
    int timedt = 0;
    
    if (dt>=24*3600*2) {
        time_t timeT = m_dialogInfo->createTime;
        timestr = CCCommonUtils::timeStampToMD(timeT);
    }
    else {
        if(dt>=24*60*60){
            timedt =dt/(24*60*60);
            timestr = CC_ITOA(timedt);
            timestr.append(_lang("105592"));
        }else if(dt>=60*60){
            timedt =dt/(60*60);
            timestr = CC_ITOA(timedt);
            timestr.append(_lang("105591"));
        }else if(dt>=60){
            timedt =dt/60;
            timestr = CC_ITOA(timedt);
            timestr.append(_lang("105590"));
        }else{
            timestr ="1";
            timestr.append(_lang("105590"));
        }
        timestr.append(" ");
        timestr.append(_lang("105593"));
    }
    timestr = CCCommonUtils::timeStampToDate(m_dialogInfo->createTime);
    int dh=0;
    m_contentText1->setDimensions(CCSize(305, 0));
    m_contentText2->setDimensions(CCSize(305, 0));
    int chatRoomSelfFlag=0;
    if (m_dialogInfo->type==CHAT_ROOM) {
        if (m_dialogInfo->uid!="") {
            chatRoomSelfFlag=m_dialogInfo->uid==GlobalData::shared()->playerInfo.uid?1:-1;
        }
    }
    if(m_dialogInfo->type==0 || m_dialogInfo->type==MAIL_MOD_PERSONAL || chatRoomSelfFlag==1){//自己发的内容
        m_cellnode1->setVisible(true);
        m_translateNode2->setVisible(false);
        string mpic = GlobalData::shared()->playerInfo.pic;
        mpic = mpic+".png";
        auto pic = CCLoadSprite::createSprite(mpic.c_str());
        float scale = 75/pic->getContentSize().width;
        scale= scale>1?1.0:scale;
        pic->setScale(scale);
        m_headNode1->removeAllChildren();
        m_headNode1->addChild(pic);
        if (GlobalData::shared()->playerInfo.isUseCustomPic()) {
            m_headImgNode->initHeadImgUrl2(m_headNode1, GlobalData::shared()->playerInfo.getCustomPicUrl(), 1.0f, 75, true);
        }
        m_contentText1->setString(m_dialogInfo->contents.c_str());


        int wd = m_contentText1->getContentSize().width*m_contentText1->getOriginScaleX();
        CCLOG("hg:%s",m_dialogInfo->contents.c_str());
        CCLOG("hg:%d",wd);
        int wd1 = 0;
        if(wd<305){
            wd1 = wd+18*2;
           // m_contentText1->cocos2d::CCLabelTTF::setDimensions(CCSize(wd, 0));
        }else{
            wd1 = 352;
            m_contentText1->setDimensions(CCSize(305, 0));
        }
        dh = m_contentText1->getContentSize().height*m_contentText1->getOriginScaleY();
        float bgDh = dh+24*2;
        m_cellBGSpr1->setContentSize(CCSize(wd1, bgDh));
        m_cellBGSpr11->setContentSize(CCSize(wd1, bgDh));
        float bgY = m_cellBGSpr1->getPositionY();
        m_contentText1->setPositionY(bgY-bgDh/2);
        if(bgDh>82){
            this->m_totalNode->setPositionY(bgDh-82);
        }else{
            this->m_totalNode->setPositionY(0);
        }
        if (dh>basedh) {
//            m_cellBGSpr1->setContentSize(CCSize(wd1, 72+(dh-basedh)));
//            m_cellBGSpr11->setContentSize(CCSize(wd1, 72+(dh-basedh)));
//            m_contentText1->setPositionY(-(dh-basedh)/2);
//           this->m_totalNode->setPositionY((dh-basedh)+5);
        }else{
//            m_cellBGSpr1->setContentSize(CCSize(wd1, 72));
//            m_cellBGSpr11->setContentSize(CCSize(wd1, 72));
//              this->m_totalNode->setPositionY(0);
 //           m_contentText1->setPositionY(0);
        }
        
    }else{
        m_cellnode2->setVisible(true);
        string mpic = m_mailInfo->pic;
        int picVer=m_mailInfo->picVer;
        
        if (chatRoomSelfFlag==-1) {
            CCDictionary* userInfoDic=dynamic_cast<CCDictionary*>(ChatController::getInstance()->m_chatUserInfoDic->objectForKey(m_dialogInfo->uid));
            if (userInfoDic!=NULL) {
                mpic=dynamic_cast<CCString*>(userInfoDic->objectForKey("headPic"))->getCString();
                picVer=dynamic_cast<CCInteger*>(userInfoDic->objectForKey("headPicVer"))->getValue();
            }
        }
        mpic = mpic+".png";
        auto pic = CCLoadSprite::createSprite(mpic.c_str());
        float scale = 75/pic->getContentSize().width;
        scale= scale>1?1.0:scale;
        pic->setScale(scale);
        m_headNode2->removeAllChildren();
        m_headNode2->addChild(pic);
        if (CCCommonUtils::isUseCustomPic(picVer)) {
            m_headImgNode->initHeadImgUrl2(m_headNode2, CCCommonUtils::getCustomPicUrl(m_mailInfo->fromUid, m_mailInfo->picVer), 1.0f, 75, true);
        }
        
        string tempCont = "(To All)Hi all!\nThis mail is specially for those of you who are doing that event. I find it kinda annoying but i think i have some idea that can help gather some serious points in that gathering food part. Send your troops gather gold before that parts of event starts. it takes 24 h to gather 74 gold.  so if you gonna send one cap of troops 20 hours before gathering part starts you should get 1 .9 M points 2 hours after it started. Hope that info can be helpful. Btw itss good to use a gathering scroll to boost gathering speed!";
//        tempCont = " Send your troops gather gold before that parts of event starts. it takes 24 h to gather 74 gold.   Send";
        int lblH = 0;
        bool flag = checkSameLangBySystem(m_dialogInfo->originalLang);//本地语言和和 源语言一致，也不翻译
        //处理原文/译文  自己不翻译 /*&& info.originalLang != "en"cocos2d::extension::CCDevice::getLanguage()*/
        if(!m_dialogInfo->translationMsg.empty()){
            m_contentText2->setString(m_dialogInfo->translationMsg.c_str());
            m_contentText1->setString(m_dialogInfo->contents.c_str());
            lblH= MAX(m_contentText2->getContentSize().height*m_contentText2->getOriginScaleY(), m_contentText1->getContentSize().height*m_contentText1->getOriginScaleY()) ;
        }else{
            m_contentText2->setString(m_dialogInfo->contents.c_str());
            lblH= m_contentText2->getContentSize().height*m_contentText2->getOriginScaleY();
        }
        if(!flag && GlobalData::shared()->mail_translation && m_dialogInfo->fromUid!=GlobalData::shared()->playerInfo.uid && m_dialogInfo->translationMsg!=m_dialogInfo->contents && m_dialogInfo->translationMsg!=""){
            m_translateNode2->setVisible(true);
            m_translateLabel2->setString(_lang_1("105321", m_dialogInfo->originalLang.c_str()));
            m_dialogInfo->isShowOrginal=false;
            m_isTranslate = true;
        }else{
            if(m_dialogInfo->fromUid!=GlobalData::shared()->playerInfo.uid){
                m_translateNode2->setVisible(true);
            }else{
                m_translateNode2->setVisible(false);
            }
            
            if(m_dialogInfo->isShowOrginal==true){
                m_contentText2->setString(m_dialogInfo->contents.c_str());
            }else{
                m_contentText2->setString(m_dialogInfo->translationMsg.c_str());
            }
        }
        m_contentText1->setString("");
        //m_timeText2->setString(timestr);
        CCSize size =m_contentText2->getContentSize();
        string temp1=m_contentText2->getString();
        int wd = size.width*m_contentText2->getOriginScaleX();
        int wd1 = 0;
        if(wd<305){
            wd1=wd+18*2;
           // m_contentText2->cocos2d::CCLabelTTF::setDimensions(CCSize(wd, 0));
        }else{
            wd1 = 352;
            m_contentText2->setDimensions(CCSize(305, 0));
        }
        dh = lblH;
        float bgDh = dh+24*2;
        m_cellBGSpr2->setContentSize(CCSize(wd1, bgDh));
        m_cellBGSpr21->setContentSize(CCSize(wd1, bgDh));
        float bgY = m_cellBGSpr2->getPositionY();
        if(m_translateNode2->isVisible()){
            m_contentText2->setPositionY(bgY-bgDh/2 + 20);
            m_translateNode2->setPositionY(bgY - bgDh + 25);
        }else{
            m_contentText2->setPositionY(bgY-bgDh/2);
        }
        if(bgDh>82){
            this->m_totalNode->setPositionY(bgDh-82);
        }else{
            this->m_totalNode->setPositionY(0);
        }
        
//        if (dh>basedh) {
//            m_cellBGSpr2->setContentSize(CCSize(wd1, 92+(dh-basedh)));
//            m_cellBGSpr21->setContentSize(CCSize(wd1, 92+(dh-basedh)));
//            m_contentText2->setPositionY(-(dh-basedh)/2);
//            this->m_totalNode->setPositionY((dh-basedh)+5);
//        }else{
//            m_cellBGSpr2->setContentSize(CCSize(wd1, 92));
//            m_cellBGSpr21->setContentSize(CCSize(wd1, 92));
//            this->m_totalNode->setPositionY(0);
//            m_contentText2->setPositionY(0);
//        }
    }
    m_timeText1->setString(timestr);
    //this->m_deleteBtn1->setVisible(false);
    //this->m_deleteBtn2->setVisible(false);
}

void MailDialogCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
   // CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailDialogCell::setTranslation),MSG_TRANSLATE, NULL);
}

void MailDialogCell::onExit(){
    setTouchEnabled(false);
   // CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRANSLATE);
    CCNode::onExit();
}
bool MailDialogCell::checkSameLangBySystem(string oLang){
    bool flag = false;
    std::string sLang = LocalController::shared()->getLanguageFileName();
    if(oLang.find("zh")<oLang.length() && sLang.find("zh")<sLang.length()){
        flag = true;
    }else if(oLang==sLang || oLang==""){
        flag = true;
    }
    return flag;
}
void MailDialogCell::onShieldClick(cocos2d::CCObject *pTarget, CCControlEvent touchEvent){
    if(GlobalData::shared()->chatShieldInfo.isReachLimit()){
        CCCommonUtils::flyText(_lang("105314"));
        return;
    }
    
    //    if(GlobalData::shared()->chatShieldInfo.isShield(_uid)){
    //        return;
    //    }
    YesNoDialog::show(_lang("105313").c_str(), CCCallFunc::create(this, callfunc_selector(MailDialogCell::onYesClick)));
}

void MailDialogCell::onYesClick(){
    //    ChatLockCommand* cmd = new ChatLockCommand(_uid);
    //    cmd->sendAndRelease();
}

SEL_CCControlHandler MailDialogCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTranslateClick", MailDialogCell::onTranslateClick);
    return NULL;
}

bool MailDialogCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", CCNode*, this->m_timeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellnode1", CCNode*, this->m_cellnode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode1", CCNode*, this->m_headNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText1", CCLabelIF*, this->m_timeText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText1", CCLabelIFTTF*, this->m_contentText1);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBGSpr1", CCScale9Sprite*, this->m_cellBGSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBGSpr11", CCScale9Sprite*, this->m_cellBGSpr11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headBGSpr1", CCScale9Sprite*, this->m_headBGSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBGSprArrow1", CCSprite*, this->m_cellBGSprArrow1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBGSprArrow11", CCSprite*, this->m_cellBGSprArrow11);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellnode2", CCNode*, this->m_cellnode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode2", CCNode*, this->m_headNode2);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText2", CCLabelIFTTF*, this->m_contentText2);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBGSprArrow2", CCSprite*, this->m_cellBGSprArrow2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBGSprArrow21", CCSprite*, this->m_cellBGSprArrow21);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBGSpr2", CCScale9Sprite*, this->m_cellBGSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBGSpr21", CCScale9Sprite*, this->m_cellBGSpr21);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headBGSpr2", CCScale9Sprite*, this->m_headBGSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translateNode2", CCNode*, this->m_translateNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_langNode2", CCNode*, this->m_langNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translateLabel2", CCLabelIF*, this->m_translateLabel2);
    return false;
}

void MailDialogCell::onTranslateClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_mailInfo->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    MailController::getInstance()->removeMailDialog(m_mailInfo->uid,m_dialogInfo->uid, CC_ITOA(m_dialogInfo->type));

}

void MailDialogCell::setTranslation(CCObject* msg){

}

bool MailDialogCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchPoint = pTouch->getLocation();
    if(m_dialogInfo->type==0 || m_dialogInfo->type==MAIL_MOD_PERSONAL){
        return false;
    }else{
        if(isTouchInside(m_headBGSpr2,pTouch)){
            return  true;
        }else if(m_translateNode2->isVisible() && isTouchInside(m_langNode2, pTouch)){
            return true;
        }
    }

    return false;
}

void MailDialogCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 30) {
        return;
    }
    if(m_dialogInfo->type==0 || m_dialogInfo->type==MAIL_MOD_PERSONAL){
        if(isTouchInside( m_headBGSpr1,pTouch)){
            PlayerInfoBtnPopUpView* pop =PlayerInfoBtnPopUpView::create(m_mailInfo->nowName);
            pop->InitCommonBtns(m_mailInfo->fromName,m_mailInfo->fromUid,false);
			PopupViewController::getInstance()->addPopupView(pop, false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
			ChatServiceCocos2dx::hideChatInputView();
#endif
            return;
        }
    }else{
        if(isTouchInside(m_headBGSpr2,pTouch)){
            PlayerInfoBtnPopUpView* pop =PlayerInfoBtnPopUpView::create(m_mailInfo->nowName);
            pop->InitCommonBtns(m_mailInfo->fromName,m_mailInfo->fromUid,false);
			PopupViewController::getInstance()->addPopupView(pop, false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
			ChatServiceCocos2dx::hideChatInputView();
#endif
            return;
        }
    }
    if(m_translateNode2->isVisible() && isTouchInside(m_langNode2, pTouch)){
        if(m_isTranslate==false){
            if(m_dialogInfo->translationMsg.empty()){
                MailController::getInstance()->addLocalTranslate(m_dialogInfo);
            }else{
                m_translateLabel2->setString(_lang_1("105321", m_dialogInfo->originalLang.c_str()));
                m_contentText2->setString(m_dialogInfo->translationMsg.c_str());
                m_isTranslate=true;
                m_dialogInfo->isShowOrginal=false;
            }
        }else{
            m_contentText2->setString(m_dialogInfo->contents.c_str());
            m_translateLabel2->setString(_lang("105322"));
            m_isTranslate=false;
            m_dialogInfo->isShowOrginal=true;
        }
    }
}
void MailDialogCell::openChatFun(){
  //  PopupViewController::getInstance()->addPopupView(ChatFunView::create(ChatController::getInstance()->idx, CHAT_COUNTRY), false);
}
void MailDialogCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

