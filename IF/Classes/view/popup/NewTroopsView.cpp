//
//  NewTroopsView.cpp
//  IF
//
//  Created by xxrdsg on 15-8-31.
//
//

#include "NewTroopsView.h"
#include "ArmyController.h"
#include "WorldController.h"
#include "MarchFormationView.h"
#include "PopupViewController.h"
#include "FunBuildController.h"

static const char* troops_roman[30] =  {"I","II","III","IV","V","VI","VII","VIII","IX","X","XI","XII","XIII","XIV","XV","XVI","XVII","XVIII","XIX","XX", "XXI","XXII","XXIII","XXIV","XXV","XXVI","XXVII","XXVIII","XXIX","XXX"};

NewTroopsView* NewTroopsView::create()
{
    auto ret = new NewTroopsView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewTroopsView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
//    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(200, true);
    CCLoadSprite::doResourceByCommonIndex(201, true);
    CCLoadSprite::doResourceByCommonIndex(202, true);
    CCLoadSprite::doResourceByCommonIndex(203, true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(200, false);
        CCLoadSprite::doResourceByCommonIndex(201, false);
        CCLoadSprite::doResourceByCommonIndex(202, false);
        CCLoadSprite::doResourceByCommonIndex(203, false);
        CCLoadSprite::doResourceByCommonIndex(504, false);
    });
    auto frame = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchnode = CCSpriteBatchNode::createWithTexture(frame->getTexture());
    auto size = CCDirector::sharedDirector()->getWinSize();
    int curH = 0;
    while (curH < size.height) {
        auto spr = CCLoadSprite::createSprite("technology_09.png");
        spr->setAnchorPoint(CCPointZero);
        spr->setPosition(ccp(0, curH));
        tBatchnode->addChild(spr);
        curH += spr->getContentSize().height;
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        tBatchnode->setScaleX(1536.0 / 640);
    }
    this->addChild(tBatchnode);
    auto tmpCCB = CCBLoadFile("NewTroopsView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    int add = size.height - tmpCCB->getContentSize().height;
    tBatchnode->setPositionY(tBatchnode->getPositionY() - add);
    m_listNode->setPositionY(m_listNode->getPositionY() - add);
    m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + add));
    m_scrollView = CCScrollView::create(m_listNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setContentSize(m_listNode->getContentSize());
    m_scrollView->setAnchorPoint(ccp(0, 0));
    m_scrollView->setPosition(ccp(0, 0));
    m_scrollView->setDelegate(this);
    m_listNode->addChild(m_scrollView);
    m_mainNode = CCNode::create();
    m_scrollView->addChild(m_mainNode);
    
    refreshView(nullptr);
    return true;
}

void NewTroopsView::refreshView(cocos2d::CCObject * obj)
{
    float troops_cell_width = 158;
    float troops_cell_height = 280;
    int cellCnt = 4;
    if (CCCommonUtils::isIosAndroidPad())
    {
        troops_cell_width = 1536 / 5.0;
        troops_cell_height = 552;
        cellCnt = 5;
    }
    m_mainNode->removeAllChildrenWithCleanup(true);
    int cury = 0;
    
    //俩按钮
    if (FunBuildController::getInstance()->getMainCityLv() >= 15 && GlobalData::shared()->march_formation_switch == 2 && ArmyController::getInstance()->getTotalArmy() > 0) {
        auto cell = NewTroopsBtn::create();
        cell->setPosition(0, cury);
        m_mainNode->addChild(cell);
        if (CCCommonUtils::isIosAndroidPad())
        {
            cury -= 126;
        } else {
            cury -= 55;
        }
    }
    
    //army title
    auto armyTitle = NewTroopsArmyTitle::create();
    armyTitle->setPosition(0, cury);
    m_mainNode->addChild(armyTitle);
    if (CCCommonUtils::isIosAndroidPad())
    {
        cury -= 289;
    } else {
        cury -= 162;
    }
    
    //army cells
    vector<string> bu;
    vector<string> qi;
    vector<string> gong;
    vector<string> che;
    int id = 0;
    int type = 0;
    for (auto it = GlobalData::shared()->armyList.rbegin(); it != GlobalData::shared()->armyList.rend(); ++it)
    {
        id = atoi((it->first).c_str());
        type = id / 100;
        if (it->second.free > 0) {
            switch (type) {
                case 1070:
                    bu.push_back(it->first);
                    break;
                case 1071:
                    qi.push_back(it->first);
                    break;
                case 1072:
                    gong.push_back(it->first);
                    break;
                case 1073:
                    che.push_back(it->first);
                    break;
                    
                default:
                    break;
            }
        }
    }
    if (bu.size()) {
        for (int i = 0; i < bu.size(); ++i) {
            auto cell = NewTroopsCell::create(bu.at(i));
            cell->setPosition((i % cellCnt) * troops_cell_width + 4, cury - floor(i * 1.0 / cellCnt ) * troops_cell_height);
            m_mainNode->addChild(cell);
        }
        cury -= (ceil(bu.size() * 1.0 / cellCnt)) * troops_cell_height;
    }
    if (qi.size()) {
        if (bu.size())
        {
            auto spr = CCLoadSprite::createScale9Sprite("Army_flag_04.png");
            spr->setPreferredSize(CCSize(642, 6));
            spr->setAnchorPoint(ccp(0, 1));
            spr->setPosition(ccp(-1, cury-10));
            if (CCCommonUtils::isIosAndroidPad()) {
                spr->setPreferredSize(CCSize(1540, 12));
                spr->setPosition(ccp(-2, cury - 20));
            }
            m_mainNode->addChild(spr);
            cury -= (25 + spr->getPreferredSize().height);
            if (CCCommonUtils::isIosAndroidPad()) {
                cury -= 25;
            }
        }
        for (int i = 0; i < qi.size(); ++i) {
            auto cell = NewTroopsCell::create(qi.at(i));
            cell->setPosition((i % cellCnt) * troops_cell_width + 4, cury - floor(i * 1.0 / cellCnt) * troops_cell_height);
            m_mainNode->addChild(cell);
        }
        cury -= (ceil(qi.size() * 1.0 / cellCnt)) * troops_cell_height;
    }
    if (gong.size()) {
        if (bu.size() + qi.size()) {
            auto spr = CCLoadSprite::createScale9Sprite("Army_flag_04.png");
            spr->setPreferredSize(CCSize(642, 6));
            spr->setAnchorPoint(ccp(0, 1));
            spr->setPosition(ccp(-1, cury-10));
            if (CCCommonUtils::isIosAndroidPad()) {
                spr->setPreferredSize(CCSize(1540, 12));
                spr->setPosition(ccp(-2, cury - 20));
            }
            m_mainNode->addChild(spr);
            cury -= (25 + spr->getPreferredSize().height);
            if (CCCommonUtils::isIosAndroidPad()) {
                cury -= 25;
            }
        }
        for (int i = 0; i < gong.size(); ++i) {
            auto cell = NewTroopsCell::create(gong.at(i));
            cell->setPosition((i % cellCnt) * troops_cell_width + 4, cury - floor(i * 1.0 / cellCnt) * troops_cell_height);
            m_mainNode->addChild(cell);
        }
        cury -= (ceil(gong.size() * 1.0 / cellCnt)) * troops_cell_height;
    }
    if (che.size()) {
        if (bu.size() + qi.size() + gong.size())
        {
            auto spr = CCLoadSprite::createScale9Sprite("Army_flag_04.png");
            spr->setPreferredSize(CCSize(642, 6));
            spr->setAnchorPoint(ccp(0, 1));
            spr->setPosition(ccp(-1, cury-10));
            if (CCCommonUtils::isIosAndroidPad()) {
                spr->setPreferredSize(CCSize(1540, 12));
                spr->setPosition(ccp(-2, cury - 20));
            }
            m_mainNode->addChild(spr);
            cury -= (25 + spr->getPreferredSize().height);
            if (CCCommonUtils::isIosAndroidPad()) {
                cury -= 25;
            }
        }
        for (int i = 0; i < che.size(); ++i) {
            auto cell = NewTroopsCell::create(che.at(i));
            cell->setPosition((i % cellCnt) * troops_cell_width + 4, cury - floor(i * 1.0 / cellCnt) * troops_cell_height);
            m_mainNode->addChild(cell);
        }
        cury -= (ceil(che.size() * 1.0 / cellCnt)) * troops_cell_height;
    }
    
    if ((bu.size() + qi.size() + gong.size() + che.size()) == 0) {
        cury -= 70;//两大类间隔一定距离
        if (CCCommonUtils::isIosAndroidPad())
        {
            cury -= 70;
        }
    } else {
        cury -= 20;
        if (CCCommonUtils::isIosAndroidPad()) {
            cury -= 20;
        }
    }
    
    //陷阱 title
    auto trapCell = NewTroopsTrapTitle::create();
    trapCell->setPosition(ccp(0, cury));
    m_mainNode->addChild(trapCell);
    if (CCCommonUtils::isIosAndroidPad()) {
        cury -= 160;
    } else {
        cury -= 80;
    }
    
    
    //陷阱 cell
    map< int, vector<string> > traps;
    for (auto it = GlobalData::shared()->fortList.rbegin(); it != GlobalData::shared()->fortList.rend(); ++it)
    {
        if (it->second.free > 0) {
            int tid = atoi((it->first).c_str());
            int level = tid % 10;
            traps[level].push_back(it->first);
        }
    }
    bool st = false;
    for (auto it = traps.rbegin(); it != traps.rend(); ++it) {
        if ((it->second).size() > 0)
        {
            if (st) {
                auto spr = CCLoadSprite::createScale9Sprite("Army_flag_04.png");
                spr->setPreferredSize(CCSize(642, 6));
                spr->setAnchorPoint(ccp(0, 1));
                spr->setPosition(ccp(-1, cury-10));
                if (CCCommonUtils::isIosAndroidPad()) {
                    spr->setPreferredSize(CCSize(1540, 12));
                    spr->setPosition(ccp(-2, cury - 20));
                }
                m_mainNode->addChild(spr);
                cury -= (25 + spr->getPreferredSize().height);
            }
            st = true;
            for (int i = 0; i < it->second.size(); ++i)
            {
                auto cell = NewTroopsCell::create(it->second.at(i));
                cell->setPosition(ccp((i % cellCnt) * troops_cell_width + 4, cury - ((int)(i / cellCnt)) * troops_cell_height));
                m_mainNode->addChild(cell);
            }
            cury -= (ceil(it->second.size() * 1.0 / cellCnt)) * troops_cell_height;
        }
    }
    if (!st) {
        //为了显示没有陷阱的dialog
        if (CCCommonUtils::isIosAndroidPad())
        {
            cury -= 120;
        } else {
            cury -= 60;
        }
    }
    
    //上下边界固定不可拖动
    if (abs(cury) < m_listNode->getContentSize().height)
    {
        cury -= (m_listNode->getContentSize().height - abs(cury));
    }
    
    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width, abs(cury)));
    m_mainNode->setPosition(0, abs(cury));
    m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - m_scrollView->getContentSize().height));
}

void NewTroopsView::scrollViewDidScroll(CCScrollView *view)
{
    float mindy = m_scrollView->minContainerOffset().y ;
    float maxdy = m_scrollView->maxContainerOffset().y ;
    float dy = m_scrollView->getContentOffset().y;
    if (dy < mindy)
    {
        m_scrollView->setContentOffset(ccp(0, mindy));
    }
    if (dy > maxdy) {
        m_scrollView->setContentOffset(ccp(0, maxdy));
    }
}

void NewTroopsView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("108724"));
}

void NewTroopsView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler NewTroopsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return nullptr;
}
bool NewTroopsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    return false;
}

////////-----------class NewTroopsCell
NewTroopsCell* NewTroopsCell::create(string identity)
{
    auto ret = new NewTroopsCell(identity);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewTroopsCell::init()
{
    if (!CCNode::init()) {
        return false;
    }
    CCBLoadFile("NewTroopsCell", this, this);
    m_iconNode->removeAllChildrenWithCleanup(true);
    m_levelLabel->setColor({160, 177, 200});
    m_numLabel->setColor({160, 177, 200});
    
    
    int id = atoi(m_id.c_str());
    if (id >= 107000 && id <107400)
    {
        if (GlobalData::shared()->armyList.find(m_id) != GlobalData::shared()->armyList.end()) {
            auto& info = GlobalData::shared()->armyList[m_id];
            auto icon = CCLoadSprite::createSprite((info.getHeadIcon()).c_str());
            CCCommonUtils::setSpriteMaxSize(icon, 222, true);
            if (id >= 107300)
            {
                CCCommonUtils::setSpriteMaxSize(icon, 200, true);
            }
            m_iconNode->addChild(icon);
            m_numLabel->setString(CC_CMDITOA(info.free));
            m_levelLabel->setString(troops_roman[info.armyLevel]);
        }
    } else {
        if (GlobalData::shared()->fortList.find(m_id) != GlobalData::shared()->fortList.end()) {
            auto& info = GlobalData::shared()->fortList[m_id];
            auto icon = CCLoadSprite::createSprite((info.getHeadIcon()).c_str());
            CCCommonUtils::setSpriteMaxSize(icon, 170, true);
            m_iconNode->addChild(icon);
            m_numLabel->setString(CC_CMDITOA(info.free));
            int level = id % 10;
            m_levelLabel->setString(troops_roman[level]);
        }
    }
    float w = m_numLabel->getContentSize().width * m_numLabel->getOriginScaleX();
    float curscale = m_numLabel->getScale();
    float maxw = 100.0;
    if (CCCommonUtils::isIosAndroidPad()) {
        maxw = 200.0;
    }
    if (w > maxw) {
        float scale = maxw / w * curscale;
        m_numLabel->setScale(scale);
    }
    
    return true;
}

bool NewTroopsCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelLabel", CCLabelIF*, this->m_levelLabel);
    return false;
}

//////------------class NewTroopsBtn
bool NewTroopsBtn::init()
{
    CCNode::init();
    CCBLoadFile("NewTroopsBtn", this, this);
    return true;
}

void NewTroopsBtn::onFormation1Click(CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->addPopupInView(MarchFormationView::create(1));
}

void NewTroopsBtn::onFormation2Click(CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->addPopupInView(MarchFormationView::create(2));
}

SEL_CCControlHandler NewTroopsBtn::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation1Click", NewTroopsBtn::onFormation1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation2Click", NewTroopsBtn::onFormation2Click);
    return nullptr;
}

bool NewTroopsBtn::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    return false;
}

/////------------class NewTroopsArmyTitle
NewTroopsArmyTitle* NewTroopsArmyTitle::create()
{
    auto ret = new NewTroopsArmyTitle();
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewTroopsArmyTitle::init()
{
    CCNode::init();
    CCBLoadFile("NewTroopsArmyTitle", this, this);
    
    float dw = 10;
    if (CCCommonUtils::isIosAndroidPad()) {
        dw = 20;
    }
    m_armyLabel->setString(_lang("102149"));
    m_armyLabel->setColor({127, 127, 127});
    m_armyNum->setString(CC_CMDITOA(ArmyController::getInstance()->getTotalFree()));
    m_armyNum->setPositionX(m_armyLabel->getPositionX() + m_armyLabel->getContentSize().width * m_armyLabel->getOriginScaleX() + dw);
    m_armyNum->setColor({160, 177, 202});
    
    m_marchLabel->setString(_lang("102141"));
    int maxMarchNum = WorldController::getInstance()->getMaxMarchCount();
    int nowMarchNum = maxMarchNum - WorldController::getInstance()->getCurrentMarchCount();
    string msg = CC_ITOA(nowMarchNum);
    msg += "/";
    msg += CC_ITOA(maxMarchNum);
    m_marchNum->setString(msg);
    m_marchNum->setPositionX(m_marchLabel->getPositionX() + m_marchLabel->getContentSize().width * m_marchLabel->getOriginScaleX() + dw);
    m_marchLabel->setColor({127, 127, 127});
    m_marchNum->setColor({127, 127, 127});
    
    m_foodSpdLabel->setString(_lang("102125"));
    int upKeep = ArmyController::getInstance()->getTotalUpKeep();
    msg = CC_CMDITOA(upKeep);
    msg += "/h";
    m_foodSpd->setString(msg);
    m_foodSpd->setPositionX(m_foodSpdLabel->getPositionX() + m_foodSpdLabel->getContentSize().width * m_foodSpdLabel->getOriginScaleX() + dw);
    m_foodSpdLabel->setColor({127, 127, 127});
    m_foodSpd->setColor({127, 127, 127});
    
    m_woundedLabel->setString(_lang("102186"));
    int deadNum = 0;
    for(auto Tit = GlobalData::shared()->treatList.begin(); Tit != GlobalData::shared()->treatList.end(); Tit++){
        deadNum += Tit->second.dead;
    }
    msg = CC_CMDITOA(deadNum) + "/" + CC_CMDITOA(ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY));
    m_woundedNum->setString(msg);
    m_woundedNum->setPositionX(m_woundedLabel->getPositionX() + m_woundedLabel->getContentSize().width * m_woundedLabel->getOriginScaleX() + dw);
    m_woundedLabel->setColor({127, 127, 127});
    m_woundedNum->setColor({127, 127, 127});
    
    if (ArmyController::getInstance()->getTotalFree() > 0)
    {
        m_myTroopLabel->setString(_lang("103694"));
    } else {
        m_myTroopLabel->setString(_lang("103695"));
    }
    
    m_myTroopLabel->setColor({72, 60, 44});
    
    
    return true;
}

SEL_CCControlHandler NewTroopsArmyTitle::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return nullptr;
}

bool NewTroopsArmyTitle::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armyLabel", CCLabelIF*, this->m_armyLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armyNum", CCLabelIF*, this->m_armyNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchLabel", CCLabelIF*, this->m_marchLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchNum", CCLabelIF*, this->m_marchNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedLabel", CCLabelIF*, this->m_woundedLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedNum", CCLabelIF*, this->m_woundedNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodSpdLabel", CCLabelIF*, this->m_foodSpdLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodSpd", CCLabelIF*, this->m_foodSpd);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_myTroopLabel", CCLabelIF*, this->m_myTroopLabel);
    return false;
}

/////-----------class NewTroopsTrapTitle
NewTroopsTrapTitle* NewTroopsTrapTitle::create()
{
    auto ret = new NewTroopsTrapTitle();
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewTroopsTrapTitle::init()
{
    CCNode::init();
    CCBLoadFile("NewTroopsTrapTitle", this, this);
    m_trapLabel->setString(_lang("102185"));
    string msg = CC_CMDITOA(ArmyController::getInstance()->getTotalFortNum()) +"/"+ CC_CMDITOA( ArmyController::getInstance()->getMaxNumByType(FORT));
    m_trapNum->setString(msg);
    float dw = 10;
    if (CCCommonUtils::isIosAndroidPad()) {
        dw = 20;
    }
    m_trapNum->setPositionX(m_trapLabel->getPositionX() + m_trapLabel->getContentSize().width * m_trapLabel->getOriginScaleX() + dw);
    m_trapLabel->setColor({127, 127, 127});
    m_trapNum->setColor({127, 127, 127});
    m_tipLabel->setColor({72, 60, 44});
    if (ArmyController::getInstance()->getTotalFortNum() <= 0)
    {
        m_tipLabel->setString(_lang("103696"));
    } else {
        m_tipLabel->setString("");
    }
    return true;
}

bool NewTroopsTrapTitle::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipLabel", CCLabelIF*, this->m_tipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_trapLabel", CCLabelIF*, this->m_trapLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_trapNum", CCLabelIF*, this->m_trapNum);
    return false;
}