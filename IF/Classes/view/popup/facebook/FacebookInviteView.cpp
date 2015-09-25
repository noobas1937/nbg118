//
//  FacebookInviteView.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-29.
//
//

#include "FacebookInviteView.h"
#include "PopupViewController.h"
#include "FBUtilies.h"
#include "LoadWebImg.h"
#include "ParticleFireAni.h"
#include "FBMineInviteCommand.h"
#include "DataRecordCommand.h"
#include <spine/Json.h>
#include <cctype>
#include <algorithm>
#include "FunBuildController.h"

#define  MSG_FB_INVITE_CELL_CLICK           "MFB_invite_cell_click"

FacebookInviteView::~FacebookInviteView()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBRequestResult);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FB_INVITE_CELL_CLICK);
}

FacebookInviteView::FacebookInviteView(){
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FacebookInviteView::getRequestFriends), MSG_FBRequestResult, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FacebookInviteView::onCellClickHandle), MSG_FB_INVITE_CELL_CLICK, NULL);
}

void FacebookInviteView::onCellClickHandle(CCObject *data){
    int num = m_data->count();
    int haveSelect = 0;
    for (int i=0; i<num; i++) {
        auto dic = _dict(m_data->objectAtIndex(i));
        string flag = dic->valueForKey("flag")->getCString();
        if (flag=="1") {
            haveSelect += 1;
        }
    }
    m_selectAll = haveSelect>0?true:false;
    CCCommonUtils::setButtonSprite(m_selectBtn, m_selectAll?"mail_red_btn.png":"btn_green3.png");
    CCCommonUtils::setButtonTitle(m_selectBtn, _lang(m_selectAll?"101298":"102145").c_str());
}

void FacebookInviteView::getRequestFriends(CCObject *data){
    this->scheduleOnce(schedule_selector(FacebookInviteView::delayShowData), 0.2);
}

void FacebookInviteView::delayShowData(float t){
    CCCommonUtils::flyHint("", "", _lang("105552"));
    m_data->removeAllObjects();
    int result = CCUserDefault::sharedUserDefault()->getIntegerForKey(FB_RequestResult, -1); //1成功  2错误 3取消
    if(true){//result==1
        int num = m_srcData->count();
        for (int i=0; i<num; i++) {
            auto dic = _dict(m_srcData->objectAtIndex(i));
            if (dic!=NULL) {
                string flag = dic->valueForKey("flag")->getCString();
                if (flag=="0") {
                    if(m_data->count()>=50){
                        break;
                    }
                    dic->setObject(CCString::create("1"), "flag");
                    m_data->addObject(dic);
                }
            }
        }
        if(m_inviteNum!=0){
            CCArray* p = CCArray::create();
            p->addObject(CCInteger::create(m_inviteNum));
            CCArray* d = CCArray::create();
            DataRecordCommand *cmd = new DataRecordCommand(FB_INVITE_FRIEND, p, d);
            cmd->sendAndRelease();
        }
        m_tip->setVisible(m_data->count()<=0);
    }
    m_tabView->reloadData();
}

void FacebookInviteView::inviteSuccess(CCObject *data){
    m_data->removeAllObjects();
    for (int i=0; i<m_srcData->count(); i++) {
        auto dic = _dict(m_srcData->objectAtIndex(i));
        if (dic!=NULL) {
            string flag = dic->valueForKey("flag")->getCString();
            if (flag=="0") {
                m_data->addObject(dic);
            }
        }
    }
    m_tabView->reloadData();
}

void FacebookInviteView::initData(){
    string friends = GlobalData::shared()->inviteFriendsInfo;
    if(friends!=""){
        CCLOG(" friends =%s",friends.c_str());
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
            m_srcData->addObject(friendInfo);
        }
        Json_dispose(fjson);
        m_data->removeAllObjects();
        int num = m_srcData->count();
        if(num>=50){
            num = 50;
        }
        for (int i=0; i<num; i++) {
            CCDictionary* dic = _dict(m_srcData->objectAtIndex(i));
            dic->setObject(CCString::create("1"), "flag");
            m_data->addObject(dic);
        }
    }
}

FacebookInviteView *FacebookInviteView::create(CCArray* fdata,string ftype){
    FacebookInviteView *ret = new FacebookInviteView();
    if(ret && ret->init(fdata,ftype)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void FacebookInviteView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("107057"));
    if(m_ftype=="0"){
        this->setTitleName(_lang("105075"));
    }
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
//    if(GlobalData::shared()->inviteFriendsInfo!=""){
//        this->initData();
//    }
//    this->unschedule(schedule_selector(FacebookInviteView::refreshData));
//    this->schedule(schedule_selector(FacebookInviteView::refreshData),1);
    this->refreshData(0.0);
}

void FacebookInviteView::onExit(){
    GlobalData::shared()->isBind = false;
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool FacebookInviteView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void FacebookInviteView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void FacebookInviteView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_viewBg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool FacebookInviteView::init(CCArray* fdata,string ftype){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        m_ftype = ftype;
        auto node = CCBLoadFile("InviteFriends", this, this);
        this->setContentSize(node->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height-prev;
        
        if (CCCommonUtils::isIosAndroidPad()) {
            add = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        
        m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width,m_listContainer->getContentSize().height+add));
        m_listContainer->setPositionY(m_listContainer->getPositionY() - add);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - add);
        
        CCCommonUtils::setButtonTitle(m_sendBtn, _lang("107057").c_str());
        if(m_ftype=="0"){
            CCCommonUtils::setButtonTitle(m_sendBtn, _lang("105075").c_str());
        }
        CCCommonUtils::setButtonTitle(m_selectBtn, _lang("101298").c_str());
        CCCommonUtils::setButtonTitle(m_searchBtn, _lang("115269").c_str());
        auto spriteText = CCLoadSprite::createScale9Sprite("world_title_3.png");
        
        if (CCCommonUtils::isIosAndroidPad()) {
            m_inputName = CCEditBox::create(m_searchNode->getContentSize(), spriteText);
        } else {
            m_inputName = CCEditBox::create(CCSizeMake(420,44),spriteText);
        }
        m_inputName->setMaxLength(20);
        m_inputName->setFontSize(24);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_inputName->setFontSize(48);
            m_inputName->setAnchorPoint(CCPointZero);
        }
        m_inputName->setFontColor(ccBLACK);
        m_inputName->setReturnType(kKeyboardReturnTypeDone);
        m_inputName->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
        m_inputName->setText("");
        m_inputName->setPlaceHolder("");
        m_searchNode->addChild(m_inputName);
        
        m_data = CCArray::create();
        m_srcData = CCArray::create();
        
        if (fdata!=NULL) {
            m_srcData->addObjectsFromArray(fdata);
            int num = m_srcData->count();
            for (int i=0; i<num; i++) {
                auto dic = _dict(m_srcData->objectAtIndex(i));
                dic->setObject(CCString::create("0"), "flag");
                if(m_data->count()<50){
                    dic->setObject(CCString::create("1"), "flag");
                    m_data->addObject(m_srcData->objectAtIndex(i));
                }
            }
            CCLog("m_srcData=%d",m_srcData->count());
        }
    
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_tabView);
        
        m_tip->setString(_lang("107067"));
        m_tip->setVisible(false);
        
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        
        //initData();
        m_inviteNum = 0;
        m_selectAll = true;
        ret = true;
    }
    return ret;
}

void FacebookInviteView::refreshData(float t){
    this->removeLoadingAni();
    if(m_data->count()>0){
        this->unschedule(schedule_selector(FacebookInviteView::refreshData));
    }
    m_tabView->reloadData();
    m_tip->setVisible(m_data->count()<=0);
}

void FacebookInviteView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_listContainer->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_listContainer->addChild(m_loadingIcon,1000000);
    }
}


void FacebookInviteView::removeLoadingAni(){
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParentAndCleanup(true);
    }
    m_loadingIcon = NULL;
}

void FacebookInviteView::onSendClick(CCObject *pSender, CCControlEvent event){
    CCArray* arr = CCArray::create();
    for (int i=0; i<m_data->count(); i++) {
        auto dic = _dict(m_data->objectAtIndex(i));
        string flag = dic->valueForKey("flag")->getCString();
        if (arr->count()>50) {
            break;
        }
        if (flag=="1") {
            string id = dic->valueForKey("id")->getCString();;
            arr->addObject(CCString::create(id));
        }
    }
    m_inviteNum = arr->count();
    GlobalData::shared()->isBind = true;
    
    
    string msg = _lang("107087");
    FBUtilies::postFBSelectedFriendList(arr,msg,m_ftype);
    FBUtilies::appEventShareLog(CC_ITOA(GlobalData::shared()->playerInfo.selfServerId), GlobalData::shared()->playerInfo.level, FunBuildController::getInstance()->getMainCityLv(), "107087");
    if (FBUtilies::fbIsLogin() && m_ftype=="2") {
        double lastTime = 0.0;
        string timeStr = CCUserDefault::sharedUserDefault()->getStringForKey("facebookCallForHelpTime","");
        if (timeStr!="") {
            lastTime = atof(timeStr.c_str());
        }
        std:: map<string, CCDictionary*>::iterator it = GlobalData::shared()->shareFbmap.find("call_for_help");
        double ctime = GlobalData::shared()->getWorldTime();
        if (it!=GlobalData::shared()->shareFbmap.end()) {
            CCDictionary* dict = it->second;
            if (dict) {
                int on = dict->valueForKey("ON")->intValue();
                double limit = dict->valueForKey("Limit")->doubleValue();
                string permission = dict->valueForKey("permissions")->getCString();
                if (lastTime > 0) {
                    lastTime = lastTime + limit;
                }
                bool flag = FBUtilies::fbHasGranted(permission);
                if (on==1 && ctime >= lastTime && flag) {
                    CCUserDefault::sharedUserDefault()->setStringForKey("facebookCallForHelpTime", CC_ITOA(ctime));
                    CCUserDefault::sharedUserDefault()->flush();
                    string fbName = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERNAME, "");
                    string name = _lang_1(dict->valueForKey("name")->getCString(),fbName.c_str());
                    string caption = _lang_1(dict->valueForKey("caption")->getCString(),fbName.c_str());
                    string linkDescription = _lang_1(dict->valueForKey("linkDescription")->getCString(),fbName.c_str());
                    string pictureUrl = dict->valueForKey("pictureUrl")->getCString();
                    string link = dict->valueForKey("link")->getCString();
                    string ref = dict->valueForKey("ref")->getCString();
                    FBUtilies::fbPublishFeedDialog(name,caption,linkDescription,link,pictureUrl,1,ref);
                    CCLog("fb ask for help push ");
                }else{
                    CCLog("fb time or on key error on=%d limit=%f permission=%s flag=%d lastTime=%f ctime=%f",on,limit,permission.c_str(),flag,lastTime,ctime);
                }
            }else{
                CCLog("fb dict null");
            }
        }else{
            CCLog("fb have not call_for_help object");
        }
    }else{
        CCLog("fb facebook not login");
    }

}

void FacebookInviteView::onSellectClick(CCObject *pSender, CCControlEvent event){
    m_selectAll = !m_selectAll;
    CCCommonUtils::setButtonSprite(m_selectBtn, m_selectAll?"mail_red_btn.png":"btn_green3.png");
    CCCommonUtils::setButtonTitle(m_selectBtn, _lang(m_selectAll?"101298":"102145").c_str());
    int num = m_data->count();
    for (int i=0; i<num; i++) {
        auto dic = _dict(m_data->objectAtIndex(i));
        dic->setObject(CCString::create(m_selectAll?"1":"0"), "flag");
    }
    m_tabView->reloadData();
    
}

void FacebookInviteView::onSearchClick(CCObject *pSender, CCControlEvent event){
    m_selectAll = true;
    std::string sName = m_inputName->getText();
    int num = m_srcData->count();
    m_data->removeAllObjects();
    std::transform(sName.begin(),sName.end(),sName.begin(),::toupper);
    for (int i=0; i<num; i++) {
        auto dic = _dict(m_srcData->objectAtIndex(i));
        if (dic!=NULL) {
            string name = dic->valueForKey("name")->getCString();
            std::transform(name.begin(),name.end(),name.begin(),::toupper);
            if (name.find(sName) < name.length() && m_data->count()<50) {
                dic->setObject(CCString::create("1"), "flag");
                m_data->addObject(dic);
            }
        }
    }
    refreshData(0.0);
}

bool FacebookInviteView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectBtn", CCControlButton*, this->m_selectBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip", CCLabelIF*, this->m_tip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchBtn", CCControlButton*, this->m_searchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchNode", CCNode*, this->m_searchNode);
    return false;
}

SEL_CCControlHandler FacebookInviteView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendClick", FacebookInviteView::onSendClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSellectClick", FacebookInviteView::onSellectClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSearchClick", FacebookInviteView::onSearchClick);
    return NULL;
}

void FacebookInviteView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize FacebookInviteView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(286, 319);
    }
    return CCSize(120, 145);
}

CCTableViewCell* FacebookInviteView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    FriendPicCell* cell = (FriendPicCell*)table->dequeueGrid();
    CCDictionary* friendInfo = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData(friendInfo);
        }else{
            cell = FriendPicCell::create(friendInfo,m_listContainer);
        }
    }
    return cell;
}

ssize_t FacebookInviteView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_data->count()/5 + (m_data->count()%5==0?0:1);
    return num;
}

unsigned int FacebookInviteView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 5;
}

FriendPicCell *FriendPicCell::create(CCDictionary* info,CCNode* clickArea){
    FriendPicCell *ret = new FriendPicCell(info,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void FriendPicCell::setData(CCDictionary* info){
    m_info = info;
    if (m_info==NULL) {
        return;
    }
    m_head->removeAllChildrenWithCleanup(true);
    string pic = m_info->valueForKey("picture")->getCString();
    LoadWebImg* img = LoadWebImg::create();
    img->setLoadURL(pic);
    img->setScale(1.7);
    m_head->addChild(img);
    string flag = m_info->valueForKey("flag")->getCString();
    m_selectNode->setVisible(flag=="1");
    string name = m_info->valueForKey("name")->getCString();
    if(name.length()>9){
        name = CCCommonUtils::subStrByUtf8(name, 0, 8);
        name.append("...");
    }
    m_nameTxt->setString(name.c_str());
}

void FriendPicCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void FriendPicCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool FriendPicCell::init(){
    auto bg = CCBLoadFile("FriendPicCell", this, this);
    this->setContentSize(bg->getContentSize());
    
    setData(m_info);
    return true;
}

bool FriendPicCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_isDraging = false;
    if(isTouchInside(m_clickNode, pTouch) && isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void FriendPicCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_clickNode, pTouch) && isTouchInside(m_clickArea, pTouch)){
        m_selectNode->setVisible(true);
        string flag = m_info->valueForKey("flag")->getCString();
        if (flag=="1") {
            flag = "0";
        }else{
            flag = "1";
        }
        m_info->setObject(CCString::create(flag), "flag");
        m_selectNode->setVisible(flag=="1");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FB_INVITE_CELL_CLICK);
    }
}

void FriendPicCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void FriendPicCell::changePic(){
    
}

void FriendPicCell::changeSuccess(){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_PLAYER_PIC);
}

bool FriendPicCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectNode", CCNode*, this->m_selectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    return false;
}

SEL_CCControlHandler FriendPicCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}