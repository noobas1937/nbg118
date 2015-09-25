//
//  InviteForGiftView.cpp
//  IF
//
//  Created by xxrdsg on 14-11-20.
//
//

#include "InviteForGiftView.h"
#include "PopupViewController.h"
#include "InviteForGiftCommand.h"
#include "CommandBase.h"
#include "NetController.h"
#include "CCCommonUtils.h"
#include "MailController.h"
#include "../../Ext/CCDevice.h"
#include "GameController.h"
#include "FlyHint.h"
#include "FunBuildController.h"
#include "FBAppRequestFriendView.h"
#include "FacebookInviteView.h"
#include "FBUtilies.h"
#include <spine/Json.h>
#include "UserBindCommand.h"
#include "FBCheckRequestCommand.h"
#include "GCMRewardController.h"
#include "BranchController.h"


InviteForGiftView::InviteForGiftView(){
   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InviteForGiftView::refreshData), FB_ACCEPT_APP_REQUEST, NULL);
   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InviteForGiftView::loginSuccess), MSG_FBLoginSucess, NULL);
   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InviteForGiftView::bindSuccess), MSG_USER_BIND_OK, NULL);
   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InviteForGiftView::getRequestFriends), FB_APP_FriendsList, NULL);
   
   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InviteForGiftView::afterGetFriendsInfo), MSG_FBFriendsList, NULL);
   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InviteForGiftView::getInviteFriends), MSG_FBIviteFriends, NULL);
   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InviteForGiftView::getBranchUrl), "branch", NULL);
};

InviteForGiftView::~InviteForGiftView(){
   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FB_ACCEPT_APP_REQUEST);
   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBLoginSucess);
   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USER_BIND_OK);
   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FB_APP_FriendsList);
   
   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFriendsList);
   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBIviteFriends);
   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "branch");
};

void InviteForGiftView::getRequestFriends(CCObject *data){
   GameController::getInstance()->removeWaitInterface();
   CCArray* arr = dynamic_cast<CCArray*>(data);
   if (arr!=NULL) {
      m_requestArr->removeAllObjects();
      int num = arr->count();
      for (int i=0; i<num; i++) {
         CCDictionary* friendInfo = _dict(arr->objectAtIndex(i));
//         friendInfo->writeToFile("/Users/zhangjun/Desktop/dictf/friendInfoD");
         string fromId = friendInfo->valueForKey("fromId")->getCString();
//         string action_type = friendInfo->valueForKey("action_type")->getCString();
//         if (action_type=="send" || action_type=="invite") {
            if (m_requestUids.find(fromId) < m_requestUids.length()) {
               // 有相同的了
               continue;
            }
            m_requestUids.append(",");
            m_requestUids.append(fromId);
            m_requestArr->addObject(friendInfo);
//         }
      }
   }else{
      this->scheduleOnce(schedule_selector(InviteForGiftView::delayShowData), 0.1);
   }
   this->scheduleOnce(schedule_selector(InviteForGiftView::delayShowAppRequestView), 0.2);
}

void InviteForGiftView::delayShowAppRequestView(float t){
   string ids = "";
   int num = m_requestArr->count();
   for(int i=0;i<num;i++){
      CCDictionary* friendInfo = _dict(m_requestArr->objectAtIndex(i));
      string fromId = friendInfo->valueForKey("fromId")->getCString();
      ids.append(fromId);
      if (ids!="" && i!=(num-1)) {
         ids.append(",");
      }
   }
   if(ids!=""){
      FBCheckRequestCommand* cmd = new FBCheckRequestCommand(ids);
      cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(InviteForGiftView::getServerCallBack), NULL));
      cmd->sendAndRelease();
   }else{
      GameController::getInstance()->removeWaitInterface();
      CCArray* arr = CCArray::create();
      if(m_fbClick){
//         PopupViewController::getInstance()->addPopupInView(FBAppRequestFriendView::create(arr));
         m_data->addObjectsFromArray(arr);
         refreshTableView();
      }else{
         CCLog("fb no find binding friend");
      }
      m_fbClick = false;
      m_fbNode->setVisible(false);
      m_upBgSpr1->setVisible(false);
      m_tableViewNode->setVisible(true);
   }
}

void InviteForGiftView::getServerCallBack(CCObject *data){
   NetResult* result = dynamic_cast<NetResult*>(data);
   if (result==NULL) {
      return ;
   }
   GameController::getInstance()->removeWaitInterface();
   CCArray* arr = CCArray::create();
   CCDictionary* params = _dict(result->getData());
   if(params->objectForKey("unknown")){
      string unknown = params->valueForKey("unknown")->getCString();
      int num = m_requestArr->count();
      for(int i=0;i<num;i++){
         CCDictionary* friendInfo = _dict(m_requestArr->objectAtIndex(i));
         string fromId = friendInfo->valueForKey("fromId")->getCString();
         if (unknown.find(fromId) < unknown.length()) {
            // 没有找到
            continue;
         }else{
            arr->addObject(friendInfo);
         }
      }
   }
   if(m_fbClick || arr->count()>0){
//      PopupViewController::getInstance()->addPopupInView(FBAppRequestFriendView::create(arr));
      m_data->addObjectsFromArray(arr);
      refreshTableView();
   }else{
      CCLog("no find binding friend");
   }
   m_fbClick = false;
   m_fbNode->setVisible(false);
   m_upBgSpr1->setVisible(false);
   m_tableViewNode->setVisible(true);
}

void InviteForGiftView::refreshTableView(CCObject *obj){
   if (m_data->count()<1) {
      m_tvTipTxt->setString(_lang("107104"));
      m_tvTipTxt->setVisible(true);
   }else{
      m_tvTipTxt->setVisible(false);
      m_tabView->reloadData();
   }
}

void InviteForGiftView::delayShowData(float t){
   m_requestArr->removeAllObjects();
   string friends = CCUserDefault::sharedUserDefault()->getStringForKey("appRequestFriends","");
   if(friends!=""){
      CCLOG("fb appRequestFriends friends =%s",friends.c_str());
      Json* fjson = Json_create(friends.c_str());
      int size = Json_getSize(fjson);
      for (int i=0; i<size; i++) {
         Json *item = Json_getItemAt(fjson, i);
//         string action_type = Json_getString(item,"action_type","");
//         if (action_type=="send" || action_type=="invite") {
            string id = Json_getString(item,"id","");
            string created_time = Json_getString(item,"created_time","");
            string fromId = "";
            string fromName = "";
            Json* from = Json_getItem(item,"from");
            if(from){
               fromId = Json_getString(from,"id","");
               fromName = Json_getString(from,"name","");
            }
            if (m_requestUids.find(fromId) < m_requestUids.length()) {
               // 有相同的了
               continue;
            }else{
               m_requestUids.append(",");
               m_requestUids.append(fromId);
            }
            CCDictionary* friendInfo = CCDictionary::create();
            friendInfo->setObject(CCString::create(id.c_str()), "id");
            friendInfo->setObject(CCString::create(created_time.c_str()), "created_time");
            friendInfo->setObject(CCString::create(fromId.c_str()), "fromId");
            friendInfo->setObject(CCString::create(fromName.c_str()), "fromName");
            m_requestArr->addObject(friendInfo);
//         }
      }
      CCLog("fb appRequestFriends m_data =%d",m_requestArr->count());
      Json_dispose(fjson);
   }
}

InviteForGiftView* InviteForGiftView::create(){
    InviteForGiftView* ret = new InviteForGiftView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool InviteForGiftView::init(){
    if (!PopupBaseView::init()) {
        return false;
    }
   setIsHDPanel(true);
   CCLoadSprite::doResourceByCommonIndex(207, true);
   CCLoadSprite::doResourceByCommonIndex(500, true);
   CCLoadSprite::doResourceByCommonIndex(502, true);
   CCLoadSprite::doResourceByCommonIndex(504, true);
   setCleanFunction([](){
      CCLoadSprite::doResourceByCommonIndex(207, false);
      CCLoadSprite::doResourceByCommonIndex(500, false);
      CCLoadSprite::doResourceByCommonIndex(504, true);
   });
    m_ccbNode = CCBLoadFile("InviteForGiftView", this, this);
    this->setContentSize(m_ccbNode->getContentSize());
   
   string branchOnStr = GlobalData::shared()->branchOnStr;
   m_android=false;
   m_ios=false;
   if (branchOnStr!="") {
      vector<string> vec;
      CCCommonUtils::splitString(branchOnStr, "|", vec);
      if(vec.size()==2){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
         if (vec[0]=="1") {
            m_android=true;
         }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
         if (vec[1]=="1") {
            m_ios=true;
         }
#endif
      }
   }
   //ios不显示输入邀请码
   if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS){
      m_isIOS=true;
      m_infoGiftNode1->setVisible(false);
      m_infoGiftNode2->setVisible(false);
      float dh=m_infoGiftNode1->getContentSize().height;
      if (CCCommonUtils::isIosAndroidPad())
      {
         m_scrollNode1->setPositionY(-204.4);
      } else {
         m_scrollNode1->setPositionY(m_scrollNode1->getPositionY()+dh+12);
         m_scrollNode1->setContentSize(CCSize(m_scrollNode1->getContentSize().width, m_scrollNode1->getContentSize().height+dh));
         m_tableViewNode->setContentSize(CCSize(m_tableViewNode->getContentSize().width, m_tableViewNode->getContentSize().height+dh));
         m_tableViewNode->setPositionY(m_tableViewNode-dh);
         m_tvTipTxt->setPositionY(m_tvTipTxt->getPositionY()+dh);
      }
   }else{
      m_isIOS=false;
   }
    m_inviterUid = "";
    m_inviterName = "";
    m_inviterServer = "";
    m_isInvited = false;
    m_inviCode = GlobalData::shared()->playerInfo.inviCode;//邀请码
    m_requestArr = CCArray::create();
   m_installData = CCArray::create();
   m_inviteData = CCArray::create();
   
    int preH = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newH = m_buildBG->getContentSize().height;
    int addH = newH - preH;
   
   if (CCCommonUtils::isIosAndroidPad())
   {
      addH = CCDirector::sharedDirector()->getWinSize().height - 2048;
   }
    
    auto preSize = m_scrollNode2->getContentSize();
    m_scrollNode2->setContentSize(CCSize(preSize.width, preSize.height + addH));
    auto scrNodePos = m_scrollNode2->getPosition();
    m_scrollNode2->setPosition(scrNodePos.x, scrNodePos.y - addH);
   scrNodePos = m_titleNode3->getPosition();
   m_titleNode3->setPosition(scrNodePos.x, scrNodePos.y + addH);
    
    preSize = m_scrollNode1->getContentSize();
    m_scrollNode1->setContentSize(CCSize(preSize.width, preSize.height + addH));
   preSize = m_tableViewNode->getContentSize();
   m_tableViewNode->setContentSize(CCSize(preSize.width, preSize.height + addH));
    scrNodePos = m_tableViewNode->getPosition();
    m_tableViewNode->setPosition(scrNodePos.x, scrNodePos.y - addH);
   
    CCCommonUtils::setButtonTitle(m_giftBtn, _lang("105365").c_str());//接受邀请
    CCCommonUtils::setButtonTitle(m_inviteBtn, _lang("105359").c_str());//邀请朋友
    CCCommonUtils::setButtonTitle(m_enterBtn, _lang("105039").c_str());
    CCCommonUtils::setButtonTitle(m_mailBtn, _lang("105048").c_str());
    CCCommonUtils::setButtonTitle(m_rwdInfoBtn, _lang("105055").c_str());
   m_jumpBtnText->setString(_lang("105359").c_str());//邀请朋友
   if (m_android||m_ios) {
      m_copyBtnText->setString(_lang("105375").c_str());//复制我的邀请码
   }else{
      m_copyBtnText->setString(_lang("105360").c_str());//复制我的邀请码
   }
   
   m_branchBtnText->setString(_lang("105037").c_str());//邀请好友branch
   m_rewardBtnText->setString(_lang("105036").c_str());//领取奖励
   
//    m_codeLabel->setString(m_inviCode);
    m_infoLabel1->setString(_lang("105038"));
    m_infoLabel2->setString(_lang("105044"));
    m_infoLabel3->setString(_lang("105045"));
    m_infoLabel4->setString(_lang("105046"));
    m_infoLabel5->setString(_lang("105047"));
//    m_infoLabel8->setString(_lang("105040"));
//    m_infoLabel9->setString(_lang("105041"));
//    m_infoLabel10->setString(_lang("105042"));
//    m_infoLabel11->setString(_lang("105043"));
//    m_fbLabel->setString(_lang("107095"));
   
   m_infoLabel6->setString(_lang("105359").c_str());//邀请朋友
   m_infoLabel7->setString(_lang("105361").c_str());//获取奖励
   m_upBarText->setString(_lang("105362").c_str());
   m_downBarTxt1->setString("1");
   m_downBarTxt3->setString("3");
   m_downBarTxt5->setString("5");
   m_downBarTxt10->setString("10");
   m_downBarTxt20->setString("20");
   m_titleTxt1->setString(_lang("105366").c_str());//通过邀请码
   if (!GlobalData::shared()->isChinaPlatForm()) {
      m_titleTxt2->setString(_lang("105367").c_str());//接受facebook
   }
   m_titleTxt3->setString(_lang("105363").c_str());//与朋友一同成长

    m_fbText->setString(_lang("107089"));
    CCPoint p;
    int h;
    m_infoLabel12->setString(_lang("105364"));//%5
    p = m_infoLabel12->getPosition();
    h = m_infoLabel12->getContentSize().height * m_infoLabel12->getOriginScaleY();
    auto preIntroNodeSize = m_introInviteNode->getContentSize();
    m_introInviteNode->setContentSize(CCSize(preIntroNodeSize.width, p.y+ h +10));
   if (CCCommonUtils::isIosAndroidPad())
   {
      m_introInviteNode->setContentSize(CCSize(preIntroNodeSize.width, h));
      m_infoLabel12->setPositionY(h / 2.0);
   }
   
   
    auto sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    m_editBox = CCEditBox::create(CCSizeMake(420, 60), sprite9);
    m_editBox->setMaxLength(16);
    m_editBox->setFontSize(24);
    m_editBox->setFontColor(ccBLACK);
    m_editBox->setAnchorPoint(ccp(0.5, 0.5));
    m_editBox->setPosition(ccp(m_infoGiftNode1->getContentSize().width * 0.5, 30 + m_infoGiftNode1->getContentSize().height * 0.5));
//    m_editBox->setInputMode(kEditBoxInputModeSingleLine);
    m_editBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_infoGiftNode1->addChild(m_editBox);
    
    m_scrollView2 = CCScrollView::create(CCSize(m_scrollNode2->getContentSize().width, m_scrollNode2->getContentSize().height-55));
    m_scrollView2->setContentSize(CCSize(m_introInviteNode->getContentSize().width, m_introInviteNode->getContentSize().height));
    m_scrollView2->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView2->setAnchorPoint(ccp(0, 0));
    m_scrollNode2->addChild(m_scrollView2);
    m_scrollView2->setPosition(0, 0);
   
    m_introInviteNode->getParent()->removeChild(m_introInviteNode);
    m_scrollView2->addChild(m_introInviteNode);
    m_introInviteNode->setPosition(0, 0);
   m_rwdInfoBtn->setVisible(false);
    m_scrollView2->setContentOffset(ccp(0, m_scrollNode2->getContentSize().height - m_scrollView2->getContentSize().height-55));
   //tableView
   m_data = CCArray::create();
   m_tabView = CCMultiColTableView::create(this, m_tableViewNode->getContentSize());
   m_tabView->setDirection(kCCScrollViewDirectionVertical);
   m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
   m_tabView->setMultiColTableViewDelegate(this);
   m_tabView->setTouchPriority(Touch_Popup);
   m_tableViewNode->addChild(m_tabView);
   //tableView
   
    m_fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
//    m_spriteNode->setVisible(true);

    m_fbClick = false;
//    m_first = true;

    return true;
}
void InviteForGiftView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
   
}

cocos2d::CCSize InviteForGiftView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
   if (CCCommonUtils::isIosAndroidPad())
   {
      return CCSize(1500, 280);
   }
   return CCSize(600, 170);
}

CCTableViewCell* InviteForGiftView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
   if(idx >= m_data->count()){
      return NULL;
   }
   RequestFriendCell* cell = (RequestFriendCell*)table->dequeueGrid();
   CCDictionary* friendInfo = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
   if(idx < m_data->count()){
      if(cell){
         cell->setData(friendInfo);
      }else{
         cell = RequestFriendCell::create(friendInfo,m_scrollNode1);
      }
   }
   return cell;
}

ssize_t InviteForGiftView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
   int num = m_data->count();
   return num;
}

unsigned int InviteForGiftView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
   return 1;
}

void InviteForGiftView::onEnter(){
    CCNode::onEnter();
   m_progressBar->setScaleX(0/20.0);
    setTitleName(_lang("105075").c_str());
    m_page = 1;
    updateInviterInfo();
    m_inviteBtn->setEnabled(true);
    m_giftBtn->setEnabled(false);
    m_infoInviteNode->setVisible(false);
//    m_introGiftNode->setVisible(true);
    m_scrollNode1->setVisible(true);
    m_introInviteNode->setVisible(false);
   m_rewardNode->setVisible(false);
    m_scrollNode2->setVisible(false);
    m_fbNode->setVisible(false);
   m_upBgSpr1->setVisible(false);
   m_tableViewNode->setVisible(true);
    auto cmd = new InviteInfoCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(InviteForGiftView::onInviteInfoCallBack), NULL));
    cmd->sendAndRelease();
   onClickInviteBtn(NULL,CCControlEvent::TOUCH_DOWN);
}

void InviteForGiftView::onExit(){
    GameController::getInstance()->removeWaitInterface();
    CCNode::onExit();
}

SEL_CCControlHandler InviteForGiftView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGiftBtn", InviteForGiftView::onClickGiftBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickInviteBtn", InviteForGiftView::onClickInviteBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCopyClick", InviteForGiftView::onCopyClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEnterClick", InviteForGiftView::onEnterClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMailBtnClick", InviteForGiftView::onMailBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJumpBtnClick", InviteForGiftView::onJumpBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRwdInfoClick", InviteForGiftView::onRwdInfoClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFacebookClick", InviteForGiftView::onFacebookClick);
   CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardBtnClick", InviteForGiftView::onRewardBtnClick);
   CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBranchBtnClick", InviteForGiftView::onBranchBtnClick);
    return NULL;
}

bool InviteForGiftView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spriteNode", CCNode*, this->m_spriteNode);
   
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollNode1", CCNode*, this->m_scrollNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollNode2", CCNode*, this->m_scrollNode2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoGiftNode1", CCNode*, this->m_infoGiftNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoGiftNode2", CCNode*, this->m_infoGiftNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoInviteNode", CCNode*, this->m_infoInviteNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_introGiftNode", CCNode*, this->m_introGiftNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_introInviteNode", CCNode*, this->m_introInviteNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_giftBtn", CCControlButton*, this->m_giftBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inviteBtn", CCControlButton*, this->m_inviteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_copyBtn", CCControlButton*, this->m_copyBtn);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_copyBtnText", CCLabelIF*, this->m_copyBtnText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_enterBtn", CCControlButton*, this->m_enterBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtn", CCControlButton*, this->m_mailBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jumpBtn", CCControlButton*, this->m_jumpBtn);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jumpBtnText", CCLabelIF*, this->m_jumpBtnText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdInfoBtn", CCControlButton*, this->m_rwdInfoBtn);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_codeLabel", CCLabelIF*, this->m_codeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel1", CCLabelIF*, this->m_infoLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel2", CCLabelIF*, this->m_infoLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel3", CCLabelIF*, this->m_infoLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel4", CCLabelIF*, this->m_infoLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel5", CCLabelIF*, this->m_infoLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel6", CCLabelIF*, this->m_infoLabel6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel7", CCLabelIF*, this->m_infoLabel7);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel8", CCLabelIF*, this->m_infoLabel8);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel9", CCLabelIF*, this->m_infoLabel9);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel10", CCLabelIF*, this->m_infoLabel10);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel11", CCLabelIF*, this->m_infoLabel11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel12", CCLabelIF*, this->m_infoLabel12);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel13", CCLabelIF*, this->m_infoLabel13);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel14", CCLabelIF*, this->m_infoLabel14);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel15", CCLabelIF*, this->m_infoLabel15);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel16", CCLabelIF*, this->m_infoLabel16);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbLabel", CCLabelIF*, this->m_fbLabel);
   
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbNode", CCNode*, this->m_fbNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_facebookBtn", CCControlButton*, this->m_facebookBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbText", CCLabelIF*, this->m_fbText);
   //rewardNode
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode", CCNode*, this->m_rewardNode);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode1", CCNode*, this->m_rwdNode[0]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode2", CCNode*, this->m_rwdNode[1]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode3", CCNode*, this->m_rwdNode[2]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode4", CCNode*, this->m_rwdNode[3]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode1", CCNode*, this->m_rwdPicNode[0]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode2", CCNode*, this->m_rwdPicNode[1]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode3", CCNode*, this->m_rwdPicNode[2]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode4", CCNode*, this->m_rwdPicNode[3]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNum1", CCLabelIF*, this->m_rwdNum[0]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNum2", CCLabelIF*, this->m_rwdNum[1]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNum3", CCLabelIF*, this->m_rwdNum[2]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNum4", CCLabelIF*, this->m_rwdNum[3]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdCover1", CCLayerColor*, this->m_rwdCover[0]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdCover2", CCLayerColor*, this->m_rwdCover[1]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdCover3", CCLayerColor*, this->m_rwdCover[2]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdCover4", CCLayerColor*, this->m_rwdCover[3]);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBtn", CCControlButton*, this->m_rewardBtn);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBtnText", CCLabelIF*, this->m_rewardBtnText);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBarText", CCLabelIF*, this->m_upBarText);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downBarTxt1", CCLabelIF*, this->m_downBarTxt1);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downBarTxt3", CCLabelIF*, this->m_downBarTxt3);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downBarTxt5", CCLabelIF*, this->m_downBarTxt5);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downBarTxt10", CCLabelIF*, this->m_downBarTxt10);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downBarTxt20", CCLabelIF*, this->m_downBarTxt20);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressBar", CCScale9Sprite*, this->m_progressBar);
   
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt1", CCLabelIF*, this->m_titleTxt1);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt2", CCLabelIF*, this->m_titleTxt2);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt3", CCLabelIF*, this->m_titleTxt3);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBgSpr1", CCNode*, this->m_upBgSpr1);
   
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tvTipTxt", CCLabelIF*, this->m_tvTipTxt);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tableViewNode", CCNode*, this->m_tableViewNode);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode3", CCNode*, this->m_titleNode3);
   
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jumpBtnNode", CCNode*, this->m_jumpBtnNode);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_copyBtnNode", CCNode*, this->m_copyBtnNode);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_branchBtnNode", CCNode*, this->m_branchBtnNode);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_branchBtn", CCControlButton*, this->m_branchBtn);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_branchBtnText", CCLabelIF*, this->m_branchBtnText);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbIcon", CCNode*, this->m_fbIcon);
   
    return false;
}

void InviteForGiftView::onRewardBtnClick(CCObject *pSender, CCControlEvent event){

//   GCMRewardController::getInstance()->flyReward(m_rewardArr1);
//   GCMRewardController::getInstance()->flyToolReward(m_rewardArr2);
   auto cmd = new InviteRewardCommand(m_countNumber);
   cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(InviteForGiftView::onInviteRewardCallBack), NULL));
   cmd->sendAndRelease();
}

void InviteForGiftView::onClickGiftBtn(CCObject * pSender, CCControlEvent pCCControlEvent){
//   m_spriteNode->setVisible(!m_isIOS);
    m_inviteBtn->setEnabled(true);
    m_giftBtn->setEnabled(false);
    m_fbNode->setVisible(false);
   m_upBgSpr1->setVisible(false);
   m_tableViewNode->setVisible(true);
    if (m_isInvited) {
        m_infoGiftNode1->setVisible(false);
       if (!m_isIOS)
          m_infoGiftNode2->setVisible(true);
    } else {
       if (!m_isIOS)
          m_infoGiftNode1->setVisible(true);
        m_infoGiftNode2->setVisible(false);
        string preuid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
        if(!FBUtilies::fbIsLogin() || preuid==""){
           m_fbNode->setVisible(true);
           m_upBgSpr1->setVisible(true);
           m_tableViewNode->setVisible(false);
        }
       if (GlobalData::shared()->isChinaPlatForm()) {
          //m_infoLabel1->setString("");
          m_fbNode->setVisible(false);
          m_upBgSpr1->setVisible(false);
          m_titleTxt1->setString(_lang("105366").c_str());//通过邀请码
       }
    }
    m_infoInviteNode->setVisible(false);
//    m_introGiftNode->setVisible(true);
    m_scrollNode1->setVisible(true);
   m_tableViewNode->setPositionX(0);
    m_introInviteNode->setVisible(false);
   m_rewardNode->setVisible(false);
    m_scrollNode2->setVisible(false);
    m_page = 1;
}

void InviteForGiftView::onClickInviteBtn(CCObject * pSender, Control::EventType pCCControlEvent){
//   m_spriteNode->setVisible(true);
    m_inviteBtn->setEnabled(false);
    m_giftBtn->setEnabled(true);
    m_infoInviteNode->setVisible(true);
   if (GlobalData::shared()->isBranchOn) {//branch //
      m_jumpBtnNode->setVisible(false);
      m_copyBtnNode->setVisible(false);
      m_branchBtnNode->setVisible(true);
   }else{
      m_jumpBtnNode->setVisible(true);
      m_copyBtnNode->setVisible(true);
      m_branchBtnNode->setVisible(false);
   }
    m_infoGiftNode1->setVisible(false);
    m_editBox->setText("");
    m_infoGiftNode2->setVisible(false);
    m_introInviteNode->setVisible(true);
   m_rewardNode->setVisible(true);
    m_scrollNode2->setVisible(true);
//    m_introGiftNode->setVisible(false);
    m_scrollNode1->setVisible(false);
   m_tableViewNode->setPositionX(1000);
   if (CCCommonUtils::isIosAndroidPad())
   {
      m_tableViewNode->setPositionX(2000);
   }
   if(GlobalData::shared()->isChinaPlatForm()){
      m_jumpBtnNode->setVisible(false);
      m_copyBtnNode->setPositionX(320);
      m_branchBtnNode->setVisible(false);
   }
    m_page = 2;
}

void InviteForGiftView::onCopyClick(CCObject * pSender, Control::EventType pCCControlEvent){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   if (m_android) {
      //复制激活码改成激活链接
      string str = m_inviCode;
      str+="|";
      str+=cocos2d::extension::CCDevice::getDeviceUid();
      BranchController::getInstance()->sendToBranch("InviteForGift", str);//m_inviCode
   }else{
      cocos2d::extension::CCDevice::clipboardSetText(m_inviCode);
      PopupViewController::getInstance()->addPopupView(InvitationMethodView::create(m_inviCode));
   }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   if (m_ios) {
      //复制激活码改成激活链接
      string str = m_inviCode;
      str+="|";
      str+=cocos2d::extension::CCDevice::getDeviceUid();
      BranchController::getInstance()->sendToBranch("InviteForGift", str);//m_inviCode
   }else{
      cocos2d::extension::CCDevice::clipboardSetText(m_inviCode);
      PopupViewController::getInstance()->addPopupView(InvitationMethodView::create(m_inviCode));
   }
#endif
//   //复制激活码改成激活链接
//   string str = m_inviCode;
//   str+="|";
//   str+=cocos2d::extension::CCDevice::getDeviceUid();
//   BranchController::getInstance()->sendToBranch("InviteForGift", str);//m_inviCode
//   
//    cocos2d::extension::CCDevice::clipboardSetText(m_inviCode);
////    CCCommonUtils::flyHint("", "", _lang("105078"));
//   PopupViewController::getInstance()->addPopupView(InvitationMethodView::create(m_inviCode));
}

void InviteForGiftView::onEnterClick(CCObject * pSender, Control::EventType pCCControlEvent){
   int lv = FunBuildController::getInstance()->getMainCityLv();
   if (lv >= 6) {
      PopupViewController::getInstance()->addPopupView(InviteWarningView::create(CCCallFunc::create(this, callfunc_selector(InviteForGiftView::enterCode)), CCCallFunc::create(this, callfunc_selector(InviteForGiftView::clearCode))));
   } else {
      enterCode();
   }
}

void InviteForGiftView::enterCode(){
    std::string getcode = m_editBox->getText();
    if (getcode != "") {
        InviteAcceptCommand* cmd = new InviteAcceptCommand(getcode);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(InviteForGiftView::onCodeCallBack), NULL));
        cmd->sendAndRelease();
        GameController::getInstance()->showWaitInterface();
    } else {
        m_editBox->setText("");
        CCCommonUtils::flyHint("", "", _lang("105077"));
    }
}

void InviteForGiftView::clearCode() {
   m_editBox->setText("");
}

void InviteForGiftView::onMailBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if (m_isInvited) {
        MailController::getInstance()->openMailDialogViewFirst(m_inviterName, m_inviterUid);
    }
}

void InviteForGiftView::onBranchBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
   CCLOGFUNC();
//   BranchController::getInstance()->excute("InviteForGift");
   string str = m_inviCode;
   str+="|";
   str+=cocos2d::extension::CCDevice::getDeviceUid();
   BranchController::getInstance()->sendToBranch("InviteForGift", str);//m_inviCode
   
//   //得到credits
//   int credits = BranchController::getInstance()->getCredit();
//   CCLog("credits;%d",credits);
   
//   InviteAcceptCommand* cmd = new InviteAcceptCommand("4e3bbeb2fa3c1");
//   cmd->sendAndRelease();
   
}

//***************************** fb ****************************
void InviteForGiftView::onJumpBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
//    PopupViewController::getInstance()->addPopupView(InvitationMethodView::create());
   //    PopupViewController::getInstance()->removePopupView(this);
   //    string url = "https://www.facebook.com/Clash.Of.Kings.Game";
   //    GameController::getInstance()->goTurntoUrl(url);
   GlobalData::shared()->isBind = true;
   this->unschedule(schedule_selector(InviteForGiftView::checkFb));
   this->schedule(schedule_selector(InviteForGiftView::checkFb),0.2);
   m_time = 0;
   m_waitInterface1 = GameController::getInstance()->showWaitInterface(m_jumpBtn);
   m_fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
   if(FBUtilies::fbIsLogin() && m_fbUid!=""){
      //FBUtilies::getFBFriendList();
      FBUtilies::inviteFriends();
   }else{
      GlobalData::shared()->isBind = true;
      if(FBUtilies::fbIsLogin()){
         FBUtilies::fbLogout();
      }
      FBUtilies::fbLogin();
   }
}
void InviteForGiftView::checkFb(float _time){
   if(m_waitInterface1){
      m_waitInterface1->remove();
      m_waitInterface1 = NULL;
   }
   if(m_time==1){
      
      this->unschedule(schedule_selector(InviteForGiftView::checkFb));
      GlobalData::shared()->isBind = true;
      string link = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
      link = "https://fb.me/827859737255348?feed_key=android_appInvite";
#else
      link = "https://fb.me/789290781112244?feed_key=ios_appInvite";
#endif
      //FBUtilies::appInvite(link);
      CCArray* arr = CCArray::create();
      arr->addObjectsFromArray(m_installData);
      arr->addObjectsFromArray(m_inviteData);
//      if(arr->count()>0){
//         PopupViewController::getInstance()->removePopupView(this);
         PopupViewController::getInstance()->addPopupInView(FacebookInviteView::create(arr,"0"));
//      }else{
//         FBUtilies::postFBSelectedFriendList(arr,_lang("107077"));
//         PopupViewController::getInstance()->removePopupView(this);
//      }
   }
}

void InviteForGiftView::loginSuccess(CCObject* param){
   if(!m_giftBtn->isEnabled()){
      auto view = dynamic_cast<InviteForGiftView*>(PopupViewController::getInstance()->getCurrentPopupView());
      if(view==NULL) return ;
//      m_first = false;
   }
   string m_facebookUid =CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_uid", "");
   if (m_facebookUid == "") {
      FBUtilies::fbLogout();//不是同一个号
      return;
   }
   string preuid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
   string preuName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
   string m_facebookName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
   //bindSuccess(NULL);
   if(m_fbUid==""){
      UserBindCommand* cmd = new UserBindCommand("",m_facebookUid,"","","",1,preuid,"",m_facebookName,preuName);
      cmd->sendAndRelease();
      GameController::getInstance()->showWaitInterface();
   }else{
      if(m_fbUid!=m_facebookUid){
         CCCommonUtils::flyHint("", "", _lang("107078"));
         FBUtilies::fbLogout();//不是同一个号
      }else{
         if(!m_giftBtn->isEnabled()){
            FBUtilies::getAppRequestList();
         }else{
            FBUtilies::inviteFriends();
         }
      }
   }
}

void InviteForGiftView::bindSuccess(CCObject* param){
   if(!m_giftBtn->isEnabled()){
      auto view = dynamic_cast<InviteForGiftView*>(PopupViewController::getInstance()->getCurrentPopupView());
      if(view==NULL) return ;
      FBUtilies::getAppRequestList();
   }else{
      GameController::getInstance()->removeWaitInterface();
      FBUtilies::inviteFriends();
   }
}

void InviteForGiftView::afterGetFriendsInfo(CCObject* param){
   GameController::getInstance()->removeWaitInterface();
   CCArray* friends = dynamic_cast<CCArray*>(param);
   m_time += 1;
   m_installData->removeAllObjects();
   if (friends) {
      m_installData->addObjectsFromArray(friends);
   }else{
      if(GlobalData::shared()->installFriendsInfo!=""){
         Json* fjson = Json_create(GlobalData::shared()->installFriendsInfo.c_str());
         string ids = "";
         if(fjson){
            int size = Json_getSize(fjson);
            for (int i=0; i<size; i++) {
               Json *item = Json_getItemAt(fjson, i);
               string name = Json_getString(item,"name","");
               string id = Json_getString(item,"id","");
               string url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",id.c_str())->getCString();
               
               CCDictionary* friendInfo = CCDictionary::create();
               friendInfo->setObject(CCString::create(id.c_str()), "id");
               friendInfo->setObject(CCString::create(name.c_str()), "name");
               friendInfo->setObject(CCString::create(url.c_str()), "picture");
               friendInfo->setObject(CCString::create("1"), "flag");
               m_installData->addObject(friendInfo);
            }
            Json_dispose(fjson);
         }
      }
   }
   CCLOG("fb m_installData count=%d",m_installData->count());
}

void InviteForGiftView::getInviteFriends(CCObject *param){
   CCLOG("fb getInviteFriends");
   CCArray* friendArr = dynamic_cast<CCArray*>(param);
   m_inviteData->removeAllObjects();
   m_time += 1;
   if(friendArr!=NULL){
      m_inviteData->addObjectsFromArray(friendArr);
   }else{
      string info = CCUserDefault::sharedUserDefault()->getStringForKey("inviteFriends", "");
      GlobalData::shared()->inviteFriendsInfo = info;
      if(GlobalData::shared()->inviteFriendsInfo!=""){
         string friends = GlobalData::shared()->inviteFriendsInfo;
         CCLOG(" inviteFriendsInfo =%s",friends.c_str());
         Json* fjson = Json_create(friends.c_str());
         int size = Json_getSize(fjson);
         for (int i=0; i<size; i++) {
            Json *item = Json_getItemAt(fjson, i);
            string name = Json_getString(item,"name","");
            string id = Json_getString(item,"id","");
            string url = "";
            Json *picture =Json_getItem(item,"picture");
            if(picture!=NULL){
               Json *pictureData =Json_getItem(picture,"data");
               if(pictureData!=NULL){
                  url = Json_getString(pictureData,"url","");
               }
            }
            CCDictionary* friendInfo = CCDictionary::create();
            friendInfo->setObject(CCString::create(id.c_str()), "id");
            friendInfo->setObject(CCString::create(name.c_str()), "name");
            friendInfo->setObject(CCString::create(url.c_str()), "picture");
            friendInfo->setObject(CCString::create("0"), "flag");
            m_inviteData->addObject(friendInfo);
         }
         Json_dispose(fjson);
      }
   }
}
//***************************** fb ****************************

void InviteForGiftView::onRwdInfoClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(InviteRewardInfoView::create());
}

void InviteForGiftView::getBranchUrl(CCObject *param){
   string url = dynamic_cast<CCString*>(param)->getCString();
   CCLog("InviteForGiftView::getBranchUrl url=%s",url.c_str());
   if(GlobalData::shared()->isBranchOn){
      //分享链接
      string link = url;
      string name = "Clash Of Kings";
      string caption = _lang("105371");
      string linkDescription = _lang("105372");
      string pictureUrl = "https://fbcok.eleximg.com/cok/img/feed/branch_share.jpg";
      string ref = "";
      //手动分享
      FBUtilies::fbPublishFeedDialog(name,caption,linkDescription,link,pictureUrl,0,ref);
   }else{
      cocos2d::extension::CCDevice::clipboardSetText(url);
      PopupViewController::getInstance()->addPopupView(InvitationMethodView::create(url));
   }
}

void InviteForGiftView::onFacebookClick(CCObject *pSender, CCControlEvent event){
   CCLOG("onFacebookClick");
   if(m_fbNode->isVisible()){
      m_fbClick = true;
      if(FBUtilies::fbIsLogin() && m_fbUid!=""){
         FBUtilies::getAppRequestList();
      }else{
         GlobalData::shared()->isBind = true;
         if(FBUtilies::fbIsLogin()){
            FBUtilies::fbLogout();
         }
         FBUtilies::fbLogin();
      }
   }
}

void InviteForGiftView::updateInviterInfo(){
    if (m_isInvited) {
        m_infoLabel4->setString(_lang("105046") + m_inviterName);
        m_infoLabel5->setString(_lang("105047") + m_inviterServer);
        if(m_page == 1) {
            m_infoGiftNode1->setVisible(false);
            if (!m_isIOS)
               m_infoGiftNode2->setVisible(true);
        }
    } else {
        if (m_page == 1) {
            if (!m_isIOS)
               m_infoGiftNode1->setVisible(true);
            m_infoGiftNode2->setVisible(false);
        }
    }
}

void InviteForGiftView::onCodeCallBack(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dict = _dict(result->getData());
    if(dict) {
        if(dict->objectForKey("errorCode")) {
            CCCommonUtils::flyHint("", "", _lang("105077"));
            m_editBox->setText("");
            GameController::getInstance()->removeWaitInterface();
            return;
        }
        m_inviterUid = dict->valueForKey("uid")->getCString();
        m_inviterName = dict->valueForKey("name")->getCString();
        m_inviterServer = dict->valueForKey("server")->getCString();
        m_isInvited = true;
        updateInviterInfo();
    }
    GameController::getInstance()->removeWaitInterface();
}

void InviteForGiftView::refreshData(CCObject* param){
   auto dict = _dict(param);
   if(dict) {
      if(dict->objectForKey("errorCode")) {
         CCCommonUtils::flyHint("", "", _lang("105077"));
         m_editBox->setText("");
         GameController::getInstance()->removeWaitInterface();
         return;
      }
      m_inviterUid = dict->valueForKey("uid")->getCString();
      m_inviterName = dict->valueForKey("name")->getCString();
      m_inviterServer = dict->valueForKey("server")->getCString();
      m_isInvited = true;
      updateInviterInfo();
   }
}

void InviteForGiftView::onInviteInfoCallBack(CCObject* param) {
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dict = _dict(result->getData());
    if (dict->objectForKey("uid") && dict->objectForKey("name") && dict->objectForKey("server")) {
        m_inviterUid = dict->valueForKey("uid")->getCString();
        m_inviterName = dict->valueForKey("name")->getCString();
        m_inviterServer = dict->valueForKey("server")->getCString();
        m_isInvited = true;
        updateInviterInfo();
    }
   m_receiveCount = dict->valueForKey("receiveCount")->intValue();
   m_rewardSaveDict=CCDictionary::createWithDictionary(dict);
    m_inviteCount = dict->valueForKey("inviteCount")->intValue();//邀请人数
   m_inviteCount=m_inviteCount>20?20:m_inviteCount;
   refreshReward(m_receiveCount);//设置奖品数组
   
   string preuid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
   if(!m_isInvited && FBUtilies::fbIsLogin() && preuid!="" ){//&& m_first
      FBUtilies::getAppRequestList();
      GameController::getInstance()->showWaitInterface();
      this->scheduleOnce(schedule_selector(InviteForGiftView::removeWaitInterface), 5);
   }
   
   m_fbNode->setVisible(false);
   m_upBgSpr1->setVisible(false);
   m_tableViewNode->setVisible(true);
   if(!FBUtilies::fbIsLogin() || preuid==""){
      m_fbNode->setVisible(true);
      m_upBgSpr1->setVisible(true);
      m_tableViewNode->setVisible(false);
   }
//   m_first = false;
}
void InviteForGiftView::removeWaitInterface(float t){
   GameController::getInstance()->removeWaitInterface();
}

void InviteForGiftView::onInviteRewardCallBack(CCObject* param) {
   NetResult* result = dynamic_cast<NetResult*>(param);
   auto dict = _dict(result->getData());
   CCArray* arr=dynamic_cast<CCArray*>(dict->objectForKey("reward"));
   if(arr&&arr->count()>0){
      if (m_receiveCount<1) {
         m_receiveCount=1;
      }else if (m_receiveCount>=1&&m_receiveCount<3){
         m_receiveCount=3;
      }else if (m_receiveCount>=3&&m_receiveCount<5){
         m_receiveCount=5;
      }else if (m_receiveCount>=5&&m_receiveCount<10){
         m_receiveCount=10;
      }else{
         m_receiveCount=20;
      }
      refreshReward(m_receiveCount);
      GCMRewardController::getInstance()->retReward2(arr,true);
   }
}

void InviteForGiftView::refreshReward(int receiveCount){

   int countNumber=0;
   if (receiveCount<1) {
      countNumber=1;
   }else if (receiveCount>=1&&receiveCount<3){
      countNumber=3;
   }else if (receiveCount>=3&&receiveCount<5){
      countNumber=5;
   }else if (receiveCount>=5&&receiveCount<10){
      countNumber=10;
   }else{
      countNumber=20;
   }
   m_countNumber=countNumber;
   m_progressBar->setScaleX(m_inviteCount/20.0);
//   if (m_inviteCount>=countNumber) {
//      m_progressBar->setScaleX(countNumber/20.0);
//   }
   CCArray* tempArr1=dynamic_cast<CCArray*>(m_rewardSaveDict->objectForKey("reward"));
   CCArray* tempArr2;
   if (tempArr1) {
      for (int i=0; i<tempArr1->count(); i++) {
         CCDictionary* tempDict1 = dynamic_cast<CCDictionary*>(tempArr1->objectAtIndex(i));
         int count = tempDict1->valueForKey("count")->intValue();
         if (count==countNumber) {
            tempArr2 = dynamic_cast<CCArray*>(tempDict1->objectForKey("reward"));
            if (tempArr2->count()==2) {//如果只有两个奖励，则剩下的两个显示锁图片
               auto lock_pic2 = CCLoadSprite::createSprite("iron_lock.png");
               m_rwdPicNode[2]->addChild(lock_pic2);
               auto lock_pic3 = CCLoadSprite::createSprite("iron_lock.png");
               m_rwdPicNode[3]->addChild(lock_pic3);
            }
            CCObject *obj;
            int j=0;
            CCARRAY_FOREACH(tempArr2, obj){
               auto dict = dynamic_cast<CCDictionary*>(obj);
               int type = dict->valueForKey("type")->intValue();
               switch (type) {
                  case R_GOLD:{
                     int addValue = dict->valueForKey("value")->intValue();
                     //设置奖品图片
                     float r_size=85;
                     auto pic = CCLoadSprite::createSprite("ui_gold.png");
                     CCCommonUtils::setSpriteMaxSize(pic,r_size,true);
                     m_rwdPicNode[j]->removeAllChildren();
                     m_rwdPicNode[j]->addChild(pic);
                     m_rwdNum[j]->setString(CC_ITOA(addValue));
                     break;
                  }
                  case R_GOODS:{
                     CCDictionary *goodsDict = _dict(dict->objectForKey("value"));
                     int num = goodsDict->valueForKey("num")->intValue();
                     int itemId = goodsDict->valueForKey("id")->intValue();
                     //设置奖品图片
                     m_rwdPicNode[j]->removeAllChildren();
                     float r_size=85;
                     CCCommonUtils::createGoodsIcon(itemId, m_rwdPicNode[j], CCSize(r_size, r_size));
                     m_rwdNum[j]->setString(CC_ITOA(num));
                     break;
                  }
                  default:
                     break;
               }
               j++;
            }
            
         }
      }
   }
   if (m_inviteCount>=countNumber&&m_receiveCount<20) {
      //能领奖
      m_rwdCover[0]->setVisible(false);
      m_rwdCover[1]->setVisible(false);
      m_rwdCover[2]->setVisible(false);
      m_rwdCover[3]->setVisible(false);
      m_rewardBtn->setEnabled(true);
      CCCommonUtils::setButtonSprite(m_rewardBtn, "activity1_12.png");
   }else{
      //不能领奖
      m_rwdCover[0]->setVisible(true);
      m_rwdCover[1]->setVisible(true);
      m_rwdCover[2]->setVisible(true);
      m_rwdCover[3]->setVisible(true);
      m_rewardBtn->setEnabled(false);
      m_rewardBtn->setColor({83,83,83});
   }
}

InvitationMethodView::InvitationMethodView(){
//   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InvitationMethodView::afterGetFriendsInfo), MSG_FBFriendsList, NULL);
//   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InvitationMethodView::getInviteFriends), MSG_FBIviteFriends, NULL);
//   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InvitationMethodView::loginSuccess), MSG_FBLoginSucess, NULL);
//   CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(InvitationMethodView::bindSuccess), MSG_USER_BIND_OK, NULL);
};

InvitationMethodView::~InvitationMethodView(){
//   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFriendsList);
//   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBIviteFriends);
//   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBLoginSucess);
//   CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USER_BIND_OK);
};

InvitationMethodView* InvitationMethodView::create(string inviteStr) {
    auto ret = new InvitationMethodView();
    if (ret && ret->init(inviteStr)){
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool InvitationMethodView::init(string inviteStr) {
    bool ret = false;
    if (PopupBaseView::init()) {
       m_inviteStr=inviteStr;
       setIsHDPanel(true);
        CCBLoadFile("InvitationMethodView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        m_installData = CCArray::create();
        m_inviteData = CCArray::create();
       
//       string inviCode = GlobalData::shared()->playerInfo.inviCode;//邀请码
//       m_infoLabel1->setString(_lang_1("105368", m_inviteStr.c_str()));
       string branchOnStr = GlobalData::shared()->branchOnStr;
       bool isUrl=false;
       if (branchOnStr!="") {
          vector<string> vec;
          CCCommonUtils::splitString(branchOnStr, "|", vec);
          if(vec.size()==2){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
             if (vec[0]=="1") {
                isUrl=true;
             }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
             if (vec[1]=="1") {
                isUrl=true;
             }
#endif
          }
       }
       if (isUrl) {
          m_infoLabel1->setString(_lang_1("105376", m_inviteStr.c_str()));
       }else{
          m_infoLabel1->setString(_lang_1("105368", m_inviteStr.c_str()));
       }
       
       
        m_infoLabel2->setString(_lang("105063"));
//        CCCommonUtils::setButtonTitle(m_goFacebookBtn, _lang("105060").c_str());
        CCCommonUtils::setButtonTitle(m_goTwitterBtn, _lang("105061").c_str());
       
       if (GlobalData::shared()->isChinaPlatForm()) {
          m_twitterIcon->setVisible(false);
          m_goTwitterBtn->setVisible(false);
          m_infoLabel2->setVisible(false);
       }
        ret = true;
    }
    return ret;
}

void InvitationMethodView::onEnter() {
    PopupBaseView::onEnter();
   setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
   setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void InvitationMethodView::onExit() {
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void InvitationMethodView::onGoTwitterClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
    string url = "https://twitter.com/ClashOfKingsCOK";
    GameController::getInstance()->goTurntoUrl(url);
}

SEL_CCControlHandler InvitationMethodView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoFacebookClick", InvitationMethodView::onGoFacebookClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoTwitterClick", InvitationMethodView::onGoTwitterClick);
    return NULL;
}

bool InvitationMethodView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel1", CCLabelIF*, m_infoLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel2", CCLabelIF*, m_infoLabel2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goFacebookBtn", CCControlButton*, m_goFacebookBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goTwitterBtn", CCControlButton*, m_goTwitterBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_twitterIcon", CCNode*, m_twitterIcon);
    return false;
}

bool InvitationMethodView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void InvitationMethodView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}
///

InviteRewardInfoView* InviteRewardInfoView::create() {
    auto ret = new InviteRewardInfoView();
    if (ret && ret->init()){
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool InviteRewardInfoView::init() {
    bool ret = false;
    if (PopupBaseView::init()) {
       setIsHDPanel(true);
        CCBLoadFile("InviteRewardInfoView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        m_infoLabel1->setString(_lang("105064"));
        m_infoLabel2->setString(_lang("105065"));
        m_infoLabel3->setString(_lang("105066"));
        m_infoLabel4->setString(_lang("105067"));
        
        vector<string> tmpvec;
        string tmpstr;
        
        CCCommonUtils::splitString(_lang("115169"), "}", tmpvec);
        tmpstr = "100";
        tmpstr += tmpvec[1];
        tmpstr += " * 1";
        m_infoLabel5->setString(tmpstr);
        tmpvec.clear();
        tmpstr = _lang("104000");
        tmpstr += " * 2";
        m_infoLabel13->setString(tmpstr);
        
        tmpstr = _lang("104054");
        tmpstr += " * 1";
        m_infoLabel6->setString(tmpstr);
        tmpstr = _lang("104297");
        tmpstr += " * 1";
        m_infoLabel14->setString(tmpstr);
        
        CCCommonUtils::splitString(_lang("115169"), "}", tmpvec);
        tmpstr = "100";
        tmpstr += tmpvec[1];
        tmpstr += " * 2";
        m_infoLabel7->setString(tmpstr);
        tmpvec.clear();
        tmpstr = _lang("104148");
        tmpstr += " * 2";
        m_infoLabel15->setString(tmpstr);
        
        tmpstr = _lang("104054");
        tmpstr += " * 1";
        m_infoLabel8->setString(tmpstr);
        tmpstr = _lang("104297");
        tmpstr += " * 2";
        m_infoLabel16->setString(tmpstr);
        
        CCCommonUtils::splitString(_lang("115169"), "}", tmpvec);
        tmpstr = "100";
        tmpstr += tmpvec[1];
        tmpstr += " * 3";
        m_infoLabel9->setString(tmpstr);
        tmpvec.clear();
        tmpstr = _lang("104148");
        tmpstr += " * 2";
        m_infoLabel17->setString(tmpstr);
        
        tmpstr = _lang("104054");
        tmpstr += " * 2";
        m_infoLabel10->setString(tmpstr);
        tmpstr = _lang("104297");
        tmpstr += " * 3";
        m_infoLabel18->setString(tmpstr);
        
        CCCommonUtils::splitString(_lang("115169"), "}", tmpvec);
        tmpstr = "100";
        tmpstr += tmpvec[1];
        tmpstr += " * 5";
        m_infoLabel11->setString(tmpstr);
        tmpvec.clear();
        tmpstr = _lang("104138");
        tmpstr += " * 2";
        m_infoLabel19->setString(tmpstr);
        
        tmpstr = _lang("104054");
        tmpstr += " * 3";
        m_infoLabel12->setString(tmpstr);
        tmpvec.clear();
        tmpstr = _lang("104297");
        tmpstr += " * 4";
        m_infoLabel20->setString(tmpstr);
        
        ret = true;
    }
    return ret;
}

void InviteRewardInfoView::onEnter() {
    PopupBaseView::onEnter();
   setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
   setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void InviteRewardInfoView::onExit() {
    setTouchEnabled(false);
    PopupBaseView::onExit();
}


SEL_CCControlHandler InviteRewardInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool InviteRewardInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel1", CCLabelIF*, m_infoLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel2", CCLabelIF*, m_infoLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel3", CCLabelIF*, m_infoLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel4", CCLabelIF*, m_infoLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel5", CCLabelIF*, m_infoLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel6", CCLabelIF*, m_infoLabel6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel7", CCLabelIF*, m_infoLabel7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel8", CCLabelIF*, m_infoLabel8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel9", CCLabelIF*, m_infoLabel9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel10", CCLabelIF*, m_infoLabel10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel11", CCLabelIF*, m_infoLabel11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel12", CCLabelIF*, m_infoLabel12);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel13", CCLabelIF*, m_infoLabel13);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel14", CCLabelIF*, m_infoLabel14);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel15", CCLabelIF*, m_infoLabel15);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel16", CCLabelIF*, m_infoLabel16);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel17", CCLabelIF*, m_infoLabel17);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel18", CCLabelIF*, m_infoLabel18);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel19", CCLabelIF*, m_infoLabel19);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel20", CCLabelIF*, m_infoLabel20);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    return false;
}

bool InviteRewardInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void InviteRewardInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}



/////
InviteWarningView* InviteWarningView::create(CCCallFunc* yesfunc, CCCallFunc* nofunc) {
   auto ret = new InviteWarningView();
   if (ret && ret->init(yesfunc, nofunc)) {
      ret->autorelease();
   } else {
      CC_SAFE_DELETE(ret);
   }
   return ret;
}

bool InviteWarningView::init(CCCallFunc* yesfunc, CCCallFunc* nofunc) {
   bool ret = false;
   if (PopupBaseView::init()) {
      setIsHDPanel(true);
      CCBLoadFile("InviteWarningView", this, this);
      CCSize size = CCDirector::sharedDirector()->getWinSize();
      setContentSize(size);
      
      m_infoLabel->setString(_lang("2000274"));
      setYesCallback(yesfunc);
      setNoCallback(nofunc);
      CCCommonUtils::setButtonTitle(m_okBtn, _lang("105260").c_str());
      CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("108532").c_str());
      ret = true;
   }
   return ret;
}

void InviteWarningView::onEnter(){
   PopupBaseView::onEnter();
   setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
   setTouchEnabled(true);
   //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void InviteWarningView::onExit() {
   setTouchEnabled(false);
   PopupBaseView::onExit();
}

SEL_CCControlHandler InviteWarningView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
   CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", InviteWarningView::onOKClick);
   CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelClick", InviteWarningView::onCancelClick);
   return NULL;
}

bool InviteWarningView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
//   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, m_okBtn);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn", CCControlButton*, m_cancelBtn);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
   return false;
}

void InviteWarningView::onOKClick(CCObject * pSender, Control::EventType pCCControlEvent) {
   this->retain();
   PopupViewController::getInstance()->removePopupView(this);
   m_onYes->execute();
   this->release();
}

void InviteWarningView::onCancelClick(CCObject * pSender, Control::EventType pCCControlEvent) {
   this->retain();
   PopupViewController::getInstance()->removePopupView(this);
   m_onNo->execute();
   this->release();
}

bool InviteWarningView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
   return true;
}

void InviteWarningView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
   if (isTouchInside(m_touchNode, pTouch)) {
      return;
   }
   this->retain();
   PopupViewController::getInstance()->removePopupView(this);
   m_onNo->execute();
   this->release();
}

