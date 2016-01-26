 //
//  SpeBuild.cpp
//  IF
//
//  Created by fubin on 14-5-15.
//
//

#include "SpeBuild.h"
#include "SceneController.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "ScienceListView.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "FunBuildController.h"
#include "BuildingHospitalPopUpView.h"
#include "AllianceMarketView.h"
#include "BattleView.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ImperialScene.h"
#include "TroopsView.h"
#include "TileOpenView.h"
#include "ActivityController.h"
#include "FacebookCollectView.h"
#include "FacebookMineView.h"
#include "UserUpgradeView.h"
#include "FBMineDigCommand.h"
#include "SocialController.h"
#include "BaoZangView.h"
#include "MerchantView.h"
#include "AllianceIntroTip.h"
#include "TimeRwdShowView.h"
#include "../../Ext/CCDevice.h"
#include "DailyActiveController.h"
#include "LotteryController.h"
#include "GoldExchangeInfoCommand.h"
#include "InviteForGiftView.h"
#include "NewPlayerQianDaoView.h"
#include "QuestController.h"
#include "DailyActiveView.h"
#include "TrainingView.h"
#include "LotteryActView.h"
#include "LotteryAct2View.h"
#include "MonthCardView.h"
#include "PortActView.h"
#include "DragonBattleJoinView.h"
#include "AllianceManager.h"
#include "NewTroopsView.h"
using namespace cocos2d;

SpeBuild* SpeBuild::create(int itemId)
{
    SpeBuild *pRet = new SpeBuild();
    if (pRet && pRet->initSpeBuild(itemId))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool SpeBuild::initSpeBuild(int itemId)
{
    m_buildingKey = itemId;
    m_touchName = "";
    
    if(m_sprArray) {
        m_sprArray->removeAllObjects();
        m_personArray->removeAllObjects();
        m_blentSprArray->removeAllObjects();
    }
    else {
        m_sprArray = CCArray::create();
        m_personArray = CCArray::create();
        m_blentSprArray = CCArray::create();
    }
    
    if(true)
    {
        m_mainNode = CCNode::create();
        this->addChild(m_mainNode);
        m_isOpen = false;
        //string ccbName = "wasteland_"; //d by ljf
        string ccbName = "wasteland_new_"; //a by ljf
        bool isTile = true;
        if (itemId == BIG_TILE_1) {
            ccbName = ccbName+CC_ITOA(GlobalData::shared()->contryResType)+"_1";
        }
        else if (itemId == BIG_TILE_2) {
            ccbName = ccbName+CC_ITOA(GlobalData::shared()->contryResType)+"_2";
        }
        else if (itemId == BIG_TILE_3) {
            ccbName = ccbName+CC_ITOA(GlobalData::shared()->contryResType)+"_3";
        }
        else if (itemId == BIG_TILE_4) {
            ccbName = ccbName+CC_ITOA(GlobalData::shared()->contryResType)+"_4";
        }
        else if (itemId == BIG_TILE_5) {
            ccbName = ccbName+CC_ITOA(GlobalData::shared()->contryResType)+"_5";
        }
        else if(itemId == SPE_BUILD_FLAG) {
            ccbName = "military_flag";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_HD) {
            ccbName = "pichd_2";
            isTile = false;
            
        }
        else if(itemId == SPE_BUILD_GOLD) {
            ccbName = "piclongdong_2";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_MAIL) {
            ccbName = "Mail";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_GOLD_BOX) {
            ccbName = "picjiuguan_2";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_CHR_TREE) {
            ccbName = "ChristmasTree";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_WILL_OPEN) {
            ccbName = "CityBuild_Open";
            isTile = false;
        }else if (itemId == SPE_BUILD_MERCHANTE){
            ccbName = "picMerchante";
            isTile = false;
        }
        else if (itemId == SPE_BUILD_LOTTERY){
            ccbName = "piclotteryBuild_2";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_SHIP) {
            ccbName = "pichuochuan";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_CARGO) {
            ccbName = "picCargo";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_NEWED) {
            ccbName = "picZhangpeng";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_XIONGDI) {
            ccbName = "picxiongdihui";
            isTile = false;
        }
        else if(itemId == SPE_BUILD_DRAGON) {
            ccbName = "picxiongdihui";
            isTile = false;
        }
        else if (itemId == SPE_BUILD_ZIYUANMEN) {
            ccbName = "piziyuanmen";
            isTile = false;
        }
        
        if (isTile) {
            auto tileInfo = FunBuildController::getInstance()->m_bigTileMap[itemId];
            if (tileInfo.level <= FunBuildController::getInstance()->getMainCityLv()) {
                //ccbName = ccbName + "_2"; //d by ljf, 改用料子系统实现此效果，而不是用两种ccb
                m_isOpen = true;
            }
        }
        
        CCBLoadFile(ccbName.c_str(),m_mainNode,this);
        
        auto pt = m_touchNode->getPosition();
        mainWidth = (m_touchNode->getContentSize().width*m_touchNode->getScaleX()/2+pt.x)*2;
        mainHeight = (m_touchNode->getContentSize().height*m_touchNode->getScaleY()/2+pt.y)*2;
        
        m_effectSpr = CCSprite::create();
    }
    
    this->setContentSize(CCSizeMake(mainWidth, mainHeight));
    this->setAnchorPoint(ccp(0,0));
    
    m_signNode = CCNode::create();
    m_upEffectNode = CCNode::create();
    m_textNode = CCNode::create();
    m_textTouchNode = CCNode::create();
    m_spTextNode = CCNode::create();
    
    if(itemId == SPE_BUILD_HD) {
        if (1) {
            int zOrder = m_upEffectNode->getZOrder();
            auto m_HTPHead = CCLoadSprite::createScale9Sprite("feedback_head.png");
            m_HTPHead->setInsetBottom(10);
            m_HTPHead->setInsetTop(5);
            m_HTPHead->setInsetLeft(5);
            m_HTPHead->setInsetRight(10);
            m_HTPHead->setAnchorPoint(ccp(0.5, 0.5));
            m_HTPHead->setContentSize(CCSize(250,170));
            m_HTPHead->setPosition(m_upEffectNode->getPosition()+ccp(43, 100));
            m_textNode->addChild(m_HTPHead);
            m_HTPHead->setZOrder(zOrder*1000+1);
            
            auto m_HTPHead1 = CCLoadSprite::createSprite("feedback_head1.png");
            m_HTPHead1->setPosition(m_HTPHead->getPosition()+ccp(44.0, -85.0));
            m_HTPHead1->setZOrder(zOrder*1000+2);
             m_textNode->addChild(m_HTPHead1);
            
            auto biaoTiBg = CCLoadSprite::createSprite("biaotikuangHD.png");
            biaoTiBg->setPosition(ccp(m_spTextNode->getPosition().x - 30,m_spTextNode->getPosition().y+65));
            m_spTextNode->addChild(biaoTiBg);
            
            m_timeEventlabel = CCLabelIFTTF::create();
            m_timeEventlabel->setFntFile(getNBFont(NB_FONT_Bold_Border));
            m_timeEventlabel->setColor({255,198,0});
            m_timeEventlabel->setAnchorPoint(ccp(0.5,0.5));
            m_timeEventlabel->setFontSize(20);
            m_timeEventlabel->setString(_lang("133000").c_str());
            m_spTextNode->addChild(m_timeEventlabel);
            //m_timeEventlabel->setPosition(ccp(m_upEffectNode->getPosition().x+15,m_upEffectNode->getPosition().y+115));
            m_timeEventlabel->setPosition(ccp(m_upEffectNode->getPosition().x - 30,m_upEffectNode->getPosition().y+67));
            
//            m_timelabel = CCLabelIF::create("",18);
//            m_timelabel->setFntFile(getNBFont(NB_FONT_Bold_Border));
//            m_timelabel->setColor({255,198,0});
//            m_timelabel->setAnchorPoint(ccp(0.5,0.5));
//            m_upEffectNode->addChild(m_timelabel);
            //m_timelabel->setPosition(ccp(m_upEffectNode->getPosition().x+10,m_upEffectNode->getPosition().y+135));
//            m_timelabel->setPosition(ccp(m_upEffectNode->getPosition().x-30,m_upEffectNode->getPosition().y+108));
            
            m_HTPlabel = CCLabelIF::create();
            m_HTPlabel->setString("");//todo:放到语言文件中
            m_HTPlabel->setDimensions(CCSize(210,180));
            m_HTPlabel->setColor({108,58,25});
            m_HTPlabel->setAnchorPoint(ccp(0,0.5));
            m_textNode->addChild(m_HTPlabel);
            m_HTPlabel->setZOrder(zOrder*1000+5);
            m_HTPlabel->setPosition(ccp(m_upEffectNode->getPosition().x-59,m_upEffectNode->getPosition().y+100));//115
            std::string defLan = LocalController::shared()->getLanguageFileName();
            if(defLan=="ar"){
                m_HTPlabel->setPosition(ccp(m_upEffectNode->getPosition().x-62,m_upEffectNode->getPosition().y+80));//115
            }
            m_timeStr = "";
            m_countTime = 0;
//            double endTime = ActivityController::getInstance()->eventInfo.endTime - GlobalData::shared()->getWorldTime();
//            m_textNode->setVisible(endTime>0);
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpeBuild::onShowTextAni), this);
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SpeBuild::onShowTextAni), this, 1,kCCRepeatForever, 0.0f, false);
            m_textNode->setVisible(false);
            m_countTime = 121;
            this->onShowTextAni(0);
        }
    } else if (itemId == SPE_BUILD_CARGO) {
//        CCSprite *sprite = CCLoadSprite::createSprite("UI_biaotikuang.png");
        auto sprite9 = CCLoadSprite::createScale9Sprite("UI_biaotikuang.png");
        sprite9->setPreferredSize(CCSize(100, 30));
        m_textNode->addChild(sprite9, 1, 1);
        sprite9->setPositionY(-80);
        m_timeEventlabel = CCLabelIFTTF::create();
        m_timeEventlabel->setFntFile(getNBFont(NB_FONT_Bold_Border));
        m_timeEventlabel->setColor({255,198,0});
        m_timeEventlabel->setAnchorPoint(ccp(0.5,0.5));
        m_timeEventlabel->setFontSize(20);
        m_textNode->addChild(m_timeEventlabel, 1, 2);
        m_timeEventlabel->setPosition(ccp(0, -80));
        m_textNode->setVisible(false);
        
//        CCControlButton* btn = CCControlButton::create(CCLoadSprite::createScale9Sprite("but_03.png"));
//        btn->setPreferredSize(CCSize(90, 95));
//        btn->setAnchorPoint(ccp(0.5, 0));
//        btn->setPosition(0, 0);
//        btn->addTargetWithActionForControlEvents(this, cccontrol_selector(SpeBuild::onClickRecBtn), CCControlEventTouchUpInside);
//        m_upEffectNode->addChild(btn);
//        m_upEffectNode->setPosition(ccp(0, 80));
        
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpeBuild::checkLeftTime), this);
        checkLeftTime(0);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SpeBuild::checkLeftTime), this, 1,kCCRepeatForever, 0.0f, false);
    }else if(itemId == SPE_BUILD_GOLD) {
        m_timelabel = CCLabelIF::create(_lang("107000").c_str(),24);
        m_timelabel->setFntFile(getNBFont(NB_FONT_Bold_Border));
        m_timelabel->setColor({255,198,0});
        m_timelabel->setAnchorPoint(ccp(0.5,0.5));
        m_timelabel->setPosition(ccp(-130,-100));
        m_textNode->addChild(m_timelabel);
        
        m_goldMinLock = CCLoadSprite::createSprite("build_lock.png");
        m_goldMinLock->setPosition(ccp(-130, -20));
        m_goldMinLock->setScale(2);
        CCSprite* bg = CCLoadSprite::createSprite("res_btn.png");
        bg->setPosition(ccp(0, 40));
        CCSprite* mineGold = CCLoadSprite::createSprite("ui_gold.png");
        mineGold->setPosition(ccp(0, 47));
        mineGold->setScale(1.0);
        m_goldAniNode = CCNode::create();
        m_goldAniNode->setPosition(ccp(-130, -80));
        m_goldAniNode->setContentSize(CCSize(0,0));
        m_goldAniNode->addChild(bg);
        m_goldAniNode->addChild(mineGold);
        
        if(GlobalData::shared()->playerInfo.gmFlag==1){
            CCRotateBy* rt1 = CCRotateBy::create(0.1, 15);
            CCRotateBy* rt2 = CCRotateBy::create(0.1, -30);
            CCRotateBy* rt3 = CCRotateBy::create(0.1, 22);
            CCRotateBy* rt4 = CCRotateBy::create(0.1, -14);
            CCRotateBy* rt5 = CCRotateBy::create(0.1, 7);
            CCDelayTime* dt = CCDelayTime::create(5);
            CCSequence* seq = CCSequence::create(rt1, rt2, rt3, rt4, rt5, dt, NULL);
            m_goldAniNode->runAction(CCRepeatForever::create(seq));
            
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpeBuild::checkMine), this);
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SpeBuild::checkMine), this, 1,kCCRepeatForever, 0.0f, false);
        }
        
    }else if(itemId == SPE_BUILD_GOLD_BOX) {
        for (int i=1; i<3; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("BaoZangGlow_%d",i)->getCString());
            particle->setPosition(CCPoint(40, 15));
            m_upEffectNode->addChild(particle);
        }
        
        dailyQuestCallBack(nullptr);
        
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpeBuild::onShowTextAni2), this);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SpeBuild::onShowTextAni2), this, 1,kCCRepeatForever, 0.0f, false);
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SpeBuild::dailyQuestCallBack), DAILYQEUST_REWARD, nullptr);
        
    }else if(itemId == SPE_BUILD_MERCHANTE){
        int mainCityL = FunBuildController::getInstance()->getMainCityLv();
        if (mainCityL >= 6) {
            int state = ToolController::getInstance()->getMerchante_state();
            CCNode* tipNode = m_textNode->getChildByTag(20000);
            if(state == 2){
                if(tipNode){
                    m_textNode->removeChildByTag(20000);
                }
                if(this->isVisible()){
                    this->setVisible(false);
                }
            }else if(state==0){
                ToolController::getInstance()->setMerchante_state(1);
                this->getAnimationManager()->runAnimationsForSequenceNamed("1");
                for (int i=1; i<3; i++) {
                    auto particle = ParticleController::createParticle(CCString::createWithFormat("BaoZangGlow_%d",i)->getCString());
                    particle->setPosition(CCPoint(0, 10));
                    m_upEffectNode->addChild(particle);
                }
                
                if(FunBuildController::getInstance()->getMainCityLv()>=5 && !tipNode){
                    AllianceIntroTip *introTip = AllianceIntroTip::createAutoAdjust(AllianceIntroTip_Top, "", 300);
                    introTip->setTag(20000);
                    introTip->setText(_lang("104935"));
                    m_textNode->addChild(introTip);
                    introTip->setAnimationAndPlay(3, 5);
                    introTip->setPositionY(50);
                }
            }
        }
    } else if(itemId == SPE_BUILD_SHIP) {
        addShipPop(CCString::create("init"));
        
    } else if (itemId == SPE_BUILD_ZIYUANMEN)
    {
        auto node = Node::create();
        auto spr = CCLoadSprite::createSprite("feedback_head1.png");
        spr->setAnchorPoint(Vec2(1, 0));
        spr->setPosition(0, 0);
        spr->setFlippedX(true);
        auto bg9Spr = CCLoadSprite::createScale9Sprite("feedback_head.png");
        bg9Spr->setAnchorPoint(Vec2(0.5, 0));
        bg9Spr->setPosition(0, 14.4);
        auto label = CCLabelIF::create((_lang("500027") ).c_str());
        label->setColor({91, 43, 2});
        label->setAnchorPoint(Vec2(0.5, 0));
        label->setFontSize(25);
        label->setPosition(-2.6, 41.4);
        label->setDimensions(Size(180, 0));
        label->setHorizontalAlignment(TextHAlignment::CENTER);
        label->setVerticalAlignment(TextVAlignment::BOTTOM);
        float w = label->getContentSize().width * label->getOriginScaleX();
        float h = label->getContentSize().height * label->getOriginScaleY();
        h = h + 50;
        h = h < 50? 50 : h;
        bg9Spr->setScale(1);
        bg9Spr->setPreferredSize(Size(210, h));
        bg9Spr->setContentSize(Size(210, h));
        node->addChild(bg9Spr);
        node->addChild(spr);
        node->addChild(label);
        node->setPositionY(40);
        m_textNode->addChild(node);
        node->setTag(888);
        node->setVisible(true);
    }

    return true;
}

void SpeBuild::checkLeftTime(float _time) {
    if (m_buildingKey == SPE_BUILD_ZIYUANMEN) {
        if (m_textNode) {
            if (m_textNode->getChildByTag(888)) {
                m_textNode->getChildByTag(888)->setVisible(!m_textNode->getChildByTag(888)->isVisible());
            }
        }
    }
    
    if (m_buildingKey == SPE_BUILD_CARGO) {
        time_t targetTime = PortActController::getInstance()->m_nextRewardTime;
        time_t nowTime = GlobalData::shared()->getTimeStamp();
        int leftTime = 0;
        leftTime = targetTime - nowTime;
        if (targetTime <= 0) {
            m_textNode->setVisible(false);
            removeSpeBuildState(NULL);
            return;
        }
        if (leftTime <= 0) {
            m_textNode->setVisible(false);
            if (leftTime == 0) {
                addSpeBuildState();
            } else {
                if (!m_upEffectNode->isVisible()) {
                    addSpeBuildState();
                }
            }
        } else {
            m_textNode->setVisible(true);
            m_upEffectNode->setVisible(false);
            m_timeEventlabel->setString(CC_SECTOA(leftTime));
        }
        
        CCScale9Sprite* sprite9 = dynamic_cast<CCScale9Sprite*>(m_textNode->getChildByTag(1));
        sprite9->setPreferredSize(CCSize(m_timeEventlabel->getContentSize().width * m_timeEventlabel->getOriginScaleX() + 50, m_timeEventlabel->getContentSize().height * m_timeEventlabel->getOriginScaleY() + 10));
    }
}

void SpeBuild::checkMine(float _time){
    string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if(_fbUid!=""){
        double mineTime = SocialController::getInstance()->mineTime - GlobalData::shared()->getWorldTime();
        if(SocialController::getInstance()->miner==SocialController::getInstance()->xml_miner && (mineTime<=0 ||SocialController::getInstance()->mineTime==0)){
            if(m_goldAniNode->getParent()==NULL){
                m_upEffectNode->addChild(m_goldAniNode);
            }
        }else{
            if(m_goldAniNode->getParent()!=NULL){
                m_goldAniNode->removeFromParent();
            }
        }
        if(m_goldMinLock->getParent()!=NULL){
            m_goldMinLock->removeFromParent();
        }
    }else{
        if(m_goldAniNode->getParent()!=NULL){
            m_goldAniNode->removeFromParent();
        }
        if(m_goldMinLock->getParent()==NULL){
            m_upEffectNode->addChild(m_goldMinLock);
        }
    }
}

void SpeBuild::onShowTextAni(float _time){
    if(!m_textNode) return ;
//    double tempTime = ActivityController::getInstance()->eventInfo.readyTime - GlobalData::shared()->getWorldTime();
//    double readyTime = ActivityController::getInstance()->eventInfo.beginTime - GlobalData::shared()->getWorldTime();
//    double endTime = ActivityController::getInstance()->eventInfo.endTime - GlobalData::shared()->getWorldTime();
//    string id = ActivityController::getInstance()->eventInfo.id;
//    int eventId = ActivityController::getInstance()->eventInfo.curPhase;
//    string timeStr = "";
//    if(tempTime>0){
//        if(eventId==0){
//            m_timeStr = _lang("105800");
//            m_timeStr += "\n";
//            m_timeStr += _lang_1("105804", CC_SECTOA(tempTime));
//            timeStr = _lang_1("105804", CC_SECTOA(tempTime));
//        }else{
//            m_timeStr = _lang_1("105802",CC_ITOA(eventId));
//            m_timeStr += "\n";
//            m_timeStr += _lang_1("105804", CC_SECTOA(tempTime));
//            timeStr = _lang_1("105804", CC_SECTOA(tempTime));
//        }
//    }else if(readyTime>0){
//        m_timeStr = _lang_1("105803",CC_ITOA(eventId));
//        m_timeStr += "\n";
//        m_timeStr += _lang_1("105804", CC_SECTOA(readyTime));
//        timeStr = _lang_1("105804", CC_SECTOA(readyTime));
//    }else if(endTime>=0){
//        m_timeStr = _lang_1("105803",CC_ITOA(eventId));
//        m_timeStr += "\n";
//        m_timeStr += _lang_1("105805", CC_SECTOA(endTime));
//        timeStr = _lang_1("105805", CC_SECTOA(endTime));
//    }else{
//        m_timeStr = _lang("105801");
//        timeStr = "";
//    }
//    m_timelabel->setString(timeStr);
//    m_HTPlabel->setString(m_timeStr.c_str());
    if(m_countTime==5 && m_textNode->isVisible())
    {
        m_countTime = 0;
        m_textNode->setVisible(false);
    }
    if(!m_textNode->isVisible() && m_countTime>15)
    {
        m_timeStr = ActivityController::getInstance()->getActivityInfo();
        m_HTPlabel->setString(m_timeStr.c_str());
        m_textNode->setVisible(m_timeStr.length()>0);
        m_countTime = 0;
    }
    m_countTime += 1;
}

void SpeBuild::onShowTextAni2(float _time){
    if(!m_textNode) return ;
    string temp = changeShowIcon();
    if (temp!="") {
        if (temp!=m_touchName) {
            dailyQuestCallBack(nullptr);
        }
        m_textNode->setVisible(true);
        m_countTime = 0;
        return;
    }
    if(m_countTime==5 && m_textNode->isVisible())
    {
        m_countTime = 0;
        m_textNode->setVisible(false);
    }
    if(!m_textNode->isVisible() && m_countTime>30)
    {
        dailyQuestCallBack(nullptr);
//        if (!DailyActiveController::shared()->needShow())
//        {
//            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpeBuild::onShowTextAni2), this);
//            return;
//        }
        m_textNode->setVisible(true);
        m_countTime = 0;
    }
    m_countTime += 1;
}

void SpeBuild::dailyQuestCallBack(cocos2d::CCObject *pObj)
{
    string iconName = "";
    string lastName = m_touchName;
    m_touchName = "";
    float scale = 1.0f;
    string temp = changeShowIcon();
    string randomStr = randomShowIcon();
    if (temp!="") {
        m_touchName = temp;
    }else if(randomStr!=""){
        m_touchName = randomStr;
    }else if (ActivityController::getInstance()->m_isTrialOpen == 1) {
        iconName = "training_enter.png";
        m_touchName = "training";
        scale = 0.8f;
    }
    if (m_touchName.length() == 0 || (m_touchName == lastName && lastName.length() != 0))
    {
        return;
    }
    m_textNode->removeAllChildren();
    bool flag = true;
    if (m_touchName=="roulette") {
        if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/zhuanpan_1.json") &&
            CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas"))
        {
            auto animationObj = new IFSkeletonAnimation("Spine/Imperial/zhuanpan_1.json","Imperial/Imperial_30.atlas");
            if (animationObj) {
                animationObj->setVisibleStop(false);
                animationObj->setPosition(ccp(0, 60));
                m_textNode->addChild(animationObj,3);
                spTrackEntry* entry = animationObj->setAnimation(0, "animation", true);
                if(entry){
                    animationObj->setTimeScale(entry->endTime/1.0f);
                }
                flag = false;
            }
        }
    }else if(m_touchName=="dailyActive"){
        if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/dailyReward.json") &&
            CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas"))
        {
            auto animationObj = new IFSkeletonAnimation("Spine/Imperial/dailyReward.json","Imperial/Imperial_30.atlas");
            if (animationObj) {
                animationObj->setVisibleStop(false);
                animationObj->setPosition(ccp(0, 60));
                m_textNode->addChild(animationObj,3);
                spTrackEntry* entry = animationObj->setAnimation(0, "animation", true);
                if(entry){
                    animationObj->setTimeScale(entry->endTime/1.0f);
                }
                flag = false;
            }
        }
    }else if(m_touchName=="baozang"){//暂无，先加上
        if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/baoxiang.json") &&
            CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas"))
        {
            auto animationObj = new IFSkeletonAnimation("Spine/Imperial/baoxiang.json","Imperial/Imperial_30.atlas");
            if (animationObj) {
                animationObj->setVisibleStop(false);
                animationObj->setPosition(ccp(0, 60));
                m_textNode->addChild(animationObj,3);
                spTrackEntry* entry = animationObj->setAnimation(0, "animation", true);
                if(entry){
                    animationObj->setTimeScale(entry->endTime/1.0f);
                }
                flag = false;
            }
        }
    }else{
        CCSprite* sp = CCLoadSprite::createSprite(iconName.c_str());
        sp->setPosition(ccp(0, 110));
        sp->setScale(scale);
        m_textNode->addChild(sp, 2);
    }
    auto m_HTPHead = CCLoadSprite::createScale9Sprite("feedback_head.png");
    m_HTPHead->setInsetBottom(10);
    m_HTPHead->setInsetTop(5);
    m_HTPHead->setInsetLeft(5);
    m_HTPHead->setInsetRight(10);
    m_HTPHead->setAnchorPoint(ccp(0.5, 0.5));
    m_HTPHead->setContentSize(CCSize(100,100));
    m_HTPHead->setPosition(ccp(0, 110));
    m_textNode->addChild(m_HTPHead);
    m_HTPHead->setZOrder(1);
    m_HTPHead->setVisible(flag);
    
    auto m_HTPHead1 = CCLoadSprite::createSprite("feedback_head1.png");
    m_HTPHead1->setPosition(m_HTPHead->getPosition()+ccp(20.0, -58.0));
    m_HTPHead1->setZOrder(2);
    m_textNode->addChild(m_HTPHead1);
    m_HTPHead1->setVisible(flag);
    m_countTime = 30;

    m_textNode->setVisible(false);
    
    m_textTouchNode->setContentSize(m_HTPHead->getContentSize());
    m_textTouchNode->setPosition( m_HTPHead->getPosition() + ccp(-m_HTPHead->getContentSize().width/2, -m_HTPHead->getContentSize().height/2) );
    m_textNode->addChild(m_textTouchNode, 3);
}

string SpeBuild::changeShowIcon(){
    string tName = "";
    int cost = LotteryController::shared()->getLotteryCost1();
    if (LotteryController::shared()->getOpenFlag2()==1 && GlobalData::shared()->analyticID!="common" && cost==0 )
    {
        tName = "roulette";
    }else if (DailyActiveController::shared()->getDailyActiveOpenFlag() == 1 && DailyActiveController::shared()->canGetReward()){//日常奖励
        tName = "dailyActive";
    }else if(!GlobalData::shared()->isXiaoMiPlatForm() && (SocialController::getInstance()->miner !=0 && SocialController::getInstance()->miner == SocialController::getInstance()->xml_miner)){
        tName = "baozang";
    }
    return tName;
}

string SpeBuild::randomShowIcon(){
    string temp1 = "";
    string temp2 = "";
    string tName = "";
    long total = GlobalData::shared()->resourceInfo.lChip;
    if (LotteryController::shared()->getOpenFlag2()==1 && GlobalData::shared()->analyticID!="common" && total>=1000) {
        temp1 = "roulette";
    }
//    if(DailyActiveController::shared()->getDailyActiveOpenFlag() == 1 && DailyActiveController::shared()->getActivePoints()<500){
//        temp2 = "dailyActive";
//    }
    int rand = GlobalData::shared()->getRand(1,100000);
    if (temp1!="" && temp2!="") {
        if (rand%2==0) {
            tName = temp1;
        }else{
            tName = temp2;
        }
    }else{
        if (temp1!="") {
            tName = temp1;
        }else if(temp2!=""){
            tName = temp2;
        }
    }
    return tName;
}

void SpeBuild::onClickRecBtn(cocos2d::CCObject *pTarget, CCControlEvent touchEvent){
    if (m_buildingKey == SPE_BUILD_CARGO) {
        time_t targetTime = PortActController::getInstance()->m_nextRewardTime;
        time_t nowTime = GlobalData::shared()->getTimeStamp();
        if (targetTime <= 0) {
            m_textNode->setVisible(false);
            removeSpeBuildState(NULL);
            return;
        }
        if (nowTime - targetTime >= 0)
        {
            PopupViewController::getInstance()->addPopupView(TimeRwdShowView::create());
        } else {
            CCCommonUtils::flyHint("", "", _lang("111094"));
        }
    }
    else if (m_buildingKey == SPE_BUILD_NEWED) {
        
        if (QuestController::getInstance()->currentAchList.find("2102501") != QuestController::getInstance()->currentAchList.end()) {
            QuestInfo* qusetTmp = QuestController::getInstance()->currentAchList["2102501"];
            if(qusetTmp->state == ACCEPT) {
                CCCommonUtils::flyHint("", "", _lang("106046"), 3, 200);
                return;
            }
        }
        
        if(PortActController::getInstance()->m_newPalyerRDMap.size() <= 0)
        {
            return;
        }
        
        PopupViewController::getInstance()->addPopupInView(NewPlayerQianDaoView::create());
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("SMC_%d", m_buildingKey));
    }
}

void SpeBuild::setNamePos(int x, int y, CCLayer* sginLayer, CCSpriteBatchNode* popBatchNode, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch)
{
    parentX = x;
    parentY = y;
    
    m_signLayer = sginLayer;
    m_popBatchNode = popBatchNode;
    m_batchNode = batchNode;
    m_blentBatchNode = blentbatch;
    
    m_signNode->setPosition(ccp(x,y));
    m_upEffectNode->setPosition(ccp(x+mainWidth/2,y+mainHeight/2));
    m_textNode->setPosition(ccp(x+mainWidth/2,y+mainHeight/2));
//    m_spTextNode->setPosition(m_upEffectNode->getPosition());
     m_spTextNode->setPosition(m_upEffectNode->getPosition()-Vec2(100,0));
    sginLayer->addChild(m_signNode);
    sginLayer->addChild(m_upEffectNode);
    sginLayer->addChild(m_spTextNode,10000000);
    sginLayer->addChild(m_textNode,100000000);
    
    int tmpOffx = parentX;
    int tmpOffy = parentY;
    if(m_buildingKey == SPE_BUILD_GOLD_BOX || m_buildingKey == SPE_BUILD_SHIP)
    {
        tmpOffx = 0;
        tmpOffy = 0;
        m_batchNode->setPosition(ccp(parentX, parentY));
    }
    
    int tmpOrd = 0;
    if(m_mainNode) {// m_buildingKey!=9995 &&
        auto array = m_mainNode->getChildren();
        if (array.size() >0) {
            CCNode* tmpNode = dynamic_cast<CCNode*>(array.at(0));//ccb 中的layer层
            if (tmpNode) {
                auto arr1 = tmpNode->getChildren();
                if (arr1.size()>0) {
                    CCNode* allSprNode = dynamic_cast<CCNode*>(arr1.at(0));//ccb 中的layer的 node
                    if (allSprNode) {
                        auto& arrSpr = allSprNode->getChildren();
                        if (true) {
                            for (int i=0; i<arrSpr.size(); i++) {
                                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(arrSpr.at(i));
                                if (tmpSpr) {
                                    if(tmpSpr->getContentSize().width==0&&tmpSpr->getContentSize().height==0)
                                        continue;//fusheng ccb死了的图  就不向batchNode里加
                                    tmpSpr->removeFromParent();
                                    tmpSpr->getTexture()->setAntiAliasTexParameters();
                                    tmpSpr->setPosition(ccp(tmpSpr->getPositionX()+tmpOffx, tmpSpr->getPositionY()+tmpOffy));
                                    
                                    if (m_blentBatchNode) {
                                        auto batchBlend = m_blentBatchNode->getBlendFunc();
                                        auto particleBlend = tmpSpr->getBlendFunc();
                                        if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                                            m_blentBatchNode->addChild(tmpSpr);
                                            m_blentSprArray->addObject(tmpSpr);
                                        }
                                        else {
                                            m_batchNode->addChild(tmpSpr);
                                            if(m_buildingKey!=SPE_BUILD_SHIP || (tmpSpr->getTag() != 4 && tmpSpr->getTag() !=5)){
                                                 m_sprArray->addObject(tmpSpr);
                                            }
                                        }
                                    }
                                    else {
                                        m_batchNode->addChild(tmpSpr);
                                        if(m_buildingKey!=SPE_BUILD_SHIP || (tmpSpr->getTag() != 4 && tmpSpr->getTag() !=5)){
                                            m_sprArray->addObject(tmpSpr);
                                        }
                                    }
                                    tmpSpr->setZOrder(zOrder*1000+tmpOrd);
                                    tmpOrd++;
                                    i--;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else {
    }
    
    drowEffectSpr(zOrder, tmpOrd);
    drowPersonSpr(zOrder, tmpOrd);
    addSpeBuildState();
}

void SpeBuild::addSpeBuildState()
{
    if (m_buildingKey == SPE_BUILD_CARGO) {
        PortActController::getInstance()->addPush();
        if (!PortActController::getInstance()->isCanRwd())
            return;
        m_upEffectNode->stopAllActions();
        m_upEffectNode->removeAllChildrenWithCleanup(true);
        CCControlButton* btn = CCControlButton::create(CCLoadSprite::createScale9Sprite("bnt_02.png"));
        
        btn->setPreferredSize(CCSize(98, 111));
        btn->setAnchorPoint(ccp(0.5, 0));
//        btn->setPosition(0, 0);
        btn->setPosition(-7, 80);//
        btn->addTargetWithActionForControlEvents(this, cccontrol_selector(SpeBuild::onClickRecBtn), CCControlEventTouchUpInside);
        btn->setTouchPriority(4);
        m_upEffectNode->addChild(btn);
        auto pic = CCLoadSprite::createSprite("icon_cargoOk.png");
//        pic->setPosition(ccp(0, 65));
        pic->setPosition(ccp(-7, 145));
        pic->setAnchorPoint(ccp(0.5, 0.5));
        m_upEffectNode->addChild(pic);
        auto pos = m_upEffectNode->getPosition();
        m_upEffectNode->setPosition(ccp(parentX + mainWidth / 2,parentY + mainHeight / 2 + 10));
        m_upEffectNode->setScale(1);
        playShakeState();
    }
    else if (m_buildingKey == SPE_BUILD_NEWED) {
        if (PortActController::getInstance()->m_isGetNewRD!=0) {
            return;
        }
        m_upEffectNode->stopAllActions();
        m_upEffectNode->removeAllChildrenWithCleanup(true);
        CCControlButton* btn = CCControlButton::create(CCLoadSprite::createScale9Sprite("but_03.png"));
        btn->setPreferredSize(CCSize(128, 133));
        btn->setAnchorPoint(ccp(0.5, 0));
        btn->setPosition(0, 0);
        btn->addTargetWithActionForControlEvents(this, cccontrol_selector(SpeBuild::onClickRecBtn), CCControlEventTouchUpInside);
        btn->setTouchPriority(4);
        m_upEffectNode->addChild(btn);
        auto pic = CCLoadSprite::createSprite("newrd_icon.png");
        pic->setPosition(ccp(0, 65));
        m_upEffectNode->addChild(pic);
        auto pos = m_upEffectNode->getPosition();
        m_upEffectNode->setPosition(ccp(parentX + mainWidth / 2,parentY + mainHeight / 2 + 10));
        m_upEffectNode->setScale(1);
        playShakeState();
    }
}

void SpeBuild::playShakeState()
{
    CCRotateBy* rt1 = CCRotateBy::create(0.1, 15);
    CCRotateBy* rt2 = CCRotateBy::create(0.1, -30);
    CCRotateBy* rt3 = CCRotateBy::create(0.1, 22);
    CCRotateBy* rt4 = CCRotateBy::create(0.1, -14);
    CCRotateBy* rt5 = CCRotateBy::create(0.1, 7);
    CCDelayTime* dt = CCDelayTime::create(2);
    CCSequence* seq = CCSequence::create(rt1, rt2, rt3, rt4, rt5, dt, NULL);
    m_upEffectNode->runAction(CCRepeatForever::create(seq));
    m_upEffectNode->setVisible(true);
}

void SpeBuild::removeSpeBuildState(CCObject* obj)
{
    m_upEffectNode->stopAllActions();
    m_upEffectNode->removeAllChildrenWithCleanup(true);
    m_upEffectNode->setVisible(false);
}

void SpeBuild::onBuildDelete()
{
    if (m_signLayer) {
        m_signLayer->removeChild(m_signNode);
        m_signLayer->removeChild(m_upEffectNode);
        if(m_textNode){
            m_signLayer->removeChild(m_textNode);
        }
        if(m_spTextNode){
            m_signLayer->removeChild(m_spTextNode);
        }
        auto batchCount = m_parVec.size();
        while (batchCount--) {
            m_signLayer->removeChild(m_parVec[batchCount]);
        }
        m_parVec.clear();
    }
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        if (spr)
        {
            spr->removeFromParentAndCleanup(true);
        }
//        m_batchNode->removeChild(spr, true);
    }
    m_sprArray->removeAllObjects();
    if (m_blentBatchNode) {
        for (int i=0; i<m_blentSprArray->count(); i++) {
            CCSprite* spr = dynamic_cast<CCSprite*>(m_blentSprArray->objectAtIndex(i));
            if (spr)
            {
                spr->removeFromParentAndCleanup(true);
            }
//            m_blentBatchNode->removeChild(spr, true);
        }
        m_blentSprArray->removeAllObjects();
    }
    
    for (int i=0; i<m_personArray->count(); i++) {
        Person* person = dynamic_cast<Person*>(m_personArray->objectAtIndex(i));
        if (person)
        {
            person->onPersonDelete();
        }
//        person->onPersonDelete();
    }
    m_personArray->removeAllObjects();
}

void SpeBuild::onEnter() {
    CCNode::onEnter();
    if (m_buildingKey == SPE_BUILD_CARGO) {
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SpeBuild::removeSpeBuildState), MSG_NEW_TIME_RWD_END, NULL);
    }
    if (m_buildingKey == SPE_BUILD_NEWED) {
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SpeBuild::hideNewRDTip), MSG_NEW_PLAYER_RD, NULL);
    }
}

void SpeBuild::onExit() {
    if (m_buildingKey == SPE_BUILD_CARGO) {
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpeBuild::checkLeftTime), this);
    }
    if (m_buildingKey == SPE_BUILD_NEWED) {
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEW_PLAYER_RD);
    }
    if (m_buildingKey == SPE_BUILD_GOLD_BOX) {
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpeBuild::onShowTextAni2), this);
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DAILYQEUST_REWARD);
    }else if(m_buildingKey == SPE_BUILD_SHIP){
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpeBuild::onShowTextAniShip), this);
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_SHOW_COMMAND);
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PORT_LOGINRD_END);
    }
    if (m_buildingKey == SPE_BUILD_CARGO) {
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEW_TIME_RWD_END);
    }
    CCNode::onExit();
}

bool SpeBuild::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    bool ret = false;
    m_clickNameKey = "";
    
    if(m_buildingKey == SPE_BUILD_SHIP && canClickShip() && m_shipNode){
        if (m_textNode->isVisible() && m_textTouchNode) {
            if (m_textTouchNode->getParent() == m_textNode) {
                ret = isTouchInside(m_textTouchNode, pTouch);
                if (ret) {
                    m_clickNameKey = m_touchName;
                }
            }
        }
        if (!ret) {
            ret = isTouchInside(m_shipNode, pTouch);
        }
    }
    if (m_buildingKey == SPE_BUILD_GOLD_BOX) {
        if (m_textNode->isVisible() && m_textTouchNode) {
            if (m_textTouchNode->getParent() == m_textNode) {
                ret = isTouchInside(m_textTouchNode, pTouch);
                if (ret) {
                    m_clickNameKey = m_touchName;
                }
            }
        }
    }
    if (!ret) {
        ret = isTouchInside(m_touchNode, pTouch);
    }
    return ret;
}

void SpeBuild::setGary()
{
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        if (spr) {
            spr->setColor(ccGRAY);
        }
    }
    if (m_buildingKey == SPE_BUILD_FLAG) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Click");
    }
}

void SpeBuild::setWhite()
{
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        if (spr) {
            spr->setColor(ccWHITE);
        }
    }
}

void SpeBuild::playShadow()
{
    CCTintTo* tinto1 = CCTintTo::create(0.5, 166, 166, 166);
    CCTintTo* tinto2 = CCTintTo::create(0.5, 255, 255, 255);
    
    CCSequence* seq = CCSequence::create(tinto1,tinto2,NULL);
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        if (spr && spr->isVisible()) {
            spr->runAction( CCRepeatForever::create(seq) );
        }
    }
}

void SpeBuild::stopShadow()
{
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        if (spr) {
            spr->stopAllActions();
            spr->setColor(ccWHITE);
        }
    }
}

void SpeBuild::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isCanClick) {
        return;
    }
    
    //    onClickThis();
    if(m_buildingKey == SPE_BUILD_SHIP && canClickShip() && m_shipNode && isTouchInside(m_shipNode, pTouch) ){
        isCanClick=false;
        setGary();
        scheduleOnce(schedule_selector(SpeBuild::clickShip), 0.1f);
        return;
    }
    
    isCanClick=false;
    setGary();
    scheduleOnce(schedule_selector(SpeBuild::onClickThis), 0.1f);
}
bool SpeBuild::canClickShip(){
    auto iter = GlobalData::shared()->monthCardInfoList.begin();
    while (iter!=GlobalData::shared()->monthCardInfoList.end()) {
        if((*iter).second->canReward() && !(*iter).second->getAccepted()){
            if(m_shipCurAnimation.compare("collect") == 0){
                return true;
            }
        }else if((*iter).second->getBuy() == false){
            if(m_shipCurAnimation.compare("buy") == 0){
                return true;
            }
        }
        ++iter;
    }
    return false;
}
void SpeBuild::clickShip(float _time){
    auto iter = GlobalData::shared()->monthCardInfoList.begin();
    while (iter!=GlobalData::shared()->monthCardInfoList.end()) {
        if((*iter).second->canReward() && !(*iter).second->getAccepted()){
            setWhite();
            CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SpeBuild::clickShipCallBack), GOLDEXCHANGE_SHOW_COMMAND, NULL);
            MonthCardRewardCommand* cmd = new MonthCardRewardCommand((*iter).first);
            cmd->sendAndRelease();
            scheduleOnce(schedule_selector(SpeBuild::onCanClick), 0.2f);
            return;
        }else if((*iter).second->getBuy() == false){
            onClickThis(0);
            return;
        }
        ++iter;
    }
}
void SpeBuild::clickShipCallBack(cocos2d::CCObject *obj){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_SHOW_COMMAND);
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto pt = m_shipNode->getPosition();
    float mW = (m_shipNode->getContentSize().width*m_shipNode->getScaleX()/2+pt.x);
    float mH = (m_shipNode->getContentSize().height*m_shipNode->getScaleY()/2+pt.y);
    int startPosx = this->getParent()->getPositionX() + mW ;
    int startPosy = this->getParent()->getPositionY() + mH;
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if(layer){
        auto pt = layer->onGetSceenPt(startPosx, startPosy);
        pt.y = pt.y-winSize.height;
        string picStr = "gold";
        auto endPt = ccp(570,-60);
        int resType = Wood;
        srand((unsigned)time(0));
        int tmp = 5;
        int idx = 0;
        int cnt = 5;
        CCSpriteBatchNode* batchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("gold1.png")->getTexture(), 20);
        CCNode *node1 = CCNode::create();
        node1->addChild(batchNode);
        node1->setTag(19999);
        while (tmp>0) {
//            if(tmp%2==0){
                auto flyCell1 = FlyCell::createCircle(pt + ccp(0, -30), endPt, picStr, cnt, batchNode, MIN(idx*0.2,2),150,-50);
                node1->addChild(flyCell1);
                auto flyCell3 = FlyCell::createCircle(pt + ccp(-5, -15), endPt, picStr,cnt, batchNode, MIN(idx*0.2,2),50,-20);
                node1->addChild(flyCell3);
//            }else{
                auto flyCell = FlyCell::createCircle(pt + ccp(-15, -5), endPt, picStr, cnt, batchNode, MIN(idx*0.2,2),-150,50);
                node1->addChild(flyCell);
                
                auto flyCell2 = FlyCell::createCircle(pt + ccp(-10, 5), endPt, picStr, cnt, batchNode, MIN(idx*0.2,2),-50,20);
                node1->addChild(flyCell2);
//            }
            
            tmp--;
            idx++;
        }
        UIComponent::getInstance()->m_collectNode->addChild(node1);
    }
    changeShipAnimation("buy","",false);
    scheduleOnce(schedule_selector(SpeBuild::shipLeave), 2.5f);
}
void SpeBuild::shipLeave(float _time){
    if(m_buildingKey == SPE_BUILD_SHIP){
        changeShipAnimation("leave");
        if(UIComponent::getInstance()->m_collectNode->getChildByTag(19999)){
            UIComponent::getInstance()->m_collectNode->removeChildByTag(19999);
        }
    }
}
void SpeBuild::onClickThis(float _time)
{
    setWhite();
    
    if (m_buildingKey == BIG_TILE_1 || m_buildingKey == BIG_TILE_2
        || m_buildingKey == BIG_TILE_3 || m_buildingKey == BIG_TILE_4 || m_buildingKey == BIG_TILE_5) {
        
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        auto tileInfo = FunBuildController::getInstance()->m_bigTileMap[m_buildingKey];
        if(tileInfo.level <= FunBuildController::getInstance()->getMainCityLv()) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            int tx = parentX+m_signCCBNode->getPositionX();
            int ty = parentY+m_signCCBNode->getPositionY();
            float _scale = 0.8;
            if (CCCommonUtils::isIosAndroidPad())
            {
                _scale = 1.2;
            }
            layer->onMoveToPos(tx, ty, TYPE_POS_UP, 0.25, _scale);
            
            PopupViewController::getInstance()->addPopupInView(TileOpenView::create(m_buildingKey));
        }
        else {
            CCCommonUtils::flyHint("lock-trees_1.png", "", _lang_1("102118", CC_ITOA(tileInfo.level)));
//            CCCommonUtils::flyText(_lang_1("102118", CC_ITOA(tileInfo.level)));
        }
    }
    else if (m_buildingKey == SPE_BUILD_FLAG) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_details);
        PopupViewController::getInstance()->addPopupInView(NewTroopsView::create());
//        PopupViewController::getInstance()->addPopupInView(TroopsView::create(0));
//        CCCommonUtils::flyHint("", "", _lang("102467"));
//        CCLOG("click flag");
    }
    else if (m_buildingKey == SPE_BUILD_SHIP) {
        
//        this->getAnimationManager()->runAnimationsForSequenceNamed("Leave");
        
        SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
        if (m_clickNameKey == "monthcard") {
            PopupViewController::getInstance()->addPopupView(MonthCardView::create());
        }else if (m_clickNameKey == "qiandao") {
            PopupViewController::getInstance()->addPopupInView(PortActView::create());
        }else {
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if (layer) {
#pragma mark buttonPos
//                layer->onShowSpeBtnsView(parentX+mainWidth/2, parentY+120, m_buildingKey);
                layer->onShowSpeBtnsView(parentX+mainWidth/2, parentY+30, m_buildingKey);
            }
        }
    }
    else if (m_buildingKey == SPE_BUILD_CARGO) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
//        PortActController::getInstance()->startTimeRwd();
        onClickRecBtn(NULL, Control::EventType::TOUCH_DOWN);
    }
    else if (m_buildingKey == SPE_BUILD_HD) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
        ActivityController::getInstance()->openActivityView();
    }
    else if (m_buildingKey == SPE_BUILD_GOLD_BOX) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
        if (m_clickNameKey == "dailyActive") {
            PopupViewController::getInstance()->addPopupInView(DailyActiveView::create());
        }else if (m_clickNameKey == "training") {
            PopupViewController::getInstance()->addPopupInView(Training1View::create());
        }else if (m_clickNameKey == "roulette") {
            if (GlobalData::shared()->fromCountry == "KR" || GlobalData::shared()->fromCountry == "kr")
            {//韩国玩家先进到这里受审
                if (LotteryController::shared()->getOpenFlag2_ko()==0)
                {
                    CCCommonUtils::flyHint("", "", _lang("111126"));
                    return;
                }
            }
            if (LotteryController::shared()->lotteryInfo.type == 2)
            {
                PopupViewController::getInstance()->addPopupInView(LotteryAct2View::create());
            }
            else
            {
                PopupViewController::getInstance()->addPopupInView(LotteryActView::create());
            }
        }else if (m_clickNameKey == "baozang") {
            PopupViewController::getInstance()->addPopupInView(BaoZangView::create());
        }else {
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if (layer) {
                layer->onShowSpeBtnsView(parentX+mainWidth/2, parentY+50, m_buildingKey);
            }
        }
    }
    else if (m_buildingKey == SPE_BUILD_WILL_OPEN) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
        CCCommonUtils::flyHint("", "", _lang("119000"));
    }else if (m_buildingKey == SPE_BUILD_MERCHANTE){
        int cityLv = FunBuildController::getInstance()->getMainCityLv();
        if (cityLv<6) {
            string tmpInfo = _lang("106054");
            CCCommonUtils::flyHint("", "", tmpInfo);
            scheduleOnce(schedule_selector(SpeBuild::onCanClick), 0.2f);
            return;
        }
        
        if(isVisible()){
            SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
            int state = ToolController::getInstance()->getMerchante_state();
            if(state==2){
                CCCommonUtils::flyHint("", "", _lang("104940"));
            }else{
//                auto sT = ToolController::getInstance()->getMerchante_time_s1();
//                auto eT = ToolController::getInstance()->getMerchante_time_e1();
//                auto nT = WorldController::getInstance()->getTime();
//                
//                if(nT < eT && (nT > sT || nT == sT)){
                    PopupViewController::getInstance()->addPopupInView(MerchantView::create());
//                }else if(eT < nT || eT == nT){
//                }else{
//                    CCCommonUtils::flyHint("", "", _lang("104940"));
//                }
                
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                       , CCString::createWithFormat("SMC_%d", m_buildingKey));
            }
        }
    }else if (m_buildingKey == SPE_BUILD_GOLD) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
        CCLOG("click gold mine");
    }
    else if (m_buildingKey == SPE_BUILD_XIONGDI) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
        PopupViewController::getInstance()->addPopupInView(InviteForGiftView::create());
//        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//        if (layer) {
//            layer->onShowSpeBtnsView(parentX+mainWidth/2, parentY+50, m_buildingKey);
//        }
    }
    else if (m_buildingKey == SPE_BUILD_NEWED) {
        SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
        onClickRecBtn(NULL, CCControlEvent::TOUCH_DOWN);
    }
    else if(m_buildingKey == SPE_BUILD_DRAGON){
        CCLog("click dragon building");
        PopupViewController::getInstance()->addPopupInView(DragonBattleJoinView::create());
    }
    else if (m_buildingKey == SPE_BUILD_ZIYUANMEN) {
        PopupViewController::getInstance()->addPopupInView(MainCityView::create(FUN_BUILD_MAIN_CITY_ID));
    }
    
    scheduleOnce(schedule_selector(SpeBuild::onCanClick), 0.2f);
}

void SpeBuild::digMineSuccess(CCObject* param){
    this->checkMine(0);
}

void SpeBuild::onCanClick(float _time)
{
    isCanClick = true;
}

void SpeBuild::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void SpeBuild::canShowState()
{
}

void SpeBuild::drowEffectSpr(int zOrder, int tmpOrd)
{
    if (m_isOpen) {
        auto tmp_ptArray = CCPointArray::create(20);
        if (m_buildingKey == BIG_TILE_1) {
            tmp_ptArray->addControlPoint(ccp(parentX+250, parentY+270));
            tmp_ptArray->addControlPoint(ccp(parentX+90, parentY+130));
        }
        else if (m_buildingKey == BIG_TILE_2) {
            tmp_ptArray->addControlPoint(ccp(parentX+370, parentY+210));
            tmp_ptArray->addControlPoint(ccp(parentX+70, parentY+150));
        }
        else if (m_buildingKey == BIG_TILE_3) {
            tmp_ptArray->addControlPoint(ccp(parentX+220, parentY+210));
        }
        else if (m_buildingKey == BIG_TILE_4) {
            tmp_ptArray->addControlPoint(ccp(parentX+170, parentY+180));
        }
        else if (m_buildingKey == BIG_TILE_5) {
            tmp_ptArray->addControlPoint(ccp(parentX+220, parentY+210));
        }
        for (int j=0; j<tmp_ptArray->count(); j++) {
            auto particle1 = ParticleController::createParticle("Collection_Loop_3");
            particle1->setPosition(tmp_ptArray->getControlPointAtIndex(j));
            addParticleToBatch(particle1);
            
            auto particle2 = ParticleController::createParticle("Collection_Loop_4");
            particle2->setPosition(tmp_ptArray->getControlPointAtIndex(j));
            addParticleToBatch(particle2);
        }
    }
    
    if (m_buildingKey == SPE_BUILD_HD) {
        auto tmp_ptArray = CCPointArray::create(20);
        tmp_ptArray->addControlPoint(ccp(parentX+120, parentY+170));
        for (int j=0; j<tmp_ptArray->count(); j++) {
            auto particle1 = ParticleController::createParticle("Collection_Loop_3");
            particle1->setPosition(tmp_ptArray->getControlPointAtIndex(j));
            addParticleToBatch(particle1);
            
            auto particle2 = ParticleController::createParticle("Collection_Loop_4");
            particle2->setPosition(tmp_ptArray->getControlPointAtIndex(j));
            addParticleToBatch(particle2);
        }
    }
    
    if (m_buildingKey == SPE_BUILD_GOLD) {
        auto tmp_ptArray = CCPointArray::create(20);
        tmp_ptArray->addControlPoint(ccp(parentX+180, parentY+170));
        for (int j=0; j<tmp_ptArray->count(); j++) {
            auto particle1 = ParticleController::createParticle("Collection_Loop_3");
            particle1->setPosition(tmp_ptArray->getControlPointAtIndex(j));
            addParticleToBatch(particle1);
            
            auto particle2 = ParticleController::createParticle("Collection_Loop_4");
            particle2->setPosition(tmp_ptArray->getControlPointAtIndex(j));
            addParticleToBatch(particle2);
        }
    }
    
    if (m_buildingKey == SPE_BUILD_FLAG) {
        auto particle1 = ParticleController::createParticle("ArmyFlag_1");
        particle1->setPosition(ccp(parentX+0, parentY+20));
        addParticleToBatch(particle1);
        
        auto particle2 = ParticleController::createParticle("ArmyFlag_2");
        particle2->setPosition(ccp(parentX+0, parentY+250));
        addParticleToBatch(particle2);
        
        auto particle3 = ParticleController::createParticle("ArmyFlag_3");
        particle3->setPosition(ccp(parentX+0, parentY+250));
        addParticleToBatch(particle3);
    }
    
    if (m_buildingKey == SPE_BUILD_CARGO) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("DailyLogGlow_1")->getCString());
        auto particle2 = ParticleController::createParticle(CCString::createWithFormat("DailyLogGlow_2")->getCString());
        particle1->setPosition(ccp(parentX + 60, parentY + 30));
        particle2->setPosition(ccp(parentX + 60, parentY + 30));
        addParticleToBatch(particle1);
        addParticleToBatch(particle2);
    }
}

void SpeBuild::drowPersonSpr(int zOrder, int tmpOrd)
{
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler SpeBuild::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler SpeBuild::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool SpeBuild::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_signNode", CCNode*, this->m_signCCBNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spr", CCSprite*, this->m_spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode", CCNode*, this->m_guideNode);
    return false;
}

void SpeBuild::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
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
    m_signLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void SpeBuild::onShowParticle()
{
    if (true) {
        auto tmp_ptArray = CCPointArray::create(20);
        if (m_buildingKey == BIG_TILE_1) {
            tmp_ptArray->addControlPoint(ccp(parentX+170, parentY+210));
        }
        else if (m_buildingKey == BIG_TILE_2) {
            tmp_ptArray->addControlPoint(ccp(parentX+270, parentY+210));
        }
        else if (m_buildingKey == BIG_TILE_3) {
            tmp_ptArray->addControlPoint(ccp(parentX+270, parentY+210));
        }
        else if (m_buildingKey == BIG_TILE_4) {
            tmp_ptArray->addControlPoint(ccp(parentX+270, parentY+210));
        }
        else if (m_buildingKey == BIG_TILE_5) {
            tmp_ptArray->addControlPoint(ccp(parentX+270, parentY+210));
        }
        for (int j=0; j<tmp_ptArray->count(); j++) {
            auto particle = ParticleController::createParticle("Leaf");
            particle->setPosition(tmp_ptArray->getControlPointAtIndex(j));
            addParticleToBatch(particle);
        }
    }
}

void SpeBuild::onHideParticle()
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        m_signLayer->removeChild(m_parVec[batchCount]);
    }
    m_parVec.clear();
}

void SpeBuild::onPlayUnlock()
{
    isCanClick = false;
    //begin d by ljf
    //this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(SpeBuild::playFadeOut));
    //this->getAnimationManager()->runAnimationsForSequenceIdTweenDuration(1, 0);
    //end d by ljf
    //begin a by ljf
    onShowParticle();
    this->schedule(schedule_selector(SpeBuild::dtPlayFadeOut), 0, 0, 1.0f);
    //playFadeOut();
    //end a by ljf
    
}

void SpeBuild::playFadeOut()
{
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    layer->onRemoveSpeBuild();
    onHideParticle();
    
}
void SpeBuild::updateShipState(string state){
    if(state.empty()){
        auto iter = GlobalData::shared()->monthCardInfoList.begin();
        bool shipIn = false;
        while (iter!=GlobalData::shared()->monthCardInfoList.end()) {
            if((*iter).second->canReward() && !(*iter).second->getAccepted()){
                shipIn = true;
                changeShipAnimation("come","collect");
                addShipPop(CCString::create("collect"));
                break;
            }else if((*iter).second->getBuy() == false){
                shipIn = true;
                changeShipAnimation("buy");
                addShipPop(CCString::create("buy"));
                break;
            }
            ++iter;
        }
        if(shipIn == false){
            changeShipAnimation("out");
            addShipPop(CCString::create("out"));
        }
    }else{
        changeShipAnimation(state);
        addShipPop(CCString::create(state));
    }
   
}
void SpeBuild::changeShipAnimation(string animation,string toAnimation,bool partical){
    m_shipToAnimation = toAnimation;
    if(!toAnimation.empty()){
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(SpeBuild::resetShipAnimation));
    }else{
        this->getAnimationManager()->setAnimationCompletedCallback(this,NULL);
    }
    if(partical){
        addShipParticle(animation);
    }
    m_shipCurAnimation = animation;
    this->getAnimationManager()->runAnimationsForSequenceNamed(animation.c_str());
}
void SpeBuild::resetShipAnimation(){
    this->getAnimationManager()->setAnimationCompletedCallback(this,NULL);
    m_shipCurAnimation=m_shipToAnimation;
    this->getAnimationManager()->runAnimationsForSequenceNamed(m_shipToAnimation.c_str());
    addShipParticle(m_shipToAnimation);
}
void SpeBuild::addShipParticle(string state){
    if(m_buildingKey != SPE_BUILD_SHIP)
        return;
    return;//fusheng 不添加粒子特效
    CCParticleBatchNode *newPBatch =NULL;
    if(!m_signLayer->getChildByTag(19999)){
       newPBatch = ParticleController::createParticleBatch();
       newPBatch->setTag(19999);
       m_signLayer->addChild(newPBatch);
    }else{
        newPBatch =dynamic_cast<CCParticleBatchNode*>(m_signLayer->getChildByTag(19999)) ;
    }
    CCSpriteBatchNode *newBatch = NULL;
    if(!m_signLayer->getChildByTag(20000)){
        newBatch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::createSprite("Effect_0277.png")->getTexture(), 10);
        newBatch->setTag(20000);
        m_signLayer->addChild(newBatch);
    }else{
        newBatch =dynamic_cast<CCSpriteBatchNode*>(m_signLayer->getChildByTag(20000)) ;
    }
    if(state.compare("collect") == 0){
        if(newPBatch){
            newPBatch->removeAllChildren();
            for (int i=1; i<3; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("BaoZangGlow_%d",i)->getCString());
                particle->setPosition(ccp(parentX+m_shipParticalRect[0].origin.x + m_shipParticalRect[0].size.width*0.5 ,parentY+m_shipParticalRect[0].origin.y +m_shipParticalRect[0].size.height*0.5));
                newPBatch->addChild(particle);
            }
        }
        if(newBatch){
            newBatch->removeAllChildren();
        }
    }else if(state.compare("buy") == 0){
        if(newBatch){
            newBatch->removeAllChildren();
            int cnt = 5;
            int idx = 0;
            while(idx<cnt){
                CCSize pNodeSize = m_shipParticalRect[1].size;
                ccColor3B color = {158, 158, 255};
                CCPoint pos = ccp(parentX+m_shipParticalRect[1].origin.x + m_shipParticalRect[1].size.width*0.55 ,parentY + m_shipParticalRect[1].origin.y);
                auto flyCell = FlyCell::createCycleLight("Effect_0277.png",pNodeSize.height,idx,cnt,newBatch,color,pos);
                ++idx;
            }
        }
        if(newPBatch){
            newPBatch->removeAllChildren();

        }
    }else{
        if(newPBatch){
            newPBatch->removeAllChildren();
        }
        if(newBatch){
            newBatch->removeAllChildren();
        }
    }
}
void SpeBuild::addShipPop(CCObject *ccObj){
    string iconName = "";
    m_touchName = "";
    if (PortActController::getInstance()->m_isRdLoginDay) {
        if(ccObj && dynamic_cast<CCString*>(ccObj)){
            string state = dynamic_cast<CCString*>(ccObj)->getCString();
            if(state.compare("buy")==0){
                iconName = "monthcard_icon.png";
                m_touchName = "monthcard";
            }
        }else{
            auto iter = GlobalData::shared()->monthCardInfoList.begin();
            while (iter!=GlobalData::shared()->monthCardInfoList.end()) {
                if((*iter).second->getBuy() == false){
                    iconName = "monthcard_icon.png";
                    m_touchName = "monthcard";
                    break;
                }
                ++iter;
            }
        }
    } else {
        iconName = "qiandao_icon.png";
        m_touchName = "qiandao";
        if(ccObj && dynamic_cast<CCString*>(ccObj)){
            string state = dynamic_cast<CCString*>(ccObj)->getCString();
            if(state.compare("init")==0){
                CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SpeBuild::addShipPop), PORT_LOGINRD_END, NULL);
            }
        }
    }
    m_textNode->removeAllChildren();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SpeBuild::onShowTextAniShip), this);
    float scale = 1.0f;
    if (iconName.empty()){
        return;
    }
    CCSprite* sp = CCLoadSprite::createSprite(iconName.c_str());
    sp->setPosition(ccp(0, 200));
    sp->setScale(scale);
    auto m_HTPHead = CCLoadSprite::createScale9Sprite("feedback_head.png");
    m_HTPHead->setInsetBottom(10);
    m_HTPHead->setInsetTop(5);
    m_HTPHead->setInsetLeft(5);
    m_HTPHead->setInsetRight(10);
    m_HTPHead->setAnchorPoint(ccp(0.5, 0.5));
    m_HTPHead->setContentSize(CCSize(100,100));
    m_HTPHead->setPosition(ccp(0, 200));
    m_textNode->addChild(m_HTPHead);
    m_HTPHead->setZOrder(1);
    auto m_HTPHead1 = CCLoadSprite::createSprite("feedback_head1.png");
    m_HTPHead1->setPosition(m_HTPHead->getPosition()+ccp(20.0, -58.0));
    m_HTPHead1->setZOrder(2);
    m_textNode->addChild(m_HTPHead1);
    m_countTime = 30;
    
    m_textNode->addChild(sp, 2);
    m_textNode->setVisible(false);
    
    m_textTouchNode->setContentSize(m_HTPHead->getContentSize());
    m_textTouchNode->setPosition( m_HTPHead->getPosition() + ccp(-m_HTPHead->getContentSize().width/2, -m_HTPHead->getContentSize().height/2) );
    m_textNode->addChild(m_textTouchNode, 3);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SpeBuild::onShowTextAniShip), this, 1,kCCRepeatForever, 0.0f, false);
}
void SpeBuild::onShowTextAniShip(float _time){
    if(!m_textNode) return ;
    
    if(m_countTime==5 && m_textNode->isVisible()){
        m_countTime = 0;
        m_textNode->setVisible(false);
    }
    if(!m_textNode->isVisible() && m_countTime>30){
        m_textNode->setVisible(true);
        m_countTime = 0;
    }
    m_countTime += 1;
}

void SpeBuild::hideNewRDTip(CCObject *ccObj)
{
    if (m_upEffectNode) {
        m_upEffectNode->stopAllActions();
        m_upEffectNode->removeAllChildrenWithCleanup(true);
        m_upEffectNode->setVisible(false);
    }
}