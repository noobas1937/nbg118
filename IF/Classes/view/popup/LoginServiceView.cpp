//
//  LoginServiceView.cpp
//  IF
//
//  Created by wangzhenlei on 13-9-25.
//
//
#include "../../Ext/CCDevice.h"
#include "LoginServiceView.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "LocalController.h"
#include "NetController.h"
#include "ServerLsitCell.h"
#include "CCCommonUtils.h"
#include "SetAccountView.h"
#include <spine/Json.h>

using namespace cocos2d;
void LoginServiceView::onEnter(){
    CCNode::onEnter();
}

void LoginServiceView::onExit(){
    CCNode::onExit();
}

LoginServiceView* LoginServiceView::create(int type){
    LoginServiceView* ret = new LoginServiceView(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LoginServiceView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    auto bg = CCBLoadFile("LoginService", this, this);
    this->setContentSize(bg->getContentSize());
    setTitleName(_lang("105212"));
    changeBGHeight(m_bg);

    this->m_text1->setString(_lang("105211").c_str());
    this->m_text2->setString(_lang("105212").c_str());
    this->m_newAccountBtn->setVisible(false);
    CCCommonUtils::setButtonTitle(this->m_newAccountBtn, _lang("105213").c_str());
    map<int,ServerInfo> *serverArray = &(GlobalData::shared()->serverList);
    if(serverArray->empty()){
        CCHttpRequest* request = new CCHttpRequest();
        string uuid = cocos2d::extension::CCDevice::getDeviceUid();
        string url;
#if COCOS2D_DEBUG == 1
        url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?uuid=%s&loginFlag=%d&loginFlag=%d",SERVERLIST_IP1,uuid.c_str(),GlobalData::shared()->playerInfo.gmFlag,0)->getCString();
#elif COCOS2D_DEBUG == 2
        url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?uuid=%s&loginFlag=%d&loginFlag=%d",SERVERLIST_IP1,uuid.c_str(),GlobalData::shared()->playerInfo.gmFlag,0)->getCString();
#else
        url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?uuid=%s&loginFlag=%d&loginFlag=%d",SERVERLIST_IP2,uuid.c_str(),GlobalData::shared()->playerInfo.gmFlag,0)->getCString();
#endif
        CCLOG("change URL: %s",url.c_str());
        request->setUrl(url.c_str());
        request->setRequestType(CCHttpRequest::Type::POST);
        request->setResponseCallback(this, httpresponse_selector(LoginServiceView::onGetServerList));
        request->setTag("error_report");
        CCHttpClient::getInstance()->send(request);
        CC_SAFE_RELEASE(request);
    }else{
        showTableView();
    }

    return true;
}

void LoginServiceView::onGetServerList(CCHttpClient* client, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLOG("%s completed", response->getHttpRequest()->getTag());
        if (strcmp("error_report",response->getHttpRequest()->getTag())!=0) {
            // return;
        }
    }
    int m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    Json *c = Json_create(serverInfo.c_str());
    Json *serverList=Json_getItem(c,"serverList");
    string lastLoggedServer=Json_getString(c,"lastLoggedServer","");
    int country=Json_getInt(c,"country",0);
    int size = Json_getSize(serverList);
    GlobalData::shared()->loginServerInfo.recommandCountry = country;
    
    map<int,ServerInfo> *serverArray = &(GlobalData::shared()->serverList);
    
    bool bslectIp = false;
    string recommendIp="";
    for (int i=0; i<size; i++) {
        
        Json *c2 = Json_getItemAt(serverList, i);
        string id = Json_getString(c2,"id","-1");
        string port = Json_getString(c2, "port","-1");
        string new1 = Json_getString(c2,"new","-1");
        string name = Json_getString(c2, "name","-1");
        string recommend = Json_getString(c2,"recommend","");
        string zone = Json_getString(c2, "zone","");
        string ip = Json_getString(c2, "ip","");
        string nickName = Json_getString(c2, "gameUserName","");
        string level = Json_getString(c2, "gameUserLevel","-1");
        string gameUid = Json_getString(c2, "gameUid", "");
        
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::create(id), "id");
        dict->setObject(CCString::create(port), "port");
        dict->setObject(CCString::create(new1), "bNew");
        dict->setObject(CCString::create(name), "name");
        dict->setObject(CCString::create(recommend), "bRecommend");
        dict->setObject(CCString::create(zone), "zone");
        dict->setObject(CCString::create(ip), "ip");
        dict->setObject(CCString::create(nickName), "nickName");
        dict->setObject(CCString::create(level), "level");
        dict->setObject(CCString::create(gameUid), "gameUid");
        (*serverArray)[i] = ServerInfo(dict);
        
        
        if(id==lastLoggedServer){
            bslectIp = true;
        }
        if(recommend=="true"){
            recommendIp = ip;
        }
        
    }
    Json_dispose(c);
    showTableView();
}
void LoginServiceView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    LoginServiceView* wCell = dynamic_cast<LoginServiceView*>(cell);
    if(wCell == NULL){
        return;
    }

}
cocos2d::CCSize LoginServiceView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSizeMake(260, 65);
}

CCTableViewCell* LoginServiceView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    map<int,ServerInfo> *serverArray = &(GlobalData::shared()->serverList);
    if(serverArray->size() < idx + 1){
        return NULL;
    }

    auto it = serverArray->find(idx);
    ServerListCell* cell = (ServerListCell*)table->dequeueGrid();
    if(cell){
        cell->setInfo(idx, it->second);
    }else{
        cell = ServerListCell::create(idx,it->second);
    }
    

    return cell;
}

ssize_t LoginServiceView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    map<int,ServerInfo> *serverArray = &(GlobalData::shared()->serverList);
    return serverArray->size();
}

unsigned int LoginServiceView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}
void LoginServiceView::showTableView()
{
    string ip = NetController::shared()->getIp();
    map<int,ServerInfo> *serverArray = &(GlobalData::shared()->serverList);
    map<int, ServerInfo>::iterator it;
    for(it=serverArray->begin(); it != serverArray->end(); ++it)
    {
        if(it->second.ip==ip){
            ServerListCell* cell = ServerListCell::create(it->first,it->second);
            this->m_recentNode->addChild(cell);
            break;
        }
    }
    
    m_tableView = CCMultiColTableView::create(this, CCSize(550,560));
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setMultiColTableViewDelegate(this);
    this->m_listNode->addChild(m_tableView);
    this->m_listNode->setPositionY(this->m_listNode->getPositionY()-300);
    refreshView();
}
void LoginServiceView::refreshView(){
    m_tableView->reloadData();
}

bool LoginServiceView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_recentNode", CCNode*, this->m_recentNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text1", CCLabelIF*, this->m_text1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text2", CCLabelIF*, this->m_text2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newAccountBtn", CCControlButton*, this->m_newAccountBtn);
    return true;
}

SEL_CCControlHandler LoginServiceView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNewAccountBtnClick", LoginServiceView::onNewAccountBtnClick);

    return NULL;
}
void LoginServiceView::onNewAccountBtnClick(cocos2d::CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_USERNAME, "");
    LogoutCommand* cmd=new LogoutCommand();
    cmd->sendAndRelease();
}
