//
//  DailyRwdView.cpp
//  IF
//
//  Created by xxrdsg on 14-12-31.
//
//

#include "DailyRwdView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "ActivityController.h"
#include "RewardController.h"
#include "TipsView.h"
#include "UseToolView.h"
#include "SoundController.h"
#include "UserBindCommand.h"
#include "FBUtilies.h"
#include "YesNoDialog.h"
#include "FeedRecordCommand.h"
#include <spine/Json.h>
#include "FunBuildController.h"
#include "ChestUseView.h"

DailyRwdView::DailyRwdView():m_touchNode(NULL),m_bgNode(NULL),m_scrollView(NULL){
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0,0));
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyRwdView::afterGetFriendsInfo), MSG_FBFriendsList, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyRwdView::loginSuccess), MSG_FBLoginSucess, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyRwdView::bindSuccess), MSG_USER_BIND_OK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyRwdView::getInviteFriends), MSG_FBIviteFriends, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyRwdView::getRequestFriends), MSG_FBRequestResult, NULL);
}

DailyRwdView::~DailyRwdView(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFriendsList);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBIviteFriends);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBLoginSucess);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USER_BIND_OK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBRequestResult);
}

void DailyRwdView::afterGetFriendsInfo(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    CCArray* friends = dynamic_cast<CCArray*>(param);
    if (friends) {
        m_friendDatas->removeAllObjects();
        int num = friends->count();
        string ids = "";
        for (int i=0; i<num; i++) {
            auto dic = _dict(friends->objectAtIndex(i));
            if(dic){
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = dic->valueForKey("id")->getCString();
                info->isCheck = true;
                info->name = dic->valueForKey("name")->getCString();
                info->url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",dic->valueForKey("id")->getCString())->getCString();
                string devices = dic->valueForKey("devices")->getCString();
                m_friendDatas->addObject(info);
            }
        }
    }else{
        string installFriendsInfo = CCUserDefault::sharedUserDefault()->getStringForKey("installFriendsInfo", "");
        if(installFriendsInfo!=""){
            GlobalData::shared()->installFriendsInfo = installFriendsInfo;
            Json* fjson = Json_create(installFriendsInfo.c_str());
            if (fjson) {
                int size = Json_getSize(fjson);
                CCLog("parse installFriendsInfo =%s",installFriendsInfo.c_str());
                for (int i=0; i<size; i++) {
                    Json *item = Json_getItemAt(fjson, i);
                    string name = Json_getString(item,"name","");
                    string id = Json_getString(item,"id","");
                    string url = "";
                    FBFriendInfo* info = FBFriendInfo::create();
                    info->id = id;
                    info->isCheck = true;
                    info->name = name;
                    info->url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",id.c_str())->getCString();
                    m_friendDatas->addObject(info);
                }
                Json_dispose(fjson);
            }
        }
    }
    addFriends();
    m_count += 1;
}

void DailyRwdView::loginSuccess(CCObject* p){
    string m_facebookUid =CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_uid", "");
    if (m_facebookUid == "") {
        return;
    }
    string preuid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if(preuid!="" && preuid!=m_facebookUid){
        CCCommonUtils::flyHint("", "", _lang("107078"));
        FBUtilies::fbLogout();//不是同一个号
        return ;
    }
    string preuName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
    string m_facebookName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
    UserBindCommand* cmd = new UserBindCommand("",m_facebookUid,"","","",1,preuid,"",m_facebookName,preuName);
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void DailyRwdView::bindSuccess(CCObject* p){
    this->unschedule(schedule_selector(DailyRwdView::checkFriend));
    this->schedule(schedule_selector(DailyRwdView::checkFriend),0.35);
    FBUtilies::getFBFriendList();
    FBUtilies::inviteFriends();
}

void DailyRwdView::getInviteFriends(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    CCArray* inviteFriends = dynamic_cast<CCArray*>(param);
    if(inviteFriends){
        int num = inviteFriends->count();
        string ids = "";
        for (int i=0; i<num; i++) {
            auto dic = _dict(inviteFriends->objectAtIndex(i));
            if(dic){
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = dic->valueForKey("id")->getCString();
                info->isCheck = true;
                info->name = dic->valueForKey("name")->getCString();
                info->url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",dic->valueForKey("id")->getCString())->getCString();
                m_inviteDatas->addObject(info);
            }
        }
    }else{

        string info = CCUserDefault::sharedUserDefault()->getStringForKey("inviteFriends", "");
        if(info!=""){
            GlobalData::shared()->inviteFriendsInfo = info;
            string friends = info;
            if(friends!=""){
                Json* fjson = Json_create(friends.c_str());
                CCLog("parse inviteFriends info =%s",info.c_str());
                if (fjson) {
                    int size = Json_getSize(fjson);
                    for (int i=0; i<size; i++) {
                        Json *item = Json_getItemAt(fjson, i);
                        string name = Json_getString(item,"name","");
                        string id = Json_getString(item,"id","");
                        
                        FBFriendInfo* info = FBFriendInfo::create();
                        info->id = id;
                        info->isCheck = true;
                        info->name = name;
                        info->url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",id.c_str())->getCString();
                        m_inviteDatas->addObject(info);
                    }
                    Json_dispose(fjson);
                }
            }
        }
    }
    addFriends();
    m_count += 1;
}

void DailyRwdView::getRequestFriends(CCObject *data){
    string  fbRequestId = CCUserDefault::sharedUserDefault()->getStringForKey(FB_RequestID,"");
    int result = CCUserDefault::sharedUserDefault()->getIntegerForKey(FB_RequestResult, -1); //1成功  2错误 3取消
    m_requestId = fbRequestId;
    if(m_requestId==""){
        CCLog("m_requestId is null");
        return ;
    }
    m_requestId.append("_feed");
    string link = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    link = "https://fb.me/827859737255348?feed_key=";
#else
    link = "https://fb.me/789290781112244?feed_key=";
#endif
    link.append(m_requestId);
    int index  = GlobalData::shared()->getRand(1,3);
    string fbIcon = CCString::createWithFormat("http://cok.eleximg.com/cok/img/fb_feed_0%d.png",index)->getCString();
    string caption = _lang("111076");
    if(caption==""){
        caption = _lang("107088");
    }
    string desc = _lang("111076");
    if(desc==""){
        desc = _lang("107087");
    }
    CCLog("fb fbRequestId=%s",fbRequestId.c_str());
    //FBUtilies::fbPublishFeedDialog("Clash Of Kings",caption,desc,link,fbIcon,1);
    
    FeedRecordCommand* cmd = new FeedRecordCommand(fbRequestId);
    cmd->sendAndRelease();
    this->getDailyReward();
}

DailyRwdView* DailyRwdView::create(int type)
{
    DailyRwdView* ret = new DailyRwdView();
    if(ret && ret->init(type)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DailyRwdView::init(int type)
{
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    CCNode* bg =NULL;
    m_viewType = type;
    if(m_viewType==1){
        bg = CCBLoadFile("qiandaoPopView",this,this);
        GameController::getInstance()->callXCApi("action=loginDay_7rwd_pop");
        m_selectBtn->setTouchPriority(0);
        m_friendBtn->setTouchPriority(0);
        m_rwdBtn->setTouchPriority(0);
        m_friendBtn->setOpacity(20);
        
        m_rwdBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);//fusheng add
    }else{
        bg = CCBLoadFile("qiandaoView",this,this);
        GameController::getInstance()->callXCApi("action=loginDay_7rwd_click");
        int dh = CCDirector::sharedDirector()->getWinSize().height - 852;
        if (CCCommonUtils::isIosAndroidPad())
        {
            dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        
        m_scNode->setPositionY(m_scNode->getPositionY() - dh);
        m_scNode->setContentSize(CCSize(m_scNode->getContentSize().width, m_scNode->getContentSize().height + dh));
        if (PortActController::getInstance()->m_isNewTimeRwd )
        {
            m_scNode->setContentSize(CCSize(m_scNode->getContentSize().width, m_scNode->getContentSize().height + 87));
            m_shipPicNode->setPositionY(m_shipPicNode->getPositionY() + 87);
        }
        auto tsize = m_listNode->getContentSize();
        //    m_listBg->setPreferredSize(CCSize(tsize.width + 30, tsize.height + 35));
        //    m_bottomNode->setPositionY(m_bottomNode->getPositionY() - dh);
        if(m_bgNode){
            m_bgNode->setPositionY(m_bgNode->getPositionY() - dh);
            auto tbg = CCLoadSprite::loadResource("bg0.png");
            auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
            auto pic = CCLoadSprite::createSprite("bg0.png");
            int maxHeight = CCDirector::sharedDirector()->getWinSize().height  - pic->getContentSize().height+1100;
            int curHeight = 0;
            while(curHeight < maxHeight)
            {
                auto pic2Left = CCLoadSprite::createSprite("bg0.png");
                pic2Left->setAnchorPoint(ccp(0, 0));
                pic2Left->setPosition(ccp(0, curHeight));
                tBatchNode->addChild(pic2Left);
                auto pic2Right = CCLoadSprite::createSprite("bg0.png");
                pic2Right->setScaleX(-1);
                pic2Right->setAnchorPoint(ccp(1, 0));
                pic2Right->setPosition(ccp(320, curHeight));
                if (CCCommonUtils::isIosAndroidPad())
                {
                    pic2Left->setScaleX(2.4);
                    pic2Right->setScaleX(2.4);
                    pic2Right->setPosition(ccp(768, curHeight));
                }
                tBatchNode->addChild(pic2Right);
                curHeight += pic2Left->getContentSize().height;
            }
            auto pic1Left = CCLoadSprite::createSprite("bg0.png");
            curHeight = CCDirector::sharedDirector()->getWinSize().height - 300 - pic1Left->getContentSize().height;
            pic1Left->setPosition(ccp(0, curHeight));
            pic1Left->setAnchorPoint(ccp(0, 0));
            tBatchNode->addChild(pic1Left);
            auto pic1Right = CCLoadSprite::createSprite("bg0.png");
            pic1Right->setScaleX(-1);
            pic1Right->setPosition(ccp(320, curHeight));
            if (CCCommonUtils::isIosAndroidPad())
            {
                pic1Left->setScaleX(2.4);
                pic1Right->setScaleX(2.4);
                pic1Right->setPosition(ccp(768, curHeight));
            }
            pic1Right->setAnchorPoint(ccp(1, 0));
            tBatchNode->addChild(pic1Right);
            tBatchNode->setPosition(0, 0);
            m_bgNode->addChild(tBatchNode);
            if(CCCommonUtils::isIosAndroidPad())
            {
                tBatchNode->setScaleX(1536/640);
            }
        }
        
        m_selectBtn->setTouchPriority(Touch_Popup);
        m_friendBtn->setTouchPriority(Touch_Popup);
        m_rwdBtn->setTouchPriority(Touch_Popup);
        m_rwdBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);//fusheng add
        
        m_scrollView = CCScrollView::create(m_scNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(Touch_Popup);
        m_scNode->addChild(m_scrollView);
        
        m_moveNode->removeFromParent();
        m_moveNode->setPosition(320, 750);
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            m_moveNode->setPosition(768, 0);
//        }
        m_scrollView->addChild(m_moveNode);
        
        m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,900));//1350
        m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - 900));
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,600));
//            m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - 600));
//        }
        
        if(m_tipTxt1){
            m_tipTxt1->setString(_lang("111073"));
        }
        if(m_tipTxt2){
            m_tipTxt2->setString(_lang("111074"));
            int h = 620 + m_tipTxt2->getContentSize().height * m_tipTxt2->getOriginScaleY();
            m_moveNode->setPosition(320, h);
            m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width, h + 160));//1350
            m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - h - 160));
        }
        m_tipTxt3->setString(_lang("111067"));
        m_tipTxt3->setVisible(false);
        m_selectBtn->setOpacity(20);
        m_friendBtn->setOpacity(20);
    }
    
    m_friendTxt->setString(_lang("111064"));
    
    m_selectBtn->setSelected(true);
    m_selectSpr->setVisible(m_selectBtn->isVisible());
    
    
    m_vipLabel->setString(_lang("111072"));
    setContentSize(bg->getContentSize());
    m_friendNode->setVisible(false);
    
    m_data = CCArray::create();
    m_tabView = CCMultiColTableView::create(this, m_listNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup_Item);
    m_listNode->addChild(m_tabView);
    
    m_waitInterface1 = NULL;
    m_waitInterface2 = NULL;
    CCCommonUtils::setButtonTitle(m_rwdBtn, _lang("111052").c_str());
    
    m_itemId = PortActController::getInstance()->m_loginDay;
    if (PortActController::getInstance()->m_isRdLoginDay == 0 && PortActController::getInstance()->m_isVipRdLoginDay == 0) {//今天没有领取
        if (m_itemId < PortActController::getInstance()->m_loginDayMap.size()) {
            m_itemId += 1;
        }
    }
    
    m_friendDatas = CCArray::create();
    m_inviteDatas = CCArray::create();
    m_showDatas = CCArray::create();
    m_count = 0;
    m_sendId = "";
    m_inviteNum = 0;
    m_notInstallNum = 0;
    m_installNum = 0;
    
    bool bVip = !GlobalData::shared()->isXiaoMiPlatForm();
    string fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if(FBUtilies::fbIsLogin() && fbUid!="" && !GlobalData::shared()->isXiaoMiPlatForm()){
        this->unschedule(schedule_selector(DailyRwdView::checkFriend));
        this->schedule(schedule_selector(DailyRwdView::checkFriend),0.5);
        FBUtilies::getFBFriendList();
        FBUtilies::inviteFriends();
        if(bVip==true && m_viewType==1){
            bVip=false;
        }
        m_infoNode->setVisible(true);
    }else{
        m_infoNode->setVisible(false);
    }
    
    m_vipLabel->setVisible(bVip);
    return true;
}

void DailyRwdView::checkFriend(float t_time){
    if (m_count>=2) {
        this->unschedule(schedule_selector(DailyRwdView::checkFriend));
        this->initFriend();
    }
}

void DailyRwdView::resetTabViewPos()
{
    int allLineNum = ceil(PortActController::getInstance()->m_loginDayMap.size() / 5.0);
    int curLine = (m_itemId - 1) / 5 + 1;
    int showLineNum = m_listNode->getContentSize().height / gridSizeForTable(NULL).height;
    if (curLine <= showLineNum) {
        m_tabView->setContentOffset(ccp(0, m_listNode->getContentSize().height - m_tabView->getContentSize().height));
    } else {
        if (allLineNum - showLineNum < curLine) {
            m_tabView->setContentOffset(ccp(0, 0));
        } else {
            m_tabView->setContentOffset(ccp(0, m_listNode->getContentSize().height - (allLineNum - curLine + 1) * gridSizeForTable(NULL).height));
        }
    }
    m_friendNode->setVisible(false);
}

bool DailyRwdView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(m_viewType==1 && m_touchNode) {
        return true;
    }
    return false;
}
void DailyRwdView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(m_viewType==1 && m_touchNode) {
        if (!isTouchInside(m_touchNode, pTouch)) {
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
}
void DailyRwdView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}

void DailyRwdView::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyRwdView::onRmvWaitInter1), PORT_LOGINRD_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyRwdView::onRmvWaitInter2), PORT_VIP_LOGINRD_END, NULL);
    generateData(NULL);
    refreshRDData();
    refreshTitle();
    refreshBtnState();
    resetTabViewPos();
    if(m_viewType==1){
        setSwallowsTouches(false);
        setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        setTouchEnabled(true);
        //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default,false);
    }
}

void DailyRwdView::onExit(){
    if (m_waitInterface1 != NULL) {
        m_waitInterface1->remove();
        m_waitInterface1 = NULL;
    }
    if (m_waitInterface2 != NULL) {
        m_waitInterface2->remove();
        m_waitInterface2 = NULL;
    }
    GlobalData::shared()->isBind = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PORT_LOGINRD_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PORT_VIP_LOGINRD_END);
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

void DailyRwdView::generateData(CCObject *p)
{
    m_data->removeAllObjects();
    m_listNode->removeAllChildrenWithCleanup(true);
    for (int i = 1; i <= PortActController::getInstance()->m_loginDayMap.size(); ++i)
    {
        //m_data->addObject(CCInteger::create(i));
        DailyCell* cell =DailyCell::create(i,m_scNode);
        

        
        
//        if(i<=4){
//                cell->setPosition(ccp(i*140 - 125, 190));
//            }else{
//                cell->setPosition(ccp((i-4)*140 -50 , 20));
//            }
        
        
        
        if(i<=4){
            cell->setPosition(ccp(i*143 - 132 -11, 190 - 50));
        }
        else if(i==7)
        {
            cell->setPosition(ccp((i-4)*143 - 57 -13, 20 - 84 - 15));
        }
        else{
            cell->setPosition(ccp((i-4)*143 -132 -11, 20 - 84 - 15));
        }
        
        
        
        m_listNode->addChild(cell);
        
    }
    //m_tabView->reloadData();
}

void DailyRwdView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
    if (!this->isVisible()) {
        return;
    }
    DailyCell* dailyCell = dynamic_cast<DailyCell*>(cell);
    if (dailyCell && m_listNode->isVisible()) {
        dailyCell->cellTouchEnded(m_tabView->m_pCurTouch);
    }
}

CCSize DailyRwdView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    return CCSize(136+2 , 196 + 25);
}

CCTableViewCell* DailyRwdView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if (idx >= m_data->count()) {
        return NULL;
    }
    
    int itemId = dynamic_cast<CCInteger*>(m_data->objectAtIndex(idx))->getValue();
    DailyCell* cell = (DailyCell*)table->dequeueGrid();
    if (cell) {
        cell->setData(itemId);
    } else {
        cell = DailyCell::create(itemId,m_listNode);
    }
    return cell;
}

ssize_t DailyRwdView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table)
{
    return ceil(m_data->count()/5.0);
}

unsigned int DailyRwdView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable)
{
    return 5;
}


void DailyRwdView::refreshRDData()
{

    //刷新奖励数据
//    auto& arr = PortActController::getInstance()->m_loginDayMap[m_itemId].vipReward;
//    if(arr==NULL || arr->count()<=0) return ;
//    //数组只有一个元素  即只有一种vip奖励
//    CCDictionary* item = _dict(arr->objectAtIndex(0));
//    int type = item->valueForKey("type")->intValue();
//    string name = "";
//    string num = "";
//    string info = "";
//    if (type == R_GOODS) {
//        CCDictionary* value = _dict(item->objectForKey("value"));
//        num = value->valueForKey("num")->getCString();
//        string toolId = value->valueForKey("id")->getCString();
//        name = CCCommonUtils::getNameById(toolId);
//    } else {
//        name = RewardController::getInstance()->getNameByType(type, 0);
//        num = item->valueForKey("value")->getCString();
//    }
    //m_vipLabel->setString(_lang_2("111050", num.c_str(), name.c_str()));
}

void DailyRwdView::onSelectClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    if(m_selectSpr->isVisible()){
        YesNoDialog* dialog = YesNoDialog::showVariableTitle(_lang("111068").c_str(),CCCallFunc::create(this, callfunc_selector(DailyRwdView::yesFuns)),_lang("111066").c_str());
        CCCommonUtils::setButtonTitle(dialog->m_btnCancel, _lang("111065").c_str());
        dialog->showCancelButton();
    }else{
        m_selectSpr->setVisible(true);
    }
}

void DailyRwdView::yesFuns(){
    m_selectSpr->setVisible(false);
}
void DailyRwdView::onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->removeAllPopupView();
}
void DailyRwdView::onFriendClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    if(!m_friendNode->isVisible()){
        m_friendNode->setVisible(true);
        m_listNode->setVisible(false);
        if(m_friendNode->getChildrenCount()<=1){
            this->initFriend();
        }
    }else{
        m_friendNode->setVisible(false);
        m_listNode->setVisible(true);
    }
}

void DailyRwdView::initFriend(){
    int num = m_showDatas->count();
    if(m_friendNode->getChildrenCount()<=1){
        int totalH = 10*36-5;
        for (int i=0; i<num; i++) {
            FBFriendInfo* info = dynamic_cast<FBFriendInfo*>(m_showDatas->objectAtIndex(i));
            DailyFriendCell* cell = DailyFriendCell::create(info,m_scNode);
            int row = i/2;
            int col = i%2;
            cell->setPosition(ccp(col*300+20, totalH - (row+1)*36));
            m_friendNode->addChild(cell);
        }
    }
    m_tipTxt3->setVisible(num<=0);
}

void DailyRwdView::addFriends(){
    m_showDatas->removeAllObjects();
    int inviteNum = 6;
    int installNum = 14;
    int total = 20;
    int num = m_friendDatas->count();
    m_sendId = "";
    m_installNum = 0;
    m_notInstallNum = 0;
    if(num>installNum){
        for (int i=0; i<200; i++) {
            int rand = GlobalData::shared()->getRand(0,num-1);
            FBFriendInfo* info = dynamic_cast<FBFriendInfo*>(m_friendDatas->objectAtIndex(rand));
            if (m_sendId.find(info->id)<m_sendId.length()) {
                continue;
            }
            if(m_showDatas->count()>=installNum){
                break;
            }
            m_sendId.append(",");
            m_sendId.append(info->id);
            m_showDatas->addObject(info);
        }
    }else{
        m_showDatas->addObjectsFromArray(m_friendDatas);
    }
    m_installNum = m_showDatas->count();
    
    int num2 = m_inviteDatas->count();
    int tNum = m_showDatas->count();
    if(num2>inviteNum){
        for (int i=0; i<100; i++) {
            int rand = GlobalData::shared()->getRand(0,num2-1);
            FBFriendInfo* info = dynamic_cast<FBFriendInfo*>(m_inviteDatas->objectAtIndex(rand));
            if (m_sendId.find(info->id)<m_sendId.length()) {
                continue;
            }
            if(m_showDatas->count()>=(inviteNum+tNum)){
                break;
            }
            m_sendId.append(",");
            m_sendId.append(info->id);
            m_showDatas->addObject(info);
        }
        m_notInstallNum = inviteNum;
    }else{
        m_showDatas->addObjectsFromArray(m_inviteDatas);
        m_notInstallNum = m_inviteDatas->count();
    }
    int prev = 0;
    if(m_showDatas->count()<total){
        if(num>installNum){
            prev = m_showDatas->count();
            for (int i=0; i<m_friendDatas->count(); i++) {
                FBFriendInfo* info = dynamic_cast<FBFriendInfo*>(m_friendDatas->objectAtIndex(i));
                if (m_sendId.find(info->id)<m_sendId.length()) {
                    continue;
                }
                if(m_showDatas->count()>=total){
                    break;
                }
                m_sendId.append(",");
                m_sendId.append(info->id);
                m_showDatas->addObject(info);
            }
            m_installNum += (m_showDatas->count() - prev) ;
        }else if(num2>inviteNum){
            prev = m_showDatas->count();
            for (int i=0; i<m_inviteDatas->count(); i++) {
                FBFriendInfo* info = dynamic_cast<FBFriendInfo*>(m_inviteDatas->objectAtIndex(i));
                if (m_sendId.find(info->id)<m_sendId.length()) {
                    continue;
                }
                if(m_showDatas->count()>=total){
                    break;
                }
                m_sendId.append(",");
                m_sendId.append(info->id);
                m_showDatas->addObject(info);
            }
            m_notInstallNum += (m_showDatas->count() - prev) ;
        }
    }
}

SEL_CCControlHandler DailyRwdView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRwdBtn", DailyRwdView::onClickRwdBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTipBtn", DailyRwdView::onClickTipBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSelectClick", DailyRwdView::onSelectClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFriendClick", DailyRwdView::onFriendClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", DailyRwdView::onCloseBtnClick);
    return NULL;
}

bool DailyRwdView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shipPicNode", CCNode*, this->m_shipPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBg", CCScale9Sprite*, this->m_listBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dayNode", CCNode*, this->m_dayNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLabel", CCLabelIF*, this->m_vipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdBtn", CCControlButton*, this->m_rwdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdList", CCNode*, this->m_rwdList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftSpr", CCSprite*, this->m_leftSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightSpr", CCSprite*, this->m_rightSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_friendBtn", CCControlButton*, this->m_friendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectBtn", CCControlButton*, this->m_selectBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_friendTxt", CCLabelIF*, this->m_friendTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_friendNode", CCNode*, this->m_friendNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCSprite*, m_selectSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scNode", CCNode*, this->m_scNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt1", CCLabelIF*, this->m_tipTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt2", CCLabelIF*, this->m_tipTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt3", CCLabelIF*, this->m_tipTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    
    return false;
}

void DailyRwdView::onClickRwdBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{

    GlobalData::shared()->isBind = true;
    m_inviteNum = 0;
    string fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if (!m_selectSpr->isVisible() || !FBUtilies::fbIsLogin() || m_showDatas->count()<=0 || fbUid=="") {
        this->getDailyReward();
    }else{
        if(FBUtilies::fbIsLogin()){
            string dialog = "111062";
            string msg = _lang("111062");
            if(msg==""){
                dialog = "107077";
                msg = _lang("107077");
            }
            CCArray* arr = CCArray::create();
            for (int i=0; i<m_showDatas->count(); i++) {
                FBFriendInfo* fInfo = dynamic_cast<FBFriendInfo*>(m_showDatas->objectAtIndex(i));
                if(fInfo && fInfo->isCheck){
                    arr->addObject(CCString::create(fInfo->id));
                }
            }
            m_inviteNum = arr->count();
            FBUtilies::postFBSelectedFriendList(arr,msg,"1");
            FBUtilies::appEventShareLog(CC_ITOA(GlobalData::shared()->playerInfo.selfServerId), GlobalData::shared()->playerInfo.level, FunBuildController::getInstance()->getMainCityLv(), dialog);
        }else{
            m_count = 0;
            bool flag = FBUtilies::fbLogin();
            if(!flag){
                //todo 2.3 android 以下，直接领取
                this->getDailyReward();
            }
        }
    }
}

void DailyRwdView::getDailyReward(){
    m_waitInterface1 = GameController::getInstance()->showWaitInterface(m_rwdBtn);
    if (PortActController::getInstance()->m_isRdLoginDay == 0) {//今天没有领取
        if(m_inviteNum==0){
            m_notInstallNum = 0;
            m_installNum = 0;
        }
        //获取奖励
        PortActController::getInstance()->startGetCheckInRwd(0,m_inviteNum,m_notInstallNum,m_installNum);
    } else {//今天已领取
        refreshBtnState();
        if (m_waitInterface1 != NULL) {
            m_waitInterface1->remove();
            m_waitInterface1 = NULL;
        }
    }
}

void DailyRwdView::onClickTipBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    //tip
    string tips = _lang("111059") + "\n" + _lang("111060") + "\n" + _lang("111061");
    if(GlobalData::shared()->isXiaoMiPlatForm()){
        tips =  _lang("111060") + "\n" + _lang("111061");
    }
    PopupViewController::getInstance()->addPopupView(TipsView::create(tips, kCCTextAlignmentLeft));
}

void DailyRwdView::onRmvWaitInter1(CCObject* params)
{
    refreshTitle();
    refreshBtnState();
    if(m_waitInterface1 != NULL){
        m_waitInterface1->remove();
        m_waitInterface1 = NULL;
    }
}

void DailyRwdView::onRmvWaitInter2(CCObject* params)
{
    refreshTitle();
    refreshBtnState();
    if(m_waitInterface2 != NULL){
        m_waitInterface2->remove();
        m_waitInterface2 = NULL;
    }
}

void DailyRwdView::refreshBtnState()
{
    if (PortActController::getInstance()->m_isRdLoginDay) {
        m_rwdBtn->setEnabled(false);
    } else {
        m_rwdBtn->setEnabled(true);
    }
}

void DailyRwdView::refreshTitle()
{
    int day = PortActController::getInstance()->m_loginDay;
    m_titleLabel->setString(_lang_1("111051", CC_ITOA(day)));
    m_leftSpr->setPositionX(-m_titleLabel->getContentSize().width * m_titleLabel->getOriginScaleX() * 0.5 - 10);
    m_rightSpr->setPositionX(m_titleLabel->getContentSize().width * m_titleLabel->getOriginScaleX() * 0.5 + 10);
}

void DailyRwdView::setListNodeVisible(bool st)
{
    if (m_listNode) {
        m_listNode->setVisible(st);
    }
}

//class DailyCell
DailyCell* DailyCell::create(int itemId,CCNode* clickArea)
{
    auto ret = new DailyCell(clickArea);
    if (ret && ret->init(itemId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DailyCell::init(int itemId)
{
    CCBLoadFile("qiandaoCell",this,this);

//    setContentSize(CCSize(114, 112 + 25));
//    m_touchNode->setPosition(114 / 2, 25 + 112 / 2);
//    m_particleNode->setPosition(114 / 2, 27 + 112 / 2);
    
        setContentSize(CCSize(136+2, 196 + 25));
        m_touchNode->setPosition(138 / 2, 25 + 196 / 2);
//        m_particleNode->setPosition(138 / 2, 27 + 196 / 2);

    setData(itemId);
    return true;
}

void DailyCell::setData(int itemId)
{
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    m_flashSpr->setVisible(false);
    m_bgSpr = NULL;
    m_picSpr = NULL;
    m_numLabel->setString("");
    m_dayLabel->setString("");
    m_bgNode->removeAllChildren();
    m_picNode->removeAllChildren();
    m_particleNode->setVisible(false);
    
    m_itemId = itemId;
    
    if(m_itemId == 7)//fusheng 第七天 特殊处理
    {
        m_itemBG->setSpriteFrame("nb_common11_itembg7.png");
        m_particleNode->removeAllChildren();
        
        auto sp = CCLoadSprite::createSprite("nb_common11_itembg7_grow.png");
        
        m_numBG->setPositionX(m_numBG->getPositionX() + 45);
        
        m_numLabel->setPositionX(m_numLabel->getPositionX() + 45);
        
        m_particleNode->addChild(sp);
    }
    
    //fusheng test
//    PortActController::getInstance()->m_loginDay = 7;
//    PortActController::getInstance()->m_isRdLoginDay = 0;
//    PortActController::getInstance()->m_isVipRdLoginDay = 1;
    
    if (!PortActController::getInstance()->m_loginDayMap[m_itemId].reward) {
        return;
    }
    auto& arr = PortActController::getInstance()->m_loginDayMap[m_itemId].reward;
    if(arr==NULL || arr->count()<=0) return ;
    
    m_rdNode->setVisible(false);
    m_spr1->setVisible(false);
    m_spr2->setVisible(false);
    m_spr3->setVisible(false);
    if (m_itemId==1 || m_itemId==2 || m_itemId==3) {
        m_spr1->setVisible(true);
    }else if (m_itemId==6) {
        m_spr3->setVisible(true);
    }else {
        m_spr2->setVisible(true);
    }
    
    if (arr->count() > 1)
    {
//        m_particleNode->setScale(1.3);
        m_dayLabel->setString(_lang_1("111058", CC_ITOA(m_itemId)));
        m_rdNode->setVisible(true);
        
        if (m_itemId < PortActController::getInstance()->m_loginDay) {
            onRefreshBaoXiang(true);
            m_flashSpr->setVisible(true);
        } else if (m_itemId == PortActController::getInstance()->m_loginDay){
            if (PortActController::getInstance()->m_isRdLoginDay == 0 && PortActController::getInstance()->m_isVipRdLoginDay == 1)
            {
                m_particleNode->setVisible(true);
            }
            else {
                onRefreshBaoXiang(true);
                m_flashSpr->setVisible(true);
            }
        } else if (m_itemId == (PortActController::getInstance()->m_loginDay + 1)){
            if (PortActController::getInstance()->m_isRdLoginDay == 0 && PortActController::getInstance()->m_isVipRdLoginDay == 0) {//这一天啥奖也没领
                m_particleNode->setVisible(true);
            }
        } else {
            
        }
    }
    else
    {
        //数组只有一个元素  即只有一种奖励
        CCDictionary* item = _dict(arr->objectAtIndex(0));
        int type = item->valueForKey("type")->intValue();
        string pic = "";
        string name = "";
        string num = "";
        string bg = "kuang";
        if (type == R_GOODS) {
            CCDictionary* value = _dict(item->objectForKey("value"));
            num = value->valueForKey("num")->getCString();
            string toolId = value->valueForKey("id")->getCString();
            pic = CCCommonUtils::getIcon(toolId);
            name = CCCommonUtils::getNameById(toolId);
        } else if (type == R_EQUIP) {
            CCDictionary* value = _dict(item->objectForKey("value"));
            num = value->valueForKey("num")->getCString();
            string toolId = value->valueForKey("id")->getCString();
            pic = CCCommonUtils::getIcon(toolId);
            name = CCCommonUtils::getNameById(toolId);
        } else {
            pic = RewardController::getInstance()->getPicByType(type, 0);
            name = RewardController::getInstance()->getNameByType(type, 0);
            num = item->valueForKey("value")->getCString();
        }
        int multiple = PortActController::getInstance()->m_loginDayMap[m_itemId].multiple;
        bg += CC_ITOA(multiple);
        bg += ".png";
        m_bgSpr = CCLoadSprite::createSprite(bg.c_str());
        m_picSpr = CCLoadSprite::createSprite(pic.c_str());
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            m_bgSpr->setScale(2.f);
//            CCCommonUtils::setSpriteMaxSize(m_picSpr, 90*2, true);
//        }
//        else
//            CCCommonUtils::setSpriteMaxSize(m_picSpr, 90, true);
        
        
        if(m_itemId == 7)//fusheng 第七天 特殊处理
        {
            CCCommonUtils::setSpriteMaxSize(m_picSpr, 152, true);
        }
        else
        {
            CCCommonUtils::setSpriteMaxSize(m_picSpr, 110, true);
        }
        
        m_bgNode->addChild(m_bgSpr);
        m_picNode->addChild(m_picSpr);
        m_numLabel->setString(num);
        int isShowDay = PortActController::getInstance()->m_loginDayMap[m_itemId].showDay;
        if (true) {//isShowDay == 1
            m_dayLabel->setString(_lang_1("111058", CC_ITOA(m_itemId)));
            m_dayLabel->setVisible(true);
        } else {
            m_dayLabel->setString("");
            m_dayLabel->setVisible(false);
        }
        
        CCCommonUtils::setSpriteGray(m_bgSpr, false);
        CCCommonUtils::setSpriteGray(m_picSpr, false);
        CCCommonUtils::setSpriteGray(m_itemBG, false);
        
        if (m_itemId < PortActController::getInstance()->m_loginDay) {
            CCCommonUtils::setSpriteGray(m_bgSpr, true);
            CCCommonUtils::setSpriteGray(m_picSpr, true);
            CCCommonUtils::setSpriteGray(m_itemBG, true);
            
            m_dayLabel->setColor(ccWHITE);
            
            m_flashSpr->setVisible(true);
            m_particleNode->setVisible(false);
        } else if (m_itemId == PortActController::getInstance()->m_loginDay){
            if (PortActController::getInstance()->m_isRdLoginDay == 0 && PortActController::getInstance()->m_isVipRdLoginDay == 1)
            {
                CCCommonUtils::setSpriteGray(m_bgSpr, false);
                CCCommonUtils::setSpriteGray(m_picSpr, false);
                CCCommonUtils::setSpriteGray(m_itemBG, false);
                
                

                m_flashSpr->setVisible(false);
                m_particleNode->setVisible(true);
            }
            else {
                CCCommonUtils::setSpriteGray(m_bgSpr, true);
                CCCommonUtils::setSpriteGray(m_picSpr, true);
                CCCommonUtils::setSpriteGray(m_itemBG, true);

                m_dayLabel->setColor(ccWHITE);
                
                m_flashSpr->setVisible(true);
                m_particleNode->setVisible(false);
            }
        } else if (m_itemId == (PortActController::getInstance()->m_loginDay + 1)){
            if (PortActController::getInstance()->m_isRdLoginDay == 0 && PortActController::getInstance()->m_isVipRdLoginDay == 0) {//这一天啥奖也没领
                CCCommonUtils::setSpriteGray(m_bgSpr, false);
                CCCommonUtils::setSpriteGray(m_picSpr, false);
                CCCommonUtils::setSpriteGray(m_itemBG, false);
                
                
                m_flashSpr->setVisible(false);
                m_particleNode->setVisible(true);
            }
        } else {
            
        }
    }
}

void DailyCell::onEnter() {
    CCNode::onEnter();
    
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyCell::refreshRd), PORT_LOGINRD_END, NULL);
}

void DailyCell::onExit() {
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PORT_LOGINRD_END);
    CCNode::onExit();
}


bool DailyCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_startPos = pTouch->getLocation();
    if (!m_clickArea->isVisible() || !isTouchInside(m_clickArea, pTouch)) {
        return false;
    }
    return true;
}

void DailyCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
     cellTouchEnded(pTouch);
}

void DailyCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void DailyCell::cellTouchEnded(CCTouch* pTouch)
{
    CCPoint pos  = pTouch->getLocation();
    CCNode* node = this->getParent();
    if (isTouchInside(m_touchNode, pTouch) && fabs(pos.y - m_startPos.y) <=30 && node && node->isVisible()) {
        int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
        int dy = pTouch->getLocation().y - pTouch->getStartLocation().y;
        if (fabs(dx) > 10 || fabs(dy) > 10) {
            return;
        } else {
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            if(PortActController::getInstance()->m_loginDayMap.find(m_itemId) == PortActController::getInstance()->m_loginDayMap.end()){
                return;
            }
            if(m_rdNode->isVisible())
            {
                auto dict = CCDictionary::create();
                auto itemEffectObj = CCDictionary::create();
                CCArray* tmpArr = PortActController::getInstance()->m_loginDayMap[m_itemId].reward;
                if(tmpArr == NULL){
                    return;
                }
                auto newArr = CCArray::createWithArray(tmpArr);
                
                itemEffectObj->setObject(newArr, "reward");
                auto tmparray = CCArray::create();
                dict->setObject(itemEffectObj, "itemEffectObj");
                dict->setObject(CCString::create(CC_ITOA(99999)), "day");
                PopupViewController::getInstance()->addPopupView(ChestRDView::create(dict),false,false);
            }
            else
            {
                bool st = false;
                
                if (PortActController::getInstance()->m_isRdLoginDay == 0) {
                    if (PortActController::getInstance()->m_isVipRdLoginDay == 0) {
                        if (m_itemId == (PortActController::getInstance()->m_loginDay + 1)) {
                            st = true;
                        }
                    } else {
                        if (m_itemId == PortActController::getInstance()->m_loginDay) {
                            st = true;
                        }
                    }
                }
                PopupViewController::getInstance()->addPopupView(DailyRwdPop::create(m_itemId));
                
//            if (st) {
//                PortActController::getInstance()->startGetCheckInRwd(0);
//            } else {
//                PopupViewController::getInstance()->addPopupView(DailyRwdPop::create(m_itemId));
//            }
                
            }
        }
    }
}

SEL_CCControlHandler DailyCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

void DailyCell::refreshRd(CCObject* params)
{
    if(params) {
        int itemId = dynamic_cast<CCInteger*>(params)->getValue();
        if (itemId != m_itemId) {
            
        } else {
            if (m_bgSpr) {
                CCCommonUtils::setSpriteGray(m_bgSpr, true);
                CCCommonUtils::setSpriteGray(m_itemBG, true);

            }
            if (m_picSpr) {
                CCCommonUtils::setSpriteGray(m_picSpr, true);
            }
            if (m_rdNode->isVisible()) {
                onRefreshBaoXiang(true);
            }
            
            m_dayLabel->setColor(ccWHITE);
            
            m_flashSpr->setVisible(true);
            m_particleNode->setVisible(false);
        }
    }
}

void DailyCell::onRefreshBaoXiang(bool st)
{
    if (m_spr1->isVisible()) {
        CCCommonUtils::setSpriteGray(m_spr1, st);
    }
    if (m_spr2->isVisible()) {
        CCCommonUtils::setSpriteGray(m_spr2, st);
    }
    if (m_spr3->isVisible()) {
        CCCommonUtils::setSpriteGray(m_spr3, st);
    }
    CCCommonUtils::setSpriteGray(m_wenhao, st);
    CCCommonUtils::setSpriteGray(m_itemBG, st);//fusheng add
    if(st)
        m_dayLabel->setColor(ccWHITE);
    
}

bool DailyCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numBG", CCSprite*, m_numBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemBG", CCSprite*, m_itemBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dayLabel", CCLabelIF*, m_dayLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flashSpr", CCSprite*, m_flashSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rdNode", CCNode*, m_rdNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spr1", CCSprite*, m_spr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spr2", CCSprite*, m_spr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spr3", CCSprite*, m_spr3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wenhao", CCSprite*, m_wenhao);
    return false;
}

///pop
DailyRwdPop* DailyRwdPop::create(int itemId)
{
    auto ret = new DailyRwdPop();
    if (ret && ret->init(itemId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DailyRwdPop::init(int itemId)
{
    bool ret = false;
    if (!PopupBaseView::init()) {
        return ret;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    CCBLoadFile("qiandaoTips", this, this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    
    m_itemId = itemId;
    if(PortActController::getInstance()->m_loginDayMap.find(m_itemId) == PortActController::getInstance()->m_loginDayMap.end()){
        return false;
    }
    auto& arr = PortActController::getInstance()->m_loginDayMap[m_itemId].reward;
    //数组只有一个元素  即只有一种奖励
    CCDictionary* item = _dict(arr->objectAtIndex(0));
    int type = item->valueForKey("type")->intValue();
    string pic = "";
    string name = "";
    string des = "";
    string bg = "kuang";
    int num = 0;
    if (type == R_GOODS) {
        CCDictionary* value = _dict(item->objectForKey("value"));
        num = value->valueForKey("num")->intValue();
        string toolId = value->valueForKey("id")->getCString();
        pic = CCCommonUtils::getIcon(toolId);
        name = CCCommonUtils::getNameById(toolId);
        des = (ToolController::getInstance()->getToolInfoById(atoi(toolId.c_str()))).des;
    } else {
        pic = RewardController::getInstance()->getPicByType(type, 0);
        name = RewardController::getInstance()->getNameByType(type, 0);
        num = item->valueForKey("num")->intValue();
    }
    int multiple = PortActController::getInstance()->m_loginDayMap[m_itemId].multiple;
    bg += CC_ITOA(multiple);
    bg += ".png";
    auto bgSpr = CCLoadSprite::createSprite(bg.c_str());
    auto picSpr = CCLoadSprite::createSprite(pic.c_str());
    CCCommonUtils::setSpriteMaxSize(picSpr, 90, true);
    m_bgNode->addChild(bgSpr);
    m_picNode->addChild(picSpr);
    if(num>1){
        name.append("   X");
        name.append(CC_ITOA(num));
    }
    m_nameLabel->setString(name);
    m_desLabel->setString(_lang(des));
    
    ret = true;
    return ret;
}

void DailyRwdPop::onEnter()
{
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void DailyRwdPop::onExit()
{
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool DailyRwdPop::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void DailyRwdPop::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler DailyRwdPop::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool DailyRwdPop::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    return false;
}

//DailyFriendCell
DailyFriendCell* DailyFriendCell::create(FBFriendInfo* info,CCNode* clickNode)
{
    auto ret = new DailyFriendCell(info,clickNode);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DailyFriendCell::init()
{
    CCLoadSprite::doResourceByCommonIndex(11, true);
    auto node = CCBLoadFile("DailyFriendCell",this,this);
    setContentSize(node->getContentSize());
    m_nameTxt->setString(m_info->name.c_str());
    m_selectBtn->setSelected(m_info->isCheck);
    m_selectSpr->setVisible(m_selectBtn->isSelected());
    return true;
}

void DailyFriendCell::onEnter() {
    CCNode::onEnter();
}

void DailyFriendCell::onExit() {
    CCNode::onExit();
}

void DailyFriendCell::onSelectClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    if (m_clickNode==NULL || !m_clickNode->isVisible()) {
        return ;
    }
    m_selectBtn->setSelected(!m_selectBtn->isSelected());
    m_selectSpr->setVisible(m_selectBtn->isSelected());
    if(m_selectSpr->isVisible()){
        m_info->isCheck = true;
    }else{
        m_info->isCheck = false;
    }
}

SEL_CCControlHandler DailyFriendCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSelectClick", DailyFriendCell::onSelectClick);
    return NULL;
}

bool DailyFriendCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectBtn", CCControlButton*, m_selectBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCSprite*, m_selectSpr);
    return false;
}


