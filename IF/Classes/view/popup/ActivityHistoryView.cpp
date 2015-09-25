//
//  ActivityHistoryView.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#include "ActivityHistoryView.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "ActivityScoreHistoryCommand.h"
#include "ActivityController.h"
#include "GetUserRankInfoCommand.h"
#include "GeAlliancetRankInfoCommand.h"
#include "CKFGetTopInfoCommand.h"

ActivityHistoryView *ActivityHistoryView::create(int type){
    ActivityHistoryView *ret = new ActivityHistoryView(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ActivityHistoryView::onEnter(){
    switch (m_type) {
        case 0:
            m_titleTxt->setString(_lang("105825"));
            break;
        case 1:
            m_titleTxt->setString(_lang("105836"));
            break;
        case 2:
            m_titleTxt->setString(_lang("133085"));
            break;
        case 3:
            m_titleTxt->setString(_lang("133086"));
            break;
        case 4:
            m_titleTxt->setString(_lang("138017"));
            break;
        case 5:
            m_titleTxt->setString(_lang("138015"));
            break;
        default:
            break;
    }
    PopupBaseView::onEnter();
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}

void ActivityHistoryView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool ActivityHistoryView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_viewBg, pTouch)){
        m_needclose = true;
    }
    return true;
}

void ActivityHistoryView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(m_needclose){
        m_needclose = false;
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool ActivityHistoryView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto cf = CCLoadSprite::getSF("Alliance_Ranking2.png");
        if (cf==NULL) {
            CCLoadSprite::doResourceByCommonIndex(7, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(7, false);
            });
        }
        auto node = CCBLoadFile("ActivityHistoryView", this, this);
        this->setContentSize(node->getContentSize());
        //m_tip->setString(_lang("105834"));
        m_data = CCArray::create();
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(-50);
        m_infoList->addChild(m_tabView);
        
        switch (m_type) {
            case 0:
            {
                ActivityScoreHistoryCommand* cmd = new ActivityScoreHistoryCommand();
                cmd->setCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::getServerData), NULL));
                cmd->setFailCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::removeLoadingAni), NULL));
                cmd->sendAndRelease();
                ActivityController::getInstance()->activityRecord(3);
            }
                break;
            case 1:
            {
                ActivityLastHistoryCommand* cmd1 = new ActivityLastHistoryCommand();
                cmd1->setCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::getTopHistoryData), NULL));
                cmd1->setFailCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::removeLoadingAni), NULL));
                cmd1->sendAndRelease();
            }

                break;
            case 2:
            {
                GeAlliancetRankInfoCommand* cmd2 = new GeAlliancetRankInfoCommand();
                cmd2->setCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::getTopAllianceRankData), NULL));
                cmd2->setFailCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::removeLoadingAni), NULL));
                cmd2->sendAndRelease();
            }
                break;
            case 3:
            {
                GetUserRankInfoCommand* cmd3 = new GetUserRankInfoCommand();
                cmd3->setCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::getTopUserRankData), NULL));
                cmd3->setFailCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::removeLoadingAni), NULL));
                cmd3->sendAndRelease();
            }
                break;
            case 4:
            {
                CKFGetTopInfoCommand* cmd3 = new CKFGetTopInfoCommand("user");
                cmd3->setCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::getCKFData), NULL));
                cmd3->setFailCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::removeLoadingAni), NULL));
                cmd3->sendAndRelease();
            }
                break;
            case 5:
            {
                CKFGetTopInfoCommand* cmd3 = new CKFGetTopInfoCommand("kingdom");
                cmd3->setCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::getCKFData), NULL));
                cmd3->setFailCallback(CCCallFuncO::create(this,callfuncO_selector(ActivityHistoryView::removeLoadingAni), NULL));
                cmd3->sendAndRelease();
            }
                break;
            default:
                break;
        }
        this->addLoadingAni();
        ret = true;
    }
    return ret;
}

void ActivityHistoryView::getTopAllianceRankData(CCObject* data){
    this->removeLoadingAni(NULL);
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = (CCArray*)dic->objectForKey("list");
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));
                if(item){
                    double round = item->valueForKey("time")->doubleValue();
                    CCDictionary* rankDic = CCDictionary::create();
                    rankDic->setObject(CCString::create(item->valueForKey("time")->getCString()), "time");
                    m_data->addObject(rankDic);
                    CCArray* rankList = dynamic_cast<CCArray*>(item->objectForKey("roundList"));
                    if(rankList==NULL){
                        rankList = CCArray::create();
                    }
                    m_data->addObjectsFromArray(rankList);
                }
            }
        }
    }
    this->reFresh();
}

void ActivityHistoryView::getCKFData(CCObject* data){
    this->removeLoadingAni(NULL);
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = (CCArray*)dic->objectForKey("list");
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));
                if(item){
                    if (item->objectForKey("round")) {
                        int round = item->valueForKey("round")->intValue();
                        CCDictionary* rankDic = CCDictionary::create();
                        rankDic->setObject(CCString::create(item->valueForKey("round")->getCString()), "round");
                        m_data->addObject(rankDic);
                    }
                    CCArray* rankList = dynamic_cast<CCArray*>(item->objectForKey("roundList"));
                    if(rankList==NULL){
                        rankList = CCArray::create();
                    }
                    m_data->addObjectsFromArray(rankList);
                }
            }
        }
    }
    this->reFresh();
}

void ActivityHistoryView::getTopUserRankData(CCObject* data){
    this->removeLoadingAni(NULL);
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = (CCArray*)dic->objectForKey("list");
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));
                if(item){
                    double round = item->valueForKey("time")->doubleValue();
                    CCDictionary* rankDic = CCDictionary::create();
                    rankDic->setObject(CCString::create(item->valueForKey("time")->getCString()), "time");
                    m_data->addObject(rankDic);
                    CCArray* rankList = dynamic_cast<CCArray*>(item->objectForKey("roundList"));
                    if(rankList==NULL){
                        rankList = CCArray::create();
                    }
                    m_data->addObjectsFromArray(rankList);
                }
            }
        }
    }
    this->reFresh();
}

void ActivityHistoryView::getServerData(CCObject* data){
    this->removeLoadingAni(NULL);
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = (CCArray*)dic->objectForKey("list");
        if(array){
            if (array->count()>0) {
                auto item = dynamic_cast<CCDictionary*>(array->objectAtIndex(0));
                if(item){
                    int round = -99;
                    if(item->objectForKey("round")){
                        round = item->valueForKey("round")->intValue();
                    }
                    if(round==0){
                        if(m_type==0){
                            array->removeObjectAtIndex(0);
                        }
                        //array->addObject(item);
                    }
                }
            }
            array->reverseObjects();
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));
                if(item){
                    int round = item->valueForKey("round")->intValue();
                    if(m_type==1 && round!=0){
                        continue;
                    }
                    CCDictionary* rankDic = CCDictionary::create();
                    rankDic->setObject(CCString::create(CC_ITOA(round)), "round");
                    m_data->addObject(rankDic);
                    CCArray* rankList = dynamic_cast<CCArray*>(item->objectForKey("rankList"));
                    if(rankList==NULL){
                        rankList = CCArray::create();
                    }
                    if(m_type==0){
                        m_data->addObjectsFromArray(rankList);
                    }else{
                        int num = rankList->count();
                        for (int i=0; i<num; i++) {
                            auto itemDic = _dict(rankList->objectAtIndex(i));
                            itemDic->setObject(CCString::create("totalRank"), "totalRank");
                        }
                        m_data->addObjectsFromArray(rankList);
                    }
                }
            }
            CCDictionary* rankDic = CCDictionary::create();
            rankDic->setObject(CCString::create("history"), "history");
            m_data->addObject(rankDic);
        }
    }
    this->reFresh();
}


void ActivityHistoryView::getTopHistoryData(CCObject* data){
    this->removeLoadingAni(NULL);
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = (CCArray*)dic->objectForKey("list");
        if(array){
            //array->reverseObjects();
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));
                if(item){
                    CCDictionary* rankDic = CCDictionary::create();
                    rankDic->setObject(CCString::create(item->valueForKey("time")->getCString()), "time");
                    m_data->addObject(rankDic);
                    CCArray* rankList = dynamic_cast<CCArray*>(item->objectForKey("rankList"));
                    if(rankList==NULL){
                        rankList = CCArray::create();
                    }
                    int num = rankList->count();
                    for (int i=0; i<num; i++) {
                        auto itemDic = _dict(rankList->objectAtIndex(i));
                        itemDic->setObject(CCString::create("totalRank"), "totalRank");
                    }
                    m_data->addObjectsFromArray(rankList);
                }
            }
        }
    }
    this->reFresh();
}

void ActivityHistoryView::reFresh(){
    if(m_data && m_data->count()>0){
        m_tip->setString("");
    }else if(m_type==0 || m_type==1){
        m_tip->setString(_lang("105834"));
    }else if(m_type==2 || m_type==3){
        m_tip->setString(_lang("133089"));
    }
    m_tabView->reloadData();
}

void ActivityHistoryView::removeLoadingAni(CCObject* obj){
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

void ActivityHistoryView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_infoList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon,1000000);
    }
}

bool ActivityHistoryView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip", CCLabelIF*, this->m_tip);
    return false;
}

SEL_CCControlHandler ActivityHistoryView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

CCSize ActivityHistoryView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    auto dic = _dict(m_data->objectAtIndex(idx));
    if(dic && (dic->objectForKey("round") || dic->objectForKey("time"))){
        return CCSize(510, 88);
    }
    if(dic && dic->objectForKey("history")){
        return CCSize(510, 100);
    }
    return CCSize(510, 60);
}

CCSize ActivityHistoryView::cellSizeForTable(CCTableView *table)
{
    return CCSize(510, 60);
}

CCTableViewCell* ActivityHistoryView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    ActivityHistoryCell* cell = (ActivityHistoryCell*)table->dequeueCell();
    CCDictionary* info = (CCDictionary*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = ActivityHistoryCell::create(info,m_type);
        }
    }
    return cell;
}

ssize_t ActivityHistoryView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void ActivityHistoryView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

//

ActivityHistoryCell *ActivityHistoryCell::create(CCDictionary* info,int type){
    ActivityHistoryCell *ret = new ActivityHistoryCell(info,type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ActivityHistoryCell::onEnter(){
    CCNode::onEnter();
}

void ActivityHistoryCell::onExit(){
    CCNode::onExit();
}

bool ActivityHistoryCell::init(){
    auto node = CCBLoadFile("ActivityHistoryRankCell", this, this);
    this->setContentSize(node->getContentSize());
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_numLabel->setScale(0.5);
        m_numLabel->setFontSize(m_numLabel->getFontSize() * 2);
        m_nameTxt->setScale(0.5);
        m_nameTxt->setFontSize(m_nameTxt->getFontSize() * 2);
        m_titleTxt->setScale(0.5);
        m_titleTxt->setFontSize(m_titleTxt->getFontSize() * 2);
        m_allianceTxt->setScale(0.5);
        m_allianceTxt->setFontSize(m_allianceTxt->getFontSize() * 2);
        m_timeTxt->setScale(0.5);
        m_timeTxt->setFontSize(m_timeTxt->getFontSize() * 2);
    }
    CCCommonUtils::setButtonTitle(m_btnHistory, _lang("105836").c_str());
    setData(m_info);
    return true;
}

void ActivityHistoryCell::setData(CCDictionary* info){
    m_info = info;
    int rank = m_info->valueForKey("rank")->intValue();
    m_line->setVisible(false);
    m_renderBg2->setVisible(false);
    m_renderBg1->setVisible(false);
    m_titleNode->setVisible(false);
    m_rNode->setVisible(false);
    m_numLabel->setString("");
    m_nameTxt->setString("");
    m_titleTxt->setString("");
    m_timeTxt->setString("");
    m_btnHistory->setVisible(false);
    m_titleTxt->setPositionX(0);
    if(m_info->objectForKey("history")){
        m_btnHistory->setVisible(true);
    }else if (m_info->objectForKey("time")) {
        m_titleNode->setVisible(true);
        m_titleTxt->setPositionX(-15);
        time_t time = info->valueForKey("time")->doubleValue()/1000;//GlobalData::shared()->changeTime(temp);
        m_timeTxt->setString(CCCommonUtils::timeStampToYMD(time));
        if (m_type==2) {
            m_titleTxt->setString(_lang("133009"));
        }else if(m_type==3){
            m_titleTxt->setString(_lang("133063"));
        }else if(m_type==4){
            m_timeTxt->setString("");
        }else if(m_type==5){
            m_timeTxt->setString("");
        }else{
            m_titleTxt->setString(_lang("105827"));
        }

    }else if(m_info->objectForKey("round")){
        m_titleNode->setVisible(true);
        std::string round = m_info->valueForKey("round")->getCString();
        std::string numStr = _lang_1("105826",round.c_str());
        m_titleTxt->setString(numStr);
        m_numLabel->setString("");
        m_nameTxt->setString("");
    }else{
        m_rNode->setVisible(true);
        m_numLabel->setString("");
        m_rankNode->removeAllChildrenWithCleanup(true);
        m_allianceNode->removeAllChildrenWithCleanup(true);
        m_renderBg2->setVisible(false);
        m_renderBg1->setVisible(false);
        m_line->setVisible(false);
        m_nameTxt->setColor({201,188,149});
        m_allianceTxt->setColor({173,164,154});
        if (m_info->objectForKey("totalRank") || m_type==2 || m_type==3 | m_type==4 || m_type==5) {
            if (rank<=3) {
                m_renderBg1->setVisible(true);
                m_renderBg2->setVisible(true);
                CCSprite* spr = CCLoadSprite::createSprite(CCString::createWithFormat("Alliance_Ranking%d.png",rank+1)->getCString());
                m_rankNode->addChild(spr);
                m_nameTxt->setColor({255,201,0});
                m_allianceTxt->setColor({216,168,107});
            }else{
                m_renderBg1->setVisible(true);
                m_numLabel->setString(CC_ITOA(rank));
            }
        }else{
            m_numLabel->setString(CC_ITOA(rank));
            m_line->setVisible(true);
        }
        std::string allianceAbbr = m_info->valueForKey("allianceAbbr")->getCString();
        if(m_info->objectForKey("abbr")){
            allianceAbbr = m_info->valueForKey("abbr")->getCString();
        }
        std::string nameStr = "";
        if(allianceAbbr!=""){
            nameStr.append("(");
            nameStr.append(allianceAbbr);
            nameStr.append(")");
        }
        nameStr.append(m_info->valueForKey("name")->getCString());
        if (m_info->objectForKey("serverId") && m_type!=5) {
            nameStr.append("#");
            nameStr.append(m_info->valueForKey("serverId")->getCString());
        }
        m_nameTxt->setString(nameStr);
    }
}

bool ActivityHistoryCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numLabel", CCLabelIF*, this->m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceTxt", CCLabelIF*, this->m_allianceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_line", CCNode*, this->m_line);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_renderBg1", CCNode*, this->m_renderBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_renderBg2", CCNode*, this->m_renderBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankNode", CCNode*, this->m_rankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceNode", CCNode*, this->m_allianceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleNode", CCNode*, this->m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rNode", CCNode*, this->m_rNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnHistory", CCControlButton*, this->m_btnHistory);
    return false;
}

void ActivityHistoryCell::onClickHistory(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupView(ActivityHistoryView::create(true));
}

SEL_CCControlHandler ActivityHistoryCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHistory", ActivityHistoryCell::onClickHistory);
    return NULL;
}