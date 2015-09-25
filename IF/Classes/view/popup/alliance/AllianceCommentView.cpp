//
//  AllianceCommentView.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-14.
//
//

#include "AllianceCommentView.h"
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
#include "../../../Ext/CCDevice.h"
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
#include "AllianceSendCommentCommand.h"
#include "WorldController.h"

const float numPerRow = 1.0;
AllianceCommentView* AllianceCommentView::create(std::string allianceId){
    AllianceCommentView* ret = new AllianceCommentView(allianceId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
	}
	
    return ret;
}

bool AllianceCommentView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("AllianceCommentView",this,this);
    int oldBgHeight = m_buildBG->getContentSize().height;
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    tmpCCB->setContentSize(winsize);
    m_buildBG->setContentSize(CCSize(winsize.width+2,winsize.height));
    m_bg->setContentSize(CCSize(winsize.width+2,winsize.height));
    setContentSize(winsize);
    
    m_oldEditPositionY = m_editBGNode->getPositionY();
    
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    CCCommonUtils::setButtonTitle(m_sendBtn, _lang("115929").c_str());
    type = CHAT_COMMENT;
    m_data = CCArray::create();
    
    m_editBox = InputFieldMultiLine::create(CCSizeMake(460,60),"UI_Alliance_TextBox.png",24);
    m_editBox->setAnchorPoint(ccp(0,0));
    m_editBox->setMaxChars(500);
    m_editBox->setShowLineNum(6);
    m_editBox->setLineNumber(2);
    m_editBox->setFontColor(ccBLACK);
    m_editBox->setPosition(ccp(0,0));
    m_editBox->setSwallowsTouches(true);
    m_editBox->setMoveFlag(true);
    m_editBox->setTouchPriority(1);
    m_editNode->addChild(m_editBox);

    m_editBox->setText(ChatController::getInstance()->m_tmpSaveMsg.c_str());
    m_loadingMoreText->setString(_lang("105502"));
    m_loadingMoreText->setVisible(false);
    m_loadingMoreText->setPositionY(m_loadingMoreText->getPositionY()+addHeight);
    m_loadingMoreStartY = m_loadingMoreText->getPositionY();
    //      kEditBoxInputFlagPassword:  密码形式输入
    //      kEditBoxInputFlagSensitive: 敏感数据输入、存储输入方案且预测自动完成
    //      kEditBoxInputFlagInitialCapsWord: 每个单词首字母大写,并且伴有提示
    //      kEditBoxInputFlagInitialCapsSentence: 第一句首字母大写,并且伴有提示
    //      kEditBoxInputFlagInitialCapsAllCharacters: 所有字符自动大写
    
    ChatController::getInstance()->m_commentPool.clear();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    
    ChatController::getInstance()->idx = 0;
    m_showCount = floor(m_infoList->getContentSize().height/130);
    m_tabView->reloadData();

    m_oldTablePosY = m_tabView->getPositionY();
    m_oldTableViewSize = m_tabView->getViewSize();
    
    string lang = LocalController::shared()->getLanguageFileName();
    GetAllianceCommentsCommand* cmd = new GetAllianceCommentsCommand(m_allianceId,1,lang);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceCommentView::getServerData), NULL));
    cmd->sendAndRelease();
    
    return true;
}

void AllianceCommentView::getServerData(CCObject* data){
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

void AllianceCommentView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("105007"));
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceCommentView::updateChat), MSG_CHAT_UPDATE, NULL);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AllianceCommentView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceCommentView::onEditTextEvent), MSG_INPUT_TEXT_EVENT, NULL);
	CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceCommentView::popupCloseEvent), MSG_POPUP_CLOSE_EVENT, NULL);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	scheduleOnce(schedule_selector(AllianceCommentView::showNativeTextInput), 0.01f);
#endif
}

void AllianceCommentView::showNativeTextInput(float time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ChatServiceCocos2dx::setSendMessageListener(this);
	
	CCLOG("AllianceCommentView::showNativeTextInput");
	ChatServiceCocos2dx::showChatInputView();
	ChatServiceCocos2dx::enableChatInputView();
#endif
}

void AllianceCommentView::popupCloseEvent(CCObject *params)
{
	int c = PopupViewController::getInstance()->getCurrViewCount();
	CCLOG("AllianceCommentView::popupCloseEvent getCurrViewCount: %d", c);
	
	if(type == CHAT_COMMENT && c == 1)
	{
		//关闭加入联盟的提醒
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		ChatServiceCocos2dx::showChatInputView();
		ChatServiceCocos2dx::enableChatInputView();
#endif
	}
}

void AllianceCommentView::setHeightFromNative(int height, int usableHeightSansKeyboard)
{
}

void AllianceCommentView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHAT_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_OUT);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(AllianceCommentView::onEnterFrame), this);
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_ADD_LINE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_TEXT_EVENT);
	CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, INPUTFIELD_CLOSE_EVENT);
	CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_CLOSE_EVENT);
    auto _currScene = CCDirector::sharedDirector()->getRunningScene();
    if (_currScene->getPositionY() != 0) {
        _currScene->setPositionY(0);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ChatServiceCocos2dx::setSendMessageListener(NULL);
	ChatController::getInstance()->m_tmpSaveMsg = ChatServiceCocos2dx::getChatInputText();
	
	ChatServiceCocos2dx::disableChatInputView();
	ChatServiceCocos2dx::hideChatInputView();
#else
	ChatController::getInstance()->m_tmpSaveMsg = m_editBox->getText();
#endif
    
    CCNode::onExit();
}
void AllianceCommentView::onAddlineEvent(CCObject * params)
{
    int changeHeight = dynamic_cast<CCInteger*>(params)->getValue();
    m_tabView->setPositionY(m_tabView->getPositionY()+changeHeight);
    m_tabView->setViewSize(CCSizeMake(m_tabView->getViewSize().width, m_tabView->getViewSize().height-changeHeight));
    m_editBGNode->setPositionY(m_editBGNode->getPositionY()+changeHeight);
}

void AllianceCommentView::onEditTextEvent(cocos2d::CCObject *params)
{
    string num = CC_ITOA( dynamic_cast<CCInteger*>(params)->getValue());
    m_inputNum->setString(num + "/500");
}
bool AllianceCommentView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_editBox, pTouch)) {
        CCLOG("isTouchInside");
    }
    m_tabView->setBounceable(true);
    CCLOG("isTouchInside false");
    return true;
}

void AllianceCommentView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    auto _currScene = CCDirector::sharedDirector()->getRunningScene();
    if (_currScene->getPositionY() != 0) {
        _currScene->setPositionY(0);
    }
}

void AllianceCommentView::onEnterFrame(float dt)
{
    delayTime++;
}

SEL_CCControlHandler AllianceCommentView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPostBtnClick", AllianceCommentView::onPostBtnClick);
    return NULL;
}

bool AllianceCommentView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, this->m_editNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBg", CCScale9Sprite*, this->m_editBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBGNode", CCNode*, this->m_editBGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputNum", CCLabelIF*, this->m_inputNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchEditNode", CCNode*, m_touchEditNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingMoreText", CCLabelIF*, m_loadingMoreText);
    return false;
}
void AllianceCommentView::onPostBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    if (delayTime<ChatController::getInstance()->chat_interval)
    {
        CCCommonUtils::flyText(_lang("105307"));
        return;
    }
    delayTime=0;
    
    m_sendMsg = m_editBox->getText();
    string lang = LocalController::shared()->getLanguageFileName();
    AllianceSendCommentCommand* cmd = new AllianceSendCommentCommand(m_allianceId,m_sendMsg,lang);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceCommentView::callBackSuccess), NULL));
    cmd->sendAndRelease();
    m_editBox->setText("");
    m_editBox->closeIME();
    m_editBox->openIME();
    m_tabView->setPositionY(m_oldTablePosY);
    m_tabView->setViewSize(m_oldTableViewSize);
    m_editBGNode->setPositionY(m_oldEditPositionY);
}

void AllianceCommentView::callBackSuccess(CCObject* data){
    ChatInfo info;
    info.uid = GlobalData::shared()->playerInfo.uid;
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        info.asn = GlobalData::shared()->playerInfo.allianceInfo.shortName;
    }
    info.senderPic = GlobalData::shared()->playerInfo.pic;
    info.senderPicVer = GlobalData::shared()->playerInfo.picVer;
    info.msg = m_sendMsg;
    info.name = GlobalData::shared()->playerInfo.name;
    info.type = CHAT_COMMENT;
    info.allianceId = GlobalData::shared()->playerInfo.allianceInfo.uid;
    info.time = WorldController::getInstance()->getTime()/1000.0;
    
    ChatController::getInstance()->m_commentPool.push_back(info);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_UPDATE);
}

void AllianceCommentView::sendMessage(string msg)
{
	CCLOG("AllianceCommentView::sendMessage");
	if (delayTime<ChatController::getInstance()->chat_interval)
	{
		CCCommonUtils::flyText(_lang("105307"));
		return;
	}
	delayTime=0;
    m_sendMsg = msg;
    string lang = LocalController::shared()->getLanguageFileName();
    AllianceSendCommentCommand* cmd = new AllianceSendCommentCommand(m_allianceId,m_sendMsg,lang);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceCommentView::callBackSuccess), NULL));
    cmd->sendAndRelease();
}

void AllianceCommentView::updateChat(CCObject* obj)
{
	CCLOG("AllianceCommentView::updateChat()");
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
        m_tabView->setContentOffset(curr);
    }
    m_tabView->setTouchEnabled(true);
}

void AllianceCommentView::scrollViewDidScroll(CCScrollView* view)
{

}

void AllianceCommentView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}
CCSize AllianceCommentView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    int height = 0;
    auto& info = ChatController::getInstance()->m_commentPool[idx];
    
    height = 130;
    int _tmpHeight = getTextDimension(info.msg.c_str()).height;
    if (_tmpHeight > 60) {
        height += _tmpHeight - 60;
    }
    if (idx > 0)
    {
        time_t lt = ChatController::getInstance()->m_commentPool[idx-1].time;
        if(info.isTimeShowed&&info.showedTime!=""){
            
        }else if ((info.time-lt) < ChatController::getInstance()->chat_time) {
            height -= 30;
        }
    }
    return CCSize(640,height);
}
CCSize AllianceCommentView::getTextDimension(const char *text)
{
    int len = (int)strlen(text);
    if ((len==0)  ||  (len==1 && text[0] == '\n')) {
        CCLabelTTF *label = CCLabelTTF::create( "sample" , "Helvetica", 20 );
        label->setDimensions(CCSizeMake(470, 0));
        CCSize size = label->getContentSize();
        size.width = 0;
        return size;
    }
    CCLabelTTF *label = CCLabelTTF::create( text, "Helvetica", 20);
    label->setDimensions(CCSizeMake(470, 0));
    return label->getContentSize();
}

cocos2d::CCSize AllianceCommentView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 130);
}

CCTableViewCell* AllianceCommentView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if (idx >= ChatController::getInstance()->m_commentPool.size()) {
        return NULL;
    }
    ChatCell* cell = (ChatCell*)table->dequeueCell();
    if(cell){
        cell->setData(type,idx);
    }else{
        cell = ChatCell::create(type, idx, m_infoList);
    }
    return cell;
}

ssize_t AllianceCommentView::numberOfCellsInTableView(CCTableView *table){
    m_cellNum = ceil(ChatController::getInstance()->m_commentPool.size() / numPerRow);
    return m_cellNum;
}
