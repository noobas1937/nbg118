//
//  MainCityMoreView.cpp
//  IF
//
//  Created by fubin on 14-2-26.
//
//

#include "MainCityMoreView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "MainCityView.h"
#include "ArmyController.h"
#include "GeneralManager.h"
#include "SceneController.h"
#include "ImperialScene.h"

MainCityMoreView* MainCityMoreView::create(int buildId, int type){
    MainCityMoreView* ret = new MainCityMoreView();
    if(ret && ret->init(buildId, type)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MainCityMoreView::init(int buildId, int type)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_buildId = buildId;
    m_type = type;
    auto tmpCCB = CCBLoadFile("MainCityMoreView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int oldBgHeight = m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int newBgHeight = m_buildBG->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    }
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    updateInfo();
    
    return true;
}

void MainCityMoreView::updateInfo()
{
    int curNum = 0;
    int curStorage = 0;
    int outputPreHour = 0;
    int oriOutPutPreH = 0;
    int effect = 0;
    float sumTotalValue = 0;
    float effectNum = 0;
    float vipEffect = 0;
    float ablityEffect = 0;
    float scienceEffect = 0;
    
    int buildType = FUN_BUILD_FOOD;
    if (m_type == Wood) {
        curNum = GlobalData::shared()->resourceInfo.lWood;
        curStorage = GlobalData::shared()->resourceInfo.lStorageWood;
        outputPreHour = FunBuildController::getInstance()->WoodOutPreSed*3600;
        oriOutPutPreH = FunBuildController::getInstance()->oriWoodPreH;
        effect = 50;
        buildType = FUN_BUILD_WOOD;
    }
    else if (m_type == Stone) {
        curNum = GlobalData::shared()->resourceInfo.lStone;
        curStorage = GlobalData::shared()->resourceInfo.lStorageStone;
        outputPreHour = FunBuildController::getInstance()->StoneOutPreSed*3600;
        oriOutPutPreH = FunBuildController::getInstance()->oriStonePreH;
        effect = 51;
        buildType = FUN_BUILD_STONE;
    }
    else if (m_type == Iron) {
        curNum = GlobalData::shared()->resourceInfo.lIron;
        curStorage = GlobalData::shared()->resourceInfo.lStorageIron;
        outputPreHour = FunBuildController::getInstance()->IronOutPreSed*3600;
        oriOutPutPreH = FunBuildController::getInstance()->oriIronPreH;
        effect = 52;
        buildType = FUN_BUILD_IRON;
    }
    else if (m_type == Food) {
        curNum = GlobalData::shared()->resourceInfo.lFood;
        curStorage = GlobalData::shared()->resourceInfo.lStorageFood;
        outputPreHour = FunBuildController::getInstance()->FoodOutPreSed*3600;
        oriOutPutPreH = FunBuildController::getInstance()->oriFoodPreH;
        effect = 53;
        buildType = FUN_BUILD_FOOD;
    }
    
    effectNum = CCCommonUtils::getEffectValueByNum(effect);
    vipEffect = CCCommonUtils::getVipEffectValueByNum(effect);
    ablityEffect = GeneralManager::getInstance()->getValueByEffect(effect);
    scienceEffect = effectNum-vipEffect-ablityEffect;
    
    int _titleH = 150;
    int _itemH = 50;
    int curY = 0;
    int curX = 0;
    if (CCCommonUtils::isIosAndroidPad()) {
        _titleH = 270;
        _itemH = 80;
        curY = 0;
        curX = 0;
    }
    
    int buildIncrease = 0;
    vector<pair<int, int>> vecBuildLv;
    map<int,FunBuildInfo>::iterator it;
    it = FunBuildController::getInstance()->curBuildsInfo->begin();
    for (; it != FunBuildController::getInstance()->curBuildsInfo->end(); it++)
    {
        if (it->second.type == buildType) {
            double effectTime = it->second.effectTime;
            int gapTime = effectTime - GlobalData::shared()->getWorldTime();
            if(gapTime>0 && effectTime>0){
                int oriOneBuildOut = atoi( (it->second).para[1].c_str() );
                buildIncrease += oriOneBuildOut*(1+effectNum*1.0/100);
            }
            vecBuildLv.push_back(pair<int, int>(it->first,it->second.level));
        }
    }
    
    auto _comp = [](pair<int, int> p1, pair<int, int> p2){
        if (p1.second > p2.second) {
            return true;
        }
        return false;
    };
    sort(vecBuildLv.begin(), vecBuildLv.end(), _comp);
    for (int i=(vecBuildLv.size()-1); i>=0; i--) {
        int bid = vecBuildLv[i].first;
        auto cell = TmpBuildCell::create(bid, effectNum);
        cell->setTouchNode(m_infoList);
        cell->setPosition(ccp(curX+0, curY));
        m_scrollView->addChild(cell);
        curY += _titleH+10;
    }
    
    curY += 20;
    
    sumTotalValue = buildIncrease + oriOutPutPreH*effectNum*1.0/100;
    
    auto infoBg = CCLoadSprite::createScale9Sprite("UI_frame_shense.png");
    infoBg->setPosition(ccp(curX+0, curY-20));
    m_scrollView->addChild(infoBg);
    int infoBgHight = 0;
    
    string msg1 = "";
    string msg2 = "";
    if (m_type == -1) {
    }
    else {
        int offSetX = 120;
        if (CCCommonUtils::isIosAndroidPad()) {
            offSetX = 300;
        }
        if (1) {//建筑加成
            msg1 = _lang("102300");
            float value = buildIncrease;
            msg2 = CC_CMDITOA(value);
            auto cell = TmpInfoCell::create(msg1, msg2+"/h", value, sumTotalValue);
            cell->setPosition(ccp(curX+offSetX, curY));
            m_scrollView->addChild(cell);
            curY += _itemH;
            infoBgHight += _itemH;
        }
        
        if (1) {//技能加成
            msg1 = _lang("102109");
            float value = oriOutPutPreH*ablityEffect*1.0/100;
            msg2 = CC_CMDITOA(value);
            auto cell = TmpInfoCell::create(msg1, msg2+"/h", value, sumTotalValue);
            cell->setPosition(ccp(curX+offSetX, curY));
            m_scrollView->addChild(cell);
            curY += _itemH;
            infoBgHight += _itemH;
        }
        
        if (1) {//科技加成
            msg1 = _lang("102175");
            float value = oriOutPutPreH*scienceEffect*1.0/100;
            msg2 = CC_CMDITOA(value);
            auto cell = TmpInfoCell::create(msg1, msg2+"/h", value, sumTotalValue);
            cell->setPosition(ccp(curX+offSetX, curY));
            m_scrollView->addChild(cell);
            curY += _itemH;
            infoBgHight += _itemH;
        }
        
        if (1) {//vip加成
            msg1 = _lang("102189");
            float value = oriOutPutPreH*vipEffect*1.0/100;
            msg2 = CC_CMDITOA(value);
            auto cell = TmpInfoCell::create(msg1, msg2+"/h", value, sumTotalValue);
            cell->setPosition(ccp(curX+offSetX, curY));
            m_scrollView->addChild(cell);
            curY += _itemH;
            infoBgHight += _itemH;
        }
        
        if (1) {//总资源加成
            msg1 = _lang("102190");
            msg2 = CC_CMDITOA(sumTotalValue);
            auto cell = TmpMsgCell::create(msg1, msg2+"/h");
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(curX+offSetX-100, curY));
            }
            else
                cell->setPosition(ccp(curX+offSetX-50, curY));
            m_scrollView->addChild(cell);
            curY += _itemH-10;
            if (CCCommonUtils::isIosAndroidPad())
            {
                curY -= 20;
            }
            infoBgHight += _itemH-10;
        }
        
        if (1) {//原始产量
            msg1 = _lang("102304");
            msg2 = CC_CMDITOA(oriOutPutPreH);
            auto cell = TmpMsgCell::create(msg1, msg2+"/h");
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(curX+offSetX-100, curY));
            }
            else
                cell->setPosition(ccp(curX+offSetX-50, curY));
            m_scrollView->addChild(cell);
            curY += _itemH-10;
            infoBgHight += _itemH-10;
        }
    }
    
    curY += 10;
    auto resTitle = ResMoreInfoCell::create(m_type, "",oriOutPutPreH, sumTotalValue);
    if (CCCommonUtils::isIosAndroidPad()) {
        resTitle->setPosition(ccp(curX, curY));
    }
    else
        resTitle->setPosition(ccp(curX+50, curY));
    m_scrollView->addChild(resTitle);
    curY += _titleH;
    infoBgHight += _titleH;
    
    curY+= 20;
    if (CCCommonUtils::isIosAndroidPad()) {
        infoBg->setContentSize(CCSizeMake(1496, infoBgHight+80));
    }
    else
        infoBg->setContentSize(CCSizeMake(640, infoBgHight+40));
    
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
}

void MainCityMoreView::onEnter(){
    CCNode::onEnter();
}

void MainCityMoreView::onExit(){
    CCNode::onExit();
}

SEL_CCControlHandler MainCityMoreView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

bool MainCityMoreView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    return false;
}

void MainCityMoreView::doWhenClose(){
    PopupViewController::getInstance()->addPopupInView(MainCityView::create(FUN_BUILD_MAIN_CITY_ID));
}

////
ResMoreInfoCell* ResMoreInfoCell::create(int restype, string msg, int value, int effValue)
{
    auto ret = new ResMoreInfoCell();
    if (ret && ret->init(restype, msg, value, effValue)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ResMoreInfoCell::init(int restype, string msg, int value, int effValue)
{
    bool ret = true;
    CCBLoadFile("ResMoreInfoCell02",this,this);
    string resName = CCCommonUtils::getResourceNameByType(restype);
    m_nameLabel->setString(_lang_1("102134", resName.c_str()));
    
    string picName = CCCommonUtils::getResourceIconByType(restype);
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    if (pic) {
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(pic, 160, true);
        }
        else
            CCCommonUtils::setSpriteMaxSize(pic, 80, true);
        m_picNode->addChild(pic);
    }
    
    m_valueLabel->setString(CC_ITOA(value+effValue));
    m_effLabel->setString("");
//    if (effValue>0) {
//        string effStr = CC_ITOA(effValue);
//        m_effLabel->setString("+"+effStr);
//    }
    m_hLabel->setString("/h");
    m_effLabel->setPositionX(m_valueLabel->getPositionX()+m_valueLabel->getContentSize().width*m_valueLabel->getOriginScaleX());
    m_hLabel->setPositionX(m_effLabel->getPositionX()+m_effLabel->getContentSize().width*m_effLabel->getOriginScaleX());
    
    return ret;
}

void ResMoreInfoCell::onEnter() {
    CCNode::onEnter();
}
void ResMoreInfoCell::onExit() {
    CCNode::onExit();
}
SEL_CCControlHandler ResMoreInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool ResMoreInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueLabel", CCLabelIF*, m_valueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effLabel", CCLabelIF*, m_effLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hLabel", CCLabelIF*, m_hLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    return false;
}

////
TmpInfoCell* TmpInfoCell::create(string msg1, string msg2, float value, float sum)
{
    auto ret = new TmpInfoCell();
    if (ret && ret->init(msg1, msg2, value, sum)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TmpInfoCell::init(string msg1, string msg2, float value, float sum)
{
    bool ret = true;
    CCBLoadFile("TmpInfoCell",this,this);
    m_msg1Label->setString(msg1.c_str());
    m_msg2Label->setString(msg2.c_str());
    m_bar->setScaleX(0);
    if (sum>0) {
        float pro = value*1.0/sum;
        m_bar->setScaleX(pro);
    }
    return ret;
}

void TmpInfoCell::onEnter() {
    CCNode::onEnter();
}
void TmpInfoCell::onExit() {
    CCNode::onExit();
}
SEL_CCControlHandler TmpInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool TmpInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgFrame", CCScale9Sprite*, m_bgFrame);
    return false;
}

////
TmpMsgCell* TmpMsgCell::create(string msg1, string msg2)
{
    auto ret = new TmpMsgCell();
    if (ret && ret->init(msg1, msg2)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TmpMsgCell::init(string msg1, string msg2)
{
    bool ret = true;
    CCBLoadFile("TmpMsgCell",this,this);
    m_msg1Label->setString(msg1.c_str());
    m_msg2Label->setString(msg2.c_str());
    return ret;
}

void TmpMsgCell::onEnter() {
    CCNode::onEnter();
}
void TmpMsgCell::onExit() {
    CCNode::onExit();
}
SEL_CCControlHandler TmpMsgCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool TmpMsgCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    return false;
}

////
TmpBuildCell* TmpBuildCell::create(int buildItem, float effValue)
{
    auto ret = new TmpBuildCell();
    if (ret && ret->init(buildItem, effValue)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TmpBuildCell::init(int buildItem, float effValue)
{
    bool ret = true;
    m_buildId = buildItem;
    CCBLoadFile("TmpBuildCell",this,this);
    m_normalNode->setVisible(false);
    m_upNode->setVisible(false);
    m_effNode->setVisible(false);
    
    auto& info = FunBuildController::getInstance()->getFunbuildById(buildItem);
    string pic = info.pic+"_"+CC_ITOA(GlobalData::shared()->contryResType)+".png";
    auto icon = CCLoadSprite::createSprite(pic.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        icon->setScale(1.5);
    }
    m_iconNode->addChild(icon);
    
    if (info.state == FUN_BUILD_NORMAL) {
        double effectTime = info.effectTime;
        int gapTime = effectTime - GlobalData::shared()->getWorldTime();
        if(gapTime>0 && effectTime>0){
            m_efflvLabel->setString(_lang_1("102272", CC_ITOA(info.level)));
            string valueInfo = info.para[1];
            int addValue = atoi(valueInfo.c_str())*effValue*1.0/100;
            if (addValue>0) {
                valueInfo = "("+valueInfo;
                string addStr = CC_ITOA(addValue);
                m_effValue2Label->setString("+"+addStr);
                m_tmp1Label->setString(")");
            }
            m_effValueLabel->setString(valueInfo);
            m_hLabel->setString("/h");
            
            m_effValueLabel->setPositionX(m_effValue2Label->getPositionX()-m_effValue2Label->getContentSize().width*m_effValue2Label->getOriginScaleX());
            
            auto m_gainSpr = CCSprite::create();
            CCCommonUtils::makeEffectSpr(m_gainSpr, "Gain_%d.png", 16,0.1);
            m_gainSpr->setScale(2.0);
            m_gainSpr->setPosition(ccp(-90, -70));
            m_gainSpr->setOpacity(175);
            m_iconNode->addChild(m_gainSpr);
            
            m_effNode->setVisible(true);
        }
        else {
            m_lvLabel->setString(_lang_1("102272", CC_ITOA(info.level)));
            string valueInfo = info.para[1];
            int addValue = atoi(valueInfo.c_str())*effValue*1.0/100;
            if (addValue>0) {
                string addInfo = CC_ITOA(addValue);
                m_valueAddLabel->setString("+"+addInfo);
            }
            m_addHLabel->setString("/h");
            m_valueLabel->setString(valueInfo);
            m_valueLabel->setPositionX(m_valueAddLabel->getPositionX()-m_valueAddLabel->getContentSize().width*m_valueAddLabel->getOriginScaleX());
            m_normalNode->setVisible(true);
        }
    }
    else {//升级
        m_curLvLabel->setString(_lang_1("102272", CC_ITOA(info.level)));
        string nextLv = _lang_1("102272", CC_ITOA(info.level+1));
        m_nextLvLabel->setString("->"+nextLv);
        
        string nextParas = info.nextLevelParas;
        vector<string> nextVec;
        CCCommonUtils::splitString(nextParas, ",", nextVec);
        
        m_curValueLabel->setString(info.para[1]);
        m_nextValueLabel->setString("->"+nextVec[0]);
        
        auto upIcon = CCLoadSprite::createSprite("icon_build_up_2.png");
        upIcon->setPosition(ccp(-70, 0));
        m_iconNode->addChild(upIcon);
        
        m_upNode->setVisible(true);
    }
    return ret;
}

void TmpBuildCell::onEnter() {
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, true);
}
void TmpBuildCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}
SEL_CCControlHandler TmpBuildCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool TmpBuildCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_normalNode", CCNode*, m_normalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effNode", CCNode*, m_effNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueLabel", CCLabelIF*, m_valueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueAddLabel", CCLabelIF*, m_valueAddLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addHLabel", CCLabelIF*, m_addHLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curLvLabel", CCLabelIF*, m_curLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLvLabel", CCLabelIF*, m_nextLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curValueLabel", CCLabelIF*, m_curValueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextValueLabel", CCLabelIF*, m_nextValueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_efflvLabel", CCLabelIF*, m_efflvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effValueLabel", CCLabelIF*, m_effValueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effValue2Label", CCLabelIF*, m_effValue2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tmp1Label", CCLabelIF*, m_tmp1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hLabel", CCLabelIF*, m_hLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    return false;
}

void TmpBuildCell::setTouchNode(CCNode* touchNode)
{
    m_touchNode = touchNode;
}

bool TmpBuildCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_touchNode && isTouchInside(m_touchNode, pTouch) && isTouchInside(m_bg, pTouch)) {
        m_startY = pTouch->getLocation().y;
        return true;
    }
    return false;
}

void TmpBuildCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int endY = pTouch->getLocation().y;
    if (abs(endY-m_startY) >= 20) {
        return ;
    }
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if (layer) {
        layer->onMoveToBuildAndPlay(m_buildId, true);
        PopupViewController::getInstance()->removeAllPopupView();
    }
}