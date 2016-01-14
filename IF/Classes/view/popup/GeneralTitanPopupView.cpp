//
//  GeneralsPopupView.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-11.
//
//

#include "GeneralTitanPopupView.h"
#include "TitanFeedCommand.h"
#include "PropSpeedupView.h"
#include "TitanInfoCommand.h"
#include "BuildUpgradeView.h"
#include "TitanUpgradeView.h"
#include "UseResToolView.h"
//#include "GeneralManager.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "DataRecordCommand.h"
#include "MainCityView.h"
#include "TitanInView.h"
//#include "ParticleController.h"
//#include "ChangeNickNameView.h"
//#include "RollingText.h"
//#include "ChangePicView.h"
//#include "ShieldUnlockPopUpView.h"
//#include "SettingPopUpView.h"
#include "ToolController.h"
#include "YesNoDialog.h"
//#include "ChangePicCommand.h"
//#include "WorldController.h"
//#include "RoleInfoView.h"
//#include "AllRankListPopUpView.h"
#include "GeneralSkillListPopUpView.h"
#include "FunBuildController.h"
#include "SoundController.h"
#include "EquipmentController.h"
//#include "EquipmentInfo.h"
//#include "AchievementNewView.h"
#include "AchievementController.h"
#include "QueueController.h"
#include "ConfirmDialogView.h"
//
//#include "EquipNewUseView.h"
#include "SceneController.h"
//#include "ImperialScene.h"
//#include "WorldMapView.h"
//
//#include "GetUserInfoCommand.h"
//#include "AllianceInfoView.h"
//#include "InvitesAllianceCommand.h"
//#include "CheckAllianceInfoView.h"
//#include "MailController.h"
//#include "ActivityController.h"
//
////const int cellW = 640;
//static int cellW = 520;
#include "NBCommonUtils.h"
using namespace NBCommonUtils;

#define MSG_BUILD_CELL "msg_build_cell"

GeneralTitanPopupView* GeneralTitanPopupView::create(){
    GeneralTitanPopupView* ret = new GeneralTitanPopupView();
    ret->m_playerUid = "";
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//void GeneralsPopupView::createInfoByUid(const string &uid) {
//    if (uid == GlobalData::shared()->playerInfo.uid) {
//        PopupViewController::getInstance()->addPopupInView(GeneralsPopupView::create());
//        return;
//    }
//    
//    auto ret = new GeneralsPopupView();
//    auto cmd = new GetUserInfoCommand(uid);
//    cmd->setCallback(CCCallFuncO::create(ret, callfuncO_selector(GeneralsPopupView::onGetPlayerInfoCallback), nullptr));
//    cmd->sendAndRelease();
//    
//}
//
GeneralTitanPopupView::GeneralTitanPopupView():m_titanId(""),feedCD(0),beginUpdate(false),upgradeCD(0),isUpgrading(false),queue_id(-1),nbpb(NULL),upgradeCDTotal(0),isGuideFeed(false),m_arrow(NULL),perTimeForFeedCD(0),perTimeForUpgradeCD(0),_tmpGold(0),_resGold(0),_tmpTime(0),canResetUpgrade(true),isUptermsUnfold(false),canUpgrade(true){}


void GeneralTitanPopupView::onGetPlayerInfoCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        this->release();
        return;
    }
    
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        this->release();
        return;
    }
    
    this->m_playerUid = "";
    auto playerInfo = new PlayerInfo();
    playerInfo->updateInfo(info);
    this->m_playerUid = playerInfo->uid;
    this->m_info = playerInfo;
    if(info->objectForKey("alliance")){
        auto dict = dynamic_cast<CCDictionary*>(info->objectForKey("alliance"));
        if(dict->objectForKey("alliance")){
            auto alliance = dynamic_cast<CCDictionary*>(dict->objectForKey("alliance"));
            if(alliance){
                playerInfo->setAllianceId(alliance->valueForKey("uid")->getCString());
            }
            playerInfo->allianceInfo.updateAllianceInfo(alliance);
            playerInfo->allianceInfo.serverId = playerInfo->selfServerId;
        }
    }
    this->m_medalIconId = info->valueForKey("medal")->getCString();
    float pro = info->valueForKey("achieveNum")->floatValue();
    this->m_achievePro = CCString::createWithFormat("%.f", pro*100)->getCString();
    CCArray* medalList = dynamic_cast<CCArray*>(info->objectForKey("medalList"));
    this->m_medalCntStr = "0";
    if (medalList)
    {
        int count = medalList->count();
        this->m_medalCntStr = CC_ITOA(count);
    }
    AchievementController::getInstance()->getOtherMedalInfo(medalList);
    
    this->init();
    
    string data = "action=openFriendRoleInfoView";
    GameController::getInstance()->callXCApi(data);
    if (m_medalIconId.length() > 0)
    {
        data = "action=displayMedal.";
        data.append(m_medalIconId);
        GameController::getInstance()->callXCApi(data);
    }
    
    PopupViewController::getInstance()->addPopupInView(this);
    
    this->release();
    CC_SAFE_RELEASE_NULL(playerInfo);
}



void GeneralTitanPopupView::updateTitanInfo(CCObject* obj)
{
    //fusheng 监听的是建筑队列完成  建筑属性还没有改变  所以给0.1秒时间反应
//    auto seq = Sequence::createWithTwoActions(DelayTime::create(0.1), CCCallFunc::create(
//                                                                                         [this]
//    {
//       
//    }
//                                                                                         ));
    
//    canResetUpgrade = true;
    updateInfo(nullptr);
    
//    seq->setTag(1234567);
//    
//    this->runAction(seq);
    
    
}

void GeneralTitanPopupView::updateInfo(CCObject* obj)
{

    canUpgrade = true;//fusheng 默认可以升级
    
    auto& m_info = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    
    if(m_info.state == FUN_BUILD_UPING)//fusheng 升级中不显示条件
    {
        m_scrollView->getContainer()->removeAllChildren();//升级按钮是否可点
        
        m_instantBtn->setEnabled(false);
        
        m_upBtn->setEnabled(false);
        
        m_titanUpingTip->setVisible(true);
        
        canResetUpgrade = false;//升级中为false;
        
        m_timeSpr1->setVisible(false);
        
        m_timeLabel->setVisible(false);
        
        return;
    }
    else
    {
         m_timeSpr1->setVisible(true);
        
         m_timeLabel->setVisible(true);
        
         m_titanUpingTip->setVisible(false);
        
         canResetUpgrade = true;
    }
    
    if (!canResetUpgrade) {//阻止显示条件
        return;
    }
    
    bool isCanUp = true;
    
    string name = "";
    int silver_need = 0;
    int wood_need = 0;
    int time_need = 0;
    int stone_need = 0;
    int food_need = 0;
    int iron_need = 0;
    string tool_need = "";
    string building = "";
    string destip = "";
    int level = 0;
    


    name = m_info.name;
    silver_need = m_info.silver_need;
    wood_need = m_info.wood_need;
    time_need = m_info.time_need;
    stone_need = m_info.stone_need;
    food_need = m_info.food_need;
    iron_need = m_info.iron_need;
    tool_need = m_info.tool_need;
    building = m_info.building;
    level = m_info.level;
    //        destip = m_info.destip;
    destip = m_info.tip;
    
    _tmpTime = time_need;
    m_nameLabel->setString(_lang(name));
    
    //    string title = _lang_1("102272", CC_ITOA(level));
    string title = _lang_1("102272", " ");
    if(level>=GlobalData::shared()->MaxBuildLv) {
        title += _lang("102183");
    }
    m_lvLabelPre->setString(title);
    
    m_lvLabel->setString(CC_ITOA(level));
    vector<cocos2d::CCLabelIF *> labels;
    labels.push_back(m_lvLabelPre);
    labels.push_back(m_lvLabel);
    arrangeLabel(labels);
    
    //    string nextLv = _lang_1("102272", );
    m_nextLvLabelPre->setString(title);
    m_nextLvLabel->setString(CC_ITOA(level+1));
    
    labels.clear();
    labels.push_back(m_nextLvLabelPre);
    labels.push_back(m_nextLvLabel);
    arrangeLabel(labels);
    
    float tmpEffect = CCCommonUtils::getEffectValueByNum(68);
    _tmpTime = time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
    _tmpGold = CCCommonUtils::getGoldByTime(_tmpTime);
    _resGold = 0;
    
    string timeInfo = CC_SECTOA(_tmpTime);
    m_timeLabel->setString(timeInfo.c_str());
    
    
    int _itemH = 46;
    int curY = 25;
    int curX = 30;
    
    if (CCCommonUtils::isIosAndroidPad()) {
        _itemH = 87;
        curX = 60;
        curY = 50;
    }
    
    m_instantBtn->setEnabled(true);
    m_scrollView->getContainer()->removeAllChildren();
    
    m_lfood = 0;
    m_lwood = 0;
    m_liron = 0;
    m_lstone = 0;
    

    
    
    
    if (stone_need>0) {
        auto item3 = UpgradeCell::create(0, Stone, stone_need);
        item3->setPosition(ccp(curX, curY));
        item3->setTouchNode(m_infoList,FUN_BUILD_MAIN_CITY_ID);
        m_scrollView->addChild(item3);
        curY += _itemH;
        
        if (!item3->m_isOk) {
            //            isCanUp = false;
        }
        
        int tmpSub = stone_need - GlobalData::shared()->resourceInfo.lStone;
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Stone, tmpSub);
            m_lstone = tmpSub;
        }
    }
    if (iron_need>0) {
        auto item4 = UpgradeCell::create(0, Iron, iron_need);
        item4->setPosition(ccp(curX, curY));
        item4->setTouchNode(m_infoList,FUN_BUILD_MAIN_CITY_ID);
        m_scrollView->addChild(item4);
        curY += _itemH;
        
        if (!item4->m_isOk) {
            //            isCanUp = false;
        }
        
        int tmpSub = iron_need - GlobalData::shared()->resourceInfo.lIron;
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Iron, tmpSub);
            m_liron = tmpSub;
        }
    }
    if (food_need>0) {
        auto item5 = UpgradeCell::create(0, Food, food_need);
        item5->setPosition(ccp(curX, curY));
        item5->setTouchNode(m_infoList,FUN_BUILD_MAIN_CITY_ID);
        m_scrollView->addChild(item5);
        curY += _itemH;
        
        if (!item5->m_isOk) {
            //            isCanUp = false;
        }
        
        int tmpSub = food_need - GlobalData::shared()->resourceInfo.lFood;
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Food, tmpSub);
            m_lfood = tmpSub;
        }
    }
    
    
    if (wood_need>0) {
        auto item2 = UpgradeCell::create(0, Wood, wood_need);
        item2->setPosition(ccp(curX, curY));
        item2->setTouchNode(m_infoList,FUN_BUILD_MAIN_CITY_ID);
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        if (!item2->m_isOk) {
            //            isCanUp = false;
        }
        
        int tmpSub = wood_need - GlobalData::shared()->resourceInfo.lWood;
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Wood, tmpSub);
            m_lwood = tmpSub;
        }
    }
    
    if (tool_need != "") {
        std::vector<string> toolItems;
        CCCommonUtils::splitString(tool_need, "|", toolItems);
        for (int i=0; i<toolItems.size(); i++) {
            string curTool = toolItems[i];
            std::vector<string> item;
            CCCommonUtils::splitString(curTool, ";", item);
            int id = atoi(item[0].c_str());
            int num = atoi(item[1].c_str());
            
            auto itemTool = UpgradeCell::create(1, id, num);
            itemTool->setPosition(ccp(curX, curY));
            itemTool->setTouchNode(m_infoList,FUN_BUILD_MAIN_CITY_ID);
            m_scrollView->addChild(itemTool);
            curY += _itemH;
            
            if (!itemTool->m_isOk) {
                auto toolInfo = ToolController::getInstance()->getToolInfoById(id);
                _resGold += (num - toolInfo.getCNT()) * toolInfo.price;
                //                isCanUp = false;
            }
        }
    }
    
    _tmpGold = _tmpGold + _resGold;
    m_inBtnGoldNum->setString(CC_ITOA(_tmpGold));
    
    if (building!="") {
        std::vector<string> buildItems;
        CCCommonUtils::splitString(building, "|", buildItems);
        for (int i=0; i<buildItems.size(); i++) {
            string curBuild = buildItems[i];
            std::vector<string> tmpbuildVec;
            CCCommonUtils::splitString(curBuild, ";", tmpbuildVec);
            int paramBuild = atoi(tmpbuildVec[0].c_str())+atoi(tmpbuildVec[1].c_str());
            auto itemBuild = UpgradeCell::create(2, paramBuild, 0);
            itemBuild->setPosition(ccp(curX, curY));
            itemBuild->setTouchNode(m_infoList,FUN_BUILD_MAIN_CITY_ID);
            m_scrollView->addChild(itemBuild);
            curY += _itemH;
            
            if (!itemBuild->m_isOk) {
                isCanUp = false;
               // m_instantBtn->setEnabled(false);
            }
        }
    }
    if(GlobalData::shared()->titanInfo.level<GlobalData::shared()->MaxBuildLv) {//fusheng 没有满级  龙等级满级使用建筑满级
        //fusheng 泰坦喂食状态
        auto item_titanStatus = UpgradeCell::create(8, 0, 0);
        item_titanStatus->setPosition(ccp(curX, curY));
        item_titanStatus->setTouchNode(m_infoList,FUN_BUILD_MAIN_CITY_ID);
        m_scrollView->addChild(item_titanStatus);
        curY += _itemH;
    }
    
    
    //没有队列，显示条件不足
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, 1);//_tmpTime, 取空闲队列，并不判断有效期是否够用
    if(QID_MAX == qid)
    {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
        auto itemQueue = UpgradeCell::create(3, Food, qid);
        itemQueue->setPosition(ccp(curX, curY));
        itemQueue->setTouchNode(m_infoList,FUN_BUILD_MAIN_CITY_ID);
        m_scrollView->addChild(itemQueue);
        curY += _itemH;
        m_qid = qid;
    }
    else {
        m_qid = qid;
    }
//    m_upBtn->setEnabled(isCanUp);
    
    canUpgrade = isCanUp;
    
    curY -= 23;
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
    bool touchable = curY < m_infoList->getContentSize().height?false:true;
    m_scrollView->setTouchEnabled(touchable);
    
    //fusheng 添加喂食度判断
    if(GlobalData::shared()->titanInfo.exp<GlobalData::shared()->titanInfo.nextExp)
    {
//        m_instantBtn->setEnabled(false);
//        m_upBtn->setEnabled(false);
        
        canUpgrade = false;//fusheng canUpgrage表示按钮可点击时不同分支判断条件
    }

    
    if(GlobalData::shared()->titanInfo.level >= GlobalData::shared()->MaxBuildLv || GlobalData::shared()->titanInfo.level != m_info.level) {//fusheng 没有满级  龙等级满级使用建筑满级    当龙的等级和建筑等级不一致时不可以升级
        m_instantBtn->setEnabled(false);
        m_upBtn->setEnabled(false);
        canUpgrade = false;
    }
    
    if(!canUpgrade && m_upBtn->isEnabled())//fusheng 按钮可点击 且 现在不能升级时 置灰
    {
        m_upBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
        
        m_upBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::SELECTED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
        
        m_upBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::HIGH_LIGHTED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
        
        
        m_instantBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
        
        m_instantBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::SELECTED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
        
        m_instantBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::HIGH_LIGHTED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
        
    }
    else if (canUpgrade && m_upBtn->isEnabled())//fusheng 按钮可点击 且 可升级时不置灰
    {
        m_upBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
        
        m_upBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::SELECTED)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
        
        m_upBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::HIGH_LIGHTED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
        
        
        
        m_instantBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
        
        m_instantBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::SELECTED)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
        
        m_instantBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::HIGH_LIGHTED)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);

    }
    
    
}

//void GeneralTitanPopupView::onEnter()
//{
//    CCNode::onEnter();
//    UIComponent::getInstance()->showResourceBar(true);
//    if (m_openNum>0) {
//        updateInfo(NULL);
//    }
//    
//    m_openNum++;
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TitanUpgradeView::updateInfo), MSG_REFREASH_BUILD_UPGRADE, NULL);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TitanUpgradeView::updateTitanInfo), MSG_TITAN_INFORMATION_RESET, NULL);
//    setTouchEnabled(true);
//    int sid = SceneController::getInstance()->currentSceneId;
//    
//    
//    
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
//}

//void TitanUpgradeView::onExit()
//{
//    UIComponent::getInstance()->showResourceBar(false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_BUILD_UPGRADE);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_INFORMATION_RESET);
//    
//    setTouchEnabled(false);
//    
//    //    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD)
//    //    {
//    //        CCLoadSprite::doResourceByPathIndex(IMPERIAL_PATH,22, true);
//    //    }
//    CCNode::onExit();
//}

bool GeneralTitanPopupView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    onShowNextUnlockItem();
    
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(513, true);
    
    
    this->setCleanFunction([]{
        CCLoadSprite::doResourceByCommonIndex(305, false);
        CCLoadSprite::doResourceByCommonIndex(513, false);
        
    });
    
    auto bg = CCBLoadFile("GeneralTitanDetail",this,this);
    

    
    
    m_upBtnMsgLabel->setString(_lang("102104").c_str());
    m_btnMsgLabel->setString(_lang("102127").c_str());
    
    if(m_arrow == NULL)
    {
        m_arrow = CCLoadSprite::createSprite("UI_hand.png");
        m_arrow->setFlipX(true);
        m_arrow->setFlipY(true);
        
        m_arrow->setAnchorPoint(Vec2(0.5,0));
        
        m_arrow->setRotation(180);
        
        m_arrow->setPosition( m_titanFeedBtn->getPosition()+Vec2(0,m_titanFeedBtn->getContentSize().height/2+m_arrow->getContentSize().height));
        
        m_titanExtNode->addChild(m_arrow);
        
        auto move1 = MoveBy::create(0.25, Vec2(0,-m_titanFeedBtn->getContentSize().height/4));
        
        auto move2 = MoveBy::create(0.25, Vec2(0,m_titanFeedBtn->getContentSize().height/4));
        
        auto seq = Sequence::create(move1,move2, NULL);
        
        auto rep = RepeatForever::create(seq);

        rep->setTag(89757);
        
        m_arrow->setVisible(false);
        
        m_arrow->runAction(rep);
    }
        
    

   
    setContentSize(bg->getContentSize());
    
    setIsHDPanel(true);

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    if (!CCCommonUtils::isIosAndroidPad()) {
        //changeBGHeight(m_buildBG);
        CCSize temp = m_buildBG->getContentSize();
        temp.height=size.height;
        m_buildBG->setContentSize(temp);
    }
    float extH = getExtendHeight();
    float add =  size.height - 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        add =  size.height - 2048;
        extH = size.height - 2048;
    }
    m_bottomNode->setPositionY(m_bottomNode->getPositionY()-extH);
    
    //m_mainNode->setPosition();
    if(add>108)
    {
        float tempX = (add - 108)/2;
        
        if(tempX>50)//上移50像素
            tempX -= 50;
            
        m_mainNode->setPositionY(m_mainNode->getPositionY()-108-tempX);
        

        
    }
    ;
    float infoListTop  = m_infoList->convertToWorldSpaceAR(Vec2::ZERO).y;
    float infoListBottom = m_DashedBL->convertToWorldSpaceAR(Vec2::ZERO).y;
    
    Size sm_infoList = m_infoList->getContentSize();
    sm_infoList.height = infoListTop - infoListBottom;
    
    m_infoList->setContentSize(sm_infoList);
    
    m_titanUpingTip->setNormalizedPosition(Vec2(0.5,0.5));
    
//    m_titanUpingTip->setString("hello uping");
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());//fusheng
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    m_UpTermsBG->setContentSize(Size(m_DashedTR->convertToWorldSpace(Vec2::ZERO).x - m_DashedBL->convertToWorldSpace(Vec2::ZERO).x,m_DashedTR->convertToWorldSpace(Vec2::ZERO).y-m_DashedBL->convertToWorldSpace(Vec2::ZERO).y));
    
    
    

    
    //fusheng 产生一个虚线外框
//    Node* node = createDashed(Size(m_DashedTR->convertToWorldSpace(Vec2::ZERO).x - m_DashedBL->convertToWorldSpace(Vec2::ZERO).x,m_DashedTR->convertToWorldSpace(Vec2::ZERO).y-m_DashedBL->convertToWorldSpace(Vec2::ZERO).y));
//    m_DashedBL->addChild(node);
    
//    auto pp = m_infoList->convertToNodeSpace( m_DashedBL->convertToWorldSpace(Vec2::ZERO));

    
    
    this->m_receiveGlow->setVisible(false);
    bool isOpenLongJing = false;

//    m_Titan = Titan::create(1);
//    m_Titan->setScale(0.7);
//    m_titanPos->addChild(m_Titan);
    auto tieanInView = TitanInView::create();
    tieanInView->setTag(10086);
    m_titanPosInView->addChild(tieanInView );
//    auto sdsds = cocos2d::Label::createWithBMFont(getNBFont(NB_FONT_Bold), "222221122");
//        this->m_titanPos->addChild(sdsds);
    
    m_ProTitanExtMaxWidth = m_ProTitanExt->getContentSize().width;
    
    m_ProTiTanAPMaxWidth = m_ProTiTanAP->getContentSize().width;
 
    m_ProTitanExt->setVisible(true);
    

    
    this->scheduleUpdate();
    
    m_feedCDBtnTxt->setString(_lang("500003").c_str());
    
    
    this->m_titanStatusDesc->setString(_lang("500006"));//fusheng 需要文本
    

    
    m_titanFeedBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    
    m_toolSpeedUpBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    
    m_CleanFeedCDBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    

//    m_titanFeedBtn->setZoomOnTouchDown(false);
//    
//    m_CleanFeedCDBtn->setZoomOnTouchDown(false);//fusheng 不做放缩
//    
//    m_toolSpeedUpBtn->setZoomOnTouchDown(false);
//    
//    m_speedUpBtn->setZoomOnTouchDown(false);

    
    m_titanAPTxtPre->setString("AP");//fusheng 需要文本
    
//    m_titanExtTxtPre->setString("Exp");//fusheng 需要文本
    
    m_toolSpeedUpTxt->setString(_lang("500009"));
    
    m_dragonTip->setString(_lang("500017"));
    
    
    Scale9Sprite* Mask = Scale9Sprite::createWithSpriteFrameName("nb_feedMask.png");//fusheng titan升级时间进度条
    
    Mask->setCapInsets(Rect(10,1,10,11));
    
    auto maskSize = Mask->getContentSize();
    
    maskSize.width += m_titanUpgradePro->getContentSize().width;
    
    Mask->setContentSize(maskSize);
    
    auto spMask =convertNodeToSprite(Mask);

    
    nbpb = NBProgressBar::create(m_titanUpgradePro,spMask,0.2,-10);
    
    nbpb->setPosition(75,-132);
    
    titanUpingNode->addChild(nbpb);
    
    nbpb->setVisible(false);
    
    
    
    

    
  
    return true;
}

CCNode* GeneralTitanPopupView::getGuideNode(string _key)
{
    
    FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    if (_key == "Titan_Feed") {
        
        if(fbiInfo.state == FUN_BUILD_UPING || fbiInfo.level>1)//fusheng 升级中或者级别大于1
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Feed"));
            return NULL;
        }
        else
        {
            if (!m_titanFeedBtn->isVisible() || !m_titanFeedBtn->isEnabled()) {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Feed"));
                return NULL;
            }else {
                return m_titanFeedBtn;
            }
        }
        
    }
//    else if (_key == "Titan_Up") {
//        if(fbiInfo.state == FUN_BUILD_UPING || fbiInfo.level>1)//fusheng 升级中或者级别大于1
//        {
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Up"));
//            return NULL;
//        }
//        return m_titanUngrade;
//    }
    else if (_key == "Titan_Speed") {
        if(fbiInfo.state == FUN_BUILD_UPING && fbiInfo.level==1)//fusheng 1级且升级中
        {
            if (!m_speedUpBtn->isVisible() || !m_speedUpBtn->isEnabled()) {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Speed"));
                return NULL;
            }else {
                return m_speedUpBtn;
            }
            
        }
        else
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Speed"));
            return NULL;
        }
        
    }else if (_key == "Titan_Close") {
        GeneralTitanPopupView* generalTitanPopupView = dynamic_cast<GeneralTitanPopupView*>(PopupViewController::getInstance()->getCurrentPopupView());
        
        if(generalTitanPopupView)
        {
            return UIComponent::getInstance()->m_popupReturnBtn;//fusheng 当前界面是GeneralTitanPopupView时
        }
        return NULL;
    }else if(_key == "Titan_End")
    {
        if(fbiInfo.state != FUN_BUILD_UPING && fbiInfo.level==1)//fusheng 1级且不在升级中
        {
           return m_upBtn;
        }
        else
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_End"));
        }
        
        
        
    }
    return NULL;
}

void GeneralTitanPopupView::handleTitanUpgrade(CCObject* obj)
{
    
    
    string evolution = CCCommonUtils::getPropById(CC_ITOA(GlobalData::shared()->titanInfo.tid), "evolution");
    
    if(evolution == "1")
    {
        if(GlobalData::shared()->titanInfo.level == 2)
        {
            
            
            TitanInView* view = dynamic_cast<TitanInView*>(  m_titanPosInView->getChildByTag(10086));
            view->m_Titan->changeTitanState(Titan::eActState::Cheer);//fusheng 蛋破碎
            if (view) {
                
                CCLOG("GeneralTitanPopupView titan sheng ji ");
                //            view->setVisible(false);
            }
            
            auto node =DragonUpgradeAniNode::create(GlobalData::shared()->titanInfo.tid);
            
            m_titanPosInView->addChild(node);
            
        }
        else if(true)
        {
        
//            TitanInView* view = dynamic_cast<TitanInView*>(  m_titanPosInView->getChildByTag(10086));
//            if (view) {
//                view->removeFromParent();
//                
//                auto tieanInView = TitanInView::create();
//                
//                tieanInView->setTag(10086);
//                
//                m_titanPosInView->addChild(tieanInView );
//                
//            }
            auto node = Node::create();
            node->setTag(10010);
            auto particle = ParticleController::createParticle("SmithyFireLoop_2");//fusheng 添加粒子特效
            for(int i =0 ;i<9;i++)
            {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonLv_%d",i)->getCString());
                if(particle)
                {
                    node->addChild(particle);
                    
                    if (i<2) {
                        particle->setPositionY(223);
                    }
                }
                
            }
            node->setScale(1.5);
            m_titanPosInView->addChild(node);
            
            
            this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create( CC_CALLBACK_0(GeneralTitanPopupView::aniCallBack, this )),nullptr));//fusheng 固定时间
            
        }
        
    }
    
}
void GeneralTitanPopupView::aniCallBack()
{
    handleTianUpgradeAnimationComplete(NULL);
}

void GeneralTitanPopupView::handleTianUpgradeAnimationComplete(CCObject* obj)
{
//    if(GlobalData::shared()->titanInfo.level == 2)
//    {
//        
    
    TitanInView* view = dynamic_cast<TitanInView*>(  m_titanPosInView->getChildByTag(10086));
    
    if (view) {
        
        //            view->resetDisplay(GlobalData::shared()->titanInfo.tid);//fusheng 换成龙
        view->removeFromParent();
        
        auto tieanInView = TitanInView::create();
        
        tieanInView->setTag(10086);
        
        m_titanPosInView->addChild(tieanInView );
        
        tieanInView->m_Titan->changeTitanState(Titan::eActState::Cheer);//fusheng 升级动画完成后 播放一个cheer
        
        CCLOG("GeneralTitanPopupView titan sheng ji animation complete");
        //            view->setVisible(true);
    }
    CCParticleSystemQuad* particle = dynamic_cast<CCParticleSystemQuad*>(  m_titanPosInView->getChildByTag(10010));
    
    if(particle)
    {
        particle->removeFromParent();
    }
    
//    }
    
}


void GeneralTitanPopupView::resetAttribute(CCObject* obj)
{
    CCDictionary *params = dynamic_cast<CCDictionary*>(obj);

    
    m_titanInfo = GlobalData::shared()->titanInfo;
    
    
    if(m_titanInfo.titanId=="")
    {
        return;
    }

   
    if(m_titanInfo.titanId!="")
    {
        m_titanId = m_titanInfo.titanId;
        ArmyInfo armTitanInfo = GlobalData::shared()->armyList[m_titanId]; //fusheng map中通过id获得泰坦属性
        m_titanAttackTxt->setString(CC_ITOA(armTitanInfo.attack));
        m_titanMagicTxt->setString(CC_ITOA(armTitanInfo.attack));
        m_Txt1->setString(CC_ITOA(armTitanInfo.attack));
        m_Txt2->setString(CC_ITOA(armTitanInfo.attack));
    }
  
    m_instantBtn->setEnabled(true);//fusheng 设置为默认值
    
    m_upBtn->setEnabled(true);
    
    beginUpdate = false;
    
    isUpgrading = false;
    
    
    
    
    
    bool isCanFeed = true;
    
    isFoodEnough = true;//fusheng 标记粮食充足
    
    FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    
    
    if( GlobalData::shared()->titanInfo.level == fbiInfo.level)
        updateInfo(obj);//fusheng 升级信息

    

    m_nameLabel->setString(_lang(fbiInfo.name));
    
    
    if(m_titanInfo.status==0)//fusheng 暂时没写 出征后龙的不同
    {
        
    }
    else
    {
        
    }
    int level = m_titanInfo.level;
    
    string title = _lang_1("102272", " ");
    if(level>=GlobalData::shared()->MaxBuildLv) {
        title += _lang("102183");
        m_nextLvLabelPre->setVisible(false);
        m_nextLvLabel->setVisible(false);
        nb_arrow->setVisible(false);
        
        
        ArmyInfo titanInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId]; //fusheng map中通过id获得
        
        m_titanCurrentZDLInNext->setString(CC_ITOA(titanInfo.power));
    }
    else
    {
        m_nextLvLabelPre->setVisible(true);
        m_nextLvLabel->setVisible(true);
        nb_arrow->setVisible(true);
        
        
        
        ArmyInfo titanInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId]; //fusheng map中通过id获得泰坦属性
        ArmyInfo nextTitanInfo = GlobalData::shared()->armyList[ CC_ITOA(CCString::create(GlobalData::shared()->titanInfo.titanId)->intValue()+1)];
        
        m_titanCurrentZDLInNext->setString(CC_ITOA(titanInfo.power));
        m_titanZDLAddValue->setString(CCString::createWithFormat("+%.1f",nextTitanInfo.power-titanInfo.power)->getCString());
        

    }
    m_lvLabelPre->setString(title);
    
    m_lvLabel->setString(CC_ITOA(level));
    vector<cocos2d::CCLabelIF *> labels;
    labels.push_back(m_lvLabelPre);
    labels.push_back(m_lvLabel);
    arrangeLabel(labels);
    
    
    m_nextLvLabelPre->setString(title);
    m_nextLvLabel->setString(CC_ITOA(level+1));
    
    
    labels.clear();
    labels.push_back(m_nextLvLabelPre);
    labels.push_back(m_nextLvLabel);
    arrangeLabel(labels);
    
    
    //fusheng  begin 体力
    if(m_titanInfo.maxManual!=0)
    {
        float ratio =((float)m_titanInfo.currentManual)/m_titanInfo.maxManual;
        
        auto size = this->m_ProTiTanAP->getContentSize();
        
        size.width = m_ProTiTanAPMaxWidth * ratio;
        
        
        m_ProTiTanAP->setVisible(ratio != 0);
        
        auto oriSize = m_ProTiTanAP->getOriginalSize();
        
        if (size.width<oriSize.width) {
            
            this->m_ProTiTanAP->setContentSize(oriSize);
            
        }
        else
        {
            this->m_ProTiTanAP->setContentSize(size);
            
        }
        
        
        
        
        this->m_titanAPTxt_0->setString("(");
        this->m_titanAPTxt_1->setString(CCString::createWithFormat("%d",m_titanInfo.currentManual<0?0:m_titanInfo.currentManual)->getCString());
        this->m_titanAPTxt_2->setString(CCString::createWithFormat("/%d)",m_titanInfo.maxManual)->getCString());
        
        vector<cocos2d::CCLabelIF *> labels;
        labels.push_back(m_titanAPTxtPre);
        labels.push_back(m_titanAPTxt_0);
        labels.push_back(m_titanAPTxt_1);
        labels.push_back(m_titanAPTxt_2);
        arrangeLabel(labels);
        
    }
    else
    {
        
        auto size = this->m_ProTiTanAP->getContentSize();
        size.width = 0;
        this->m_ProTiTanAP->setContentSize(size);
        //        this->m_titanAPTxt->setString("数据异常");
        
    }
    //fusheng  end 体力
    
    if(level<GlobalData::shared()->MaxBuildLv) {//fusheng 没有满级  龙等级满级使用建筑满级
       
    }
    else
    {
       
    }
    ;
    
    if(fbiInfo.state == FUN_BUILD_UPING)
    {
//        CCCommonUtils::setSpriteGray(m_titanUngrade, true);
        changeUpterms(true,bottomBraceState::BBCLOSE);

        
        this->m_titanFeedBtn->setEnabled(false);//fusheng 泰坦升级时
        
        titanFeedNode->setVisible(false);
        
        titanUpingNode->setVisible(true);
        
        this->upgradeCD = (fbiInfo.updateTime - GlobalData::shared()->getWorldTime());
        
        perTimeForUpgradeCD = 0;//fusheng 重0计算
        
        isUpgrading = true;
        
        beginUpdate = true;
        
       
        
        if (GlobalData::shared()->allQueuesInfo.find(1102) != GlobalData::shared()->allQueuesInfo.end()) {
            
            string key = GlobalData::shared()->allQueuesInfo[1102].key;
            

            if (GlobalData::shared()->allQueuesInfo[1102].itemId == 400000) {
                queue_id = 1102;
            }
            
            
        }
        
        if (GlobalData::shared()->allQueuesInfo.find(1101) != GlobalData::shared()->allQueuesInfo.end()) {
            
            string key = GlobalData::shared()->allQueuesInfo[1101].key;
            
            
            if (GlobalData::shared()->allQueuesInfo[1101].itemId == 400000) {
                queue_id = 1101;
            }
            
        }
        
        if (GlobalData::shared()->allQueuesInfo.find(queue_id) != GlobalData::shared()->allQueuesInfo.end()) {
            
           upgradeCDTotal = GlobalData::shared()->allQueuesInfo[1101].finishTime - GlobalData::shared()->allQueuesInfo[1101].startTime;
        }
        
        
        int toolID = ToolController::getInstance()->getSPDItem(ItemSpdMenu_City);
        
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(toolID);
        
        
//        auto sp = m_toolSpeedUpBtn->getBackgroundSpriteForState(Control::State::DISABLED);
        
        
        
        if(toolInfo.getCNT()>0)//fusheng 判断加速道具个数
        {

            
            setGrayNode(m_toolSpeedUpBtnNode, true);
            
            m_toolSpeedUpTxt->setColor(ccColor3B(0,73,19));
            
            
            
            m_toolSpeedUpBtn->setEnabled(true);
            
            setGrayNode(m_toolSpeedUpBtnNode, false);
        }
        else
        {
            m_toolSpeedUpTxt->setColor(ccGRAY);
          
            setGrayNode(m_toolSpeedUpBtnNode, true);
            
            m_toolSpeedUpBtn->setEnabled(false);
        }
        
        
        
        if (upgradeCD>=0) {
            
            string timeInfo = CC_SECTOA((int)this->upgradeCD);
            this->m_upgradeCDTxt->setString(timeInfo);
            
            
            //        if(upgradeCDTotal!=0&&upgradeCDTotal>0) //fusheng 进度条
            //        {
            //            float ratio = (upgradeCDTotal-upgradeCD)/upgradeCDTotal;
            //            nbpb->setPercent(ratio);
            //        }
            //        else
            //        {
            //            nbpb->setPercent(1);
            //        }
            
        }
        
        CCCommonUtils::setSpriteGray(m_BottomBraceArrow, true);
    }
    else
    {
        CCCommonUtils::setSpriteGray(m_BottomBraceArrow, false);
        CCCommonUtils::setSpriteGray(m_titanUngrade, false);
        titanFeedNode->setVisible(true);
        titanUpingNode->setVisible(false);

        
        isUpgrading = false;
        
        
        
        //fusheng begin 经验
        if(m_titanInfo.nextExp!=0)
        {
            
            
            this->m_titanExtTxt_0->setString("(");
            this->m_titanExtTxt_1->setString(CCString::createWithFormat("%s",CC_ITOA_K((long)m_titanInfo.exp))->getCString());
            this->m_titanExtTxt_2->setString(CCString::createWithFormat("/%s)",CC_ITOA_K((long)m_titanInfo.nextExp))->getCString());
            vector<cocos2d::CCLabelIF *> labels;
            labels.push_back(m_titanExtTxt_0);
            labels.push_back(m_titanExtTxt_1);
            labels.push_back(m_titanExtTxt_2);
            arrangeLabel(labels);
            
            float ratio =((float)m_titanInfo.exp)/m_titanInfo.nextExp;
            
            if(ratio > 1)
                ratio = 1;
            
            auto size = this->m_ProTitanExt->getContentSize();
            
            size.width = m_ProTitanExtMaxWidth*ratio;
            
            m_ProTitanExt->setVisible(ratio != 0);
            
            auto oriSize = m_ProTitanExt->getOriginalSize();
            
            if (size.width<oriSize.width) {
                
                this->m_ProTitanExt->setContentSize(oriSize);
                
            }
            else
            {
                this->m_ProTitanExt->setContentSize(size);
                
            }
            
        }
        else
        {
            
            
            auto size = this->m_ProTitanExt->getContentSize();
            size.width = 0;
            
            this->m_ProTitanExt->setContentSize(size);
            
        }
        //fusheng end 经验

        
       
        
        
       
        
        //fusheng begin 经验 and feedCD
        if(m_titanInfo.exp>=m_titanInfo.nextExp)
        {
            isCanFeed = false;
            this->m_CleanFeedCDBtn->setEnabled(false);
            this->m_CleanFeedCDBtnNode->setVisible(false);
            
            this->m_titanFeedBtnNode->setVisible(true);
            
            
            m_titanFeedTxt->setString(_lang("500002"));//fusheng 需要文本
            if(GlobalData::shared()->titanInfo.level == 1)
                m_titanFeedTxt->setString(_lang("500011"));//fusheng 孵化
            changeUpterms(true,bottomBraceState::BBSHOW);
        }
        else
        {
            
            if(m_titanInfo.feedcdfix!=0)//fusheng 修改 feedCD
            {
                
                this->feedCD = m_titanInfo.feedcdfix - GlobalData::shared()->getWorldTime();
                
                perTimeForFeedCD = 0;//fusheng 重0计算
                
                if (this->feedCD <= 0) {
                    this->feedCD=0;
                    beginUpdate |= false;
                    
                    
                    this->m_CleanFeedCDBtn->setEnabled(false);
                    this->m_CleanFeedCDBtnNode->setVisible(false);
                    
                    this->m_titanFeedBtn->setEnabled(true);
                    this->m_titanFeedBtnNode->setVisible(true);
                    //            titanFeedNode->setVisible(true);
                    //            titanUpingNode->setVisible(false);
                    
                    
                }
                else
                {
                    beginUpdate = true;
                    this->m_CleanFeedCDBtn->setEnabled(true);
                    m_CleanFeedCDBtnNode->setVisible(true);
                    
                    this->m_titanFeedBtn->setEnabled(false);
                    m_titanFeedBtnNode->setVisible(false);
                    
                    if (feedCD>=0) {
                        
                        
                        string timeInfo = CC_SECTOA((int)this->feedCD);
                        this->m_feedCDTxt->setString(timeInfo);
                        m_feedCDGoldTxt->setString(CC_CMDITOAL(CCCommonUtils::getGoldByTime((int)this->feedCD)));
                        
                    }
                    
                    
                }
                string timeInfo = CC_SECTOA((int)this->feedCD);
                this->m_feedCDTxt->setString(timeInfo);
                
                
            }
            else
            {
                this->feedCD=0;
                beginUpdate |= false;
                this->m_CleanFeedCDBtn->setEnabled(false);
                m_CleanFeedCDBtnNode->setVisible(false);
                
                m_titanFeedBtnNode->setVisible(true);
                
                string timeInfo = CC_SECTOA((int)this->feedCD);
                this->m_feedCDTxt->setString(timeInfo);
                
                
            }
        }
        //fusheng end 经验 and feedCD
        
        
        //fusheng being 钻石 or 粮食
        if(m_titanInfo.feedNum>=m_titanInfo.feedMaxNum)
        {
            m_titanFeedStatus_0->setString(_lang("500001"));
            m_titanFeedStatus_1->setString(CCString::createWithFormat("%3d",m_titanInfo.feedMaxNum)->getCString());
            m_titanFeedStatus_2->setString(CCString::createWithFormat("/%d",m_titanInfo.feedMaxNum)->getCString());
            
            m_titanFeedStatus_0->setColor(ccColor3B(195,206,254));
            
            m_titanFeedStatus_1->setVisible(true);
            m_titanFeedStatus_2->setVisible(true);
            //        m_titanFeedStatus_0->setPosition(138,-139);
            vector<cocos2d::CCLabelIF *> labels;
            labels.push_back(m_titanFeedStatus_0);
            labels.push_back(m_titanFeedStatus_1);
            labels.push_back(m_titanFeedStatus_2);
            arrangeLabel(labels);
            
            //        m_titanFeedTxt->setString("Accelerated growth");//fusheng 需要文本
            m_titanFeedTxt->setString(_lang("500002"));
            
            int gold = (m_titanInfo.feedNum - m_titanInfo.feedMaxNum)*50+CCCommonUtils::getGoldByTime(3600);
            gold = gold>1000? 1000:gold;
            this->m_needGlod->setString(CC_CMDITOAL(gold));
            
            m_needGoldNode->setVisible(true);
            m_needFoodNode->setVisible(false);
            
        }
        else
        {
            
            //fusheng begin 粮食
            this->m_needFood->setString(CC_CMDITOAL(m_titanInfo.feedFoodNum));
            
            if(m_titanInfo.feedNum<m_titanInfo.feedMaxNum)//fusheng 只有在小于免费喂食的情况下  才判断粮食的
            {
                if(params)
                {
                    
                    auto food = params->valueForKey("food");
                    if(food->length()!=0)
                    {
                        this->m_currentFoodNum->setString(CCString::createWithFormat("/%s",CC_CMDITOAL(food->longValue()).c_str())->getCString());
                        
                        if(m_titanInfo.feedFoodNum>food->longValue())
                        {
                            

                            isFoodEnough = false;
                            
                        }
                        else
                        {

                        }
                    }
                    else
                    {
                        //long lfood = GlobalData::shared()->resourceInfo.lFood;
                        
                        
                        
                        long lfood = GlobalData::shared()->resourceInfo.lFood;
                        if(m_titanInfo.feedFoodNum>lfood)
                        {
                            

                            
                            isFoodEnough = false;
                        }
                        else
                        {

                        }
                        
                        //            this->m_currentFoodNum->setString(CCString::createWithFormat("/%ld",lfood)->getCString());
                        this->m_currentFoodNum->setString(CCString::createWithFormat("/%s",CC_CMDITOAL(lfood).c_str())->getCString());
                    }
                }
                else
                {
                    long lfood = GlobalData::shared()->resourceInfo.lFood;
                    
                    
                    this->m_currentFoodNum->setString(CCString::createWithFormat("/%s",CC_CMDITOAL(lfood).c_str())->getCString());
                    
                    if(m_titanInfo.feedFoodNum>lfood)
                    {
                        

                        
                        isFoodEnough = false;
                    }
                    else
                    {

                    }
                }
            }
            //fusheng end 粮食
            
            
            
            
            m_needGoldNode->setVisible(false);
            m_needFoodNode->setVisible(true);
            m_titanFeedStatus_0->setString(_lang("500001"));
            m_titanFeedStatus_1->setString(CCString::createWithFormat("%3d",m_titanInfo.feedNum)->getCString());
            m_titanFeedStatus_2->setString(CCString::createWithFormat("/%d",m_titanInfo.feedMaxNum)->getCString());
            
            //        m_titanFeedStatus_0->setPosition(138,-139);
            m_titanFeedStatus_0->setColor(ccColor3B(195,206,254));
            
            m_titanFeedStatus_1->setVisible(true);
            m_titanFeedStatus_2->setVisible(true);
            
            vector<cocos2d::CCLabelIF *> labels;
            labels.push_back(m_titanFeedStatus_0);
            labels.push_back(m_titanFeedStatus_1);
            labels.push_back(m_titanFeedStatus_2);
            arrangeLabel(labels);
            
            
            m_titanFeedTxt->setString(_lang("500002"));//fusheng 需要文本
            
            if(GlobalData::shared()->titanInfo.level == 1)
                m_titanFeedTxt->setString(_lang("500011"));//fusheng 孵化
        }
        //fusheng being 钻石 or 粮食
        
        
        
        
        

        
        this->m_titanFeedBtn->setEnabled(isCanFeed);
        
        setGrayNode(m_titanFeedBtnNode,!isCanFeed);
        
        if(isCanFeed)
        {
            m_titanFeedTxt->setColor(ccColor3B(31,72,14));
            
            m_needFood->setColor(ccColor3B(167,104,3));
//            m_needFood->setColor(ccWHITE);
            
            m_needGlod->setColor(ccColor3B(41,95,128));
        }
        else
        {
            m_titanFeedTxt->setColor(ccGRAY);
            
            m_needFood->setColor(ccGRAY);
            
            m_needGlod->setColor(ccGRAY);
        }


    }
    


    
    

    
}


//
void GeneralTitanPopupView::loadResource(){
//    CCLoadSprite::doResourceByCommonIndex(105, true);
//    CCLoadSprite::doResourceByCommonIndex(100, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(3, true);
//    CCLoadSprite::doResourceByCommonIndex(503, true);
//    CCLoadSprite::doResourceByCommonIndex(7, true);
}
//

void GeneralTitanPopupView::setGuideFeed(bool guideFeed)
{
    isGuideFeed = guideFeed;
    
    if(isGuideFeed)
    {
        if (m_arrow) {
            m_arrow->setVisible(true);
        }
        
        
        auto seq = Sequence::createWithTwoActions( DelayTime::create(4),CCCallFunc::create([this](){
            if (this->m_arrow) {
                this->m_arrow->setVisible(false);
            }
            
        } ));
        seq->setTag(888888);
        this->runAction(seq);
    }
    else
    {
        if (m_arrow) {
            m_arrow->setVisible(false);
        }
    }

}
void GeneralTitanPopupView::onEnter(){
    PopupBaseView::onEnter();

    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
   
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralTitanPopupView::resetAttribute), MSG_TITAN_INFORMATION_RESET, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralTitanPopupView::handleTitanUpgrade), MSG_TITAN_UPGRADE_COMPLETE, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralTitanPopupView::handleTianUpgradeAnimationComplete), MSG_TITAN_UPGRADE_ANIMATION_COMLETE, NULL);
    
     CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralTitanPopupView::updateTitanInfo), MSG_REFREASH_BUILD_UPGRADE, NULL);
    
    
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD) //fusheng 在世界场景里 加载Imperial_22.plist
    {
        CCLoadSprite::doResourceByPathIndex(IMPERIAL_PATH,22, true);
    }
//
    onRefreshEquip();
    
    resetAttribute(nullptr);//fusheng onEnter时刷新数据
    
    
    if(m_titanInfo.exp>=m_titanInfo.nextExp)
    {
        FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
        
        if(fbiInfo.state != FUN_BUILD_UPING)
        {
            changeUpterms(true,bottomBraceState::BBSHOW);
        }
    }
    else
    {
        changeUpterms(true,bottomBraceState::BBCLOSE);
    }
    

}

void GeneralTitanPopupView::onExit(){

    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_INFORMATION_RESET);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_UPGRADE_COMPLETE);

    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_UPGRADE_ANIMATION_COMLETE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_BUILD_UPGRADE);
    

    

//    CCLoadSprite::doResourceByGeneralIndex(3, false);
    this->stopAllActionsByTag(888888);
    
    isGuideFeed = false;//fusheng 遗留代码
    
    if (m_arrow) {
        m_arrow->setVisible(false);
    }
    
    PopupBaseView::onExit();
}

void GeneralTitanPopupView::changeUpterms(bool isForce,bottomBraceState state)
{
   
    
    
    FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    
    
    if (isForce || fbiInfo.state != FUN_BUILD_UPING) {//强制改变和不处于升级时相应  显示 or 隐藏 m_UpTermsNode
       
        
        if (isForce) {
            
            if (state == bottomBraceState::BBSHOW) {

                isUptermsUnfold = true;
            }
            else if (state == bottomBraceState::BBCLOSE)
            {

                isUptermsUnfold = false;
            }
            else
            {
                isUptermsUnfold = !isUptermsUnfold;

            }
                
        }
        else
        {
            isUptermsUnfold = !isUptermsUnfold;
        }
        
        
        
        if (m_UpTermsNode) {
            
            if(isUptermsUnfold)
            {
                m_BottomBraceArrow->setFlippedY(false);
            }
            else
            {
                m_BottomBraceArrow->setFlippedY(true);
            }
            
            
            
            
            if(state == bottomBraceState::BBNONE)
            {
                
                float dt = 0.1;
                
                m_UpTermsNode->stopAllActions();
                
                auto act =Sequence::createWithTwoActions( ScaleTo::create(isUptermsUnfold?0:m_UpTermsNode->getScaleY()/1*dt,1,isUptermsUnfold?1:0),CCCallFunc::create([this]()
                                                                                                                                    
                {
                                                                                                                                                                      
                   
                    m_scrollView->setContentOffset( Vec2(m_scrollView->getContentOffset().x,m_scrollView->getContentOffset().y-1));
                
                     m_scrollView->setContentOffset( Vec2(m_scrollView->getContentOffset().x,m_scrollView->getContentOffset().y+1));//fusheng scrollView 属性不变 不显示
                
                
                }
                                                                                                                                                                      ));
                
                m_UpTermsNode->runAction(act);

                
                
            }
            else if(state == bottomBraceState::BBSHOW)
            {
                
                m_UpTermsNode->stopAllActions();
                m_UpTermsNode->setScaleY(1);
                
                
                m_scrollView->setContentOffset( Vec2(m_scrollView->getContentOffset().x,m_scrollView->getContentOffset().y-1));
                
                m_scrollView->setContentOffset( Vec2(m_scrollView->getContentOffset().x,m_scrollView->getContentOffset().y+1));//fusheng scrollView 属性不变 不显示
            }
            else if(state == bottomBraceState::BBCLOSE)
            {
                
                m_UpTermsNode->stopAllActions();
                
                m_UpTermsNode->setScaleY(0);
                
                
            }
        }
    }
}
void GeneralTitanPopupView::onCreateOrUpClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (!canUpgrade) {
        CCCommonUtils::flyHint("", "", _lang("500019"));
        changeUpterms(true,bottomBraceState::BBSHOW);
        
        return;
    }
    
    
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, _tmpTime);
    if(QID_MAX == qid)
    {
        bool st = true;
        if (m_qid != QID_MAX) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            if (qInfo.endTime!=0) {
                //可租用的队列
                int lastTime = qInfo.endTime - GlobalData::shared()->getWorldTime();
                if (lastTime < _tmpTime) {
                    lastTime = MAX(0, lastTime);
                    int needTime = _tmpTime - lastTime;
                    const int baseTime = FunBuildController::getInstance()->building_base_k8;
                    int needDay = ceil(needTime*1.0/86400);
                    int minDay = baseTime / 86400 ;
                    needDay = MAX(needDay, minDay);
                    int day = 0;
                    if(needDay%minDay!=0){
                        int tmp = floor(needDay*1.0/minDay);
                        day = tmp+1;
                        needDay = day * minDay;
                    }else{
                        day = needDay/minDay;
                    }
                    needTime = day * baseTime;
                    int needCost = day * FunBuildController::getInstance()->building_base_k9;
                    m_rentTime = needTime;
                    st = false;
                    std::string showTip = _lang_1("102297", CC_ITOA(needDay));
                    std::string t2 = "";
                    auto m_info = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
                    if(m_info.level > 0){
                        t2 = _lang_1("102502",_lang(m_info.name).c_str());
                    }else{
                        t2 = _lang_1("102503",_lang(m_info.name).c_str());
                    }
                    YesNoDialog::showQueueConfirm(showTip.c_str(),CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::onYesRent)),_lang("102501").c_str(),t2.c_str(),lastTime,_tmpTime,_lang("102174").c_str(),needCost);
                }
            }
        }
        
        //        qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
        //        int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
        //        YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(TitanUpgradeView::onGotoSpdView)), tmpTime, _lang("104903").c_str());
        
        if (st) {
            CCCommonUtils::flyHint("icon_time.png", "", _lang("102120"));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_CELL);
        }
    }
    else {

            auto m_info = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
            if (m_info.type == FUN_BUILD_MAIN && m_info.level==(GlobalData::shared()->shield_base-1)) {
                YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::onOkUp)));
            }
            else {
                onOkUp();
            }
    
    }
}

void GeneralTitanPopupView::onOkUp()
{
    if (_resGold>0) {
        string _dialog = "102312";//升级提示
        string _btnDialog = "102104";

        //        YesNoDialog::showButtonAndGold( _lang_1(_dialog, CC_ITOA(_resGold)) , CCCallFunc::create(this, callfunc_selector(TitanUpgradeView::onLastUp)), _lang("").c_str(),_resGold);
        PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(m_lfood,m_lwood,m_liron,m_lstone,0,_resGold
                                                                                   ,this,callfunc_selector(GeneralTitanPopupView::onLastUp)
                                                                                   , _lang(_dialog), _lang(_btnDialog)),false);
    }
    else {
        onLastUp();
    }
}
void GeneralTitanPopupView::onYesRent()
{
    QueueController::getInstance()->rentQueue(m_qid, m_rentTime);
}

void GeneralTitanPopupView::onLastUp()
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
//    for (int j = 1; j<willUnlockItems.size(); j++)
//    {
//        CCUserDefault::sharedUserDefault()->setStringForKey(willUnlockItems[j].c_str(),"1");
//    }
    CCUserDefault::sharedUserDefault()->flush();
    
    bool isForce = false;
    if (_resGold>0) {
        isForce = true;
    }

        auto m_info = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
        if (m_info.state == FUN_BUILD_NORMAL && FunBuildController::getInstance()->startUpFunBuild(FUN_BUILD_MAIN_CITY_ID, _resGold, isForce))
        {
//            m_isReturn = false;
            //            PopupViewController::getInstance()->removeAllPopupView();
            //            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            //            if(layer)
            //                layer->showBuildBtns(m_buildId);
            
//            if(isForce)//fusheng 如果是强制建立  退出所有弹出窗 //fusheng 不管
//                PopupViewController::getInstance()->removeAllPopupView();
//            else
//                PopupViewController::getInstance()->goBackPopupView();
        }
    
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_End"));
}

void GeneralTitanPopupView::onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    
    if (!canUpgrade) {
        CCCommonUtils::flyHint("", "", _lang("500019"));
        
        changeUpterms(true,bottomBraceState::BBSHOW);
        
        return;
    }
    
    auto m_info = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    if (m_info.type == FUN_BUILD_MAIN && m_info.level==(GlobalData::shared()->shield_base-1)) {
        YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::showWarningInstantUp)));
    }
    else {
        showWarningInstantUp();
    }
    
}
void GeneralTitanPopupView::showWarningInstantUp(){
    std::string name = "";
    

        auto m_info = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
        name = m_info.name;
    
    //    std::string showString = _lang_1("102498",_lang(name).c_str());
    std::string showString = _lang("500008");
    
    YesNoDialog::showButtonAndGold(showString.c_str(), CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::onOkInstantUp)),m_btnMsgLabel->getString().c_str(), _tmpGold);
}

void GeneralTitanPopupView::onOkInstantUp()
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
    
    for (int j = 1; j<willUnlockItems.size(); j++)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(willUnlockItems[j].c_str(),"1");
    }
    CCUserDefault::sharedUserDefault()->flush();
    

        auto m_info = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
        if (m_info.state == FUN_BUILD_NORMAL && FunBuildController::getInstance()->startUpFunBuild(FUN_BUILD_MAIN_CITY_ID,_tmpGold))
        {
            m_isReturn = false;
            PopupViewController::getInstance()->removeAllPopupView();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
            
            
            //fusheng 如果在世界里的话  更新主城数据
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD)
            {
                auto biter = GlobalData::shared()->imperialInfo.find(FUN_BUILD_MAIN_CITY_ID);
                
                if (biter!=GlobalData::shared()->imperialInfo.end()) {
                    
                    FunBuildController::getInstance()->completeUpOrCreate(FUN_BUILD_MAIN_CITY_ID,false,true);//fusheng 主城数据刷新
                    biter->second.state = FUN_BUILD_UPING_END;//fusheng  设置状态 保证正常逻辑
                }
            }
        }
    
}

void GeneralTitanPopupView::onShowNextUnlockItem()
{
    //    return;

    int m_buildId = FUN_BUILD_MAIN_CITY_ID;
    
    int itemId = m_buildId / 1000 ;
    

    string keys = "unlock_item";
    
    string msgInfo = CCCommonUtils::getPropById(CC_ITOA(itemId), keys);
    //    if (itemId == FUN_BUILD_SCIENE) {
    //        msgInfo = "1,700100,700200,730000|2,710000,711400,711500,730100,730200,730300,730400|3,720000|4,720100,720200,730500|5,700300,730600,730700,730800,730900|6,720300,720400,731000|7,700400,700500,710300|8,731100,731200,731300,731400|9,731500|10,700600,710500,720500,720600,720700|11,720800,731600|12,700700,710600|13,731700,731800,731900,732000|14,710400,732100|15,700800,711600,711700,720900|16,700900,701000,721000,732200,732300,732400,732500|17,732600|18,701200,701300,710700,732700,732800,732900,733000|19,721100,721200,733100|20,711000,733200|21,701400|22,701500,701600,711200,711300,721400,733300,733400,733500,733600|24,701700,733700|25,701800,721500,721600,721700,733800,733900,734000,734100|26,701900,711100|27,702000,702100,721800,734200|28,734300,734400,734500,734600|30,734700";
    //    }
    //    else if (itemId == FUN_BUILD_BARRACK1) {
    //        msgInfo = "4,107001|7,107002|10,107003|13,107004|16,107005|19,107006|22,107007|26,107008|30,107009";
    //    }
    //    else if (itemId == FUN_BUILD_BARRACK2) {
    //        msgInfo = "4,107101|7,107102|10,107103|13,107104|16,107105|19,107106|22,107107|26,107108|30,107109";
    //    }
    //    else if (itemId == FUN_BUILD_BARRACK3) {
    //        msgInfo = "4,107201|7,107202|10,107203|13,107204|16,107205|19,107206|22,107207|26,107208|30,107209";
    //    }
    //    else if (itemId == FUN_BUILD_BARRACK4) {
    //        msgInfo = "4,107301|7,107302|10,107303|13,107304|16,107305|19,107306|22,107307|26,107308|30,107309";
    //    }
    //    else if (itemId == FUN_BUILD_FORT) {
    //        msgInfo = "2,107910|3,107920|6,107901|8,107911|10,107921|12,107902|14,107912|16,107922|18,107903|20,107913|22,107923|24,107904|27,107914|30,107924";
    //    }
    //    else if (itemId == FUN_BUILD_MAIN) {
    //        msgInfo = "2,410000|3,425000|4,416000,403000,411000|5,426000,429000|6,401000,402000|7,407000|8,428000|10,414000|15,412000";
    //    }
    
    if (msgInfo=="") {
        return;
    }
    
    int nextLv = 1;

        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        nextLv = m_info.level+1;
    
    vector<string> msgItemVec;
    vector<string> finalVec;
    vector<string> nextVec;
    CCCommonUtils::splitString(msgInfo, "|", msgItemVec);
    for (int i=0; i<msgItemVec.size(); i++)
    {
        finalVec.clear();
        CCCommonUtils::splitString(msgItemVec[i], ",", finalVec);
        if (finalVec.size()>0 && finalVec[0] == CC_ITOA(nextLv)) {
            break;
        }
        
        if (atoi(finalVec[0].c_str()) > nextLv)
        {
            if (nextVec.size() == 0) {
                nextVec = finalVec;
            }
            else {
                if (atoi(nextVec[0].c_str()) > atoi(finalVec[0].c_str())) {
                    nextVec = finalVec;
                }
            }
        }
        
        if (i == msgItemVec.size()-1) {
            finalVec.clear();
        }
    }
    
    if (finalVec.size()<=0) {
        finalVec = nextVec;

    }else {
        willUnlockItems.clear();
        willUnlockItems = finalVec;
    }
    
    if (finalVec.size()<=0) {
        return;
    }
    
}


SEL_CCControlHandler GeneralTitanPopupView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSkillBtnClick", GeneralsPopupView::onSkillBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllianceClick", GeneralsPopupView::onAllianceClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTitanFeedClick", GeneralTitanPopupView::onTitanFeedClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCleanFeedCDClick", GeneralTitanPopupView::onCleanFeedCDClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSpeedUpClick", GeneralTitanPopupView::onSpeedUpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onToolSpeedUpClick", GeneralTitanPopupView::onToolSpeedUpClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", GeneralTitanPopupView::onCreateOrUpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstantClick", GeneralTitanPopupView::onInstantClick);

    return NULL;
}
bool GeneralTitanPopupView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAttackTxt", CCLabelIF*, this->m_titanAttackTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanMagicTxt", CCLabelIF*, this->m_titanMagicTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Txt1", CCLabelIF*, this->m_Txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Txt2", CCLabelIF*, this->m_Txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanExtTxt_0", CCLabelIF*, this->m_titanExtTxt_0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanExtTxt_1", CCLabelIF*, this->m_titanExtTxt_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanExtTxt_2", CCLabelIF*, this->m_titanExtTxt_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_feedCDTxt", CCLabelIF*, this->m_feedCDTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upgradeCDTxt", CCLabelIF*, this->m_upgradeCDTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolSpeedUpTxt", CCLabelIF*, this->m_toolSpeedUpTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonTip", CCLabelIF*, this->m_dragonTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanUpingTip", CCLabelIF*, this->m_titanUpingTip);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedStatus_0", CCLabelIF*, this->m_titanFeedStatus_0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedStatus_1", CCLabelIF*, this->m_titanFeedStatus_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedStatus_2", CCLabelIF*, this->m_titanFeedStatus_2);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanUpgradePro", CCScale9Sprite*, this->m_titanUpgradePro);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_UpTermsBG", CCScale9Sprite*, this->m_UpTermsBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BottomBraceBG", CCScale9Sprite*, this->m_BottomBraceBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_UpTermsNode", CCNode*, this->m_UpTermsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BottomBraceArrow", CCSprite*, this->m_BottomBraceArrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BottomBraceBGNode", CCNode*, this->m_BottomBraceBGNode);

    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedTxt", CCLabelIF*, this->m_titanFeedTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_feedCDBtnTxt", CCLabelIF*, this->m_feedCDBtnTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, this->m_instantBtn);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabelPre", CCLabelIF*, this->m_lvLabelPre);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLvLabelPre", CCLabelIF*, this->m_nextLvLabelPre);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLvLabel", CCLabelIF*, this->m_nextLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanCurrentZDLInNext", CCLabelIF*, this->m_titanCurrentZDLInNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanZDLAddValue", CCLabelIF*, this->m_titanZDLAddValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, this->m_inBtnGoldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needFood", CCLabelIF*, this->m_needFood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_currentFoodNum", CCLabelIF*, this->m_currentFoodNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedBtn", CCControlButton*, this->m_titanFeedBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedUpBtn", CCControlButton*, this->m_speedUpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_CleanFeedCDBtn", CCControlButton*, this->m_CleanFeedCDBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_feedCDGoldTxt", CCLabelIF*, this->m_feedCDGoldTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanExtTxtPre", CCLabelIF*, this->m_titanExtTxtPre);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolSpeedUpBtn", CCControlButton*, this->m_toolSpeedUpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodStatus", CCSprite*, this->m_foodStatus);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanDetail", CCSprite*, this->m_titanDetail);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanSkill", CCSprite*, this->m_titanSkill);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanUngrade", CCSprite*, this->m_titanUngrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedBtnSprite", CCSprite*, this->m_titanFeedBtnSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeSpr1", CCSprite*, this->m_timeSpr1);
    
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bustPic", CCNode*, this->m_bustPic);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchLayer", CCNode*, this->m_touchLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolSpeedUpBtnNode", CCNode*, this->m_toolSpeedUpBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedUpBtnNode", CCNode*, this->m_speedUpBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList_TR", CCNode*, this->m_infoList_TR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_DashedTR", CCNode*, this->m_DashedTR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_DashedBL", CCNode*, this->m_DashedBL);

    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedBtnNode", CCNode*, this->m_titanFeedBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_CleanFeedCDBtnNode", CCNode*, this->m_CleanFeedCDBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "titanFeedNode", CCNode*, this->titanFeedNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "titanUpingNode", CCNode*, this->titanUpingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedCDNode", CCNode*, this->m_titanFeedCDNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanStatusDesc", CCLabelIF*, this->m_titanStatusDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needGlod", CCLabelIF*, this->m_needGlod);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needFoodNode", CCNode*, this->m_needFoodNode);
    
    
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "nb_arrow", CCSprite*, this->nb_arrow);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bNode", CCNode*, this->m_bNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", Label*, this->m_nameTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pointText", CCLabelIF*, this->m_pointText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expTxt", CCLabelIF*, this->m_expTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expBar", CCScale9Sprite*, this->m_expBar);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBg", CCSprite*, this->m_leftBg);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBg", CCSprite*, this->m_rightBg);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_renameBtn", CCControlButton*, this->m_renameBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt3", CCLabelIF*, this->m_bTxt3);
////    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt4", CCLabelIF*, this->m_bTxt4);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt5", CCLabelIF*, this->m_bTxt5);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt6", CCLabelIF*, this->m_bTxt6);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode1", CCNode*, this->m_clickNode1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode2", CCNode*, this->m_clickNode2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode3", CCNode*, this->m_clickNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode4", CCNode*, this->m_clickNode4);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode5", CCNode*, this->m_clickNode5);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode6", CCNode*, this->m_clickNode6);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode7", CCNode*, this->m_clickNode7);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headIcon", CCNode*, this->m_headIcon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTouch", CCNode*, this->m_nameTouch);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollContainer", CCNode*, this->m_scrollContainer);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnText", CCLabelIF*, this->m_btnText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, this->m_goldNum);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resetPicBtn", CCControlButton*, this->m_resetPicBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldIcon", CCSprite*, this->m_goldIcon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resetNode", CCNode*, this->m_resetNode);
////    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picCover", CCScale9Sprite*, this->m_picCover);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_staminePro", CCScale9Sprite*, this->m_staminePro);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stamineBG", CCScale9Sprite*, this->m_stamineBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stamineText", CCLabelIF*, this->m_stamineText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stamineIcon", CCSprite*, this->m_stamineIcon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
//    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needGoldNode", CCNode*, this->m_needGoldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanExtNode", CCNode*, this->m_titanExtNode);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ProTitanExt", CCScale9Sprite*, this->m_ProTitanExt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ProTiTanAP", CCScale9Sprite*, this->m_ProTiTanAP);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanPosInView", CCNode*, this->m_titanPosInView);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxt_0", CCLabelIF*, this->m_titanAPTxt_0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxt_1", CCLabelIF*, this->m_titanAPTxt_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxt_2", CCLabelIF*, this->m_titanAPTxt_2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxtPre", CCLabelIF*, this->m_titanAPTxtPre);
    
////    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillBtn", CCControlButton*, this->m_skillBtn);
//    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode1", CCNode*, this->m_equipNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode2", CCNode*, this->m_equipNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode3", CCNode*, this->m_equipNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode4", CCNode*, this->m_equipNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode5", CCNode*, this->m_equipNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode6", CCNode*, this->m_equipNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode7", CCNode*, this->m_equipNode7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode8", CCNode*, this->m_equipNode8);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr1", CCSprite*, this->m_eBgSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr2", CCSprite*, this->m_eBgSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr3", CCSprite*, this->m_eBgSpr3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr4", CCSprite*, this->m_eBgSpr4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr5", CCSprite*, this->m_eBgSpr5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr6", CCSprite*, this->m_eBgSpr6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr7", CCSprite*, this->m_eBgSpr7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr8", CCSprite*, this->m_eBgSpr8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle7", CCNode*, this->m_particle7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle8", CCNode*, this->m_particle8);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red1Pt", CCSprite*, this->m_red1Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red2Pt", CCSprite*, this->m_red2Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red3Pt", CCSprite*, this->m_red3Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red4Pt", CCSprite*, this->m_red4Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red5Pt", CCSprite*, this->m_red5Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red6Pt", CCSprite*, this->m_red6Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red7Pt", CCSprite*, this->m_red7Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red8Pt", CCSprite*, this->m_red8Pt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel1", CCLabelIF*, this->m_equipLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel2", CCLabelIF*, this->m_equipLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel3", CCLabelIF*, this->m_equipLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel4", CCLabelIF*, this->m_equipLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel5", CCLabelIF*, this->m_equipLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel6", CCLabelIF*, this->m_equipLabel6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel7", CCLabelIF*, this->m_equipLabel7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel8", CCLabelIF*, this->m_equipLabel8);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg1", CCScale9Sprite*, this->m_eLvBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg2", CCScale9Sprite*, this->m_eLvBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg3", CCScale9Sprite*, this->m_eLvBg3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg4", CCScale9Sprite*, this->m_eLvBg4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg5", CCScale9Sprite*, this->m_eLvBg5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg6", CCScale9Sprite*, this->m_eLvBg6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg7", CCScale9Sprite*, this->m_eLvBg7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg8", CCScale9Sprite*, this->m_eLvBg8);
    
    
//
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillNumNode", CCNode*, this->m_skillNumNode);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagTipNode", CCNode*, this->m_bagTipNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagTipNum", CCLabelIF*, this->m_bagTipNum);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selfInfoNode", CCNode*, this->m_selfInfoNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_otherNode", CCNode*, this->m_otherNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalNode", CCNode*, this->m_medalNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expOthBar", CCScale9Sprite*, this->m_expOthBar);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expOthTxt", CCLabelIF*, this->m_expOthTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvOthTxt", CCLabelIF*, this->m_lvOthTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalLable", CCLabelIF*, this->m_medalLable);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgBtn", CCControlButton*, this->m_msgBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnAlliance", CCControlButton*, this->m_btnAlliance);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noMedalLable", CCLabelIF*, this->m_noMedalLable);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalParticleNode", CCNode*, this->m_medalParticleNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, this->m_starNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speStarNode", CCNode*, this->m_speStarNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speStarTTF", CCLabelIF*, this->m_speStarTTF);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipTipNode", CCNode*, this->m_equipTipNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipTipLabel", CCLabelIF*, this->m_equipTipLabel);
//    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_receiveGlow", CCNode*, this->m_receiveGlow);
    
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
//
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockL", CCSprite*, this->m_lockL);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockR", CCSprite*, this->m_lockR);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button", CCScale9Sprite*, this->m_button);
    
    return false;
}
void GeneralTitanPopupView::update(float time){
//    int currt = WorldController::getInstance()->currentStamine;
//    int total = GlobalData::shared()->worldConfig.stamineMax;
//    m_stamineText->setString(std::string("") + CC_ITOA(currt) + "/" + CC_ITOA(total));
//    currt = MIN(currt, total);
//    float s = 1.0 * currt / total;
//    m_staminePro->setScaleX(s);
//    m_tip->update(time);
    
    TitanInView* view = dynamic_cast<TitanInView*>(  m_titanPosInView->getChildByTag(10086));
    
    if (view) {
        view->m_Titan->update(time);
    }
    
    if(!beginUpdate)//fusheng 有时间改变
    {
        return;
    }
    
    if (isUpgrading)//fusheng 计算升级时间
    {
        if (this->upgradeCD>0) {
            
            this->perTimeForUpgradeCD += time;
            
            if (this->perTimeForUpgradeCD > 1) {
                
                this->perTimeForUpgradeCD -= 1;
                
                this->calUpgradeCD();
                
            }
            
           
        }
        else
        {
            
            resetAttribute(nullptr);
        }
    }
    else
    {
        if (this->feedCD>0) { //fusheng 计算喂食CD时间
            
            
            
            this->perTimeForFeedCD += time;
            
            if (this->perTimeForFeedCD > 1) {
                
                this->perTimeForFeedCD -= 1;
                
                this->calFeedCD();
                
            }

            
        }
        else
        {
            
            resetAttribute(nullptr);
        }
    }
    
    
    
    return;
}
//
void GeneralTitanPopupView::onSkillClick(CCObject * pSender, Control::EventType pCCControlEvent){
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(GeneralTitanPopupView::animationFunc));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
    
//    CCNode* pic = m_bustPic->getChildByTag(0);
//    if (pic) {
//        CCSprite* icon = dynamic_cast<CCSprite*>(pic);
//        if (icon) {
//            icon->runAction(CCFadeOut::create(0.2));
//        }
//    }
    
//    m_pic->runAction(CCFadeOut::create(0.2f));
}
void GeneralTitanPopupView::onTitanFeedClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(this->feedCD<=0)
    {
        if(m_titanInfo.exp>=m_titanInfo.nextExp)
        {
            CCCommonUtils::flyHint("", "", "please ungrade titan!");
            return;
        }
        
        if(!isFoodEnough)
        {
            
            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(WorldResourceType::Food));
            return;
        }
        if(m_titanInfo.feedNum<m_titanInfo.feedMaxNum)
        {
            TitanInView* view = dynamic_cast<TitanInView*>(  m_titanPosInView->getChildByTag(10086));
            
            if (view && GlobalData::shared()->titanInfo.level != 1) {
                view->m_Titan->changeTitanState(Titan::eActState::Cheer);
            }


            TitanFeedCommand *tfCommand = new TitanFeedCommand();
            tfCommand->sendAndRelease();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Feed"));
            m_titanFeedBtn->setEnabled(false);
            
        }
        else
        {
            
            int gold = (m_titanInfo.feedNum - m_titanInfo.feedMaxNum)*50+CCCommonUtils::getGoldByTime(3600);
            gold = gold>1000? 1000:gold;
            YesNoDialog::showButtonAndGold(_lang("500018").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::AccGrowthCallBack)), "OK", gold);

        }

    }
//    else
//    {
//        
//        YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::speedUpCallBack)), (int)this->feedCD, _lang("104903").c_str());
//    }
    
    
}
void GeneralTitanPopupView::AccGrowthCallBack()
{
    
    if(m_titanInfo.feedNum>=m_titanInfo.feedMaxNum)
    {
        
        int gold = (m_titanInfo.feedNum - m_titanInfo.feedMaxNum)*50+CCCommonUtils::getGoldByTime(3600);
        gold = gold>1000? 1000:gold;
    
        if (gold>0) {
            //判断金币数量是否足够
            if (GlobalData::shared()->playerInfo.gold < gold) {
                YesNoDialog::gotoPayTips();
                return ;
            }
        }
    }

    
    
    TitanInView* view = dynamic_cast<TitanInView*>(  m_titanPosInView->getChildByTag(10086));
    
    if (view && GlobalData::shared()->titanInfo.level != 1) {
        view->m_Titan->changeTitanState(Titan::eActState::Cheer);
    }
    TitanFeedCommand *tfCommand = new TitanFeedCommand();
    tfCommand->sendAndRelease();
    m_titanFeedBtn->setEnabled(false);
}

void GeneralTitanPopupView::onSpeedUpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    if(fbiInfo.state == FUN_BUILD_UPING && fbiInfo.level==1)//fusheng 1级且升级中
    {
        GeneralTitanPopupView::spdCallBack();
        return;
        
    }
    int tmpTime = GlobalData::shared()->allQueuesInfo[queue_id].finishTime - GlobalData::shared()->getWorldTime();
    
    YesNoDialog::showTime( _lang("500008").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::spdCallBack)), tmpTime, _lang("104903").c_str());
    

//
////    int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
//    YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::speedUpCallBack)), (int)this->feedCD, _lang("104903").c_str());
//    
    
}

void GeneralTitanPopupView::spdCallBack()
{
    int tmpTime = GlobalData::shared()->allQueuesInfo[queue_id].finishTime - GlobalData::shared()->getWorldTime();
    int gold = CCCommonUtils::getGoldByTime(tmpTime);
    if (tmpTime<=GlobalData::shared()->freeSpdT) {
        gold = 0;
    }
    FunBuildController::getInstance()->costCD(FUN_BUILD_MAIN_CITY_ID, "", gold);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Speed"));
    
};

void GeneralTitanPopupView::onCleanFeedCDClick(CCObject * pSender, Control::EventType pCCControlEvent){
    
    
    YesNoDialog::showTime( _lang("500004").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::speedUpCallBack)), (int)this->feedCD, _lang("104903").c_str(),false);
    
    
}

void GeneralTitanPopupView::onToolSpeedUpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    //
            FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID); //fusheng 泰坦道具加速
    
            if(fbiInfo.state == FUN_BUILD_UPING)
            {
               int toolID = ToolController::getInstance()->getSPDItem(ItemSpdMenu_City);
    
                auto& toolInfo = ToolController::getInstance()->getToolInfoById(toolID);
    
                if(toolInfo.getCNT()>0)
                {
                    if (queue_id != -1) {
                        PropSpeedupView::show(ItemSpdMenu_City, FUN_BUILD_MAIN_CITY_ID, queue_id);//fusheng 注意 不是只有1101  还有1102
                    }
                    
                }
            }
    

//    YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::speedUpCallBack)), (int)this->feedCD, _lang("104903").c_str());
    
    
}
void GeneralTitanPopupView::speedUpCallBack()
{
    
    int qid = CCCommonUtils::getQueueSortByType(15) *1000 + (15 + 1)*100+1;
    
    int gold = CCCommonUtils::getGoldByTime((int)this->feedCD);
    if( gold <= GlobalData::shared()->playerInfo.gold ){
        QueueController::getInstance()->startCCDQueue(qid, "", false, gold,"",1);
        //this->removeFromParent();
    }
    else {
        YesNoDialog::gotoPayTips();
    }
}


void GeneralTitanPopupView::animationFunc(){
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    PopupViewController::getInstance()->addPopupInView(GeneralSkillListPopUpView::create(&GlobalData::shared()->generals.begin()->second,"50000"));
}
//
//void GeneralsPopupView::animationFunc1(){
//    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
//    PopupViewController::getInstance()->goBackPopupView();
//}
//
bool GeneralTitanPopupView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
//    m_TouchMoved = false;
//    m_TouchPoint = pTouch->getLocation();
//    if(isTouchInside(m_touchNode, pTouch) && isTouchInside(m_infoNode, pTouch)){
//        if ((m_playerUid=="" || m_playerUid==GlobalData::shared()->playerInfo.uid)) {
//            this->m_tip->setVisible(true);
//            if (CCCommonUtils::isIosAndroidPad()) {
//                this->m_tip->setPosition(ccp(-350, -250));
//            }
//            else
//                this->m_tip->setPosition(ccp(-150, -350));
//        }
//    }
//    if (m_info) {
//        if(isTouchInside(m_eBgSpr1, pTouch)){
//            onShowEquipTip(0);
//        }else if(isTouchInside(m_eBgSpr2, pTouch)){
//            onShowEquipTip(1);
//        }else if(isTouchInside(m_eBgSpr3, pTouch)){
//            onShowEquipTip(2);
//        }else if(isTouchInside(m_eBgSpr4, pTouch)){
//            onShowEquipTip(3);
//        }else if(isTouchInside(m_eBgSpr5, pTouch)){
//            onShowEquipTip(4);
//        }else if(isTouchInside(m_eBgSpr6, pTouch)){
//            onShowEquipTip(5);
//        }else if(isTouchInside(m_eBgSpr7, pTouch)){
//            onShowEquipTip(6);
//        }else if(isTouchInside(m_eBgSpr8, pTouch)){
//            onShowEquipTip(7);
//        }
//    }
//    m_x = 1000;
//    m_startY = pTouch->getLocation().y;
//    return true;
//            if(isTouchInside(m_eBgSpr1, pTouch)){//fusheng 不提示装备未开放
//                CCCommonUtils::flyHint("", "", _lang("E100008"));
//            }else if(isTouchInside(m_eBgSpr2, pTouch)){
//                CCCommonUtils::flyHint("", "", _lang("E100008"));
//            }else if(isTouchInside(m_eBgSpr3, pTouch)){
//                CCCommonUtils::flyHint("", "", _lang("E100008"));
//            }else if(isTouchInside(m_eBgSpr4, pTouch)){
//                CCCommonUtils::flyHint("", "", _lang("E100008"));
//            }else if(isTouchInside(m_eBgSpr5, pTouch)){
//                CCCommonUtils::flyHint("", "", _lang("E100008"));
//            }else if(isTouchInside(m_eBgSpr6, pTouch)){
//                CCCommonUtils::flyHint("", "", _lang("E100008"));
//            }else if(isTouchInside(m_eBgSpr7, pTouch)){
//                CCCommonUtils::flyHint("", "", _lang("E100008"));
//            }else if(isTouchInside(m_eBgSpr8, pTouch)){
//                CCCommonUtils::flyHint("", "", _lang("E100008"));
//            }
    
//    if(isTouchInside(m_titanFeed, pTouch))
//    {
//        return true;
//    }
    if(isTouchInside(this->m_titanUngrade, pTouch))
    {
        if (m_titanId == "") {
            return false;
        }
        

        
        FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);//fusheng 使用建筑物状态
        
        if(fbiInfo.state == FUN_BUILD_UPING)
        {
        
            CCCommonUtils::flyHint("", "", _lang("500007"));
            return true;
        }
        
        PopupViewController::getInstance()->addPopupInView(TitanUpgradeView::create(FUN_BUILD_MAIN_CITY_ID,CCString::create(m_titanId)->intValue()));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Up"));
        return true;
    }
    if(isTouchInside(this->m_clickNode4, pTouch))
    {
        if (m_titanId == "") {
            return false;
        }

        
        
//        QueueInfo &qinfo =  GlobalData::shared()->allQueuesInfo[1101];
//        
//        if(qinfo.key == "400000000" && qinfo.finishTime > GlobalData::shared()->getWorldTime())
//        {
//            CCCommonUtils::flyHint("", "", "Titan is upgrading");
//            return true;
//        }
//        
//        PopupViewController::getInstance()->addPopupInView(TitanUpgradeView::create(400000000,CCString::create(m_titanId)->intValue()));
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Up"));
        return true;
    }
    
    
    if(isTouchInside(this->m_titanSkill, pTouch)){

        
        
        CCCommonUtils::flyHint("", "", _lang("E100008"));//fusheng 技能界面先关闭

//        
//        
//                onSkillClick(NULL, CCControlEvent::TOUCH_DOWN);
//        
//        CCArray* p = CCArray::create();
//        p->addObject(CCInteger::create(GENERAL_OPEN));
//        CCArray* d = CCArray::create();
//        
//        DataRecordCommand* cmd = new DataRecordCommand(OPEN_PANEL, p, d);
//        cmd->sendAndRelease();

        return true;

        
    }
    if(isTouchInside(this->m_titanDetail, pTouch))
    {
//        CCCommonUtils::flyHint("", "", _lang("E100008"));
        PopupViewController::getInstance()->addPopupInView(MainCityView::create(FUN_BUILD_MAIN_CITY_ID));
        return true;

    }
    
    if(isTouchInside(this->m_BottomBraceBGNode, pTouch))
    {
        changeUpterms();
        return true;
        
    }
    
    return true;
    
}

void GeneralTitanPopupView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
//    m_TouchMoved = true;
//    Vec2 oldPoint = m_TouchPoint;
//    m_TouchPoint = pTouch->getLocation();
//    float dtx = m_TouchPoint.x - oldPoint.x;
//    CCLOG("Titan touch dtx:%f",dtx);
//    m_Titan->onRotationY(dtx);
//    return;
}

void GeneralTitanPopupView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GeneralTitanPopupView::calFeedCD(float ChangeTime )
{
    feedCD += ChangeTime;
    
    if (feedCD>=0) {
        
        
        string timeInfo = CC_SECTOA((int)this->feedCD);
        this->m_feedCDTxt->setString(timeInfo);
        m_feedCDGoldTxt->setString(CC_CMDITOAL(CCCommonUtils::getGoldByTime((int)this->feedCD)));
       
    }
//    else
//    {
//        resetAttribute(nullptr);
//    }
}

void GeneralTitanPopupView::calUpgradeCD(float ChangeTime )
{
    upgradeCD += ChangeTime;
    
    if (upgradeCD>=0) {
       
        string timeInfo = CC_SECTOA((int)this->upgradeCD);
        this->m_upgradeCDTxt->setString(timeInfo);
        
        
        //        if(upgradeCDTotal!=0&&upgradeCDTotal>0) //fusheng 进度条
        //        {
        //            float ratio = (upgradeCDTotal-upgradeCD)/upgradeCDTotal;
        //            nbpb->setPercent(ratio);
        //        }
        //        else
        //        {
        //            nbpb->setPercent(1);
        //        }

    }
//    else
//    {
//        resetAttribute(nullptr);
//    }
}


//void GeneralsPopupView::resetRankPop(CCObject *obj){
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_VIEW_IN);
//    m_addPopup = false;
//}
void GeneralTitanPopupView::onRefreshEquip()
{
    CCLoadSprite::doResourceByCommonIndex(100, true);
    m_equipNode1->removeAllChildren();
    m_equipNode2->removeAllChildren();
    m_equipNode3->removeAllChildren();
    m_equipNode4->removeAllChildren();
    m_equipNode5->removeAllChildren();
    m_equipNode6->removeAllChildren();
    m_equipNode7->removeAllChildren();
    m_equipNode8->removeAllChildren();
    m_EquipMap.clear();
    
    m_red1Pt->setVisible(false);
    m_red2Pt->setVisible(false);
    m_red3Pt->setVisible(false);
    m_red4Pt->setVisible(false);
    m_red5Pt->setVisible(false);
    m_red6Pt->setVisible(false);
    m_red7Pt->setVisible(false);
    m_red8Pt->setVisible(false);
    
    
    m_eLvBg1->setVisible(false);
    m_eLvBg2->setVisible(false);
    m_eLvBg3->setVisible(false);
    m_eLvBg4->setVisible(false);
    m_eLvBg5->setVisible(false);
    m_eLvBg6->setVisible(false);
    m_eLvBg7->setVisible(false);
    m_eLvBg8->setVisible(false);
    
    
    
    m_equipLabel1->setString("");
    m_equipLabel2->setString("");
    m_equipLabel3->setString("");
    m_equipLabel4->setString("");
    m_equipLabel5->setString("");
    m_equipLabel6->setString("");
    m_equipLabel7->setString("");
    m_equipLabel8->setString("");
    

    
    return;//fusheng
    
    //打开屏蔽装备
//    if(m_info&&(!m_info->showEquip)){
    if(true){
        for (int i=0; i<=5; i++) {
            string siteIcon = "";
            if (i==0) {
                siteIcon = "icon_weapons_close.png";
            }else if (i==1) {
                siteIcon = "icon_peeping_close.png";
            }else if (i==2) {
                siteIcon = "icon_clothes_close.png";
            }else if (i==3) {
                siteIcon = "icon_trousers_close.png";
            }else if (i==4) {
                siteIcon = "icon_shoes_close.png";
            }else if (i==5) {
                siteIcon = "icon_ring_close.png";
            }else if (i==6) {
                siteIcon = "icon_long_close.png";
            }
//            else if (i==7) {
//                siteIcon = "icon_long_close.png";
//            }
            auto icon = CCLoadSprite::createSprite(siteIcon.c_str(), true,CCLoadSpriteType_DEFAULT);
            
            if (i==0) {
                m_equipNode1->addChild(icon);
                m_red1Pt->setVisible(false);
                m_eLvBg1->setVisible(false);
            }else if (i==1) {
                m_equipNode2->addChild(icon);
                m_red2Pt->setVisible(false);
                m_eLvBg2->setVisible(false);
            }else if (i==2) {
                m_equipNode3->addChild(icon);
                m_red3Pt->setVisible(false);
                m_eLvBg3->setVisible(false);
            }else if (i==3) {
                m_equipNode4->addChild(icon);
                m_red4Pt->setVisible(false);
                m_eLvBg4->setVisible(false);
            }else if (i==4) {
                m_equipNode5->addChild(icon);
                m_red5Pt->setVisible(false);
                m_eLvBg5->setVisible(false);
            }else if (i==5) {
                m_equipNode6->addChild(icon);
                m_red6Pt->setVisible(false);
                m_eLvBg6->setVisible(false);
            }else if (i==6) {
//                if (isOpenLongJing) {
//                    m_equipNode7->addChild(icon);
//                    m_red7Pt->setVisible(false);
//                    m_eLvBg7->setVisible(false);
//                }
            }
//            else if (i==7) {
//                m_equipNode8->addChild(icon);
//                m_red8Pt->setVisible(false);
//                m_eLvBg8->setVisible(false);
//            }
        }
        return;
    }//打开屏蔽装备
    
    vector<int> tmpEquip;
    if (m_info) {
        tmpEquip = m_info->m_equip;
    }else {
        map<string, int>::iterator it = EquipmentController::getInstance()->m_MyOnEquip.begin();
        for (; it != EquipmentController::getInstance()->m_MyOnEquip.end(); it++) {
            auto& info = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            tmpEquip.push_back(it->second);
            m_EquipMap[info.site] = it->first;
        }
    }
    
    map<int, int> tmpMap;
    for (int j=0; j<tmpEquip.size() ; j++) {
        auto& info = EquipmentController::getInstance()->EquipmentInfoMap[tmpEquip[j]];
        int site = info.site;
        m_EquipSiteMap[site] = tmpEquip[j];
        
        string bgPath = CCCommonUtils::getToolBgByColor(info.color);
        auto bg = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(bg, 90, true);
        
        string bgKuangStr="";
        if (info.color==WHITE) {
            bgKuangStr="icon_kuang_bai.png";
        }else if (info.color==GREEN){
            bgKuangStr="icon_kuang_lv.png";
        }else if (info.color==BLUE){
            bgKuangStr="icon_kuang_lan.png";
        }else if (info.color==PURPLE){
            bgKuangStr="icon_kuang_zi.png";
        }else if (info.color==ORANGE){
            bgKuangStr="icon_kuang_cheng.png";
        }else if (info.color==GOLDEN){
            bgKuangStr="icon_kuang_jin.png";
        }
        CCSprite *bgKuang=NULL;
        if (bgKuangStr!="") {
            bgKuang = CCLoadSprite::createSprite(bgKuangStr.c_str());
        }
        
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(tmpEquip[j]));
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        
        string lvMsg = "Lv.";
        lvMsg = lvMsg + CC_ITOA(info.level);
        tmpMap[site] = 1;
        if (site==0) {
            m_equipNode1->addChild(bg);
            m_equipNode1->addChild(icon);
            m_equipNode1->addChild(bgKuang);
            m_equipLabel1->setString(lvMsg.c_str());
        }else if (site==1) {
            m_equipNode2->addChild(bg);
            m_equipNode2->addChild(icon);
            m_equipNode2->addChild(bgKuang);
            m_equipLabel2->setString(lvMsg.c_str());
        }else if (site==2) {
            m_equipNode3->addChild(bg);
            m_equipNode3->addChild(icon);
            m_equipNode3->addChild(bgKuang);
            m_equipLabel3->setString(lvMsg.c_str());
        }else if (site==3) {
            m_equipNode4->addChild(bg);
            m_equipNode4->addChild(icon);
            m_equipNode4->addChild(bgKuang);
            m_equipLabel4->setString(lvMsg.c_str());
        }else if (site==4) {
            m_equipNode5->addChild(bg);
            m_equipNode5->addChild(icon);
            m_equipNode5->addChild(bgKuang);
            m_equipLabel5->setString(lvMsg.c_str());
        }else if (site==5) {
            m_equipNode6->addChild(bg);
            m_equipNode6->addChild(icon);
            m_equipNode6->addChild(bgKuang);
            m_equipLabel6->setString(lvMsg.c_str());
        }else if (site==6) {            
            m_equipNode7->addChild(bg);
            m_equipNode7->addChild(icon);
            m_equipNode7->addChild(bgKuang);
            m_equipLabel7->setString(lvMsg.c_str());
            
            //特效 白绿蓝紫橙金
            m_particle7->removeAllChildren();
            if(1==info.color){//绿
                int part = 3;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }else if (2==info.color){//蓝
                int part = 1;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }else if (3==info.color){//紫
                int part = 4;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }else if (4==info.color){//橙
                int part = 5;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }else if (5==info.color){//金
                int part = 2;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }
        }
//        else if (site==7) {
//            m_equipNode8->addChild(bg);
//            m_equipNode8->addChild(icon);
//            m_equipNode8->addChild(bgKuang);
//            m_equipLabel8->setString(lvMsg.c_str());
//        }
    }
    
    
    for (int i=0; i<=7; i++) {
        if (tmpMap.find(i) == tmpMap.end()) {
            string siteIcon = "";
            if (i==0) {
                siteIcon = "icon_weapons.png";
            }else if (i==1) {
                siteIcon = "icon_peeping.png";
            }else if (i==2) {
                siteIcon = "icon_clothes.png";
            }else if (i==3) {
                siteIcon = "icon_trousers.png";
            }else if (i==4) {
                siteIcon = "icon_shoes.png";
            }else if (i==5) {
                siteIcon = "icon_ring.png";
            }else if (i==6) {
                siteIcon = "icon_long_close.png";
            }
//            else if (i==7) {
//                siteIcon = "icon_long_close.png";
//            }
            auto icon = CCLoadSprite::createSprite(siteIcon.c_str(), true,CCLoadSpriteType_DEFAULT);
//            CCCommonUtils::setSpriteMaxSize(icon, 90, true);
//            CCCommonUtils::setSpriteGray(icon, true);
            
            bool isHave = EquipmentController::getInstance()->IsExsitRealEquipBySite(i);
            if (m_info) {
                isHave = false;
            }
            
            if (i==0) {
                m_equipNode1->addChild(icon);
                if (isHave) {
                    m_red1Pt->setVisible(true);
                }
            }else if (i==1) {
                m_equipNode2->addChild(icon);
                if (isHave) {
                    m_red2Pt->setVisible(true);
                }
            }else if (i==2) {
                m_equipNode3->addChild(icon);
                if (isHave) {
                    m_red3Pt->setVisible(true);
                }
            }else if (i==3) {
                m_equipNode4->addChild(icon);
                if (isHave) {
                    m_red4Pt->setVisible(true);
                }
            }else if (i==4) {
                m_equipNode5->addChild(icon);
                if (isHave) {
                    m_red5Pt->setVisible(true);
                }
            }else if (i==5) {
                m_equipNode6->addChild(icon);
                if (isHave) {
                    m_red6Pt->setVisible(true);
                }
            }else if (i==6) {
                if (isOpenLongJing) {
                    m_equipNode7->addChild(icon);
                    if (isHave) {
                        m_red7Pt->setVisible(true);
                    }
                }
            }
//            else if (i==7) {
//                m_equipNode8->addChild(icon);
//                if (isHave) {
//                    m_red8Pt->setVisible(true);
//                }
//            }
        }
    }
}



DragonUpgradeAniNode* DragonUpgradeAniNode::create(int tid){
    DragonUpgradeAniNode* ret = new DragonUpgradeAniNode();
    
    if(ret && ret->init(tid)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonUpgradeAniNode::init(int tid)
{
//    auto bg = CCBLoadFile("pic415000_2_1",this,this);
//
//    
//    getAnimationManager()->runAnimations("Havest");
//
//    getAnimationManager()->setAnimationCompletedCallback(this, CC_CALLFUNC_SELECTOR(DragonUpgradeAniNode::animationCallBack));

    
    
    
    return true;
}

void DragonUpgradeAniNode::onEnter()
{
    Node::onEnter();
    
    auto bg = CCBLoadFile("Titan_eggfx",this,this);
    
   
    getAnimationManager()->runAnimations("FadeIn");
    
    float time = getAnimationManager()->getSequenceDuration("FadeIn");
    
//    getAnimationManager()->setAnimationCompletedCallback(this, CC_CALLFUNC_SELECTOR(DragonUpgradeAniNode::animationCallBack));

    
    
    this->runAction(Sequence::create(DelayTime::create(time*0.75),CallFunc::create(CC_CALLBACK_0(DragonUpgradeAniNode::animationCallBack,this)),nullptr));
};

void DragonUpgradeAniNode::onExit()
{
//    CCLoadSprite::doResourceByCommonIndex(513, false);
    Node::onExit();
};

void DragonUpgradeAniNode::animationCallBack()
{
    CCLOG("animation complete !!!!");
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_UPGRADE_ANIMATION_COMLETE, CCString::createWithFormat("animationComplete"));
    
  
    this->removeFromParent();
};
SEL_CCControlHandler DragonUpgradeAniNode::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{

    return NULL;
}
bool DragonUpgradeAniNode::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{

    
    return false;
}


