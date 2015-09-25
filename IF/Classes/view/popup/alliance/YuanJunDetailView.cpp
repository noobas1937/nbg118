//
//  YuanJunDetailView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-12.
//
//

#include "YuanJunDetailView.h"
#include "PopupViewController.h"
#include "SoundController.h"
#include "AllianceInfoMembersView.h"
#include "ChangeAllianceAttributesCommand.h"
#include "MailWritePopUpView.h"
#include "AllianceInfoMembersView.h"
#include "AllianceMarketView.h"
#include "FunBuildController.h"
#include "AllianceHelpView.h"
#include "ChangeAllianceFlagView.h"
#include "AllianceRallyListCommand.h"
#include "YuanYunReturnCommand.h"

#define  MSG_YUAN_JUN_CELL_CLICK       "msg_yuan_jun_cell_click"
#define  MSG_REMOVE_ONE_YUAN_JUN_CELL       "msg_remove_one_yuan_jun_cell"

YuanJunDetailView* YuanJunDetailView::create(){
    YuanJunDetailView* ret = new YuanJunDetailView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool YuanJunDetailView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    auto tmpCCB = CCBLoadFile("YuanJunDetailView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int preH = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int newH = m_viewBg->getContentSize().height;
        int addH = newH- preH;
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height+addH));
        m_infoList->setPositionY(m_infoList->getPositionY()-addH);
    }
    
    m_data = CCArray::create();
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    

    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_STABLE);
    if (buildId>0) {
        FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(buildId);
        if(&info!=NULL){
            m_maxSoldier = atoi(info.para[3].c_str())+CCCommonUtils::getEffectValueByNum(141);
            
        }
    }
    std::string numStr = "0/";
    numStr.append(CC_ITOA(m_maxSoldier));
    m_numTxt->setString(numStr);
    m_tip->setString(_lang("115173"));
    AllianceRallyListCommand* cmd = new AllianceRallyListCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(YuanJunDetailView::getServerData), NULL));
    cmd->sendAndRelease();
    
    return true;
}

void YuanJunDetailView::getServerData(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        m_currentSoldier = dic->valueForKey("forces")->intValue();
        std::string numStr = CC_CMDITOA(m_currentSoldier);
        numStr.append("/");
        numStr.append(CC_CMDITOA(m_maxSoldier).c_str());
        m_numTxt->setString(numStr);
        CCArray* array = dynamic_cast<CCArray*>(dic->objectForKey("list"));
        if(array){
            int num = array->count();
            for (int i=0; i<num; i++) {
                auto yuan = _dict(array->objectAtIndex(i));
                if(yuan){
                    YuanJunInfo* info = new YuanJunInfo();
                    info->parseInfo(yuan);
                    m_data->addObject(info);
                    info->release();
                }
            }
            m_tip->setVisible(num<=0);
        }else{
            m_tip->setVisible(true);
        }
    }
    m_tabView->reloadData();
}

CCSize YuanJunDetailView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    YuanJunInfo* obj = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(idx));
    if (obj->getOpen()) {
        int num = obj->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        if (CCCommonUtils::isIosAndroidPad()) {
            int addH = row*200;
            return CCSize(1440, 225+addH+100);
        }
        else {
            int addH = row*100;
            return CCSize(604, 120+addH+60);
        }
    }
    
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1440, 225);
    }
    else
        return CCSize(604, 115);
}
CCSize YuanJunDetailView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1440, 225);
    }
    else
        return CCSize(604, 115);
}

CCTableViewCell* YuanJunDetailView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    YuanJunDetailCell* cell = (YuanJunDetailCell*)table->dequeueCell();
    YuanJunInfo* obj = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData(obj);
        }else{
            cell = YuanJunDetailCell::create(obj);
        }
    }
    return cell;
}

ssize_t YuanJunDetailView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void YuanJunDetailView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void YuanJunDetailView::onEnter()
{
    CCNode::onEnter();
    this->setTitleName(_lang("108728"));
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(YuanJunDetailView::cellClick), MSG_YUAN_JUN_CELL_CLICK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(YuanJunDetailView::removeOneCell), MSG_REMOVE_ONE_YUAN_JUN_CELL, NULL);
    
}

void YuanJunDetailView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_YUAN_JUN_CELL_CLICK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REMOVE_ONE_YUAN_JUN_CELL);
    CCNode::onExit();
}

void YuanJunDetailView::cellClick(CCObject* param){
    CCString* uuidObj = dynamic_cast<CCString*>(param);
    if(uuidObj && m_data){
        std::string uuid = uuidObj->getCString();
        int num = m_data->count();
        for (int i=0; i< num ;i++) {
            YuanJunInfo* info = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(i));
            if(info && info->getUuid()==uuid){
                info->setOpen(!info->getOpen());
                break;
            }
        }
    }
    m_tabView->reloadData();
}

void YuanJunDetailView::removeOneCell(CCObject* param){
    CCString* uuidObj = dynamic_cast<CCString*>(param);
    if (uuidObj && m_data) {
        for (int i=0; i<m_data->count(); i++) {
            YuanJunInfo* info = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(i));
            if(info && info->getUuid()==uuidObj->getCString()){
                m_currentSoldier -= info->getNum();
                m_data->removeObjectAtIndex(i);
                break;
            }
        }
        m_currentSoldier = MAX(m_currentSoldier, 0);
        std::string numStr = CC_ITOA(m_currentSoldier);
        numStr.append("/");
        numStr.append(CC_ITOA(m_maxSoldier));
        m_numTxt->setString(numStr);
        m_tabView->reloadData();
    }
}

SEL_CCControlHandler YuanJunDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    
    return NULL;
}

bool YuanJunDetailView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip", CCLabelIF*, this->m_tip);
    return false;
}

YuanJunDetailCell *YuanJunDetailCell::create(YuanJunInfo* info){
    YuanJunDetailCell *ret = new YuanJunDetailCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void YuanJunDetailCell::setData(YuanJunInfo* info){
    
    m_info = info;
    m_icon->removeAllChildrenWithCleanup(true);
    m_playerName->setString(m_info->getName());
    m_powerTxt->setString(_lang_1("115029",CC_CMDITOA(m_info->getPower()).c_str()));
    m_numTxt->setString(_lang_1("115156", CC_CMDITOA(m_info->getNum()).c_str()));
    m_sendHomeBtn->setVisible(false);
    m_arrow->setRotation(m_info->getOpen()?90:0);
    m_soldierNode->removeAllChildrenWithCleanup(true);
    std::string useArmyId = "";
    int num = m_info->getSoldiers()->count();
    int maxCount = 0;
    int count = 0;
    for(int i=0; i<num; i++){
        auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
        count = dic->valueForKey("count")->intValue();
        std::string armyId = dic->valueForKey("armyId")->getCString();
        if(count>maxCount){
            maxCount = count;
            useArmyId = armyId;
        }
    }
    if(m_info->getOpen()){
        num = m_info->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        int addH = 0;
        if (CCCommonUtils::isIosAndroidPad()) {
            addH = row*200 + 100;
        }
        else
            addH = row*100 + 60;
        m_moveNode->setPosition(ccp(0, addH));
        m_soldierNode->setPosition(ccp(0, addH));
        maxCount = 0;
        for (int i=0; i<num; i++) {
            auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
            YuanJunSoldierCell* cell = YuanJunSoldierCell::create(dic);
            int rowIndex = i/2;
            int col = i%2;
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(col==0?20:818, -200-rowIndex*200));
            }
            else
                cell->setPosition(ccp(col==0?10:330, -100-rowIndex*100));
            m_soldierNode->addChild(cell);
            count = dic->valueForKey("count")->intValue();
            std::string armyId = dic->valueForKey("armyId")->getCString();
            if(count>maxCount){
                maxCount = count;
                useArmyId = armyId;
            }
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            m_bg->setContentSize(CCSize(1440, 225+addH));
        }
        else
            m_bg->setContentSize(CCSize(604,110+addH));
        if (num>0) {
            m_sendHomeBtn->setVisible(true);
        }
    }else{
        m_sendHomeBtn->setVisible(false);
        m_moveNode->setPosition(ccp(0, 0));
        m_soldierNode->setPosition(ccp(0, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_bg->setContentSize(CCSize(1440,225));
        }
        else
            m_bg->setContentSize(CCSize(604,110));
    }
    if(m_info->getGenerals() && m_info->getGenerals()->count()>0){
        std::string head = m_info->getPic();
        head.append(".png");
        m_icon->removeAllChildrenWithCleanup(true);
        CCSprite* headSpr = CCLoadSprite::createSprite(head.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(headSpr, 190);
        }
        else
            CCCommonUtils::setSpriteMaxSize(headSpr, 88);
        m_icon->addChild(headSpr);
    }else{
        m_icon->removeAllChildrenWithCleanup(true);
        std::string icon = "ico" + useArmyId+ "_small.png";
        CCSprite* headSpr = CCLoadSprite::createSprite(icon.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(headSpr, 190);
        }
        else
            CCCommonUtils::setSpriteMaxSize(headSpr, 88);
        m_icon->addChild(headSpr);
    }
}

void YuanJunDetailCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void YuanJunDetailCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_pic_click");
    CCNode::onExit();
}

bool YuanJunDetailCell::init(){
    auto bg = CCBLoadFile("YuanJunDetailCell", this, this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCSize(1440, 225));
    }
    else
        this->setContentSize(bg->getContentSize());
    CCCommonUtils::setButtonTitle(m_sendHomeBtn, _lang("115157").c_str());
    setData(m_info);
    return true;
}

bool YuanJunDetailCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickNode, pTouch)){
        return true;
    }
    return false;
}

void YuanJunDetailCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(isTouchInside(m_clickNode, pTouch) && fabsf(end.y-m_touchPos.y)<30){
        if(isTouchInside(m_sendHomeBtn, pTouch) && m_sendHomeBtn->isVisible()){
            return ;
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_YUAN_JUN_CELL_CLICK,CCString::create(m_info->getUuid()));
    }
}

void YuanJunDetailCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void YuanJunDetailCell::clickHead(CCObject* data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        std::string clickPic = str->getCString();
    }
}

void YuanJunDetailCell::onSendHomeClick(CCObject *pSender, CCControlEvent event){
    YuanYunReturnCommand* cmd = new YuanYunReturnCommand(m_info->getUuid());
    cmd->sendAndRelease();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_ONE_YUAN_JUN_CELL,CCString::create(m_info->getUuid()));
    m_sendHomeBtn->setEnabled(false);
}


bool YuanJunDetailCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerName", CCLabelIF*, this->m_playerName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCNode*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sendHomeBtn", CCControlButton*, this->m_sendHomeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_soldierNode", CCNode*, this->m_soldierNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    return false;
}

SEL_CCControlHandler YuanJunDetailCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendHomeClick", YuanJunDetailCell::onSendHomeClick);
    return NULL;
}

//
YuanJunSoldierCell *YuanJunSoldierCell::create(CCDictionary* info){
    YuanJunSoldierCell *ret = new YuanJunSoldierCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void YuanJunSoldierCell::onEnter(){
    CCNode::onEnter();
}

void YuanJunSoldierCell::onExit(){
    CCNode::onExit();
}

bool YuanJunSoldierCell::init(){
    auto bg = CCBLoadFile("YuanJunSoldierCell", this, this);
    this->setContentSize(bg->getContentSize());
    std::string armyId = m_info->valueForKey("armyId")->getCString();
    std::string name = CCCommonUtils::getNameById(armyId);
    std::string icon = "ico" + armyId+ "_small.png";
    std::string count = m_info->valueForKey("count")->getCString();
    
    string path = armyId.substr(armyId.size()-2);
    int num  = atoi(path.c_str())+1;
    path = "Roman_";
    path.append(CC_ITOA(num));
    path.append(".png");
    auto pic  = CCLoadSprite::createSprite(path.c_str());
    
    m_icon->removeAllChildrenWithCleanup(true);
    m_nameTxt->setString(name);
    m_numTxt->setString(CC_CMDITOA(atoi(count.c_str())));
    CCSprite* spr = CCLoadSprite::createSprite(icon.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        spr->setScale(1.4);
    }
    else
        spr->setScale(0.7);
    m_icon->addChild(spr);
    m_icon->addChild(pic);
    pic->setPositionY(-30);
    return true;
}


bool YuanJunSoldierCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    return false;
}

SEL_CCControlHandler YuanJunSoldierCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
