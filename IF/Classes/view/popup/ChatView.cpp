//
//  ChatView.cpp
//  IF
//
//  Created by fubin on 13-9-25.
//
//

#include "ChatView.h"
#include "SceneController.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "ChatController.h"
#include "ChatCell.h"
#include "YesNoDialog.h"
#include "ChatLockCommand.h"
#include "ChatUnLockCommand.h"
#include "MailWritePopUpView.h"
#include "GetUserInfoCommand.h"
#include "../../Ext/CCDevice.h"
#include "RoleInfoView.h"
#include "UIComponent.h"
#include "InvitesAllianceCommand.h"
#include "InputFieldMultiLine.h"
#include "JoinAllianceView.h"
#include "AlertAddAllianceView.h"
#include "CheckAllianceInfoView.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include "GetAllianceCommentsCommand.h"
#include "AllianceCommentShieldAdd.h"
#include "WorldController.h"
#include "AllianceSendCommentCommand.h"
#include "SceneContainer.h"
#include "ToolController.h"

#define SHOW_COVER_NATIVE_INPUT          0

const float numPerRow = 1.0;
ChatView* ChatView::create(int type,string allianceId,int countryType){
    ChatView* ret = new ChatView();
    if(ret && ret->init(type,allianceId,countryType)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
	}
	
    return ret;
}

bool ChatView::init(int _type,string allianceId,int countryType)
{
    // tao.yu 暂时关闭联盟聊天
//    _type = CHAT_COUNTRY;
    /////////////////
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    if(_type==CHAT_COMMENT){
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(205, false);
        });
    }
    m_countryChatType = (CountryChatType)countryType;
    auto tmpCCB = CCBLoadFile("ChatView",this,this);
    int oldBgHeight = m_buildBG->getContentSize().height;
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    tmpCCB->setContentSize(winsize);
    if (CCCommonUtils::isIosAndroidPad()) {
        winsize = CCSize(640, 852);
    }
    m_buildBG->setContentSize(CCSize(winsize.width+2,winsize.height));
    m_bg->setContentSize(CCSize(winsize.width+2,winsize.height));
    m_noticeBG1->setPreferredSize(CCSize(winsize.width,m_noticeBG1->getPreferredSize().height));
    m_noticeBG2->setScaleX(winsize.width/m_noticeBG2->getContentSize().width);
    m_noticeBG2->setPositionX(winsize.width*0.5);
    if (CCCommonUtils::isIosAndroidPad()) {
        winsize = CCDirector::sharedDirector()->getWinSize();
    }
    setContentSize(winsize);
    
    m_allianceId = allianceId;
    m_oldEditPositionY = m_editBGNode->getPositionY();
    
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    if(_type==CHAT_COMMENT){
        ChatController::getInstance()->m_commentPool.clear();
        m_tabNode->setVisible(false);
        m_bg->setContentSize(CCSize(640.0,1136));
        m_bg->setPositionY(-89.1);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_bg->setContentSize(CCSize(640, 1146));
            m_bg->setPositionY(-79.1);
        }
        oldHeight = m_infoList->getContentSize().height + 110;
    }
    //    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    CCCommonUtils::setButtonTitle(m_sendBtn, _lang("105302").c_str());
    if(_type!=CHAT_COUNTRY || ChatController::getInstance()->getIsNoticeOpen() == false){
        m_editBox = InputFieldMultiLine::create(CCSizeMake(460,60),"UI_Alliance_TextBox.png",24);
        m_btnState->setVisible(false);
        m_editBox->setPosition(ccp(0,0));
        m_editBox->setMaxChars(500);
    }else{
        m_editBox = InputFieldMultiLine::create(CCSizeMake(360,60),"UI_Alliance_TextBox.png",24);
        m_btnState->setVisible(true);
        m_editBox->setPosition(ccp(80,0));
    }
    //CCCommonUtils::setButtonTitleColor(m_sendBtn, ccc3(253, 187, 9));
    type = _type;
    
    //    auto sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    
    m_editBox->setAnchorPoint(ccp(0,0));
    //    m_editBox->setFontSize(20);
    //m_editBox->setPlaceHolder("");
	
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
//    m_editBox->setShowLineNum(3);
//    m_editBox->setLineNumber(3);
//#else
	m_editBox->setShowLineNum(6);
	m_editBox->setLineNumber(2);
//#endif
    m_editBox->setFontColor(ccBLACK);
    //    m_editBox->setInputMode(kEditBoxInputModeSingleLine);
    //    m_editBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    //    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    
    m_editBox->setSwallowsTouches(true);
    m_editBox->setMoveFlag(true);
    m_editBox->setTouchPriority(1);
    m_editNode->addChild(m_editBox);

    m_sendBtn->setTouchPriority(0);
    //m_editBox->setVisible(false);
    //    CCEditText* text = CCEditText::create(CCSizeMake(485,45),"UI_Alliance_TextBox.png");
    //    this->m_editNode->addChild(text);
    //
    m_loadingMoreText->setString(_lang("105502"));
    m_loadingMoreText->setVisible(false);
    m_loadingMoreText->setPositionY(m_loadingMoreText->getPositionY()+addHeight);
    m_loadingMoreStartY = m_loadingMoreText->getPositionY();
    //      kEditBoxInputFlagPassword:  密码形式输入
    //      kEditBoxInputFlagSensitive: 敏感数据输入、存储输入方案且预测自动完成
    //      kEditBoxInputFlagInitialCapsWord: 每个单词首字母大写,并且伴有提示
    //      kEditBoxInputFlagInitialCapsSentence: 第一句首字母大写,并且伴有提示
    //      kEditBoxInputFlagInitialCapsAllCharacters: 所有字符自动大写
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    //    m_tabView->setTableViewDelegate(this);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView, -1);
    
    CCCommonUtils::setButtonTitle(m_countryBtn, _lang("105300").c_str());
    CCCommonUtils::setButtonTitle(m_unionBtn, _lang("105602").c_str());
    if (type == CHAT_COUNTRY) {
        m_countryBtn->setEnabled(false);
        m_unionBtn->setEnabled(true);
    }
    else
    {
        m_countryBtn->setEnabled(true);
        m_unionBtn->setEnabled(false);
    }
    
    m_showCount = floor(m_infoList->getContentSize().height/130);
    m_tabView->reloadData();
    if (m_cellNum>m_showCount) {
        m_tabView->setContentOffset(m_tabView->maxContainerOffset());
        m_maxOffsetFlag = true;
    }
    else
    {
        m_maxOffsetFlag = false;
    }
    UIComponent::getInstance()->updateNewChatNews(0);
    if(ChatController::getInstance()->m_chatAlliancePool.size()>0){
        m_isFirstVisitAlliance = false;
    }
    else
    {
        m_isFirstVisitAlliance = true;
    }
    if(ChatController::getInstance()->m_chatCountryPool.size()>0){
        m_isFirstVisitCountry = false;
    }
    else
    {
        m_isFirstVisitCountry = true;
    }
    m_oldTablePosY = m_tabView->getPositionY();
    m_oldTableViewSize = m_tabView->getViewSize();
    
    if(ToolController::getInstance()->m_toolInfos.find(ITEM_SEND_NOTICE)!=ToolController::getInstance()->m_toolInfos.end()){
        auto &info = ToolController::getInstance()->getToolInfoById(ITEM_SEND_NOTICE);
//        std::string tName = _lang(info.getName().c_str());
        std::string showText = _lang_1("105332", info.getName().c_str());
        CCSize rollingSize = m_noticeTxtNode->getContentSize();
        m_rollingText = FlyRollingText::create(showText.c_str(), rollingSize.width, 22, ccc3(255,198,0), 60, 0);
        m_noticeTxtNode->addChild(m_rollingText);
        m_rollingText->setPosition(ccp(rollingSize.width*0.5, rollingSize.height*0.5));
    }else{
        m_rollingText = NULL;
    }
    
    if(type == CHAT_COMMENT){
        string lang = LocalController::shared()->getLanguageFileName();
        GetAllianceCommentsCommand* cmd = new GetAllianceCommentsCommand(m_allianceId,1,lang);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatView::getServerData), NULL));
        cmd->sendAndRelease();
    }else if(type == CHAT_COUNTRY && ChatController::getInstance()->getIsNoticeOpen()==true){
        switchState(true);
    }
    m_editBox->setText(ChatController::getInstance()->m_tmpSaveMsg.c_str());
    if(m_editBox->getLineNumber()>2){
        int changeH = m_editBox->getContentSize().height - 60;
        onAddlineEvent(CCInteger::create(changeH));
    }
    return true;
}

void ChatView::getServerData(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    CCArray* array = dynamic_cast<CCArray*>(result->getData());
    if(array){
        ChatController::getInstance()->m_commentPool.clear();
        int num = array->count();
        for (int i=0; i<num; i++) {
            auto dic = _dict(array->objectAtIndex(i));
            if (dic) {
                ChatInfo info = ChatInfo(dic);
                info.type = CHAT_COMMENT;
                ChatController::getInstance()->m_commentPool.insert(ChatController::getInstance()->m_commentPool.begin(), info);
            }
        }
    }
    m_tabView->reloadData();
}

void ChatView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("105316"));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_IN);
    if (type == CHAT_ALLIANCE && !GlobalData::shared()->playerInfo.isInAlliance()) {
        //ChatController::getInstance()->sendRequestChat(type);
        m_tabView->reloadData();
        if(GlobalData::shared()->playerInfo.isfirstJoin&&(!GlobalData::shared()->playerInfo.isInAlliance()))
            PopupViewController::getInstance()->addPopupView(AlertAddAllianceView::create());
        showJointxt(true);
    }
    if(type == CHAT_COUNTRY && m_isFirstVisitCountry){
        ChatController::getInstance()->sendRequestChat(type);
        m_isFirstVisitCountry = false;
    }
    if(type == CHAT_ALLIANCE && GlobalData::shared()->playerInfo.isInAlliance() && m_isFirstVisitAlliance){
        ChatController::getInstance()->sendRequestChat(type);
        m_isFirstVisitAlliance = false;
    }
    
    if(type == CHAT_COMMENT){
        setTitleName(_lang("115929"));
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatView::setChatName), MSG_CHAT_NAME, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatView::updateChat), MSG_CHAT_UPDATE, NULL);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ChatView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatView::onAddlineEvent), MSG_INPUT_ADD_LINE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatView::onEditTextEvent), MSG_INPUT_TEXT_EVENT, NULL);
	CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatView::onInputFieldCloseEvent), INPUTFIELD_CLOSE_EVENT, NULL);
	
	CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatView::popupCloseEvent), MSG_POPUP_CLOSE_EVENT, NULL);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	ChatServiceCocos2dx::setSendMessageListener(this);
	scheduleOnce(schedule_selector(ChatView::showNativeTextInput), 0.01f);
	ChatServiceCocos2dx::setSendButtonText(_lang("105302").c_str());
	ChatServiceCocos2dx::setEditTextHintText("");
#endif
}

void ChatView::showNativeTextInput(float time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	CCLOG("ChatView::showNativeTextInput");
	set2dxTextInputVisible(false);
	
	ChatServiceCocos2dx::showChatInputView();
	ChatServiceCocos2dx::enableChatInputView();
#endif
}

void ChatView::hideNativeTextInput()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
//	CCLOG("ChatView::hideNativeTextInput");
	set2dxTextInputVisible(true);
	
	ChatServiceCocos2dx::disableChatInputView();
	ChatServiceCocos2dx::hideChatInputView();
#endif
}

void ChatView::set2dxTextInputVisible(bool visible)
{
//	m_editBGNode->setVisible(visible);
//	m_editNode->setVisible(visible);
}

void ChatView::popupCloseEvent(CCObject *params)
{
	int c = PopupViewController::getInstance()->getCurrViewCount();
	CCLOG("ChatView::popupCloseEvent getCurrViewCount: %d", c);
	
	if(type == CHAT_ALLIANCE && c == 1)
	{
		//关闭加入联盟的提醒
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
		showNativeTextInput(0.0);
#endif
	}else if(type == CHAT_COUNTRY && c == 1)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
		showNativeTextInput(0.0);
#endif
	}else if(type == CHAT_COMMENT && c == 1)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
		showNativeTextInput(0.0);
#endif
	}
}

void ChatView::setHeightFromNative(int height, int usableHeightSansKeyboard)
{
	return;
	
	CCLOG("ChatView::setHeightFromNative()");
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	int heightShrink = usableHeightSansKeyboard == height ? 0 : (winsize.height - height);
	CCLog("h: %d", height);
	CCLog("usableHeightSansKeyboard: %d", usableHeightSansKeyboard);
	CCLog("heightShrink: %d", heightShrink);
	CCLog("old getPositionY(): %f", m_tabView->getPositionY());
	CCLog("old getViewSize().height: %f", m_tabView->getViewSize().height);
	CCLog("m_oldTableViewSize.height: %f", m_oldTableViewSize.height);
	
	m_tabView->setPositionY(m_oldTablePosY + heightShrink);
	CCLog("new getPositionY(): %f", m_tabView->getPositionY());
	m_tabView->setViewSize(CCSizeMake(m_tabView->getViewSize().width, m_oldTableViewSize.height-heightShrink));
	CCLog("new getViewSize().height: %f", m_tabView->getViewSize().height);
//	CCPoint pt = m_tabView->getContentOffset();
	
	CCLog("m_tabView->minContainerOffset(): %f", m_tabView->minContainerOffset().y);
	CCLog("m_tabView->maxContainerOffset(): %f", m_tabView->maxContainerOffset().y);
	
	//	if(heightShrink>0){
	//		m_tabView->setContentOffset(m_tabView->maxContainerOffset());
	//	}else{
	//		m_tabView->setContentOffset(m_tabView->maxContainerOffset());
	//	}
	
	CCPoint curr = m_tabView->getContentOffset();
	CCPoint max = m_tabView->maxContainerOffset();
	CCPoint min = m_tabView->minContainerOffset();
	if (curr.y == max.y || (!m_maxOffsetFlag && m_cellNum > m_showCount)) {
		m_tabView->setContentOffset(m_tabView->maxContainerOffset());
		m_maxOffsetFlag = true;
	}
	else if(m_maxOffsetFlag)
	{
		//curr.y += m_tabView->minContainerOffset().y - min.y;
		m_tabView->setContentOffset(curr);
	}
	// m_tabView->setBounceable(true);
	// m_tabView->setTouchEnabled(true);
	m_tabView->setTouchEnabled(true);
	
	//	m_tabView->setContentOffset(pt);
	//	m_tabView->setViewSize(CCSizeMake(m_tabView->getViewSize().width, h));
}

void ChatView::onClose()
{
    m_editBox->closeIME();
    PopupViewController::getInstance()->goBackPopupView();
}

void ChatView::onExit()
{
    GameController::getInstance()->removeWaitInterface();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHAT_NAME);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHAT_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_OUT);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ChatView::onEnterFrame), this);
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_ADD_LINE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_TEXT_EVENT);
	CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, INPUTFIELD_CLOSE_EVENT);
	CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_CLOSE_EVENT);
    auto _currScene = CCDirector::sharedDirector()->getRunningScene();
    if (_currScene->getPositionY() != 0) {
        _currScene->setPositionY(0);
    }
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        UIComponent::getInstance()->changeChatChannel(type);
    }
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	ChatServiceCocos2dx::setSendMessageListener(NULL);
	ChatController::getInstance()->m_tmpSaveMsg = ChatServiceCocos2dx::getChatInputText();
	
	hideNativeTextInput();
#else
	ChatController::getInstance()->m_tmpSaveMsg = m_editBox->getText();
#endif
    CCNode::onExit();
}
void ChatView::onAddlineEvent(CCObject * params)
{
    int changeHeight = dynamic_cast<CCInteger*>(params)->getValue();
    m_tabView->setPositionY(m_tabView->getPositionY()+changeHeight);
    m_tabView->setViewSize(CCSizeMake(m_tabView->getViewSize().width, m_tabView->getViewSize().height-changeHeight));
    m_editBGNode->setPositionY(m_editBGNode->getPositionY()+changeHeight);
}
void ChatView::onInputFieldCloseEvent(cocos2d::CCObject *params)
{
//    m_editBGNode->setPositionY(m_oldEditPositionY);
}
void ChatView::onEditTextEvent(cocos2d::CCObject *params)
{
    string num = CC_ITOA( dynamic_cast<CCInteger*>(params)->getValue());
    if (type == CHAT_COUNTRY && m_countryChatType == C_COUNTRY_NOTICE) {
        m_inputNum->setString(num + std::string("/") + CC_ITOA(ChatController::getInstance()->getNoticeCharMax()));
    }else{
        m_inputNum->setString(num + "/500");
    }
}
bool ChatView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_editBox, pTouch)) {
        CCLOG("isTouchInside");
    }
    m_tabView->setBounceable(true);
    CCLOG("isTouchInside false");
    return true;
}
void ChatView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    auto _currScene = CCDirector::sharedDirector()->getRunningScene();
    if (_currScene->getPositionY() != 0) {
        onInputFieldCloseEvent(NULL);
        _currScene->setPositionY(0);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	ChatServiceCocos2dx::disableChatInputView();
	ChatServiceCocos2dx::enableChatInputView();
#endif
}

void ChatView::onEnterFrame(float dt)
{
    //    CCPoint min = m_tabView->minContainerOffset();
    //    CCPoint max = m_tabView->maxContainerOffset();
    //    CCPoint curr = m_tabView->getContentOffset();
    //    CCLOG("min %f,%f",min.x,min.y);
    //    CCLOG("max %f,%f",max.x,max.y);
    //    CCLOG("curr %f,%f",curr.x,curr.y);
    
    delayTime++;
    return;
    
    string tmp = m_editBox->getText();
    string lastStr = "";
    for (int i = 0; i<tmp.length(); i++) {
        if (0<=tmp[i])//&& tmp[i]<=256
        {
            lastStr += tmp[i];
        }
    }
    m_editBox->setText(lastStr.c_str());
}

SEL_CCControlHandler ChatView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseView", ChatView::onCloseView);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeState", ChatView::onChangeState);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPostBtnClick", ChatView::onPostBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUnionClick", ChatView::onUnionClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCountryClick", ChatView::onCountryClick);
    return NULL;
}

bool ChatView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnState", CCControlButton*, this->m_btnState);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, this->m_editNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_countryBtn", CCControlButton*, this->m_countryBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unionBtn", CCControlButton*, this->m_unionBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBg", CCScale9Sprite*, this->m_editBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBGNode", CCNode*, this->m_editBGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputNum", CCLabelIF*, this->m_inputNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchEditNode", CCNode*, m_touchEditNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingMoreText", CCLabelIF*, m_loadingMoreText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabNode", CCNode*, this->m_tabNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeBGNode", CCNode*, this->m_noticeBGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeBG1", CCScale9Sprite*, this->m_noticeBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeBG2", CCSprite*, this->m_noticeBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeTxtNode", CCNode*, this->m_noticeTxtNode);
    return false;
}
void ChatView::onChangeState(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    if(m_countryChatType==C_COUNTRY_MSG){
        m_countryChatType = C_COUNTRY_NOTICE;
    }else{
        m_countryChatType = C_COUNTRY_MSG;
    }
    switchState(false);
}
void ChatView::switchState(bool bInit){
    if(ChatController::getInstance()->getIsNoticeOpen()==false)
        return;
    switch (m_countryChatType) {
        case C_COUNTRY_NOTICE:{
            setButtonSprite(m_btnState,"chat_country_btn2.png");
            setButtonSprite(m_sendBtn, "btn_green3.png");
            m_editBox->setMaxChars(ChatController::getInstance()->getNoticeCharMax());
            if(bInit){
                m_editBox->changeBG("chat_notice_input_bg.png");
                CCSize size = m_infoList->getContentSize();
                m_tabView->setViewSize(CCSize(size.width, size.height - 80));
                m_tabView->setPositionY(80);
            }else{
                CCSize size = m_tabView->getViewSize();
                m_tabView->setViewSize(CCSize(size.width, size.height - 80));
                m_tabView->setPositionY(m_tabView->getPositionY()+80);
            }
            m_oldTablePosY = 80;
            m_oldTableViewSize = m_tabView->getViewSize();
            m_noticeBGNode->setVisible(true);
            break;
        }
        case C_COUNTRY_MSG:{
            setButtonSprite(m_btnState,"chat_country_btn1.png");
            setButtonSprite(m_sendBtn, "btn_xiao_hong.png");
            m_editBox->setMaxChars(500);
            m_editBox->changeBG("UI_Alliance_TextBox.png");
            if(bInit){
                CCSize size = m_infoList->getContentSize();
                m_tabView->setViewSize(CCSize(size.width, size.height));
                m_tabView->setPositionY(0);
            }else{
                CCSize size = m_tabView->getViewSize();
                m_tabView->setViewSize(CCSize(size.width, size.height + 80));
                m_tabView->setPositionY(m_tabView->getPositionY()-80);
            }
            m_oldTablePosY = 0;
            m_oldTableViewSize = m_tabView->getViewSize();
            m_noticeBGNode->setVisible(false);
            break;
        }
        default:
            break;
    }
}
void ChatView::setButtonSprite(CCControlButton* btn,std::string imgName){
    CCScale9Sprite* scale9 = CCLoadSprite::createScale9Sprite(imgName.c_str());
    btn->setBackgroundSpriteForState(scale9, CCControlStateNormal);
    CCScale9Sprite* scale91 = CCLoadSprite::createScale9Sprite(imgName.c_str());
    btn->setBackgroundSpriteForState(scale91, CCControlStateHighlighted);
    CCScale9Sprite* scale92 = CCLoadSprite::createScale9Sprite(imgName.c_str());
    btn->setBackgroundSpriteForState(scale92, CCControlStateDisabled);
    
}
void ChatView::onPostBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    //    if(GlobalData::shared()->playerInfo.renameTime == 0) {
    //        //PopupViewController::getInstance()->addPopupView(ChangeNickNameView::create(""));
    //        YesNoDialog::showVariableTitle(_lang("105225").c_str(),CCCallFunc::create(this, callfunc_selector(ChatView::reNameFunc)),_lang("105221").c_str());
    //    }
    //    else {
    if (delayTime<ChatController::getInstance()->chat_interval)
    {
        CCCommonUtils::flyText(_lang("105307"));
        return;
    }
    delayTime=0;
    if(type==CHAT_COMMENT){
        m_sendMsg = m_editBox->getText();
        string lang = LocalController::shared()->getLanguageFileName();
        AllianceSendCommentCommand* cmd = new AllianceSendCommentCommand(m_allianceId,m_sendMsg,lang);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatView::callBackSuccess), NULL));
        cmd->sendAndRelease();
    }else{
        if(type == CHAT_COUNTRY && m_countryChatType == C_COUNTRY_NOTICE){
            m_editBox->closeIME();
            sendNotice();
            return;
        }else{
         ChatController::getInstance()->sendCountryChat(m_editBox->getText().c_str(), type);
        }
    }
    m_editBox->setText("");
    m_editBox->closeIME();
    m_editBox->openIME();
    m_tabView->setPositionY(m_oldTablePosY);
    m_tabView->setViewSize(m_oldTableViewSize);
    m_editBGNode->setPositionY(m_oldEditPositionY);
    
    //    }s
}
void ChatView::sendNotice(){
    auto tmpT = ChatController::getInstance()->getNoticeBanTime();
    int lastTime = GlobalData::shared()->changeTime(tmpT) - GlobalData::shared()->getWorldTime();
    if(lastTime>0){
        YesNoDialog::showTimeWithDes(_lang("105334").c_str(), _lang("105335").c_str(), lastTime);
        return;
    }
    
    std::string msg = m_editBox->getText();
    int itemid = ITEM_SEND_NOTICE;
    if(msg.empty() && itemid>0){
        return;
    }
//    if(ToolController::getInstance()->m_toolInfos.find(itemid) != ToolController::getInstance()->m_toolInfos.end()){
        auto& info = ToolController::getInstance()->getToolInfoById(itemid);
        if(info.getCNT()>0){
            YesNoDialog::show(_lang_1("104955", info.getName().c_str()), CCCallFunc::create(this, callfunc_selector(ChatView::sureSendNoticeByTool)),0,false,CCCallFunc::create(this, callfunc_selector(ChatView::cancelSendNotice)));
        }else{
            YesNoDialog::showButtonAndGold(_lang_1("105333",info.getName().c_str()), CCCallFunc::create(this, callfunc_selector(ChatView::sureSendNoticeByPoint)), _lang("confirm").c_str(), ChatController::getInstance()->getNoticePrice());
        }
//    }
}
void ChatView::sureSendNoticeByPoint(){
//    if(ToolController::getInstance()->m_toolInfos.find(ITEM_SEND_NOTICE) != ToolController::getInstance()->m_toolInfos.end()){
//        auto& info = ToolController::getInstance()->getToolInfoById(ITEM_SEND_NOTICE);
    
        if (!CCCommonUtils::isEnoughResourceByType(Gold,ChatController::getInstance()->getNoticePrice())) {
            YesNoDialog *dialog = YesNoDialog::showYesDialog(_lang("104912").c_str());
            return;
        }
        ChatController::getInstance()->sendNotice(m_editBox->getText(), ITEM_SEND_NOTICE, true);
//    }
    m_editBox->setText("");
    m_editBox->openIME();
    m_tabView->setPositionY(m_oldTablePosY);
    m_tabView->setViewSize(m_oldTableViewSize);
    m_editBGNode->setPositionY(m_oldEditPositionY);
}
void ChatView::sureSendNoticeByTool(){
    ChatController::getInstance()->sendNotice(m_editBox->getText(), ITEM_SEND_NOTICE, false);
    m_editBox->setText("");
    m_editBox->openIME();
    m_tabView->setPositionY(m_oldTablePosY);
    m_tabView->setViewSize(m_oldTableViewSize);
    m_editBGNode->setPositionY(m_oldEditPositionY);
}
void ChatView::cancelSendNotice(){
    m_editBox->openIME();
    m_editBox->setText("");
    m_tabView->setPositionY(m_oldTablePosY);
    m_tabView->setViewSize(m_oldTableViewSize);
    m_editBGNode->setPositionY(m_oldEditPositionY);
}



void ChatView::onTextChanged(string msg)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	scheduleOnce(schedule_selector(ChatView::onTextChangedHandler), 0.01f);
#endif
}

void ChatView::onTextChangedHandler(float time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	m_editBox->setText(ChatServiceCocos2dx::getChatInputText().c_str());
#endif
}

void ChatView::sendMessage(string msg)
{
	CCLOG("ChatView::sendMessage");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	m_sendMsg = msg;
	scheduleOnce(schedule_selector(ChatView::onSendMessageHandler), 0.01f);
#endif
}

void ChatView::onSendMessageHandler(float time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	string msg = m_sendMsg;
	CCLOG("msg: %s", msg.c_str());
	
	if(type == CHAT_ALLIANCE && !GlobalData::shared()->playerInfo.isInAlliance())
	{
		return;
	}
	if (delayTime<ChatController::getInstance()->chat_interval)
	{
		CCCommonUtils::flyText(_lang("105307"));
		return;
	}
	delayTime=0;
	if(type==CHAT_COMMENT){
		m_sendMsg = msg;
		string lang = LocalController::shared()->getLanguageFileName();
		AllianceSendCommentCommand* cmd = new AllianceSendCommentCommand(m_allianceId,m_sendMsg,lang);
		cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChatView::callBackSuccess), NULL));
		cmd->sendAndRelease();
	}else{
		ChatController::getInstance()->sendCountryChat(msg.c_str(), type);
	}
#endif
}

void ChatView::onBackPressed()
{
	CCLOG("ChatView::onBackPressed");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	scheduleOnce(schedule_selector(ChatView::onBackPressedHandler), 0.01f);
#endif
}

void ChatView::onBackPressedHandler(float time)
{
	CCLOG("ChatView::onBackPressedHandler");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	SceneContainer* sceneContainer = dynamic_cast<SceneContainer*>(CCDirector::sharedDirector()->getRunningScene());
	sceneContainer->keyBackClicked();
#endif
}

void ChatView::callBackSuccess(CCObject * param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if (result==NULL || result->getData()==NULL) {
        return ;
    }
    auto dic = _dict(result->getData());
    if(dic){
        string tempMsg = dic->valueForKey("comment")->getCString();
        ChatInfo info;
        info.uid = GlobalData::shared()->playerInfo.uid;
        if (GlobalData::shared()->playerInfo.isInAlliance()) {
            info.asn = GlobalData::shared()->playerInfo.allianceInfo.shortName;
            info.stype = 2;
            info.senderPic = GlobalData::shared()->playerInfo.allianceInfo.icon;
        }else{
            info.stype = 1;
            info.senderPic = GlobalData::shared()->playerInfo.pic;
        }
        info.msg = tempMsg;
        info.name = GlobalData::shared()->playerInfo.name;
        info.type = CHAT_COMMENT;
        info.allianceId = GlobalData::shared()->playerInfo.allianceInfo.uid;
        info.senderAllianceId = GlobalData::shared()->playerInfo.allianceInfo.uid;
        info.time = WorldController::getInstance()->getTime()/1000.0;
        
        ChatController::getInstance()->m_commentPool.push_back(info);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_UPDATE);
    }
}

void ChatView::reNameFunc(){
    //    PopupViewController::getInstance()->addPopupView(ChangeNickNameView::create(""));
}

//void ChatView::onCloseView(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
//{
//    ChatController::getInstance()->m_tmpSaveMsg = m_editBox->getText();
//    PopupViewController::getInstance()->removeAllPopupView();
//}

void ChatView::onUnionClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (type == CHAT_COUNTRY) {
        type = CHAT_ALLIANCE;
        if (!GlobalData::shared()->playerInfo.isInAlliance()) {
			if(GlobalData::shared()->playerInfo.isfirstJoin&&(!GlobalData::shared()->playerInfo.isInAlliance())){
                PopupViewController::getInstance()->addPopupView(AlertAddAllianceView::create());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
				hideNativeTextInput();
				set2dxTextInputVisible(false);
#endif
			}
            showJointxt(true);
            //YesNoDialog::show(_lang("E100068").c_str(), [](){});
            //return;
            //CCCommonUtils::flyHint("", "", _lang("E100068"));
        }
        if(m_isFirstVisitAlliance){
            ChatController::getInstance()->sendRequestChat(type);
            m_isFirstVisitAlliance = false;
        }
        m_tabView->reloadData();
        m_unionBtn->setEnabled(false);
        m_countryBtn->setEnabled(true);
        if (m_cellNum>m_showCount) {
            m_tabView->setContentOffset(m_tabView->maxContainerOffset());
            m_maxOffsetFlag = true;
        }
        else
        {
            m_maxOffsetFlag = false;
        }
        m_noticeBGNode->setVisible(false);
        m_btnState->setVisible(false);
        if(ChatController::getInstance()->getIsNoticeOpen()==true){
            string text = m_editBox->getText();
            m_editBox->setText("");
            m_editBox->resize(CCSizeMake(460,60));
            m_editBox->setPositionX(0);
            m_editBox->setText(text);
        }
    }
}

void ChatView::onCountryClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (type == CHAT_ALLIANCE) {
        showJointxt(false);
        type = CHAT_COUNTRY;
        if(m_isFirstVisitCountry){
            ChatController::getInstance()->sendRequestChat(type);
            m_isFirstVisitCountry = false;
        }
        m_tabView->reloadData();
        m_unionBtn->setEnabled(true);
        m_countryBtn->setEnabled(false);
        if (m_cellNum>m_showCount) {
            m_tabView->setContentOffset(m_tabView->maxContainerOffset());
            m_maxOffsetFlag = true;
        }
        else
        {
            m_maxOffsetFlag = false;
		}
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
//		showNativeTextInput(0.0);
//#endif
        if(ChatController::getInstance()->getIsNoticeOpen()){
            m_btnState->setVisible(true);
            string text = m_editBox->getText();
            m_editBox->setText("");
            m_editBox->resize(CCSizeMake(360,60));
            m_editBox->setPositionX(80);
            switchState(true);
            m_editBox->setText(text);
        }else{
            m_btnState->setVisible(false);
            m_noticeBGNode->setVisible(false);
        }
    }
}

void ChatView::onJoinClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    GlobalData::shared()->playerInfo.isChatView = true;
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
    GlobalData::shared()->playerInfo.isChatView = false;
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	hideNativeTextInput();
#endif
}
void ChatView::showJointxt(bool show){
    if(!show){
        m_infoList->removeChildByTag(124);
        return;
    }
    else {
        //node
        CCNode* node = CCNode::create();
        node->setPosition(m_infoList->getContentSize()/2);
        node->setTag(124);
        //txt
        CCLabelIF* txt = CCLabelIF::create(_lang("E100068").c_str(), 24, CCSize(400,NULL), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        txt->setColor({77,37,0});
        txt->setPosition(ccp(0,100));
        node->addChild(txt);
        //button
        CCControlButton* btn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_green3.png"));
        CCCommonUtils::setButtonTitle(btn, _lang("115020").c_str());
        btn->setPosition(ccp(0,-50));
        btn->setPreferredSize(CCSize(244,74));
        btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ChatView::onJoinClick), CCControlEventTouchUpInside);
        node->addChild(btn);
        
        m_infoList->addChild(node);
    }
}

void ChatView::setChatName(CCObject* params)
{
}

void ChatView::updateChat(CCObject* obj)
{
	CCLOG("ChatView::updateChat()");
    _canRequest = true;
    m_loadingMoreText->setVisible(false);
    GameController::getInstance()->removeWaitInterface();
    CCPoint curr = m_tabView->getContentOffset();
    CCPoint max = m_tabView->maxContainerOffset();
    CCPoint min = m_tabView->minContainerOffset();
    m_tabView->reloadData();
    if (curr.y == max.y || (!m_maxOffsetFlag && m_cellNum > m_showCount)) {
        m_tabView->setContentOffset(m_tabView->maxContainerOffset());
        m_maxOffsetFlag = true;
    }
    else if(m_maxOffsetFlag)
    {
        //curr.y += m_tabView->minContainerOffset().y - min.y;
        m_tabView->setContentOffset(curr);
    }
   // m_tabView->setBounceable(true);
   // m_tabView->setTouchEnabled(true);
    m_tabView->setTouchEnabled(true);
    // m_tabView->setContentOffset(curr);
}

void ChatView::scrollViewDidScroll(CCScrollView* view)
{
    if (type == CHAT_COUNTRY){
        m_loadingMoreText->setVisible(false);
        m_loadingMoreText->setPositionY(m_loadingMoreStartY);
        return;
    }
    float curY = m_tabView->getContentOffset().y;
    float minY = m_tabView->minContainerOffset().y;
    if ((minY-curY) >30) {
        float dy = minY-curY;

        m_loadingMoreText->setPositionY(m_loadingMoreStartY-(dy-30));
        
        if (_lastPosY > curY) {//向下滑动

            if(_canRequest&&ChatController::getInstance()->canAllianceRequest){

                _canRequest = false;
                scheduleOnce(schedule_selector(ChatView::reSetRequest), 5.0f);
                CCLOG("request data from server");
                if (type == CHAT_ALLIANCE) {
                    if((minY-curY) >80){
                        m_tabView->setContentOffset(ccp(0, minY-80));
                    }
                    m_loadingMoreText->setVisible(true);
                    ChatController::getInstance()->sendRequestChat(type);
                    m_tabView->setTouchEnabled(false);
                    m_tabView->setBounceable(false);
                    GameController::getInstance()->showWaitInterface();
                }
            }else{
                m_loadingMoreText->setVisible(false);
            }
        }else{
            m_loadingMoreText->setVisible(false);
        }
    }else{
        m_loadingMoreText->setVisible(false);
    }
    _lastPosY = curY;
}

void ChatView::reSetRequest(float _time)
{
    _canRequest = true;
}

void ChatView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    ChatCell* tmpCell = dynamic_cast<ChatCell*>(cell);
    if (tmpCell) {
        //        tmpCell->cellTouchEnded(m_tabView->m_pTouchedCell);
    }
}
CCSize ChatView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    int height = 0;
//    int line = 0;
    if (type == CHAT_COUNTRY)
    {
        auto& info = ChatController::getInstance()->m_chatCountryPool[idx];
        
        height = 130;
        int _tmpHeight = 50;
        if(!info.translationMsg.empty()){
            _tmpHeight = MAX(getTextDimension(info.msg.c_str()).height,getTextDimension(info.translationMsg.c_str()).height);
        }else{
            _tmpHeight = getTextDimension(info.msg.c_str()).height;
        }
        if (_tmpHeight > 50) {
            height += _tmpHeight - 45;
        }
        if (idx > 0)
        {
            time_t lt = ChatController::getInstance()->m_chatCountryPool[idx-1].time;
            if(info.isTimeShowed&&info.showedTime!=""){
            
            }else if ((info.time-lt) < ChatController::getInstance()->chat_time) {
                height -= 30;
            }
        }
    }else if (type == CHAT_COMMENT)
    {
        auto& info = ChatController::getInstance()->m_commentPool[idx];
        
        height = 130;
        int _tmpHeight = getTextDimension(info.msg.c_str()).height;
        if(!info.translationMsg.empty()){
            _tmpHeight = MAX(getTextDimension(info.msg.c_str()).height,getTextDimension(info.translationMsg.c_str()).height);
        }
        if (_tmpHeight > 50) {
            height += _tmpHeight - 45;
        }
        if (idx > 0)
        {
            time_t lt = ChatController::getInstance()->m_commentPool[idx-1].time;
            if(info.isTimeShowed&&info.showedTime!=""){
                
            }else if ((info.time-lt) < ChatController::getInstance()->chat_time) {
                height -= 30;
            }
        }
    }
    else
    {
        auto& info = ChatController::getInstance()->m_chatAlliancePool[idx];

        height = 130;
        int _tmpHeight = 50;
        if(!info.translationMsg.empty()){
            _tmpHeight = MAX(getTextDimension(info.msg.c_str()).height,getTextDimension(info.translationMsg.c_str()).height);
        }else{
            _tmpHeight = getTextDimension(info.msg.c_str()).height;
        }
        if (_tmpHeight > 50) {
            height += _tmpHeight - 50;
        }

        if (idx > 0)
        {
            time_t lt = ChatController::getInstance()->m_chatAlliancePool[idx-1].time;
            if(info.isTimeShowed&&info.showedTime!=""){
                
            }else if ((info.time-lt) < ChatController::getInstance()->chat_time) {
                height -= 30;
            }
        }
    }
//    if (line > 2) {
//        height += 25 * (line - 2);
//    }
    //    CCLOG("msg:%s, height:%d, line:%d, size:%d",info.msg.c_str(),height,line,info.msg.size());
    return CCSize(640,height);
}
CCSize ChatView::getTextDimension(const char *text)
{
    int len = (int)strlen(text);
    if ((len==0)  ||  (len==1 && text[0] == '\n')) {
        CCLabelIFTTF *label = CCLabelIFTTF::create( "sample" , "Helvetica", 20 );
        label->setDimensions(CCSizeMake(450, 0));
        CCSize size = label->getContentSize();
        size.width = 0;
        return size;
    }
    CCLabelIFTTF *label = CCLabelIFTTF::create( text, "Helvetica", 20);
    label->setDimensions(CCSizeMake(450, 0));
    return label->getContentSize();
}

cocos2d::CCSize ChatView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 130);
}

CCTableViewCell* ChatView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if (type == CHAT_COUNTRY && idx >= ChatController::getInstance()->m_chatCountryPool.size()) {
        return NULL;
    }
    else if ( type == CHAT_ALLIANCE && idx >= ChatController::getInstance()->m_chatAlliancePool.size()) {
        return NULL;
    }
    else if ( type == CHAT_COMMENT && idx >= ChatController::getInstance()->m_commentPool.size()) {
        return NULL;
    }
    ChatCell* cell = (ChatCell*)table->dequeueCell();
    if(cell){
        cell->setData(type,idx,m_allianceId);
    }else{
        cell = ChatCell::create(type, idx, m_infoList,m_allianceId);
    }
    return cell;
    
}

ssize_t ChatView::numberOfCellsInTableView(CCTableView *table){
    m_cellNum = 0;
    if (type == CHAT_COUNTRY) {
        m_cellNum = ceil(ChatController::getInstance()->m_chatCountryPool.size() / numPerRow);
    }else if (type == CHAT_COMMENT) {
        m_cellNum = ceil(ChatController::getInstance()->m_commentPool.size() / numPerRow);
    }
    else {
        m_cellNum = ceil(ChatController::getInstance()->m_chatAlliancePool.size() / numPerRow);
    }
    return m_cellNum;
}

//unsigned int ChatView::numberOfGridsInCell(  cocos2d::extension::TableView *multiTable){
//    return numPerRow;
//}

ChatFunView* ChatFunView::create(int idx, int type) {
    auto ret = new ChatFunView();
    if (ret && ret->init(idx, type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ChatFunView::init(int idx, int type) {
    bool ret = false;
    if (PopupBaseView::init()) {
        m_idx = idx;
        m_type = type;
        setIsHDPanel(true);
        CCBLoadFile("AllianceFunBtnView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        ret = true;
        ChatInfo info;
        const char* btnNames[8] = {"108584","115922","115923","105304","105308","105309","115020","105207"};
        if (m_type==CHAT_COUNTRY){
            info = ChatController::getInstance()->m_chatCountryPool[m_idx];
            m_uid = info.uid;
            m_name = info.name;
            m_msg = info.msg;
            m_allianceId = info.allianceId;
            info.isNew = false;
            if(GlobalData::shared()->playerInfo.isInAlliance() && info.uid!=GlobalData::shared()->playerInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank>=3 && ChatController::getInstance()->newChaterAllianceId ==""){
                m_buttonFuns.push_back(C_INVITE);
            }
            if(!GlobalData::shared()->playerInfo.isInAlliance() && info.asn!="" && info.uid!=GlobalData::shared()->playerInfo.uid){
                m_buttonFuns.push_back(C_JOIN);
            }
            m_buttonFuns.push_back(C_BLOCK_PLAYER);
            m_buttonFuns.push_back(C_MSG_COPY);
            m_buttonFuns.push_back(C_SEND_MAIL);
            m_buttonFuns.push_back(C_VIEW_PLAYER);
        }
        else if (m_type==CHAT_ALLIANCE){
            info = ChatController::getInstance()->m_chatAlliancePool[m_idx];
            m_uid = info.uid;
            m_name = info.name;
            m_msg = info.msg;
            m_allianceId = info.allianceId;
            info.isNew = false;
            m_buttonFuns.push_back(C_INVITE);
            m_buttonFuns.push_back(C_BLOCK_PLAYER);
            m_buttonFuns.push_back(C_MSG_COPY);
            m_buttonFuns.push_back(C_SEND_MAIL);
            m_buttonFuns.push_back(C_VIEW_PLAYER);
        }else if (m_type==CHAT_COMMENT){
            info = ChatController::getInstance()->m_commentPool[m_idx];
            m_uid = info.uid;
            m_name = info.name;
            m_msg = info.msg;
            m_allianceId = info.allianceId;
            info.isNew = false;
            if(GlobalData::shared()->playerInfo.isInAlliance() && m_allianceId != GlobalData::shared()->playerInfo.allianceInfo.uid &&  GlobalData::shared()->playerInfo.allianceInfo.rank>=4){
                m_buttonFuns.push_back(C_INVITE);
                m_buttonFuns.push_back(C_BLOCK_PLAYER);
                m_buttonFuns.push_back(C_BLOCK_ALLIANCE);
                m_buttonFuns.push_back(C_MSG_COPY);
                m_buttonFuns.push_back(C_SEND_MAIL);
                m_buttonFuns.push_back(C_VIEW_PLAYER);
            }else if(GlobalData::shared()->playerInfo.isInAlliance() && m_allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid &&  GlobalData::shared()->playerInfo.allianceInfo.rank>=4){
                m_buttonFuns.push_back(C_INVITE);
                m_buttonFuns.push_back(C_BLOCK_PLAYER);
                m_buttonFuns.push_back(C_MSG_COPY);
                m_buttonFuns.push_back(C_SEND_MAIL);
                m_buttonFuns.push_back(C_VIEW_PLAYER);
            }else {
                m_buttonFuns.push_back(C_MSG_COPY);
                m_buttonFuns.push_back(C_SEND_MAIL);
                m_buttonFuns.push_back(C_VIEW_PLAYER);
            }
        }
//        GlobalData::shared()->playerInfo.gmFlag = 3;
        if(info.uid!=GlobalData::shared()->playerInfo.uid&&GlobalData::shared()->playerInfo.gmFlag>1){//显示进言按钮
//            if(info.banTime>GlobalData::shared()->getWorldTime()){//ban time
//                CCCommonUtils::setButtonTitle(m_banBtn, _lang("105209").c_str());
//                if(GlobalData::shared()->playerInfo.gmFlag==2){
//                }
//            }else{
                m_buttonFuns.push_back(C_BAN);
//            }
        }
        m_nameLabel->setString(m_name.c_str());
        
        if (m_uid == GlobalData::shared()->playerInfo.uid || m_uid.compare(CHAT_NOTICE_SYS_DIALOG) == 0) {
            m_buttonFuns.clear();
            m_buttonFuns.push_back(C_MSG_COPY);
        }
        int num = m_buttonFuns.size()-1;
        for (int i=num; i>=0; i--) {
            int index = m_buttonFuns[i];
            auto Btn= CCControlButton::create(CCLoadSprite::createScale9Sprite("but_blue.png"));
            Btn->setPreferredSize(CCSize(400, 60));
            Btn->setAnchorPoint(ccp(0.5, 0.5));
            Btn->setPositionY((num-i)*75);
            Btn->setSwallowsTouches(false);
            if (CCCommonUtils::isIosAndroidPad()) {
                Btn->setPreferredSize(CCSize(800, 120));
                Btn->setPositionY((num - i) * 150 + 30);
                Btn->setTitleTTFSizeForState(40, CCControlStateNormal);
                Btn->setTitleTTFSizeForState(40, CCControlStateHighlighted);
                Btn->setTitleTTFSizeForState(40, CCControlStateDisabled);
            }
            CCCommonUtils::setButtonTitle(Btn, _lang(btnNames[index]).c_str());
            m_btnNode->addChild(Btn);
            m_buttonNodes.insert(m_buttonNodes.begin(), Btn);
            if(index==C_BAN){
                m_banBtn = Btn;
//                if(info.uid!=GlobalData::shared()->playerInfo.uid && GlobalData::shared()->playerInfo.gmFlag>1){//显示进言按钮
                    if(info.banTime>GlobalData::shared()->getWorldTime()){//ban time
                        CCCommonUtils::setButtonTitle(Btn, _lang("105209").c_str());
                        if(GlobalData::shared()->playerInfo.gmFlag==2){
                            m_banBtn->setEnabled(false);
                        }
                        Btn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"), CCControlStateDisabled);
                    }
                    else
                    {
                        m_banBtn->setEnabled(true);
                        CCCommonUtils::setButtonTitle(Btn, _lang("105207").c_str());
                    }
                    
//                }
            }
            if(index==C_BLOCK_PLAYER && m_type !=CHAT_COMMENT && m_uid != GlobalData::shared()->playerInfo.uid){
                if (GlobalData::shared()->chatShieldInfo.isShield(m_uid)) {
                    CCCommonUtils::setButtonTitle(Btn, _lang("105315").c_str());
                }else{
                    CCCommonUtils::setButtonTitle(Btn, _lang("105312").c_str());
                }
            }
        }
        int totalH = m_buttonFuns.size()*72 + 100;
        if (CCCommonUtils::isIosAndroidPad()) {
            totalH = m_buttonFuns.size() * 144 + 200;
        }
        m_bg->setContentSize(CCSize(604,totalH));
        m_titleNode->setPositionY(totalH-55);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_titleNode->setPositionY(totalH - 110);
            m_bg->setContentSize(CCSize(1208, totalH));
        }
    }
    return ret;
}

void ChatFunView::onEnter() {
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
	CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatFunView::setBanBtnStatus), MSG_CHAT_BAN, NULL);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	ChatServiceCocos2dx::hideChatInputView();
#endif
}

ChatFunView::~ChatFunView(){
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
//	showNativeTextInput(0.0);
//#endif
}

void ChatFunView::onExit() {
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHAT_BAN);
    PopupBaseView::onExit();
}

bool ChatFunView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void ChatFunView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!isTouchInside(m_bg, pTouch)){
        closeSelf();
        return;
    }
    int index = 0;
    int num = m_buttonNodes.size();
    for (int i=0; i<num; i++) {
        CCNode* node = m_buttonNodes[i];
        if(isTouchInside(node, pTouch)){
            index = i;
            break;
        }
    }
    switch (m_buttonFuns[index]) {
        case C_INVITE:
            onInviteClick(NULL,Control::EventType::TOUCH_DOWN);
            break;
        case C_BLOCK_PLAYER:
            if(m_uid!="" && (m_type == CHAT_COUNTRY || m_type == CHAT_ALLIANCE)){
                onBlockBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            }else if(m_uid!="" && m_type == CHAT_COMMENT){
                YesNoDialog::show( _lang_1("115925",m_name.c_str()) , CCCallFunc::create(this, callfunc_selector(ChatFunView::onYesBlock)));
            }
            break;
        case C_BLOCK_ALLIANCE:
            if(m_allianceId!=""){
                YesNoDialog::show( _lang_1("115926",m_name.c_str()) , CCCallFunc::create(this, callfunc_selector(ChatFunView::onYesBlockAlliance)));
            }
            break;
        case C_MSG_COPY:
            onCopyBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            break;
        case C_SEND_MAIL:
            onMailBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            break;
        case C_VIEW_PLAYER:
            onViewBtnClick(NULL,Control::EventType::TOUCH_DOWN);
            break;
        case C_JOIN:
            onJoinClick(NULL,Control::EventType::TOUCH_DOWN);
            break;
        case C_BAN:
            onBanClick(NULL,Control::EventType::TOUCH_DOWN);
            break;
        default:
            break;
            
    }
    return;
}

void ChatFunView::onYesBlockAlliance(){
    AllianceCommentShieldAdd* cmd = new AllianceCommentShieldAdd("",m_allianceId);
    cmd->sendAndRelease();
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler ChatFunView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCopyBtnClick", ChatFunView::onCopyBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMailBtnClick", ChatFunView::onMailBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onViewBtnClick", ChatFunView::onViewBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBlockBtnClick", ChatFunView::onBlockBtnClick);
////    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelBtnClick", ChatFunView::onCancelBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInviteClick", ChatFunView::onInviteClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJoinClick", ChatFunView::onJoinClick);
    return NULL;
}

bool ChatFunView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleBg", CCNode*, m_titleBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, m_btnNode);
    return false;
}

void ChatFunView::onJoinClick(CCObject * pSender, Control::EventType pCCControlEvent){
    GetUserInfoCommand* cmd = new GetUserInfoCommand(m_uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatFunView::joinFunc), NULL));
    cmd->sendAndRelease();
}

void ChatFunView::setBanBtnStatus(CCObject* obj){
    if(GlobalData::shared()->playerInfo.gmFlag<=1 || m_type==CHAT_COMMENT){
        return;
    }
    ChatInfo info;
    if (m_type==CHAT_COUNTRY){
        info = ChatController::getInstance()->m_chatCountryPool[m_idx];
    }else if(m_type==CHAT_ALLIANCE){
        info = ChatController::getInstance()->m_chatAlliancePool[m_idx];
    }
    if(info.banTime>GlobalData::shared()->getWorldTime()){//ban time
        CCCommonUtils::setButtonTitle(m_banBtn, _lang("105209").c_str());
        if(GlobalData::shared()->playerInfo.gmFlag==2){
            m_banBtn->setEnabled(false);
        }
    }else{
        CCCommonUtils::setButtonTitle(m_banBtn, _lang("105207").c_str());
        m_banBtn->setEnabled(true);
    }
}
void ChatFunView::onBanClick(CCObject * pSender, Control::EventType pCCControlEvent){
    ChatInfo info;
    if (m_type==CHAT_COUNTRY){
        info = ChatController::getInstance()->m_chatCountryPool[m_idx];
    }else{
        info = ChatController::getInstance()->m_chatAlliancePool[m_idx];
    }

    if(info.banTime>GlobalData::shared()->getWorldTime()){//ban time
        if(GlobalData::shared()->playerInfo.gmFlag==3){
            CCCommonUtils::setButtonTitle(m_banBtn, _lang("105207").c_str());
            ChatUnBanCommand* cmd = new ChatUnBanCommand(m_uid);
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatFunView::bandFunc), NULL));
            cmd->sendAndRelease();
        }
    }else{
        auto dialog = YesNoDialog::show( _lang_1("105210",m_name.c_str()) , CCCallFunc::create(this, callfunc_selector(ChatFunView::onYesBan)));
        dialog->showMuteTimeNode();
    }
}
void ChatFunView::onYesBan(){
    if (m_banBtn==nullptr) {
        return ;
    }
    if(ChatController::getInstance()->getBanTime() <0)
    {
        ChatController::getInstance()->setBanTime(3600);
    }
    ChatBanCommand* cmd = new ChatBanCommand(m_uid,ChatController::getInstance()->getBanTime());
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatFunView::bandFunc), NULL));
    cmd->sendAndRelease();
    CCCommonUtils::setButtonTitle(m_banBtn, _lang("105209").c_str());
    m_banBtn->setEnabled(false);
}
void ChatFunView::bandFunc(CCObject* param){
}
void ChatFunView::joinFunc(CCObject* param){
    NetResult* net = dynamic_cast<NetResult*>(param);
    if(net==NULL) return ;
    CCDictionary* dic = dynamic_cast<CCDictionary*>(net->getData());
    if(dic==NULL) return ;
    if(dic->objectForKey("alliance")){
        auto dict = dynamic_cast<CCDictionary*>(dic->objectForKey("alliance"));
        if(dict->objectForKey("alliance")){
            
            auto alliance = dynamic_cast<CCDictionary*>(dict->objectForKey("alliance"));
            if(alliance){
                AllianceInfo* allianceInfo = new AllianceInfo();
                allianceInfo->updateAllianceInfo(alliance);
                PopupViewController::getInstance()->removeAllPopupView();
                PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(allianceInfo));
                allianceInfo->autorelease();
            }
        }
    }
}
void ChatFunView::onInviteClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_uid=="") return ;
    InvitesAllianceCommand* cmd = new InvitesAllianceCommand(m_uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatFunView::inviteFunc), NULL));
    cmd->sendAndRelease();
    PopupViewController::getInstance()->removePopupView(this);
}
void ChatFunView::inviteFunc(CCObject* obj){
    if(m_name=="") return ;
    string name = m_name;
    string msg = _lang_1("115182", name.c_str());
    msg.append("  (").append(_lang("115181")).append(")");CCLOG("invite %s",msg.c_str());
    ChatController::getInstance()->sendCountryChat(msg.c_str(), CHAT_ALLIANCE, 2);
}
void ChatFunView::onCopyBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    cocos2d::extension::CCDevice::clipboardSetText(m_msg);
    PopupViewController::getInstance()->removePopupView(this);
}

void ChatFunView::onMailBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    std::string userName = m_name;
    //    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->removePopupView(this);
//    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create(userName, ""));
    MailController::getInstance()->openMailDialogViewFirst(m_name, m_uid);
}

void ChatFunView::onViewBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_uid=="") {
        PopupViewController::getInstance()->removeAllPopupView();
        return;
    }
    if(m_uid==GlobalData::shared()->playerInfo.uid){
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(&GlobalData::shared()->playerInfo,1));
    }else{
        PopupViewController::getInstance()->removePopupView(this);
        RoleInfoView::createInfoByUid(m_uid);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
        ChatServiceCocos2dx::hideChatInputView();
#endif
//        GetUserInfoCommand* cmd = new GetUserInfoCommand(m_uid);
//        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ChatFunView::callFunc), NULL));
//        cmd->sendAndRelease();
    }
}

void ChatFunView::callFunc(CCObject* param){
    NetResult* net = dynamic_cast<NetResult*>(param);
    if(net==NULL) return ;
    CCDictionary* dic = dynamic_cast<CCDictionary*>(net->getData());
    if(dic==NULL) return ;
    PlayerInfo* info = new PlayerInfo();
    info->updateInfo(dic);
    if(dic->objectForKey("alliance")){
        auto dict = dynamic_cast<CCDictionary*>(dic->objectForKey("alliance"));
        if(dict->objectForKey("alliance")){
            auto alliance = dynamic_cast<CCDictionary*>(dict->objectForKey("alliance"));
            if(alliance){
                info->setAllianceId(alliance->valueForKey("uid")->getCString());
            }
            info->allianceInfo.updateAllianceInfo(alliance);
        }
    }
    PopupViewController::getInstance()->removePopupView(this);
    PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(info,1));
	info->release();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && SHOW_COVER_NATIVE_INPUT
	ChatServiceCocos2dx::hideChatInputView();
#endif
}

void ChatFunView::onBlockBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    //    YesNoDialog::show(_lang("E100008").c_str(), [](){});
    //    return;
    if (m_uid=="") {
        PopupViewController::getInstance()->removePopupView(this);
        return;
    }
    if (GlobalData::shared()->chatShieldInfo.isShield(m_uid)) {
        string _uuid = GlobalData::shared()->chatShieldInfo.getUuid(m_uid);
        ChatUnLockCommand* cmd = new ChatUnLockCommand(_uuid);
        cmd->sendAndRelease();
        PopupViewController::getInstance()->removePopupView(this);
    }
    else {
        auto dialog = YesNoDialog::show( _lang_1("105313",m_name.c_str()) , CCCallFunc::create(this, callfunc_selector(ChatFunView::onYesBlock)));
        dialog->showCancelButton();
    }
}

void ChatFunView::onYesBlock()
{
    if(m_type==CHAT_COMMENT && m_uid!=""){
        AllianceCommentShieldAdd* cmd = new AllianceCommentShieldAdd(m_uid,"");
        cmd->sendAndRelease();
    }else{
        if(m_uid!="") {
            ChatLockCommand* cmd = new ChatLockCommand(m_uid);
            cmd->sendAndRelease();
        }
    }
    PopupViewController::getInstance()->removePopupView(this);
}

//void ChatFunView::onCancelBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
//{
//    PopupViewController::getInstance()->removePopupView(this);
//}