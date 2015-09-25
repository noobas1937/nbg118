//
//  LotteryActView.cpp
//  IF
//
//  Created by chenfubi on 15/1/13.
//
//

#include "LotteryActView.h"
#include "LotteryActCommand.h"
#include "PopupViewController.h"
#include "LotteryAct2View.h"
#include "RewardController.h"
#include "YesNoDialog.h"
#include "GCMRewardController.h"
#include "ParticleController.h"
#include "LotteryRotateView.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "ParticleFireAni.h"
#include "SoundController.h"
#include "SceneController.h"

LotteryActView* LotteryActView::create(bool playAni/*=false*/)
{
    LotteryActView* ret = new LotteryActView();
    if (ret && ret->init(playAni))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LotteryActView::init(bool playAni)
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(11, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        setCleanFunction([](){
            //CCLoadSprite::doResourceByCommonIndex(11, false);
            //CCLoadSprite::doResourceByCommonIndex(8, false);
        });

        auto ccb = CCBLoadFile("LotteryActView", this, this);
        this->setContentSize(ccb->getContentSize());
        int extH = getExtendHeight();
        float oldH = m_bg0->getContentSize().height;
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        float newH = winSize.height;
        if (newH > oldH)
        {
            m_bg0->setScaleY(newH/oldH);
            m_bg1->setScaleY(newH/oldH);
        }
        m_listNode->setPositionY(m_listNode->getPositionY() - extH/2);
        m_buyNode->setPositionY(m_buyNode->getPositionY() - extH);
        m_colorLayer->setContentSize(winSize);
        m_colorLayer->setPositionY(m_colorLayer->getPositionY() - extH);
        
        m_popLayer = CCLayer::create();
        initRewardMap();
//        m_superNode->setVisible(LotteryController::shared()->lotteryInfo.lotteryCnt>=5);
        m_superNode->setVisible(false);
        m_superIcon->setVisible(LotteryController::shared()->lotteryInfo.superMode == 1);
        m_infoTTF1->setString(_lang("111105"));
        m_superTTF->setString(_lang_1("111104", "5"));
        initInfoTTF2();
        m_goldTTF->setString(_lang("111116"));
        
        bool lotteryGuide = CCUserDefault::sharedUserDefault()->getBoolForKey("LOTTERYGUIDE");
        m_aniGuide->setVisible(!lotteryGuide);
        m_lightNode->setVisible(false);
        
        ParticleFireAni* par = ParticleFireAni::create();
        par->setPosition(ccp(50.0f, 0.0f));
        m_fireNode->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        par2->setPosition(ccp(555.0f, 0.0f));
        m_fireNode->addChild(par2);
        
        m_goldNum->setString(_lang("102153"));
		bRet = true;
        if (playAni)
        {
            this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(LotteryActView::openComplete));
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
        }
        m_arrowSp->setVisible(false);
        
        if(LotteryController::shared()->getSendCMD()==true){
            m_waitInterface= GameController::getInstance()->showWaitInterface(m_colorLayer,-1);
            CCSize wSize = m_colorLayer->getContentSize();
            CCPoint wPos = m_colorLayer->getPosition();
            m_waitInterface->setPosition(ccp(wPos.x + wSize.width*0.5, wSize.height*0.5));
            m_colorLayer->setVisible(true);
        }else{
            m_waitInterface = NULL;
        }
	}while(0);
	return bRet;
}

void LotteryActView::initInfoTTF2()
{
    m_infoTTF2->setVisible(false);
    return;
    
    int hasCnt = LotteryController::shared()->lotteryInfo.lotteryCnt;
    int lastCnt = 5 - hasCnt;
    m_infoTTF2->setString(_lang_1("111103", CC_ITOA(lastCnt)));
    m_infoTTF2->setVisible(lastCnt>0);
}

void LotteryActView::openComplete()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    this->getAnimationManager()->runAnimationsForSequenceNamed("0");
}

void LotteryActView::updateAccount(CCObject* pObj)
{
    int cost = LotteryController::shared()->getLotteryCost1();
    long total = GlobalData::shared()->resourceInfo.lChip;
    m_chipTTF2->setString(CC_CMDITOA(total));
    if (cost == 0)
    {
        m_infoTTF3->setString(_lang("111124"));
    }
    else
    {
        m_infoTTF3->setString(_lang_1("111125", CC_CMDITOA(cost).c_str()));
    }
}

void LotteryActView::initRewardMap()
{
    m_rewardMap.clear();
    CCArray* arr = LotteryController::shared()->getLotteryReward();
    CCString* str = NULL;
    for (int i = 0; i < arr->count(); i++)
    {
        vector<string> strVec;
        str = dynamic_cast<CCString*>(arr->objectAtIndex(i));
        if (str)
        {
            CCCommonUtils::splitString(str->getCString(), ":", strVec);
            m_rewardMap[i] = strVec;//strVec.at(0):ID,strVec.at(1):cnt
        }
    }
    
    m_aniNode->removeAllChildren();
    LotteryRotateView* rotateView = LotteryRotateView::create();
    rotateView->setRotateData(m_rewardMap, false);
    rotateView->setTag(999);
    m_aniNode->addChild(rotateView);
}

void LotteryActView::playAnimation(CCObject* pObj/*=null*/)
{
    if(LotteryController::shared()->getSendCMD()==true)
        return;
        
    int useGold = 0;
    CCString* s = dynamic_cast<CCString*>(pObj);
    if (s != NULL)
    {
        useGold = s->intValue();
    }
    
    LotteryController::shared()->setSendCMD(true);
    LotteryActCommand* cmd = new LotteryActCommand(-1, useGold, 1);
//    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(LotteryActView::commandCallBack), NULL));
    cmd->sendAndRelease();
}

void LotteryActView::commandCallBack(CCObject *pObj)
{
    m_cmdComplete = true;
    NetResult* result = dynamic_cast<NetResult*>(pObj);
    if (result == NULL)
    {
        return;
    }
    m_rewardDic = _dict(result->getData());
    const CCString *pStr = m_rewardDic->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        m_aniComplete = true;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACTVIEW_GET, ccs("error"));
        return ;
    }
//    double updateTime = m_rewardDic->valueForKey("updatetime")->doubleValue();
//    LotteryController::shared()->setUpdateTime(updateTime);
    updateAccount(NULL);
    CCDictionary* rewardsDic = _dict(m_rewardDic->objectForKey("rewards"));
    if (rewardsDic->count() == 0)
    {
        m_aniComplete = true;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACTVIEW_GET, ccs("error"));
        return ;
    }
    m_rewardId = rewardsDic->valueForKey("rewardId")->intValue();

    if (m_rewardId == 0)
    {
        m_itemId = rewardsDic->valueForKey("type")->getCString();
        m_itemCnt = rewardsDic->valueForKey("value")->intValue();
    }
    else if (m_rewardId == 100)
    {
        m_itemId = "100";
    }
    else
    {
        if (rewardsDic->objectForKey("items"))
        {
            CCArray* arr = dynamic_cast<CCArray*>(rewardsDic->objectForKey("items"));
            CCObject* obj = NULL;
            CCARRAY_FOREACH(arr, obj){
                auto dict = dynamic_cast<CCDictionary*>(obj);
                int type = dict->valueForKey("type")->intValue();
                if (type == R_GOODS)
                {
                    m_itemId = _dict(dict->objectForKey("value"))->valueForKey("itemId")->getCString();
                    m_itemCnt = _dict(dict->objectForKey("value"))->valueForKey("rewardAdd")->intValue();
                    mIsGoods=true;
                }
                else
                {
                    m_itemId = CC_ITOA(type);
                    m_itemCnt = dict->valueForKey("value")->intValue();
                }
            }
        }
    }
    
    if (atoi(m_itemId.c_str()) == R_CHIP)
    {
        int cnt = GlobalData::shared()->resourceInfo.lChip - m_itemCnt;
        m_chipTTF2->setString(CC_CMDITOA(cnt));
    }
    m_aniComplete = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACTVIEW_GET, ccs(m_itemId));
}

void LotteryActView::doRewardAct()
{
    m_canTouch = true;
    
    //todocfb supermodel
//    if (!m_superNode->isVisible())
//    {
//        m_superNode->setVisible(LotteryController::shared()->lotteryInfo.lotteryCnt>=5);
//        initInfoTTF2();
//    }
    
    if (!m_rewardDic || m_rewardDic->count()==0 || (m_itemId.length()==0 && m_itemCnt==0))
    {
        return;
    }
    
    CCDictionary* rewardsDic = _dict(m_rewardDic->objectForKey("rewards"));

    if (m_rewardId == 0)
    {
        CCArray* arr = CCArray::create();
        arr->addObject(rewardsDic);
//        GCMRewardController::getInstance()->flyReward(GCMRewardController::getInstance()->getFlyArr(arr));
        int id = atoi(m_itemId.c_str());
        CCCommonUtils::flyHintWithDefault(getIcon(m_itemId), "", _lang_1("104913", LotteryController::shared()->getLotteryName(id, m_itemCnt).c_str()));
    }
    else if (m_rewardId == 100)
    {
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(LotteryActView::closeComplete));
        this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
    }
    else
    {
//        if (rewardsDic->objectForKey("items"))
//        {
//            CCArray* arr = dynamic_cast<CCArray*>(rewardsDic->objectForKey("items"));
//            CCDictionary* d = dynamic_cast<CCDictionary*>(arr->objectAtIndex(0));
//            if (d && d->valueForKey("type")->intValue() != 7)
//            {
//                GCMRewardController::getInstance()->flyReward(GCMRewardController::getInstance()->getFlyArr(arr));
//            }
//            else
//            {
//                GCMRewardController::getInstance()->flyToolReward(GCMRewardController::getInstance()->getFlyArr(arr));
//            }
//        }
        int id = atoi(m_itemId.c_str());
        CCCommonUtils::flyHintWithDefault(getIcon(m_itemId), "", _lang_1("104913", LotteryController::shared()->getLotteryName(id, m_itemCnt).c_str()));
    }
    m_rewardDic = NULL;
    m_itemCnt=0;
   
}

void LotteryActView::closeComplete()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    this->getAnimationManager()->runAnimationsForSequenceNamed("0");
    LotteryController::shared()->lotteryInfo.type = 2;
    PopupViewController::getInstance()->removePopupView(this);
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_putdown);
    PopupViewController::getInstance()->addPopupInView(LotteryAct2View::create());
}

void LotteryActView::addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        auto &batch = m_parPopVec[batchCount];
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
    m_popLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parPopVec.push_back(newBatch);
}

void LotteryActView::showPopParticle(CCObject* pObj)
{
    if (m_itemCnt==0 && m_itemId.length()==0)
    {
        removePopParticle(1.5);
        m_arrowSp->setVisible(false);
        m_arrowSp->stopAllActions();
        this->scheduleOnce(schedule_selector(LotteryActView::rewardAniBack), 3.0f);
        return;
    }
    removePopParticle(0);
    m_popNode->addChild(m_popLayer);
    
    int curColor = -1;
    curColor = atoi(CCCommonUtils::getPropById(m_itemId, "color").c_str());
    
    string tmpStart = "MallBag_";
    int count = 3;
    CCPoint p = ccp(0, 0);
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(p);
        addPopParticleToBatch(particle);
    }
    
    m_popLayer->removeChildByTag(2);
    string strEquipTmp = getIcon(m_itemId);
    auto iconEquipTmp = mIsGoods? CCLoadSprite::createSprite(strEquipTmp.c_str(), true,CCLoadSpriteType_GOODS) : CCLoadSprite::createSprite(strEquipTmp.c_str(),true,CCLoadSpriteType_DEFAULT);
    iconEquipTmp->setPosition(p);
    iconEquipTmp->setTag(2);
    m_popLayer->addChild(iconEquipTmp);
    
    iconEquipTmp->setScale(0.1);
    CCFiniteTimeAction *pAction1 = CCSequence::create(CCScaleTo::create(0.35, 1.5),CCScaleTo::create(0.15, 1.2),NULL);
    iconEquipTmp->runAction(CCSequence::create(pAction1, NULL));
    SoundController::sharedSound()->stopAllEffects();
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_loteryrwd);
    this->scheduleOnce(schedule_selector(LotteryActView::removePopParticle), 1.5);
    
    doRewardAct();
    m_arrowSp->setVisible(false);
    m_arrowSp->stopAllActions();
    this->scheduleOnce(schedule_selector(LotteryActView::rewardAniBack), 3.0f);
}

void LotteryActView::removePopParticle(float dt)
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        m_popLayer->removeChild(m_parPopVec[batchCount]);
    }
    m_parPopVec.clear();
    
    m_popNode->removeAllChildren();
    if (dt != 0)
    {
//        doRewardAct();
//        m_arrowSp->setVisible(false);
//        m_arrowSp->stopAllActions();
//        this->scheduleOnce(schedule_selector(LotteryActView::rewardAniBack), 3.0f);
    }
}

void LotteryActView::rewardAniBack(float dt)
{
    if (atoi(m_itemId.c_str()) == R_CHIP)
    {
        int cnt = GlobalData::shared()->resourceInfo.lChip;
        m_chipTTF2->setString(CC_CMDITOA(cnt));
    }
    isChipEnough();
}

string LotteryActView::getIcon(string itemId)
{
    std::string fix = ".png";
    switch (atoi(itemId.c_str()))
    {
        case Silver:
            return "Lottery_Mithril" + fix;
        case Wood:
            return "Lottery_wood" + fix;
        case Iron:
            return "Lottery_stone" + fix;
        case Food:
            return "Lottery_food" + fix;
        case Stone:
            return "Lottery_Mithril" + fix;
        case Gold:
            return "Lottery_gold" + fix;
        case R_CHIP:
            return "Lottery_bargaining-chip1" + fix;
        case R_DIAMOND:
            return "Lottery_bargaining-chip2" + fix;
        case 100:
            return "Lottery_baoxiang.png";
        default:
            return CCCommonUtils::getIcon(itemId);
    }
    return std::string("");
}

void LotteryActView::onSuperModeClick()
{
    return;//todocfb转盘关闭超级模式
    if (LotteryController::shared()->lotteryInfo.superMode == 0)
    {
        LotteryController::shared()->lotteryInfo.superMode = 1;
    }
    else
    {
        LotteryController::shared()->lotteryInfo.superMode = 0;
    }
    m_superIcon->setVisible(LotteryController::shared()->lotteryInfo.superMode == 1);
    updateAccount(NULL);
    initRewardMap();
}

void LotteryActView::rotateListener(cocos2d::CCObject *pObj)
{
    CCDictionary* dict = _dict(pObj);
    string key = dict->valueForKey("key")->getCString();
    if (key == LOTTERYACTVIEW_COM)
    {
        if (strcmp(dict->valueForKey("value")->getCString(), "error") != 0)
        {
            showPopParticle(pObj);
        }
        else
        {
            m_arrowSp->setVisible(false);
            m_arrowSp->stopAllActions();
            rewardAniBack(0);
        }
        m_aniComplete = true;
        m_particleNode->removeAllChildren();
        m_lightNode->setVisible(false);
        this->getAnimationManager()->runAnimationsForSequenceNamed("0");
    }
    else if (key == LOTTERYACTVIEW_SEND)
    {
        LotteryRotateView* rotateView = dynamic_cast<LotteryRotateView*>(m_aniNode->getChildByTag(999));
        rotateView->setEnabled(false);
        playAnimation(dict->objectForKey("value"));
        int direction = dict->valueForKey("value1")->intValue();
        int count = 2;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d","LuckTurntable_",i)->getCString());
            particle->setPositionType(kCCPositionTypeRelative);
            if (direction == 1)
            {
                particle->setScale(-1);
                particle->setPosition(ccp(-286, 0));
            }
            m_particleNode->addChild(particle);
        }
        m_arrowSp->setVisible(true);
        m_lightNode->setVisible(true);
//        for (int i = 0; i < m_lightNode->getChildrenCount(); i++)
//        {
//            m_lightNode->getChildByTag(i)->setVisible(true);
//        }
        this->getAnimationManager()->runAnimationsForSequenceNamed("1");
    }
    else if (key == LOTTERYACTVIEW_UPDATE)
    {
        updateAccount(NULL);
    }
    else if (key == "HIDEGUIDENODE")
    {
        m_aniGuide->setVisible(false);
    }
    else if (key == "REMOVEPARTICLE")
    {
        int value = dict->valueForKey("value")->intValue();
        if (value == 0)
        {
            m_particleNode->removeAllChildren();
        }
        else if (value == 1)
        {
            m_lightNode->setVisible(false);
//            for (int i = 0; i < m_lightNode->getChildrenCount(); i++)
//            {
//                m_lightNode->getChildByTag(i)->setVisible(true);
//                if (i%value == 0)
//                {
//                    m_lightNode->getChildByTag(i)->setVisible(false);
//                }
//            }
        }
    }
}

bool LotteryActView::isChipEnough()
{
    m_buyNode->setVisible(false);
    m_colorLayer->setVisible(m_waitInterface?true:false);
    m_buyBtn->setEnabled(true);
    LotteryRotateView* rotateView = dynamic_cast<LotteryRotateView*>(m_aniNode->getChildByTag(999));
    int cost = LotteryController::shared()->getLotteryCost1() - GlobalData::shared()->resourceInfo.lChip;
    if (cost > 0)
    {
        m_buyNode->setVisible(true);
        rotateView->setEnabled(false);
        m_colorLayer->setVisible(true);
        onGameTick(0);
        return false;
    }
    rotateView->setEnabled(m_waitInterface?false:true);
    return true;
}

void LotteryActView::onBuyBtnClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    CCDictionary* dict = CCDictionary::create();
    PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_LOTTERY1,dict,"111101"));
    
}

void LotteryActView::buyChipBack(cocos2d::CCObject *pObj)
{
    updateAccount(NULL);
    isChipEnough();
}

void LotteryActView::onGameTick(float dt)
{
    if (m_buyNode->isVisible())
    {
        double timeStamp = GlobalData::shared()->getTimeStamp();
        if(GlobalData::shared()->tomorrow_time <= timeStamp){
            GlobalData::shared()->tomorrow_time += 86400;
            isChipEnough();
        }else{
            double lastTime = GlobalData::shared()->tomorrow_time - timeStamp;
            m_timeTTF->setString(_lang_1("111128", CC_SECTOA(lastTime)));
        }
//        time_t infoTime = GlobalData::shared()->getTimeStamp();
//        string t2 = CCCommonUtils::timeStampToUTCHSM(infoTime);
//        vector<string> timeVec;
//        CCCommonUtils::splitString(t2, ":", timeVec);
//        int pastTime = 0;
//        if (timeVec.size() >= 3)
//        {
//            pastTime = atoi(timeVec.at(0).c_str())*3600+atoi(timeVec.at(1).c_str())*60+atoi(timeVec.at(2).c_str());
//        }
//        int lastTime = 24*3600 - pastTime;
//        
//        if (lastTime <= 0)
//        {
//            isChipEnough();
//        }
    }
}

void LotteryActView::onEnter()
{
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryActView::commandCallBack), LOTTERYACTVIEWCMD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryActView::rotateListener), LOTTERYROTATEVIEW, NULL);
    isChipEnough();
    setTitleName(_lang("111100"));
    updateAccount(NULL);
    this->schedule(schedule_selector(LotteryActView::onGameTick), 1.0f);
}

void LotteryActView::onExit()
{
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYACTVIEWCMD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYROTATEVIEW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "BUYCHIPBACK");
    this->unschedule(schedule_selector(LotteryActView::rewardAniBack));
    this->unschedule(schedule_selector(LotteryActView::onGameTick));

    CCNode::onExit();
    if(m_rewardDic&&m_rewardDic->count()>0&&m_itemId!=""&&m_itemCnt!=0)
    {
        doRewardAct();
    }
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
    CCLoadSprite::doResourceByCommonIndex(11, false);
    CCLoadSprite::doResourceByCommonIndex(8, false);
}

bool LotteryActView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    if(layer){
        FlyHint* flyHit = dynamic_cast<FlyHint*>(layer->getChildByTag(19999));
        if(flyHit){
            flyHit->stopAllActions();
            flyHit->removeSelf();
            this->unschedule(schedule_selector(LotteryActView::removePopParticle));
            removePopParticle(0.0f);
            this->unschedule(schedule_selector(LotteryActView::rewardAniBack));
            rewardAniBack(0.0f);
            return false;
        }
    }
//    m_touchTime = clock() / 1000;
    return true;
}

void LotteryActView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_superBg, pTouch))
    {
        onSuperModeClick();
        return;
    }
}

bool LotteryActView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_superNode", CCNode*, m_superNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popNode", CCNode*, m_popNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniGuide", CCNode*, m_aniGuide);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_chipTTF2", CCLabelIF*, m_chipTTF2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF1", CCLabelIF*, m_infoTTF1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF2", CCLabelIF*, m_infoTTF2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF3", CCLabelIF*, m_infoTTF3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_superTTF", CCLabelIF*, m_superTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_superBg", CCSprite*, m_superBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_superIcon", CCSprite*, m_superIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg0", CCSprite*, m_bg0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCSprite*, m_bg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSp", CCSprite*, m_arrowSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniNode", CCNode*, m_aniNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lightNode", CCNode*, m_lightNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyNode", CCNode*, m_buyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, m_goldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldTTF", CCLabelIF*, m_goldTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_colorLayer", CCLayerColor*, m_colorLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCControlButton*, m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode", CCNode*, m_fireNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTTF", CCLabelIF*, m_timeTTF);
    return false;
}

SEL_CCControlHandler LotteryActView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBuyBtnClick", LotteryActView::onBuyBtnClick);
    return NULL;
}


