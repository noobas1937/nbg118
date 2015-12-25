//
//  ManagerKingdomView.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-10.
//
//

#include "ManagerKingdomView.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"
#include "GameController.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "WorldController.h"
#include "CountryChooseFlagView.h"
#include "UIComponent.h"
#include "KingdomNameChangeView.h"
#include "KingChangePalaceView.h"
#include "KingsGiftView.h"
#include "ButtonLightEffect.h"

ManagerKingdomView *ManagerKingdomView::create(){
    ManagerKingdomView *ret = new ManagerKingdomView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ManagerKingdomView::ManagerKingdomView(){
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ManagerKingdomView::refreshKingdomBanner), KINGDOM_BANNER_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ManagerKingdomView::refreshKingdomPalace), KINGDOM_PALACE_CHANGE, NULL);
}

ManagerKingdomView::~ManagerKingdomView(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, KINGDOM_BANNER_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, KINGDOM_PALACE_CHANGE);
}

void ManagerKingdomView::onEnter(){
    PopupBaseView::onEnter();
    //setTitleName(_lang("110021"));
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ManagerKingdomView::refreshKingdomName), KINGDOM_NAME_CHANGE, NULL);
    KingdomOfficersCommand* cmd = new KingdomOfficersCommand();
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ManagerKingdomView::getOfficersData), NULL));
    cmd->sendAndRelease();
}

void ManagerKingdomView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, KINGDOM_NAME_CHANGE);
    PopupBaseView::onExit();
}

void ManagerKingdomView::refreshKingdomName(CCObject* data){
    m_nameBtn->setEnabled(false);
    m_animNode->setVisible(false);
    string kingdomName = this->getKing()->kingdomName;
    m_kingTxt->setString(_lang_1("110023",kingdomName.c_str()));
}

void ManagerKingdomView::refreshKingdomBanner(CCObject* data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        m_banner = str->getCString();
        this->refreshBanner();
        m_chBanner = true;
        m_bannerAni->setVisible(!m_chBanner);
    }
}

void ManagerKingdomView::refreshKingdomPalace(CCObject* data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        m_palace = str->getCString();
        //this->refreshPalace();
        m_chThronePicFlag = true;
        m_palaceAni->setVisible(!m_chThronePicFlag);
    }
}

void ManagerKingdomView::refreshBanner(){
    string banner = "";
    if (m_banner=="") {
        banner = "UN.png";
    }else{
        //大陆地区，看到台湾的，都成了大陆的国旗
        if((banner=="TW" || banner=="HK") && CCCommonUtils::checkTaiWanFlag()){
            banner = "CN";
        }
        banner = CCCommonUtils::changeChinaFlag(banner);
        banner = m_banner + ".png";
    }
    m_flagNode->removeAllChildrenWithCleanup(true);
    CCLoadSprite::doResourceByCommonIndex(208, true);
    auto flag = CCLoadSprite::createSprite(banner.c_str());
    CCCommonUtils::setSpriteMaxSize(flag, (int)(0.38*157));
    m_flagNode->addChild(flag);
}

void ManagerKingdomView::refreshPalace(){
//    m_palaceNode->removeAllChildrenWithCleanup(true);
//    if(m_palace==""){
//        m_palace = "301";
//    }
//    if(m_palace=="303"){
//        auto sprite = CCLoadSprite::createSprite(CCString::createWithFormat("throne_tower_%s.png",m_palace.c_str())->getCString());
//        sprite->setPosition(ccp(1.6, 20.7));
//        sprite->setScale(0.15);
//        m_palaceNode->addChild(sprite);
//        
//        auto sprite2 = CCLoadSprite::createSprite(CCString::createWithFormat("throne_%s.png",m_palace.c_str())->getCString());
//        sprite2->setPosition(ccp(0, 0));
//        sprite2->setScale(0.15);
//        m_palaceNode->addChild(sprite2);
//    }else{
//        auto sprite = CCLoadSprite::createSprite(CCString::createWithFormat("throne_tower_%s.png",m_palace.c_str())->getCString());
//        sprite->setPosition(ccp( -0.4, 21.7));
//        sprite->setScale(0.15);
//        m_palaceNode->addChild(sprite);
//        
//        auto sprite2 = CCLoadSprite::createSprite(CCString::createWithFormat("throne_%s.png",m_palace.c_str())->getCString());
//        sprite2->setPosition(ccp(0, 0));
//        sprite2->setScale(0.15);
//        m_palaceNode->addChild(sprite2);
//    }
}

bool ManagerKingdomView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPos = pTouch->getLocation();
    if(isTouchInside(m_modifyBtn, pTouch) && m_modifyBtn->isEnabled()){
         m_modifyBtn->setHighlighted(true);
    }
    if(isTouchInside(m_changeBtn, pTouch)){
        m_changeBtn->setHighlighted(true);
    }
    return true;
}

void ManagerKingdomView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_modifyBtn, pTouch) && m_modifyBtn->isEnabled()){
        m_modifyBtn->setHighlighted(true);
    }
    if(isTouchInside(m_changeBtn, pTouch)){
        m_changeBtn->setHighlighted(true);
    }
}

void ManagerKingdomView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_modifyBtn, pTouch) && m_modifyBtn->isEnabled()){
        m_modifyBtn->setHighlighted(false);
    }
    m_changeBtn->setHighlighted(false);
    if(isTouchInside(m_giftArea, pTouch)){
       PopupViewController::getInstance()->addPopupInView(KingsGiftView::create());
    }else if(isTouchInside(m_flagArea, pTouch) && !m_chBanner && CCCommonUtils::isShowFlagForChinese()){
        PopupViewController::getInstance()->addPopupInView(CountryChooseFlagView::create());
    }else if(isTouchInside(m_palaceArea, pTouch)  && !m_chThronePicFlag){
        PopupViewController::getInstance()->addPopupInView(KingChangePalaceView::create(m_palace));
    }else if(isTouchInside(m_scNode, pTouch)){
        CCPoint end = pTouch->getLocation();
        if(fabsf(end.x-m_startPos.x) <30 && fabsf(end.y-m_startPos.y)<30){
            if(isTouchInside(m_modifyBtn, pTouch) && m_modifyBtn->isEnabled()){
                onModifyResClick(NULL,Control::EventType::TOUCH_DOWN);
            }
        }
    }else if(isTouchInside(m_kingNode, pTouch)){
        OfficeInfo* info = NULL;
        for (int i=0; i<m_data->count(); i++) {
            OfficeInfo* temp = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i));
            string oid = CC_ITOA(temp->id);
            if(oid==KINGDOM_KING_ID){
                info = temp;
                break;
            }
        }
        if(info){
            PopupViewController::getInstance()->addPopupView(KingdomRemoveAppointView::create(info,false));
        }
    }
}

bool ManagerKingdomView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//        int curHight = -500;
//        if (CCCommonUtils::isIosAndroidPad()) {
//            curHight = -1200;
//        }
//        m_palace = "301";
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
        CCLoadSprite::doResourceByCommonIndex(208, true);
        CCLoadSprite::doResourceByCommonIndex(102, true);
        CCLoadSprite::doResourceByCommonIndex(307, true);
        CCLoadSprite::doResourceByCommonIndex(310, true);
        if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
            CCLoadSprite::doResourceByWorldIndex(2, true);
        }
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(208, false);
            CCLoadSprite::doResourceByCommonIndex(102, false);
            CCLoadSprite::doResourceByCommonIndex(307, false);
            CCLoadSprite::doResourceByCommonIndex(310, false);
            if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
                CCLoadSprite::doResourceByWorldIndex(2, false);
            }
        });

        auto tmpCCB = CCBLoadFile("ManagerKingdomView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else {
            this->setContentSize(tmpCCB->getContentSize());
            int prevH = m_viewBg->getContentSize().height;
            changeBGHeight(m_viewBg);
            int add = m_viewBg->getContentSize().height - prevH;
            m_scNode->setContentSize(CCSize(m_scNode->getContentSize().width,m_scNode->getContentSize().height+add));
            m_scNode->setPositionY(m_scNode->getPositionY()-add);
        }
        
        m_viewBg->setVisible(false);
        m_animNode = CCNode::create();
        m_nameBtn->getParent()->addChild(m_animNode);
        m_animNode->setPosition(m_nameBtn->getPosition());
        CCSize btnSize = m_nameBtn->getContentSize();
        if (CCCommonUtils::isIosAndroidPad())
        {
            btnSize.width = m_nameBtn->getContentSize().width * m_nameBtn->getScaleX();
            btnSize.height = m_nameBtn->getContentSize().height * m_nameBtn->getScaleY();
        }
        ButtonLightEffect* btnEffect = ButtonLightEffect::create(btnSize);
        m_animNode->addChild(btnEffect);
        
//        CCSprite* Spr = CCSprite::create();
//        m_animNode->addChild(Spr);
//        string curPath = "ButtonLight_%d.png";
//        CCCommonUtils::makeInternalEffectSpr(Spr, curPath, 12, 0.07,0,2.0);
//        Spr->setAnchorPoint(ccp(0.5, 0.5));
//        float scalex = m_nameBtn->getContentSize().width/Spr->getContentSize().width;
//        float scaley = (m_nameBtn->getContentSize().height)/Spr->getContentSize().height;
//        Spr->setScaleX(scalex);
//        Spr->setScaleY(scaley);
//        Spr->setOpacity(175);
        m_animNode->setVisible(false);
        
        m_data = CCArray::create();
        
        m_scrollView = CCScrollView::create(m_scNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(Touch_Popup_Item);
        m_scNode->addChild(m_scrollView);
        
        if (CCCommonUtils::isIosAndroidPad()) {
            m_officalsNode->removeFromParent();
            m_officalsNode->setPositionY(m_officalsNode->getPositionY() + 1800);
            m_scrollView->addChild(m_officalsNode);
            
            m_servantNode->removeFromParent();
            m_servantNode->setPositionY(m_servantNode->getPositionY() + 1800);
            m_scrollView->addChild(m_servantNode);
            
            m_resNode->removeFromParent();
            m_resNode->setPositionY(m_resNode->getPositionY()+ 1800);
            m_scrollView->addChild(m_resNode);
            m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,3000));//1350
            m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - 3000));
        }
        else {
            m_officalsNode->removeFromParent();
            m_officalsNode->setPositionY(m_officalsNode->getPositionY() + 275+710 +65);
            m_scrollView->addChild(m_officalsNode);
            
            m_servantNode->removeFromParent();
            m_servantNode->setPositionY(m_servantNode->getPositionY() + 275+710+65);
            m_scrollView->addChild(m_servantNode);
            
            m_resNode->removeFromParent();
            m_resNode->setPositionY(m_resNode->getPositionY()+ 275+710+25+65);
            m_scrollView->addChild(m_resNode);
            m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,1410));//1350
            m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - 1410));
        }
        
        CCDictElement *element;
        auto dictSK = LocalController::shared()->DBXMLManager()->getGroupByKey("office");
        CCDICT_FOREACH(dictSK, element){
            std::string id = element->getStrKey();
            OfficeInfo* info = OfficeInfo::create();
            auto tempDic = _dict(element->getObject());
            info->parse(tempDic);
            m_data->addObject(info);
        }
        for (int i=0;i<8; i++) {
            OfficeInfo* info = NULL;
            if(m_data->count()>9){
                info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i+1));
            }
            KingdomOfficalsCell* ocell = KingdomOfficalsCell::create(info,m_scNode,true);
            if(i>3){
                if (CCCommonUtils::isIosAndroidPad()) {
                    ocell->setPosition(ccp((i-4)*350, -500));
                }
                else
                    ocell->setPosition(ccp((i-4)*154, -390));
            }else{
                if (CCCommonUtils::isIosAndroidPad()) {
                    ocell->setPosition(ccp(i*350, 0));
                }
                else
                    ocell->setPosition(ccp(i*154, -150));
            }
            m_oNode->addChild(ocell);
        }
        
        for (int i=0;i<8; i++) {
            OfficeInfo* info = NULL;
            if(m_data->count()==17){
                info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i+9));
            }
            KingdomServantCell* ocell = KingdomServantCell::create(info,m_scNode,true);
            if(i>3){
                if (CCCommonUtils::isIosAndroidPad()) {
                    ocell->setPosition(ccp((i-4)*350, -420));
                }
                else
                    ocell->setPosition(ccp((i-4)*154, -295));
            }else{
                if (CCCommonUtils::isIosAndroidPad()) {
                    ocell->setPosition(ccp(i*350, 0));
                }
                else
                    ocell->setPosition(ccp(i*154, -95));
            }
            m_sNode->addChild(ocell);
        }
        
        m_modifyBtn->setTouchPriority(Touch_Popup);
        m_changeBtn->setTouchPriority(Touch_Popup);
        
        m_officialsTxt1->setString(_lang("110026"));
        m_officialsTxt2->setString(_lang("110027"));
        m_kingTxt->setString(_lang_1("110023",""));
        m_resTxt->setString(_lang("110034"));
        CCCommonUtils::setButtonTitle(m_nameBtn, _lang("110001").c_str());
        CCCommonUtils::setButtonTitle(m_modifyBtn, _lang("110035").c_str());
        CCCommonUtils::setButtonTitle(m_changeBtn, _lang("110091").c_str());
        
//        KingdomOfficersCommand* cmd = new KingdomOfficersCommand();
//        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ManagerKingdomView::getOfficersData), NULL));
//        cmd->sendAndRelease();
        
        m_chenckName = "";
        m_chBanner = true;
        m_bannerAni->setVisible(!m_chBanner);
        m_nameBtn->setEnabled(false);
        m_modifyBtn->setEnabled(false);
        m_chTime = 0;
        m_resIndex = -1;
        refreshRes(m_resIndex);
        enableTouchSwallow(tmpCCB);
        m_changeBtn->setSwallowsTouches(false);
        m_modifyBtn->setSwallowsTouches(false);
        m_woodBtn->setSwallowsTouches(true);
        m_foodBtn->setSwallowsTouches(true);
        m_ironBtn->setSwallowsTouches(true);
        m_stoneBtn->setSwallowsTouches(true);
        m_nameBtn->setSwallowsTouches(false);
        this->refreshPalace();
        m_countryFlagNode->setVisible(CCCommonUtils::isShowFlagForChinese());
        ret = true;
    }
    return ret;
}
void ManagerKingdomView::enableTouchSwallow(Node* tmpCCB)
{
    if(dynamic_cast<CCControlButton*>(tmpCCB))
    {
        dynamic_cast<CCControlButton*>(tmpCCB)->setSwallowsTouches(false);
    }
    if(tmpCCB->getChildrenCount()>0)
    {
        for(auto child: tmpCCB->getChildren())
        {
            enableTouchSwallow(child);
        }
    }
    
}

void ManagerKingdomView::getOfficersData(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* officers = dynamic_cast<CCArray*>(dic->objectForKey("officers"));
        string kingdomName = dic->valueForKey("kingdomName")->getCString();
        m_banner = dic->valueForKey("banner")->getCString();
        if(dic->objectForKey("resIndex")){
            m_resIndex = dic->valueForKey("resIndex")->intValue();
            refreshRes(m_resIndex);
        }
        if (dic->objectForKey("thronePic")) {
            m_palace = dic->valueForKey("thronePic")->getCString();
            this->refreshPalace();
        }
        refreshBanner();
        if(kingdomName==""){
            kingdomName = string("#") + CC_ITOA(GlobalData::shared()->playerInfo.selfServerId);
        }
        //kingdomName = (kingdomName + "#" + CC_ITOA(GlobalData::shared()->playerInfo.selfServerId));
        string kingAllianceName = dic->valueForKey("alliancename")->getCString();
        int num = officers->count();
        for (int i=0; i<num; i++) {
            auto officerInfo = _dict(officers->objectAtIndex(i));
            int posId = officerInfo->valueForKey("positionId")->intValue();
            int oNum = m_data->count();
            for (int j=0; j<oNum; j++) {
                OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(j));
                if (posId==info->id) {
                    info->playerName = officerInfo->valueForKey("name")->getCString();
                    info->uid = officerInfo->valueForKey("uid")->getCString();
                    info->playerIcon = officerInfo->valueForKey("pic")->getCString();
                    info->abbr = officerInfo->valueForKey("abbr")->getCString();
                    info->picVer = officerInfo->valueForKey("picVer")->intValue();
                    info->chNameFlag = officerInfo->valueForKey("chNameFlag")->boolValue();
                    info->m_chBanner = officerInfo->valueForKey("chBannerFlag")->boolValue();
                    info->time = officerInfo->valueForKey("time")->doubleValue();
                    info->allianceName = officerInfo->valueForKey("alliancename")->getCString();
                    if(info->id==216000){
                        m_chBanner = info->m_chBanner;
                        m_bannerAni->setVisible(!m_chBanner);
                        m_chThronePicFlag = officerInfo->valueForKey("chThronePicFlag")->boolValue();;
                        m_palaceAni->setVisible(!m_chThronePicFlag);
                        if(!info->chNameFlag){
                            m_nameBtn->setEnabled(true);
                            m_animNode->setVisible(true);
                        }
                        string temp = "";
                        if(info->uid==""){
                            temp = _lang("110077");
                            m_playerTxt->setString(_lang_1("110025",temp.c_str()));
                        }else{
                            //temp = _lang_1("110025",info->playerName.c_str());
                            temp = info->playerName;
                            if (temp.length()>19) {
                                temp = CCCommonUtils::subStrByUtf8(temp,0,19);
                                temp.append("...");
                            }
                            m_playerTxt->setString(temp.c_str());
                        }
                        if(info->allianceName==""){
                            m_allianceTxt->setString("");
                        }else{
                            m_allianceTxt->setString(info->allianceName.c_str());
                        }
                        info->kingdomName = kingdomName;
                        m_kingTxt->setString(_lang_1("110023",kingdomName.c_str()));
                        m_chTime = info->time;
                        this->unschedule(schedule_selector(ManagerKingdomView::checkResTime));
                        this->schedule(schedule_selector(ManagerKingdomView::checkResTime),1);
                        checkResTime(0.0);
                        m_kingIcon = info->playerIcon;
                        asyDelayLoad(NULL);
                    }
                }
            }
        }
        this->refreshView(NULL);
    }
}

void ManagerKingdomView::asyDelayLoad(CCObject* p){
    m_iconNode->removeAllChildrenWithCleanup(true);
    CCLoadSprite::doResourceByCommonIndex(307, true);
    auto pic = CCLoadSprite::createSprite(CCString::createWithFormat("%s_middle.png",m_kingIcon.c_str())->getCString());
    m_iconNode->addChild(pic);
}

OfficeInfo* ManagerKingdomView::getKing(){
    OfficeInfo* info = NULL;
    int num = m_data->count();
    for (int i=0;i<num; i++) {
        OfficeInfo* temp = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i));
        if(temp && temp->id==atoi(KINGDOM_KING_ID)){
            info = temp;
            break;
        }
    }
    return info;
}

void ManagerKingdomView::checkResTime(float f_time){
    auto gapTime = (m_chTime - WorldController::getInstance()->getTime())/1000;
    if(gapTime>0){
        CCCommonUtils::setButtonTitle(m_modifyBtn, CC_SECTOA(gapTime));
        m_modifyBtn->setEnabled(false);
    }else{
        this->unschedule(schedule_selector(ManagerKingdomView::checkResTime));
        m_modifyBtn->setEnabled(true);
    }
}

void ManagerKingdomView::refreshView(CCObject* p){
    auto& arr = m_oNode->getChildren();
    for (auto child : arr)
    {
        KingdomOfficalsCell* officals = dynamic_cast< KingdomOfficalsCell*>(child);
        if (officals!=NULL) {
            officals->refreshView();
        }
    }
    auto& arr2 = m_oNode->getChildren();
    for (auto child2 : arr2)
    {
        KingdomServantCell* servant = dynamic_cast< KingdomServantCell*>(child2);
        if (servant!=NULL) {
            servant->refreshView();
        }
    }
}

bool ManagerKingdomView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_resNode", CCNode*, this->m_resNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_officalsNode", CCNode*, this->m_officalsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_servantNode", CCNode*, this->m_servantNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingTxt", CCLabelIFTTF*, this->m_kingTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_officialsTxt1", CCLabelIF*, this->m_officialsTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_officialsTxt2", CCLabelIF*, this->m_officialsTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scNode", CCNode*, this->m_scNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_resTxt", CCLabelIF*, this->m_resTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameBtn", CCControlButton*, this->m_nameBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_modifyBtn", CCControlButton*, this->m_modifyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodBtn", CCControlButton*, this->m_woodBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodBtn", CCControlButton*, this->m_foodBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironBtn", CCControlButton*, this->m_ironBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneBtn", CCControlButton*, this->m_stoneBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_changeBtn", CCControlButton*, this->m_changeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceTxt", CCLabelIFTTF*, this->m_allianceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerTxt", CCLabelIFTTF*, this->m_playerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_oNode", CCNode*, this->m_oNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sNode", CCNode*, this->m_sNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_flagNode", CCNode*, this->m_flagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_flagArea", CCNode*, this->m_flagArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingNode", CCNode*, this->m_kingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bannerAni", CCNode*, this->m_bannerAni);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_palaceNode", CCNode*, this->m_palaceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_palaceAni", CCNode*, this->m_palaceAni);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_palaceArea", CCNode*, this->m_palaceArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_giftArea", CCNode*, this->m_giftArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_countryFlagNode", CCNode*, this->m_countryFlagNode);
    return false;
}

SEL_CCControlHandler ManagerKingdomView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeNameClick", ManagerKingdomView::onChangeNameClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onResClick1", ManagerKingdomView::onResClick1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onResClick2", ManagerKingdomView::onResClick2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onResClick3", ManagerKingdomView::onResClick3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onResClick4", ManagerKingdomView::onResClick4);
    return NULL;
}

void ManagerKingdomView::onChangeNameClick(CCObject *pSender, CCControlEvent event){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    PopupViewController::getInstance()->addPopupView(KingdomNameChangeView::create(this->getKing()));
}

void ManagerKingdomView::onModifyResClick(CCObject *pSender, CCControlEvent event){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    if(m_resIndex<0){
        //没有选择
        return ;
    }
    KingdomExecCommand* cmd = new KingdomExecCommand();
    cmd->putParam("type", CCInteger::create(CHANGE_RESOURCE_FLUSH));
    cmd->putParam("resType", CCInteger::create(m_resIndex));
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ManagerKingdomView::modifyOutputResSuccess), NULL));
    cmd->sendAndRelease();
    m_modifyBtn->setEnabled(false);
    GameController::getInstance()->showWaitInterface();
}

void ManagerKingdomView::modifyOutputResSuccess(CCObject* data){
    CCLog("modifyOutputResSuccess");
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    int index  = WorldController::getInstance()->getIndexByPoint(WorldController::getInstance()->selfPoint);
    if(dic && dic->objectForKey("points")){
        string points = dic->valueForKey("points")->getCString();
        Json* pointsJson = Json_create(points.c_str());
        if(pointsJson==NULL){
            return ;
        }
        int size = Json_getSize(pointsJson);
        for (int i=0; i<size; i++) {
            Json *item = Json_getItemAt(pointsJson, i);
            if(item){
                index = Json_getInt(item,"i",0);
                int type = Json_getInt(item,"t",0);
                CCLOG("index=%d  type=%d",index,type);
            }
        }
        Json_dispose(pointsJson);
    }
    WorldController::getInstance()->playAniTargetIndex = index;
    CCPoint pt = WorldController::getPointByIndex(index);
    PopupViewController::getInstance()->removeAllPopupView();
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt,true);
        WorldMapView::instance()->playFreshResAni(index);
    }else{
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
}

void ManagerKingdomView::onResClick1(CCObject *pSender, CCControlEvent event){
    CCLog("onResClick1");
    this->refreshRes(Wood);
}

void ManagerKingdomView::onResClick2(CCObject *pSender, CCControlEvent event){
    CCLog("onResClick2");
    this->refreshRes(Food);
}

void ManagerKingdomView::onResClick3(CCObject *pSender, CCControlEvent event){
    CCLog("onResClick3");
    this->refreshRes(Iron);
}

void ManagerKingdomView::onResClick4(CCObject *pSender, CCControlEvent event){
    CCLog("onResClick4");
    this->refreshRes(Stone);
}

void ManagerKingdomView::refreshRes(int index){
    switch (index) {
        case Wood:
            m_woodBtn->setEnabled(false);
            m_foodBtn->setEnabled(true);
            m_ironBtn->setEnabled(true);
            m_stoneBtn->setEnabled(true);
            m_resIndex = Wood;
            break;
        case Food:
            m_woodBtn->setEnabled(true);
            m_foodBtn->setEnabled(false);
            m_ironBtn->setEnabled(true);
            m_stoneBtn->setEnabled(true);
            m_resIndex = Food;
            break;
        case Iron:
            m_woodBtn->setEnabled(true);
            m_foodBtn->setEnabled(true);
            m_ironBtn->setEnabled(false);
            m_stoneBtn->setEnabled(true);
            m_resIndex = Iron;
            break;
        case Stone:
            m_woodBtn->setEnabled(true);
            m_foodBtn->setEnabled(true);
            m_ironBtn->setEnabled(true);
            m_stoneBtn->setEnabled(false);
            m_resIndex = Stone;
            break;
        default:
            m_woodBtn->setEnabled(true);
            m_foodBtn->setEnabled(true);
            m_ironBtn->setEnabled(true);
            m_stoneBtn->setEnabled(true);
            break;
    }
    auto gapTime = (m_chTime - WorldController::getInstance()->getTime())/1000;
    if(index>=0 && gapTime<0){
        m_modifyBtn->setEnabled(true);
    }else{
        m_modifyBtn->setEnabled(false);
    }
    
}