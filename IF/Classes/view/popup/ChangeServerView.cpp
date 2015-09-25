//
//  ChangeServerView02.cpp
//  IF
//
//  Created by liweiyu on 14-9-18.
//
//

#include "ChangeServerView.h"
#include "NetController.h"
#include "SceneController.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "GuideController.h"
#include "WorldController.h"
#include "CCThreadManager.h"
#include "ToolController.h"
#include "ChatController.h"
#include "ScienceController.h"
#include "QueueController.h"
#include "GmController.h"
#include <spine/Json.h>
#include "FBUtilies.h"
#include "GMCommand.h"

ChangeServeView *ChangeServeView::create(){
    ChangeServeView *ret = new ChangeServeView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ChangeServeView::onEnter(){
    setTitleName("ChangeServer");
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void ChangeServeView::onExit(){
    PopupBaseView::onExit();
    setTouchEnabled(false);
}
bool ChangeServeView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BG", CCScale9Sprite*, m_BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_container", CCNode*, m_container);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3", CCControlButton*, m_btn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4", CCControlButton*, m_btn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5", CCControlButton*, m_btn5);
    return true;
}

SEL_CCControlHandler ChangeServeView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn1", ChangeServeView::onClickBtn1);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn2", ChangeServeView::onClickBtn1);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn3", ChangeServeView::onClickBtn1);
    return NULL;
}

bool ChangeServeView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        auto bg = CCBLoadFile("ChangeSeverView", this, this);
        changeBGHeight(m_BG);
        changeBGHeight(m_container);
        this->setContentSize(bg->getContentSize());
        CCSize viewSize = m_container->getContentSize();
        m_container->removeAllChildren();
        
        info_name = "";
        info_ip = "";
        info_zone = "";
        info_port = SERVER_PORT;
        info_gameUid = "";
        m_celeted = -1;
        
        setData();
        
        float y = m_BG->getContentSize().height - 30;
        //IP
        m_titleIp = CCLabelIF::create("IP");
        m_titleIp->setPosition(ccp(170, y));
        m_titleIp->setAnchorPoint(ccp(1,0.5));
        m_titleIp->setAlignment(kCCTextAlignmentRight);
        m_container->addChild(m_titleIp);
        m_ip = CCEditBox::create(CCSizeMake(400,45), CCLoadSprite::createScale9Sprite("UI_chat_text.png"));
        m_ip->setMaxLength(15);
        m_ip->setFontColor(ccc3(0, 0, 0));
        m_ip->setFontSize(22);
        m_ip->setReturnType(kKeyboardReturnTypeDone);
        m_ip->setPosition(ccp(viewSize.width*0.6, y));
        m_ip->setText("");
        m_container->addChild(m_ip);

        //PORT
        y -= 50;
        m_titlePort = CCLabelIF::create("PORT");
        m_titlePort->setPosition(ccp(170, y));
        m_titlePort->setAnchorPoint(ccp(1,0.5));
        m_titlePort->setAlignment(kCCTextAlignmentRight);
        m_container->addChild(m_titlePort);
        m_port = CCEditBox::create(CCSizeMake(400,45), CCLoadSprite::createScale9Sprite("UI_chat_text.png"));
        m_port->setMaxLength(4);
        m_port->setFontColor(ccc3(0, 0, 0));
        m_port->setFontSize(22);
        m_port->setReturnType(kKeyboardReturnTypeDone);
        m_port->setPosition(ccp(viewSize.width*0.6, y));
        m_port->setText("");
        m_container->addChild(m_port);

        //ZONE
        y -= 50;
        m_titleZone = CCLabelIF::create("ZONE");
        m_titleZone->setPosition(ccp(170, y));
        m_titleZone->setAnchorPoint(ccp(1,0.5));
        m_titleZone->setAlignment(kCCTextAlignmentRight);
        m_container->addChild(m_titleZone);
        m_zone = CCEditBox::create(CCSizeMake(400,45), CCLoadSprite::createScale9Sprite("UI_chat_text.png"));
        m_zone->setMaxLength(15);
        m_zone->setFontColor(ccc3(0, 0, 0));
        m_zone->setFontSize(22);
        m_zone->setReturnType(kKeyboardReturnTypeDone);
        m_zone->setPosition(ccp(viewSize.width*0.6, y));
        m_zone->setText("");
        m_container->addChild(m_zone);
        
        //GAME UID
        y -= 50;
        m_titleGameUid = CCLabelIF::create("GAME UID");
        m_titleGameUid->setPosition(ccp(170, y));
        m_titleGameUid->setAnchorPoint(ccp(1,0.5));
        m_titleGameUid->setAlignment(kCCTextAlignmentRight);
        m_container->addChild(m_titleGameUid);
        m_gameUid = CCEditBox::create(CCSizeMake(400,45), CCLoadSprite::createScale9Sprite("UI_chat_text.png"));
        m_gameUid->setPlaceHolder("GAME UID");
        m_gameUid->setMaxLength(20);
        m_gameUid->setFontColor(ccc3(0, 0, 0));
        m_gameUid->setFontSize(22);
        m_gameUid->setReturnType(kKeyboardReturnTypeDone);
        m_gameUid->setPosition(ccp(viewSize.width*0.6, y));
        m_gameUid->setText("");
        m_container->addChild(m_gameUid);
        
        //btn change
        y -= 65;
        m_btnChange = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_green3.png"));
        m_btnChange->setPreferredSize(CCSize(240,70));
        CCCommonUtils::setButtonTitle(m_btnChange, "change");
        m_btnChange->addTargetWithActionForControlEvents(this, cccontrol_selector(ChangeServeView::onChangeClick), CCControlEventTouchUpInside);
        m_btnChange->setPosition(ccp(viewSize.width/2, y - 40));
        m_container->addChild(m_btnChange);
        
        //cur
        y -= 80;
        //setCurServer();
        m_curServer = CCLabelIF::create("");
        m_curIp = CCLabelIF::create(CCString::createWithFormat("curIP:%s",CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP).c_str())->getCString());
        m_curPort = CCLabelIF::create(CCString::createWithFormat("curPort:%d",CCUserDefault::sharedUserDefault()->getIntegerForKey(ACCOUNT_PORT))->getCString());
        m_curZone = CCLabelIF::create(CCString::createWithFormat("curZone:%s",CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_ZONE).c_str())->getCString());
        m_curGameUid = CCLabelIF::create(CCString::createWithFormat("curGameUid:%s",CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID).c_str())->getCString());
        m_curServer->setAnchorPoint(ccp(0,0.5));
        m_curIp->setAnchorPoint(ccp(0,0.5));
        m_curPort->setAnchorPoint(ccp(0,0.5));
        m_curZone->setAnchorPoint(ccp(0,0.5));
        m_curGameUid->setAnchorPoint(ccp(0,0.5));
        m_curServer->setPosition(ccp(200, y-40));
        m_curIp->setPosition(ccp(200, y-80));
        m_curPort->setPosition(ccp(200, y-120));
        m_curZone->setPosition(ccp(200, y-160));
        m_curGameUid->setPosition(ccp(200, y-200));
        m_container->addChild(m_curServer);
        m_container->addChild(m_curIp);
        m_container->addChild(m_curPort);
        m_container->addChild(m_curZone);
        m_container->addChild(m_curGameUid);
        
        //tableView
        y -= 240;
        m_serverList = CCNode::create();
        m_serverList->setContentSize(CCSize(640, y));
        m_serverList->setAnchorPoint(CCPointZero);
        m_container->addChild(m_serverList);
//        showTableView();
        ret = true;
    }
    return ret;
}

void ChangeServeView::onChangeClick(CCObject *pSender, CCControlEvent event){
    info_ip = m_ip->getText();
    info_zone = m_zone->getText();
    info_port = atoi(m_port->getText());
    info_gameUid = m_gameUid->getText();

    if(info_gameUid == ""){
        int index = 0;
        while (index < m_serverGroupVec.size()) {
            auto info = m_serverGroupVec[index];
            if(info->ip == info_ip){
                info_serverId = info->serverId;
                break;
            }
            index++;
        }
        GMCommand *cmd = new GMCommand(info_ip, info_zone, info_port, info_gameUid, info_serverId);
        cmd->sendAndRelease();
        return;
    }
    
    CCLOG(">>> %s", info_ip.c_str());
    CCLOG(">>> %d", info_port);
    CCLOG(">>> %s", info_zone.c_str());
    CCLOG(">>> %s", info_gameUid.c_str());
    if(FBUtilies::fbIsLogin()){//如果fb是登录状态的token 要把它清掉
        FBUtilies::fbLogout();
        CCUserDefault::sharedUserDefault()->setIntegerForKey(FUN_BUILD_MAIN_LEVEL, 0);
        CCUserDefault::sharedUserDefault()->flush();
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, info_ip);
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, info_zone);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, info_port);
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, info_gameUid);
    
    NetController::shared()->setIp(info_ip);
    NetController::shared()->setZone(info_zone);
    NetController::shared()->setPort(info_port);
    
    NetController::shared()->setIsLogout(true);
    NetController::shared()->disconnect();
    
    LogoutCommand* cmd = new LogoutCommand();
    CCDictionary* _params = CCDictionary::create();
    _params->setObject(CCString::create(info_zone), "zoneName");
    _params->setObject(CCString::create("true"), "isReLogin");
    CCDictionary* dict = CCDictionary::create();
    dict->setObject(CCString::create("logout"), "cmd");
    dict->setObject(_params, "params");
    GmController::getInstance()->isGmLogin = 1;
    cmd->handleRecieve(dict);
    
}

bool ChangeServeView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPoint = pTouch->getLocation();
    return true;
}
void ChangeServeView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (fabs(m_touchPoint.y - pTouch->getLocation().y)<10) {
        int num = m_buttonNodes.size();
        for (int i = 0; i<num; i++) {
            CCCommonUtils::setButtonSprite(m_buttonNodes[i], "btn_04grey.png");
            if(isTouchInside(m_buttonNodes[i], pTouch)){
                m_celeted = i;
                m_ip->setText(m_serverGroupVec[i]->ip.c_str());
                m_port->setText(CC_ITOA(m_serverGroupVec[i]->port));
                m_zone->setText(m_serverGroupVec[i]->zone.c_str());
            }
        }
        if(m_celeted!=-1)
            CCCommonUtils::setButtonSprite(m_buttonNodes[m_celeted], "btn_04.png");
    }
}

void ChangeServeView::setCurServer(){
    string curServerIp = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP).c_str();
    string server_name = "";
    int num = m_serverGroupVec.size();
    for (int i=0; i<num; i++) {
        if(m_serverGroupVec[i]->ip == curServerIp){
            server_name = m_serverGroupVec[i]->name;
            break;
        }
    }
    if (m_curServer) {
        m_curServer->setString(CCString::createWithFormat("curServer:%s",server_name.c_str())->getCString());
    }
    else{
        m_curServer = CCLabelIF::create(CCString::createWithFormat("curServer:%s",server_name.c_str())->getCString());
    }
}

void ChangeServeView::tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell){
}
cocos2d::CCSize ChangeServeView::cellSizeForTable(  cocos2d::extension::TableView *table){
    return CCSizeMake(640, 70*ceil(m_serverGroupVec.size()/3.0));
}

CCTableViewCell* ChangeServeView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    if(idx >= 1){
        return NULL;
    }
    
    CCTableViewCell* cell = table->dequeueCell();
    if(cell){
        
    }else{
        cell = new CCTableViewCell();
        cell->autorelease();
        int num = m_buttonNodes.size();
        for (int i=0; i<num; ++i) {
            
            float x = 106 + 213*(i%3);
            float y = 70*ceil(num/3.0) - 35 - i/3*70;
            m_buttonNodes[i]->setPosition(x, y);
            
            cell->addChild(m_buttonNodes[i]);
        }
    }
    
    
    return cell;
}

ssize_t ChangeServeView::numberOfCellsInTableView(  cocos2d::extension::TableView *table){
    return 1;
}

//unsigned int ChangeServeView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
//    return 3;
//}
void ChangeServeView::setData(){
    CCHttpRequest* request = new CCHttpRequest();
    string url;
#if COCOS2D_DEBUG == 1
        url = CCString::createWithFormat("http://10.1.4.106/gameservice/getserverlist.php?gmFlag=%d", GlobalData::shared()->playerInfo.gmFlag)->getCString();
#elif COCOS2D_DEBUG == 2
        url = CCString::createWithFormat("http://10.1.4.121/gameservice/getserverlist.php?gmFlag=%d", GlobalData::shared()->playerInfo.gmFlag)->getCString();
#else
        url = CCString::createWithFormat("http://p1.cok.elexapp.com/gameservice/getserverlist.php?gmFlag=%d", GlobalData::shared()->playerInfo.gmFlag)->getCString();
#endif
    CCLOG("change URL: %s",url.c_str());
    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(ChangeServeView::onGetServerList));
    request->setTag("error_report");
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void ChangeServeView::onGetServerList(CCHttpClient* client, CCHttpResponse* response)
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
    int size = Json_getSize(serverList);
    
    for (int i=0; i<size; i++) {
        
        Json *c2 = Json_getItemAt(serverList, i);
        string port = Json_getString(c2, "port","-1");
        string name = Json_getString(c2, "name","-1");
        string zone = Json_getString(c2, "zone","");
        string ip = Json_getString(c2, "ip","");
        string serverId = Json_getString(c2, "id", "-1");
        
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::create(port), "port");
        dict->setObject(CCString::create(name), "name");
        dict->setObject(CCString::create(zone), "zone");
        dict->setObject(CCString::create(ip), "ip");
        dict->setObject(CCString::create(serverId), "id");
        
        ServerGroup* group = new ServerGroup(dict);
        m_serverGroupVec.push_back(group);
        createBtns(i, dict);
    }
    Json_dispose(serverList);
    showTableView();
    setCurServer();
}

void ChangeServeView::createBtns(int idx, CCDictionary *dic){
    CCLabelIF* title = CCLabelIF::create(m_serverGroupVec[idx]->name.c_str(), 24);
    CCControlButton* btn = CCControlButton::create(title, CCLoadSprite::createScale9Sprite("btn_04grey.png"));
//    btn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_04grey.png"), CCControlStateDisabled);
//    btn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_04grey.png"), CCControlStateHighlighted);
//    btn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_04grey.png"), CCControlStateSelected);
    btn->setPreferredSize(CCSize(200,60));
    btn->setTouchPriority(5);
    //    float x = 120 + 200*(idx%3) ;
    //    float y = m_container->getContentSize().height - 30 - 70*((idx)/3);
    //    btn->setPosition(x, y);
    //    m_container->addChild(btn);
    m_buttonNodes.push_back(btn);
    btn->setEnabled(true);
    btn->setSwallowsTouches(false);
}
void ChangeServeView::showTableView()
{
    m_tableView = CCTableView::create(this, m_serverList->getContentSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setDelegate(this);
    m_serverList->addChild(m_tableView);
    m_tableView->reloadData();
}

//===============
ServerGroup::ServerGroup(CCDictionary* dict)
{
    const CCString *pStr = dict->valueForKey("id");

    pStr = dict->valueForKey("name");
    if(pStr->compare("")!=0){
        name=pStr->getCString();
    }else{
        name = "";
    }

    pStr = dict->valueForKey("zone");
    if(pStr->compare("")!=0){
        zone=pStr->getCString();
    }else{
        zone = "";
    }
    
    pStr = dict->valueForKey("ip");
    if(pStr->compare("")!=0){
        ip=pStr->getCString();
    }else{
        ip = "";
    }
    
    pStr = dict->valueForKey("port");
    if(pStr->compare("")!=0){
        port=pStr->intValue();
    }else{
        port = -1;
    }
    
    pStr = dict->valueForKey("id");
    if(pStr->compare("")!=0){
        serverId=pStr->intValue();
    }else{
        serverId = -1;
    }
}
