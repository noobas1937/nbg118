//
//  KingdomAppointView.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-10.
//
//

#include "KingdomAppointView.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"

#define CLICK_OFFICALS_CELL_EVNET "click_officals_cell_event"
#define REFRESH_OFFICALS_CELL_DATA "refres_officals_cell_data"

static int m_clickId = 0;

KingdomAppointView *KingdomAppointView::create(string playerId,string playerName){
    KingdomAppointView *ret = new KingdomAppointView(playerId,playerName);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void KingdomAppointView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("110069"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(KingdomAppointView::clickCellHandle), CLICK_OFFICALS_CELL_EVNET, NULL);
}

void KingdomAppointView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CLICK_OFFICALS_CELL_EVNET);
    PopupBaseView::onExit();
}

void KingdomAppointView::clickCellHandle(CCObject* param){
    CCInteger* intObj = dynamic_cast<CCInteger*>(param);
    if(intObj){
        m_currentIndex = intObj->getValue();
    }
    this->btnHandle();
}

void KingdomAppointView::btnHandle(){
    int num = m_srcData->count();
    bool flag = false;
    for (int i=0; i<num; i++) {
        OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_srcData->objectAtIndex(i));
        if(info && m_currentIndex==info->id){
            m_appointBtn->setVisible(info->uid=="");
        }
        if(info && info->uid==m_playerId){
            flag = true;
        }
    }
    if (m_currentIndex==-1) {
        m_appointBtn->setEnabled(false);
        m_removeBtn->setEnabled(false);
        m_appointBtn->setVisible(true);
        m_removeBtn->setVisible(false);
    }else{
        m_appointBtn->setEnabled(!flag);
        m_removeBtn->setEnabled(!m_appointBtn->isVisible());
        m_removeBtn->setVisible(!m_appointBtn->isVisible());
    }
}

bool KingdomAppointView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void KingdomAppointView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

bool KingdomAppointView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//        int curHight = -500;
//        while (curHight<maxHight) {
//            auto bg = CCLoadSprite::createSprite("technology_09.png");
//            bg->setAnchorPoint(ccp(0, 1));
//            bg->setPosition(ccp(0, curHight));
//            if (CCCommonUtils::isIosAndroidPad()) {
//                bg->setScaleX(2.4);
//            }
//            curHight += bg->getContentSize().height;
//            tBatchNode->addChild(bg);
//        }
//        this->addChild(tBatchNode);
        
        CCLoadSprite::doResourceByCommonIndex(310, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(310, false);
        });
        
        auto tmpCCB = CCBLoadFile("KingdomAppointView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else {
            this->setContentSize(tmpCCB->getContentSize());
            int prevH = m_viewBg->getContentSize().height;
            changeBGHeight(m_viewBg);
            int add = m_viewBg->getContentSize().height - prevH;
            m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height+add));
            m_infoList->setPositionY(m_infoList->getPositionY()-add);
            m_appointBtn->setPositionY(m_appointBtn->getPositionY()-add);
            m_removeBtn->setPositionY(m_removeBtn->getPositionY()-add);
        }
        m_viewBg->setVisible(false);
        
        m_data = CCArray::create();
        m_srcData = CCArray::create();
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        m_tab1->setEnabled(false);
        m_tab2->setEnabled(true);
        CCCommonUtils::setButtonTitle(m_tab1, _lang("110026").c_str());
        CCCommonUtils::setButtonTitle(m_tab2, _lang("110027").c_str());
        CCCommonUtils::setButtonTitle(m_appointBtn, _lang("110064").c_str());
        CCCommonUtils::setButtonTitle(m_removeBtn, _lang("110068").c_str());
        
        CCDictElement *element;
        auto dictSK = LocalController::shared()->DBXMLManager()->getGroupByKey("office");
        CCDICT_FOREACH(dictSK, element){
            std::string id = element->getStrKey();
            OfficeInfo* info = OfficeInfo::create();
            auto tempDic = _dict(element->getObject());
            info->parse(tempDic);
            m_srcData->addObject(info);
        }
        
        for (int i=1; i<9; i++) {
            m_data->addObject(m_srcData->objectAtIndex(i));
        }
        m_currentIndex = -1;
        m_clickId = -1;
        this->refreshData();
        
        KingdomOfficersCommand* cmd = new KingdomOfficersCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(KingdomAppointView::getOfficersData), NULL));
        cmd->sendAndRelease();
        
        ret = true;
    }
    return ret;
}

void KingdomAppointView::getOfficersData(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* officers = dynamic_cast<CCArray*>(dic->objectForKey("officers"));
        int num = officers->count();
        for (int i=0; i<num; i++) {
            auto officerInfo = _dict(officers->objectAtIndex(i));
            int posId = officerInfo->valueForKey("positionId")->intValue();
            int oNum = m_srcData->count();
            for (int j=0; j<oNum; j++) {
                OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_srcData->objectAtIndex(j));
                if (posId==info->id) {
                    info->playerName = officerInfo->valueForKey("name")->getCString();
                    string uid = officerInfo->valueForKey("uid")->getCString();
                    info->uid = uid;
                    info->playerIcon = officerInfo->valueForKey("pic")->getCString();
                    info->abbr = officerInfo->valueForKey("abbr")->getCString();
                    info->chNameFlag = officerInfo->valueForKey("chNameFlag")->boolValue();
                    info->time = officerInfo->valueForKey("time")->doubleValue();
                    info->picVer = officerInfo->valueForKey("picVer")->intValue();
                    break;
                }
            }
        }
        this->refreshData();
    }
}

void KingdomAppointView::refreshData(){
    m_tabView->reloadData();
    this->btnHandle();
}

bool KingdomAppointView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab1", CCControlButton*, this->m_tab1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab2", CCControlButton*, this->m_tab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_appointBtn", CCControlButton*, this->m_appointBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_removeBtn", CCControlButton*, this->m_removeBtn);
    return false;
}

SEL_CCControlHandler KingdomAppointView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab1", KingdomAppointView::onClickTab1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab2", KingdomAppointView::onClickTab2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAppointBtn", KingdomAppointView::onClickAppointBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRemoveAppointedBtn", KingdomAppointView::onRemoveAppointedBtn);
    return NULL;
}

void KingdomAppointView::onClickTab1(CCObject *pSender, CCControlEvent event){
    m_tab1->setEnabled(false);
    m_tab2->setEnabled(true);
    m_currentIndex = -1;
    m_clickId = -1;
    m_data->removeAllObjects();
    for (int i=1; i<9; i++) {
        m_data->addObject(m_srcData->objectAtIndex(i));
    }
    this->refreshData();
}

void KingdomAppointView::onClickTab2(CCObject *pSender, CCControlEvent event){
    m_currentIndex = -1;
    m_clickId = -1;
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(false);
    m_data->removeAllObjects();
    for (int i=9; i<17; i++) {
        m_data->addObject(m_srcData->objectAtIndex(i));
    }
    this->refreshData();
}

void KingdomAppointView::onClickAppointBtn(CCObject *pSender, CCControlEvent event){
    int num = m_data->count();
    double leftTime = 0;
    for (int i=0; i<num; i++) {
        OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i));
        if(info && m_currentIndex==info->id && info->appointTime>0){
            leftTime = GlobalData::shared()->getWorldTime() - info->appointTime;
            break;
        }
    }
    if (leftTime > 0) {
        YesNoDialog::show(_lang_1("138159",CC_SECTOA(leftTime)),NULL);
        return ;
    }
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    if(m_currentIndex<0) return ;
    string oName = CCCommonUtils::getNameById(CC_ITOA(m_currentIndex));
    YesNoDialog::show(_lang_2("110083",m_playerName.c_str(),oName.c_str()), CCCallFunc::create(this, callfunc_selector(KingdomAppointView::appointedFun)));
}

void KingdomAppointView::appointedFun(){
    KingdomExecCommand* cmd = new KingdomExecCommand();
    cmd->putParam("type", CCInteger::create(APPOINT));
    cmd->putParam("uid", CCString::create(m_playerId));
    cmd->putParam("posId", CCString::create(CC_ITOA(m_currentIndex)));
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(KingdomAppointView::appointedSuccess), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void KingdomAppointView::onRemoveAppointedBtn(CCObject *pSender, CCControlEvent event){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    if(m_currentIndex<0) return ;
    int num = m_data->count();
    string pName = "";
    for (int i=0; i<num; i++) {
        OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i));
        if(info && m_currentIndex==info->id){
            pName = info->playerName;
            break;
        }
    }
    string oName = CCCommonUtils::getNameById(CC_ITOA(m_currentIndex));
    YesNoDialog::show(_lang_2("110084",pName.c_str(),oName.c_str()), CCCallFunc::create(this, callfunc_selector(KingdomAppointView::removeAppointedFun)));
}

void KingdomAppointView::appointedSuccess(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        string uid = dic->valueForKey("uid")->getCString();
        string pic = dic->valueForKey("pic")->getCString();
        string name = dic->valueForKey("name")->getCString();
        int posId = dic->valueForKey("posId")->intValue();
        string abbr = dic->valueForKey("abbr")->getCString();
        for (int i=0; i<m_data->count(); i++) {
            OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i));
            if(info->id==posId){
                info->uid = uid;
                info->playerIcon = pic;
                info->playerName = name;
                info->abbr = abbr;
                break;
            }
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REFRESH_OFFICALS_CELL_DATA,CCInteger::create(m_currentIndex));
    this->btnHandle();
}

void KingdomAppointView::removeAppointedFun(){
    int num = m_data->count();
    string uid = "";
    for (int i=0; i<num; i++) {
        OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i));
        if(info && m_currentIndex==info->id){
            uid = info->uid;
            break;
        }
    }
    
    if (uid!="") {
        KingdomExecCommand* cmd = new KingdomExecCommand();
        cmd->putParam("type", CCInteger::create(FIRE));
        cmd->putParam("uid", CCString::create(uid));
        cmd->putParam("posId", CCString::create(CC_ITOA(m_currentIndex)));
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(KingdomAppointView::removeAppointedSuccess), NULL));
        cmd->sendAndRelease();
        m_removeBtn->setEnabled(false);
        GameController::getInstance()->showWaitInterface();
    }
}

void KingdomAppointView::removeAppointedSuccess(CCObject* param){
    int num = m_data->count();
    for (int i=0; i<num; i++) {
        OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i));
        if(info && m_currentIndex==info->id){
            info->uid = "";
            info->playerIcon = "";
            info->playerName = "";
            break;
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REFRESH_OFFICALS_CELL_DATA,CCInteger::create(m_currentIndex));
    this->btnHandle();
}

CCSize KingdomAppointView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 432);
    }
    return CCSize(640, 180);
}

CCSize KingdomAppointView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 432);
    }
    return CCSize(640, 180);
}

CCTableViewCell* KingdomAppointView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    KingdomAppointCell* cell = (KingdomAppointCell*)table->dequeueCell();
    OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(idx));
    if(cell){
        cell->setData(info);
    }else{
        cell = KingdomAppointCell::create(info,m_infoList);
    }
    return cell;
}

ssize_t KingdomAppointView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void KingdomAppointView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{

}

//
KingdomAppointCell *KingdomAppointCell::create(OfficeInfo* info,CCNode* showArea){
    KingdomAppointCell *ret = new KingdomAppointCell(info,showArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool KingdomAppointCell::init(){
    auto node = CCBLoadFile("KingdomAppointCell", this, this);
    this->setContentSize(node->getContentSize());
    m_headImgNode = HFHeadImgNode::create();
    this->setData(m_info);
    return true;
}

void KingdomAppointCell::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(KingdomAppointCell::clickCellHandle), CLICK_OFFICALS_CELL_EVNET, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(KingdomAppointCell::refreshCell), REFRESH_OFFICALS_CELL_DATA, NULL);
    
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void KingdomAppointCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CLICK_OFFICALS_CELL_EVNET);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, REFRESH_OFFICALS_CELL_DATA);
    CCNode::onExit();
}

void KingdomAppointCell::clickCellHandle(CCObject* param){
    CCInteger* intObj = dynamic_cast<CCInteger*>(param);
    if(intObj){
        m_clickId = intObj->getValue();
        m_selectNode->setVisible(m_info->id==m_clickId);
    }
}

void KingdomAppointCell::refreshCell(CCObject* param){
    CCInteger* intObj = dynamic_cast<CCInteger*>(param);
    if(intObj && intObj->getValue()==m_info->id){
        this->setData(m_info);
    }
}

void KingdomAppointCell::setData(OfficeInfo* info){
    m_info = info;
    if(m_info==NULL) return ;
    m_selectNode->setVisible(m_clickId==m_info->id);
    if(m_info->isServant){
        m_txt1->setColor({255,38,0});
        m_txt2->setColor({255,38,0});
        m_txt3->setColor({255,38,0});
    }else{
        m_txt1->setColor({165,249,0});
        m_txt2->setColor({165,249,0});
        m_txt3->setColor({165,249,0});
    }
    m_iconNode->removeAllChildrenWithCleanup(true);
    string iconStr = "";
    if(m_info->uid==""){
        m_nameTxt->setString(m_info->name.c_str());
        iconStr = m_info->icon + ".png";
    }else{
        string temp = "";
        if(m_info->abbr!=""){
            temp.append("(");
            temp.append(m_info->abbr);
            temp.append(")");
        }
        temp.append(m_info->playerName);
        if (temp.length()>14) {
            temp = CCCommonUtils::subStrByUtf8(temp,0,14);
            temp.append("...");
        }
        m_nameTxt->setString(temp.c_str());
        iconStr = m_info->playerIcon + ".png";
    }
    auto pic = CCLoadSprite::createSprite(iconStr.c_str());
    m_iconNode->addChild(pic);
    CCCommonUtils::setSpriteMaxSize(pic, 75);
    if (CCCommonUtils::isUseCustomPic(m_info->picVer))
    {
        m_headImgNode->initHeadImgUrl2(m_iconNode, CCCommonUtils::getCustomPicUrl(m_info->uid, m_info->picVer), 1.0f, 75, true);
    }
    
    int num = MIN(m_info->tips.size(),m_info->params.size());
    m_tipNode1->setVisible(false);
    m_tipNode2->setVisible(false);
    m_tipNode3->setVisible(false);
    switch (num) {
        case 1:
            m_txt1->setString(_lang_1(m_info->tips[0], m_info->params[0].c_str()));
            m_tipNode1->setVisible(true);
            break;
        case 2:
            m_txt1->setString(_lang_1(m_info->tips[0], m_info->params[0].c_str()));
            m_txt2->setString(_lang_1(m_info->tips[1], m_info->params[1].c_str()));
            m_tipNode1->setVisible(true);
            m_tipNode2->setVisible(true);
            break;
        case 3:
            m_txt1->setString(_lang_1(m_info->tips[0], m_info->params[0].c_str()));
            m_txt2->setString(_lang_1(m_info->tips[1], m_info->params[1].c_str()));
            m_txt3->setString(_lang_1(m_info->tips[2], m_info->params[2].c_str()));
            m_tipNode1->setVisible(true);
            m_tipNode2->setVisible(true);
            m_tipNode3->setVisible(true);
            break;
        default:
            break;
    }
}

bool KingdomAppointCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPos = pTouch->getLocation();
    return true;
}

void KingdomAppointCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    if(fabsf(pos.y - m_startPos.y)>30){
        return ;
    }
    if(isTouchInside(m_showArea, pTouch) && isTouchInside(m_clickArea, pTouch)){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CLICK_OFFICALS_CELL_EVNET,CCInteger::create(m_info->id));
    }
}

bool KingdomAppointCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectNode", CCSprite*, this->m_selectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtValue1", CCLabelIF*, this->m_txtValue1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtValue2", CCLabelIF*, this->m_txtValue2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtValue3", CCLabelIF*, this->m_txtValue3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode1", CCNode*, this->m_tipNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode2", CCNode*, this->m_tipNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode3", CCNode*, this->m_tipNode3);
    return false;
}

SEL_CCControlHandler KingdomAppointCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}