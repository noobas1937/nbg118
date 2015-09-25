//
//  ToolCreateView.cpp
//  IF
//
//  Created by 付彬 on 15/5/6.
//
//

#include "ToolCreateView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "QueueController.h"
#include "CCThreadManager.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "FunBuildInfo.h"
#include "UseToolView.h"
#include "SoundController.h"
#include "HelpshiftCocos2dx.h"
#include "VipUtil.h"
#include "WorldMapView.h"
#include "SceneController.h"
#include "ImperialScene.h"
#include "FunBuild.h"
#include "GuideController.h"
#include "EquipmentBagView.h"
#include "ConfirmDialogView.h"
#include "StoreBuyConfirmDialog.h"

ToolCreateView* ToolCreateView::create(){
    ToolCreateView* ret = new ToolCreateView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ToolCreateView::init()
{
    if (!ArcPopupBaseView::init(TYPE_POS_MID)) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_WORKSHOP);
    if (buildId==0) {
        return false;
    }
    auto& buildInfo = FunBuildController::getInstance()->getFunbuildById(buildId);
    int bdPara1 = atoi(buildInfo.para[1].c_str());
    int bdPara2 = atoi(buildInfo.para[2].c_str());
    
    startPt = ccp(0, 0);
    auto bg = CCBLoadFile("ToolCreateView",this,this);
    int extH = getExtendHeight();
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    m_flyNode->setVisible(false);
    
    int tmpPoxY = 70;
    if (extH/4 < tmpPoxY) {
        tmpPoxY = extH/4;
    }
    m_queueNode->setPositionY(m_queueNode->getPositionY()-tmpPoxY);
    
    m_buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_WORKSHOP);
    
    m_toolNodeVec.push_back(m_tool1Node);
    m_toolNodeVec.push_back(m_tool2Node);
    m_toolNodeVec.push_back(m_tool3Node);
    m_toolNodeVec.push_back(m_tool4Node);
    m_toolNodeVec.push_back(m_tool5Node);
    m_toolNodeVec.push_back(m_tool6Node);
    m_toolNodeVec.push_back(m_tool7Node);
    m_toolNodeVec.push_back(m_tool8Node);
    m_toolNodeVec.push_back(m_tool9Node);
    m_toolNodeVec.push_back(m_tool10Node);
    m_toolNodeVec.push_back(m_tool11Node);
    m_toolNodeVec.push_back(m_tool12Node);
    
    m_makeTools = ToolController::getInstance()->m_makeTools;
    for (int i=0; i<m_makeTools.size(); i++)
    {
        if (i<m_toolNodeVec.size())
        {
            CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
            string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_makeTools[i]));
            auto icon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
            icon->setPosition(ccp(58, 58));
            CCCommonUtils::setSpriteMaxSize(icon, 90);
            m_toolNodeVec[i]->addChild(icon);
        }
    }
    m_tipNode->setVisible(false);
    m_spdIcon->setVisible(false);
    
    m_cancelBtn[0]->setVisible(false);
    m_cancelBtn[1]->setVisible(false);
    m_cancelBtn[2]->setVisible(false);
    m_cancelBtn[3]->setVisible(false);
    m_cancelBtn[4]->setVisible(false);
    
    m_sumGold = 99999;
    m_oldSumTime = 1;
    m_qid = QID_MAX;
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
    if (tqid!=QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
        int TmpItemId = QueueController::getInstance()->getItemId(qInfo.key);
        m_curCreatingItemId = TmpItemId;
        m_qid = tqid;
        
        CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_curCreatingItemId));
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
        CCCommonUtils::setSpriteMaxSize(icon, 120);
        m_curIconNode->addChild(icon);
        
        CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
        CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
        CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
        icon->runAction(repeatForever);
        
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(m_curCreatingItemId);
        m_sumGold = toolInfo.crt_gold;
        m_oldSumTime = toolInfo.crt_cd;
        
        updateWaitCells(qInfo.allId);
    }
    //设置空位
    m_canUse[0]=true;
    m_canUse[1]=true;
    m_canUse[2]=true;
    m_canUse[3]=true;
    m_canUse[4]=true;
    int j=GlobalData::shared()->playerInfo.materialBackup-2;
    for (int i=4; i>j; i--) {
        if (i<0) {
            break;
        }
        auto lock_pic = CCLoadSprite::createSprite("iron_lock.png");
        m_waitInNode[i]->addChild(lock_pic);
        m_cancelBtn[i]->setVisible(false);
        m_canUse[i]=false;
    }
    
    onEnterFrame(0);
    _openNum = 0;
    m_curOptIdx = -1;
    m_willIconPath = "";
    m_waitInterface = NULL;
    m_buyTouchNode->setVisible(false);
    m_buyItemId=0;
    return true;
}

void ToolCreateView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ToolCreateView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ToolCreateView::updateCrtRet), MATETOOL_CREATE_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ToolCreateView::updateLockCell), "updateLockCell", NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ToolCreateView::setViewByMate), "setViewByMate", NULL);
    
    _openNum++;
    if (_openNum>1 && m_qid == QID_MAX) {
    }
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    
    string key = CCUserDefault::sharedUserDefault()->getStringForKey("firstOpenToolCreateView", "");
    if (key == "") {
        CCUserDefault::sharedUserDefault()->setStringForKey("firstOpenToolCreateView", "3120100");
        GuideController::share()->setGuide("3120100");
    }
}

void ToolCreateView::onExit(){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    GlobalData::shared()->isBind = false;
    UIComponent::getInstance()->showEquipOrBagBtn(0);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ToolCreateView::onEnterFrame), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MATETOOL_CREATE_END);
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "updateLockCell");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "setViewByMate");
    if(m_buyItemId!=0){
        PopupViewController::getInstance()->addPopupInView(EquipmentBagView::create());
        PopupViewController::getInstance()->addPopupInView(MateCreateView::create(m_buyItemId,0,0,0));
    }
    
    CCNode::onExit();
}

SEL_CCControlHandler ToolCreateView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelBtn1Click", ToolCreateView::onCancelBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelBtn2Click", ToolCreateView::onCancelBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelBtn3Click", ToolCreateView::onCancelBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelBtn4Click", ToolCreateView::onCancelBtn4Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelBtn5Click", ToolCreateView::onCancelBtn5Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBuyBtnClick", ToolCreateView::onBuyBtnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBoxBtnClick", ToolCreateView::onBoxBtnClick);
    return NULL;
}

bool ToolCreateView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, this->m_particleNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_makeNode", CCNode*, this->m_makeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_make2Node", CCNode*, this->m_make2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curIconNode", CCNode*, this->m_curIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lastTime", CCLabelIF*, this->m_lastTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spdIcon", CCSprite*, this->m_spdIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode", CCNode*, this->m_tipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBg", CCSprite*, this->m_tipBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipName", CCLabelIF*, this->m_tipName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTime", CCLabelIF*, this->m_tipTime);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNode", CCNode*, this->m_flyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool1Node", CCNode*, this->m_tool1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool2Node", CCNode*, this->m_tool2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool3Node", CCNode*, this->m_tool3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool4Node", CCNode*, this->m_tool4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool5Node", CCNode*, this->m_tool5Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool6Node", CCNode*, this->m_tool6Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool7Node", CCNode*, this->m_tool7Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool8Node", CCNode*, this->m_tool8Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool9Node", CCNode*, this->m_tool9Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool10Node", CCNode*, this->m_tool10Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool11Node", CCNode*, this->m_tool11Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tool12Node", CCNode*, this->m_tool12Node);
    
    //等待空位
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_queueNode", CCNode*, this->m_queueNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitNode1", CCNode*, this->m_waitNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitNode2", CCNode*, this->m_waitNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitNode3", CCNode*, this->m_waitNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitNode4", CCNode*, this->m_waitNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitNode5", CCNode*, this->m_waitNode[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitInNode1", CCNode*, this->m_waitInNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitInNode2", CCNode*, this->m_waitInNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitInNode3", CCNode*, this->m_waitInNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitInNode4", CCNode*, this->m_waitInNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitInNode5", CCNode*, this->m_waitInNode[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn1", CCControlButton*, this->m_cancelBtn[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn2", CCControlButton*, this->m_cancelBtn[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn3", CCControlButton*, this->m_cancelBtn[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn4", CCControlButton*, this->m_cancelBtn[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn5", CCControlButton*, this->m_cancelBtn[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_boxBtn", CCControlButton*, this->m_boxBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyTouchNode", CCNode*, this->m_buyTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBg", CCSprite*, this->m_buyBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buySprNode", CCNode*, this->m_buySprNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_price", CCLabelIF*, this->m_price);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCControlButton*, this->m_buyBtn);
    
    return false;
}

void ToolCreateView::onCancelBtn1Click(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    ToolController::getInstance()->cancelMakeTool(1);
}
void ToolCreateView::onCancelBtn2Click(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    ToolController::getInstance()->cancelMakeTool(2);
}
void ToolCreateView::onCancelBtn3Click(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    ToolController::getInstance()->cancelMakeTool(3);
}
void ToolCreateView::onCancelBtn4Click(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    ToolController::getInstance()->cancelMakeTool(4);
}
void ToolCreateView::onCancelBtn5Click(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    ToolController::getInstance()->cancelMakeTool(5);
}
void ToolCreateView::onBoxBtnClick(CCObject *pSender, CCControlEvent event){
//    PopupViewController::getInstance()->removePopupView(this);
    playClose();
    PopupViewController::getInstance()->addPopupInView(EquipmentBagView::create(true));
}

void ToolCreateView::onBuyBtnClick(CCObject *pSender, CCControlEvent event){
    CCLOGFUNC();
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_buyItemId);
    int price = m_buyPrice;
//    if(info.ishot&&info.isShowHot)
//        price = info.hotprice;
//    else
//        price = info.price;
    int cCount = 0;
    if(info.type==3 && !info.getPara().empty()){
        cCount = atoi(info.getPara().c_str());
    }
    StoreBuyConfirmDialog* dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), info.getName().c_str(), _lang(info.des).c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(ToolCreateView::onBuy), NULL),m_buyBg->convertToWorldSpace(CCPointZero),Gold,cCount);
    int maxNum = floor(GlobalData::shared()->playerInfo.gold/price);
    maxNum = MAX(1, maxNum);
    dialog->showSliderBar(maxNum);
}
void ToolCreateView::onBuy(CCObject *ccObj)
{
    int buyCount = 1;
    if(ccObj){
        CCInteger* cInt = dynamic_cast<CCInteger*>(ccObj);
        if(cInt){
            buyCount = cInt->getValue();
        }
    }

    ToolController::getInstance()->buyTool(m_buyItemId,buyCount, CCCallFunc::create(this, callfunc_selector(ToolCreateView::onRetBuy)),200*buyCount,false,true,"StoreMall");
}
void ToolCreateView::onRetBuy()
{
//    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_buyItemId);
//    if(info.ishot&&info.isShowHot)
//        setLeftHot(true,info.hotnum);
//    
//    if(m_buyItemId!=-1){
//        ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_buyItemId);
//        if(info1.ishot&&info1.isShowHot)
//            setRightHot(true, info1.hotnum);
//    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY_CONFIRM_OK);
//    m_isClick = false;
}

void ToolCreateView::onOKCreateMate()
{
    if (m_qid == QID_MAX)
    {
        m_waitInterface= GameController::getInstance()->showWaitInterface(m_touchNode);
        m_waitInterface->setPosition(ccp(m_touchNode->getContentSize().width/2, m_touchNode->getContentSize().height/2));
        int itemId = m_makeTools[m_curOptIdx];
//        ToolController::getInstance()->flyToolReward(itemId);
        ToolController::getInstance()->startMakeTool(itemId);
    }
    else
    {
        int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
        if (tqid!=QID_MAX) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
            if (qInfo.allId!="") {
                vector<string> vec;
                string s = qInfo.allId;
                CCCommonUtils::splitString(s, ";", vec);
                if (vec.size()>=GlobalData::shared()->playerInfo.materialBackup) {
                    CCCommonUtils::flyHint("", "", _lang("111401"));
                }else{
                    int itemId = m_makeTools[m_curOptIdx];
                    ToolController::getInstance()->startMakeTool(itemId);
                }
            }

        }
        
    }
}

void ToolCreateView::onCostGoldBack()
{
    if (m_qid != QID_MAX) {
        if (_tmpGold > 0) {
            if (GlobalData::shared()->playerInfo.gold < _tmpGold) {
                YesNoDialog::gotoPayTips();
                return;
            }
        }
        QueueController::getInstance()->startCCDQueue(m_qid, "", false, _tmpGold,"",5);
    }
}

void ToolCreateView::setViewByMate(CCObject* p)
{
    m_buyItemId = dynamic_cast<CCInteger*>(p)->getValue();
    m_boxBtn->setVisible(false);
    m_buyTouchNode->setVisible(true);
    m_buyBtn->setVisible(false);
    m_buyPrice=0;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_buyItemId);
    string iconPath = info.icon;
    iconPath+=".png";
    string bgPath = CCCommonUtils::getToolBgByColor(info.color);
    auto nicon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
    CCCommonUtils::setSpriteMaxSize(nicon, 80, true);
    auto nbg = CCLoadSprite::createSprite(bgPath.c_str());
    CCCommonUtils::setSpriteMaxSize(nbg, 80, true);
    m_buySprNode->addChild(nbg);
    m_buySprNode->addChild(nicon);
    string makeStr = CCCommonUtils::getPropById(CC_ITOA(m_buyItemId) , "make");
    vector<string> vec;
    CCCommonUtils::splitString(makeStr, "|", vec);
    if (vec.size()==3) {
        m_buyPrice = atoi(vec[2].c_str());
        if (m_buyPrice>0) {
            m_price->setString(vec[2]);
            m_buyBtn->setVisible(true);
        }
        
    }
}

void ToolCreateView::updateCrtRet(CCObject* p)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
//    this->getAnimationManager()->runAnimationsForSequenceNamed("Start");
    
    m_qid = QID_MAX;
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
    if (tqid!=QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
        int itemId = QueueController::getInstance()->getItemId(qInfo.key);
        m_curCreatingItemId = itemId;
        m_qid = tqid;
        
        m_curIconNode->removeAllChildren();
        CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_curCreatingItemId));
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
        CCCommonUtils::setSpriteMaxSize(icon, 120);
        m_curIconNode->addChild(icon);
        
        CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
        CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
        CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
        icon->runAction(repeatForever);
        
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(m_curCreatingItemId);
        m_sumGold = toolInfo.crt_gold;
        m_oldSumTime = toolInfo.crt_cd;
        
        updateWaitCells(qInfo.allId);
    }
    onEnterFrame(0);
}

void ToolCreateView::updateWaitCells(string allId){
    if (allId!="") {
        vector<string> vec;
        string s = allId;
        CCCommonUtils::splitString(s, ";", vec);
        for (int i=1; i<vec.size(); ++i)
        {
            string waitIconPath = CCCommonUtils::getIcon(vec[i].c_str());
            auto waitIcon = CCLoadSprite::createSprite(waitIconPath.c_str(),true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(waitIcon, 120);
            m_waitInNode[i-1]->removeAllChildren();
            m_waitInNode[i-1]->addChild(waitIcon);
            m_cancelBtn[i-1]->setVisible(true);
        }
        int j=(GlobalData::shared()->playerInfo.materialBackup-2);
        int k=vec.size()-2;
        for(;j>k; --j){
            if(j<0){
                break;
            }
            m_waitInNode[j]->removeAllChildren();
            m_cancelBtn[j]->setVisible(false);
        }
    }
}

bool ToolCreateView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    startPt = pTouch->getLocation();
    for (int i=0; i<m_toolNodeVec.size(); i++) {
        if ( isTouchInside(m_toolNodeVec[i], pTouch) ) {
            refreshFlyNode(i);
            break;
        }
    }
    
    return true;
}

void ToolCreateView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    auto curPt = pTouch->getLocation();
    auto flyPos = m_flyNode->getParent()->convertTouchToNodeSpace(pTouch);
    m_flyNode->setPosition(flyPos);
    if (m_flyNode->isVisible()) {
        if (isTouchInside(m_queueNode, pTouch) || isTouchInside(m_make2Node, pTouch)) {
            onShowWillMate(true);
        }else {
            onShowWillMate(false);
        }
    }
}

void ToolCreateView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_willIconPath = "";
    m_tipNode->setVisible(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_STOP_BUILD_SHADOW, CCString::create(CC_ITOA(m_buildId)));
    auto endPt = pTouch->getLocation();
    
    if (m_flyNode->isVisible())
    {
        if (isTouchInside(m_queueNode, pTouch) || isTouchInside(m_make2Node, pTouch))
        {
            onOKCreateMate();
        }
        m_flyNode->setVisible(false);
        if (m_curOptIdx>=0 && m_curOptIdx<m_toolNodeVec.size()) {
            m_toolNodeVec[m_curOptIdx]->setVisible(true);
        }
    }
    else
    {
        if (isTouchInside(m_makeNode, pTouch) || isTouchInside(m_touchNode, pTouch))
        {
            if (m_qid != QID_MAX && isTouchInside(m_makeNode, pTouch))
            {
                YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(ToolCreateView::onCostGoldBack)), m_curTime, _lang("104903").c_str(), false, m_sumTime, m_sumGold);
            }
            
        }else {
//            this->closeSelf();
            if (isTouchInside(m_boxBtn, pTouch)||isTouchInside(m_buyTouchNode, pTouch)){
                return;
            }else if (isTouchInside(m_waitNode[0], pTouch)){
                if (m_canUse[0]==false) {
                    openCell();
                }
                return;
            }else if (isTouchInside(m_waitNode[1], pTouch)){
                if (m_canUse[1]==false) {
                    openCell();
                }
                return;
            }else if (isTouchInside(m_waitNode[2], pTouch)){
                if (m_canUse[2]==false) {
                    openCell();
                }
                return;
            }else if (isTouchInside(m_waitNode[3], pTouch)){
                if (m_canUse[3]==false) {
                    openCell();
                }
                return;
            }else if (isTouchInside(m_waitNode[4], pTouch)){
                if (m_canUse[4]==false) {
                    openCell();
                }
                return;
            }
            playClose();
        }
    }
}

void ToolCreateView::openCell(){
    if (m_canUse[0]==false) {
        string queue_unlock = CCCommonUtils::getPropById("100101", "queue_unlock");
        showOpenCell(queue_unlock);
    }else if (m_canUse[1]==false) {
        std::string queue_unlock = CCCommonUtils::getPropById("100102", "queue_unlock");
        showOpenCell(queue_unlock);
    }else if (m_canUse[2]==false) {
        std::string queue_unlock = CCCommonUtils::getPropById("100103", "queue_unlock");
        showOpenCell(queue_unlock);
    }else if (m_canUse[3]==false) {
        std::string queue_unlock = CCCommonUtils::getPropById("100104", "queue_unlock");
        showOpenCell(queue_unlock);
    }else if (m_canUse[4]==false) {
        std::string queue_unlock = CCCommonUtils::getPropById("100105", "queue_unlock");
        showOpenCell(queue_unlock);
    }
}

void ToolCreateView::showOpenCell(string queue_unlock){
    if (queue_unlock!="") {
        vector<string> vec;
        CCCommonUtils::splitString(queue_unlock, "|", vec);
        if (vec.size()==2) {
            int type = atoi(vec[0].c_str());
            int cost = atoi(vec[1].c_str());
        
            m_resType=type;
            m_resCost=cost;
            if (cost>0) {
                YesNoDialog::showResCost(_lang("135012").c_str(), CCCallFunc::create(this, callfunc_selector(ToolCreateView::onCostResBack)), _lang("102148").c_str(), false, cost, type);
            }else{
                YesNoDialog::showResCost(_lang("135012").c_str(), CCCallFunc::create(this, callfunc_selector(ToolCreateView::onCostResBack)), _lang("103672").c_str(), false, cost, type);
            }
        }
    }
}

void ToolCreateView::onCostResBack()
{
    switch (m_resType) {
        case Wood:
            if (GlobalData::shared()->resourceInfo.lWood<m_resCost) {
                CCCommonUtils::flyText(_lang("102197"));
                return;
            }
            break;
        case Iron:
            if (GlobalData::shared()->resourceInfo.lIron<m_resCost) {
                CCCommonUtils::flyText(_lang("102197"));
                return;
            }
            break;
        case Food:
            if (GlobalData::shared()->resourceInfo.lFood<m_resCost) {
                CCCommonUtils::flyText(_lang("102197"));
                return;
            }
            break;
        case Stone:
            if (GlobalData::shared()->resourceInfo.lStone<m_resCost) {
                CCCommonUtils::flyText(_lang("102197"));
                return;
            }
            break;
        case Gold:
        {
            if (GlobalData::shared()->playerInfo.gold<m_resCost) {
                YesNoDialog::gotoPayTips();
                return;
            }
        }
            break;
        default:
            break;
    }
    //开建造空位
    int curPlace=0;
    for(int i=0;i<5;i++){
        if (m_canUse[i]==false) {
            curPlace=i+2;
            break;
        }else{
            continue;
        }
        
    }
    ToolController::getInstance()->addMakeToolQueue(curPlace);
}
//开空位回调
void ToolCreateView::updateLockCell(CCObject* p){
    CCDictionary* dict = dynamic_cast<CCDictionary*>(p);
    if(dict->valueForKey("materialBackup")){
        int materialBackup = dict->valueForKey("materialBackup")->intValue();
        GlobalData::shared()->playerInfo.materialBackup = materialBackup;
        m_waitInNode[materialBackup-2]->removeAllChildren();
        m_cancelBtn[materialBackup-2]->setVisible(false);
        m_canUse[materialBackup-2]=true;
    }
    if (m_resCost>0) {//同步资源
        if(dict->objectForKey("gold")) {
            int tmpInt = dict->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        if(dict->objectForKey("wood")) {
            int tmpInt = dict->valueForKey("wood")->intValue();
            GlobalData::shared()->resourceInfo.lWood -= tmpInt;
        }
        if(dict->objectForKey("stone")) {
            int tmpInt = dict->valueForKey("stone")->intValue();
            GlobalData::shared()->resourceInfo.lStone -= tmpInt;
        }
        if(dict->objectForKey("iron")) {
            int tmpInt = dict->valueForKey("iron")->intValue();
            GlobalData::shared()->resourceInfo.lIron -= tmpInt;
        }
        if(dict->objectForKey("food")) {
            int tmpInt = dict->valueForKey("food")->intValue();
            GlobalData::shared()->resourceInfo.lFood -= tmpInt;
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
}

void ToolCreateView::refreshFlyNode(int idx)
{
    if (idx>=m_makeTools.size() || idx>=m_toolNodeVec.size()) {
        return;
    }
    
    int itemId = m_makeTools[idx];
    m_flyNode->removeAllChildren();
    m_flyNode->setVisible(true);
    m_curOptIdx = idx;
    
    m_tipBg->setFlipX(false);
    if (idx == 0) {
        m_tipNode->setPosition(ccp(-30, 365));
    }else if (idx == 1) {
        m_tipNode->setPosition(ccp(-126, 308));
    }else if (idx == 2) {
        m_tipNode->setPosition(ccp(-170, 205));
    }else if (idx == 3) {
        m_tipNode->setPosition(ccp(-170, 93));
    }else if (idx == 4) {
        m_tipNode->setPosition(ccp(-124, -12));
    }else if (idx == 5) {
        m_tipNode->setPosition(ccp(-26, -73));
    }else if (idx == 6) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(30, 365));
    }else if (idx == 7) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(126, 308));
    }else if (idx == 8) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(170, 205));
    }else if (idx == 9) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(170, 93));
    }else if (idx == 10) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(124, -12));
    }else if (idx == 11) {
        m_tipBg->setFlipX(true);
        m_tipNode->setPosition(ccp(26, -73));
    }
    
    auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
    
    string iconPath = "";
    CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
    string toolId = CC_ITOA(itemId);
    iconPath = CCCommonUtils::getIcon(toolId);
    m_willIconPath = iconPath;
    auto flyIcon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
    CCCommonUtils::setSpriteMaxSize(flyIcon, 160, true);
    m_flyNode->setPosition(m_toolNodeVec[idx]->getPosition());
    m_flyNode->addChild(flyIcon);
    
    m_toolNodeVec[idx]->setVisible(false);
    
    m_tipName->setString( CCCommonUtils::getNameById(toolId) );
    m_tipTime->setString( CC_SECTOA(toolInfo.crt_cd) );
    
    m_tipNode->setVisible(true);
}

void ToolCreateView::onShowWillMate(bool show)
{
    if (m_qid == QID_MAX)
    {
        if (show && m_willIconPath!="")
        {
            if (m_curIconNode->getChildrenCount() <= 0) {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAY_BUILD_SHADOW, CCString::create(CC_ITOA(m_buildId)));
            }
            m_curIconNode->removeAllChildren();
            CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
            auto icon = CCLoadSprite::createSprite(m_willIconPath.c_str(),true,loadType);
            CCCommonUtils::setSpriteMaxSize(icon, 120);
            CCCommonUtils::setSpriteGray(icon, true);
            m_curIconNode->addChild(icon);
        }
        else {
            if (m_curIconNode->getChildrenCount() > 0) {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_STOP_BUILD_SHADOW, CCString::create(CC_ITOA(m_buildId)));
            }
            m_curIconNode->removeAllChildren();
        }
    }else if(m_willIconPath!=""){
        int next=0;
        for (int i=0; i<5; i++) {
            if (m_cancelBtn[i]->isVisible()) {
                if(i==4){
                    return;
                }
                continue;
            }
            next=i;
            break;
        }
        int i=GlobalData::shared()->playerInfo.materialBackup-1;
        if (next>=i) {
            return;
        }
        
        if (show) {
            CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
            auto icon = CCLoadSprite::createSprite(m_willIconPath.c_str(),true,loadType);
            CCCommonUtils::setSpriteMaxSize(icon, 120);
            CCCommonUtils::setSpriteGray(icon, true);
            m_waitInNode[next]->addChild(icon);
        }else{
            m_waitInNode[next]->removeAllChildren();
        }
            
    }
}

void ToolCreateView::flyEnd()
{
    m_flyNode->removeAllChildren();
    m_flyNode->setScale(1.0);
}

void ToolCreateView::onEnterFrame(float dt)
{
    if (m_qid == QID_MAX) {
        return;
    }
    
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    int tmpCurTime = GlobalData::shared()->getWorldTime();
    if(qInfo.startTime==0){
        qInfo.startTime = tmpCurTime;
    }
    m_sumTime = qInfo.finishTime - qInfo.startTime;
    m_curTime = qInfo.finishTime - tmpCurTime;
    m_curTime = MAX(0, m_curTime);
    m_sumTime = MAX(1, m_sumTime);
    
    if (m_curTime>0) {
        m_spdIcon->setVisible(true);
        if (qInfo.finishTime<=0) {
            m_curTime = 0;
            m_sumTime = 1;
        }
        m_lastTime->setString(CC_SECTOA(m_curTime));
        _tmpGold = m_sumGold*(m_curTime*1.0/m_oldSumTime)+0.5;
        _tmpGold = MAX(1, _tmpGold);
    }
    else {
        QueueController::getInstance()->autoHavestMateTool(m_qid, m_buildId);
        m_spdIcon->setVisible(false);
        m_lastTime->setString("");
        m_curIconNode->removeAllChildren();
        m_qid = QID_MAX;
//        this->getAnimationManager()->runAnimationsForSequenceNamed("End");
    }
}

void ToolCreateView::playClose()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(ToolCreateView::AnimationClose));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void ToolCreateView::AnimationClose()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(ToolCreateView::closeView),NULL);
}

void ToolCreateView::closeView(CCObject* params)
{
//    PopupViewController::getInstance()->goBackPopupView();
    this->closeSelf();
}

void ToolCreateView::AnimationFadeIn()
{
    string name = this->getAnimationManager()->getLastCompletedSequenceName();
    if (name=="FadeIn" && m_qid!=QID_MAX) {
        this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
        this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    }
}