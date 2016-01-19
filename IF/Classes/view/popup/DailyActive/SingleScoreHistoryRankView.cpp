//
//  SingleScoreHistoryRankView.cpp
//  IF
//
//  Created by 付彬 on 15/11/25.
//
//

#include "SingleScoreHistoryRankView.hpp"
#include "ActivityController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "GuideController.h"
#include "ToolController.h"
#include "RewardController.h"
#include "QuestController.h"
#include "ChestUseView.h"

#define FRESH_HIS_CELL "refresh.history.cell"

static int sCellIdx = -1;
static int sType = 0; // 0 单人积分历史排行  1 联盟积分历史排行  2 联盟积分联盟内部贡献度排行 3 王国单人历史排行 4 王国联盟历史排行 5 王国联盟内部贡献

SingleScoreHistoryRankView* SingleScoreHistoryRankView::create(int type)
{
    auto ret = new SingleScoreHistoryRankView();
    if (ret && ret->init(type)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool SingleScoreHistoryRankView::init(int type)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    auto tmpCCB = CCBLoadFile("SinglelHistoryListView", this, this);
    setContentSize(tmpCCB->getContentSize());
    
    sType = type;
    m_data = CCArray::create();
    
    if (sType == 1 || sType == 4) {
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(205, false);
        });
    }
    
    changeBGHeight(m_bg);
    float extH = getExtendHeight();
    m_infoList->setContentSize( m_infoList->getContentSize() + Size(0, extH) );
    m_infoList->setPositionY(m_infoList->getPositionY()-extH);
    
    if (sType == 0) {
        m_info1Label->setString(_lang("150316"));
        m_info2Label->setString(_lang("150315"));
    }
    else if (sType == 1) {
        m_info1Label->setString(_lang("150366"));
        m_info2Label->setString(_lang("150315"));
    }
    else if (sType == 2) {
        m_info1Label->setString(_lang("150380"));
        int curNum = 0;
        map<string, ActivityInfo*>::iterator it = ActivityController::getInstance()->m_singleScoreRwdMap.begin();
        for (; it != ActivityController::getInstance()->m_singleScoreRwdMap.end(); it++)
        {
            if (it->second->type == 2) {
                curNum = it->second->currentScore;
                break;
            }
        }
        m_info2Label->setString(_lang_1("150364", CC_ITOA(curNum)));
    }
    else if (sType == 3) {
        m_info1Label->setString(_lang("150316"));
        m_info2Label->setString(_lang("150315"));
    }
    else if (sType == 4) {
        m_info1Label->setString(_lang("150366"));
        m_info2Label->setString(_lang("150315"));
    }
    else if (sType == 5) {
        m_info1Label->setString(_lang("150380"));
        int curNum = 0;
        map<string, ActivityInfo*>::iterator it = ActivityController::getInstance()->m_kingScoreRwdMap.begin();
        for (; it != ActivityController::getInstance()->m_kingScoreRwdMap.end(); it++)
        {
            if (it->second->type == 4) {
                curNum = it->second->currentScore;
                break;
            }
        }
        m_info2Label->setString(_lang_1("150364", CC_ITOA(curNum)));
    }
    
    m_tableView = CCTableView::create(this, m_infoList->getContentSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_infoList->addChild(m_tableView);
    
    m_waitInterface = NULL;
    if (sType==0 && ActivityController::getInstance()->m_historyTimeVec.size()<=0) {
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        ActivityController::getInstance()->getHistroyRank(1);
    }else if (sType==1 && ActivityController::getInstance()->m_historyAllTimeVec.size()<=0) {
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        ActivityController::getInstance()->getHistroyRank(2);
    }else if (sType==2 && (ActivityController::getInstance()->m_allConUsers==NULL || ActivityController::getInstance()->m_allConUsers->count()<=0)) {
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        
        map<string, ActivityInfo*>::iterator it = ActivityController::getInstance()->m_allScoreRwdMap.begin();
        for (; it != ActivityController::getInstance()->m_allScoreRwdMap.end(); it++)
        {
            ActivityInfo* info = it->second;
            if (it->second->type == 2) {
                ActivityController::getInstance()->getAllianceConRank(info->activityId, 0);
                int curScore = it->second->currentScore;
                m_info2Label->setString(_lang_1("150364", CC_ITOA(curScore)));
                break;
            }
        }
        
    }
    else if (sType==3 && ActivityController::getInstance()->m_historyKingPlTimeVec.size()<=0) {//王国单人历史排行
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        ActivityController::getInstance()->getHistroyRank(3);
    }
    else if (sType==4 && ActivityController::getInstance()->m_historyKingAlRankMap.size()<=0) {//王国联盟历史排行
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        ActivityController::getInstance()->getHistroyRank(4);
    }else if (sType==5 && (ActivityController::getInstance()->m_kingAlConUsers==NULL || ActivityController::getInstance()->m_kingAlConUsers->count()<=0)) {
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        
        map<string, ActivityInfo*>::iterator it = ActivityController::getInstance()->m_kingScoreRwdMap.begin();
        for (; it != ActivityController::getInstance()->m_kingScoreRwdMap.end(); it++)
        {
            ActivityInfo* info = it->second;
            if (it->second->type == 4) {
                ActivityController::getInstance()->getAllianceConRank(info->activityId, 1);
                int curScore = it->second->currentScore;
                m_info2Label->setString(_lang_1("150364", CC_ITOA(curScore)));
                break;
            }
        }
        
    }else {
        refreshData();
    }
    
    return true;
}

void SingleScoreHistoryRankView::onEnter()
{
    CCNode::onEnter();
    if (sType == 0) {
        setTitleName(_lang("150314"));
    }
    else if (sType == 1) {
        setTitleName(_lang("150367"));
    }
    else if (sType == 2) {
        setTitleName(_lang("150370"));
    }
    else if (sType == 3) {
        setTitleName(_lang("150471"));
    }
    else if (sType == 4) {
        setTitleName(_lang("150472"));
    }
    else if (sType == 5) {
        setTitleName(_lang("150370"));
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SingleScoreHistoryRankView::retData), MSG_SCORE_RANK_HISTORY_VIEW, NULL);
}
void SingleScoreHistoryRankView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCORE_RANK_HISTORY_VIEW);
    CCNode::onExit();
}

void SingleScoreHistoryRankView::retData(CCObject* param)
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    refreshData();
}

void SingleScoreHistoryRankView::refreshData()
{
    m_data->removeAllObjects();
    vector<int> timeVec = ActivityController::getInstance()->m_historyTimeVec;
    
    if (sType==0) {
        timeVec = ActivityController::getInstance()->m_historyTimeVec;
    }else if (sType==1) {
        timeVec = ActivityController::getInstance()->m_historyAllTimeVec;
    }else if (sType==2) {
    }else if (sType==3) {
        timeVec = ActivityController::getInstance()->m_historyKingPlTimeVec;
    }else if (sType==4) {
        timeVec = ActivityController::getInstance()->m_historyKingAlTimeVec;
    }
    
    if (sType==0 || sType==1 || sType==3 || sType==4)
    {
        for (int i=0; i<timeVec.size(); i++)
        {
            time_t timeKey = timeVec[i];
            if (sType==0) {
                if (ActivityController::getInstance()->m_historyRankMap.find(timeKey) != ActivityController::getInstance()->m_historyRankMap.end() )
                {
                    auto titlDict = CCDictionary::create();
                    titlDict->setObject(CCString::create( _lang_1("150318", CCCommonUtils::timeStampToDate(timeKey).c_str()) ), "title");
                    m_data->addObject(titlDict);
                    
                    m_data->addObjectsFromArray( ActivityController::getInstance()->m_historyRankMap[timeKey] );
                }
            }else if (sType==1) {
                if (ActivityController::getInstance()->m_historyAllRankMap.find(timeKey) != ActivityController::getInstance()->m_historyAllRankMap.end() )
                {
                    auto titlDict = CCDictionary::create();
                    titlDict->setObject(CCString::create( _lang_1("150318", CCCommonUtils::timeStampToDate(timeKey).c_str()) ), "title");
                    m_data->addObject(titlDict);
                    
                    m_data->addObjectsFromArray( ActivityController::getInstance()->m_historyAllRankMap[timeKey] );
                }
            }else if (sType==3) {
                if (ActivityController::getInstance()->m_historyKingPlRankMap.find(timeKey) != ActivityController::getInstance()->m_historyKingPlRankMap.end() )
                {
                    auto titlDict = CCDictionary::create();
                    titlDict->setObject(CCString::create( _lang_1("150318", CCCommonUtils::timeStampToDate(timeKey).c_str()) ), "title");
                    m_data->addObject(titlDict);
                    
                    if (ActivityController::getInstance()->m_historyKingRankMap.find(timeKey) != ActivityController::getInstance()->m_historyKingRankMap.end() )
                    {
                        auto titlDict1 = CCDictionary::create();
                        titlDict1->setObject(CCString::create( _lang("150468") ), "topKing");
                        m_data->addObject(titlDict1);
                        m_data->addObjectsFromArray( ActivityController::getInstance()->m_historyKingRankMap[timeKey] );
                    }
                    auto titlDict2 = CCDictionary::create();
                    titlDict2->setObject(CCString::create( _lang("150469") ), "topKing");
                    m_data->addObject(titlDict2);
                    m_data->addObjectsFromArray( ActivityController::getInstance()->m_historyKingPlRankMap[timeKey] );
                }
            }else if (sType==4) {
                if (ActivityController::getInstance()->m_historyKingAlRankMap.find(timeKey) != ActivityController::getInstance()->m_historyKingAlRankMap.end() )
                {
                    auto titlDict = CCDictionary::create();
                    titlDict->setObject(CCString::create( _lang_1("150318", CCCommonUtils::timeStampToDate(timeKey).c_str()) ), "title");
                    m_data->addObject(titlDict);
                    
                    if (ActivityController::getInstance()->m_historyKingRankMap.find(timeKey) != ActivityController::getInstance()->m_historyKingRankMap.end() )
                    {
                        auto titlDict1 = CCDictionary::create();
                        titlDict1->setObject(CCString::create( _lang("150468") ), "topKing");
                        m_data->addObject(titlDict1);
                        m_data->addObjectsFromArray( ActivityController::getInstance()->m_historyKingRankMap[timeKey] );
                    }
                    
                    auto titlDict2 = CCDictionary::create();
                    titlDict2->setObject(CCString::create( _lang("150470") ), "topKing");
                    m_data->addObject(titlDict2);
                    m_data->addObjectsFromArray( ActivityController::getInstance()->m_historyKingAlRankMap[timeKey] );
                }
            }
        }
    }
    else
    {
        auto titlDict = CCDictionary::create();
        titlDict->setObject(CCString::create( _lang("150371") ), "title");
        m_data->addObject(titlDict);
        if (sType==2 && ActivityController::getInstance()->m_allConUsers) {
            m_data->addObjectsFromArray( ActivityController::getInstance()->m_allConUsers );
        }
        else if (sType==5 && ActivityController::getInstance()->m_kingAlConUsers) {
            m_data->addObjectsFromArray( ActivityController::getInstance()->m_kingAlConUsers );
        }
    }
    
    
    m_tableView->reloadData();
    
    if (m_data->count()<=0) {
        m_info3Label->setString(_lang("150343"));
    }
}

SEL_CCControlHandler SingleScoreHistoryRankView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", SingleScoreHistoryRankView::onTipBtnClick);
    return NULL;
}
bool SingleScoreHistoryRankView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info1Label", CCLabelIF*, this->m_info1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2Label", CCLabelIF*, this->m_info2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info3Label", CCLabelIF*, this->m_info3Label);
    return false;
}

void SingleScoreHistoryRankView::onTipBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45220");
//    if (sType==0) {
//        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("150317"),kCCTextAlignmentLeft));
//    }else if (sType==1) {
//        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("150374"),kCCTextAlignmentLeft));
//    }else if (sType==2) {
//        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("150374"),kCCTextAlignmentLeft));
//    }
}

CCSize SingleScoreHistoryRankView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return CCSizeZero;
    }
    return CCSizeMake(620, 60);
}

CCSize SingleScoreHistoryRankView::cellSizeForTable(  cocos2d::extension::TableView *table)
{
    return CCSizeMake(620, 60);
}

CCTableViewCell* SingleScoreHistoryRankView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return NULL;
    }
    CCDictionary* dict = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    SingleScoreHistoryRankCell* cell = dynamic_cast<SingleScoreHistoryRankCell*>(table->dequeueCell());
    if (cell == nullptr)
    {
        cell = SingleScoreHistoryRankCell::create();
    }
    
    if ( dict->objectForKey("title") )
    {
        string title = dict->valueForKey("title")->getCString();
        cell->setTitleData(title);
        if(sType == 2) {
            cell->setSubTitleData(_lang("150372"));
        }
    }
    else if ( dict->objectForKey("topKing") )
    {
        string title = dict->valueForKey("topKing")->getCString();
        cell->setTopData(title);
    }
    else
    {
        if (dict->objectForKey("banner"))
        { //王国排名数据 {"id":11,"rank":1,"name":"inner11","banner":"CN"}
            string server = CC_ITOA( dict->valueForKey("id")->intValue() );
            server = "#"+server;
            string name = dict->valueForKey("name")->getCString();
            string pic = dict->valueForKey("banner")->getCString();
            string rank = CC_ITOA(dict->valueForKey("rank")->intValue());
            // TODO : guojiang
            pic = ""; // CCCommonUtils::getCountryIconByName(pic);
            cell->setCountryData(rank, server, name, idx, pic);
        }
        else
        {
            if (sType == 0 || sType == 3) {
                string uid = dict->valueForKey("uid")->getCString();
                string server = CC_ITOA( dict->valueForKey("server")->intValue() );
                server = "#"+server;
                string name = dict->valueForKey("name")->getCString();
                string reward = dict->valueForKey("rewardId")->getCString();
                string rank = CC_ITOA(dict->valueForKey("rank")->intValue());
                int picVer = dict->valueForKey("picVer")->intValue();
                string pic = dict->valueForKey("pic")->getCString();
                
                cell->setItemData(rank, server, name, reward, idx, pic, picVer, uid);
            }
            else if (sType == 1 || sType == 4) {
                string uid = dict->valueForKey("allianceId")->getCString();
                string server = CC_ITOA( dict->valueForKey("server")->intValue() );
                server = "#"+server;
                string name = dict->valueForKey("name")->getCString();
                string abbr = dict->valueForKey("abbr")->getCString();
                string reward = dict->valueForKey("rewardId")->getCString();
                string rank = CC_ITOA(dict->valueForKey("rank")->intValue());
                string pic = dict->valueForKey("icon")->getCString();
                if (pic == "") {
                    pic = "Allance_flay";
                }
                
                name = "("+abbr+")"+name;
                cell->setItemData(rank, server, name, reward, idx, pic);
            }
            else if (sType == 2 || sType == 5) {
                string uid = dict->valueForKey("uid")->getCString();
                int picVer = dict->valueForKey("picVer")->intValue();
                string pic = dict->valueForKey("pic")->getCString();
                string name = dict->valueForKey("name")->getCString();
                string reward = "";
                string rank = CC_ITOA(dict->valueForKey("rank")->intValue());
                int score = dict->valueForKey("score")->intValue();
                string sv = "+ ";
                
                cell->setItemData(rank, "", name, reward, idx, pic, picVer, uid);
                cell->setValueData(sv+CC_ITOA(score));
            }
        }
    }
    
    return cell;
}

ssize_t SingleScoreHistoryRankView::numberOfCellsInTableView(  cocos2d::extension::TableView *table)
{
    return m_data->count();
}

/*
 SingleScoreHistoryRankCell
 */
SingleScoreHistoryRankCell* SingleScoreHistoryRankCell::create()
{
    auto ret = new SingleScoreHistoryRankCell();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}
bool SingleScoreHistoryRankCell::init()
{
    if (!CCNode::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("SingleHistoryListCell", this, this);
    setContentSize(CCSizeMake(620, 60));
    m_headImgNode = HFHeadImgNode::create();
    return true;
}
void SingleScoreHistoryRankCell::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SingleScoreHistoryRankCell::retData), GETRT_REWARD_INFO, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SingleScoreHistoryRankCell::refresh), FRESH_HIS_CELL, NULL);
    setTouchEnabled(true);
}
void SingleScoreHistoryRankCell::onExit()
{
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GETRT_REWARD_INFO);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FRESH_HIS_CELL);
    CCNode::onExit();
}
SEL_CCControlHandler SingleScoreHistoryRankCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool SingleScoreHistoryRankCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankNode", CCNode*, this->m_rankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankLabel", CCLabelIF*, this->m_rankLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerLabel", CCLabelIF*, this->m_playerLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverLabel", CCLabelIF*, this->m_serverLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rect", CCScale9Sprite*, this->m_rect);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueLabel", CCLabelIF*, this->m_valueLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverNode", CCNode*, this->m_serverNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headbg", CCSprite*, this->m_headbg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_albg", CCSprite*, this->m_albg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgGreen", CCSprite*, this->m_bgGreen);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgRed", CCSprite*, this->m_bgRed);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, this->m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subTitleLabel", CCLabelIF*, this->m_subTitleLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topLabel", CCLabelIF*, this->m_topLabel);
    return false;
}

void SingleScoreHistoryRankCell::setTitleData(string title)
{
    m_rewardId = "";
    m_titleLabel->setString(title);
    
    m_rankNode->setVisible(false);
    m_topNode->setVisible(false);
    m_titleNode->setVisible(true);
    m_cellIdx = -1;
    m_rect->setVisible(false);
}

void SingleScoreHistoryRankCell::setSubTitleData(string subTitle)
{
    m_subTitleLabel->setString(subTitle);
}

void SingleScoreHistoryRankCell::setTopData(string title)
{
    m_topLabel->setString(title);
    m_topNode->setVisible(true);
    m_titleNode->setVisible(false);
    m_rankNode->setVisible(false);
}

void SingleScoreHistoryRankCell::setValueData(string value)
{
    m_valueLabel->setString(value);
}

void SingleScoreHistoryRankCell::setItemData(string rank, string server, string player, string rewardId, int idx, string pic, int picVer, string uid)
{
    m_cellIdx = idx;
    m_rankLabel->setString(rank);
    m_serverLabel->setString(server);
    m_playerLabel->setString(player);
    m_rewardId = rewardId;
    
    m_pic = pic;
    m_picVer = picVer;
    
    m_rankNode->setVisible(true);
    m_titleNode->setVisible(false);
    m_topNode->setVisible(false);
    m_rect->setVisible(false);
    if (sCellIdx == m_cellIdx) {
        m_rect->setVisible(true);
    }
    
    m_iconNode->removeAllChildren();
    if ( sType == 2 || sType == 5 )
    {//联盟成员贡献度排行
        m_serverNode->setVisible(false);
        m_headNode->setPositionX(100);
        m_playerLabel->setPositionX(140);
        
        m_headbg->setVisible(true);
        m_albg->setVisible(false);
        
        if (m_pic!="") {
            m_pic = m_pic+".png";
            // TODO : guojiang
            auto head = CCLoadSprite::createSprite(m_pic.c_str());
//            auto head = CCLoadSprite::createSprite(m_pic.c_str(), true, CCLoadSpriteType_HEAD_ICON);
            CCCommonUtils::setSpriteMaxSize(head, 50);
            m_iconNode->addChild(head);
            if (CCCommonUtils::isUseCustomPic(m_picVer))
            {
                m_headImgNode->initHeadImgUrl2(m_iconNode, CCCommonUtils::getCustomPicUrl(uid, m_picVer), 1.0f, 50, true);
            }
        }
    }
    else {
        m_serverNode->setVisible(true);
        
        if (sType == 1 || sType == 4) {//联盟排行
            m_headbg->setVisible(false);
            m_albg->setVisible(true);
            if (m_pic!="") {
                m_pic = m_pic+".png";
                auto head = CCLoadSprite::createSprite(m_pic.c_str());
                CCCommonUtils::setSpriteMaxSize(head, 50);
                m_iconNode->addChild(head);
            }
        }
        else if (sType == 0 || sType == 3) {
            m_headbg->setVisible(true);
            m_albg->setVisible(false);
            
            if (m_pic!="") {
                m_pic = m_pic+".png";
                // TODO : guojiang
                auto head = CCLoadSprite::createSprite(m_pic.c_str());
//                auto head = CCLoadSprite::createSprite(m_pic.c_str(), true, CCLoadSpriteType_HEAD_ICON);
                CCCommonUtils::setSpriteMaxSize(head, 50);
                m_iconNode->addChild(head);
                if (CCCommonUtils::isUseCustomPic(m_picVer))
                {
                    m_headImgNode->initHeadImgUrl2(m_iconNode, CCCommonUtils::getCustomPicUrl(uid, m_picVer), 1.0f, 50, true);
                }
            }
        }
    }
    
    if (atoi(rank.c_str()) > 3) {
        m_bgRed->setVisible(false);
        m_bgGreen->setVisible(true);
    }else {
        m_bgRed->setVisible(true);
        m_bgGreen->setVisible(false);
    }
}

void SingleScoreHistoryRankCell::setCountryData(string rank, string server, string player, int idx, string pic)
{
    m_cellIdx = idx;
    m_rankLabel->setString(rank);
    m_serverLabel->setString(server);
    m_playerLabel->setString(player);
    m_rewardId = "";
    
    m_pic = pic;
    
    m_rankNode->setVisible(true);
    m_titleNode->setVisible(false);
    m_topNode->setVisible(false);
    m_rect->setVisible(false);
    
    m_iconNode->removeAllChildren();
    m_serverNode->setVisible(true);
    
    m_headbg->setVisible(false);
    m_albg->setVisible(false);
    if (m_pic!="") {
        auto head = CCLoadSprite::createSprite(m_pic.c_str());
        CCCommonUtils::setSpriteMaxSize(head, 50);
        m_iconNode->addChild(head);
    }
    
    if (atoi(rank.c_str()) > 3) {
        m_bgRed->setVisible(false);
        m_bgGreen->setVisible(true);
    }else {
        m_bgRed->setVisible(true);
        m_bgGreen->setVisible(false);
    }
}

void SingleScoreHistoryRankCell::retData(CCObject* param)
{
    if (m_cellIdx == sCellIdx) {
        showRwd();
    }
}

bool SingleScoreHistoryRankCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_rankNode->isVisible() && isTouchInside(m_rankNode,pTouch)){
        return true;
    }
    return false;
}

void SingleScoreHistoryRankCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int sY = pTouch->getStartLocation().y;
    int eY = pTouch->getLocation().y;
    if(fabs(sY-eY)>10){
        return;
    }
    
    int rewardId = atoi( m_rewardId.c_str() );
    if (isTouchInside(m_rankNode,pTouch) && rewardId>0){
        if (QuestController::getInstance()->m_stageRDMap.find(rewardId) != QuestController::getInstance()->m_stageRDMap.end()) {
            sCellIdx = m_cellIdx;
            showRwd();
        }
        else {
            sCellIdx = m_cellIdx;
            QuestController::getInstance()->startGetRewardById(rewardId);
        }
    }
}

void SingleScoreHistoryRankCell::refresh(CCObject* param)
{
    m_rect->setVisible(false);
}

void SingleScoreHistoryRankCell::showRwd()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(FRESH_HIS_CELL);
    m_rect->setVisible(true);

    int rewardId = atoi( m_rewardId.c_str() );
    if (QuestController::getInstance()->m_stageRDMap.find(rewardId) != QuestController::getInstance()->m_stageRDMap.end()) {
        //
        
        auto dict = CCDictionary::create();
        auto itemEffectObj = CCDictionary::create();
        CCArray* tmpArr = QuestController::getInstance()->m_stageRDMap[rewardId];
        
        auto newArr = CCArray::createWithArray(tmpArr);
        
        itemEffectObj->setObject(newArr, "reward");
        auto tmparray = CCArray::create();
        dict->setObject(itemEffectObj, "itemEffectObj");
        dict->setObject(CCString::create(CC_ITOA(1)), "day");
        PopupViewController::getInstance()->addPopupView(ChestRDView::create(dict),false,false);
        
    }
    
}