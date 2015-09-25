
#include "MerchantView.h"
#include "ToolController.h"
#include "SceneController.h"
#include "StoreMallCellTip.h"
#include "YesNoDialog.h"
#include "PopupViewController.h"
#include "GameController.h"
#include "WorldController.h"
#include "StoreBuyConfirmDialog.h"
#include "CCMathUtils.h"
#include "ParticleController.h"
#include "TipsView.h"
#include "SoundController.h"
#include "EquipmentController.h"

#define MERCHANTE_REFRESH_TIME_KEY "merchante.fresh.key"
#define MERCHANTE_REFRESH_COST_KEY "merchante.fresh.cost"
#define MERCHANTE_REFRESH_SHOW_FLAG_KEY "merchante.fresh.show.flag"
#define MSG_MERCHANTE_REFRESH_ANIMATION_FINISH "merchante.refresh.animation.finish"
#define MERCHANTE_SHINE_MAX_FRAME 13
#define MERCHANTE_SHINE_EFF_MAX_FRAME 12

const int merchantTipNum = 8;
const char* merchantTips[merchantTipNum] = {"104934","104935","104936","104937","104938","104945","104946","104947"};
MerchantView* MerchantView::create(){
    MerchantView* ret = new MerchantView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MerchantView::MerchantView():m_isInit(false),m_onTimer(false),m_refreshCost(0),m_refreshTime(0),m_dataCount(0),m_isRefresh(false),m_refreshTipType(0){
    
}
MerchantView::~MerchantView(){
    
}

bool MerchantView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    setTitleName("");
    auto tmpCCB = CCBLoadFile("MerchantView",this,this);
        setContentSize(tmpCCB->getContentSize());
        auto size = CCDirector::sharedDirector()->getWinSize();
    if (CCCommonUtils::isIosAndroidPad())
    {
        size = CCSize(640, 852);
    }
        //    m_bgNode->removeAllChildren();
        float dh = size.height-TOP_HEIGHT - 340;
        //    int count = (size.height-170)/44+1;
        //    for (int i=0; i<count; i++) {
        //        auto spr = CCLoadSprite::createSprite("Items_bg_big.png");
        //        this->m_bgNode->addChild(spr);
        //        spr->setPositionY(-i*44);
        //    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        dh += 80;
    }
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,dh));
    m_onTimer = false;
    auto orgSize = CCDirector::sharedDirector()->getVisibleSize();
    CCLayerColor *color = CCLayerColor::create(ccc4(0, 0, 0, 255),orgSize.width,orgSize.height);
    this->addChild(color);
    color->setZOrder(-10);
    color->setPositionY(tmpCCB->getPositionY() - 340);
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setDelegate(this);
    m_infoList->addChild(m_tabView);
    m_itemNode1->setVisible(false);
    m_lblInfo1->setString(_lang("104950"));
    m_lblTitleRefresh->setString(_lang("104932"));
//    float posX = m_timeTitleText->getContentSize().width * m_timeTitleText->getOriginScaleX() + m_timeTitleText->getPositionX();
//    m_timeLeftText->setPositionX(posX+5);
    float sizeW = m_lblTitleRefresh->getContentSize().width * m_lblTitleRefresh->getOriginScaleX();
    CCSize sizeBtn = m_btnRefresh->getContentSize();
    if(sizeBtn.width-200<sizeW){
        m_btnRefresh->setPreferredSize(CCSize(sizeW+200, sizeBtn.height));
        float offsetX = (sizeW - (sizeBtn.width-200))*0.5;
        m_lblTitleRefresh->setPositionX(m_lblTitleRefresh->getPositionX() + offsetX);
        m_sprRefresh->setPositionX(m_sprRefresh->getPositionX()+offsetX);
        m_lblRefresh->setPositionX(m_lblRefresh->getPositionX()+offsetX);
    }
    return true;
}
void MerchantView::refreshTip(){
    m_itemNode1->stopAllActions();
    int index = 0;
    int min = 5;
    int max = merchantTipNum-1;
    if(m_lastIndex>=min && m_lastIndex<=max){
        int a = CCMathUtils::getRandomInt(0, 1);
        if(m_lastIndex==min){
            index = CCMathUtils::getRandomInt(min+1, max);
        }else if(m_lastIndex == max){
            index = CCMathUtils::getRandomInt(min, max-1);
        }else{
            index = (a == 0)? CCMathUtils::getRandomInt(min, m_lastIndex-1):CCMathUtils::getRandomInt(m_lastIndex+1, max);
        }
    }else{
        index = CCMathUtils::getRandomInt(min, max);
    }
    setTip(index);
    m_itemNode1->runAction(CCSequence::create(CCDelayTime::create(90),CCCallFunc::create(this,callfunc_selector(MerchantView::refreshTip)),NULL));
}
void MerchantView::setTip(int index){
    m_lastIndex = index;
//    CCLog("___________tipindex:%s",merchantTips[index]);
    m_lblDialog->setString(_lang(merchantTips[index]));
    CCScrollView *scrollView = dynamic_cast<CCScrollView*>(m_itemNode1->getChildByTag(100));
    if(scrollView){
        float dialogH = m_lblDialog->getContentSize().height * m_lblDialog->getOriginScaleY();
        float scrollH = 120;
        if(dialogH > scrollH){
            m_lblDialog->setPositionY(0);
            scrollView->setTouchEnabled(true);
            scrollView->setContentOffset(ccp(0, scrollH - dialogH));
            scrollView->getContainer()->setContentSize(CCSize(320,dialogH));
        }else{
            m_lblDialog->setPositionY(scrollH - dialogH);
            scrollView->setTouchEnabled(false);
        }
    }
}
void MerchantView::onEnter(){
    PopupBaseView::onEnter();
    
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MerchantView::closeView), MSG_TOOL_MERCHANTE_LEAVE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MerchantView::resetRefreshTip), MSG_BUY_CONFIRM_OK, NULL);
    double timeStamp = GlobalData::shared()->getTimeStamp();
    double leftT = GlobalData::shared()->tomorrow_time - timeStamp;
    if(leftT > 0){
        this->schedule(schedule_selector(MerchantView::onNewDay), leftT, 1, 0);
    }else if(leftT < 0){
        GlobalData::shared()->tomorrow_time += 86400;
        leftT = GlobalData::shared()->tomorrow_time - timeStamp;
        this->schedule(schedule_selector(MerchantView::onNewDay), leftT, 1, 0);
    }else{
        GlobalData::shared()->tomorrow_time += 86400;
        this->schedule(schedule_selector(MerchantView::onNewDay), 86400, 1, 0);
    }
    if(!m_isInit){
        getHotItems();
    }
}
void MerchantView::onNewDay(float dt){
    GlobalData::shared()->tomorrow_time += 86400;
    CCUserDefault::sharedUserDefault()->setIntegerForKey(MERCHANTE_REFRESH_COST_KEY, 0);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(MERCHANTE_REFRESH_SHOW_FLAG_KEY, 0);
    CCUserDefault::sharedUserDefault()->flush();
    getHotItems();
    this->unschedule(schedule_selector(MerchantView::onNewDay));
    this->schedule(schedule_selector(MerchantView::onNewDay), 86400, 1, 0);
}
void MerchantView::closeView(CCObject *ccObj){
    closeSelf();
}
void MerchantView::resetRefreshTip(CCObject *ccObj){
    m_refreshTipType = 0;
    auto iter = ToolController::getInstance()->m_toolMerchantInfos.begin();
    while (iter!=ToolController::getInstance()->m_toolMerchantInfos.end()) {
        if((*iter).itemId == ToolController::getInstance()->merchantBestTool.itemId && (*iter).itemNum == ToolController::getInstance()->merchantBestTool.itemNum && (*iter).color == ToolController::getInstance()->merchantBestTool.color){
            m_refreshTipType = 3;
            return;
        }
        ++iter;
    }
//    if(m_refreshTipType!=3){
        std::string key = CCUserDefault::sharedUserDefault()->getStringForKey(MERCHANTE_REFRESH_TIME_KEY);
        if(!key.empty()){
            double refreshTomorrow = atof(key.c_str());
            double timeStamp = GlobalData::shared()->getTimeStamp();
            if(GlobalData::shared()->tomorrow_time > timeStamp){
                double tomorrow = GlobalData::shared()->tomorrow_time;
                if(tomorrow - refreshTomorrow > 0.1){
                    m_refreshTipType = 1;
                    return;
                }
            }
        }else{
            m_refreshTipType = 1;
        }
        int oldCost = CCUserDefault::sharedUserDefault()->getIntegerForKey(MERCHANTE_REFRESH_COST_KEY);
        int flag = CCUserDefault::sharedUserDefault()->getIntegerForKey(MERCHANTE_REFRESH_SHOW_FLAG_KEY);
        switch (flag) {
            case 0:{
                if(oldCost!=m_refreshCost){
                    m_refreshTipType = 2;
                }
                CCUserDefault::sharedUserDefault()->setIntegerForKey(MERCHANTE_REFRESH_SHOW_FLAG_KEY, 1);
                break;
            }
            case 1:{
                if(m_refreshCost > ToolController::getInstance()->getMerchanteCostTip()){
                    m_refreshTipType = 2;
                    CCUserDefault::sharedUserDefault()->setIntegerForKey(MERCHANTE_REFRESH_SHOW_FLAG_KEY, 2);
                }
                break;
            }
            default:
                break;
        }
        CCUserDefault::sharedUserDefault()->flush();
//        if(oldCost>0 && oldCost!=m_refreshCost){
//            m_refreshTipType = 2;
//        }else if(oldCost == 0 || m_refreshCost==0){
//            m_refreshTipType = 1;
//        }
//    }
}
void MerchantView::onExit(){
    setTouchEnabled(false);
    if(m_onTimer) {
        this->unschedule(schedule_selector(MerchantView::onTimer));
        m_onTimer = false;
    }
    this->unschedule(schedule_selector(MerchantView::onNewDay));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_MERCHANTE_LEAVE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,  MSG_BUY_CONFIRM_OK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MERCHANTE_REFRESH_ANIMATION_FINISH);

    PopupBaseView::onExit();
}
void MerchantView::getHotItems(bool useGold){
    if(m_isRefresh==true)
        return;
    m_isRefresh=true;
    if(m_isInit && useGold){
        m_showRefreshEff=true;
        ToolController::getInstance()->setMerchante_animation(true);
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MerchantView::finishAnimation), MSG_MERCHANTE_REFRESH_ANIMATION_FINISH, NULL);
        m_curFinAnimation=0;
        CCUserDefault::sharedUserDefault()->setIntegerForKey(MERCHANTE_REFRESH_COST_KEY, m_refreshCost);
        CCUserDefault::sharedUserDefault()->flush();
    }
    GameController::getInstance()->showWaitInterface();
    CommandBase *cmd = NULL;
    if(!useGold){
        cmd = new ToolMerchantGetCommand();
    }else{
        cmd = new ToolMerchantRefreshCommand();
    }
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MerchantView::refreshTime), NULL));
    cmd->sendAndRelease();
}

void MerchantView::refreshTime(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dict = _dict(result->getData());
    if (dict->objectForKey("goldCost")) {
        m_refreshCost =  dict->valueForKey("goldCost")->intValue();
        if(m_refreshCost <= 0){
            setButtonTitle(m_btnRefresh, _lang("115062").c_str());
            m_lblRefresh->setVisible(false);
            m_lblTitleRefresh->setVisible(false);
            m_sprRefresh->setVisible(false);
        }else{
            setButtonTitle(m_btnRefresh,"");
            m_lblRefresh->setString(CC_ITOA(m_refreshCost));
            m_lblRefresh->setVisible(true);
            m_lblTitleRefresh->setVisible(true);
            m_sprRefresh->setVisible(true);
        }
    }
//    m_refreshTime = ToolController::getInstance()->getMerchante_time_e1();
    m_refreshTime = GlobalData::shared()->tomorrow_time;
    double nowTime = GlobalData::shared()->getTimeStamp();
    
    m_leftTime = m_refreshTime - nowTime;
    m_timeLeftText->setString(_lang("102164") + ": " + CC_SECTOA(m_leftTime));
    if (!m_onTimer) {
        this->schedule(schedule_selector(MerchantView::onTimer),1.0);
        m_onTimer = true;
    }else {
        this->unschedule(schedule_selector(MerchantView::onTimer));
        this->schedule(schedule_selector(MerchantView::onTimer),1.0);
    }
    m_timeNode->setVisible(true);
    if(ToolController::getInstance()->merchantBestTool.itemId!=0){
        CCLabelIF *label = dynamic_cast<CCLabelIF*>(m_itemNode1->getChildByTag(1));
        CCNode *picNode = dynamic_cast<CCNode*>(m_itemNode1->getChildByTag(2));
        CCLabelIF *label1 = dynamic_cast<CCLabelIF*>(m_itemNode1->getChildByTag(3));
        CCNode *nodePirce = m_itemNode1->getChildByTag(5);
        CCSprite *sprPrice = dynamic_cast<CCSprite*>(m_itemNode1->getChildByTag(6));
        CCLabelIF *lblPrice =dynamic_cast<CCLabelIF*>(m_itemNode1->getChildByTag(7));
        const MerchantToolInfo& bestTool = ToolController::getInstance()->merchantBestTool;
        if(label && picNode && label1 && nodePirce && sprPrice && lblPrice){
            bool showPrice = bestTool.price>0?true:false;
            if(showPrice){
                if(m_itemNode1->getChildByTag(8)){
                    m_itemNode1->removeChildByTag(8);
                }
                if(bestTool.priceType<WorldResource_Max){
                    std::string urlPrice = CCCommonUtils::getResourceIconByType(bestTool.priceType);
                    if(!urlPrice.empty()){
                        CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(urlPrice.c_str());
                        if(pFrame){
                            sprPrice->setDisplayFrame(pFrame);
                        }
                    }
                    sprPrice->setVisible(showPrice);
                }else{
                    CCNode *priceNode = CCNode::create();
                    auto priceSpr = CCLoadSprite::createSprite("Items_icon_kuang2.png");
                    priceSpr->setScale(0.4);
                    priceNode->addChild(priceSpr);
                    CCCommonUtils::createGoodsIcon(bestTool.priceType, priceNode,CCSize(35, 35));
                    m_itemNode1->addChild(priceNode);
                    priceNode->setTag(8);
                    priceNode->setPosition(sprPrice->getPosition() - ccp(10, -20));
                    sprPrice->setVisible(false);
                }
                lblPrice->setString(CC_CMDITOA(bestTool.price));
            }
            lblPrice->setVisible(showPrice);
            int oldPrice = 0;
            label1->setString(std::string(" x") + CC_CMDITOA(bestTool.itemNum));
            if(bestTool.itemId <WorldResource_Max){
                std::string resourceName = CCCommonUtils::getResourceNameByType(bestTool.itemId);
                label->setString(resourceName);
                auto pic = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(bestTool.itemId).c_str());
                if(pic){
                    auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(WHITE).c_str());
                    CCCommonUtils::setSpriteMaxSize(iconBg, 95,true);
                    picNode->addChild(iconBg);
                    CCCommonUtils::setSpriteMaxSize(pic, 75,true);
                    picNode->addChild(pic);
                }
                oldPrice = bestTool.priceHot;
                m_desName->setString(resourceName);
                m_desLabel->setString(resourceName + " x"+ CC_ITOA(bestTool.itemNum));
            }else{
                if(bestTool.type == MerchantTool_GOODS){
                    CCCommonUtils::createGoodsIcon(bestTool.itemId, picNode, CCSize(95, 95),NULL,m_desName,m_desLabel);
                    label->setString(m_desName->getString());
                    if(bestTool.priceHot>0){
                        oldPrice = bestTool.priceHot;
                    }else{
                        auto &info = ToolController::getInstance()->getToolInfoById(bestTool.itemId);
                        if(info.price_hot>0 && bestTool.priceType == Gold){
                            oldPrice = info.price_hot * bestTool.itemNum;
                        }
                    }
                }else if(bestTool.type == MerchantTool_EQUIP){
                    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[bestTool.itemId];
                    string ename = _lang(eInfo.name);
                    string ebgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
                    auto ebgIcon = CCLoadSprite::createSprite(ebgPath.c_str());
                    CCCommonUtils::setSpriteMaxSize(ebgIcon, 95, true);
                    picNode->addChild(ebgIcon);
                    string epicStr = CCCommonUtils::getIcon(CC_ITOA(bestTool.itemId));
                    auto eicon = CCLoadSprite::createSprite(epicStr.c_str(),true,CCLoadSpriteType_EQUIP);
                    CCCommonUtils::setSpriteMaxSize(eicon, 90, true);
                    picNode->addChild(eicon);
                    m_desName->setString(ename);
                    m_desLabel->setString(_lang(eInfo.des));
                    oldPrice = bestTool.priceHot;
                }
                
                if(showPrice && oldPrice>0 && nodePirce->getChildByTag(2)){
                    CCLabelIF *lbl = dynamic_cast<CCLabelIF*>(nodePirce->getChildByTag(2));
                    if(lbl){
                        lbl->setString(CC_CMDITOA(oldPrice));
                        CCScale9Sprite *scaleSpr = dynamic_cast<CCScale9Sprite*>(nodePirce->getChildByTag(3));
                        if(scaleSpr){
                            float scale = (lbl->getContentSize().width * lbl->getOriginScaleX()+10) * 1.0 / 107.0;
                            scaleSpr->setScaleX(scale);
                        }
                        lblPrice->setPositionY(52);
                        sprPrice->setPositionX(186);
                        sprPrice->setScale(1);
                        nodePirce->setVisible(true);
                    }else{
                        lblPrice->setPositionY(35);
                        sprPrice->setPositionX(196);
                        sprPrice->setScale(1);
                        nodePirce->setVisible(false);
                    }
                }else{
                    lblPrice->setPositionY(35);
                    sprPrice->setPositionX(186);
                    sprPrice->setScale(1);
                    nodePirce->setVisible(false);
                }
            }
        }
//        addEffect();
//        if(!m_itemNode1->getChildByTag(10000)){
//            CCSize size = m_itemNode1->getContentSize();
//            ccColor3B color = {251, 183, 40};//{138,62,251};
//            MerchantShining *shine = MerchantShining::create(color,size);
//            shine->showEff2(true,0.7);
//            shine->setShineAlpha(255);
//            m_itemNode1->addChild(shine);
//            shine->setTag(10000);
//            shine->setPosition(ccp(size.width*0.5,size.height*0.5));
//            shine->startPlay(true);
//        }
    }else{
        m_desNode->setVisible(false);
        m_itemNode1->removeAllChildren();
        CCScrollView *scrollView = CCScrollView::create(CCSize(320, 140));
        scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_lblDialog = CCLabelIF::create();
        m_lblDialog->setAnchorPoint(CCPointZero);
        m_lblDialog->setColor(ccc3(194, 158, 116));
        m_lblDialog->setDimensions(CCSize(320,0));
        m_lblDialog->setFontSize(20);
        scrollView->addChild(m_lblDialog);
        m_itemNode1->addChild(scrollView, 0, 100);
        scrollView->setPosition(ccp(0, 40));
        m_timeNode->setVisible(false);
//        m_timeTitleText->setString(_lang("104931"));
        CCCommonUtils::setButtonTitle(m_btnRefresh,"");
        int index = CCMathUtils::getRandomInt(0, 4);
        setTip(index);
        m_itemNode1->runAction(CCSequence::create(CCDelayTime::create(90),CCCallFunc::create(this,callfunc_selector(MerchantView::refreshTip)),NULL));
    }
    m_itemNode1->setVisible(true);
    m_isRefresh=false;
    m_isInit = true;
    refreshView();
}
void MerchantView::refreshView(CCObject *ccObj){
    generateData();
    m_tabView->reloadData();
    m_showRefreshEff=false;
}
void MerchantView::finishAnimation(CCObject *ccObj){
    ++m_curFinAnimation;
    int maxIndex = ceil(m_infoList->getContentSize().height/cellSizeForTable(m_tabView).height);
    if(m_curFinAnimation >= maxIndex){
        ToolController::getInstance()->setMerchante_animation(false);
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MERCHANTE_REFRESH_ANIMATION_FINISH);
        m_curFinAnimation=0;
    }
}
void MerchantView::onTimer(float dt) {
    double nowTime = GlobalData::shared()->getTimeStamp();
    m_leftTime = m_refreshTime - nowTime;
    if (m_leftTime < 0) {
        StoreBuyConfirmDialog *dialog = dynamic_cast<StoreBuyConfirmDialog*>(PopupViewController::getInstance()->getCurrentPopupView());
        if(dialog){
            dialog->closeSelf();
        }
        ToolController::getInstance()->m_merchantCurrIndex = 0;
//        if(m_isRefresh == true)
//            return;
        if(m_onTimer){
            this->unschedule(schedule_selector(MerchantView::onTimer));
            m_onTimer = false;
        }
//        closeSelf();
//        getHotItems();
    } else {
        string txt = _lang("102164") + ": " + CC_SECTOA(m_leftTime);
        m_timeLeftText->setString(txt);
    }
}
void MerchantView::generateData(){
    auto &items = ToolController::getInstance()->m_toolMerchantInfos;
    m_dataCount = (int)items.size();
}
SEL_CCControlHandler MerchantView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRefreshClick", MerchantView::onRefreshClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipClick", MerchantView::onTipClick);
    return NULL;
}
bool MerchantView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", CCNode*, this->m_timeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblInfo1", CCLabelIF*, this->m_lblInfo1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTitleText", CCLabelIF*, this->m_timeTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLeftText", CCLabelIF*, this->m_timeLeftText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnRefresh", CCControlButton*, this->m_btnRefresh);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblRefresh", CCLabelIF*, this->m_lblRefresh);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprRefresh", CCSprite*, this->m_sprRefresh);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitleRefresh", CCLabelIF*, this->m_lblTitleRefresh);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode1", CCNode*, this->m_itemNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_receiveGlow", CCNode*, this->m_receiveGlow);
    return false;
}
bool MerchantView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_desNode->setVisible(false);
    if(!isTouchInside(m_infoList, pTouch)){
        return true;
    }
    return false;
}
void MerchantView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void MerchantView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_infoList, pTouch)){
    }
    if(m_itemNode1->isVisible() && isTouchInside(m_itemNode1->getChildByTag(4),pTouch)){
        m_desNode->setVisible(true);
    }
}
CCSize MerchantView::tableCellSizeForIndex(CCTableView *table, ssize_t idx){
    if(idx >= m_dataCount){
        return CCSizeZero;
    }
    return CCSize(600, 185);
}
CCSize MerchantView::cellSizeForTable(CCTableView *table){
    return CCSize(600, 185);
}
CCTableViewCell* MerchantView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    if(idx >= m_dataCount){
        return NULL;
    }
//    auto &items = ToolController::getInstance()->m_toolMerchantInfos;
    MerchantTabelCell* cell = (MerchantTabelCell*)table->dequeueCell();
    if(cell){
        cell->setData(idx,m_showRefreshEff);
    }else{
        cell = MerchantTabelCell::create(idx,m_infoList);
    }
    return cell;
}
ssize_t MerchantView::numberOfCellsInTableView(CCTableView *table){
    return m_dataCount;
}
void MerchantView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){
    
}
void MerchantView::scrollViewDidScroll(CCScrollView* view){
    float mindy = m_tabView->minContainerOffset().y-20;
    float dy = m_tabView->getContentOffset().y;
    if(dy<mindy){
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}
void MerchantView::onRefreshClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(ToolController::getInstance()->getMerchante_animation()==true)
        return;
    resetRefreshTip();
    if(m_isRefresh==true)
        return;
    switch (m_refreshTipType) {
        case 1:
        case 2:{
            if(m_refreshCost <= 0 ){
                onOkRefresh();
            }else{
                YesNoDialog::showButtonAndGold(_lang("104939").c_str(), CCCallFunc::create(this, callfunc_selector(MerchantView::onOkRefresh)), _lang("104932").c_str(), m_refreshCost);
            }
            break;
        }
        case 3:{
            YesNoDialog::showButtonAndGold(_lang("104951").c_str(), CCCallFunc::create(this, callfunc_selector(MerchantView::onOkRefresh)), _lang("104932").c_str(), m_refreshCost);
            break;
        }
        default:{
            onOkRefresh();
            break;
        }
    }
}
void MerchantView::addEffect(){
    string tmpStart = "UIGlow_LR_";
    float offset = 80.0f;
    float scale = 0.9f;
    float scale1 = 0.5f;
    int count = 3;
    for (int i=1; i <= count; i++) {
        if(i==2){
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(offset, 0));
        particle->setScaleY(scale);
        particle->setScaleX(scale1);
        m_receiveGlow->addChild(particle);
    }
    for (int i=1; i <= count; i++) {
        if(i==2){
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(-offset, 0));
        particle->setScaleY(scale);
        particle->setScaleX(scale1);
        m_receiveGlow->addChild(particle);
    }
    tmpStart = "UIGlow_UD_";
    for (int i=1; i <= count; i++) {
        if(i==2){
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(0, offset));
        particle->setScaleX(scale);
        particle->setScaleY(scale1);
        m_receiveGlow->addChild(particle);
    }
    for (int i=1; i <= count; i++) {
        if(i==2){
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(0, -offset));
        particle->setScaleX(scale);
        particle->setScaleY(scale1);
        m_receiveGlow->addChild(particle);
    }
}
void MerchantView::onOkRefresh(){
    if (CCCommonUtils::isEnoughResourceByType(Gold,m_refreshCost)) {
        getHotItems(true);
    }else{
        YesNoDialog::gotoPayTips();
    }
    if(m_refreshTipType==1){
        CCUserDefault::sharedUserDefault()->setStringForKey(MERCHANTE_REFRESH_TIME_KEY, CC_ITOA(GlobalData::shared()->tomorrow_time));
        CCUserDefault::sharedUserDefault()->setIntegerForKey(MERCHANTE_REFRESH_SHOW_FLAG_KEY,0);
        CCUserDefault::sharedUserDefault()->flush();
    }
    m_refreshTipType = 0;
}
void MerchantView::onTipClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("104952")));
}
//////////////////////////////
//           cell
//////////////////////////////
MerchantCell::~MerchantCell(){
}
MerchantCell* MerchantCell::create(int index, CCNode* touchNode){
    auto ret = new MerchantCell();
    if (ret && ret->init(index, touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MerchantCell::init(int index,CCNode* touchNode){
    bool ret = true;
//    aniNode = new CCAniNode();
    CCBLoadFile("MerchantCell",this,this);
//    addChild(aniNode);
    this->setContentSize(CCSize(606, 180));
    this->ignoreAnchorPointForPosition(true);
    m_lblSuper->setString(_lang("104933"));
    m_superNode->setVisible(false);
    m_touchNode = touchNode;
    setDataIndex(index);
    setData();
    animationCallback();
    return ret;
}

void MerchantCell::setData(){
    m_isClick=false;
    m_costNode1->removeAllChildren();
    m_info1 = &(ToolController::getInstance()->m_toolMerchantInfos.at(m_index));
    m_desNode->setVisible(false);
    m_priceLabel1->setColor({255,255,0});
    m_buyBtngraySpr1->setVisible(false);
    m_superNode->setVisible(false);
    m_picNode1->removeAllChildren();
    int showOldPrice = 0;
//    if(m_info1->itemId < WorldResource_Max){
    if(m_info1->type == MerchantTool_RESOURCE){
        m_nameLabel1->setString(CCCommonUtils::getResourceNameByType(m_info1->itemId)  + std::string(" ×") + CC_CMDITOA(m_info1->itemNum));
        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(m_info1->itemId).c_str());
        if(pic){
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(WHITE).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 92,true);
            m_picNode1->addChild(iconBg);
            CCCommonUtils::setSpriteMaxSize(pic, 70,true);
            m_picNode1->addChild(pic);
        }
        showOldPrice = m_info1->priceHot;
    }else if(m_info1->type == MerchantTool_GOODS){
        auto &info = ToolController::getInstance()->getToolInfoById(m_info1->itemId);
        m_nameLabel1->setString(info.getName()  + std::string(" ×") + CC_CMDITOA(m_info1->itemNum));
        CCCommonUtils::createGoodsIcon(info.itemId, m_picNode1, CCSize(92, 92));
        //test code
//        m_info1->priceType = info.itemId;
        //is hot
        if(m_info1->priceHot>0){
            showOldPrice = m_info1->priceHot;
        }else{
            if(info.price_hot>0 && m_info1->priceType == Gold){
                showOldPrice = info.price_hot * m_info1->itemNum;
            }
        }
    }else if(m_info1->type == MerchantTool_EQUIP){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_info1->itemId];
        string ename = _lang(eInfo.name) +std::string(" ×") + CC_CMDITOA(m_info1->itemNum);
        m_nameLabel1->setString(ename);
        string ebgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        auto ebgIcon = CCLoadSprite::createSprite(ebgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(ebgIcon, 92, true);
        m_picNode1->addChild(ebgIcon);
        string epicStr = CCCommonUtils::getIcon(CC_ITOA(m_info1->itemId));
        auto eicon = CCLoadSprite::createSprite(epicStr.c_str(),true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(eicon, 87, true);
        m_picNode1->addChild(eicon);
        m_priceLabel1->setPositionY(m_costNode1->getPositionY());
        m_hotLineSpr1->setVisible(false);
        m_hotpriceLabel1->setVisible(false);
        showOldPrice = m_info1->priceHot;
    }
    
    if(showOldPrice>0){
        m_priceLabel1->setPositionY(-m_hotpriceLabel1->getPositionY());
        m_hotpriceLabel1->setString(CC_CMDITOA(showOldPrice));
        m_hotLineSpr1->setVisible(true);
        m_hotpriceLabel1->setVisible(true);
    }else{
        m_priceLabel1->setPositionY(m_costNode1->getPositionY());
        m_hotLineSpr1->setVisible(false);
        m_hotpriceLabel1->setVisible(false);
    }
    if(m_info1->priceType < WorldResource_Max){
        auto costIcon = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(m_info1->priceType).c_str());
        if(costIcon){
            costIcon->setTag(100);
            m_costNode1->addChild(costIcon);
        }
    }else{
        CCNode *priceNode = CCNode::create();
        auto priceSpr = CCLoadSprite::createSprite("Items_icon_kuang2.png");
        priceNode->addChild(priceSpr);
        priceSpr->setScale(0.4);
        auto priceSize = priceSpr->getContentSize();
        CCCommonUtils::createGoodsIcon(m_info1->priceType, priceNode,CCSize(35, 35));
        m_costNode1->addChild(priceNode);
    }
    
    
    m_priceLabel1->setString(CC_CMDITOA(m_info1->price));
    setLeftHot(false,m_info1->num);
    if(m_info1->color<2 && m_shiningNode){
        m_shiningNode->removeAllChildren();
        m_shiningNode=NULL;
    }else if(m_info1->color>1){
        ccColor3B color;
        float alpha;
        bool showEff;
        if(m_info1->itemId == ToolController::getInstance()->merchantBestTool.itemId && m_info1->itemNum == ToolController::getInstance()->merchantBestTool.itemNum && m_info1->color == ToolController::getInstance()->merchantBestTool.color){
            color = {251, 183, 40};
            alpha = 255;
            showEff=true;
            m_superNode->setVisible(true);
        }else{
            switch (m_info1->color) {
                case 1:{
                    color = {93,219,68};
                    alpha = 153;
                    showEff=false;
                    break;
                }
                case 2:{
                    color = {59,172,224};
                    alpha = 204;
                    showEff=false;
                    break;
                }
                default:{
                    color = {138,62,251};
                    alpha = 255;
                    showEff=true;
                    m_superNode->setVisible(true);
                    break;
                }
            }
        }
        if(!m_shiningNode){
            CCSize size = this->getContentSize();
            size.width += 30;
            size.height += 10;
            m_shiningNode = MerchantShining::create(color,size);
            addChild(m_shiningNode);
            m_shiningNode->setPosition(ccp(size.width*0.5,size.height*0.5));
        }else{
            m_shiningNode->setShineColor(color);
        }
        m_shiningNode->setShineAlpha(alpha);
        m_shiningNode->showEff2(showEff);
//        m_shiningNode->startPlay(false);
//        m_shiningNode->setVisible(false);
    }
}

void MerchantCell::onEnter() {
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    ToolController::getInstance()->m_merchantCurrIndex=0;
}

void MerchantCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler MerchantCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool MerchantCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_clickInSide = false;
    m_num = 0;
    if (isTouchInside(m_touchNode,pTouch)) {
        m_clickInSide = true;
        if (isTouchInside(this,pTouch)) {
            m_startPoint=pTouch->getStartLocation();
            if(isTouchInside(m_picBG1,pTouch)){
                this->schedule(schedule_selector(MerchantCell::setLeft),0.2);
            }else if(isTouchInside(m_buyBtnSpr1,pTouch)){
                if(m_info1->num<=0){
                    return false;
                }else{
                    m_buyBtnSpr1->setScale(1.2);
                }
            }
            return true;
        }
    }
    
    return false;
}
void MerchantCell::setLeft(float dt){
    this->unschedule(schedule_selector(MerchantCell::setLeft));
    setDesNode(m_info1,0);
}
void MerchantCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_buyBtnSpr1->setScale(1.0);
    m_desNode->setVisible(false);
    this->unschedule(schedule_selector(MerchantCell::setLeft));
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    if(isTouchInside(m_buyBtnSpr1,pTouch)){
        onClickBuyBtn1(NULL,Control::EventType::TOUCH_DOWN);
    }
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    layer->removeAllChildren();
}
void MerchantCell::setDesNode(MerchantToolInfo* minfo,int index){
//    if(minfo->itemId<WorldResource_Max)
    if(m_info1->type == MerchantTool_RESOURCE)
        return;
    
    m_desNode->setVisible(true);
    m_desNode->setPositionX(290);
    if(m_info1->type == MerchantTool_GOODS){
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(minfo->itemId);
        if(info.type==5 && !info.getPara().empty()){
            m_desNode->setVisible(false);
            auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
            if(layer->getChildrenCount()<=0){
                StoreMallCellTip* tip = StoreMallCellTip::create(info.getPara(),info.getName());
                layer->addChild(tip);
                float x = m_startPoint.x;
                if(index==1){
                    x =100;
                }
                tip->setPosition(x, m_startPoint.y);
            }
        }else{
            m_desName->setString(info.getName().c_str());
            m_desLabel->setString(_lang(info.des).c_str());
        }
    }else if( m_info1->type == MerchantTool_EQUIP){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_info1->itemId];
        string ename = _lang(eInfo.name);
        m_desName->setString(ename);
        m_desLabel->setString(_lang(eInfo.des).c_str());
    }
    
}
void MerchantCell::onClickBuyBtn1(CCObject * pSender, Control::EventType pCCControlEvent){
    if(ToolController::getInstance()->getMerchante_animation()==true)
        return;
    
    if(!m_clickInSide){
        return;
    }
    if(m_isClick)
        return;
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    m_isClick = true;
    m_bBuy = false;
    int price = m_info1->price;
    StoreBuyConfirmDialog *dialog  = NULL;
//    if(m_info1->itemId<WorldResource_Max){
    if(m_info1->type == MerchantTool_RESOURCE){
        dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getResourceIconByType(m_info1->itemId).c_str(), m_nameLabel1->getString().c_str(), "", price,-1,CCCallFuncO::create(this, callfuncO_selector(MerchantCell::onBuyTool1), NULL),m_picBG1->convertToWorldSpace(CCPointZero),m_info1->priceType);
    }else if(m_info1->type == MerchantTool_GOODS){
        auto &info = ToolController::getInstance()->getToolInfoById(m_info1->itemId);
        dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), m_nameLabel1->getString().c_str(), _lang(info.des).c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(MerchantCell::onBuyTool1), NULL),m_picBG1->convertToWorldSpace(CCPointZero),m_info1->priceType);
    }else if(m_info1->type == MerchantTool_EQUIP){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_info1->itemId];
        dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(eInfo.itemId)).c_str(), m_nameLabel1->getString().c_str(), _lang(eInfo.des).c_str(), price,eInfo.color, CCCallFuncO::create(this, callfuncO_selector(MerchantCell::onBuyTool1), NULL),m_picBG1->convertToWorldSpace(CCPointZero),m_info1->priceType);
    }
    if(dialog){
        dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(MerchantCell::onCloseConfirm)));
    }
}
void MerchantCell::onBuyTool1(CCObject *ccObj){
    if(m_bIsBuying==true)
        return;
    m_bIsBuying=true;
    ToolController::getInstance()->m_merchantCurrIndex = m_index+1;
    ToolController::getInstance()->buyMerchantTool(CC_ITOA(m_info1->itemId),m_info1->itemNum,m_info1->price,m_info1->priceType, CCCallFunc::create(this, callfunc_selector(MerchantCell::onRetBuyTool)));
}
void MerchantCell::onRetBuyTool(){
//    setLeftHot(false,m_info1->num);
    m_bBuy = true;
    ToolController::getInstance()->m_merchantCurrIndex = 0;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY_CONFIRM_OK);
}
void MerchantCell::onCloseConfirm()
{
    m_isClick=false;
    if(m_bBuy == true){
        playAnimation();
        m_bBuy=false;
    }else{
        setData();
        animationCallback();
    }
    m_bIsBuying=false;
}
void MerchantCell::changeData(){
    setData();
}
void MerchantCell::playAnimation(bool bRefersh){
    m_bRefreshClick = bRefersh;
    if(m_shiningNode){
        m_shiningNode->startPlay(false);
        m_shiningNode->setVisible(false);
    }
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(MerchantCell::animationCallback));
    this->getAnimationManager()->runAnimationsForSequenceNamed("1");
    this->runAction(CCSequence::create(CCDelayTime::create(0.5),CCCallFunc::create(this, callfunc_selector(MerchantCell::changeData)),NULL));
    
}
void MerchantCell::animationCallback(){
    this->getAnimationManager()->runAnimationsForSequenceNamed("0");
    if(m_shiningNode){
        m_shiningNode->setVisible(true);
        m_shiningNode->startPlay(true);
    }
    if(m_bRefreshClick==true){
        m_bRefreshClick=false;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MERCHANTE_REFRESH_ANIMATION_FINISH);
}
bool MerchantCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel1", CCLabelIF*, m_nameLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel1", CCLabelIF*, m_priceLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode1", CCNode*, m_itemNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG1", CCSprite*, m_picBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtnSpr1", CCScale9Sprite*, m_buyBtnSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saleoutSpr1", CCScale9Sprite*, m_saleoutSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtngraySpr1", CCScale9Sprite*, m_buyBtngraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costNode1", CCNode*, m_costNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemHotNode1", CCNode*, m_itemHotNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotSpr1", CCSprite*, m_hotSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotgraySpr1", CCSprite*, m_hotgraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumSpr1", CCSprite*, m_hotNumSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumgraySpr1", CCSprite*, m_hotNumgraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotdesText1", CCLabelIF*, m_hotdesText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNum1Text", CCLabelIF*, m_hotNum1Text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotLineSpr1", CCScale9Sprite*, m_hotLineSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotpriceLabel1", CCLabelIF*, m_hotpriceLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemInfoNode", CCNode*, m_itemInfoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_waitingNode", CCNode*, m_waitingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_superNode", CCNode*, m_superNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblSuper", CCLabelIF*, m_lblSuper);
    return false;
}
void MerchantCell::setLeftHot(bool ishot,int num){
//    if(ishot){
//        if(num>0){
//            m_hotgraySpr1->setVisible(false);
//            m_hotSpr1->setVisible(true);
//        }else{
//            m_hotgraySpr1->setVisible(true);
//            m_hotSpr1->setVisible(false);
//        }
//        m_hotdesText1->setString(_lang("101224"));
//        m_itemHotNode1->setVisible(true);
//    }else{
//        m_itemHotNode1->setVisible(false);
//    }
//    m_hotNum1Text->setString(CC_CMDITOA(num));
//    if(num>0){
//        m_buyBtngraySpr1->setVisible(false);
//        m_buyBtnSpr1->setVisible(true);
//        m_saleoutSpr1->setVisible(false);
//        m_hotNumSpr1->setVisible(true);
//        m_hotNumgraySpr1->setVisible(false);
//        m_nameLabel1->setColor(ccc3(194, 158, 116));
//        m_priceLabel1->setColor(ccc3(255,255,0));
//        m_hotpriceLabel1->setColor(ccc3(221,172,33));
//    }else{
//        m_buyBtngraySpr1->setVisible(true);
//        m_buyBtnSpr1->setVisible(false);
//        m_saleoutSpr1->setVisible(true);
//        m_hotNumSpr1->setVisible(false);
//        m_hotNumgraySpr1->setVisible(true);
//        m_nameLabel1->setColor(ccc3(156,156,156));
//        m_priceLabel1->setColor(ccc3(156,156,156));
//        m_hotpriceLabel1->setColor(ccc3(156,156,156));
//        auto pic = dynamic_cast<CCSprite*>(m_picNode1->getChildByTag(99));
//        if(pic){
//            pic->setColor(ccc3(90,85,81));
//        }
//        auto pic1 = dynamic_cast<CCSprite*>(m_picNode1->getChildByTag(100));
//        if(pic1){
//            pic1->setColor(ccc3(90,85,81));
//        }
//        CCSprite* costPic = dynamic_cast<CCSprite*>(m_costNode1->getChildByTag(100));
//        if(costPic){
//            costPic->setColor(ccc3(90,85,81));
//        }
//    }
}
MerchantTabelCell* MerchantTabelCell::create(int index, CCNode* touchNode){
    auto ret = new MerchantTabelCell();
    if (ret && ret->init(index, touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MerchantTabelCell::init(int index,CCNode* touchNode){
    bool ret = true;
    mCell = MerchantCell::create(index, touchNode);
    if(mCell){
        addChild(mCell);
    }
    return ret;
}
void MerchantTabelCell::setData(int index,bool bAnimate){
    if(mCell){
        if(!bAnimate){
            mCell->setDataIndex(index);
            mCell->setData();
            mCell->animationCallback();
        }else{
            mCell->setDataIndex(index);
            if(index>0){
                mCell->runAction(CCSequence::create(CCDelayTime::create(index*0.3),CCCallFunc::create(this, callfunc_selector(MerchantTabelCell::playAnimation)),NULL));
            }else{
                playAnimation();
            }
        }
    }
}
void MerchantTabelCell::playAnimation(){
    if(mCell){
        mCell->playAnimation(true);
    }
}
MerchantShining::~MerchantShining(){
//    this->unschedule(schedule_selector(MerchantShining::playFrame));
}
void MerchantShining::onEnter(){
    CCNode::onEnter();
}
void MerchantShining::onExit(){
    CCNode::onExit();
}
MerchantShining* MerchantShining::create(const ccColor3B &color,const CCSize size){
    auto ret = new MerchantShining();
    if(ret && ret->init(color,size)){
        ret->autorelease();
    }else{
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}
bool MerchantShining::init(const ccColor3B &color,const CCSize size){
    if(CCNode::init()){
        CCLoadSprite::doResourceByCommonIndex(102, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(102, false);
        });
        mBottomSprite = CCLoadSprite::createSprite("TravelBusiness.png");
        mBottomSprite->setAnchorPoint(ccp(0.5, 0));
        CCSize orgSize = mBottomSprite->getContentSize();
        float fScaleX = size.width / orgSize.width;
        float fScaleY = size.height*0.5 / orgSize.height;
        mBottomSprite->setScaleX(fScaleX);
        mBottomSprite->setScaleY(-fScaleY);
        addChild(mBottomSprite);
        mBottomSprite->setPositionY(0);
        mTopSprite = CCLoadSprite::createSprite("TravelBusiness.png");
        mTopSprite->setAnchorPoint(ccp(0.5, 0));
        mTopSprite->setScaleX(fScaleX);
        mTopSprite->setScaleY(fScaleY);
        addChild(mTopSprite);
        mTopSprite->setPositionY(0);
        
        mBottomSprite->setColor(color);
        mTopSprite->setColor(color);
        ccBlendFunc cbf1 = {GL_SRC_ALPHA, GL_ONE};
        mTopSprite->setBlendFunc(cbf1);
        mBottomSprite->setBlendFunc(cbf1);
        
//        CCSpriteFrame *pEffFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ButtonLight_1.png");
//        pEffFrame->getTexture()->setAntiAliasTexParameters();
        CCSize effSize = size;
        float delayT =CCMathUtils::getRandomInt(2, 5);
        mEffSprite = ButtonLightEffect::create(effSize,color,false,delayT);
        this->addChild(mEffSprite);
        mEffSprite->setBlendFunc(cbf1);
        m_startPlay = true;
//        m_maxEffDelay = CCMathUtils::getRandomInt(50, 80);
//        m_curEffDelay = m_maxEffDelay;
//        m_playEff = true;
        return true;
    }
    return false;
}
//void MerchantShining::playFrame(float fUpdate){
//    if(m_playEff == true){
//        ++m_curEffFrame;
//        if((m_curEffFrame+1) > MERCHANTE_SHINE_EFF_MAX_FRAME){
//            mEffSprite->setVisible(false);
//            m_playEff = false;
//        }else{
//            std::string nowEffFrame = std::string("ButtonLight_") + CC_ITOA(m_curEffFrame) + ".png";
//            CCSpriteFrame *pEffFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(nowEffFrame.c_str());
//            while (!pEffFrame){
//                CCLoadSprite::doResourceByCommonIndex(102, true);
//                pEffFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(nowEffFrame.c_str());
//            }
//            pEffFrame->getTexture()->setAntiAliasTexParameters();
//            mEffSprite->setDisplayFrame(pEffFrame);
//        }
////        if((m_curEffFrame+1) > MERCHANTE_SHINE_MAX_FRAME){
////            CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("TravelBusiness_1.png");
////            mBottomSprite->setDisplayFrame(pFrame);
////            mTopSprite->setDisplayFrame(pFrame);
////            m_playEff = false;
////        }else{
////            std::string nowFrame = std::string("TravelBusiness_") + CC_ITOA(m_curEffFrame) + ".png";
////            CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(nowFrame.c_str());
////            mBottomSprite->setDisplayFrame(pFrame);
////            mTopSprite->setDisplayFrame(pFrame);
////        }
//    }else{
//        --m_curEffDelay;
//        if(m_curEffDelay <=0){
//            m_playEff = true;
//            m_curEffFrame = 1;
//            m_curEffDelay = m_maxEffDelay;
//            CCSpriteFrame *pEffFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ButtonLight_1.png");
//            while (!pEffFrame){
//                CCLoadSprite::doResourceByCommonIndex(102, true);
//                pEffFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ButtonLight_1.png");
//            }
//            pEffFrame->getTexture()->setAntiAliasTexParameters();
//            mEffSprite->setDisplayFrame(pEffFrame);
//            mEffSprite->setVisible(true);
//        }
//    }
//}
void MerchantShining::setShineColor(const ccColor3B &color){
    mBottomSprite->setColor(color);
    mTopSprite->setColor(color);
    mEffSprite->setEffColor(color);
}
void MerchantShining::setShineAlpha(const int alpha){
    mBottomSprite->setOpacity(alpha);
    mTopSprite->setOpacity(alpha);
    m_fAlpha=alpha;
}
void MerchantShining::showEff2(bool show,float scale){
    if(mEffSprite1){
        mEffSprite1->removeFromParent();
        mEffSprite1=NULL;
    }
    if(show){
        mEffSprite1 = ParticleController::createParticle("TravelBusiness");
        this->addChild(mEffSprite1);
        CCSize size = mTopSprite->getContentSize();
        mEffSprite1->setPositionY(size.height + 20*scale);
        mEffSprite1->setScale(scale);
        mEffSprite1->resumeSchedulerAndActions();
    }
}
void MerchantShining::startPlay(bool play){
    if(m_startPlay && !play){
        mEffSprite->stopEffect();
//        this->unschedule(schedule_selector(MerchantShining::playFrame));
        m_startPlay=false;
    }else if(!m_startPlay && play){
        mEffSprite->playEffect();
//        this->schedule(schedule_selector(MerchantShining::playFrame), 0.07);
        m_startPlay=true;
    }
}
