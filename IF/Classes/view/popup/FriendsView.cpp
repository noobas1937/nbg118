//
//  FriendsView.cpp
//  IF
//
//  Created by xxrdsg on 15-9-29.
//
//

#include "FriendsView.h"
#include "FriendsController.h"
#include "PopupViewController.h"
#include "MailController.h"
#include "MailWritePopUpView.h"
#include "UIComponent.h"
#include "FriendSearch.h"
#include "FBUtilies.h"
#include "RoleInfoView.h"
//#include "ThreeDTouchController.h" simon
#include "ChatServiceCocos2dx.h"
bool sortByOnlineAndName(const string& s1, const string& s2)
{
    CCDictionary* dic = FriendsController::getInstance()->m_data;
    string temp1 = s1;
    string temp2 = s2;
    if(dic == NULL || temp1=="" || temp2 == ""){
        
        return false;
    }
    if (s1.empty() || s2.empty() || !dic->objectForKey(s1) || !dic->objectForKey(s2)) {
        return true;
    }
    if(dic->objectForKey(s1) && dic->objectForKey(s2)){
        return false;
    }
    auto info1 = dynamic_cast<FriendInfo*>(dic->objectForKey(s1));
    auto info2 = dynamic_cast<FriendInfo*>(dic->objectForKey(s2));
    if(info1 == nullptr || info2 == nullptr){
        return false;
    }
    int info1kingnum = info1->serverId;
    int info2kingnum = info2->serverId;
    if (info1kingnum > info2kingnum) {
        return false;
    }else{
        return true;
    }
//    if (info1->online == 0 && info2->online > 0) {
//        return false;
//    } else if (info1->online > 0 && info2->online == 0) {
//        return true;
//    } else {
//        string str1(""), str2("");
//        if (info1->description.empty()) {
//            str1 = info1->name;
//        } else {
//            str1 = info1->description;
//        }
//        if (info2->description.empty()) {
//            str2 = info2->name;
//        } else {
//            str2 = info2->description;
//        }
//        if (strcmp(str1.c_str(), str2.c_str()) <= 0) {
//            return true;
//        } else {
//            return false;
//        }
//    }
}

void FriendsView::scrollViewDidScroll(CCScrollView* view)
{
    float mindy = m_tableView->minContainerOffset().y ;
//    float maxdy = m_tableView->maxContainerOffset().y ;
    float dy = m_tableView->getContentOffset().y;
    if (dy < mindy)
    {
        m_tableView->setContentOffset(ccp(0, mindy));
    }
//    if (dy > maxdy) {
//        m_tableView->setContentOffset(ccp(0, maxdy));
//    }
}

#pragma mark tableView
void FriendsView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}
cocos2d::CCSize FriendsView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (idx > m_data.size())
    {
        return CCSizeZero;
    }
    vector<string> vec;
    CCCommonUtils::splitString(m_data.at(idx), ";", vec);
    if (vec.size() > 1) {
        return CCSize(640, 50);
    } else {
        return CCSize(640, 138);
    }
}

cocos2d::CCSize FriendsView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 138);
}
CCTableViewCell* FriendsView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if (idx >= m_data.size())
    {
        return nullptr;
    }
    FriendsCell* cell = (FriendsCell*)table->dequeueCell();
    int onlineNum = 0, totalNum = 0;
    
    int type = 0;
    string dataUid = m_data.at(idx);
    if (FriendsController::getInstance()->m_data->objectForKey(dataUid)==NULL && FriendsController::getInstance()->m_fbData->objectForKey(dataUid)) {
        type=1;
    }
    if (cell) {
        cell->setData(dataUid,type);
    } else {
        cell = FriendsCell::create(dataUid, m_listNode,type);
    }
    return cell;
}
ssize_t FriendsView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data.size();
}

bool FriendsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_searchNode", CCNode*, this->m_searchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_searchBtn", CCControlButton*, this->m_searchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label1", CCLabelIF*, this->m_label1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label2", CCLabelIF*, this->m_label2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label3", CCLabelIF*, this->m_label3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode1", CCNode*, this->m_touchNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode2", CCNode*, this->m_touchNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode3", CCNode*, this->m_touchNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipLabel", CCLabelIF*, this->m_tipLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addFriendBtn", CCControlButton*, this->m_addFriendBtn);
    return false;
}

SEL_CCControlHandler FriendsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSearchBtn", FriendsView::onClickSearchBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddFriendBtnClick", FriendsView::onAddFriendBtnClick);
    return nullptr;
}

bool FriendsView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
void FriendsView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    float dis = ccpDistance(pTouch->getLocation(), pTouch->getStartLocation());
    if (dis < 10) {
        if (isTouchInside(m_touchNode1, pTouch)) {
//            goToGroup(FRIEND_AUDITING);
        } else if (isTouchInside(m_touchNode2, pTouch)) {
//            goToGroup(FRIEND_FRIEND);
        } else if (isTouchInside(m_touchNode3, pTouch)) {
//            goToGroup(FRIEND_FAVO);
        }
    }
}
void FriendsView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void FriendsView::onClickSearchBtn(CCObject *pSender, CCControlEvent event)
{
    
}

void FriendsView::onAddFriendBtnClick(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    PopupViewController::getInstance()->addPopupInView(FriendSearchView::create());
}

FriendsView* FriendsView::create()
{
    auto ret = new FriendsView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
//FRIEND_NONE, //啥都不是 0
//FRIEND_FACEBOOK //facebook好友 1
//FRIEND_FRIEND, //好友关系 2
//FRIEND_FAVO, //星标好友（单向）3
//FRIEND_APPLYING, //申请中 4 我请求加他为好友
//FRIEND_AUDITING, //审核申请中 5 请求加我为好友
bool FriendsView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_numMap.clear();
    m_data.clear();
    m_bOpenMap.clear();
    m_bOpenMap[FRIEND_FACEBOOK] = CCUserDefault::sharedUserDefault()->getBoolForKey("FriendsViewCellOpen1",false);
    m_bOpenMap[FRIEND_FRIEND] = CCUserDefault::sharedUserDefault()->getBoolForKey("FriendsViewCellOpen2",true);
    m_bOpenMap[FRIEND_FAVO] = CCUserDefault::sharedUserDefault()->getBoolForKey("FriendsViewCellOpen3",true);
    m_bOpenMap[FRIEND_AUDITING] = CCUserDefault::sharedUserDefault()->getBoolForKey("FriendsViewCellOpen5",true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    FriendsController::getInstance()->m_isHasFriendsViewPop = true;
    setCleanFunction([](){
        FriendsController::getInstance()->m_isHasFriendsViewPop = false;
        CCLoadSprite::doResourceByCommonIndex(6, false);
    });
    
    auto ccb = CCBLoadFile("FriendsListView", this, this);
    this->setContentSize(ccb->getContentSize());
    auto size = CCDirector::sharedDirector()->getWinSize();
    int add = size.height - ccb->getContentSize().height;
    if (CCCommonUtils::isIosAndroidPad())
    {
        add = add / 2.4;
    }
    m_bgNode->setPositionY(m_bgNode->getPositionY() - add);
    m_listNode->setPositionY(m_listNode->getPositionY() - add);
    m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + add));
    auto frame = CCLoadSprite::loadResource("FriendsList-bg.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(frame->getTexture());
    int height = 0;
    float theight = size.height;
    if (CCCommonUtils::isIosAndroidPad()) {
        theight = theight / 2.4;
    }
    while (height < theight)
    {
        CCSprite* spr = CCLoadSprite::createSprite("FriendsList-bg.png");
        spr->setAnchorPoint(ccp(1, 0));
        spr->setScaleX(1.04);
        spr->setPositionX(0);
        spr->setPositionY(height);
        tBatchNode->addChild(spr);
        
        spr = CCLoadSprite::createSprite("FriendsList-bg.png");
        spr->setAnchorPoint(ccp(0, 0));
        spr->setFlipX(true);
        spr->setScaleX(1.04);
        spr->setPositionY(height);
        spr->setPositionX(0);
        tBatchNode->addChild(spr);
        
        height += spr->getContentSize().height * spr->getScaleY() - 3;
    }
    m_bgNode->addChild(tBatchNode);
    
    m_label1->setString(_lang(""));
    m_label2->setString(_lang(""));
    m_label3->setString(_lang(""));
    
    m_tableView = CCTableView::create(this, m_listNode->getContentSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setDelegate(this);
    m_tableView->setTouchPriority(1);
    m_tableView->setPositionX(0);
    m_listNode->addChild(m_tableView);
    //获取Facebook好友
    getFBFriendList();
    //关闭搜索按钮
    m_addFriendBtn->setVisible(false);
    return true;
}
void FriendsView::onEnter()
{
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetTitleTouchMsg), FRIEND_TITLE_TOUCHED, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetMsgDeleteFriend), DELETE_FRIEND_SUC, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetMsgRejectApply), REJECT_APPLY_SUC, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetMsgAcceptApply), ACCEPT_FRIEND_APPLY_SUC, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetMsgFavoFriend), UP_FRIEND_STAR_SUC, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetMsgCacelFavaoFriend), DOWN_FRIEND_NORMAL_SUC, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetMsgChangeDesc), FRIEND_DESC_CHANGE_SUC, nullptr);
    
    //push
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetMsgAcceptedPush), RECEIVE_MY_APPLY_ACCEPTED_PUSH, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetMsgApplyMePush), RECEIVE_APPLY_PUSH, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::onGetMsgDeletedPush), RECEIVE_ME_DELETED_PUSH, nullptr);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::afterGetFriendsInfo), MSG_FBFriendsList, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::getFriendLastTalk), "FriendsLastMailInfoUpdeta", NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsView::RefreshFriendLastTalk), "FriendsViewBackFromMail", NULL);
    
    FriendsController::getInstance()->resetLocalSeenApply();
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    m_titleTxt->setString(_lang("132103").c_str());//好友列表
    setTitleName(_lang("132103"));
    refreshView();
    //得到聊天最后一句话
    RefreshFriendLastTalk(NULL);
}
void FriendsView::onExit()
{
    CCUserDefault::sharedUserDefault()->setBoolForKey("FriendsViewCellOpen1", m_bOpenMap[FRIEND_FACEBOOK]);
    CCUserDefault::sharedUserDefault()->setBoolForKey("FriendsViewCellOpen2", m_bOpenMap[FRIEND_FRIEND]);
    CCUserDefault::sharedUserDefault()->setBoolForKey("FriendsViewCellOpen3", m_bOpenMap[FRIEND_FAVO]);
    CCUserDefault::sharedUserDefault()->setBoolForKey("FriendsViewCellOpen5", m_bOpenMap[FRIEND_AUDITING]);
    CCUserDefault::sharedUserDefault()->flush();
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "FriendsViewBackFromMail");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "FriendsLastMailInfoUpdeta");
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFriendsList);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FRIEND_TITLE_TOUCHED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DELETE_FRIEND_SUC);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, REJECT_APPLY_SUC);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ACCEPT_FRIEND_APPLY_SUC);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, UP_FRIEND_STAR_SUC);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DOWN_FRIEND_NORMAL_SUC);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FRIEND_DESC_CHANGE_SUC);
    
    //PUSH
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, RECEIVE_MY_APPLY_ACCEPTED_PUSH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, RECEIVE_APPLY_PUSH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, RECEIVE_ME_DELETED_PUSH);
    setTouchEnabled(false);
    CCNode::onExit();
}

void FriendsView::onGetTitleTouchMsg(cocos2d::CCObject *obj)
{
    CCString* cs = dynamic_cast<CCString*>(obj);
    int type = cs->intValue();
    m_bOpenMap[type] = !m_bOpenMap[type];
    refreshView();
    //得到聊天最后一句话
    RefreshFriendLastTalk(NULL);
}
void FriendsView::onGetMsgDeleteFriend(CCObject* obj)
{
    refreshView();
}

void FriendsView::onGetMsgRejectApply(CCObject* obj)
{
    refreshView();
}

void FriendsView::onGetMsgAcceptApply(CCObject* obj)
{
    refreshView();
}

void FriendsView::onGetMsgFavoFriend(CCObject* obj)
{
    refreshView();
}

void FriendsView::onGetMsgCacelFavaoFriend(CCObject* obj)
{
    refreshView();
}

void FriendsView::onGetMsgChangeDesc(CCObject* obj)
{
    refreshView();
}

void FriendsView::onGetMsgDeletedPush(CCObject* obj)
{
    refreshView();
}

void FriendsView::onGetMsgAcceptedPush(CCObject* obj)
{
    refreshView();
}

void FriendsView::onGetMsgApplyMePush(CCObject* obj)
{
    refreshView();
}

void FriendsView::goToGroup(int type)
{
    if (type != FRIEND_FRIEND && type != FRIEND_FAVO && type != FRIEND_AUDITING) {
        return;
    }
    int locaInAll = 0;
    int locaInTitle = 0;
    int titleCnt = 0;
    string tStr(CC_ITOA(type));
    tStr.append(";");
    int totalCnt = m_data.size();
    for (int i = 0; i < totalCnt; ++i) {
        if (string::npos != m_data[i].find(";"))
        {
            titleCnt++;
            if (0 == m_data[i].find(tStr))
            {
                locaInTitle = titleCnt;
                locaInAll = i + 1;
            }
        }
    }
    if (totalCnt > 0 && locaInTitle > 0) {
        float viewSizeHeight = m_tableView->getViewSize().height;
        float allLen = m_tableView->getContainer()->getContentSize().height;
        float toShowLength = (titleCnt - locaInTitle + 1) * 50 + 138 * (totalCnt - locaInAll - (titleCnt - locaInTitle));
        if (toShowLength < viewSizeHeight)
        {
            if (allLen > viewSizeHeight) {
                m_tableView->setContentOffset(CCPoint(0, 0));
            }
        } else {
            m_tableView->setContentOffset(CCPoint(0, viewSizeHeight - toShowLength));
        }
    }
}

void FriendsView::refreshView()
{
    refreshData();
    
    float dy1 = m_tableView->getContentOffset().y;
    float minDy1 = m_tableView->minContainerOffset().y;
    m_tableView->reloadData();
    float minDy2 = m_tableView->minContainerOffset().y;
    float addDy = minDy2 - minDy1;
    m_tableView->setContentOffset(CCPoint(0, dy1 + addDy));
    
    int cnt = 0;
    for (auto it = m_numMap.begin(); it != m_numMap.end(); ++it) {
        if (it->first == FRIEND_FRIEND || it->first == FRIEND_FAVO || it->first == FRIEND_AUDITING || it->first == FRIEND_FACEBOOK) {
            cnt += it->second.second;
        }
    }
    if (cnt <= 0) {
        m_tipLabel->setString(_lang("132116"));
    } else {
        m_tipLabel->setString("");
    }
}
void FriendsView::refreshData()
{
//    FriendsController::getInstance()->getNumByGroup(m_numMap);
    
    m_data.clear();
    m_numMap.clear();
    map<int, vector<string> > tmpMap;
    CCDictionary* dic = FriendsController::getInstance()->m_data;
    CCDictElement* ele = nullptr;
    FriendInfo* info = nullptr;
    CCDICT_FOREACH(dic, ele)
    {
        info = dynamic_cast<FriendInfo*>(ele->getObject());
        if (info) {
            if (!info->uid.empty()) {
                if (info->relation == FRIEND_FRIEND|| info->relation == FRIEND_FAVO || info->relation == FRIEND_AUDITING || info->relation == FRIEND_FACEBOOK) {
                    tmpMap[info->relation].push_back(info->uid);
                    m_numMap[info->relation].second++;
                    if (info->online > 0) {
                        m_numMap[info->relation].first++;
                    }
                }
            }
        }
    }
    if (tmpMap.find(FRIEND_FRIEND) == tmpMap.end())
    {
        tmpMap.insert(make_pair(FRIEND_FRIEND, vector<string>()));
    }
    if (tmpMap.find(FRIEND_AUDITING) == tmpMap.end())
    {
        tmpMap.insert(make_pair(FRIEND_AUDITING, vector<string>()));
    }
    for (auto it = tmpMap.begin(); it != tmpMap.end(); ++it) {
        string a = CC_ITOA(it->first);
        a.append(";").append(CC_ITOA(m_bOpenMap[it->first]));
        int onlineNum = 0, totalNum = 0;
        if (m_numMap.find(it->first) != m_numMap.end())
        {
            onlineNum = m_numMap[it->first].first;
            totalNum = m_numMap[it->first].second;
        }
        a.append(";").append(CC_ITOA(onlineNum)).append(";").append(CC_ITOA(totalNum));
        m_data.push_back(a);
        
        if(m_bOpenMap.find(it->first) != m_bOpenMap.end()){
            if (m_bOpenMap[it->first]) {
                if(it->second.size()>0){
                    vector<string> vec = it->second;
                    std::stable_sort(it->second.begin(), it->second.end(), sortByOnlineAndName);
                }else{
                    CCLog("dddd 0");
                }
    
                for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1) {
                    m_data.push_back(*it1);
                }
            }
        }
    }
    //facebook
    map<int, vector<string> > tmpFbMap;
    CCDictionary* fbDic = FriendsController::getInstance()->m_fbData;
    CCDictElement* fbEle = nullptr;
    FriendInfo* fbInfo = nullptr;
    CCDICT_FOREACH(fbDic, fbEle)
    {
        fbInfo = dynamic_cast<FriendInfo*>(fbEle->getObject());
        if (fbInfo) {
            if (!fbInfo->uid.empty()) {
                bool hasIt = false;
                
                CCDictionary* dic = FriendsController::getInstance()->m_data;
                CCDictElement* ele = nullptr;
                FriendInfo* info = nullptr;
                CCDICT_FOREACH(dic, ele)
                {
                    info = dynamic_cast<FriendInfo*>(ele->getObject());
                    if (info->uid==fbInfo->uid) {
                        hasIt = true;
                        break;
                    }
                }
           
                if(fbInfo->fbUid != "" && hasIt==false){//facebook
//                if(fbInfo->relation == FRIEND_FACEBOOK && hasIt==false){//facebook
                    tmpFbMap[FRIEND_FACEBOOK].push_back(fbInfo->uid);
                    m_numMap[FRIEND_FACEBOOK].second++;
                    if (fbInfo->online > 0) {
                        m_numMap[FRIEND_FACEBOOK].first++;
                    }
                }
            }
        }
    }
    
    for (auto it = tmpFbMap.rbegin(); it != tmpFbMap.rend(); ++it) {
        string a = CC_ITOA(it->first);
        a.append(";").append(CC_ITOA(m_bOpenMap[it->first]));
        int onlineNum = 0, totalNum = 0;
        if (m_numMap.find(it->first) != m_numMap.end())
        {
            onlineNum = m_numMap[it->first].first;
            totalNum = m_numMap[it->first].second;
        }
        a.append(";").append(CC_ITOA(onlineNum)).append(";").append(CC_ITOA(totalNum));
        m_data.push_back(a);
        if (m_bOpenMap[it->first]) {
            for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1) {
                m_data.push_back(*it1);
            }
        }
    }
}

#pragma mark facebook
void FriendsView::getFBFriendList(){
//    FriendsController::getInstance()->fbInfo["1375926026033950"] = "00";
//    FriendsController::getInstance()->startGetFbFriends("1375926026033950");
    if(FriendsController::getInstance()->m_fbData->count()==0 && FriendsController::getInstance()->isFaceBookOpen()){
        if (FBUtilies::fbIsLogin()){
//            GameController::getInstance()->showWaitInterface();
            FBUtilies::getFBFriendList();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            this->unschedule(schedule_selector(FriendsView::checkFriendsData));
            this->schedule(schedule_selector(FriendsView::checkFriendsData),1);
            this->checkFriendsData(0);
#endif
        }
    }
}
void FriendsView::afterGetFriendsInfo(CCObject* param){
//    GameController::getInstance()->removeWaitInterface();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCArray* friends = dynamic_cast<CCArray*>(param);
    if (friends) {
        string uids="";
        int num = friends->count();
        for (int i=0; i<num; i++) {
            auto dic = _dict(friends->objectAtIndex(i));
            if(dic){
                dic->setObject(CCString::create("1"), "flag");
                string idStr = dic->valueForKey("id")->getCString();
                string nameStr = dic->valueForKey("name")->getCString();
                FriendsController::getInstance()->fbInfo[idStr] = nameStr;
                if(!FriendsController::getInstance()->isfbUidAddToFamily(idStr)){
                    uids+=idStr;
                    if (i<num-1) {
                        uids+="|";
                    }
                }

            }
        }
        FriendsController::getInstance()->startGetFbFriends(uids);
        //test
//        FriendsController::getInstance()->fbInfo["10201496460731763"] = "nameStr";
//        FriendsController::getInstance()->startGetFbFriends("10201496460731763");//"10201496460731763"
    }
#endif
}

void FriendsView::checkFriendsData(float _time){
    GlobalData::shared()->installFriendsInfo = CCUserDefault::sharedUserDefault()->getStringForKey("installFriendsInfo", "");
    if(GlobalData::shared()->installFriendsInfo!=""){
        this->unschedule(schedule_selector(FriendsView::checkFriendsData));
        string uids="";
        Json* fjson = Json_create(GlobalData::shared()->installFriendsInfo.c_str());
        if(fjson){
            int size = Json_getSize(fjson);
            for (int i=0; i<size; i++) {
                Json *item = Json_getItemAt(fjson, i);
                string nameStr = Json_getString(item,"name","");
                string idStr = Json_getString(item,"id","");
                FriendsController::getInstance()->fbInfo[idStr] = nameStr;
//                uids+=idStr;
                if(!FriendsController::getInstance()->isfbUidAddToFamily(idStr)){
                    uids+=idStr;
                    if (i<size-1) {
                        uids+="|";
                    }
                }
            }
            Json_dispose(fjson);
            FriendsController::getInstance()->startGetFbFriends(uids);
        }
    }
}

#pragma mark getFriendLastTalk
void FriendsView::getFriendLastTalk(CCObject* obj){
    refreshView();
}
void FriendsView::RefreshFriendLastTalk(CCObject* obj){
     MailController::getInstance()->getFriendMailByUids(m_data);
}

#pragma mark FriendsCell
FriendsCell* FriendsCell::create(string uid, CCNode* touchNode, int type)
{
    auto ret = new FriendsCell(uid, touchNode, type);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FriendsCell::init()
{
    CCLoadSprite::doResourceByCommonIndex(6, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
    });
    CCBLoadFile("FriendsListCell", this, this);
    CCCommonUtils::setButtonTitle(m_acceptBtn, _lang("132112").c_str());
    CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("132113").c_str());
    setData(m_uid,cellType);
    return true;
}

void FriendsCell::setData(string uid, int type)
{
    m_infoNode->setVisible(false);
    m_titleNode->setVisible(false);
    m_numLabel->setString("");
    m_applyBtn->setVisible(false);
    m_facebook_icon->setVisible(false);
    m_moreLabel->setVisible(false);
    m_uid = uid;
    cellType = type;
    bool bOpen = true;
    bool bTitle = true;
    vector<string> tv;
    CCCommonUtils::splitString(uid, ";", tv);
    if (tv.size() >1)
    {
        m_uid = tv.at(0);
        bOpen = atoi(tv.at(1).c_str());
        bTitle = true;
    } else {
        bTitle = false;
    }
    if (bTitle)
    {
        this->setContentSize(CCSize(640, 50));
        m_titleNode->setVisible(true);
        m_infoNode->setVisible(false);
        int titleType = atoi(m_uid.c_str());
        if (titleType == FRIEND_FRIEND) {
            m_groupLabel->setString(_lang("132106"));//好友
        } else if (titleType == FRIEND_FAVO) {
            m_groupLabel->setString(_lang("132105"));//星标好友
        } else if (titleType == FRIEND_AUDITING) {
            m_groupLabel->setString(_lang("132104"));//好友申请
        } else if (titleType == FRIEND_FACEBOOK) {
            m_groupLabel->setString(_lang("105786"));//facebook好友
        }
        if (bOpen) {
            m_arrowSpr->setRotation(0);
        } else {
            m_arrowSpr->setRotation(-90);
        }
        int onlineNum = 0;
        int totalNum = 0;
        if (tv.size() == 4)
        {
            onlineNum = atoi(tv.at(2).c_str());
            totalNum = atoi(tv.at(3).c_str());
        }
        string str("");
        str/*.append(CC_CMDITOA(onlineNum)).append("/")*/.append(CC_CMDITOA(totalNum));
//        if (titleType == FRIEND_AUDITING)
//        {
//            str = "";
//        }
        m_numLabel->setString(str);
        if (m_groupLabel->getContentSize().width * m_groupLabel->getOriginScaleX() + m_numLabel->getContentSize().width * m_numLabel->getOriginScaleX() > abs(m_numLabel->getPositionX() - m_groupLabel->getPositionX()))
        {
            m_groupLabel->setDimensions(CCSize(abs(m_numLabel->getPositionX() - m_groupLabel->getPositionX()) - m_numLabel->getContentSize().width * m_numLabel->getOriginScaleX(), 0));
        } else {
            m_groupLabel->setDimensions(CCSizeZero);
        }
        
        m_dise->removeFromParent();
        m_titleNode->addChild(m_dise, -1);
        
        m_arrowSpr->removeFromParent();
        m_titleNode->addChild(m_arrowSpr);
        
    }
    else
    {
        this->setContentSize(CCSize(640, 138));
        m_titleNode->setVisible(false);
        m_infoNode->setVisible(true);
        m_iconNode->removeAllChildrenWithCleanup(true);
        CCDictionary* dic = FriendsController::getInstance()->m_data;
        if (cellType==1) {
            dic = FriendsController::getInstance()->m_fbData;
        }
        if (!m_uid.empty())
        {
            if (dic->objectForKey(m_uid)) {
                FriendInfo* info = dynamic_cast<FriendInfo*>(dic->objectForKey(m_uid));
                string name("");
//                if (info->relation==FRIEND_FACEBOOK && info->fbUid!="" &&
//                    FriendsController::getInstance()->fbInfo.find(info->fbUid)!=FriendsController::getInstance()->fbInfo.end()) {
                if (info->fbUid!="" &&
                    FriendsController::getInstance()->fbInfo.find(info->fbUid)!=FriendsController::getInstance()->fbInfo.end()) {
                    string fbName = "";
                    if (!info->abbr.empty())//allianceId
                    {
                        name.append("(").append(info->abbr).append(")");
                    }
                    name.append(info->name).append(" #").append(CC_ITOA(info->serverId));
                    m_fbNameLabel->setString(name);
                    m_fbNameLabel->setMaxScaleXByWidth(500);//防爆框
                    string fbNameStr = FriendsController::getInstance()->fbInfo[info->fbUid];
                    m_nameLabel->setString(fbNameStr.c_str());
                    m_nameLabel->setFontSize(22);
                    m_nameLabel->setPositionX(m_facebook_icon->getPositionX() + 5);
                    m_facebook_icon->setVisible(true);
                }else{
                    m_fbNameLabel->setString("");
                    m_nameLabel->setFontSize(26);
                    m_nameLabel->setPositionX(m_facebook_icon->getPositionX() - 30);
                    if (info->description.empty())
                    {
                        if (!info->abbr.empty())//allianceId
                        {
                            name.append("(").append(info->abbr).append(")");
                        }
                        name.append(info->name).append(" #").append(CC_ITOA(info->serverId));
                        m_nameLabel->setString(name.c_str());
                    } else {
                        m_nameLabel->setString(info->description.c_str());
                    }
                }
                //m_nameLabel->setMaxScaleXByWidth(500);//防爆框
                if (info->mainBuildingLevel>0 && info->mainBuildingLevel<=100) {
                    m_levelTxt->setString(_lang_1("115515", CC_ITOA(info->mainBuildingLevel)));//115515=城堡等级{0}级
                }
//                m_onlineLabel->setString("中文中文中文中文中文中文中文中文中文中文中文中文中中文中文中文中文中文中文中文中中文中文中文中文中文中文中文中");
//                int width = m_onlineLabel->getContentSize().width;
//                m_onlineLabel->setWidth(400);
//                if (width >= 400) {
//                    m_moreLabel->setString("...");
//                    m_moreLabel->setVisible(true);
//                }else{
//                    m_moreLabel->setVisible(false);
//                }
//                m_onlineLabel->setLineBreakWithoutSpace(false);
//                m_onlineLabel->setFontSize(20);
//                m_onlineLabel->setColor({168,151,127});

                //聊天最后一句话
                m_moreLabel->setVisible(false);
                if(FriendsController::getInstance()->lastMailMap.find(m_uid) != FriendsController::getInstance()->lastMailMap.end()){
                    m_onlineLabel->setString(FriendsController::getInstance()->lastMailMap[m_uid].c_str());
                    int width = m_onlineLabel->getContentSize().width;
                    m_onlineLabel->setWidth(400);
                    if (width > 400) {
                        m_moreLabel->setString("...");
                        m_moreLabel->setVisible(true);
                    }else{
                        m_moreLabel->setVisible(false);
                    }
                    m_onlineLabel->setLineBreakWithoutSpace(false);
                    m_onlineLabel->setFontSize(20);
                    m_onlineLabel->setColor({168,151,127});
                }
//                if (info->online > 0) {
//                    m_onlineLabel->setString(_lang("115242"));
//                    m_onlineLabel->setColor({149, 184, 103});
//                } else {
//                    {
//                        auto dt = GlobalData::shared()->getWorldTime()-GlobalData::shared()->changeTime(info->offLineTime/1000);
//                        string timestr = "";
//                        int timedt = 0;
//                        if(dt>=24*60*60){
//                            timedt =dt/(24*60*60);
//                            timestr = CC_ITOA(timedt);
//                            timestr.append(_lang("105592"));
//                        }else if(dt>=60*60){
//                            timedt =dt/(60*60);
//                            timestr = CC_ITOA(timedt);
//                            timestr.append(_lang("105591"));
//                        }else if(dt>=60){
//                            timedt =dt/60;
//                            timestr = CC_ITOA(timedt);
//                            timestr.append(_lang("105590"));
//                        }else{
//                            timestr ="1";
//                            timestr.append(_lang("105590"));
//                        }
//                        timestr.append(" ");
//                        timestr.append(_lang("105593"));
//                        m_onlineLabel->setString(timestr);
//                    }
//                    m_onlineLabel->setColor({156, 156, 156});
//                }
//                if (info->relation == 4) {
//                    m_onlineLabel->setString("");
//                }
                m_iconNode->removeAllChildrenWithCleanup(true);
                string picPath = info->pic;
                if (picPath.empty()) {
                    picPath = "g044.png";
                } else {
                    picPath.append(".png");
                }
                auto icon = CCLoadSprite::createSprite(picPath.c_str(),true,CCLoadSpriteType_HEAD_ICON);
                CCCommonUtils::setSpriteMaxSize(icon, 104, true);
                m_iconNode->addChild(icon);
                if (CCCommonUtils::isUseCustomPic(info->picVer))
                {
                    m_headImgNode = HFHeadImgNode::create();
                    m_headImgNode->initHeadImgUrl2(m_iconNode, CCCommonUtils::getCustomPicUrl(info->uid, info->picVer), 1.0f, 104, true);
                }
                
                if (FriendsController::getInstance()->isApplyingMe(m_uid)) {
                    m_acceptBtn->setVisible(true);
                    m_cancelBtn->setVisible(true);
                    m_acceptBtn->setEnabled(true);
                    m_cancelBtn->setEnabled(true);
                } else {
                    m_acceptBtn->setVisible(false);
                    m_cancelBtn->setVisible(false);
                }
                if (info->relation==FRIEND_FACEBOOK) {
                    m_acceptBtn->setVisible(false);
                    m_cancelBtn->setVisible(false);
                    m_applyBtn->setVisible(false);
                    if (FriendsController::getInstance()->isBeingAppliedByMe(info->uid)) {
                        m_applyBtn->setEnabled(false);
                        CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132101").c_str());
                    } else if (FriendsController::getInstance()->isMyFriend(info->uid)) {
                        m_applyBtn->setEnabled(false);
                        CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132102").c_str());
                    } else {
                        m_applyBtn->setEnabled(true);
                        CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132100").c_str());
                    }
                }
                m_newIcon->setVisible(false);
                if(info->isNew){
                    m_newIcon->setVisible(true);
                    info->isNew=false;
                }
            }
        }
    }
    
    
}




bool FriendsCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_getTouchNode, pTouch)) {
        if (m_uid.length() == 1) {
            if (isTouchInside(m_touchNode2, pTouch))
            {
                return true;
            } else {
                return false;
            }
        } else {
            if (isTouchInside(m_touchNode1, pTouch)) {
                
    #if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS) and __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_9_0
                CCDictionary* dic = FriendsController::getInstance()->m_data;
                if (cellType==1) {
                    dic = FriendsController::getInstance()->m_fbData;
                }
                if (dic->objectForKey(m_uid)) {
                    if (isTouchInside(m_iconTouchNode, pTouch)) {
//                        GlobalData::shared()->peekPageType = peekType_playerInfo;
//                        ThreeDTouchController::getInstance()->sendGetUserInfoCommand(m_uid); simon
                    }
                }
   
    #endif

                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}
void FriendsCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_newIcon->setVisible(false);
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) < 10) {
        if (m_uid.length() == 1) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(FRIEND_TITLE_TOUCHED, CCString::create(m_uid));
        } else {
            CCDictionary* dic = FriendsController::getInstance()->m_data;
            if (cellType==1) {
                dic = FriendsController::getInstance()->m_fbData;
            }
            if (dic->objectForKey(m_uid)) {
                if (isTouchInside(m_iconTouchNode, pTouch)) {
//                    if (GlobalData::shared()->peekPageType == peekType_peek && GlobalData::shared()->peekPageType == popType_pop)
//                        return; simon
                    auto info = dynamic_cast<FriendInfo*>(dic->objectForKey(m_uid));
                    if (FriendsController::getInstance()->isMyFriend(m_uid)) {
                        PopupViewController::getInstance()->addPopupView(FriendPopupView::create(m_uid));
                    }else{
                        RoleInfoView::createInfoByUid(m_uid);
                    }
                }else{
                    CCLOGFUNC();
                    if(FriendsController::getInstance()->isMyFriend(m_uid)){
                        FriendInfo* info = nullptr;
                        CCDictionary* dic = FriendsController::getInstance()->m_data;
                        if (dic && !dic->objectForKey(m_uid)) {
                            dic = FriendsController::getInstance()->m_fbData;
                            if(!dic->objectForKey(m_uid)){
                                return;
                            }
                        }
                        info = dynamic_cast<FriendInfo*>(dic->objectForKey(m_uid));
                        if (!info) {
                            return;
                        }
//                        FriendInfo* info = dynamic_cast<FriendInfo*>(FriendsController::getInstance()->m_data->objectForKey(m_uid));
                        MailController::getInstance()->openMailDialogViewFirst(info->name, m_uid);
                    }
                }
            }
        }
    }
}
void FriendsCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

bool FriendsCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconTouchNode", CCNode*, this->m_iconTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode1", CCNode*, this->m_touchNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLabel", CCLabelIF*, this->m_vipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbNameLabel", CCLabelIF*, this->m_fbNameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIFTTF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreLabel", CCLabelIFTTF*, this->m_moreLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_facebook_icon", CCSprite*, this->m_facebook_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelTxt", CCLabelIF*, this->m_levelTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_onlineLabel", CCLabelIFTTF*, this->m_onlineLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_acceptBtn", CCControlButton*, this->m_acceptBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn", CCControlButton*, this->m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyBtn", CCControlButton*, this->m_applyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newIcon", CCSprite*, this->m_newIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, this->m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dise", CCScale9Sprite*, this->m_dise);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode2", CCNode*, this->m_touchNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSpr", CCSprite*, this->m_arrowSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_groupLabel", CCLabelIF*, this->m_groupLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    
    return false;
}

SEL_CCControlHandler FriendsCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAcceptBtn", FriendsCell::onClickAcceptBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCancelBtn", FriendsCell::onClickCancelBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickApplyBtn", FriendsCell::onClickApplyBtn);
    return nullptr;
}

void FriendsCell::onClickAcceptBtn(CCObject *pSender, CCControlEvent event)
{
    FriendsController::getInstance()->startAcceptApply(m_uid);
    m_acceptBtn->setEnabled(false);
}

void FriendsCell::onClickCancelBtn(CCObject *pSender, CCControlEvent event)
{
    FriendsController::getInstance()->startRejectApply(m_uid);
    m_cancelBtn->setEnabled(false);
}

void FriendsCell::onClickApplyBtn(CCObject *pSender, CCControlEvent event)
{
    CCLOGFUNC();
    m_applyBtn->setEnabled(false);
    CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132101").c_str());
    FriendsController::getInstance()->startApplyFriend(m_uid);
}

void FriendsCell::onGetMsgApplySuc(CCObject* obj)
{
    if (!obj) return;
    CCString* cs = dynamic_cast<CCString*>(obj);
    if (!cs) {
        return;
    }
    string uid = cs->getCString();
    if (strcmp(uid.c_str(), m_uid.c_str()) != 0) return;
    if (FriendsController::getInstance()->isMyFriend(uid)) {
        m_applyBtn->setEnabled(false);
        CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132102").c_str());//已是好友
    } else if (FriendsController::getInstance()->isBeingAppliedByMe(uid)) {
        m_applyBtn->setEnabled(false);
        CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132101").c_str());//已申请
        CCCommonUtils::flyHint("", "", _lang("132101"));
    }
}

void FriendsCell::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendsCell::onGetMsgApplySuc), APPLY_FRIEND_SEND_SUC, NULL);
}

void FriendsCell::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, APPLY_FRIEND_SEND_SUC);
    setTouchEnabled(false);
    CCNode::onExit();
}

#pragma mark FriendPopupView
FriendPopupView* FriendPopupView::create(string uid)
{
    auto ret = new FriendPopupView(uid);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FriendPopupView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(307, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(307, false);
        CCLoadSprite::doResourceByCommonIndex(6, false);
    });
    if (m_uid.empty()) {
        return false;
    }
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    CCBLoadFile("FriendPopUpView2", this, this);
    CCDictionary* dic = FriendsController::getInstance()->m_data;
    if (dic && !dic->objectForKey(m_uid)) {
        dic = FriendsController::getInstance()->m_fbData;
        if(!dic->objectForKey(m_uid)){
            return false;
        }
    }
    m_picNode->removeAllChildrenWithCleanup(true);
    FriendInfo* info = dynamic_cast<FriendInfo*>(dic->objectForKey(m_uid));
    if (info->relation != 1 && info->relation != 2 && info->relation != 5) {
        return false;
    }
    
    string path = info->pic;
    if (path.empty()) {
        path = "g044";
    }
    path.append("_middle.png");
    CCSprite* spr = CCLoadSprite::createSprite(path.c_str(),true,CCLoadSpriteType_HEAD_ICON_MIDDLE);
    m_picNode->addChild(spr);
    
    if (!info->description.empty()) {
        m_nameLabel1->setString(info->description);
    } else {
        m_nameLabel1->setString("");
    }
    string str("");
    if (!info->allianceId.empty() && !info->abbr.empty()) {
        str.append("(").append(info->abbr).append(")");
    }
    str.append(info->name).append(" #").append(CC_ITOA(info->serverId));
    m_nameLabel2->setString(str);
    
    {
        if (FriendsController::getInstance()->isMyPutongFriend(m_uid) || FriendsController::getInstance()->isMyPutongFBFriend(m_uid)) {
            m_iconNode1->addChild(CCLoadSprite::createSprite("FriendsList-bt-xingbiao.png"));
            m_label1->setString(_lang("132107"));
        } else {
            m_iconNode1->addChild(CCLoadSprite::createSprite("FriendsList-bt-xingbiao1.png"));
            m_label1->setString(_lang("132108"));
        }
        m_iconNode2->addChild(CCLoadSprite::createSprite("FriendsList-bt-xiugaibizhu.png"));
        m_label2->setString(_lang("132109"));
        m_iconNode3->addChild(CCLoadSprite::createSprite("tile_pop_icon2.png"));
        m_label3->setString(_lang("108721"));//领主详情
        auto spr = CCLoadSprite::createSprite("FriendsList-bt-zengsongliwu.png");
        if (spr) {
//            if (info->serverId != GlobalData::shared()->playerInfo.selfServerId && !GlobalData::shared()->isCrossServerSendGiftValid) {
//                CCCommonUtils::setSpriteGray(spr, true);
//            } simon
            m_iconNode4->addChild(spr);
        }
        m_label4->setString(_lang("101380"));
        m_iconNode5->addChild(CCLoadSprite::createSprite("FriendsList-bt-sanchuhaoyou.png"));
        m_label5->setString(_lang("132110"));
    }
    
    return true;
}

void FriendPopupView::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}

void FriendPopupView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool FriendPopupView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
void FriendPopupView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    FriendInfo* info = nullptr;
    CCDictionary* dic = FriendsController::getInstance()->m_data;
    if (dic && !dic->objectForKey(m_uid)) {
        dic = FriendsController::getInstance()->m_fbData;
        if(!dic->objectForKey(m_uid)){
            return;
        }
    }
    info = dynamic_cast<FriendInfo*>(dic->objectForKey(m_uid));
    if (!info) {
        return;
    }
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) < 10) {
        if (isTouchInside(m_nodeClick1, pTouch)) {
            if (FriendsController::getInstance()->isMyPutongFriend(m_uid)) {
                FriendsController::getInstance()->startUpToStarFriend(m_uid);
            } else if (FriendsController::getInstance()->isMyStarFriend(m_uid)){
                FriendsController::getInstance()->startDownToNormalFriend(m_uid);
            } else if(FriendsController::getInstance()->isMyPutongFBFriend(m_uid)){
                string fbUid = info->fbUid;
                string fbNameStr = FriendsController::getInstance()->fbInfo[info->fbUid];
                FriendsController::getInstance()->startFBUpToStarFriend(m_uid,fbUid,fbNameStr);
            }
            closeSelf();
        } else if (isTouchInside(m_nodeClick2, pTouch)) {
            retain();
            closeSelf();
            PopupViewController::getInstance()->addPopupView(FriendChangeDesView::create(m_uid));
            release();
        } else if (isTouchInside(m_nodeClick3, pTouch)) {
            retain();
            closeSelf();
//            MailController::getInstance()->openMailDialogViewFirst(info->name, m_uid);
            RoleInfoView::createInfoByUid(m_uid);
            release();
        } else if (isTouchInside(m_nodeClick4, pTouch)) {
//            if (info->serverId != GlobalData::shared()->playerInfo.selfServerId && !GlobalData::shared()->isCrossServerSendGiftValid) {
//                CCCommonUtils::flyHint("", "", _lang("101102"));
//                return;
//            } simon
            retain();
            closeSelf();
            auto view = MailWritePopUpView::createWithGift(info->name);
            if(view)
            {
                view->setCleanFunction([](){
                    CCLoadSprite::doResourceByCommonIndex(11, false);
                });
            }
            PopupViewController::getInstance()->addPopupInViewWithAnim(view);
            release();
        } else if (isTouchInside(m_nodeClick5, pTouch)) {
            retain();
            closeSelf();
            if (info->fbUid != "") {
                CCCommonUtils::flyHint("", "", _lang("105791"));
            }else{
                FriendsController::getInstance()->startDeleteFriend(m_uid);
            }
            release();
        } else if (!isTouchInside(m_touchNode, pTouch)) {
            this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(PopupBaseView::closeSelf));
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
        }
    }
}

void FriendPopupView::playCloseAni()
{

}

void FriendPopupView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

bool FriendPopupView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel1", CCLabelIF*, this->m_nameLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel2", CCLabelIF*, this->m_nameLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node3", CCNode*, this->m_node3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node4", CCNode*, this->m_node4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node5", CCNode*, this->m_node5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode1", CCNode*, this->m_iconNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode2", CCNode*, this->m_iconNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode3", CCNode*, this->m_iconNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode4", CCNode*, this->m_iconNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode5", CCNode*, this->m_iconNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label1", CCLabelIF*, this->m_label1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label2", CCLabelIF*, this->m_label2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label3", CCLabelIF*, this->m_label3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label4", CCLabelIF*, this->m_label4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label5", CCLabelIF*, this->m_label5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick1", CCNode*, this->m_nodeClick1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick2", CCNode*, this->m_nodeClick2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick3", CCNode*, this->m_nodeClick3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick4", CCNode*, this->m_nodeClick4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick5", CCNode*, this->m_nodeClick5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCSprite*, this->m_bg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg2", CCSprite*, this->m_bg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg3", CCSprite*, this->m_bg3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg4", CCSprite*, this->m_bg4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg5", CCSprite*, this->m_bg5);
    
    return false;
}

SEL_CCControlHandler FriendPopupView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return nullptr;
}


FriendChangeDesView* FriendChangeDesView::create(string uid)
{
    auto ret = new FriendChangeDesView(uid);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool FriendChangeDesView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    CCBLoadFile("FriendChangeDesView", this, this);
    CCDictionary* dic = FriendsController::getInstance()->m_data;
    if (dic && !dic->objectForKey(m_uid)) {
        dic = FriendsController::getInstance()->m_fbData;
        if(!dic->objectForKey(m_uid)){
            return false;
        }
    }
//    if (!FriendsController::getInstance()->m_data->objectForKey(m_uid)) {
//        return false;
//    }
    CCCommonUtils::setButtonTitle(m_okBtn, _lang("132109").c_str());
    m_editBox = InputFieldMultiLine::create(m_nameNode->getContentSize(), "01_24.png", 30);
    m_editBox->setAddH(5);
    m_editBox->setAnchorPoint(ccp(0,0));
    m_editBox->setMaxChars(MAX_FRIEND_DESCRIPTION);
    m_editBox->setLineNumber(1);
    m_editBox->setFontColor(ccBLACK);
    m_editBox->setPosition(ccp(0,0));
    m_editBox->setSwallowsTouches(true);
    m_editBox->setMoveFlag(true);
    m_editBox->setTouchPriority(1);
    m_editBox->setcalCharLen(true);
    m_editBox->setOnlySingleLine(true);
    std::string nameTip = "";
    nameTip.append(_lang("105214").c_str());
    m_editBox->setPlaceHolder(nameTip.c_str());
    m_nameNode->addChild(m_editBox);
    
    return true;
}

void FriendChangeDesView::checkNameFun(float dt)
{
    string str = m_editBox->getText();
    int len = str.length();
    if (len == 0) {
        m_okBtn->setEnabled(false);
    } else {
        m_okBtn->setEnabled(true);
    }
}

void FriendChangeDesView::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    this->schedule(schedule_selector(FriendChangeDesView::checkNameFun));
}
void FriendChangeDesView::onExit()
{
    setTouchEnabled(false);
    this->unschedule(schedule_selector(FriendChangeDesView::checkNameFun));
    CCNode::onExit();
}

bool FriendChangeDesView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
void FriendChangeDesView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_bg, pTouch)) {
        closeSelf();
    }
}

void FriendChangeDesView::onOkBtnClick(CCObject *pSender, CCControlEvent event)
{
    string str = m_editBox->getText();
    if (str.length() > 0) {
        FriendsController::getInstance()->startChangeDescription(m_uid, str);
    }
    closeSelf();
}

bool FriendChangeDesView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameNode", CCNode*, this->m_nameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    return false;
}

SEL_CCControlHandler FriendChangeDesView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkBtnClick", FriendChangeDesView::onOkBtnClick);
    return nullptr;
}






