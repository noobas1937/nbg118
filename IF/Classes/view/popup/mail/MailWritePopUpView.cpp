//
//  MailWritePopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-11.
//
//

#include "MailWritePopUpView.h"
#include "MailController.h"
#include "PopupViewController.h"
#include "MailGiftView.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "MailSendCommand.h"
#include "AllianceInviteView.h"
#include "ChatServiceCocos2dx.h"
#include "ChooseUserView.h"
#include "Utf8Utils.h"

//写邮件界面对象
void MailWritePopUpView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailWritePopUpView::onCloseEvent), MAIL_SEND_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailWritePopUpView::onUpdateStatusEvent), MSG_INPUT_TEXT_EVENT, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailWritePopUpView::onUpdateStatusEvent), MSG_MAIL_GIFT_CHOOSE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailWritePopUpView::onSendGiftBack), MAIL_SEND_GIFT_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailWritePopUpView::onRefreshName), CHAT_ROOM_NAME_REFRESH, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailWritePopUpView::onChoosePlayer), MSG_USER_CHOOSE, NULL);
    setTitleName(_lang("105513"));
    if(mInited==true){
        m_nameEditBox->addTargetedDelegate();
        m_contentEditBox->addTargetedDelegate();
    }
    if(mPropID == ITEM_SEND_GIFTALLIANCE){
        setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        setTouchEnabled(true);
        //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    }
    mInited=true;
    MailController::getInstance()->getAllianceMember();
}

void MailWritePopUpView::onExit(){
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_SEND_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_TEXT_EVENT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAIL_GIFT_CHOOSE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_SEND_GIFT_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHAT_ROOM_NAME_REFRESH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USER_CHOOSE);
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

MailWritePopUpView* MailWritePopUpView::create(std::string toName, std::string title,std::string allianceId,std::string thxMail){
    MailWritePopUpView* ret = new MailWritePopUpView(toName, title,allianceId,thxMail);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MailWritePopUpView* MailWritePopUpView::createWithGift(std::string toName, int giftid,int propID){
    MailWritePopUpView* ret = new MailWritePopUpView(toName,"","","");
    if(ret && ret->initWithGift(giftid,propID)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool MailWritePopUpView::initWithGift(int giftID,int propID){
    if(!PopupBaseView::init()){
        return false;
    }
    mPropID = propID;
    mType = 1;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    auto bg = CCBLoadFile("MailWriteView", this, this);
    this->setContentSize(bg->getContentSize());
    setTitleName(_lang("105513"));
    int preHeight = this->m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    this->m_downNode->setPositionY(m_downNode->getPositionY() - dh);
    this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
    CCSize liseNodeSize = this->m_listNode->getContentSize();
    this->m_listNode->setContentSize(CCSize(liseNodeSize.width, liseNodeSize.height + dh));
    m_nodeContent->setPositionY(m_nodeContent->getPositionY() + dh);
    CCSize wNodeSize = this->m_waitingNode->getContentSize();
    m_waitingNode->setContentSize(CCSize(wNodeSize.width,wNodeSize.height + dh));
    m_waitingNode->setPositionY(m_waitingNode->getPositionY() - dh*0.5);
    
    this->m_nameText->setString(_lang("105505").c_str());
    //    this->m_titleText->setString(_lang("105503").c_str());
    this->m_contentText->setString(_lang("105504").c_str());
    
    CCNode *searchnode = CCNode::create();
    auto spr1 = CCLoadSprite::createScale9Sprite("button_dark.png");
    CCControlButton *shareBtn = CCControlButton::create(spr1);
    searchnode->addChild(shareBtn);
    shareBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailWritePopUpView::onChooseName), CCControlEventTouchUpInside);
    CCSprite* sprIcon = CCLoadSprite::createSprite("search_icon.png");
    CCSize sprSize = sprIcon->getContentSize();
    searchnode->addChild(sprIcon);
    CCCommonUtils::setSpriteMaxSize(sprIcon, 45);
    shareBtn->setPreferredSize(CCSize(70, 70));
    this->m_nameContainer->addChild(searchnode);
    searchnode->setPosition(440, 0);
    
    CCScrollView *scrollView = CCScrollView::create(m_listNode->getContentSize());
    scrollView->setTouchPriority(Touch_Popup_Item);
    scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_nodeContent->removeFromParentAndCleanup(false);
    scrollView->addChild(m_nodeContent);
    scrollView->setTag(10000);
    m_nameEditBox = InputFieldMultiLine::create(CCSizeMake(380,40),"UI_Alliance_TextBox.png",24);
//    m_nameEditBox->setTouchPriority(Touch_Default);
    m_nameEditBox->setMaxChars(30);
    m_nameEditBox->setPosition(ccp(0, -20));
    m_nameEditBox->setLineNumber(1);
    m_nameEditBox->setFontColor({63,44,27});
    
    this->m_nameContainer->addChild(m_nameEditBox);
    m_nameEditBox->m_enabled = (mPropID == ITEM_SEND_GIFTALLIANCE)?false:true;
    
    m_contentEditBox = InputFieldMultiLine::create(CCSizeMake(475,200),"UI_Alliance_TextBox.png",24);
    m_contentEditBox->setTouchPriority(Touch_Default);
    m_contentEditBox->setMaxChars(300);
    m_contentEditBox->setLineNumber(5);
    m_contentEditBox->setPosition(ccp(0, -200));
    m_contentEditBox->setFontColor({63,44,27});
    this->m_contentContainer->addChild(m_contentEditBox);
    m_contentEditBox->setText(_lang("101383"));
    liseNodeSize = this->m_listNode->getContentSize();
    MailGiftCell *mailGiftCell = MailGiftCell::create(CCSize(liseNodeSize.width, liseNodeSize.height - 110),mPropID,giftID);
    scrollView->addChild(mailGiftCell);
    mailGiftCell->setTag(10000);
    CCSize cellSize = mailGiftCell->getContentSize();
    int needH = ceil(cellSize.height + 400);
    int nowH = m_listNode->getContentSize().height;
    m_nodeContent->setPosition(ccp(300,cellSize.height + 100));
    mailGiftCell->setPosition(ccp(0,0));
    m_listNode->addChild(scrollView);
    scrollView->setContentSize(CCSize(600,400 + cellSize.height));
    scrollView->setContentOffset(ccp(0, nowH - needH));
    
    if(needH>nowH){
        scrollView->setTouchEnabled(true);
    }else{
        scrollView->setTouchEnabled(false);
    }
    
    if(!m_toName.empty()){
        m_nameEditBox->setText(m_toName.c_str());
        m_nameEditBox->setEnabled(false);
    }
    updateBtnStatus();

    return true;
}
bool MailWritePopUpView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    mType = 0;
    auto bg = CCBLoadFile("MailWriteView", this, this);
    this->setContentSize(bg->getContentSize());
    setTitleName(_lang("105513"));
    int preHeight = this->m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    this->m_downNode->setPositionY(m_downNode->getPositionY() - dh);
    this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
    
    this->m_nameText->setString(_lang("105505").c_str());
//    this->m_titleText->setString(_lang("105503").c_str());
    this->m_contentText->setString(_lang("105504").c_str());
    
//    m_nameEditBox->setReturnType(kKeyboardReturnTypeDone);
//    m_nameEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)  
    if(MailController::getInstance()->getChatRoomEnable() && GlobalData::shared()->playerInfo.isInAlliance())
    {
        m_nameEditBox = InputFieldMultiLine::create(CCSizeMake(410,40),"UI_Alliance_TextBox.png",24);
        m_nameEditBox->setMaxChars(300);
        m_nameEditBox->setPosition(ccp(0, -20));
        m_nameEditBox->setLineNumber(1);
        m_nameEditBox->setFontColor({63,44,27});
        
        auto inviteImg = CCLoadSprite::createScale9Sprite("icon_add_mumber.png");
        m_inviteBtn = CCControlButton::create(inviteImg);
        m_inviteBtn->setPreferredSize(CCSize(40, 40));
        m_inviteBtn->setPosition(ccp(430,0));
        m_inviteBtn->setAnchorPoint(ccp(0,0.5));
        m_inviteBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailWritePopUpView::onInvitePlayer), CCControlEventTouchUpInside);
        this->m_nameContainer->addChild(m_inviteBtn);
    }
    else
    {
        m_nameEditBox = InputFieldMultiLine::create(CCSizeMake(475,40),"UI_Alliance_TextBox.png",24);
        m_nameEditBox->setMaxChars(30);
        m_nameEditBox->setPosition(ccp(0, -20));
        m_nameEditBox->setLineNumber(1);
        m_nameEditBox->setFontColor({63,44,27});
    }
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if (!ChatServiceCocos2dx::enableChatRoom){
        m_nameEditBox = InputFieldMultiLine::create(CCSizeMake(475,40),"UI_Alliance_TextBox.png",24);
        m_nameEditBox->setMaxChars(30);
        m_nameEditBox->setPosition(ccp(0, -20));
        m_nameEditBox->setLineNumber(1);
        m_nameEditBox->setFontColor({63,44,27});
    }else{
        if(GlobalData::shared()->playerInfo.isInAlliance())
        {
            m_nameEditBox = InputFieldMultiLine::create(CCSizeMake(410,40),"UI_Alliance_TextBox.png",24);
            m_nameEditBox->setMaxChars(300);
            m_nameEditBox->setPosition(ccp(0, -20));
            m_nameEditBox->setLineNumber(1);
            m_nameEditBox->setFontColor({63,44,27});
            //显示加号按钮
            auto inviteImg = CCLoadSprite::createScale9Sprite("icon_add_mumber.png");
            m_inviteBtn = CCControlButton::create(inviteImg);
            m_inviteBtn->setPreferredSize(CCSize(40, 40));
            m_inviteBtn->setPosition(ccp(430,0));
            m_inviteBtn->setAnchorPoint(ccp(0,0.5));
            m_inviteBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MailWritePopUpView::onInvitePlayer), CCControlEventTouchUpInside);
            this->m_nameContainer->addChild(m_inviteBtn);
        }
        else
        {
            m_nameEditBox = InputFieldMultiLine::create(CCSizeMake(475,40),"UI_Alliance_TextBox.png",24);
            m_nameEditBox->setMaxChars(30);
            m_nameEditBox->setPosition(ccp(0, -20));
            m_nameEditBox->setLineNumber(1);
            m_nameEditBox->setFontColor({63,44,27});
        }

    }
    
   #endif
    
    this->m_nameContainer->addChild(m_nameEditBox);

//    m_titleEditBox = InputFieldMultiLine::create(CCSizeMake(475,40),"UI_Alliance_TextBox.png",24);
//    m_titleEditBox->setMaxChars(30);
//    m_titleEditBox->setLineNumber(1);
//    m_titleEditBox->setPosition(ccp(0, -20));
//    m_titleEditBox->setFontColor({63,44,27});
////    m_titleEditBox->setReturnType(kKeyboardReturnTypeDone);
////    m_titleEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
//    this->m_titleContainer->addChild(m_titleEditBox);
    
    m_contentEditBox = InputFieldMultiLine::create(CCSizeMake(475,200),"UI_Alliance_TextBox.png",24);
    m_contentEditBox->setMaxChars(300);
    m_contentEditBox->setLineNumber(5);
    m_contentEditBox->setPosition(ccp(0, -200));
//    m_contentEditBox->setFontSize(24);
//    m_contentEditBox->setMaxLength(300);
    m_contentEditBox->setFontColor({63,44,27});
//    m_contentEditBox->setReturnType(kKeyboardReturnTypeDone);
//    m_contentEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    this->m_contentContainer->addChild(m_contentEditBox);
    
    if(!m_toName.empty()){
        m_nameEditBox->setText(m_toName.c_str());
        if(!m_allianceId.empty() || !m_thanksMail.empty()){
            m_nameEditBox->setEnabled(false);
        }
    }
    if(!m_thanksMail.empty()){
        m_contentEditBox->setText(_lang("101387"));
    }
    if(m_title != ""){
//        m_titleEditBox->setText(LocalController::shared()->TextINIManager()->getObjectByKey("105511", "1", m_title.c_str()).c_str());
    }
    updateBtnStatus();
//    m_waitInterface = NULL;
    return true;
}

bool MailWritePopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleContainer", CCNode*, this->m_titleContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentContainer", CCNode*, this->m_contentContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameContainer", CCNode*, this->m_nameContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, this->m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeContent", CCNode*, this->m_nodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprMailIcon", CCSprite*, this->m_sprMailIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitingNode", CCNode*, this->m_waitingNode);
    return false;
}

SEL_CCControlHandler MailWritePopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", MailWritePopUpView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendClick", MailWritePopUpView::onSendClick);
    return NULL;
}

void MailWritePopUpView::onSendGiftBack(cocos2d::CCObject *p){
    if(p){
        CCInteger *inter = dynamic_cast<CCInteger*>(p);
        if(inter){
            if(inter->getValue() == 0){
                m_isSendingGift=false;
//                if(m_waitInterface){
//                    m_waitInterface->remove();
//                    m_waitInterface = NULL;
//                }
            }else{
                onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
            }
        }
    }
}
void MailWritePopUpView::onCloseEvent(cocos2d::CCObject *p){
    onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
}

void MailWritePopUpView::onCloseClick(cocos2d::CCObject *pSender, CCControlEvent event){
    MailController::getInstance()->setChatRoomInfo("","","");
    m_isSendingGift=false;
//    if(m_waitInterface){
//        m_waitInterface->remove();
//        m_waitInterface = NULL;
//    }
    PopupViewController::getInstance()->goBackPopupView();
}

void MailWritePopUpView::onSendClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if (MailController::getInstance()->m_inviteMemberName!="") {
        MailController::getInstance()->createChatRoom(m_contentEditBox->getText());
    }
    else
    {
        if(mType ==1){
            if(m_nameEditBox->getText().empty())
                return;
            if(m_contentEditBox->getText().empty())
                return;
            if(m_listNode->getChildByTag(10000)){
                CCScrollView *scroll = dynamic_cast<CCScrollView*>(m_listNode->getChildByTag(10000));
                if(scroll){
                    if(scroll->getContainer()->getChildByTag(10000)){
                        MailGiftCell* cell = dynamic_cast<MailGiftCell*>(scroll->getContainer()->getChildByTag(10000));
                        if(cell && cell->getItemID()>0){
                            auto& toolinfo= ToolController::getInstance()->getToolInfoById(cell->getItemID());

                            if(mUserCount>0 && toolinfo.getCNT()<mUserCount){
                                YesNoDialog::showYesDialog(_lang("101014").c_str());
                            }else{
                                std::string toolname = toolinfo.getName();
                                std::string dialog = _lang_2("101385", toolname.c_str(), m_nameEditBox->getText().c_str());
                                YesNoDialog::show(dialog.c_str() , CCCallFunc::create(this, callfunc_selector(MailWritePopUpView::onSendGift)));
                            }
                        }
                    }
                }
            }
        }else{
            string content = m_contentEditBox->getText();
            string title = content;
            int len = content.length();
            if(len>30){
                title =content.substr(0,29);
            }
            if(MailController::getInstance()->sendMail(m_nameEditBox->getText(), title, content,m_allianceId,"",false,MAIL_SELF_SEND,m_thanksMail)){
                //        PopupViewController::getInstance()->goBackPopupView();
            }
        }
    }
}
void MailWritePopUpView::updateBtnStatus(){
    if(m_nameEditBox->getText().empty() || m_contentEditBox->getText().empty()){
        m_sendBtn->setEnabled(false);
        m_sprMailIcon->setColor({90,85,81});
        
    }else{
        if(mType == 1){
            if(m_listNode->getChildByTag(10000)){
                CCScrollView *scroll = dynamic_cast<CCScrollView*>(m_listNode->getChildByTag(10000));
                if(scroll){
                    if(scroll->getContainer()->getChildByTag(10000)){
                        MailGiftCell* cell = dynamic_cast<MailGiftCell*>(scroll->getContainer()->getChildByTag(10000));
                        if(cell && cell->getItemID()>0){
                            m_sendBtn->setEnabled(true);
                            m_sprMailIcon->setColor(ccWHITE);
                            if(mPropID == ITEM_SEND_GIFTALLIANCE && mUserCount>0){
                                auto &tool = ToolController::getInstance()->getToolInfoById(cell->getItemID());
                                int showCount = (tool.getCNT() < mUserCount)?
                                tool.getCNT(): mUserCount;
                                cell->setItemCount(showCount);
                            }
                        }else{
                            m_sendBtn->setEnabled(false);
                            m_sprMailIcon->setColor({90,85,81});
                        }
                    }
                }
            }
        }else{
            m_sendBtn->setEnabled(true);
            m_sprMailIcon->setColor(ccWHITE);
        }
    }
}
void MailWritePopUpView::onChoosePlayer(cocos2d::CCObject *p){
    if(p){
        CCString* str = dynamic_cast<CCString*>(p);
        if(str){
            m_nameEditBox->setText(str->getCString());
            updateBtnStatus();
        }else{
            CCArray *array = dynamic_cast<CCArray*>(p);
            if(array){
                if (array->count()>2) {
                    CCString *uids = dynamic_cast<CCString*>(array->objectAtIndex(0));
                    if(uids && uids->getCString()){
                        mChooseUids = uids->getCString();
                    }
                    CCString *names = dynamic_cast<CCString*>(array->objectAtIndex(1));
                    CCInteger *count = dynamic_cast<CCInteger*>(array->objectAtIndex(2));
                    mUserCount = count->getValue();
                    if(names && names->getCString()){
                        std::string showStr = "("+ CC_CMDITOA(mUserCount) +")" + names->getCString();
                        CCLabelIF* lbl = CCLabelIF::create(showStr.c_str(), 24);
                        if(lbl->getContentSize().width>300){
                            float fscale = 300 / lbl->getContentSize().width;
                            int count = floor(Utf8Utils::strlen(showStr.c_str()) * fscale) - 3;
                            showStr = CCCommonUtils::subStrByUtf8(showStr, 0, count);
                            showStr += " ...";
                        }
                        m_nameEditBox->setText(showStr);
                    }
                    array->release();
                    updateBtnStatus();
                }
            }
        }
    }
}
void MailWritePopUpView::onUpdateStatusEvent(cocos2d::CCObject *p){
    updateBtnStatus();
}
void MailWritePopUpView::onSendGift(){
//    if(m_waitInterface)
    if(m_isSendingGift==true)
        return;
    CCScrollView *scroll = dynamic_cast<CCScrollView*>(m_listNode->getChildByTag(10000));
    if(scroll){
        if(scroll->getContainer()->getChildByTag(10000)){
            MailGiftCell* cell = dynamic_cast<MailGiftCell*>(scroll->getContainer()->getChildByTag(10000));
            if(cell && cell->getItemID()>0){
                MailGiftSendCommand* cmd = NULL;
                switch (mPropID) {
                    case ITEM_SEND_GIFTALLIANCE:{
                        auto &info = ToolController::getInstance()->getToolInfoById(cell->getItemID());
                        if(info.getCNT()>=mUserCount){
                            cmd = new MailGiftSendCommand(m_nameEditBox->getText(),mPropID,cell->getItemID(),m_contentEditBox->getText(),mChooseUids);
                        }else{
                            YesNoDialog::showYesDialog(_lang("101014").c_str());
                            return;
                        }
                        break;
                    }
                    case ITEM_SEND_GIFTMORE:{
                        if(cell->getItemCount()>0){
                            cmd = new MailGiftSendCommand(m_nameEditBox->getText(),mPropID,cell->getItemID(),m_contentEditBox->getText(),"",cell->getItemCount());
                        }
                        break;
                    }
                    default:{
                        cmd = new MailGiftSendCommand(m_nameEditBox->getText(),mPropID,cell->getItemID(),m_contentEditBox->getText());
                        break;
                    }
                }

                if(cmd!=NULL){
                    cmd->sendAndRelease();
                    m_isSendingGift=true;
                }
            }
        }
    }
}
void MailWritePopUpView::onChooseName(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_isSendingGift==true)
        return;
    if(mPropID == ITEM_SEND_GIFT || mPropID == ITEM_SEND_GIFTMORE){
        auto pop = AllianceInviteView::create(0, ITEM_GIFT);
        PopupViewController::getInstance()->addPopupInViewWithAnim(pop);
    }else if(mPropID == ITEM_SEND_GIFTALLIANCE){
        showChooseView();
    }
}
void MailWritePopUpView::showChooseView(){
    int maxCount = -1;
    CCScrollView *scroll = dynamic_cast<CCScrollView*>(m_listNode->getChildByTag(10000));
    if(scroll){
        if(scroll->getContainer()->getChildByTag(10000)){
            MailGiftCell* cell = dynamic_cast<MailGiftCell*>(scroll->getContainer()->getChildByTag(10000));
            if(cell && cell->getItemID()>0){
                auto &info = ToolController::getInstance()->getToolInfoById(cell->getItemID());
                maxCount = info.getCNT();
            }
        }
    }
    auto pop = ChooseUserView::create(USER_SOURCE_ALLIANCE,maxCount,"101014",mChooseUids);
    PopupViewController::getInstance()->addPopupInViewWithAnim(pop);
}
void MailWritePopUpView::onRefreshName(cocos2d::CCObject *p){
    
    string name=MailController::getInstance()->m_inviteMemberName;
    if (name!="") {
        if (name.length()>20) {
            name=CCCommonUtils::subStrByUtf8(name,0,20);
            name.append("...");
        }
        m_nameEditBox->setText(name);
        m_sendBtn->setEnabled(true);
        m_sprMailIcon->setColor(ccWHITE);
    }
}

void MailWritePopUpView::onInvitePlayer(cocos2d::CCObject *pSender, CCControlEvent event){
    CCLOGFUNC("");
    MailController::getInstance()->setChatRoomInfo("","","");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::showMemberSelectorFrom2dx();
#elif  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //显示多人聊天选人界面
    
    ChatServiceCocos2dx::groupChatVCInit();
#endif
}

bool MailWritePopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_nameEditBox, pTouch)){
        return true;
    }
    return false;
}
void MailWritePopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void MailWritePopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_nameEditBox, pTouch)){
        showChooseView();
    }
}