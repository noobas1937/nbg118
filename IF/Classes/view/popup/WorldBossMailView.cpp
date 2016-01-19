//
//  WorldBossMailView.cpp
//  LastEmpire
//
//  Created by zhangjun on 2015-04-27 10:21:14.
//
//

#include "WorldBossMailView.h"
#include "MailController.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"
#include "RewardController.h"
#include "MailGetRewardCommand.h"
#include "WorldController.h"
#include "MailSaveCommand.h"
#include "MailPopUpView.h"
#include "MailWritePopUpView.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "WorldMapView.h"
WorldBossMailView* WorldBossMailView::create(MailInfo& info)
{
    WorldBossMailView *pRet = new WorldBossMailView(info);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}
bool WorldBossMailView::init()
{
    bool bRet = false;
    if (PopupBaseView::init())
    {
        setIsHDPanel(true);
        setMailUuid(m_info->uid);
//        auto cf = CCLoadSprite::getSF("Mail_diban.png");
        auto cf = CCLoadSprite::getSF("Mail_BG1.png");
        if (cf==NULL) {
            CCLoadSprite::doResourceByCommonIndex(504, true);
            CCLoadSprite::doResourceByCommonIndex(206, true);
            CCLoadSprite::doResourceByCommonIndex(6, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(504, false);
                CCLoadSprite::doResourceByCommonIndex(206, false);
                CCLoadSprite::doResourceByCommonIndex(6, false);
                CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERBUST);
            });
        }
        else {
            CCLoadSprite::doResourceByCommonIndex(504, true);
            CCLoadSprite::doResourceByCommonIndex(206, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(504, false);
                CCLoadSprite::doResourceByCommonIndex(206, false);
                CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERBUST);
            });
        }
        
        /*Load CCB*/
        auto bg = CCBLoadFile("NEW_WorldBossMailView.ccbi", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else
            this->setContentSize(bg->getContentSize());

        if (CCCommonUtils::isIosAndroidPad()) {
            int extH = getExtendHeight();
            this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + extH));
            m_downNode->setPositionY(m_downNode->getPositionY() - extH);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
            auto tbg = CCLoadSprite::loadResource("Mail_diban.png");
            auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
            auto picBg1 = CCLoadSprite::createSprite("Mail_diban.png");
            picBg1->setAnchorPoint(ccp(0, 0));
            picBg1->setPosition(ccp(0, 0));
            picBg1->setScaleX(2.4);
            tBatchNode->addChild(picBg1);
            int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
            int curHeight = picBg1->getContentSize().height;
            while(curHeight < maxHeight)
            {
                auto picBg2 = CCLoadSprite::createSprite("Mail_diban.png");
                picBg2->setAnchorPoint(ccp(0, 0));
                picBg2->setPosition(ccp(0, curHeight));
                picBg2->setScaleX(2.4);
                tBatchNode->addChild(picBg2);
                curHeight += picBg2->getContentSize().height;
            }
            m_bgNode->addChild(tBatchNode);
        }
        else {
            int extH = getExtendHeight();
            this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + extH));
            m_downNode->setPositionY(m_downNode->getPositionY() - extH);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
            auto tbg = CCLoadSprite::loadResource("Mail_diban.png");
            auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
            auto picBg1 = CCLoadSprite::createSprite("Mail_diban.png");
            picBg1->setAnchorPoint(ccp(0, 0));
            picBg1->setPosition(ccp(0, 0));
            tBatchNode->addChild(picBg1);
            int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
            int curHeight = picBg1->getContentSize().height;
            while(curHeight < maxHeight)
            {
                auto picBg2 = CCLoadSprite::createSprite("Mail_diban.png");
                picBg2->setAnchorPoint(ccp(0, 0));
                picBg2->setPosition(ccp(0, curHeight));
                tBatchNode->addChild(picBg2);
                curHeight += picBg2->getContentSize().height;
            }
            m_bgNode->addChild(tBatchNode);
        }
//        this->m_downNode->setContentSize(CCSize(m_downNode->getContentSize().width, m_downNode->getContentSize().height + dh));
//        m_scrollView = CCScrollView::create(m_downNode->getContentSize());
//        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
//        m_downNode->addChild(m_scrollView);//策划变动，之显示自己获得的奖励物品

        refreshView();
        
        bRet = true;
    }
    return bRet;
}

void WorldBossMailView::refreshView(CCObject* p){
//    m_info.saveDict->writeToFile("/Users/zhangjun/Desktop/dictf/m_info");
    //Boss信息
    CCDictionary *defDict=dynamic_cast<CCDictionary*>(m_info->saveDict->objectForKey("def"));
    int def_id=defDict->valueForKey("id")->intValue();
    int def_mchp=defDict->valueForKey("mchp")->intValue();//当前hp
    int def_mmhp=defDict->valueForKey("mmhp")->intValue();//总hp
    int def_mLevel=defDict->valueForKey("mLevel")->intValue();//boss等级
    
    float perHp=(def_mchp*1.0)/(def_mmhp*1.0);
    m_monsterLifeText->setString(CC_ITOA_Percent(perHp));//boss血百分比
    m_bar->setScaleX(perHp);
//    std::string nameStr = CCCommonUtils::getPropById(CC_ITOA(def_id), "name");
    std::string nameStr = CCCommonUtils::getNameById(CC_ITOA(def_id));
    if(def_mLevel>0){
        nameStr=nameStr+" "+_lang_1("115348", CC_ITOA(def_mLevel));
    }
    m_monsterNameText->setString(nameStr.c_str());
    this->m_attackText->setFntFile("Arial_Bold_Regular.fnt");
    this->m_timeText->setFntFile("Arial_Bold_Regular.fnt");
    this->m_killTxt->setFntFile("Arial_Bold_Regular.fnt");
    this->m_killCountTxt->setFntFile("Arial_Bold_Regular.fnt");
    this->m_deadTxt->setFntFile("Arial_Bold_Regular.fnt");
    this->m_deadCountTxt->setFntFile("Arial_Bold_Regular.fnt");
    this->m_woundTxt->setFntFile("Arial_Bold_Regular.fnt");
    this->m_woundCountTxt->setFntFile("Arial_Bold_Regular.fnt");
    this->m_monsterLifeText->setFntFile("Arial_Bold_Regular.fnt");
    CCDictionary *myAtt=NULL;
    string leaderName="";
    CCArray *attList=dynamic_cast<CCArray*>(m_info->saveDict->objectForKey("attList"));
    for (int i=0; i<attList->count(); i++) {
        CCDictionary *dict=dynamic_cast<CCDictionary*>(attList->objectAtIndex(i));
        string pUid=GlobalData::shared()->playerInfo.uid;
        string dUid=dict->valueForKey("uid")->getCString();
        if (pUid==dUid) {
            myAtt=dict;
        }
        string dName=dict->valueForKey("name")->getCString();
        if (dict->valueForKey("leader")->intValue()==1) {
            leaderName=dName;
        }
    }
    if(myAtt){
        //小标题
//        m_upNodeText->setString(_lang_1("137455", leaderName.c_str()));//XXX的联军
        //名字
        string myName=myAtt->valueForKey("name")->getCString();
        m_nameTxt->setString(myName.c_str());
        //头像
        string myPic=myAtt->valueForKey("pic")->getCString();
        myPic+=".png";
        int myPicVer=myAtt->valueForKey("picVer")->intValue();

        CCSprite* head = CCLoadSprite::createSprite(myPic.c_str());
        head->setScale((70/head->getContentSize().width));
//        m_picNode->addChild(head);
        string dUid=myAtt->valueForKey("uid")->getCString();
//        if (CCCommonUtils::isUseCustomPic(myPicVer))
//        {
//            m_headImgNode=HFHeadImgNode::create();
//            m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(dUid, myPicVer), 1.0f, 60, true);
//        }
        
        ///////
        auto sizeLayer = CCSize(80, 80);
        m_selfModelLayer = CCRenderTexture::create(sizeLayer.width, sizeLayer.height);
        m_selfModelLayer->setAnchorPoint(ccp(0.5, 0.5));
        ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
        auto spr = CCLoadSprite::createSprite("Mail_headBack.png");
        spr->setScale(1);
        spr->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
        auto bgCircle = CCLoadSprite::createSprite("Mail_head_backBattle.png");
        bgCircle->setBlendFunc(cbf);
        bgCircle->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
        head->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 3));
        //    pic->removeFromParent();
        m_selfModelLayer->begin();
        spr->visit();
        head->visit();
        bgCircle->visit();
        m_selfModelLayer->end();
        m_picNode->addChild(m_selfModelLayer);
        if (CCCommonUtils::isUseCustomPic(myPicVer) && dUid != "")
        {
            m_headImgNode=HFHeadImgNode::create();
            string backImg = "Mail_headBack.png";
            string renderImg = "Mail_head_backBattle.png";
//            m_headImgNode->initHeadImgUrl3(m_picNode, CCCommonUtils::getCustomPicUrl(dUid, myPicVer), 1.0f, 74, true, ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 2), sizeLayer, backImg, renderImg);  simon
        }
        
        //我的贡献
        CCDictionary *myAttData = dynamic_cast<CCDictionary*>(myAtt->objectForKey("att"));
        int kill = myAttData->valueForKey("kill")->intValue();//击杀
        int dead = myAttData->valueForKey("dead")->intValue();//死亡
        int hurt = myAttData->valueForKey("hurt")->intValue();//伤兵
//        int survived = myAttData->valueForKey("survived")->intValue();
//        int total = myAttData->valueForKey("total")->intValue();
        string killStr = _lang("105543");//击杀
        killStr+=":";
        m_killTxt->setString(killStr.c_str());
        m_killCountTxt->setString(CC_ITOA_Percent((kill*1.0)/(def_mmhp*1.0)));
        string deadStr = _lang("105544");//损失
        deadStr+=":";
        m_deadTxt->setString(deadStr.c_str());
        m_deadCountTxt->setString(CC_ITOA(dead));
        string woundStr = _lang("105545");//受伤
        woundStr+=":";
        m_woundTxt->setString(woundStr.c_str());
        m_woundCountTxt->setString(CC_ITOA(hurt));
        //获得奖品
        m_rewardTxt->setString(_lang("137452"));//获得奖励
//        CCArray *myRateReward = dynamic_cast<CCArray*>(myAtt->objectForKey("rateReward"));
//        string rewardStr="";
//        for (int i=0; i<myRateReward->count(); i++) {
//            CCDictionary *dict=dynamic_cast<CCDictionary*>(myRateReward->objectAtIndex(i));
//            rewardStr+=dict->valueForKey("type")->getCString();
//            rewardStr+=",";
//            CCDictionary *dict2=dynamic_cast<CCDictionary*>(dict->objectForKey("value"));
//            rewardStr+=dict2->valueForKey("itemId")->getCString();
//            rewardStr+=",";
//            rewardStr+=dict2->valueForKey("rewardAdd")->getCString();
//            if (i<myRateReward->count()-1) {
//                rewardStr+="|";
//            }
//        }
//        refreshRewardView(CCString::create(rewardStr));
    }
    
    setTitleName(_lang("137444"));//世界boss
    CCPoint warPoint=WorldController::getPointByIndex(m_info->saveDict->valueForKey("xy")->intValue());
    this->m_mailTitle->setString(_lang_2("105518", CC_ITOA(warPoint.x), CC_ITOA(warPoint.y)));//m_info->warPoint.x,m_info->warPoint.y
    std::string attName = "";
    std::string defName = "";
    attName = _lang("102187");//"我的部队"
    defName = nameStr;
    m_attackText->setString(_lang_2("105547", attName.c_str(), defName.c_str()));//{0}攻击{1}
    this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());//时间
//    this->m_upTitle->setString(_lang("137451"));//挑战世界BOSS战报
    //图片
    string strPos = "";
    strPos.append("X:").append(CC_ITOA(warPoint.x)).append(" Y:").append(CC_ITOA(warPoint.y));
    m_posTxt->setString(strPos.c_str());
    m_posBG->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_posBG->getContentSize().height));
    m_underlineSpr->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_underlineSpr->getContentSize().height));
    
    auto battlePic = CCLoadSprite::createSprite("worldBossMail.png");
    m_battlePicNode->removeAllChildren();
    m_battlePicNode->addChild(battlePic);
    auto monsterPic =  CCLoadSprite::createSprite("worldBossMailMonster.png",true,CCLoadSpriteType_MONSTERLAYERBUST);
    m_monsterPicNode->removeAllChildren();
    m_monsterPicNode->addChild(monsterPic);
    //获得奖品
    refreshRewardView();
    CCCommonUtils::setButtonTitle(m_getRewardBtn, _lang("105572").c_str());
    
    m_shareBtn->setVisible(false);//隐藏分享按钮
    
//    refreshRewardView();
    //只显示自己的贡献信息，注释掉下面语句
//    int cellCount=3;
//    for (int i=0; i<cellCount; i++) {
//        WorldBossMailPlayerCellView *pCell = WorldBossMailPlayerCellView::create();
//        if (i==cellCount-1) {
//            pCell->setData(i,true);
//        }else{
//            pCell->setData(i);
//        }
//        
//        pCell->setPosition(0, (PLayerCellH+10)*i);
//        m_scrollView->addChild(pCell);
//    }
//    float m_Listheight=(PLayerCellH+10)*cellCount+PLayerCellHeadH;
//    m_scrollView->setContentSize(CCSize(m_downNode->getContentSize().width, m_Listheight));
//    m_scrollView->setContentOffset(ccp(0, m_downNode->getContentSize().height - (m_Listheight)));
    
}

void WorldBossMailView::refreshRewardView(CCObject* p){
    if(m_info->rewardId != ""){
        bool gray = (m_info->rewardStatus==1);
        this->m_getRewardBtn->setEnabled(!gray);//领奖按钮
        CCCommonUtils::splitString(m_info->rewardId, "|", rewardVector);
        int count =rewardVector.size();
        count=count>3?3:count;//最多3个奖品
        for (int i=0; i<count; i++) {
            vector<std::string> vector1;
            CCCommonUtils::splitString(rewardVector[i], ",", vector1);
            int type = 0;
            int value = 0;
            int num = 0;
            if(vector1.size()>=3){
                type = RewardController::getInstance()->getTypeByName(vector1[0]);
                value = atoi(vector1[1].c_str());
                num = atoi(vector1[2].c_str());
            }
//            std::string nameStr;
//            std::string picStr;
//            if(type == R_GOODS || type == R_GENERAL){
//                nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
//                picStr =CCCommonUtils::getIcon(CC_ITOA(value));
//                if(picStr==".png"){
//                    picStr = "no_iconFlag.png";
//                }
//            }else if(type == R_HONOR || type == R_ALLIANCE_POINT){
//                nameStr = RewardController::getInstance()->getNameByType(type,0);
//                picStr = RewardController::getInstance()->getPicByType(type,0);
//            }else{
//                nameStr = CCCommonUtils::getResourceNameByType(type);
//                picStr =CCCommonUtils::getResourceIconByType(type);
//                if(picStr==".png"){
//                    picStr = "no_iconFlag.png";
//                }
//            }
//            auto icon  = (type == R_GOODS)? CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_GOODS):CCLoadSprite::createSprite(picStr.c_str());
//            m_rwdPicNode[i]->addChild(icon);
            CCCommonUtils::createGoodsIcon(value, m_rwdPicNode[i], CCSize(95, 95));
            m_rwdNum[i]->setFntFile("Arial_Bold_Regular.fnt");
            m_rwdNum[i]->setString(CC_ITOA(num));
            m_rwdCover[i]->setVisible(gray);//领过奖，图片显示灰色
        }
        
//    }else if (p){
//        CCString* cstr=dynamic_cast<CCString*>(p);
//        string str=cstr->getCString();
//        if (str!="") {
//            bool gray = (m_info.rewardStatus==1);
//            this->m_getRewardBtn->setEnabled(!gray);//领奖按钮
//            
////            gray=false;
////            this->m_getRewardBtn->setVisible(false);
//            
//            CCCommonUtils::splitString(str, "|", rewardVector);
//            int count =rewardVector.size();
//            count=count>3?3:count;//最多3个奖品
//            for (int i=0; i<count; i++) {
//                vector<std::string> vector1;
//                CCCommonUtils::splitString(rewardVector[i], ",", vector1);
//                int type = 0;
//                int value = 0;
//                int num = 0;
//                if(vector1.size()>=3){
////                    type = RewardController::getInstance()->getTypeByName(vector1[0]);
//                    type = atoi(vector1[0].c_str());
//                    value = atoi(vector1[1].c_str());
//                    num = atoi(vector1[2].c_str());
//                }
//                std::string nameStr;
//                std::string picStr;
//                if(type == R_GOODS || type == R_GENERAL){
//                    nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
//                    picStr =CCCommonUtils::getIcon(CC_ITOA(value));
//                    if(picStr==".png"){
//                        picStr = "no_iconFlag.png";
//                    }
//                }else if(type == R_HONOR || type == R_ALLIANCE_POINT){
//                    nameStr = RewardController::getInstance()->getNameByType(type,0);
//                    picStr = RewardController::getInstance()->getPicByType(type,0);
//                }else{
//                    nameStr = CCCommonUtils::getResourceNameByType(type);
//                    picStr =CCCommonUtils::getResourceIconByType(type);
//                    if(picStr==".png"){
//                        picStr = "no_iconFlag.png";
//                    }
//                }
//                auto icon  = (type == R_GOODS)? CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_GOODS):CCLoadSprite::createSprite(picStr.c_str());
//                m_rwdPicNode[i]->addChild(icon);
//                if(gray){
//                    icon->setColor({90,85,81});//领过奖，图片显示灰色
//                }
//                
//            }
//        }
    }else{
        this->m_getRewardBtn->setEnabled(false);//领奖按钮
    }

}

void WorldBossMailView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade,true);//simon;
    UIComponent::getInstance()->hideReturnBtn();

//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WorldBossMailView::refreshRewardView), MIAL_GET_REWARD_REFRESH, NULL);
}
void WorldBossMailView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MIAL_GET_REWARD_REFRESH);
    setTouchEnabled(false);
    CCNode::onExit();
}

//ccTouch
bool WorldBossMailView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(isTouchInside(this->m_listContainer, pTouch)){
        return true;
    }
    if (isTouchInside(this->m_returnSpr, pTouch)) {
        return true;
    }
    if (isTouchInside(m_posBG, pTouch)) {
        return true;
    }
    return false;
}

void WorldBossMailView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(this->m_returnSpr, pTouch)) {
        PopupViewController::getInstance()->goBackPopupView();
    }
    if (isTouchInside(m_posBG, pTouch)) {
        CCPoint warPoint=WorldController::getPointByIndex(m_info->saveDict->valueForKey("xy")->intValue());
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(warPoint);
        }else{
            int index = WorldController::getIndexByPoint(warPoint);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
        }
        //zym 2015.12.11
//        PopupViewController::getInstance()->forceClearAll(true); simon
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

void WorldBossMailView::onRwd1BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
}

void WorldBossMailView::onRwd2BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
}

void WorldBossMailView::onRwd3BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
}

void WorldBossMailView::onplayerHeadClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
}
void WorldBossMailView::onGetRewardBtnPressed(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    
    this->m_getRewardBtn->setEnabled(false);
    MailGetRewardCommand* cmd = new MailGetRewardCommand(m_info->uid, m_info->type);
    cmd->sendAndRelease();
}

SEL_CCControlHandler WorldBossMailView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", WorldBossMailView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", WorldBossMailView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShareClick", WorldBossMailView::onShareClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturnClick", WorldBossMailView::onReturnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteClick", WorldBossMailView::onWriteClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRwd1BtnPressed", WorldBossMailView::onRwd1BtnPressedClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRwd2BtnPressed", WorldBossMailView::onRwd2BtnPressedClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRwd3BtnPressed", WorldBossMailView::onRwd3BtnPressedClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "playerHeadClick", WorldBossMailView::onplayerHeadClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGetRewardBtnPressed", WorldBossMailView::onGetRewardBtnPressed);
    
    return NULL;
}

SEL_MenuHandler WorldBossMailView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CallFuncN WorldBossMailView::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool WorldBossMailView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*,this->m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*,this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_failNode", CCNode*,this->m_failNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_firstNode", CCNode*,this->m_firstNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*,this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*,this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*,this->m_bgNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*,this->m_upNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*,this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*,this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintBG", CCScale9Sprite*,this->m_hintBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackText", CCLabelIF*,this->m_attackText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*,this->m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailTitle", CCLabelIF*,this->m_mailTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*,this->m_timeText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upTitle", CCLabelIF*,this->m_upTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*,this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*,this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shareBtn", CCControlButton*,this->m_shareBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnSpr", CCSprite*,this->m_returnSpr);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*,this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintLine", CCSprite*,this->m_hintLine);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_monsterNameText", CCLabelIF*,this->m_monsterNameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_monsterLifeText", CCLabelIF*,this->m_monsterLifeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_monsterPicNode", CCNode*,this->m_monsterPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*,this->m_bar);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*,this->m_mainCellNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*,this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode1", CCNode*,this->m_rwdNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode2", CCNode*,this->m_rwdNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode3", CCNode*,this->m_rwdNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode1", CCNode*,this->m_rwdPicNode[0]);//m_rwdPicNode1
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode2", CCNode*,this->m_rwdPicNode[1]);//m_rwdPicNode2
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode3", CCNode*,this->m_rwdPicNode[2]);//m_rwdPicNode3
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdCover1", CCLayerColor*,this->m_rwdCover[0]);//m_rwdPicNode3
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdCover2", CCLayerColor*,this->m_rwdCover[1]);//m_rwdPicNode3
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdCover3", CCLayerColor*,this->m_rwdCover[2]);//m_rwdPicNode3
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNum1", CCLabelIF*,this->m_rwdNum[0]);//m_rwdPicNode3
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNum2", CCLabelIF*,this->m_rwdNum[1]);//m_rwdPicNode3
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNum3", CCLabelIF*,this->m_rwdNum[2]);//m_rwdPicNode3
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*,this->m_upCellNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBtn", CCControlButton*,this->m_iconBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwd1Btn", CCControlButton*,this->m_rwd1Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwd2Btn", CCControlButton*,this->m_rwd2Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwd3Btn", CCControlButton*,this->m_rwd3Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deadCountTxt", CCLabelIF*,this->m_deadCountTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deadTxt", CCLabelIF*,this->m_deadTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killCountTxt", CCLabelIF*,this->m_killCountTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killTxt", CCLabelIF*,this->m_killTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", Label*,this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTxt", CCLabelIF*,this->m_rewardTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNodeText", CCLabelIF*,this->m_upNodeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundCountTxt", CCLabelIF*,this->m_woundCountTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundTxt", CCLabelIF*,this->m_woundTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getRewardBtn", CCControlButton*,this->m_getRewardBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posTxt", CCLabelIF*, m_posTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG", Node*, m_posBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr", CCScale9Sprite*, m_underlineSpr);
    return false;
}

		
void WorldBossMailView::onAddSaveClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
	CCLOGFUNC();
    if(m_info->save ==1){
        MailCancelSaveCommand *cmd = new MailCancelSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
    }else{
        if(MailController::getInstance()->isMailFull(SAVEMAIL)){
            CCCommonUtils::flyText("full");
            return;
        }
        MailSaveCommand *cmd = new MailSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
    }
    if(m_info->save==0){
        m_addSaveBtn->setHighlighted(false);
    }else{
        m_addSaveBtn->setHighlighted(true);
    }
}

void WorldBossMailView::onDeleteClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
	CCLOGFUNC();
    if(m_info->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(WorldBossMailView::onOkDeleteMail)),true);
}
void WorldBossMailView::onOkDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    PopupViewController::getInstance()->goBackPopupView();
}
void WorldBossMailView::onReturnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void WorldBossMailView::onWriteClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
}
void WorldBossMailView::onShareClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
	CCLOGFUNC();
//    double gapTime = m_info->shareTime -GlobalData::shared()->getWorldTime();
//    if(gapTime<0){
//        string reportLang = "115277";
//        string msg = "";
//        int result = this->getReportResult();
//        std::string attName = "";
//        if(m_info->attUser){
//            attName = m_info->attUser->valueForKey("name")->getCString();
//        }
//        attUid = m_info->attUser->valueForKey("uid")->getCString();
//        
//        std::string defName = "";
//        if(m_info->defUser){
//            defName = m_info->defUser->valueForKey("name")->getCString();
//        }
//        defUid = m_info->defUser->valueForKey("uid")->getCString();
//        string param1 = "";
//        if(result==WIN){
//            if(isatkHelp || ownerUid==attUid){
//                reportLang = "115277";
//                param1 = defName;
//            }else{
//                reportLang = "115279";
//                param1 = attName;
//            }
//        }else if(result==DRAW){
//            if(isatkHelp || ownerUid==attUid){
//                reportLang = "115283";
//                param1 = defName;
//            }else{
//                reportLang = "115284";
//                param1 = attName;
//            }
//            
//        }else{
//            isdefHelp = false;
//            if(m_info->defHelper!=NULL){
//                cocos2d::CCObject* obj;
//                CCARRAY_FOREACH(m_info->defHelper, obj){
//                    string helpUid = dynamic_cast<CCString*>(obj)->getCString();
//                    if(helpUid==ownerUid){
//                        isdefHelp = true;
//                        break;
//                    }
//                }
//            }
//            if(isdefHelp || ownerUid==defUid){
//                reportLang = "115280";
//                param1 = attName;
//            }else{
//                reportLang = "115278";
//                param1 = defName;
//            }
//        }
//        
//        msg = _lang_1(reportLang, param1.c_str());
//        CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,msg.c_str(),CHAT_TYPE_BATTLE_SHARE);
//        cmd->putParam("reportUid", CCString::create(m_info->uid));
//        cmd->putParam("reportLang",CCString::create(reportLang));
//        cmd->sendAndRelease();
//        double shareTime = GlobalData::shared()->getWorldTime();
//        shareTime = shareTime + 10*60 - 1;
//        m_info->shareTime = shareTime;
//    }else{
//        int gap = gapTime/60 +1;
//        if(gap<=0) gap = 1;
//        CCCommonUtils::flyHint("", "", _lang_1("115276", CC_ITOA(gap)));
//    }
}

#pragma mark WorldBossMailPlayerCellView

WorldBossMailPlayerCellView::WorldBossMailPlayerCellView()
{
    CCLOGFUNC();
}

WorldBossMailPlayerCellView::~WorldBossMailPlayerCellView()
{
    CCLOGFUNC();
}
WorldBossMailPlayerCellView* WorldBossMailPlayerCellView::create()
{
    WorldBossMailPlayerCellView *pRet = new WorldBossMailPlayerCellView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}
bool WorldBossMailPlayerCellView::init()
{
    bool bRet = false;
    if (CCNode::init())
    {
        /*Load CCB*/
        CCBLoadFile("WorldBossMailPlayerCellView.ccbi", this, this);
        bRet = true;
    }
    return bRet;
}

void WorldBossMailPlayerCellView::setData(int index,bool isShowTitle){
    m_upNode->setVisible(isShowTitle);
    if (isShowTitle) {
        m_upNodeText->setString("m_upNodeText");
    }
    
    m_nameTxt->setString("m_nameTxt");
    m_killTxt->setString("m_killTxt");
    m_killCountTxt->setString("6666");
    m_deadTxt->setString("m_deadTxt");
    m_deadCountTxt->setString("66666");
    m_woundTxt->setString("m_woundTxt");
    m_woundCountTxt->setString("666666");
    m_rewardTxt->setString("m_rewardTxt");
    
    CCSprite* head = CCLoadSprite::createSprite(GlobalData::shared()->playerInfo.getPic().c_str());
    head->setScale((94/head->getContentSize().width));
    m_picNode->addChild(head);
}

void WorldBossMailPlayerCellView::onEnter()
{
    CCNode::onEnter();
}
void WorldBossMailPlayerCellView::onExit()
{
    CCNode::onExit();
}

void WorldBossMailPlayerCellView::onRwd1BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
}

void WorldBossMailPlayerCellView::onRwd2BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
}

void WorldBossMailPlayerCellView::onRwd3BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
}

void WorldBossMailPlayerCellView::onplayerHeadClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
}

SEL_CCControlHandler WorldBossMailPlayerCellView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRwd1BtnPressed", WorldBossMailPlayerCellView::onRwd1BtnPressedClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRwd2BtnPressed", WorldBossMailPlayerCellView::onRwd2BtnPressedClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRwd3BtnPressed", WorldBossMailPlayerCellView::onRwd3BtnPressedClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "playerHeadClick", WorldBossMailPlayerCellView::onplayerHeadClick);
    return NULL;
}

SEL_MenuHandler WorldBossMailPlayerCellView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CallFuncN WorldBossMailPlayerCellView::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool WorldBossMailPlayerCellView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*,this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*,this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode1", CCNode*,this->m_rwdNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode2", CCNode*,this->m_rwdNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode3", CCNode*,this->m_rwdNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode1", CCNode*,this->m_rwdPicNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode2", CCNode*,this->m_rwdPicNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdPicNode3", CCNode*,this->m_rwdPicNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*,this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBtn", CCControlButton*,this->m_iconBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwd1Btn", CCControlButton*,this->m_rwd1Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwd2Btn", CCControlButton*,this->m_rwd2Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwd3Btn", CCControlButton*,this->m_rwd3Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deadCountTxt", CCLabelIF*,this->m_deadCountTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deadTxt", CCLabelIF*,this->m_deadTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killCountTxt", CCLabelIF*,this->m_killCountTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killTxt", CCLabelIF*,this->m_killTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", Label*,this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTxt", CCLabelIF*,this->m_rewardTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNodeText", CCLabelIF*,this->m_upNodeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundCountTxt", CCLabelIF*,this->m_woundCountTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundTxt", CCLabelIF*,this->m_woundTxt);
    return false;
}


