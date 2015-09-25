//
//  EnemyInfoMoreView.cpp
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#include "EnemyInfoMoreView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "EnemyInfoController.h"
#include "GeneralSkillListPopUpView.h"
#include "BuildUpgradeView.h"

EnemyInfoMoreView* EnemyInfoMoreView::create(int keyId, int buildId){
    EnemyInfoMoreView* ret = new EnemyInfoMoreView();
    if(ret && ret->init(keyId,buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EnemyInfoMoreView::init(int keyId, int buildId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    m_keyId = keyId;
    m_buildId = buildId;
    auto tmpCCB = CCBLoadFile("EnemyInfoMoreView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    updateInfo();
    return true;
}

void EnemyInfoMoreView::updateInfo()
{
    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    string para1 = m_info.para[1];
    //    m_titleLabel->setString(_lang(m_info.name).c_str());
    
    EnemyInfoController::getInstance()->checkEnemyData();
    EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[m_keyId];
    
//    Para1参数的值	值的意义
//    1	行军者的名称
//    2	行军者的坐标
//    3	行军的类型：攻击、贸易
//    4	抵达时间
//    5	总兵力
//    6	玩家等级
    
//    7 兵种类别：包含哪些兵种
//    8 各种类别兵种的具体数量
    
//    9	行军者的战斗力
//    10	科技
//    11	行军者的战斗科技类别和各自等级
    para1 = "1;2;3;4;5;6;7;8;9;10;11;12";
    std::vector<std::string> showItems;
    CCCommonUtils::splitString(para1, ";", showItems);
    
    int _titleH = 40;
    int _itemH = 70;
    int curY = 40;
    int curX = 10;
    vector<string>::iterator result = find(showItems.begin(), showItems.end(), "11");
    if (result != showItems.end()) {//能力和技能
//        string tmpInfo = "";
//        for (int i=0; i<info.ablityTypes.size(); i++) {
//            int talentType = info.ablityTypes[i];
//            tmpInfo = tmpInfo + CC_ITOA(talentType)+"|"+CC_ITOA(talentType)+",";
//        }
//        tmpInfo = tmpInfo.substr(0, tmpInfo.length()-1);
//        auto cell = EnemyMsgCell::create(11, tmpInfo, m_keyId);
//        cell->setPosition(ccp(curX, curY));
//        m_scrollView->addChild(cell);
//        curY += _itemH;
        
        int tmpAddX = 80;
        for (int i=0; i<info.ablityTypes.size(); i++) {
            int talentType = info.ablityTypes[i];
            string picName = CCCommonUtils::getIcon(CC_ITOA(talentType));
            auto msgCell = ItemMsgCell::create(picName, CC_ITOA(talentType), 11, m_keyId);
            
            msgCell->setPosition(ccp(curX+80+tmpAddX*i, curY));
            m_scrollView->addChild(msgCell);
        }
        curY += 70;
        
        string tmpInfo = _lang_1("108603", "");
        auto cell = EnemyMsgCell::create(5, tmpInfo, m_keyId);
        cell->setPosition(ccp(curX, curY));
        m_scrollView->addChild(cell);
        curY += _itemH;
    }
    
    result = find(showItems.begin(), showItems.end(), "10");
    if (result != showItems.end()) {//科技
        
        map<int,int>::iterator it1 = info.science.begin();
        for (; it1!=info.science.end(); it1++) {
            int scienceType = it1->first;
            int scienceLv = it1->second;
            
            string scienceName = CCCommonUtils::getNameById(CC_ITOA(scienceType));
            string slv = CC_ITOA(scienceLv);
            auto msgCell2 = ItemMsgCell2::create(scienceName, slv);
            
            msgCell2->setPosition(ccp(curX+108, curY));
            m_scrollView->addChild(msgCell2);
            curY += 45;
        }
        
        string tmpInfo = _lang_1("108604", "");
        auto cell = EnemyMsgCell::create(5, tmpInfo, m_keyId);
        cell->setPosition(ccp(curX, curY));
        m_scrollView->addChild(cell);
        curY += _itemH;
    }
    
    result = find(showItems.begin(), showItems.end(), "9");
    if (result != showItems.end()) {//战斗力
        string tmpInfo = _lang_1("108602",CC_ITOA(info.power));
        
        auto cell = EnemyMsgCell::create(9, tmpInfo, m_keyId);
        cell->setPosition(ccp(curX, curY));
        m_scrollView->addChild(cell);
        curY += _itemH;
    }
    
    result = find(showItems.begin(), showItems.end(), "7");
    if (result != showItems.end()) {//兵
        vector<string>::iterator tmpIt = find(showItems.begin(), showItems.end(), "8");
        
        map<int,int>::iterator it1 = info.army.begin();
        for (; it1!=info.army.end(); it1++) {
            int armyType = it1->first;
            int armyCnt = it1->second;
            
            string soldierName = CCCommonUtils::getNameById(CC_ITOA(armyType));
            string num = CC_ITOA(armyCnt);
            auto msgCell2 = ItemMsgCell2::create(soldierName, num);
            
            msgCell2->setPosition(ccp(curX+108, curY));
            m_scrollView->addChild(msgCell2);
            curY += 45;
        }
    }
    
    result = find(showItems.begin(), showItems.end(), "5");
    if (result != showItems.end()) {//总兵力
        string tmpInfo = _lang_1("108600",CC_CMDITOA(info.soldiers).c_str() );
        
        auto cell = EnemyMsgCell::create(5, tmpInfo, m_keyId);
        cell->setPosition(ccp(curX, curY));
        m_scrollView->addChild(cell);
        curY += _itemH;
    }
    
    result = find(showItems.begin(), showItems.end(), "6");
    if (result != showItems.end()) {//玩家等级
        string tmpInfo = _lang_1("102272",CC_ITOA(info.level));
        
        auto cell = EnemyMsgCell::create(6, tmpInfo, m_keyId);
        cell->setPosition(ccp(curX, curY));
        m_scrollView->addChild(cell);
        curY += _itemH;
    }
    
    result = find(showItems.begin(), showItems.end(), "4");
    if (result != showItems.end()) {//抵达时间
        string tmpInfo = _lang_1("108599",CCCommonUtils::timeStampToDate(info.stampTime).c_str());
        
        auto cell = EnemyMsgCell::create(4, tmpInfo, m_keyId);
        cell->setPosition(ccp(curX, curY));
        m_scrollView->addChild(cell);
        curY += _itemH;
    }
    
    result = find(showItems.begin(), showItems.end(), "3");
    if (result != showItems.end()) {//行军类型
        string strTypeName = "";
        if (info.type == ENEMY_TYPE_BATTLE) {
            strTypeName = _lang("108607");
        }
        else if (info.type == ENEMY_TYPE_SCOUT) {
            strTypeName = _lang("108605");
        }
        else if (info.type == ENEMY_TYPE_TRADE) {
            strTypeName = _lang("108606");
        }
        else if (info.type == ENEMY_TYPE_TEAM) {
            strTypeName = _lang("108608");
        }
        else if (info.type == ENEMY_TYPE_MEMBER) {
            strTypeName = _lang("108609");
        }
        string tmpInfo = _lang_1("108598",strTypeName.c_str());
        
        auto cell = EnemyMsgCell::create(3, tmpInfo, m_keyId);
        cell->setPosition(ccp(curX, curY));
        m_scrollView->addChild(cell);
        curY += _itemH;
    }
    
    result = find(showItems.begin(), showItems.end(), "2");
    if (result != showItems.end()) {//坐标
        string tmpInfo = _lang_1("108597", info.xy.c_str());
        auto cell = EnemyMsgCell::create(2, tmpInfo, m_keyId);
        cell->setPosition(ccp(curX, curY));
        m_scrollView->addChild(cell);
        curY += _itemH;
    }
    
    result = find(showItems.begin(), showItems.end(), "1");
    if (result != showItems.end()) {//玩家名称
        string tmpInfo = _lang_2("108595","","");
        tmpInfo = tmpInfo + info.name;
        
        auto cell = EnemyMsgCell::create(1, tmpInfo, m_keyId);
        cell->setPosition(ccp(curX, curY));
        m_scrollView->addChild(cell);
        curY += _itemH;
    }
    auto itemBuild = ResUpTitleCell::create(_lang_1("108592", info.name.c_str()));
    itemBuild->setPosition(ccp(curX, curY));
    m_scrollView->addChild(itemBuild);
    curY += _titleH;
    
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
}

void EnemyInfoMoreView::onEnter()
{
    CCLoadSprite::doResourceByCommonIndex(105, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(105, false);
        
    });
    CCNode::onEnter();
}

void EnemyInfoMoreView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler EnemyInfoMoreView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool EnemyInfoMoreView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    return false;
}

EnemyMsgCell* EnemyMsgCell::create(int type, string msg, int idx)
{
    auto ret = new EnemyMsgCell();
    if (ret && ret->init(type, msg, idx)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EnemyMsgCell::init(int type, string msg, int idx)
{
    m_type = type;
    m_msg = msg;
    m_idx = idx;
    
    pic = CCLoadSprite::createSprite("chuzheng_reward.png");//bg_item_cell.png
    pic->setAnchorPoint(ccp(0,0.5));
    this->addChild(pic);
    
    auto tmpLabel = CCLabelIF::create(m_msg.c_str(), 24);
    tmpLabel->setAnchorPoint(ccp(0, 0.5));
    tmpLabel->setPosition(ccp(30, 0));
    tmpLabel->setColor(ccWHITE);
    this->addChild(tmpLabel);
    
    int curX = 0;
    int curY = 0;
    int addX = 60;
    ItemMsgCell* msgCell = NULL;
    if (type == 7 || type == 8)
    {
        tmpLabel->setString(_lang_2("108636", "", ""));
        curX = tmpLabel->getPositionX()+tmpLabel->getContentSize().width*tmpLabel->getOriginScaleX()+10;
        vector<string> armys;
        CCCommonUtils::splitString(msg, ",", armys);
        for (int i=0; i<armys.size(); i++)
        {
            string strTmpArmy = armys[i];
            if (type == 8)
            {
                string picName = CCCommonUtils::getIcon(strTmpArmy);
                msgCell = ItemMsgCell::create(picName, "");
            }
            else
            {
                vector<string> tinyMsgs;
                CCCommonUtils::splitString(strTmpArmy, "|", tinyMsgs);
                string picName = CCCommonUtils::getIcon(tinyMsgs[0]);
                msgCell = ItemMsgCell::create(picName, tinyMsgs[1]);
            }
            msgCell->setPosition(ccp(curX,curY));
            this->addChild(msgCell);
            curX += addX;
        }
        setContentSize(CCSizeMake(620, curY));
    }
    else if (type == 11) {
        tmpLabel->setString(_lang_1("108603", ""));
        curX = tmpLabel->getPositionX()+tmpLabel->getContentSize().width*tmpLabel->getOriginScaleX()+30;
        vector<string> talent;
        CCCommonUtils::splitString(msg, ",", talent);
        for (int i=0; i<talent.size(); i++)
        {
            string strTmpTalent = talent[i];
            vector<string> tinyMsgs;
            CCCommonUtils::splitString(strTmpTalent, "|", tinyMsgs);
            string picName = CCCommonUtils::getIcon(tinyMsgs[0]);
            msgCell = ItemMsgCell::create(picName, tinyMsgs[1], type, m_idx);
            msgCell->setPosition(ccp(curX,curY));
            this->addChild(msgCell);
            curX += addX;
        }
    }
    else if (type == 10) {
        tmpLabel->setString(_lang_1("108604", ""));
        curX = tmpLabel->getPositionX()+tmpLabel->getContentSize().width*tmpLabel->getOriginScaleX()+30;
        vector<string> sciences;
        CCCommonUtils::splitString(msg, ",", sciences);
        for (int i=0; i<sciences.size(); i++)
        {
            string strTmpScience = sciences[i];
            vector<string> tinyMsgs;
            CCCommonUtils::splitString(strTmpScience, "|", tinyMsgs);
            string picName = CCCommonUtils::getIcon(tinyMsgs[0]);
            msgCell = ItemMsgCell::create(picName, tinyMsgs[1]);
            msgCell->setPosition(ccp(curX,curY));
            this->addChild(msgCell);
            curX += addX;
        }
    }
    
    bool ret = true;
    return ret;
}

void EnemyMsgCell::setTouchNode(CCNode* node)
{
    m_touchNode = node;
}

void EnemyMsgCell::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, true);
}

void EnemyMsgCell::onExit() {
    CCNode::onExit();
    setTouchEnabled(false);
}

bool EnemyMsgCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return false;
}

void EnemyMsgCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

SEL_CCControlHandler EnemyMsgCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool EnemyMsgCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    return false;
}
//////////////
ItemMsgCell* ItemMsgCell::create(string picName, string msg, int type, int idx)
{
    auto ret = new ItemMsgCell();
    if (ret && ret->init(picName, msg, type, idx)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ItemMsgCell::init(string picName, string msg, int type, int idx)
{
    auto tmpCCB = CCBLoadFile("ItemMsgCell",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    auto pic = CCLoadSprite::createSprite(picName.c_str());
//    arm2_icon
    if (pic) {
        CCCommonUtils::setSpriteMaxSize(pic, 60);
        m_picNode->addChild(pic);
    }
    m_msgLabel->setString(msg.c_str());
    m_type = type;
    m_idx = idx;
    if (m_type == 11) {
        m_msgLabel->setString("");
        m_ablityType = msg;
    }
    
    bool ret = true;
    return ret;
}

void ItemMsgCell::onEnter() {
    CCNode::onEnter();
}

void ItemMsgCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler ItemMsgCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn", ItemMsgCell::onClickBtn);
    return NULL;
}

bool ItemMsgCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    return false;
}

void ItemMsgCell::onClickBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type==11) {
        EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[m_idx];
        auto generalInfo = GeneralInfo(info.genDic);
        PopupViewController::getInstance()->addPopupInView(GeneralSkillListPopUpView::create(&generalInfo,m_ablityType));
    }
}

////////////////
ItemMsgCell2* ItemMsgCell2::create(string picName, string msg, int type, int idx)
{
    auto ret = new ItemMsgCell2();
    if (ret && ret->init(picName, msg, type, idx)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ItemMsgCell2::init(string picName, string msg, int type, int idx)
{
//    auto tmpCCB = CCBLoadFile("ItemMsgCell",this,this);
//    this->setContentSize(tmpCCB->getContentSize());
    
    auto pic = CCLoadSprite::createScale9Sprite("cd_bar_bg.png");
    pic->setAnchorPoint(ccp(0,0.5));
    int ww = pic->getContentSize().width;
    pic->setPreferredSize(CCSizeMake(ww, 40));
    this->addChild(pic);
    
    auto tmpLabel1 = CCLabelIF::create(picName.c_str(), 24);
    tmpLabel1->setAnchorPoint(ccp(0, 0.5));
    tmpLabel1->setPosition(ccp(30, 0));
    tmpLabel1->setColor(ccWHITE);
    this->addChild(tmpLabel1);
    
    auto tmpLabel2 = CCLabelIF::create(msg.c_str(), 24);
    tmpLabel2->setAnchorPoint(ccp(0, 0.5));
    tmpLabel2->setPosition(ccp(330, 0));
    tmpLabel2->setColor(ccWHITE);
    this->addChild(tmpLabel2);
    
//    m_msgLabel->setString(msg.c_str());
    m_type = type;
    m_idx = idx;
    
    bool ret = true;
    return ret;
}

void ItemMsgCell2::onEnter() {
    CCNode::onEnter();
}

void ItemMsgCell2::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler ItemMsgCell2::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBtn", ItemMsgCell2::onClickBtn);
    return NULL;
}

bool ItemMsgCell2::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    return false;
}

void ItemMsgCell2::onClickBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type==11) {
        EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[m_idx];
        auto generalInfo = GeneralInfo(info.genDic);
        PopupViewController::getInstance()->addPopupInView(GeneralSkillListPopUpView::create(&generalInfo,m_ablityType));
    }
}


ResUpTitleCell* ResUpTitleCell::create(string msg)
{
    auto ret = new ResUpTitleCell();
    if (ret && ret->init(msg)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ResUpTitleCell::init(string msg)
{
    CCBLoadFile("ResUpTitleCell",this,this);
    m_titleLabel->setString(msg.c_str());
    bool ret = true;
    return ret;
}

void ResUpTitleCell::onEnter() {
    CCNode::onEnter();
}

void ResUpTitleCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler ResUpTitleCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool ResUpTitleCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    return false;
}