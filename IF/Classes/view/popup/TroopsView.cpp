//
//  TroopsView.cpp
//  IF
//
//  Created by fubin on 14-8-12.
//
//

#include "TroopsView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "MainCityView.h"
#include "ArmyController.h"
#include "TipsView.h"
#include "WorldController.h"
#include "AllianceIntroTip.h"
#include "MarchFormationView.h"
#include "TroopsController.h"
#define TROOPS_TIP_MSG "troops.tip.msg"
#define TROOP_TIP_TAG 10000
TroopsView* TroopsView::create(int type){
    TroopsView* ret = new TroopsView();
    if(ret && ret->init(type)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TroopsView::init(int type)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    m_type = type;
    auto tmpCCB = CCBLoadFile("MainCityMoreView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    if (CCCommonUtils::isIosAndroidPad()) {
        int newBgHeight = m_buildBG->getContentSize().height;
        int count = newBgHeight/100+1;
        for (int i = 0; i<=count; i++) {
            auto pic = CCLoadSprite::createSprite("technology_09.png");
            pic->setPositionY(-i*100);
            pic->setScaleX(2.4);
            m_BGNode->addChild(pic);
        }
    }
    else {
        int oldBgHeight = m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int newBgHeight = m_buildBG->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
        // m_BGNode->setPositionY(m_buildBG->getPositionY()-50);
        int count = newBgHeight/100+1;
        for (int i = 0; i<=count; i++) {
            auto pic = CCLoadSprite::createSprite("technology_09.png");
            pic->setPositionY(-i*100);
            m_BGNode->addChild(pic);
        }
    }
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    updateInfo();
    
    return true;
}

void TroopsView::updateInfo()
{
    int _titleH = 150;
    int _itemH = 65;
    int curY = 0;
    int curX = 8;
    if (CCCommonUtils::isIosAndroidPad()) {
        _titleH = 150;
        _itemH = 65;
        curY = 0;
        curX = 15;
    }
    else {
        _titleH = 150;
        _itemH = 65;
        curY = 0;
        curX = 8;
    }
    string msg1 = "";
    string msg2 = "";
    
    if (true) {
        /*总维护费
         陷阱数： 102142 Traps： 陷阱：
         陷阱容量： 102185 Trap Capacity： 陷阱容量：
         102186 Hospitalized: 伤兵数：
         然后起一个标题（ 102187 My Troops 我的军队 ）里面显示玩家有哪些士兵，每种士兵有几个
         再一个标题（102188 My Traps 我的陷阱）里面显示玩家有哪些陷阱，每种有几个*/
        
        int fortTotal = 0;
//        //有哪些陷阱，每种多少个
        map<std::string, ArmyInfo>::iterator it;
        for(it = GlobalData::shared()->fortList.begin(); it != GlobalData::shared()->fortList.end(); it++){
            if(it->second.free>0){
                fortTotal += it->second.free;
                break;
            }
        }
        if(fortTotal>0) {
            auto itemBuild = TroopTotalTmpInfoCell::create("","",1);
            itemBuild->setPosition(ccp(20, curY));
            if (CCCommonUtils::isIosAndroidPad()) {
                itemBuild->setPosition(ccp(50, curY));
            }
            m_scrollView->addChild(itemBuild);
            curY+=itemBuild->getContentSize().height;
//            curY += 20;
//            auto itemBuild = ResUpTitleCell::create(_lang("102188"));
//            itemBuild->setPosition(ccp(curX+20, curY));
//            m_scrollView->addChild(itemBuild);
//            curY += _itemH-20;
        }
        
        //有哪些兵
        int freeNum = 0;
        for(it = GlobalData::shared()->armyList.begin(); it != GlobalData::shared()->armyList.end(); it++){
            if(it->second.free>0 || it->second.march>0){
                freeNum += (it->second.free+it->second.march);
                break;
            }
        }
        if(freeNum>0){
            curY += 20;
            auto itemBuild = TroopTotalTmpInfoCell::create("","",0);
            itemBuild->setPosition(ccp(20, curY));
            if (CCCommonUtils::isIosAndroidPad()) {
                itemBuild->setPosition(ccp(50, curY));
            }
            m_scrollView->addChild(itemBuild);
            curY+=itemBuild->getContentSize().height;
        }
//        if(freeNum>0) {
//            auto itemLac = TroopTmpInfoCell::create(_lang("105557"),_lang("103606"),0,_lang("105558"));
//            itemLac->setPosition(ccp(curX, curY));
//            m_scrollView->addChild(itemLac);
//            curY += _itemH;
//            
//            curY += 20;
//            auto itemBuild = ResUpTitleCell::create(_lang("102187"));
//            itemBuild->setPosition(ccp(curX+20, curY));
//            m_scrollView->addChild(itemBuild);
//            curY += _itemH-20;
//        }

    }
    
    curY += 5;
    curX = 0;
    auto resTitle = TroopMoreInfoCell::create(-1, "");
    resTitle->setPosition(ccp(curX, curY));
    m_scrollView->addChild(resTitle);
    if (CCCommonUtils::isIosAndroidPad()) {
        if (FunBuildController::getInstance()->getMainCityLv() >= 15 && GlobalData::shared()->march_formation_switch ==2) {
            curY += 740;
        }
        else
            curY += 610;
    }
    else {
        if (FunBuildController::getInstance()->getMainCityLv() >= 15 && GlobalData::shared()->march_formation_switch == 2) {
            curY += 315;
        }
        else
            curY += 255;
    }
    
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
}

void TroopsView::showGuideTip(int index){
    switch (index) {
        case 1:{
            if(!m_tip1Pos.equals(CCPointZero)){
                AllianceIntroTip* introTip= AllianceIntroTip::createAutoAdjust(AllianceIntroTip_Top, _lang("102110"),m_infoList->getContentSize().width * 0.7,-80);
                introTip->setTag(TROOP_TIP_TAG+1);
                if (CCCommonUtils::isIosAndroidPad())
                {
                    introTip->setScale(1.25);
                }
                addChild(introTip);
                m_tip1Pos = this->convertToNodeSpace(m_tip1Pos);
                introTip->setPosition(m_tip1Pos);
            }
            break;
        }
        case 2:{
            removeChildByTag(TROOP_TIP_TAG+1);
            if(!m_tip2Pos.equals(CCPointZero)){
                AllianceIntroTip* introTip= AllianceIntroTip::createAutoAdjust(AllianceIntroTip_Bottom,_lang("102150"),m_infoList->getContentSize().width  * 0.7,-80);
                introTip->setTag(TROOP_TIP_TAG+2);
                addChild(introTip);
                if (CCCommonUtils::isIosAndroidPad())
                {
                    introTip->setScale(1.25);
                }
                m_tip2Pos = this->convertToNodeSpace(m_tip2Pos);
                introTip->setPosition(m_tip2Pos);
            }
            break;
        }
        default:
            break;
    }
    m_tipStep = index;
}
void TroopsView::changeGuideTip(CCObject* ccObj){
    if(m_tipStep>0 || !ccObj)
        return;
    CCArray *ccArray = dynamic_cast<CCArray*>(ccObj);
    if(ccArray){
        if(ccArray->count()>3){
            CCInteger* intTmp1 = dynamic_cast<CCInteger*>(ccArray->objectAtIndex(0));
            CCInteger* intTmp2 = dynamic_cast<CCInteger*>(ccArray->objectAtIndex(1));
            if(intTmp1 && intTmp2){
                m_tip1Pos = ccp(intTmp1->getValue(),intTmp2->getValue());
            }
            intTmp1 = dynamic_cast<CCInteger*>(ccArray->objectAtIndex(2));
            intTmp2 = dynamic_cast<CCInteger*>(ccArray->objectAtIndex(3));
            
            if(intTmp1 && intTmp2){
                m_tip2Pos = ccp(intTmp1->getValue(),intTmp2->getValue());
            }
        }
        m_scrollView->setTouchEnabled(false);
        showGuideTip(1);
    }
}
bool TroopsView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    return true;
}
void TroopsView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(m_tipStep>0){
        if(m_tipStep==1){
            showGuideTip(2);
        }else if(m_tipStep==2){
            removeChildByTag(TROOP_TIP_TAG+2);
            m_tipStep=0;
            m_tip1Pos=CCPointZero;
            m_tip2Pos=CCPointZero;
            m_scrollView->setTouchEnabled(true);
        }
    }
}
void TroopsView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}

void TroopsView::onEnter(){
    CCNode::onEnter();
   // CCDirector::sharedDirector()->getEventDispatcher()->addTargetedDelegate(this, 1, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this ,callfuncO_selector(TroopsView::changeGuideTip),TROOPS_TIP_MSG , NULL);
    setTitleName(_lang("108724"));
}

void TroopsView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TROOPS_TIP_MSG);
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler TroopsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

bool TroopsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    return false;
}

////
TroopMoreInfoCell* TroopMoreInfoCell::create(int restype, string msg)
{
    auto ret = new TroopMoreInfoCell();
    if (ret && ret->init(restype, msg)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TroopMoreInfoCell::init(int restype, string msg)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(504, false);
    });
    CCBLoadFile("ResMoreInfoCell",this,this);
//    CCCommonUtils::setButtonTitle(m_formationBtn1, _lang("103681").c_str());
//    CCCommonUtils::setButtonTitle(m_formationBtn2, _lang("103682").c_str());
    string msg2 = "";
    //伤病数
    if(1) {
        map<std::string, TreatInfo>::iterator Tit;
        int deadNum = 0;
        for(Tit = GlobalData::shared()->treatList.begin(); Tit != GlobalData::shared()->treatList.end(); Tit++){
            deadNum += Tit->second.dead;
        }
        msg2 = CC_CMDITOA(deadNum) + "/" + CC_CMDITOA(ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY));
        m_troopText5->setString(_lang("102186"));
        m_troopNumText5->setString(msg2);
    }
    //陷阱容量
    if(1) {
        msg2 = CC_CMDITOA(ArmyController::getInstance()->getTotalFortNum()) +"/"+ CC_CMDITOA( ArmyController::getInstance()->getMaxNumByType(FORT) );
        m_troopText4->setString(_lang("102185"));
        m_troopNumText4->setString(msg2);
    }
    //总维护费
    if (1) {
        msg2 = CC_CMDITOA(ArmyController::getInstance()->getTotalUpKeep());
        m_troopText3->setString(_lang("102125"));
        m_troopNumText3->setString(msg2 + "/h");
    }
    
    if (1) {//总行军数
        int machingNum = WorldController::getInstance()->getMaxMarchCount();
        int nowNum = machingNum-WorldController::getInstance()->getCurrentMarchCount();
        msg2 = CC_ITOA(nowNum) + std::string("/") + CC_ITOA(machingNum);
        m_troopText2->setString(_lang("102141"));
        m_troopNumText2->setString(msg2);
    }
    
    if (1) {//总兵力数
        msg2 = CC_CMDITOA(ArmyController::getInstance()->getTotalArmy());
        m_troopText1->setString(_lang("102149"));
        m_troopNumText1->setString(msg2);
    }

    if (FunBuildController::getInstance()->getMainCityLv() >= 15 && GlobalData::shared()->march_formation_switch == 2) {
        m_formationBtn1->setVisible(true);
        m_formationBtn2->setVisible(true);
        m_formationBtn1->setEnabled(true);
        m_formationBtn2->setEnabled(true);
    }
    else {
        m_formationBtn1->setVisible(false);
        m_formationBtn2->setVisible(false);
        m_formationBtn1->setEnabled(false);
        m_formationBtn2->setEnabled(false);
        m_moveNode->setPositionY(m_moveNode->getPositionY() - 60);
    }
    int total = 0;
    map<std::string, ArmyInfo>::iterator it;
    for(it = GlobalData::shared()->armyList.begin(); it != GlobalData::shared()->armyList.end(); it++){
        if(it->second.free>0 || it->second.march>0){
            total += (it->second.free+it->second.march);
            break;
        }
    }
    if (total <= 0) {
        m_formationBtn1->setVisible(false);
        m_formationBtn2->setVisible(false);
        m_formationBtn1->setEnabled(false);
        m_formationBtn2->setEnabled(false);
    }
    CCLoadSprite::doLoadResourceAsync(GENERAL_PATH, CCCallFuncO::create(this, callfuncO_selector(TroopMoreInfoCell::asyDelayLoad), NULL), 1);
    CCLoadSprite::doLoadResourceAsync(GENERAL_PATH, CCCallFuncO::create(this, callfuncO_selector(TroopMoreInfoCell::asyDelayLoad), NULL), 2);
    CCLoadSprite::doLoadResourceAsync(GENERAL_PATH, CCCallFuncO::create(this, callfuncO_selector(TroopMoreInfoCell::asyDelayLoad), NULL), 3);
    return ret;
}
void TroopMoreInfoCell::asyDelayLoad(CCObject * p){
    string pic = GlobalData::shared()->playerInfo.pic;
    pic+="_bust.png";
    auto picspr = CCLoadSprite::createSprite(pic.c_str());
    picspr->setScale(0.4);
    if (CCCommonUtils::isIosAndroidPad()) {
        picspr->setScale(1.0);
    }
    m_headPicNode->addChild(picspr);
    
}

void TroopMoreInfoCell::onEnter() {
    CCNode::onEnter();
}
void TroopMoreInfoCell::onExit() {
    CCNode::onExit();
}

void TroopMoreInfoCell::onFormation1Click(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(MarchFormationView::create(1));
}
void TroopMoreInfoCell::onFormation2Click(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(MarchFormationView::create(2));
}
SEL_CCControlHandler TroopMoreInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation1Click", TroopMoreInfoCell::onFormation1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation2Click", TroopMoreInfoCell::onFormation2Click);
    return NULL;
}
bool TroopMoreInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopText1", CCLabelIF*, m_troopText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopText2", CCLabelIF*, m_troopText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopText3", CCLabelIF*, m_troopText3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopText4", CCLabelIF*, m_troopText4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopText5", CCLabelIF*, m_troopText5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopNumText1", CCLabelIF*, m_troopNumText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopNumText2", CCLabelIF*, m_troopNumText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopNumText3", CCLabelIF*, m_troopNumText3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopNumText4", CCLabelIF*, m_troopNumText4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopNumText5", CCLabelIF*, m_troopNumText5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headPicNode", CCNode*, m_headPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn1", CCControlButton*, m_formationBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn2", CCControlButton*, m_formationBtn2);
    return false;
}

////
TroopTmpInfoCell* TroopTmpInfoCell::create(string msg1, string msg2, string msg3,string path1,string path2,string path3,string path4,int type,string msg4,string path5)
{
    auto ret = new TroopTmpInfoCell();
    if (ret && ret->init(msg1, msg2, msg3, path1,path2,path3,path4,type,msg4,path5)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TroopTmpInfoCell::init(string msg1, string msg2, string msg3,string path1,string path2,string path3,string path4,int type,string msg4,string path5)
{
    bool ret = true;
    CCBLoadFile("TroopsTmpInfoCell",this,this);
    if (type == 0)
    {
        initArmyData(msg1, msg2, msg3, path1, path2, path3, path4, msg4, path5);
    }
    else
    {
        initfortData(msg1, msg2, msg3, path1, path2, path3, path4, msg4);
    }
    
    return ret;
}

void TroopTmpInfoCell::initfortData(string msg1, string msg2, string msg3,string path1,string path2,string path3,string path4,string msg4)
{
    m_fortNode->setVisible(true);
    m_armyNode->setVisible(false);
    m_msg1Label->setString(msg1.c_str());
    m_msg2Label->setString(msg2.c_str());
    m_msg3Label->setString(msg3.c_str());
    if(path1!=""){
        auto pic  = CCLoadSprite::createSprite(path1.c_str());
        m_picHead->addChild(pic);
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic, 190);
        }
        else
            CCCommonUtils::setSpriteMaxSize(pic, 90);
    }
    if(path2!=""){
        auto pic1  = CCLoadSprite::createSprite(path2.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic1, 190);
        }
        m_levelSprNode->addChild(pic1);
    }else{
        m_levelSprNode->setVisible(false);
    }
    if(path3!=""){
        auto pic3  = CCLoadSprite::createSprite(path3.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic3, 190);
        }
        m_picNode1->addChild(pic3);
    }
    if(path4!=""){
        auto pic4  = CCLoadSprite::createSprite(path4.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic4, 190);
        }
        m_picNode2->addChild(pic4);
    }
}

void TroopTmpInfoCell::initArmyData(string msg1, string msg2, string msg3,string path1,string path2,string path3,string path4,string msg4,string path5)
{
    m_fortNode->setVisible(false);
    m_armyNode->setVisible(true);
    string footStr = /*_lang("105101") +*/ msg2;
    m_footNumTTF->setString(footStr.c_str());
    string marchStr = /*_lang("105103") +*/ msg4;
    m_marchNumTTF->setString(marchStr);
//    string freeStr = _lang("105102") + msg3;
//    m_freeNumTTF->setString(freeStr.c_str());
    m_nameTTF->setString(msg1.c_str());
    if(path1!=""){
        auto pic  = CCLoadSprite::createSprite(path1.c_str());
        m_picHead->addChild(pic);
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic, 190);
        }
        else
            CCCommonUtils::setSpriteMaxSize(pic, 90);
    }
    if(path2!=""){
        auto pic1  = CCLoadSprite::createSprite(path2.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic1, 190);
        }
        m_levelSprNode->addChild(pic1);
    }else{
        m_levelSprNode->setVisible(false);
    }
    if(path3!=""){
        auto pic3  = CCLoadSprite::createSprite(path3.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic3, 190);
        }
        m_freeNode->addChild(pic3);
    }
    if(path4!=""){
        auto pic4  = CCLoadSprite::createSprite(path4.c_str());
        pic4->setScale(0.8);
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic4, 190);
        }
        m_foodPicNode->addChild(pic4);
    }
    if (path5!="")
    {
        auto pic5  = CCLoadSprite::createSprite(path5.c_str());
//        pic5->setScale(0.8);
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic5, 190);
        }
        m_marchNode->addChild(pic5);
    }
}

void TroopTmpInfoCell::onEnter() {
    CCNode::onEnter();
}
void TroopTmpInfoCell::onExit() {
    CCNode::onExit();
}
SEL_CCControlHandler TroopTmpInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool TroopTmpInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg3Label", CCLabelIF*, m_msg3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelSprNode", CCNode*, m_levelSprNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picHead", CCNode*, m_picHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fortNode", CCNode*, m_fortNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armyNode", CCNode*, m_armyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodPicNode", CCNode*, m_foodPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeNode", CCNode*, m_freeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchNode", CCNode*, m_marchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTTF", CCLabelIF*, m_nameTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_footNumTTF", CCLabelIF*, m_footNumTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeNumTTF", CCLabelIF*, m_freeNumTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchNumTTF", CCLabelIF*, m_marchNumTTF);
    
    return false;
}
////
TroopTotalTmpInfoCell* TroopTotalTmpInfoCell::create(string msg1, string msg2, int type, string msg3)
{
    auto ret = new TroopTotalTmpInfoCell();
    if (ret && ret->init(msg1, msg2, type, msg3)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TroopTotalTmpInfoCell::init(string msg1, string msg2, int type, string msg3)
{
    bool ret = true;
    CCBLoadFile("TroopsTotalTmpInfoCell",this,this);
    float dy = 0;
    int count = 0;
    if (type == 0) {//兵种
        m_troopTitleText->setString(_lang("102187"));
        map<std::string, ArmyInfo>::iterator it;
        for(it = GlobalData::shared()->armyList.begin(); it != GlobalData::shared()->armyList.end(); it++){
            if(it->second.free>0 || it->second.march>0){
            int totalArmy = it->second.march + it->second.free;
               string  msg10 = CCCommonUtils::getNameById(it->second.armyId);
                string msg20 = CC_CMDITOA(it->second.free) + "/" + CC_CMDITOA(totalArmy);
                string msg30 = CC_CMDITOA(ArmyController::getInstance()->getUpKeep(&(it->second))) + "/" + CC_ITOA_K(GlobalData::shared()->resourceInfo.lFood);
                string msg40 = CC_CMDITOA(it->second.march) + "/" + CC_CMDITOA(totalArmy);
                std::string icon = it->second.getHeadIcon();
                
                string path = it->second.armyId.substr(it->second.armyId.size()-2);
                int num  = atoi(path.c_str())+1;
                path = "Roman_";
                path.append(CC_ITOA(num));
                path.append(".png");
//                auto cell = TroopTmpInfoCell::create(msg10, msg30,msg20,icon,path,"icon-cunhuo.png","icon-xiaohao.png",type,msg40,"arm2_icon.png");
                auto cell = TroopTmpInfoCell::create(msg10, msg30,msg20,icon,path,"","ui_food.png",type,msg40,"icon-cunhuo.png");
                m_listNode->addChild(cell);
                cell->setPositionY(dy);
                if (CCCommonUtils::isIosAndroidPad()) {
                    dy += 250;
                }
                else
                    dy += 127;
                count++;
            }
        }
        m_btnTip->setVisible(true);
    }
    else {
        m_btnTip->setVisible(false);
        m_troopTitleText->setString(_lang("102188"));
        map<std::string, ArmyInfo>::iterator it;
        for(it = GlobalData::shared()->fortList.begin(); it != GlobalData::shared()->fortList.end(); it++){
            if(it->second.free>0){
                string  msg10 = CCCommonUtils::getNameById(it->second.armyId);
                string msg20 = CC_CMDITOA(it->second.free);
 
                std::string icon = it->second.getBodyIcon();
                auto cell = TroopTmpInfoCell::create(msg10, "",msg20,icon,"","icon-cunhuo.png","",type);
                m_listNode->addChild(cell);
                cell->setPositionY(dy);
                if (CCCommonUtils::isIosAndroidPad()) {
                    dy += 250;
                }
                else
                    dy += 127;
                count++;
            }
        }
    }
    int dh = 0;
    if(count>1){
        dh = 127*(count-1);
        if (CCCommonUtils::isIosAndroidPad()) {
            dh = 250*(count-1);
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            m_bg1->setContentSize(CCSizeMake(m_bg1->getContentSize().width, dh+400));
            m_bg2->setContentSize(CCSizeMake(m_bg2->getContentSize().width, dh+350));
            m_bg3->setContentSize(CCSizeMake(m_bg3->getContentSize().width, dh+350));
            m_bg4->setContentSize(CCSizeMake(m_bg4->getContentSize().width, dh+350));
        }
        else {
            m_bg1->setContentSize(CCSizeMake(m_bg1->getContentSize().width, dh+265));
            m_bg2->setContentSize(CCSizeMake(m_bg2->getContentSize().width, dh+220));
            m_bg3->setContentSize(CCSizeMake(m_bg3->getContentSize().width, dh+220));
            m_bg4->setContentSize(CCSizeMake(m_bg4->getContentSize().width, dh+220));
        }
        m_titleBG->setPositionY(m_titleBG->getPositionY()+dh);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_titleNode->setPositionY(m_titleNode->getPositionY()+dh);
        }
        else
            m_titleNode->setPositionY(dh);
    }else{
        if (CCCommonUtils::isIosAndroidPad()) {
            m_bg1->setContentSize(CCSizeMake(m_bg1->getContentSize().width, 400));
            m_bg2->setContentSize(CCSizeMake(m_bg2->getContentSize().width, dh+350));
            m_bg3->setContentSize(CCSizeMake(m_bg3->getContentSize().width, dh+350));
            m_bg4->setContentSize(CCSizeMake(m_bg4->getContentSize().width, dh+350));
        }
        else {
            m_bg1->setContentSize(CCSizeMake(m_bg1->getContentSize().width, 265));
            m_bg2->setContentSize(CCSizeMake(m_bg2->getContentSize().width, dh+220));
            m_bg3->setContentSize(CCSizeMake(m_bg3->getContentSize().width, dh+220));
            m_bg4->setContentSize(CCSizeMake(m_bg4->getContentSize().width, dh+220));
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            m_titleNode->setPositionY(m_titleNode->getPositionY());
            m_titleBG->setPositionY(m_titleBG->getPositionY());
        }
        else {
            m_titleNode->setPositionY(0);
            m_titleBG->setPositionY(-7.6);
        }
    }
    m_btnTip->setPositionY(m_titleNode->getPositionY());
    this->setContentSize(m_bg1->getContentSize());
    return ret;
}

void TroopTotalTmpInfoCell::onEnter() {
    CCNode::onEnter();
}
void TroopTotalTmpInfoCell::onExit() {
    CCNode::onExit();
}
void TroopTotalTmpInfoCell::onHelpBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    CCPoint titlePos = m_titleNode->getPosition();
    CCPoint pos = m_titleNode->getParent()->convertToWorldSpace(titlePos-ccp(0,70));
    CCPoint pos1 = m_titleNode->getParent()->convertToWorldSpace(titlePos-ccp(0,160));
    CCArray *array = CCArray::create();
    array->addObject(CCInteger::create(pos.x));
    array->addObject(CCInteger::create(pos.y));
    array->addObject(CCInteger::create(pos1.x));
    array->addObject(CCInteger::create(pos1.y));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TROOPS_TIP_MSG,array);
}
SEL_CCControlHandler TroopTotalTmpInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", TroopTotalTmpInfoCell::onHelpBtnClick);
    return NULL;
}
bool TroopTotalTmpInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopTitleText", CCLabelIF*, m_troopTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCScale9Sprite*, m_bg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg2", CCScale9Sprite*, m_bg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg3", CCScale9Sprite*, m_bg3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg4", CCScale9Sprite*, m_bg4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleBG", CCScale9Sprite*, m_titleBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTip", CCControlButton*, m_btnTip);
    return false;
}