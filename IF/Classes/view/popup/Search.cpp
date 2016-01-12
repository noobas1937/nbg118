//
//  Search.cpp
//  IF
//
//  Created by xxrdsg on 15-2-4.
//
//

#include "Search.h"
#include "SoundController.h"
#include "../../Ext/CCDevice.h"
#include "PopupViewController.h"
#include "AllianceFlagPar.h"
#include "SoundController.h"
#include "CheckAllianceInfoView.h"
#include "SearchCommand.h"
#include "RoleInfoView.h"
#include "MailController.h"
#include "AllianceManager.h"
#include "ApplyAllianceCommand.h"
#include "AllianceCommentCheck.h"
#include "CancelAllianceApplyCommand.h"
#include "ChatView.h"
#include "GameController.h"
#include "ParticleFireAni.h"
#include "AllianceInfoView.h"
#include "ChatController.h"
#include "ParticleController.h"
#include "AllianceInfoMembersView.h"
#include "YesNoDialog.h"

SearchView* SearchView::create()
{
    SearchView* ret = new SearchView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SearchView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCLoadSprite::doResourceByCommonIndex(307, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(205, false);
        CCLoadSprite::doResourceByCommonIndex(307, false);
    });
    
//    auto tbg = CCLoadSprite::createSprite("technology_09.png");
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//    int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
//    int curHeight = 0;
//    while (curHeight < maxHeight) {
//        auto bg = CCLoadSprite::createSprite("technology_09.png");
//        bg->setAnchorPoint(ccp(0, 0));
//        bg->setPosition(ccp(0, curHeight));
//        tBatchNode->addChild(bg);
//        curHeight += bg->getContentSize().height;
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            bg->setScale(2.4);
//        }
//    }
//    this->addChild(tBatchNode);
    
    auto tmpCCB = CCBLoadFile("SearchView", this, this);
    this->setContentSize(tmpCCB->getContentSize());
    int dh = CCDirector::sharedDirector()->getWinSize().height - 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    m_listNode->setPositionY(m_listNode->getPositionY() - dh);
    m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + dh));
    m_bgNode->setPositionY(m_bgNode->getPositionY() - dh);
//    tBatchNode->setPositionY(tBatchNode->getPositionY() - dh);
    
    auto spriteText = CCLoadSprite::createScale9Sprite("nb_alliance_search_bg.png");
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_editBox = CCEditBox::create(CCSizeMake(420*2.2, 44*2.2), spriteText);
    }
    else
    {
        m_editBox = CCEditBox::create(CCSizeMake(420, 44), spriteText);
        
    }
    m_editBox->setMaxLength(100);
    m_editBox->setFontSize(24);
    m_editBox->setFontColor(ccWHITE);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_editBox->setText("");
    m_editBox->setPlaceHolder("");
    m_editNode->addChild(m_editBox);
    
    m_tableView = CCMultiColTableView::create(this, m_listNode->getContentSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setTouchPriority(Touch_Default);
    m_tableView->setMultiColTableViewDelegate(this);
    m_listNode->addChild(m_tableView);
    
    m_loadingIcon = NULL;
    m_playerBtn->setEnabled(true);
    m_allianceBtn->setEnabled(true);
    CCCommonUtils::setButtonTitle(m_playerBtn, _lang("108101").c_str());
    CCCommonUtils::setButtonTitle(m_allianceBtn, _lang("108102").c_str());
    CCCommonUtils::setButtonTitle(m_searchBtn, _lang("115013").c_str());
    m_allianceData = CCArray::create();
    m_playerData.clear();
    m_waitInterface = NULL;
    m_page = 1;
    m_playerNumPerPage = 0;
    m_allianceNumPerPage = 0;
    m_maxPlayerNum = 0;
    m_maxAllianceNum = 0;
    
    return true;
}

void SearchView::onEnter()
{
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
    setTitleName(_lang("113907"));
    if (m_page == 1) {
        m_playerBtn->setEnabled(false);
        m_allianceBtn->setEnabled(true);
        if (m_playerData.size() == 0) {
            generateTabView();
        } else {
            generateTabViewMore();
        }
    } else {
        m_playerBtn->setEnabled(true);
        m_allianceBtn->setEnabled(false);
        generateTabViewMore();
    }
}

void SearchView::onExit()
{
    if(m_waitInterface != NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    removeLoadingAni();
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void SearchView::generateTabView()
{
    m_tableView->reloadData();
}

void SearchView::generateTabViewMore()
{
    float curOffsetY = m_tableView->getContentOffset().y;
    float curMinOffsetY = m_tableView->minContainerOffset().y;
    m_tableView->reloadData();
    float newMinOffsetY = m_tableView->minContainerOffset().y;
    m_tableView->setContentOffset(ccp(0, newMinOffsetY - curMinOffsetY + curOffsetY));
//    m_tableView->setContentOffsetInDuration(ccp(0, newMinOffsetY - curMinOffsetY + curOffsetY + 200), 0.5);
}

void SearchView::onClickPlayerBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_page = 1;
    m_playerBtn->setEnabled(false);
    m_allianceBtn->setEnabled(true);
    generateTabView();
}

void SearchView::onClickAllianceBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_page = 2;
    m_playerBtn->setEnabled(true);
    m_allianceBtn->setEnabled(false);
    generateTabView();
}

void SearchView::onClickSearchBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    string name = m_editBox->getText();
    if (m_page == 1) {
        if (name.length() < 3) {
            m_playerData.clear();
            m_totalPlayerNum = 0;
            m_lastPlayerSearch = name;
            generateTabView();
            YesNoDialog::showYesDialog(_lang_1("113909", "3"));
            return;
        }
        if (strcmp(m_lastPlayerSearch.c_str(), name.c_str()) == 0) {
            generateTabView();
            return;
        }
        m_playerData.clear();
        m_totalPlayerNum = 0;
        addLoadingAni();
        if(m_waitInterface != NULL){
            m_waitInterface->remove();
            m_waitInterface = NULL;
        }
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_searchBtn);
        SearchPlayerCommand* cmd = new SearchPlayerCommand(name, 1);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(SearchView::getPlayerData), NULL));
        cmd->sendAndRelease();
    } else {
        if (name.length() < 3) {
            m_allianceData->removeAllObjects();
            m_totalPlayerNum = 0;
            m_lastAllianceSearch = name;
            generateTabView();
            YesNoDialog::showYesDialog(_lang_1("113997", "3"));
            return;
        }
        if (strcmp(m_lastAllianceSearch.c_str(), name.c_str()) == 0) {
            generateTabView();
            return;
        }
        m_allianceData->removeAllObjects();
        m_totalAllianceNum = 0;
        addLoadingAni();
        if(m_waitInterface != NULL){
            m_waitInterface->remove();
            m_waitInterface = NULL;
        }
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_searchBtn);
        SearchAllianceCommand* cmd = new SearchAllianceCommand(name, 1);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(SearchView::getAllianceData), NULL));
        cmd->sendAndRelease();
    }
}

void SearchView::addLoadingAni(){
    if(m_loadingIcon){
        m_listNode->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
    m_tableView->setTouchEnabled(false);
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_listNode->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_listNode->addChild(m_loadingIcon,1000000);
}

void SearchView::removeLoadingAni(){
    if(m_loadingIcon){
        m_listNode->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
    if (m_tableView) {
        m_tableView->setTouchEnabled(true);
    }
}

void SearchView::getPlayerData(CCObject* param){
    if(m_waitInterface != NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    removeLoadingAni();
    NetResult* netResult = dynamic_cast<NetResult*>(param);
    auto dict = _dict(netResult->getData());
    if (dict->objectForKey("errorCode")) {
        string code = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(code));
        generateTabView();
        return;
    }
    if (!dict->valueForKey("page")) {
        generateTabView();
        return;
    }
    int page = dict->valueForKey("page")->intValue();
    if (page == 1) {
        
        m_playerData.clear();
        m_lastPlayerSearch = dict->valueForKey("key")->getCString();
        m_totalPlayerNum = dict->valueForKey("total")->intValue();
        if (m_totalPlayerNum == 0) {
            generateTabView();
            return;
        }
        CCArray* list = dynamic_cast<CCArray*>(dict->objectForKey("list"));
        for (int i = 0; i < list->count(); ++i) {
            CCDictionary* item = _dict(list->objectAtIndex(i));
            m_playerData.push_back(PlayerSearchInfo(item));
        }
        if (list->count() < m_totalPlayerNum) {
            m_playerNumPerPage = list->count();
        }
        if (dict->objectForKey("max")) {
            m_maxPlayerNum = dict->valueForKey("max")->intValue();
        }
        generateTabView();
    } else {
        CCArray* list = dynamic_cast<CCArray*>(dict->objectForKey("list"));
        for (int i = 0; i < list->count(); ++i) {
            CCDictionary* item = _dict(list->objectAtIndex(i));
            m_playerData.push_back(PlayerSearchInfo(item));
        }
        generateTabViewMore();
    }
}

void SearchView::getAllianceData(CCObject* param)
{
    if(m_waitInterface != NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    removeLoadingAni();
    NetResult* netResult = dynamic_cast<NetResult*>(param);
    auto dict = _dict(netResult->getData());
    if (dict->objectForKey("errorCode")) {
        string code = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(code));
        generateTabView();
        return;
    }
    int page = dict->valueForKey("page")->intValue();
    if (page == 1) {
        m_allianceData->removeAllObjects();
        m_lastAllianceSearch = dict->valueForKey("key")->getCString();
        m_totalAllianceNum = dict->valueForKey("total")->intValue();
        if (m_totalAllianceNum == 0) {
            generateTabView();
            return;
        }
        CCArray* list = dynamic_cast<CCArray*>(dict->objectForKey("list"));
        for (int i = 0; i < list->count(); ++i) {
            CCDictionary* item = _dict(list->objectAtIndex(i));
            AllianceInfo* allianceInfo = new AllianceInfo();
            allianceInfo->updateAllianceInfo(item);
            m_allianceData->addObject(allianceInfo);
            allianceInfo->release();
        }
        if (list->count() < m_totalAllianceNum) {
            m_allianceNumPerPage = list->count();
        }
        if (dict->objectForKey("max")) {
            m_maxAllianceNum = dict->valueForKey("max")->intValue();
        }
        generateTabView();
    } else {
        if (dict->objectForKey("list")) {
            CCArray* list = dynamic_cast<CCArray*>(dict->objectForKey("list"));
            for (int i = 0; i < list->count(); ++i) {
                CCDictionary* item = _dict(list->objectAtIndex(i));
                AllianceInfo* allianceInfo = new AllianceInfo();
                allianceInfo->updateAllianceInfo(item);
                m_allianceData->addObject(allianceInfo);
                allianceInfo->release();
            }
        }
        generateTabViewMore();
    }
}

bool SearchView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void SearchView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void SearchView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_page == 1) {
        if (m_tableView->getContentOffset().y > 30 && m_playerData.size() && m_playerData.size() < m_totalPlayerNum && m_playerNumPerPage && m_playerData.size() < m_maxPlayerNum) {
            addLoadingAni();
            if(m_waitInterface != NULL){
                m_waitInterface->remove();
                m_waitInterface = NULL;
            }
            m_waitInterface = GameController::getInstance()->showWaitInterface(m_searchBtn);
            m_tableView->setBounceable(false);
            this->scheduleOnce(schedule_selector(SearchView::callMore), 0.0);
        }
    } else {
        if (m_tableView->getContentOffset().y > 30 && m_allianceData->count() && m_allianceData->count() < m_totalAllianceNum && m_allianceNumPerPage && m_allianceData->count() < m_maxAllianceNum) {
            addLoadingAni();
            if(m_waitInterface != NULL){
                m_waitInterface->remove();
                m_waitInterface = NULL;
            }
            m_waitInterface = GameController::getInstance()->showWaitInterface(m_searchBtn);
            m_tableView->setBounceable(false);
            this->scheduleOnce(schedule_selector(SearchView::callMore), 0.0);
        }
    }
}

void SearchView::callMore(float t)
{
    m_tableView->setBounceable(true);
    if (m_page == 1) {
        int nextPage = m_playerData.size() / m_playerNumPerPage  + 1;
        SearchPlayerCommand* cmd = new SearchPlayerCommand(m_lastPlayerSearch, nextPage);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(SearchView::getPlayerData), NULL));
        cmd->sendAndRelease();
    } else {
        int nextPage = m_allianceData->count() / m_allianceNumPerPage + 1;
        SearchAllianceCommand* cmd = new SearchAllianceCommand(m_lastAllianceSearch, nextPage);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(SearchView::getAllianceData), NULL));
        cmd->sendAndRelease();
    }
}

void SearchView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
    
}

cocos2d::CCSize SearchView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    if (m_page == 1) {
        if(CCCommonUtils::isIosAndroidPad())
        {
            return CCSize(320*2, 220*2);
        }
        return CCSize(320, 220);
    } else {
        if(CCCommonUtils::isIosAndroidPad())
        {
            return CCSize(640*2, 220*2);
        }
        return CCSize(640, 220);
    }
}

CCTableViewCell* SearchView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if (m_page == 1) {
        
        if (m_playerData.size() == 0) {
            if (m_lastPlayerSearch.length() >= 3) {
                if (idx == 0) {
                    SearchCell* cell = (SearchCell*)table->dequeueGrid();
                    if (cell) {
                        cell->setData(NULL, NULL, m_listNode, 4, 1);
                    } else {
                        cell = SearchCell::create(NULL, NULL, m_listNode, 4, 1);
                    }
                    return cell;
                } else {
                    return NULL;
                }
            } else {
                if (idx == 0) {
                    SearchCell* cell = (SearchCell*)table->dequeueGrid();
                    if (cell) {
                        cell->setData(NULL, NULL, m_listNode, 5, 1);
                    } else {
                        cell = SearchCell::create(NULL, NULL, m_listNode, 5, 1);
                    }
                    return cell;
                } else {
                    return NULL;
                }
            }

        }
        
        if (idx < m_playerData.size()) {
            SearchCell* cell = (SearchCell*)table->dequeueGrid();
            PlayerSearchInfo* info = &m_playerData[idx];
            if (cell) {
                cell->setData(info, NULL, m_listNode, 0, 1);
            } else {
                cell = SearchCell::create(info, NULL, m_listNode, 0, 1);
            }
            return cell;
        } else if (idx == m_playerData.size()) {
            if (m_playerData.size() % 2 == 0) {
                SearchCell* cell = (SearchCell*)table->dequeueGrid();
                int type = 0;
                if (m_playerData.size() < m_totalPlayerNum && m_playerData.size() < m_maxPlayerNum) {
                    type = 1;
                } else if (m_playerData.size() == m_totalPlayerNum) {
                    type = 3;
                } else if (m_playerData.size() == m_maxPlayerNum) {
                    type = 2;
                }
                if (cell) {
                    cell->setData(NULL, NULL, m_listNode, type, 1);
                } else {
                    cell = SearchCell::create(NULL, NULL, m_listNode, type, 1);
                }
                return cell;
            } else {
                return NULL;
            }
        } else {
            if (m_playerData.size() % 2 == 0) {
                return NULL;
            } else {
                if (idx == m_playerData.size() + 1) {
                    int type = 0;
                    if (m_playerData.size() < m_totalPlayerNum && m_playerData.size() < m_maxPlayerNum) {
                        type = 1;
                    } else if (m_playerData.size() == m_totalPlayerNum) {
                        type = 3;
                    } else if (m_playerData.size() == m_maxPlayerNum) {
                        type = 2;
                    }
                    SearchCell* cell = (SearchCell*)table->dequeueGrid();
                    if (cell) {
                        cell->setData(NULL, NULL, m_listNode, type, 1);
                    } else {
                        cell = SearchCell::create(NULL, NULL, m_listNode, type, 1);
                    }
                    return cell;
                } else {
                    return NULL;
                }
            }
        }
        
    } else {
        
        if (m_allianceData->count() == 0) {
            if (m_lastAllianceSearch.length() >= 3) {
                if (idx == 0) {
                    SearchCell* cell = (SearchCell*)table->dequeueGrid();
                    if (cell) {
                        cell->setData(NULL, NULL, m_listNode, 4, 2);
                    } else {
                        cell = SearchCell::create(NULL, NULL, m_listNode, 4, 2);
                    }
                    return cell;
                } else {
                    return NULL;
                }
            } else {
                if (idx == 0) {
                    SearchCell* cell = (SearchCell*)table->dequeueGrid();
                    if (cell) {
                        cell->setData(NULL, NULL, m_listNode, 5, 2);
                    } else {
                        cell = SearchCell::create(NULL, NULL, m_listNode, 5, 2);
                    }
                    return cell;
                } else {
                    return NULL;
                }
            }
        }
        
        if (idx < m_allianceData->count()) {
            AllianceInfo* info = dynamic_cast<AllianceInfo*>(m_allianceData->objectAtIndex(idx));
            SearchCell* cell = (SearchCell*)table->dequeueGrid();
            if (cell) {
                cell->setData(NULL, info, m_listNode, 0, 2);
            } else {
                cell = SearchCell::create(NULL, info, m_listNode, 0, 2);
            }
            return cell;
        } else if (idx == m_allianceData->count()) {
            SearchCell* cell = (SearchCell*)table->dequeueGrid();
            int type = 0;
            if (m_allianceData->count() < m_totalAllianceNum && m_allianceData->count() < m_maxAllianceNum) {
                type = 1;
            } else if (m_allianceData->count() == m_totalAllianceNum) {
                type = 3;
            } else if (m_allianceData->count() == m_maxAllianceNum) {
                type = 2;
            }
            if (cell) {
                cell->setData(NULL, NULL, m_listNode, type, 2);
            } else {
                cell = SearchCell::create(NULL, NULL, m_listNode, type, 2);
            }
            return cell;
        } else {
            return NULL;
        }
        
    }
}

ssize_t SearchView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table)
{
    if (m_page == 1) {
        return ceil(m_playerData.size() / 2.0) + 1;
    } else {
        return m_allianceData->count() + 1;
    }
}

unsigned int SearchView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable)
{
    if (m_page == 1) {
        return 2;
    } else {
        return 1;
    }
}

SEL_CCControlHandler SearchView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickPlayerBtn", SearchView::onClickPlayerBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAllianceBtn", SearchView::onClickAllianceBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSearchBtn", SearchView::onClickSearchBtn);
    return NULL;
}

bool SearchView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerBtn", CCControlButton*, m_playerBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceBtn", CCControlButton*, m_allianceBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchBtn", CCControlButton*, m_searchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_editNode", CCNode*, m_editNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNode", CCNode*, m_bgNode);
    return false;
}


/////Cell
SearchCell* SearchCell::create(PlayerSearchInfo * playerInfo, AllianceInfo * allianceInfo, CCNode *containerNode, int tipType, int showType)
{
    auto ret = new SearchCell();
    if (ret && ret->init(playerInfo, allianceInfo, containerNode, tipType, showType))
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SearchCell::init(PlayerSearchInfo * playerInfo, AllianceInfo * allianceInfo, CCNode *containerNode, int tipType, int showType)
{
    CCBLoadFile("SearchCell", this, this);
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setScale(2.0);
    }
    setData(playerInfo, allianceInfo, containerNode, tipType, showType);
    return true;
}

void SearchCell::setData(PlayerSearchInfo * playerInfo, AllianceInfo * allianceInfo, CCNode* containerNode, int tipType, int showType)
{
    ///////////
    /////  tipType = 0 表示默认为空
    /////  tipType = 1 拖动以获得更多
    /////  tipType = 2 无法精确查找到该领主
    /////  tipType = 3 无更多结果
    /////  tipType = 4 搜不到
    /////  tipType = 5 可输入领主名3个或者以上的字符进行领主搜索
    ////////////
    
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCLoadSprite::doResourceByCommonIndex(307, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(205, false);
        CCLoadSprite::doResourceByCommonIndex(307, false);
    });
    
    m_playerInfo = playerInfo;
    m_allianceInfo = allianceInfo;
    m_containerNode = containerNode;
    m_tipType = tipType;
    m_showType = showType;
    
    m_playerNode->setVisible(false);
    m_allianceNode->setVisible(false);
    m_tipLabel->setString("");
    m_playerIconNode->removeAllChildrenWithCleanup(true);
    m_allianceIconNode->removeAllChildrenWithCleanup(true);
    if (showType == 1) {
        if(m_tipType == 1) {
            m_tipLabel->setString(_lang("115189"));
        } else if (m_tipType == 2) {
            m_tipLabel->setString(_lang("113911"));
        } else if (m_tipType == 3) {
            m_tipLabel->setString(_lang("113912"));
        } else if (m_tipType == 4) {
            m_tipLabel->setString(_lang("113910"));
        } else if (m_tipType == 5) {
            m_tipLabel->setString(_lang_1("113909", "3"));
        }
    } else {
        if(m_tipType == 1) {
            m_tipLabel->setString(_lang("115189"));
        } else if (m_tipType == 2) {
            m_tipLabel->setString(_lang("113999"));
        } else if (m_tipType == 3) {
            m_tipLabel->setString(_lang("113912"));
        } else if (m_tipType == 4) {
            m_tipLabel->setString(_lang("113998"));
        } else if (m_tipType == 5) {
            m_tipLabel->setString(_lang_1("113997", "3"));
        }
    }
    

    
    if (m_showType == 1) {
        if (m_playerInfo == NULL) {
            m_playerNode->setVisible(false);
            return;
        }
        m_playerNode->setVisible(true);
        string str = m_playerInfo->pic;
        if (str == "" || str == "0") {
            str.clear();
            str.append("g044_middle.png");
        } else {
            str.append("_middle.png");
        }
        CCSprite* spr = CCLoadSprite::createSprite(str.c_str());
        spr->setScale(0.5);
        spr->setAnchorPoint({0.5, 0.0});
        m_playerIconNode->addChild(spr, 0, 444);
        m_playerNameLabel->setString(m_playerInfo->name.c_str());
        m_myAlliLabel->setString(_lang("108102"));
        if (m_playerInfo->abbr != "") {
            m_myAlliNameLabel->setString(m_playerInfo->abbr.c_str());
        } else {
            m_myAlliNameLabel->setString(_lang("108714"));
        }
        // m_flagBg->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Alliance_Flag_01.png")->getCString()));
        if (m_playerInfo->serverId >= 0) {
            m_kingdomLabel->setString(_lang("108755") + CC_ITOA(m_playerInfo->serverId));
        } else {
            m_kingdomLabel->setString(_lang("108755"));
        }
    } else if (m_showType == 2) {
        if (m_allianceInfo == NULL) {
            m_allianceNode->setVisible(false);
            return;
        }
        m_allianceNode->setVisible(true);
        string str = "(";
        str.append(m_allianceInfo->shortName.c_str());
        str.append(") ");
        str.append(m_allianceInfo->name.c_str());
        m_alliNameLabel->setString(str);
        m_leaderLabel->setString(_lang_1("115015", m_allianceInfo->leader.c_str()));
        m_numLabel->setString(CCString::createWithFormat("%d/%d", m_allianceInfo->currentNum, m_allianceInfo->maxNum)->getCString());
        m_forceLabel->setString(CC_CMDITOA(m_allianceInfo->totalForce).c_str());
        m_allianceIconNode->removeAllChildrenWithCleanup(true);
        auto flag = AllianceFlagPar::create(m_allianceInfo->getAllianceIcon().c_str());
        m_allianceIconNode->addChild(flag, 0, 444);
        m_alliLanLabel->setString(_lang(m_allianceInfo->language));
    }
    
    
}

void SearchCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
}

void SearchCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool SearchCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_showType == 1) {
        if (m_playerInfo == NULL) {
            return false;
        }
        if((isTouchInside(m_clickNode1, pTouch) || isTouchInside(m_clickNode2, pTouch)) && isTouchInside(m_containerNode, pTouch)){
            if (m_tipType == 0) {
                m_flagBg->setColor(ccc3(128, 128, 128));
                ((CCSprite*)(m_playerIconNode->getChildByTag(444)))->setColor(ccc3(128, 128, 128));
            }
            return true;
        }
        return false;
    } else {
        if (m_allianceInfo == NULL) {
            return false;
        }
        if (isTouchInside(m_clickNode, pTouch) && isTouchInside(m_containerNode, pTouch)) {
            if (m_tipType == 0) {
                m_bg->setColor(ccc3(128, 128, 128));
                ((CCSprite*)(m_allianceIconNode->getChildByTag(444)->getChildByTag(444)))->setColor(ccc3(128, 128, 128));
            }
            return true;
        }
        return false;
    }
}

void SearchCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_showType == 1) {
        if ( m_playerInfo!= NULL) {
            m_flagBg->setColor(ccc3(255, 255, 255));
            ((CCSprite*)(m_playerIconNode->getChildByTag(444)))->setColor(ccc3(255, 255, 255));
        } else {
            return;
        }
        if((isTouchInside(m_clickNode1, pTouch) || isTouchInside(m_clickNode2, pTouch)) && isTouchInside(m_containerNode, pTouch)){
            int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
            int dy = pTouch->getLocation().y - pTouch->getStartLocation().y;
            if (fabs(dx) > 10 || fabs(dy) > 10 ) {
                return;
            }
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            PopupViewController::getInstance()->addPopupView(SearchPlayerInfoBtnView::create(m_playerInfo));
        }
    } else {
        if (m_allianceInfo != NULL) {
            m_bg->setColor(ccc3(255, 255, 255));
            ((CCSprite*)(m_allianceIconNode->getChildByTag(444)->getChildByTag(444)))->setColor(ccc3(255, 255, 255));
        } else {
            return;
        }
        if (isTouchInside(m_clickNode, pTouch) && isTouchInside(m_containerNode, pTouch)) {
            int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
            int dy = pTouch->getLocation().y - pTouch->getStartLocation().y;
            if (fabs(dx) > 10 || fabs(dy) > 10 ) {
                return;
            }
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            PopupViewController::getInstance()->addPopupInView(SearchAllianceInfoView::create(m_allianceInfo));
        }
    }
}

SEL_CCControlHandler SearchCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}


bool SearchCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipLabel", CCLabelIF*, m_tipLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerNode", CCNode*, m_playerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode1", CCNode*, m_clickNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode2", CCNode*, m_clickNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerIconNode", CCNode*, m_playerIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_flagBg", CCSprite*, m_flagBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_myAlliLabel", CCLabelIF*, m_myAlliLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_myAlliNameLabel", CCLabelIF*, m_myAlliNameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingdomLabel", CCLabelIF*, m_kingdomLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerNameLabel", CCLabelIF*, m_playerNameLabel);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceNode", CCNode*, m_allianceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliNameLabel", CCLabelIF*, m_alliNameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leaderLabel", CCLabelIF*, m_leaderLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forceLabel", CCLabelIF*, m_forceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alliLanLabel", CCLabelIF*, m_alliLanLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIconNode", CCNode*, m_allianceIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    return false;
}

/////SearchPlayerInfoBtnView
SearchPlayerInfoBtnView* SearchPlayerInfoBtnView::create(PlayerSearchInfo* info)
{
    auto ret = new SearchPlayerInfoBtnView();
    if (ret && ret->init(info)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SearchPlayerInfoBtnView::init(PlayerSearchInfo* info)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCBLoadFile("SearchPlayerInfoBtnView", this, this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    m_info = info;
    CCCommonUtils::setButtonTitle(m_viewBtn, _lang("105309").c_str());
    CCCommonUtils::setButtonTitle(m_mailBtn, _lang("105308").c_str());
    return true;
}

void SearchPlayerInfoBtnView::onClickViewBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    string uid = m_info->uid;
    PopupViewController::getInstance()->removePopupView(this);
    RoleInfoView::createInfoByUid(uid);
}

void SearchPlayerInfoBtnView::onClickMailBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    string name = m_info->name;
    string uid = m_info->uid;
    PopupViewController::getInstance()->removePopupView(this);
    MailController::getInstance()->openMailDialogViewFirst(name, uid);
}

void SearchPlayerInfoBtnView::onEnter()
{
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void SearchPlayerInfoBtnView::onExit()
{
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool SearchPlayerInfoBtnView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_bg, pTouch)) {
        return false;
    }
    return true;
}

void SearchPlayerInfoBtnView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_bg, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler SearchPlayerInfoBtnView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickViewBtn", SearchPlayerInfoBtnView::onClickViewBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMailBtn", SearchPlayerInfoBtnView::onClickMailBtn);
    return NULL;
}
bool SearchPlayerInfoBtnView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBtn", CCControlButton*, m_viewBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtn", CCControlButton*, m_mailBtn);
    return false;
}

/////////SearchAllianceInfoView
SearchAllianceInfoView* SearchAllianceInfoView::create(AllianceInfo* info){
    SearchAllianceInfoView* ret = new SearchAllianceInfoView();
    if(ret && ret->init(info)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool SearchAllianceInfoView::init(AllianceInfo* info)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(205, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
//    auto tbg = CCLoadSprite::loadResource("technology_09.png");
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//    int curHight = -500;
//    while (curHight<maxHight) {
//        auto bg = CCLoadSprite::createSprite("technology_09.png");
//        bg->setAnchorPoint(ccp(0, 1));
//        bg->setPosition(ccp(0, curHight));
//        curHight += bg->getContentSize().height;
//        tBatchNode->addChild(bg);
//    }
//    if (CCCommonUtils::isIosAndroidPad()) {
//        tBatchNode->setScaleX(2.4);
//    }
//    this->addChild(tBatchNode);
    
    auto tmpCCB = CCBLoadFile("SearchAllianceInfoView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    int preH = m_viewBg->getContentSize().height;
    changeBGHeight(m_viewBg);
    int newH = m_viewBg->getContentSize().height;
    int addH = newH- preH;
    if (CCCommonUtils::isIosAndroidPad()) {
        addH = CCDirector::sharedDirector()->getWinSize().height - 2048;
        addH = addH / 2.4;
    }
    m_bottomNode->setVisible(false);
    m_viewBg->setVisible(false);
    if(!GlobalData::shared()->playerInfo.isInAlliance()){
        addH += 0;
        m_bottomNode->setVisible(true);
    }else{
        addH += 100;
    }
    m_funList->setContentSize(CCSize(m_funList->getContentSize().width,m_funList->getContentSize().height+addH));
    m_funList->setPositionY(m_funList->getPositionY() - addH);
    
    m_info = info;
    
    if(m_info!=NULL){
        std::string str = "(";
        str.append(m_info->shortName.c_str());
        str.append(") ");
        str.append(m_info->name.c_str());
        m_nameTxt->setString(str.c_str());
        if(GlobalData::shared()->playerInfo.isInAlliance() && m_info->uid==GlobalData::shared()->playerInfo.allianceInfo.uid){
            str = _lang_2("115016", CC_ITOA(m_info->currentNum),CC_ITOA(m_info->getSelfAllianceNum()));
        }else{
            str = _lang_2("115016", CC_ITOA(m_info->currentNum),CC_ITOA(m_info->maxNum));
        }
        m_membersTxt->setString(str);
        str = _lang_1("115015", m_info->leader.c_str());
        m_leaderTxt->setString(str.c_str());
        str = _lang_1("115017", CC_CMDITOAL(m_info->totalForce).c_str());
        m_powerTxt->setString(str);
    }
    
    m_allianceIcon->removeAllChildrenWithCleanup(true);
    
    m_applyBtn->setVisible(false);
    m_joinAlliance->setVisible(false);
    m_rovkeBtn->setVisible(false);
    //    addBtnPar(m_rovkeBtn);
    addBtnPar(m_joinAlliance);
    addBtnPar(m_applyBtn);
    m_bottomNode->setVisible(false);
    m_bottomNode->setPositionY(m_bottomNode->getPositionY()-addH);
    
    CCCommonUtils::setButtonTitle(m_applyBtn, _lang("115019").c_str());
    CCCommonUtils::setButtonTitle(m_joinAlliance, _lang("115020").c_str());
    CCCommonUtils::setButtonTitle(m_rovkeBtn, _lang("115021").c_str());
    
    AllianceFlagPar* flag = AllianceFlagPar::create(m_info->getAllianceIcon().c_str());
    m_allianceIcon->addChild(flag);
    
    m_scrollView = CCScrollView::create(m_funList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Popup);
    m_funList->addChild(m_scrollView,10000);
    
    CCSize size = m_funList->getContentSize();
    m_textNode->setPositionY(0);
    m_textNode->removeFromParent();
    m_scrollView->addChild(m_textNode);
    m_scrollView->setContentSize(CCSize(m_funList->getContentSize().width,60));
    m_scrollView->setContentOffset(ccp(0, m_funList->getContentSize().height - 60));
    
    ParticleFireAni* par = ParticleFireAni::create();
    m_fireNode1->addChild(par);
    
    ParticleFireAni* par2 = ParticleFireAni::create();
    m_fireNode2->addChild(par2);
    
    if(!GlobalData::shared()->playerInfo.isInAlliance()){
        m_bottomNode->setVisible(true);
        if (m_info->isApplied) {
            m_rovkeBtn->setVisible(true);
        }else if(m_info->recruit==1){
            m_joinAlliance->setVisible(true);
        }else{
            m_applyBtn->setVisible(true);
        }
    }
    if(!GlobalData::shared()->playerInfo.isInSelfServer()){
        m_joinAlliance->setVisible(false);
    }
    int playerServer = GlobalData::shared()->playerInfo.selfServerId;
    int allianceServer = m_info->serverId;
    if (playerServer != allianceServer) {
        m_rovkeBtn->setVisible(false);
        m_joinAlliance->setVisible(false);
        m_applyBtn->setVisible(false);
        m_bottomNode->setVisible(false);
        m_btnMember->setEnabled(false);
    }
    string lang = ( strcmp(m_info->language.c_str(), "")==0?"115600":m_info->language );
    m_language->setString(_lang(lang).c_str());
    return true;
}

void SearchAllianceInfoView::initFun(){
    std::string str = "(";
    str.append(m_info->shortName.c_str());
    str.append(") ");
    str.append(m_info->name.c_str());
    m_nameTxt->setString(str.c_str());
    str = CC_ITOA(m_info->currentNum);
    str.append("/");
    str.append(CC_ITOA(m_info->getSelfAllianceNum()));
    m_membersTxt->setString(str);
    str = _lang_1("115015", m_info->leader.c_str());
    m_leaderTxt->setString(str.c_str());
    m_powerTxt->setString(CC_CMDITOAL(m_info->totalForce).c_str());
    std::string annStr = m_info->intro;
    m_announceTxt->setString(annStr.c_str());
    
    CCCommonUtils::setButtonTitle(m_btnMail, _lang("115026").c_str());
    CCCommonUtils::setButtonTitle(m_btnMember, _lang("115027").c_str());
    CCCommonUtils::setButtonTitle(m_btnComment, _lang("115929").c_str());
}

void SearchAllianceInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, true);
    });
    this->setTitleName(_lang("115025"));
    initFun();
    GlobalData::shared()->playerInfo.isChatView = true;
}

void SearchAllianceInfoView::onExit()
{
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    setTouchEnabled(false);
    CCNode::onExit();
}

bool SearchAllianceInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void SearchAllianceInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}


void SearchAllianceInfoView::revokeAlliacne(CCObject * pSender, Control::EventType pCCControlEvent){
    CancelAllianceApplyCommand* cmd = new CancelAllianceApplyCommand(m_info->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(SearchAllianceInfoView::cancelApply), NULL));
    cmd->sendAndRelease();
    m_rovkeBtn->setEnabled(false);
}

void SearchAllianceInfoView::applyAlliacne(CCObject * pSender, Control::EventType pCCControlEvent){
    ApplyAllianceCommand* cmd = new ApplyAllianceCommand(m_info->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(SearchAllianceInfoView::success), NULL));
    cmd->sendAndRelease();
    m_applyBtn->setEnabled(false);
}

void SearchAllianceInfoView::joinAlliacne(CCObject * pSender, Control::EventType pCCControlEvent){
    ApplyAllianceCommand* cmd = new ApplyAllianceCommand(m_info->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(SearchAllianceInfoView::joinSuccess), NULL));
    cmd->sendAndRelease();
    m_joinAlliance->setEnabled(false);
}

void SearchAllianceInfoView::success(CCObject* obj){
    m_applyBtn->setVisible(false);
    m_rovkeBtn->setVisible(true);
    m_rovkeBtn->setEnabled(true);
    m_info->isApplied = true;
    AllianceManager::getInstance()->needRefresh = true;
}

void SearchAllianceInfoView::joinSuccess(CCObject* obj){
    m_joinAlliance->setEnabled(true);
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
    }
    //发聊天消息，加入联盟
    ChatController::getInstance()->sendCountryChat(_lang("115188").append("  (").append(_lang("115181")).append(")").c_str(), CHAT_ALLIANCE, 2);
}

void SearchAllianceInfoView::cancelApply(CCObject* obj){
    m_info->isApplied = false;
    m_rovkeBtn->setVisible(false);
    if (m_info->isApplied) {
        m_rovkeBtn->setVisible(true);
        m_rovkeBtn->setEnabled(true);
    }else if(m_info->recruit==1){
        m_joinAlliance->setVisible(true);
        m_joinAlliance->setEnabled(true);
    }else{
        m_applyBtn->setVisible(true);
        m_applyBtn->setEnabled(true);
    }
    AllianceManager::getInstance()->needRefresh = true;
}

void SearchAllianceInfoView::onMailClick(CCObject * pSender, Control::EventType pCCControlEvent){
    // PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create(m_info->leader, ""));
    MailController::getInstance()->openMailDialogViewFirst(m_info->leader, m_info->leaderUid);
}

void SearchAllianceInfoView::onMemberClick(CCObject * pSender, Control::EventType pCCControlEvent){
    bool isApply = GlobalData::shared()->playerInfo.allianceInfo.uid !=m_info->uid;
    if (GlobalData::shared()->playerInfo.selfServerId == m_info->serverId) {
        auto popup = AllianceInfoMembersView::create(m_info->uid,isApply);
        PopupViewController::getInstance()->addPopupInView(popup);
    } else {
//        PopupViewController::getInstance()->addPopupInView(AllianceInfoMembersView::create(m_info->uid,false));
        return;
    }
}

void SearchAllianceInfoView::onCommentClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid){
        PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COMMENT,m_info->uid),false);
    }else{
        AllianceCommentCheck* cmd = new AllianceCommentCheck(m_info->uid);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(SearchAllianceInfoView::checkComment), NULL));
        cmd->sendAndRelease();
        if(m_waitInterface){
            m_waitInterface->remove();
            m_waitInterface = NULL;
        }
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_btnComment);
    }
}

void SearchAllianceInfoView::checkComment(CCObject* obj){
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    NetResult* result = dynamic_cast<NetResult*>(obj);
    auto dic = _dict(result->getData());
    if(dic){
        bool enable = dic->valueForKey("enable")->boolValue();
        if(enable){
            PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COMMENT,m_info->uid),false);
        }else{
            CCCommonUtils::flyText(_lang("E100080"));
        }
    }else{
        PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COMMENT,m_info->uid),false);
    }
}

SEL_CCControlHandler SearchAllianceInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "joinAlliacne", SearchAllianceInfoView::joinAlliacne);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "applyAlliacne", SearchAllianceInfoView::applyAlliacne);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "revokeAlliacne", SearchAllianceInfoView::revokeAlliacne);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMailClick", SearchAllianceInfoView::onMailClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMemberClick", SearchAllianceInfoView::onMemberClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCommentClick", SearchAllianceInfoView::onCommentClick);
    return NULL;
}

bool SearchAllianceInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leaderTxt", CCLabelIFTTF*, this->m_leaderTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_membersTxt", CCLabelIF*, this->m_membersTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_announceTxt", CCLabelIFTTF*, this->m_announceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_language", CCLabelIF*, this->m_language);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textNode", CCNode*, this->m_textNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_announceNode", CCNode*, this->m_announceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIcon", CCNode*, this->m_allianceIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funList", CCNode*, this->m_funList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rovkeBtn", CCControlButton*, this->m_rovkeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyBtn", CCControlButton*, this->m_applyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_joinAlliance", CCControlButton*, this->m_joinAlliance);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMail", CCControlButton*, this->m_btnMail);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMember", CCControlButton*, this->m_btnMember);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnComment", CCControlButton*, this->m_btnComment);
    return false;
}

void SearchAllianceInfoView::addBtnPar(CCControlButton* btn){
    string tmpStart = "ShowFire_";
    string tmpStart1 = "ShowFireUp_";
    CCScale9Sprite* s9 = CCLoadSprite::createScale9Sprite("sel_general.png");
    CCSize size = btn->getPreferredSize();
    s9->setPreferredSize(size+CCSize(30,30));
    s9->setPosition(size/2);
    btn->addChild(s9);
    for (int i=1; i<=5; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
        particle->setPosition(ccp(size.width/2, -3));
        particle->setPosVar(ccp(size.width/2, 0));
        btn->addChild(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,size.width*0.3);
        particle1->setPosition(ccp(size.width/2, size.height-3));
        particle1->setPosVar(ccp(size.width/2, 0));
        btn->addChild(particle1);
        
        auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
        particle2->setPosition(ccp(0, size.height/2));
        particle2->setPosVar(ccp(0, size.height/2));
        btn->addChild(particle2);
        
        auto particle3 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart1.c_str(),i)->getCString(), CCPointZero,size.height*0.3);
        particle3->setPosition(ccp(size.width, size.height/2));
        particle3->setPosVar(ccp(0, size.height/2));
        btn->addChild(particle3);
    }
}