//
//  BaoZangView.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-18.
//
//

#include "BaoZangView.h"
#include "LoadWebImg.h"
#include "FBUtilies.h"
#include "PopupViewController.h"
#include "TipsView.h"
#include "FBSendWorkerCommand.h"
#include "SocialController.h"
#include "ParticleController.h"
#include "FacebookInviteView.h"
#include "FBMineDigCommand.h"
#include "FBMineFriendsCommand.h"
#include "FBMineHelpCommand.h"
#include "FBHelpRecordView.h"
#include "FBFriendCell.h"
#include "UserBindCommand.h"
#include "FBUtilies.h"
#include "BaoZangRewardView.h"
#include "FBMineCallHelpCommand.h"
#include "UIComponent.h"
#include "ParticleFireAni.h"
#include "FBOpenBoxAni.h"
#include "CCShake.h"
#include "GameController.h"
#include "DataRecordCommand.h"
#include "AllianceIntroTip.h"
#include <spine/Json.h>
#include "VipUtil.h"
#include "ActivityController.h"

static const char* shareIcons[5] = {"fb_share_icon1.png","fb_share_icon2.png","fb_share_icon3.png","fb_share_icon4.png","fb_share_icon5.png"};
static const char* shareLang[5] = {"107068","107069","107070","107071","107071"};
static bool tipFlag = false;
bool BaoZangView::getTipFlag(){
    return tipFlag;
}

BaoZangView::~BaoZangView()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFriendsList);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBIviteFriends);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBLoginSucess);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USER_BIND_OK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FACEBOOK_FRIEND_HELP_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFeedDialogResult);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FB_REQUEST_USER_ID);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FB_OPEN_INVITE_VIEW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBAskForHelpResult);
    
    if(m_panelType==1){
        VipUtil::showVIPActiveView();
    }
}

BaoZangView:: BaoZangView(int pType):m_panelType(pType){
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaoZangView::afterGetFriendsInfo), MSG_FBFriendsList, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaoZangView::loginSuccess), MSG_FBLoginSucess, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaoZangView::bindSuccess), MSG_USER_BIND_OK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaoZangView::faceHelpChange), MSG_FACEBOOK_FRIEND_HELP_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaoZangView::requestShareHandle), MSG_FBFeedDialogResult, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaoZangView::FbRequestUserId), FB_REQUEST_USER_ID, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaoZangView::getInviteFriends), MSG_FBIviteFriends, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaoZangView::openInviteFriends), FB_OPEN_INVITE_VIEW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BaoZangView::askForHelpHandle), MSG_FBAskForHelpResult, NULL);
}
BaoZangView* BaoZangView::create(int panelType){
    auto ret = new BaoZangView(panelType);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

void BaoZangView::askForHelpHandle(CCObject *data){
    showTipHandle(0.0);
}

void BaoZangView::openInviteFriends(CCObject *data){
    GlobalData::shared()->isBind = true;
    CCArray* arr = CCArray::create();
    arr->addObjectsFromArray(m_inviteDatas);
    arr->addObjectsFromArray(m_installDatas);
    if(arr->count()>0){
        PopupViewController::getInstance()->addPopupInView(FacebookInviteView::create(arr));
    }else{
        FBUtilies::postFBSelectedFriendList(arr,_lang("107077"));
    }
    SocialController::getInstance()->call = 1;
    if(!m_haveClick){
        string uid = "";
        string server = "";
        string fbName = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERNAME, "");
        int num = m_friendDatas->count();
        for (int i=0; i<num; i++) {
            FBFriendInfo* fbInfo1 = dynamic_cast<FBFriendInfo*>(m_friendDatas->objectAtIndex(i));
            if(fbInfo1==NULL || fbInfo1->gUid==""){
                continue;
            }
            if (uid!="") {
                uid += "|";
            }
            uid += fbInfo1->gUid;
            if (server!="") {
                server += "|";
            }
            server += CC_ITOA(fbInfo1->server);
        }
        FBMineCallHelpCommand* cmd = new FBMineCallHelpCommand(uid,server,fbName);
        //cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(BaoZangView::callHelpSuccess), NULL));
        cmd->sendAndRelease();
        m_haveClick = true;
    }
}

void BaoZangView::FbRequestUserId(CCObject* param){
    this->scheduleOnce(schedule_selector(BaoZangView::delayCheck), 0.4);
}

void BaoZangView::getInviteFriends(CCObject *param){
    CCLOG("FacebookInviteView getInviteFriends");
    CCArray* friendArr = dynamic_cast<CCArray*>(param);
    m_inviteDatas->removeAllObjects();
    if(friendArr!=NULL){
        m_inviteDatas->addObjectsFromArray(friendArr);
    }else{
        this->scheduleOnce(schedule_selector(BaoZangView::delayParseInvite), 0.2);
    }
}

void BaoZangView::delayParseInvite(float _time){
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
            m_inviteDatas->addObject(friendInfo);
        }
        Json_dispose(fjson);
    }
}

void BaoZangView::delayCheck(float t_time){
    string fbId =CCUserDefault::sharedUserDefault()->getStringForKey(FB_REQUEST_USER_ID, "");
    CCLOG("fbId=%s",fbId.c_str());
    GameController::getInstance()->removeWaitInterface();
    if(m_fbUid==fbId){
        GlobalData::shared()->installFriendsInfo = "";
        m_reLoginFb = false;
        bindSuccess(NULL);
    }else{
        m_reLoginFb = true;
        CCCommonUtils::flyHint("", "", _lang("107078"));
        FBUtilies::fbLogout();//不是同一个号
    }
}

void BaoZangView::afterGetFriendsInfo(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    CCArray* friends = dynamic_cast<CCArray*>(param);
    if (friends) {
        m_data->removeAllObjects();
        m_friendDatas->removeAllObjects();
        m_installDatas->removeAllObjects();
        int num = friends->count();
        string ids = "";
        for (int i=0; i<num; i++) {
            auto dic = _dict(friends->objectAtIndex(i));
            if(dic){
                dic->setObject(CCString::create("1"), "flag");
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = dic->valueForKey("id")->getCString();
                info->name = dic->valueForKey("name")->getCString();
                info->url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",dic->valueForKey("id")->getCString())->getCString();
                m_data->addObject(info);
                m_friendDatas->addObject(info);
                if(ids!=""){
                    ids += ",";
                }
                ids += info->id;
                m_installDatas->addObject(dic);
            }
        }
        int gapNum = 5 - m_data->count();
        if(gapNum<=0){
            gapNum = 1;
        }
        for(int i=0;i<gapNum;i++){
            FBFriendInfo* info = FBFriendInfo::create();
            info->id = CC_ITOA(12241111+i);
            info->name = "";
            info->url = "";
            info->add = true;
            m_data->addObject(info);
        }
        if(ids!="" && ActivityController::getInstance()->baozangHelpArr && ActivityController::getInstance()->baozangHelpArr->count()<=0){
            FBMineFriendsCommand* fbCmd = new FBMineFriendsCommand(ids);
            fbCmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(BaoZangView::updateServerData), NULL));
            fbCmd->sendAndRelease();
        }
        if(m_friendDatas->count()>0){
            refreshHelpData();
        }
        CCLOG("friends count=%d",friends->count());
    }
}

void BaoZangView::loginSuccess(CCObject* param){
    this->scheduleOnce(schedule_selector(BaoZangView::delayUseData), 0.2);
}

void BaoZangView::delayUseData(float _time){
    string m_facebookUid =CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_uid", "");
    if (m_facebookUid == "") {
        return;
    }
    string preuid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    string preuName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
    string m_facebookName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
    //bindSuccess(NULL);
    if(!m_reLoginFb){
        UserBindCommand* cmd = new UserBindCommand("",m_facebookUid,"","","",1,preuid,"",m_facebookName,preuName);
        cmd->sendAndRelease();
        m_reLoginFb = false;
        GameController::getInstance()->showWaitInterface();
    }else{
        m_reLoginFb = false;
        if(m_fbUid!=m_facebookUid){
            CCCommonUtils::flyHint("", "", _lang("107078"));
            m_reLoginFb = true;
            FBUtilies::fbLogout();//不是同一个号
        }else{
            GlobalData::shared()->installFriendsInfo = "";
            bindSuccess(NULL);
        }
    }
}

void BaoZangView::afterGetRequestResult(CCObject* param){
    
}

void BaoZangView::bindSuccess(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if (_fbUid != "")
    {
        m_lockNode->setVisible(false);
        m_helpNode->setVisible(true);
        m_helpTipNode->setVisible(true);
        m_buttomNode->setVisible(true);
        m_fbLoginNode->setVisible(false);
        m_btnRecord->setVisible(true);
    }
    else
    {
        m_lockNode->setVisible(false);
        m_helpNode->setVisible(false);
        m_helpTipNode->setVisible(false);
        m_buttomNode->setVisible(false);
        m_fbLoginNode->setVisible(true);
        m_btnRecord->setVisible(false);
    }
    if(GlobalData::shared()->installFriendsInfo==""){
        if(!m_requestFb){
            m_requestFb = true;
            FBUtilies::getFBFriendList();
            FBUtilies::inviteFriends();
        }
    }
    this->unschedule(schedule_selector(BaoZangView::checkFriendsData));
    this->schedule(schedule_selector(BaoZangView::checkFriendsData),1);
    this->checkFriendsData(0);
    
    this->unschedule(schedule_selector(BaoZangView::checkBaoZang));
    this->schedule(schedule_selector(BaoZangView::checkBaoZang),1);
    this->checkBaoZang(0);
}

bool BaoZangView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvTxt", CCLabelIF*, this->m_lvTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_friendNode", CCNode*, this->m_friendNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttomNode", CCNode*, this->m_buttomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_FBInfoBtn", CCControlButton*, this->m_FBInfoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCallHelp", CCControlButton*, this->m_btnCallHelp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnRecord", CCControlButton*, this->m_btnRecord);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_boxOpen", CCNode*, this->m_boxOpen);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_boxClose", CCNode*, this->m_boxClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNode", CCNode*, this->m_helpNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpDesc", CCLabelIF*, this->m_helpDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbLoginNode", CCNode*, this->m_fbLoginNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginDesc", CCLabelIF*, this->m_loginDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginBtnTxt", CCLabelIF*, this->m_loginBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNode", CCNode*, this->m_lockNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cdTxt", CCLabelIF*, this->m_cdTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickRewardArea", CCNode*, this->m_clickRewardArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_boxNode", CCNode*, this->m_boxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_boxEffectNode", CCNode*, this->m_boxEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginTipNode", CCNode*, this->m_loginTipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpTipNode", CCNode*, this->m_helpTipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLv", CCControlButton*, this->m_btnLv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_introNode", CCNode*, this->m_introNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    return true;
}

SEL_CCControlHandler BaoZangView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFBInfoBtnClick", BaoZangView::onFBInfoBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCallHelpClick", BaoZangView::onCallHelpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRecordClick", BaoZangView::onRecordClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLoginClick", BaoZangView::onLoginClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpClick", BaoZangView::onHelpClick);
    return NULL;
}

CCSize BaoZangView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(110*2.4,155.0*1.8);
    }
    return CCSize(110,155.0);
}

CCSize BaoZangView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(110*2.4,155.0*1.8);
    }
    return CCSize(110,155.0);
}

CCTableViewCell* BaoZangView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    FBFriendCell* cell = (FBFriendCell*)table->dequeueCell();
    if(cell==NULL){
        cell = FBFriendCell::create();
    }
    FBFriendInfo* info = dynamic_cast<FBFriendInfo*>(m_data->objectAtIndex(idx));
    cell->setData(info,m_friendNode);
    return cell;
}

ssize_t BaoZangView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void BaoZangView::onFBInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("107014"),kCCTextAlignmentLeft));
}

void BaoZangView::onCallHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(tipFlag){
        return ;
    }
    GlobalData::shared()->isBind = true;// ios https://fb.me/789290781112244  android https://fb.me/789279541113368
    openInviteFriends(NULL);
//    return ;//不要feed了
//    string link = "";
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    link = "https://fb.me/789279541113368?from_feed=android";
//#else
//    link = "https://fb.me/789290781112244?from_feed=ios";
//#endif
//    int rand = GlobalData::shared()->getRand(0,5);
//    int randL = GlobalData::shared()->getRand(0,4);
//    string fbIcon = CCString::createWithFormat("http://cok.eleximg.com/cok/img/%s",shareIcons[rand])->getCString();
//    FBUtilies::fbPublishFeedDialog("Clash Of Kings",_lang("107088"),_lang("107087"),link,fbIcon);//shareLang[randL]
}

void BaoZangView::callHelpSuccess(CCObject* param){
    //CCCommonUtils::flyHint("", "", _lang("107073"));
}

void BaoZangView::onfriendInfoClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("107014"),kCCTextAlignmentLeft));
}

void BaoZangView::onExpHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("107007"),kCCTextAlignmentLeft));
}

void BaoZangView::onRecordClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(FBHelpRecordView::create());
}

void BaoZangView::onLvClick(CCObject * pSender, Control::EventType pCCControlEvent){
    string tip = _lang("107055");
    tip += "\n";
    tip += _lang_2("107006", CC_ITOA(SocialController::getInstance()->exp), CC_ITOA(SocialController::getInstance()->xml_exp));
    PopupViewController::getInstance()->addPopupView(TipsView::create(tip,kCCTextAlignmentLeft));
}

void BaoZangView::onHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(!tipFlag){
        m_parVec.clear();
        CCRenderTexture* layer = CCRenderTexture::create(640, m_introNode->getContentSize().height);
        ccBlendFunc ccb1 = {GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
        layer->getSprite()->setBlendFunc(ccb1);
        layer->clear(0.0f, 0.0f, 0.0f, 0.7f);//0.0f, 0.0f, 0.0f, 0.7f
        layer->setAnchorPoint(ccp(0, 0));
        layer->setPosition(ccp(320, m_introNode->getContentSize().height/2));
        m_introNode->addChild(layer,-20);
        layer->removeAllChildren();
        layer->begin();

        CCSize size = m_helpBtn->getContentSize();
        CCNode* frieNode =  CCNode::create();
        frieNode->setAnchorPoint(ccp(0.5, 0.5));
        frieNode->setTag(999);
        m_helpBtn->addChild(frieNode);
        addParticle(frieNode,ccp(0, 0),size,false);
        
        CCPoint startPos = ccp(235, m_introNode->getContentSize().height - 250);
        CCPoint pos = ccp(100,startPos.y+120);
        size = CCSize(455,110);
        if(m_fbLoginNode->isVisible()){
            AllianceIntroTip* loginIntro = AllianceIntroTip::create(false, ccp(300, -17.0),_lang("107082"));
            loginIntro->setPosition(ccp(20, 285));
            m_introNode->addChild(loginIntro);
            
            pos = ccp(170,180);
            size = CCSize(320,80);
            addParticle(m_introNode,pos,size);
            
            CCLayerColor* intro1layer = CCLayerColor::create();
            intro1layer->setOpacity(255);
            intro1layer->setColor(ccc3(255, 255, 255));
            intro1layer->setContentSize(size);
            intro1layer->setPosition(pos);
            ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
            intro1layer->setBlendFunc(cbf);
            intro1layer->visit();
        }else{
            AllianceIntroTip* intro1 = AllianceIntroTip::create(true, ccp(300, 103.0),_lang("107085"));
            intro1->setPosition(ccp(20, m_introNode->getContentSize().height - 250));
            m_introNode->addChild(intro1);
            pos = ccp(100,startPos.y+120);
            size = CCSize(455,110);
            addParticle(m_introNode,pos,size);
            
            pos = ccp(87,startPos.y + 150);
            CCLayerColor* intro1layer = CCLayerColor::create();
            intro1layer->setOpacity(255);
            intro1layer->setColor(ccc3(255, 255, 255));
            intro1layer->setContentSize(size);
            intro1layer->setPosition(pos);
            ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
            intro1layer->setBlendFunc(cbf);
            intro1layer->visit();
            
            AllianceIntroTip* intro2 = AllianceIntroTip::create(false, ccp(260, -17.0),_lang("107084"));
            intro2->setPosition(ccp(20, 140));
            m_introNode->addChild(intro2);
            
            pos = ccp(30,0);
            size = CCSize(580,150);
            addParticle(m_introNode,pos,size);
            
            pos = ccp(30,0);
            size = CCSize(580,150);
            CCLayerColor* intro2layer = CCLayerColor::create();
            intro2layer->setOpacity(255);
            intro2layer->setColor(ccc3(255, 255, 255));
            intro2layer->setContentSize(size);
            intro2layer->setPosition(pos);
            cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
            intro2layer->setBlendFunc(cbf);
            intro2layer->visit();
            
            AllianceIntroTip* intro3 = AllianceIntroTip::create(false, ccp(180, -17.0),_lang("107086"));
            intro3->setPosition(ccp(170, 320));
            m_introNode->addChild(intro3);
            
            pos = ccp(230,230);
            size = CCSize(185,50);
            addParticle(m_introNode,pos,size);
            
            CCLayerColor* intro3layer = CCLayerColor::create();
            intro3layer->setOpacity(255);
            intro3layer->setColor(ccc3(255, 255, 255));
            intro3layer->setContentSize(size);
            intro3layer->setPosition(pos);
            cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
            intro3layer->setBlendFunc(cbf);
            intro3layer->visit();
        }

        layer->end();
        tipFlag = true;
    }else{
        tipFlag = false;
        m_helpBtn->removeChildByTag(999, true);
        m_introNode->removeAllChildrenWithCleanup(true);
    }
}

void BaoZangView::addParticle(CCNode* node,CCPoint pos,CCSize size,bool addBatch){
    if(node==NULL) return ;
    string tmpStart = "ShowFire_";
    string tmpStart1 = "ShowFireUp_";
    for (int i=1; i<=5; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
        particle->setPosition(pos+ccp(size.width/2, -3));
        particle->setPosVar(ccp(size.width/2, 0));
        if(addBatch){
            addParticleToBatch(node,particle);
        }else{
            node->addChild(particle);
        }
    
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
        particle1->setPosition(pos+ccp(size.width/2, size.height-3));
        particle1->setPosVar(ccp(size.width/2, 0));
        if(addBatch){
            addParticleToBatch(node,particle1);
        }else{
            node->addChild(particle1);
        }
        
        auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
        particle2->setPosition(pos+ccp(0, size.height/2));
        particle2->setPosVar(ccp(0, size.height/2));
        if(addBatch){
            addParticleToBatch(node,particle2);
        }else{
            node->addChild(particle2);
        }
        
        auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
        particle3->setPosition(pos+ccp(size.width, size.height/2));
        particle3->setPosVar(ccp(0, size.height/2));
        if(addBatch){
            addParticleToBatch(node,particle3);
        }else{
            node->addChild(particle3);
        }
    }
}

void BaoZangView::addParticleToBatch(CCNode* node,cocos2d::CCParticleSystemQuad *particle){
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    node->addChild(newBatch,-10);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void BaoZangView::onLoginClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if (FBUtilies::fbIsLogin())
    {
        FBUtilies::fbLogout();
    }
    GlobalData::shared()->isBind = true;
    bool flag = FBUtilies::fbLogin();
    string str = _lang("107080");
    if(!flag){
        GlobalData::shared()->isBind = false;
        CCCommonUtils::flyHint("", "", _lang("107080"));
    }
}

bool BaoZangView::init(){
    bool ret = false;
    if(PopupBaseView::init())
    {
        setIsHDPanel(true);
        GlobalData::shared()->installFriendsInfo = "";
        GlobalData::shared()->inviteFriendsInfo = "";
        CCUserDefault::sharedUserDefault()->setStringForKey("installFriendsInfo", "");
        CCUserDefault::sharedUserDefault()->setStringForKey("inviteFriendsInfo", "");
        CCUserDefault::sharedUserDefault()->flush();
        
        CCLoadSprite::doResourceByCommonIndex(207, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(207, false);
        });
        auto tmpCCB = CCBLoadFile("BaozangView", this, this);
        this->setContentSize(tmpCCB->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height-prev;
        
        m_introNode->setContentSize(m_viewBg->getContentSize());
        m_data = CCArray::create();
        m_friendDatas = CCArray::create();
        m_inviteDatas = CCArray::create();
        m_installDatas = CCArray::create();
        
        m_tableView = CCTableView::create(this, m_friendNode->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionHorizontal);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tableView->setDelegate(this);
        m_tableView->setTouchPriority(Touch_Default);
        m_friendNode->addChild(m_tableView);
        
        if (!CCCommonUtils::isIosAndroidPad())
        {
            m_friendNode->setPositionY(m_friendNode->getPositionY()-20);
            m_buttomNode->setPositionY(m_buttomNode->getPositionY() - add);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - add/2);
            m_helpNode->setPositionY(m_helpNode->getPositionY() - add);
            m_fbLoginNode->setPositionY(m_fbLoginNode->getPositionY() - add);
            m_lockNode->setPositionY(m_lockNode->getPositionY() - add/2);
        }
        else
            m_friendNode->setPositionY(m_friendNode->getPositionY()+10);
       
        
        
        CCCommonUtils::setButtonTitle(m_btnCallHelp, _lang("107057").c_str());
        CCCommonUtils::setButtonTitle(m_btnRecord, _lang("107065").c_str());//
        m_loginBtnTxt->setString(_lang("105287"));
        m_loginDesc->setString(_lang("107051"));
        m_helpDesc->setString(_lang_1("107056",CC_ITOA(SocialController::getInstance()->xml_miner - SocialController::getInstance()->miner)));
        m_cdTxt->setString(_lang("107063"));
        m_titleTxt->setString(_lang("107050"));
        
        float len = SocialController::getInstance()->miner/(1.0*SocialController::getInstance()->xml_miner);
        if(len>1) len = 1.0;
        m_progress->setContentSize(CCSize(len*408.0,22.0));
        string numStr = CC_ITOA(SocialController::getInstance()->miner);
        numStr += "/";
        numStr += CC_ITOA(SocialController::getInstance()->xml_miner);
        m_lvTxt->setString(numStr.c_str());
        
        int lv = MIN(GlobalData::shared()->glodMine_fb_k4,SocialController::getInstance()->xml_lv);
        if(lv<=0){
            lv = 1;
        }
        CCCommonUtils::setButtonTitle(m_btnLv, _lang_1("107005", CC_ITOA(lv)));
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        
        m_helpNode->setVisible(false);
        m_helpTipNode->setVisible(false);
        m_btnCallHelp->setVisible(false);
        m_reLoginFb = false;
        m_requestFb = false;
        m_fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
        if (m_fbUid != "")
        {
            m_reLoginFb = true;
            m_loginBtnTxt->setString(_lang("107001"));
            m_boxOpen->setVisible(false);
            m_boxClose->setVisible(!m_boxOpen->isVisible());
            m_btnRecord->setVisible(false);
            m_lockNode->setVisible(false);
            m_buttomNode->setVisible(true);
            if(FBUtilies::fbIsLogin()){
                m_fbLoginNode->setVisible(false);
                GameController::getInstance()->showWaitInterface();
                FBUtilies::requestForMeInfo();//去验证
            }else{
                m_fbLoginNode->setVisible(true);
            }
        }else{
            m_loginBtnTxt->setString(_lang("105287"));
            m_boxOpen->setVisible(false);
            m_boxClose->setVisible(!m_boxOpen->isVisible());
            m_btnRecord->setVisible(false);
            m_lockNode->setVisible(false);
            m_buttomNode->setVisible(true);
            m_fbLoginNode->setVisible(true);
        }
        m_timeCount = 0;
        GlobalData::shared()->inviteFriendsInfo = "";
        m_haveClick = false;
        tipFlag = false;
        
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(BaoZangView::showFriendNode));
        this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
        this->scheduleOnce(schedule_selector(BaoZangView::delayShow), 0.1);
        ret = true;
    }
    return ret;
    
}

void BaoZangView::checkBaoZang(float _time){
    m_timeCount -= 1;
    string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if(_fbUid=="" || !FBUtilies::fbIsLogin())return ;
    double mineTime = SocialController::getInstance()->mineTime - GlobalData::shared()->getWorldTime();
    if(mineTime>0){
        m_timeTxt->setString(CC_SECTOA(mineTime));
        m_lockNode->setVisible(true);
        m_helpNode->setVisible(false);
        m_helpTipNode->setVisible(false);
    }else{
        m_lockNode->setVisible(false);
        if(!m_fbLoginNode->isVisible()){
            float len = SocialController::getInstance()->miner/(1.0*SocialController::getInstance()->xml_miner);
            if(len>1) len = 1.0;
            m_progress->setContentSize(CCSize(len*408.0,22.0));
            int current = SocialController::getInstance()->miner;
            if(current>=SocialController::getInstance()->xml_miner){
                current = SocialController::getInstance()->xml_miner;
            }
            string numStr = CC_ITOA(current);
            numStr += "/";
            numStr += CC_ITOA(SocialController::getInstance()->xml_miner);
            m_lvTxt->setString(numStr.c_str());
            
            m_buttomNode->setVisible(true);
            m_helpNode->setVisible(true);
            m_helpTipNode->setVisible(true);
            if(SocialController::getInstance()->miner!=0 && SocialController::getInstance()->miner>=SocialController::getInstance()->xml_miner){
                m_boxOpen->setVisible(true);
                m_boxClose->setVisible(!m_boxOpen->isVisible());
                m_helpDesc->setString(_lang("107058"));
            }else{
                m_helpDesc->setString(_lang_1("107056",CC_ITOA(SocialController::getInstance()->xml_miner-SocialController::getInstance()->miner)));
            }
            if(SocialController::getInstance()->xml_miner>SocialController::getInstance()->miner){
                 m_btnCallHelp->setVisible(true);
            }else{
                 m_btnCallHelp->setVisible(false);
            }
        }
    }
}

void BaoZangView::checkFriendsData(float _time){
    GlobalData::shared()->installFriendsInfo = CCUserDefault::sharedUserDefault()->getStringForKey("installFriendsInfo", "");
    if(m_friendDatas->count()>0){
        this->unschedule(schedule_selector(BaoZangView::checkFriendsData));
    }
    if(GlobalData::shared()->installFriendsInfo!=""){
        Json* fjson = Json_create(GlobalData::shared()->installFriendsInfo.c_str());
        string ids = "";
        if(fjson){
            m_data->removeAllObjects();
            m_friendDatas->removeAllObjects();
            m_installDatas->removeAllObjects();
            int size = Json_getSize(fjson);
            for (int i=0; i<size; i++) {
                Json *item = Json_getItemAt(fjson, i);
                string name = Json_getString(item,"name","");
                string id = Json_getString(item,"id","");
                if(ids!=""){
                    ids +=",";
                }
                ids += id;
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = id;
                info->name = name;
                info->url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",id.c_str())->getCString();
                m_data->addObject(info);
                m_friendDatas->addObject(info);
                
                CCDictionary* friendInfo = CCDictionary::create();
                friendInfo->setObject(CCString::create(id.c_str()), "id");
                friendInfo->setObject(CCString::create(name.c_str()), "name");
                friendInfo->setObject(CCString::create(info->url.c_str()), "picture");
                friendInfo->setObject(CCString::create("1"), "flag");
                m_installDatas->addObject(friendInfo);
            }
            Json_dispose(fjson);
            int gapNum = 5 - m_data->count();
            if(gapNum<=0){
                gapNum = 1;
            }
            for(int i=0;i<gapNum;i++){
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = CC_ITOA(12241111+i);
                info->name = "";
                info->url = "";
                info->add = true;
                m_data->addObject(info);
            }
        }
        if(ids!="" && ActivityController::getInstance()->baozangHelpArr && ActivityController::getInstance()->baozangHelpArr->count()<=0){
            FBMineFriendsCommand* fbCmd = new FBMineFriendsCommand(ids);
            fbCmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(BaoZangView::updateServerData), NULL));
            fbCmd->sendAndRelease();
        }
    }
    if(m_friendDatas->count()>0){
        refreshHelpData();
    }
}

void BaoZangView::updateServerData(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* friendList = dynamic_cast<CCArray*>(dic->objectForKey("friendList"));
        if(friendList){
            ActivityController::getInstance()->baozangHelpArr->addObjectsFromArray(friendList);
        }
    }
    refreshHelpData();
}

void BaoZangView::refreshHelpData(){
    CCArray* friendList = ActivityController::getInstance()->baozangHelpArr;
    if(friendList){
        int num = friendList->count();
        for(int i=0;i<num;i++){
            auto oneInfo = dynamic_cast<CCDictionary*>(friendList->objectAtIndex(i));
            if (oneInfo) {
                string id = oneInfo->valueForKey("fId")->getCString();
                string gUid = oneInfo->valueForKey("gUid")->getCString();
                int server = oneInfo->valueForKey("server")->intValue();
                int help = oneInfo->valueForKey("help")->intValue();
                int count = m_friendDatas->count();
                for (int j=0; j<count; j++) {
                    FBFriendInfo* fbInfo = dynamic_cast<FBFriendInfo*>(m_friendDatas->objectAtIndex(j));
                    if(fbInfo!=NULL && fbInfo->id==id && help >= fbInfo->help){
                        fbInfo->server = server;
                        fbInfo->help = help;
                        fbInfo->gUid = gUid;
                        break;
                    }
                }
            }
        }
    }
    m_data->removeAllObjects();
    m_data->addObjectsFromArray(m_friendDatas);
    int count = m_data->count();
    for (int i=0; i<count; i++) {
        FBFriendInfo* fbInfo1 = dynamic_cast<FBFriendInfo*>(m_data->objectAtIndex(i));
        if(fbInfo1->help==1){
            for (int j=0; j<i; j++) {
                FBFriendInfo* fbInfo2 = dynamic_cast<FBFriendInfo*>(m_data->objectAtIndex(j));
                if (fbInfo2->help==0) {
                    m_data->swap(i, j);
                    break;
                }
            }
        }
    }
    int gapNum = 5 - m_data->count();
    if(gapNum<=0){
        gapNum = 1;
    }
    for(int i=0;i<gapNum;i++){
        FBFriendInfo* info = FBFriendInfo::create();
        info->id = CC_ITOA(12241111+i);
        info->name = "";
        info->url = "";
        info->add = true;
        m_data->addObject(info);
    }
    m_tableView->reloadData();
}

void BaoZangView::faceHelpChange(CCObject* param){
    auto dic = _dict(param);
    if(dic){
        string fbid = dic->valueForKey("fId")->getCString();
        int count = m_friendDatas->count();
        bool flag = false;
        for (int i=0; i<count; i++) {
            FBFriendInfo* fbInfo = dynamic_cast<FBFriendInfo*>(m_friendDatas->objectAtIndex(i));
            if(fbInfo->id == fbid){
                fbInfo->help = 1;
                flag = true;
                break;
            }
        }
        if(flag){
            m_data->removeAllObjects();
            m_data->addObjectsFromArray(m_friendDatas);
            int count = m_data->count();
            for (int i=0; i<count; i++) {
                FBFriendInfo* fbInfo1 = dynamic_cast<FBFriendInfo*>(m_data->objectAtIndex(i));
                if(fbInfo1->help==1){
                    for (int j=0; j<i; j++) {
                        FBFriendInfo* fbInfo2 = dynamic_cast<FBFriendInfo*>(m_data->objectAtIndex(j));
                        if (fbInfo2->help==0) {
                            m_data->swap(i, j);
                            break;
                        }
                    }
                }
            }
            int gapNum = 5 - m_data->count();
            if(gapNum<=0){
                gapNum = 1;
            }
            for(int i=0;i<gapNum;i++){
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = CC_ITOA(12241111+i);
                info->name = "";
                info->url = "";
                info->add = true;
                m_data->addObject(info);
            }
            m_tableView->reloadData();
        }
    }
}

void BaoZangView::requestShareHandle(CCObject* param){
    this->scheduleOnce(schedule_selector(BaoZangView::showTipHandle), 0.1);
}

void BaoZangView::showTipHandle(float _time){
    string tip = _lang("107081");
    CCCommonUtils::flyHint("", "", tip);
    CCArray* p = CCArray::create();
    CCArray* d = CCArray::create();
    DataRecordCommand *cmd = new DataRecordCommand(FB_SHARE_FEED, p, d);
    cmd->sendAndRelease();
}

void BaoZangView::onEnter(){
    CCNode::onEnter();
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    this->setTitleName(_lang("107050"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void BaoZangView::delayShow(float _time){
    string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if(GlobalData::shared()->installFriendsInfo=="" && _fbUid!=""){
        if(m_reLoginFb){
//            GlobalData::shared()->isBind = true;
//            FBUtilies::fbLogin();
        }else{
            if(!m_requestFb){
                FBUtilies::getFBFriendList();
                FBUtilies::inviteFriends();
                m_requestFb = true;
            }
            m_reLoginFb = false;
        }
    }
    this->unschedule(schedule_selector(BaoZangView::checkFriendsData));
    this->schedule(schedule_selector(BaoZangView::checkFriendsData),1);
    this->checkFriendsData(0.0);
    m_friendNode->setVisible(false);
    if(m_data->count()<=0){
        for(int i=0;i<5;i++){
            FBFriendInfo* info = FBFriendInfo::create();
            info->id = CC_ITOA(12241111+i);
            info->name = "";
            info->url = "";
            info->add = true;
            m_data->addObject(info);
        }
        m_tableView->reloadData();
    }
}

void BaoZangView::showFriendNode(){
    m_friendNode->setVisible(true);
}

void BaoZangView::onExit(){
    GlobalData::shared()->isBind = false;
    GameController::getInstance()->removeWaitInterface();
    setTouchEnabled(false);
    CCNode::onExit();
}

bool BaoZangView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void BaoZangView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(tipFlag){
        return ;
    }
    if(isTouchInside(m_btnLv,pTouch)){
        onLvClick(NULL,Control::EventType::TOUCH_DOWN);
    }else if(isTouchInside(m_clickRewardArea,pTouch) && m_timeCount<=0){
        if(SocialController::getInstance()->miner!=0 && SocialController::getInstance()->miner>=SocialController::getInstance()->xml_miner){
            FBMineDigCommand* cmd = new FBMineDigCommand();
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(BaoZangView::digMineSuccess), NULL));
            cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(BaoZangView::digFail), NULL));
            cmd->sendAndRelease();
            SocialController::getInstance()->miner=0;
            m_timeCount = 4;
            CCShake* shake3 = CCShake::createWithStrength(0.4, 16, 13);
            m_boxNode->stopAllActions();
            m_boxNode->runAction(shake3);
            GameController::getInstance()->showWaitInterface();
        }else{
            CCShake* shake = CCShake::createWithStrength(0.7, 15, 6);
            m_loginTipNode->stopAllActions();
            m_loginTipNode->runAction(shake);
            CCShake* shake2 = CCShake::createWithStrength(0.7, 15, 6);
            m_helpTipNode->stopAllActions();
            m_helpTipNode->runAction(shake2);
            m_timeCount = 1;
            
            CCShake* shake3 = CCShake::createWithStrength(0.7, 18, 15);
            m_boxNode->stopAllActions();
            m_boxNode->runAction(shake3);
        }
    }
    
    //test
}

void BaoZangView::digFail(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
}

void BaoZangView::playRewardAni(float _time){
    this->unschedule(schedule_selector(BaoZangView::playRewardAni));
    if(m_mutiGlod>1){
        auto spr = CCLoadSprite::createSprite(CCString::createWithFormat("scrifice_hint_%d.png",m_mutiGlod)->getCString());
        spr->setScale(15);
        m_boxEffectNode->addChild(spr);
        CCScaleTo* sc3 = CCScaleTo::create(0.7, 1,1.4);
        CCScaleTo* sc4 = CCScaleTo::create(0.07, 1,0.8);
        CCScaleTo* sc5 = CCScaleTo::create(0.06, 1,1.3);
        CCScaleTo* sc6 = CCScaleTo::create(0.15, 1,0.7);
        CCScaleTo* sc7 = CCScaleTo::create(0.15, 1,1);
        CCFadeOut* fadeout = CCFadeOut::create(0.5);
        CCCallFunc* fun = CCCallFunc::create(this, callfunc_selector(BaoZangView::playParAni));
        CCSequence* sc = CCSequence::create(sc3,fun,sc4,sc5,sc6,sc7,CCDelayTime::create(0.5),fadeout,NULL);
        spr->runAction(sc);
    }else{
        this->unschedule(schedule_selector(BaoZangView::openRewardView));
        this->schedule(schedule_selector(BaoZangView::openRewardView), 1.2);
    }
}

void BaoZangView::playParAni(){
    for (int i=1; i<9; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("ScienceOut_%d",i)->getCString());//CityRockfall_ Harvest_
        m_boxEffectNode->addChild(particle);
    }
    if(m_mutiGlod==10){
        for (int i=1; i<5; i++) {
            if(i==2 || i==4)continue;
            auto particle = ParticleController::createParticle(CCString::createWithFormat("LevelUp_%d",i)->getCString());//CityRockfall_ Harvest_
            m_boxEffectNode->addChild(particle);
        }
    }
    this->unschedule(schedule_selector(BaoZangView::openRewardView));
    this->schedule(schedule_selector(BaoZangView::openRewardView), 1.5);
}

void BaoZangView::openRewardView(float _time){
    this->unschedule(schedule_selector(BaoZangView::openRewardView));
    PopupViewController::getInstance()->addPopupView(BaoZangRewardView::create(m_rewardDic));
    this->unschedule(schedule_selector(BaoZangView::checkBaoZang));
    this->unschedule(schedule_selector(BaoZangView::showCD));
    this->schedule(schedule_selector(BaoZangView::showCD), 2.4);
}

void BaoZangView::showCD(float _time){
    this->unschedule(schedule_selector(BaoZangView::showCD));
    float len = SocialController::getInstance()->exp/(1.0*SocialController::getInstance()->xml_exp);
    if(len>1) len = 1.0;
    m_progress->setContentSize(CCSize(len*408.0,22.0));
    int lv = MIN(GlobalData::shared()->glodMine_fb_k4,SocialController::getInstance()->xml_lv);
    if(lv<=0){
        lv = 1;
    }
    m_lvTxt->setString(_lang_1("107005", CC_ITOA(lv)));
    m_lockNode->setVisible(true);
    m_boxClose->setVisible(true);
    m_boxOpen->setVisible(false);
    this->unschedule(schedule_selector(BaoZangView::checkBaoZang));
    this->schedule(schedule_selector(BaoZangView::checkBaoZang),1);
    this->checkBaoZang(0.0);
}

void BaoZangView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void BaoZangView::digMineSuccess(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    m_timeCount = 0;
    string helpInfo = "";
    m_mutiGlod = 0;
    if(param){
        NetResult* result = dynamic_cast<NetResult*>(param);
        auto dic = _dict(result->getData());
        m_rewardDic = dic;
    }
    if(m_rewardDic){
        if(m_rewardDic->objectForKey("userMine")){
            auto userMine = _dict(m_rewardDic->objectForKey("userMine"));
            SocialController::getInstance()->parse(userMine);
        }
        if(m_rewardDic->objectForKey("gold")){
            auto goldDic = _dict(m_rewardDic->objectForKey("gold"));
            m_mutiGlod = goldDic->valueForKey("multiple")->intValue();
        }
    }
    this->unschedule(schedule_selector(BaoZangView::checkBaoZang));
    m_boxClose->setVisible(false);
    m_boxOpen->setVisible(true);
    m_helpNode->setVisible(false);
    m_helpTipNode->setVisible(false);
    CCScaleTo* sc1 = CCScaleTo::create(0.07, 1,1.5);
    CCScaleTo* sc2 = CCScaleTo::create(0.08, 1,0.8);
    CCScaleTo* sc3 = CCScaleTo::create(0.05, 1,1.0);
    CCSequence* sc = CCSequence::create(sc1,sc2,sc3,CCDelayTime::create(0.15),NULL);
    m_boxNode->stopAllActions();
    m_boxNode->runAction(sc);
    
    FBOpenBoxAni* ani = FBOpenBoxAni::create(m_mutiGlod);
    this->addChild(ani);
    
    this->unschedule(schedule_selector(BaoZangView::playRewardAni));
    this->schedule(schedule_selector(BaoZangView::playRewardAni), 0.2);
}
