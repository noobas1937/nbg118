//
//  SingleScoreRankView.cpp
//  IF
//
//  Created by 付彬 on 15/11/25.
//
//

#include "SingleScoreRankView.hpp"
#include "ActivityController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "GuideController.h"
#include "ToolController.h"
#include "RewardController.h"
#include "SingleScoreHistoryRankView.hpp"

static int SType = 0; //0 个人排行榜奖励 1 联盟排行榜奖励 2 王国个人排行榜奖励 3 王国联盟排行榜奖励
SingleScoreRankView* SingleScoreRankView::create(int type)
{
    auto ret = new SingleScoreRankView();
    if (ret && ret->init(type)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool SingleScoreRankView::init(int type)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    
    auto tmpCCB = CCBLoadFile("SinglelListView", this, this);
    setContentSize(tmpCCB->getContentSize());
    
    SType = type;
    m_data = CCArray::create();
    
    changeBGHeight(m_bg);
    float extH = getExtendHeight();
    m_bottomNode->setPositionY(m_bottomNode->getPositionY() - extH);
    m_infoList->setContentSize( m_infoList->getContentSize() + Size(0, extH) );
    
    int curRank = 0;
    if (SType == 1) {
        curRank = ActivityController::getInstance()->AllianceScoreRank;
        m_info1Label->setString(_lang("150366"));
        m_info2Label->setString(_lang_1("150368", ""));
        
        m_historyBtn->setPositionX(150);
        CCCommonUtils::setButtonTitle(m_historyBtn, _lang("150367").c_str());
        CCCommonUtils::setButtonTitle(m_conRankBtn, _lang("150370").c_str());
        
    }else if (SType == 0) {
        curRank = ActivityController::getInstance()->SingleScoreRank;
        m_info1Label->setString(_lang("150309"));
        m_info2Label->setString(_lang_1("150310", ""));
        
        CCCommonUtils::setButtonTitle(m_historyBtn, _lang("150314").c_str());
        m_conRankBtn->setVisible(false);
        m_conRankBtn->setEnabled(false);
    }
    else if (SType == 2) {
        curRank = ActivityController::getInstance()->KingPlScoreRank;
        m_info1Label->setString(_lang("150309"));
        m_info2Label->setString(_lang_1("150310", ""));
        
        CCCommonUtils::setButtonTitle(m_historyBtn, _lang("150314").c_str());
        m_conRankBtn->setVisible(false);
        m_conRankBtn->setEnabled(false);
    }
    else if (SType == 3) {
        curRank = ActivityController::getInstance()->KingAlScoreRank;
        m_info1Label->setString(_lang("150309"));
        m_info2Label->setString(_lang_1("150368", ""));
        
        m_historyBtn->setPositionX(150);
        CCCommonUtils::setButtonTitle(m_historyBtn, _lang("150314").c_str());
        CCCommonUtils::setButtonTitle(m_conRankBtn, _lang("150370").c_str());
    }
    
    if (curRank == 0) {
        m_info3Label->setString("5000+");
    }else {
        m_info3Label->setString(CC_ITOA( curRank ));
    }
    m_info3Label->setPositionX( m_info2Label->getPositionX() + m_info2Label->getContentSize().width*m_info2Label->getOriginScaleX() + 10 );
    
    m_tableView = CCTableView::create(this, m_infoList->getContentSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_infoList->addChild(m_tableView);
    
    refreshData();
    
    return true;
}

void SingleScoreRankView::onEnter()
{
    CCNode::onEnter();
    if (SType == 1) {
        setTitleName(_lang("150363"));
    }else if (SType == 0) {
        setTitleName(_lang("150308"));
    }else if (SType == 2) {
        setTitleName(_lang("150467"));
    }else if (SType == 3) {
        setTitleName(_lang("150455"));
    }
}
void SingleScoreRankView::onExit()
{
    CCNode::onExit();
}

void SingleScoreRankView::refreshData()
{
    m_data->removeAllObjects();
    
    int curRank = 0;
    vector<string> rankKeys;
    
    if (SType == 1) {
        curRank = ActivityController::getInstance()->AllianceScoreRank;
        rankKeys = ActivityController::getInstance()->m_allRankKeys;
    }else if (SType == 0) {
        curRank = ActivityController::getInstance()->SingleScoreRank;
        rankKeys = ActivityController::getInstance()->m_CurRankKeys;
    }else if (SType == 2) {
        curRank = ActivityController::getInstance()->KingPlScoreRank;
        rankKeys = ActivityController::getInstance()->m_kingPlRankKeys;
    }else if (SType == 3) {
        curRank = ActivityController::getInstance()->KingAlScoreRank;
        rankKeys = ActivityController::getInstance()->m_kingAlRankKeys;
    }
    
    bool isRank = false;
    int offSetX = 0;
    
    for (int i=0; i<rankKeys.size(); i++)
    {
        string curKey = rankKeys[i];
        vector<string> tmpRank;
        CCCommonUtils::splitString(curKey, "-", tmpRank);
        string rewardId = "";
        if (SType == 1) {
            rewardId = ActivityController::getInstance()->m_allRankRwdMap[curKey];
        }else if (SType == 0) {
            rewardId = ActivityController::getInstance()->m_CurRankRwdMap[curKey];
        }else if (SType == 2) {
            rewardId = ActivityController::getInstance()->m_kingPlRankRwdMap[curKey];
        }else if (SType == 3) {
            rewardId = ActivityController::getInstance()->m_kingAlRankRwdMap[curKey];
        }
        
        auto titlDict = CCDictionary::create();
        titlDict->setObject(CCString::create( _lang_1("150311", curKey.c_str()) ), "title");
//        titlDict->setObject(CCString::create( curKey ), "title");
        if (tmpRank.size() == 2) {
            int minR = atoi(tmpRank[0].c_str());
            int maxR = atoi(tmpRank[1].c_str());
            if (minR<=curRank && curRank<=maxR) {
                titlDict->setObject(CCString::create("1"), "mine");
                isRank = true;
            }
        }
        else if (tmpRank.size() == 1) {
            if (atoi(tmpRank[0].c_str()) == curRank) {
                titlDict->setObject(CCString::create("1"), "mine");
                isRank = true;
            }
        }
        else {
            continue;
        }
        
        m_data->addObject(titlDict);
        if(!isRank) {
            offSetX += 1;
        }
        if (ActivityController::getInstance()->m_RankRwdMap.find(rewardId) != ActivityController::getInstance()->m_RankRwdMap.end() )
        {
            m_data->addObjectsFromArray( ActivityController::getInstance()->m_RankRwdMap[rewardId] );
            if (!isRank) {
                offSetX = m_data->count();
            }
        }
    }
    
    if (!isRank) {
        offSetX = 0;
    }
    
    m_tableView->reloadData();
    float miny = m_tableView->minContainerOffset().y;
    CCPoint pos  = m_tableView->getContentOffset();
    pos.y = miny+offSetX*60;
    m_tableView->setContentOffset(pos);
}

SEL_CCControlHandler SingleScoreRankView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", SingleScoreRankView::onTipBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHistoryBtnClick", SingleScoreRankView::onHistoryBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onConRankBtnClick", SingleScoreRankView::onConRankBtnClick);
    return NULL;
}
bool SingleScoreRankView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_historyBtn", CCControlButton*, this->m_historyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_conRankBtn", CCControlButton*, this->m_conRankBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info1Label", CCLabelIF*, this->m_info1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2Label", CCLabelIF*, this->m_info2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info3Label", CCLabelIF*, this->m_info3Label);
    return false;
}

void SingleScoreRankView::onTipBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45220");
}

void SingleScoreRankView::onHistoryBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    if (SType == 1) {
        PopupViewController::getInstance()->addPopupInView(SingleScoreHistoryRankView::create(1));
    }else if (SType == 0) {
        PopupViewController::getInstance()->addPopupInView(SingleScoreHistoryRankView::create(0));
    }else if (SType == 2) {
        PopupViewController::getInstance()->addPopupInView(SingleScoreHistoryRankView::create(3));
    }else if (SType == 3) {
        PopupViewController::getInstance()->addPopupInView(SingleScoreHistoryRankView::create(4));
    }
}

void SingleScoreRankView::onConRankBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    if (SType == 3) {
        PopupViewController::getInstance()->addPopupInView(SingleScoreHistoryRankView::create(5));
    }else {
        PopupViewController::getInstance()->addPopupInView(SingleScoreHistoryRankView::create(2));
    }
}

CCSize SingleScoreRankView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return CCSizeZero;
    }
    return CCSizeMake(620, 60);
}

CCSize SingleScoreRankView::cellSizeForTable(  cocos2d::extension::TableView *table)
{
    return CCSizeMake(620, 60);
}

CCTableViewCell* SingleScoreRankView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return NULL;
    }
    CCDictionary* dict = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    SingleScoreRankCell* cell = dynamic_cast<SingleScoreRankCell*>(table->dequeueCell());
    if (cell == nullptr)
    {
        cell = SingleScoreRankCell::create();
    }
    
    if ( dict->objectForKey("title") )
    {
        string title = dict->valueForKey("title")->getCString();
        if (dict->objectForKey("mine")) {
            cell->setTitleData(title, true);
        }else {
            cell->setTitleData(title, false);
        }
    }
    else
    {
        int type = dict->valueForKey("type")->intValue();
        if (type == R_GOODS || type == R_EQUIP) {
            CCDictionary *valueDict=_dict(dict->objectForKey("value"));
            int itemId=valueDict->valueForKey("id")->intValue();
            int rewardAdd=valueDict->valueForKey("num")->intValue();
            std::string nameStr = RewardController::getInstance()->getNameByType(type, itemId);
            cell->setItemData(itemId,nameStr,rewardAdd,type);
        }else {
            int rewardAdd=dict->valueForKey("value")->intValue();
            std::string nameStr = RewardController::getInstance()->getNameByType(type, 0);
            cell->setItemData(0,nameStr,rewardAdd,type);
        }
    }
    
    return cell;
}

ssize_t SingleScoreRankView::numberOfCellsInTableView(  cocos2d::extension::TableView *table)
{
    return m_data->count();
}

/*
 SingleScoreRankCell
 */
SingleScoreRankCell* SingleScoreRankCell::create()
{
    auto ret = new SingleScoreRankCell();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}
bool SingleScoreRankCell::init()
{
    if (!CCNode::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("SingleListCell", this, this);
    m_numLabel->setFntFile("pve_fnt_title.fnt");
    if (SType == 1) {
        m_stateLabel->setString(_lang("150369"));
    }else if (SType == 0) {
        m_stateLabel->setString(_lang("150312"));
    }else if (SType == 2) {
        m_stateLabel->setString(_lang("150312"));
    }else if (SType == 3) {
        m_stateLabel->setString(_lang("150369"));
    }
    setContentSize(CCSizeMake(620, 60));
    return true;
}
void SingleScoreRankCell::onEnter()
{
    CCNode::onEnter();
}
void SingleScoreRankCell::onExit()
{
    CCNode::onExit();
}
SEL_CCControlHandler SingleScoreRankCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool SingleScoreRankCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode", CCNode*, this->m_itemNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIFBMFont*, this->m_numLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, this->m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stateNode", CCNode*, this->m_stateNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stateLabel", CCLabelIF*, this->m_stateLabel);
    return false;
}

void SingleScoreRankCell::setTitleData(string title, bool isMine)
{
    m_titleLabel->setString(title);
    m_stateNode->setVisible(isMine);
    m_itemNode->setVisible(false);
    m_titleNode->setVisible(true);
}

void SingleScoreRankCell::setItemData(int itemId,string nameStr,int countStr,int itemType)
{
    m_iconNode->removeAllChildren();
    m_nameLabel->setString(nameStr.c_str());
    string str=" X"+CC_CMDITOA(countStr);
    m_numLabel->setString(str.c_str());
    if (itemType==R_GOODS) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
        if (pic) {
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 50,true);
            m_iconNode->addChild(iconBg);
            CCCommonUtils::setSpriteMaxSize(pic, 50,true);
            m_iconNode->addChild(pic);
        }
    }else if (itemType==R_EQUIP){
        std::string picStr = CCCommonUtils::getPropById(CC_ITOA(itemId), "icon");
        picStr+=".png";
        auto pic = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
        if (pic) {
            std::string colorStr = CCCommonUtils::getPropById(CC_ITOA(itemId), "color");
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(atoi(colorStr.c_str())).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 50,true);
            m_iconNode->addChild(iconBg);
            CCCommonUtils::setSpriteMaxSize(pic, 50,true);
            m_iconNode->addChild(pic);
        }
        string name=CCCommonUtils::getPropById(CC_ITOA(itemId), "name");
        m_nameLabel->setString(_lang(name).c_str());
    }else{
        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(itemType).c_str(),true,CCLoadSpriteType_DEFAULT);
        if (pic) {
            CCCommonUtils::setSpriteMaxSize(pic, 50,true);
            m_iconNode->addChild(pic);
        }
    }
    
    m_itemNode->setVisible(true);
    m_titleNode->setVisible(false);
}