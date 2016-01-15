//
//  AlliianceTerritoryView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/4/9.
//
//

#include "AllianceTerritoryView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "WorldMapView.h"
#include "AllianceTerritoryShowCommand.h"
#include "MoveCityPopUpView.h"
#include "SceneController.h"
#include "WorldController.h"
#include "AllianceAreaPopupView.h"
#include "HelpshiftCocos2dx.h"
#include "UIComponent.h"
#include "TerritoryUnlockTipsView.h"
#include "TerritoryBannerChoseView.h"
#define TERRITORY_INFO_CELL_CLICK "territory_info_cell_click"
vector<int> territoryOpenVec;
const float numPerRow = 1.0;
AllianceTerritoryView* AllianceTerritoryView::create(unsigned int target, bool isInTheWorld){
    AllianceTerritoryView* ret = new AllianceTerritoryView(target,isInTheWorld);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool AllianceTerritoryView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);

    CCLoadSprite::doResourceByCommonIndex(504, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
    });
    auto tmpCCB = CCBLoadFile("AllianceTerritoryView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    m_territoryTotalNum = 0;
    m_resourceTotalNum = 4;//////
    m_towerTotalNum = 0;
    m_warehouseTotalNum = 1;
    m_bannerTotalNum = 1;
    m_dataList.clear();
    territoryOpenVec.clear();
    //territoryOpenVec.resize(3);
    //m_dataList.resize(3);
    m_territoryData = CCArray::create();
    m_resourceData = CCArray::create();
    m_towerData = CCArray::create();
    m_warehouseData = CCArray::create();
    m_bannerData = CCArray::create();
    m_showData = CCArray::create();
    
    int extH = getExtendHeight();
    m_infoList->setPositionY(m_infoList->getPositionY() - extH);
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + extH));
    m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
//    auto tbg = CCLoadSprite::loadResource("technology_09.png");
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//    auto picBg1 = CCLoadSprite::createSprite("technology_09.png");
//    picBg1->setAnchorPoint(ccp(0, 0));
//    picBg1->setPosition(ccp(0, 0));
//    tBatchNode->addChild(picBg1);
//    int maxHeight = CCDirector::sharedDirector()->getWinSize().height - 79;
//    int curHeight = picBg1->getContentSize().height;
//    while(curHeight < maxHeight)
//    {
//        auto picBg2 = CCLoadSprite::createSprite("technology_09.png");
//        picBg2->setAnchorPoint(ccp(0, 0));
//        picBg2->setPosition(ccp(0, curHeight));
//        tBatchNode->addChild(picBg2);
//        curHeight += picBg2->getContentSize().height;
//    }
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        tBatchNode->setScaleX(1536.0 / 640.0);
//    }
//    m_bgNode->addChild(tBatchNode);
//    auto tempSprite = CCLoadSprite::createScale9Sprite("btn_equip.png");
//    m_bgNode->addChild(tempSprite);
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    
    m_titleTxt->setString(_lang("115301"));
    return true;
}

//更新领地面板内的信息
void AllianceTerritoryView::updateInfo(CCObject* data)///////默认为显示哨塔
{
    int territoryPlaceable = 0;
    m_territoryActive = false;
    m_territoryData->removeAllObjects();
    m_resourceData->removeAllObjects();
    m_towerData->removeAllObjects();
    m_warehouseData->removeAllObjects();
    m_showData->removeAllObjects();
    this->removeLoadingAni();
    m_dataList.resize(4);//todowangchaoyi
    territoryOpenVec.resize(4);//todowangchaoyi data为0 的时候判断 -1/2 +1 逻辑漏洞
//    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(data);
    if(dic){
        m_territoryTotalNum = dic->valueForKey("num")->intValue();
        CCArray* array = (CCArray*)dic->objectForKey("territory");
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = _dict(array->objectAtIndex(i));
                AllianceTerritoryInfo* info = new AllianceTerritoryInfo();
                info->parseInfo(item);
                if (info->m_territoryType == Tile_allianceArea) {
                    m_territoryData->addObject(info);
                    territoryPlaceable++;
                    //判断是领地是否生效，即是否有哨塔处于后五种状态，如有则显示可以放置超级矿
                    if (info->m_territoryState != 0 && info->m_territoryState != 1) {
                        m_territoryActive = true;
                    }
                }
                else if (info->m_territoryType == tile_superMine) {
                    m_resourceData->addObject(info);
                }
                else if (info->m_territoryType == tile_tower) {
                    if (m_territoryActive == false && info->m_territoryPtIndex == 0) {
                        info->m_territoryActive = 0;
                        info->m_lockFlag = false;
                    }
                    m_towerData->addObject(info);
                }
                else if (info->m_territoryType == tile_wareHouse) {
                    m_warehouseData->addObject(info);
                }
                else if (info->m_territoryType == tile_banner) {
                    m_bannerData->addObject(info);
                }
                CC_SAFE_RELEASE(info);
            }
        }
    }
    
    ///////默认显示哨塔
    m_dataList[0] = 0;
    if (GlobalData::shared()->alliance_territory_index == 0) {
        territoryOpenVec[0] = 1;
    }
    else
        territoryOpenVec[0] = 0;
    
    
    
    
    int m_territoryDataCount = m_territoryData->count();
    for (int i = 0; i < m_territoryTotalNum - m_territoryDataCount; i++) {
        AllianceTerritoryInfo* info = new AllianceTerritoryInfo();
        info->m_territoryType = Tile_allianceArea;
        info->m_territoryPtIndex = -1;
        info->m_territoryActive = 0;
        info->m_territoryName = "";
        info->m_lockFlag = true;
        info->m_territoryCount = i + m_territoryDataCount + 1;
        info->m_territoryName = _lang_1("115312", CC_ITOA(info->m_territoryCount));
        m_territoryData->addObject(info);
        CC_SAFE_RELEASE(info);
    }
    
    /////超级矿
    m_dataList[1] = 1;
    if (GlobalData::shared()->alliance_territory_index == 1) {
        territoryOpenVec[1] = 1;
    }
    else
        territoryOpenVec[1] = 0;
    
    
    if (m_resourceData->count() == 0) {//超级矿未放置状态，后台未返回任何数据，前台自行生成四个显示数据
        for (int i = 0; i < 4; i++) {
            AllianceTerritoryInfo* info = new AllianceTerritoryInfo();//粮食→木材→铁矿→秘银
            info->m_territoryType = tile_superMine;
            info->m_allianceAbbr = GlobalData::shared()->playerInfo.allianceInfo.shortName;
            info->m_territoryPtIndex = 0;
            if (m_territoryActive == true) {
                info->m_territoryActive = 1;
            }
            else {
                info->m_territoryActive = 0;
                info->m_lockFlag = false;
            }
            switch (i) {
                case 0:{//粮食
                    info->m_territoryResourceType = Food;
                    info->m_territoryName = _lang("115376");
                }
                    break;
                case 1:{//木材
                    info->m_territoryResourceType = Wood;
                    info->m_territoryName = _lang("115377");
                }
                    break;
                case 2:{//铁矿
                    info->m_territoryResourceType = Iron;
                    info->m_territoryName = _lang("115378");
                }
                    break;
                case 3:{//秘银
                    info->m_territoryResourceType = Stone;
                    info->m_territoryName = _lang("115379");
                }
                    break;
                default:
                    break;
            }
            m_resourceData->addObject(info);
            CC_SAFE_RELEASE(info);
        }
    }
    else if (m_resourceData->count() == 1) {//超级矿已放置状态，返回已放置的超级矿数据，前台自行生成其他三个显示数据
        AllianceTerritoryInfo* info = (AllianceTerritoryInfo*)m_resourceData->objectAtIndex(0);
        WorldResourceType resType = info->m_territoryResourceType;
        
        AllianceTerritoryInfo* info1 = new AllianceTerritoryInfo();//粮食
        info1->m_territoryType = tile_superMine;
        info1->m_allianceAbbr = GlobalData::shared()->playerInfo.allianceInfo.shortName;
        info1->m_territoryResourceType = Food;
        info1->m_territoryName = _lang("115376");
        info1->m_territoryActive = 0;
        info1->m_lockFlag = true;
        
        AllianceTerritoryInfo* info2 = new AllianceTerritoryInfo();//木材
        info2->m_territoryType = tile_superMine;
        info2->m_allianceAbbr = GlobalData::shared()->playerInfo.allianceInfo.shortName;
        info2->m_territoryResourceType = Wood;
        info2->m_territoryName = _lang("115377");
        info2->m_territoryActive = 0;
        info2->m_lockFlag = true;
        
        AllianceTerritoryInfo* info3 = new AllianceTerritoryInfo();//铁矿
        info3->m_territoryType = tile_superMine;
        info3->m_allianceAbbr = GlobalData::shared()->playerInfo.allianceInfo.shortName;
        info3->m_territoryResourceType = Iron;
        info3->m_territoryName = _lang("115378");
        info3->m_territoryActive = 0;
        info3->m_lockFlag = true;
        
        AllianceTerritoryInfo* info4 = new AllianceTerritoryInfo();//秘银
        info4->m_territoryType = tile_superMine;
        info4->m_allianceAbbr = GlobalData::shared()->playerInfo.allianceInfo.shortName;
        info4->m_territoryResourceType = Stone;
        info4->m_territoryName = _lang("115379");
        info4->m_territoryActive = 0;
        info4->m_lockFlag = true;
        
        switch (resType) {
            case Food:{
                m_resourceData->addObject(info2);
                m_resourceData->addObject(info3);
                m_resourceData->addObject(info4);
            }
                break;
            case Wood:{
                m_resourceData->insertObject(info1, 0);
                m_resourceData->addObject(info3);
                m_resourceData->addObject(info4);
            }
                break;
            case Iron:{
                m_resourceData->insertObject(info1, 0);
                m_resourceData->insertObject(info2, 1);
                m_resourceData->addObject(info4);
            }
                break;
            case Stone:{
                m_resourceData->insertObject(info1, 0);
                m_resourceData->insertObject(info2, 1);
                m_resourceData->insertObject(info3, 2);
            }
                break;
            default:
                break;
        }
        CC_SAFE_RELEASE(info1);
        CC_SAFE_RELEASE(info2);
        CC_SAFE_RELEASE(info3);
        CC_SAFE_RELEASE(info4);
    }
    
    /////箭塔
    if (GlobalData::shared()->alliance_territory_tower_switch == 1) {
        m_dataList[2] = 2;
        if (GlobalData::shared()->alliance_territory_index == 2) {
            territoryOpenVec[2] = 1;
        }
        else
            territoryOpenVec[2] = 0;;

        int size = territoryPlaceable;
        int territoryId;
        if (size <= 0) {
            territoryId = 6001;
        }
        else {
            territoryId = size + 1 + 6000;
        }
        
        m_towerTotalNum = m_towerData->count();
        bool otherFlag = true;//////目前联盟哨塔最多为5个，如果哨塔达到5个后就没有下一个哨塔开启时可获得的箭塔，不需要显示
        if (size == m_territoryTotalNum) {
            otherFlag = false;
            m_towerTotalNum = m_towerData->count();
        }
        if (otherFlag) {
            CCDictElement *element;
            auto dictEffect = LocalController::shared()->DBXMLManager()->getGroupByKey("territory");
            CCDICT_FOREACH(dictEffect, element){
                std::string territoryIdStr = element->getStrKey();
                int countTemp = atoi(territoryIdStr.c_str());
                if (countTemp == territoryId) {
                    std::string towerAddNum = CCCommonUtils::getPropById(territoryIdStr, "towermunber_add");/////获取下一个领地对应开启的箭塔数量
                    m_towerTotalNum = m_towerData->count() + atoi(towerAddNum.c_str());
                    break;
                }
            }
        }
        
        int m_towerDataCount = m_towerData->count();
        for (int i = 0; i < m_towerTotalNum - m_towerDataCount; i++) {
            AllianceTerritoryInfo* info = new AllianceTerritoryInfo();
            info->m_territoryType = tile_tower;
            info->m_territoryPtIndex = -1;
            info->m_territoryActive = 0;
            info->m_lockFlag = true;
            info->m_territoryCount = i + m_towerDataCount + 1;
            info->m_territoryName = _lang_1("115406", CC_ITOA(info->m_territoryCount));
            m_towerData->addObject(info);
            CC_SAFE_RELEASE(info);
        }
    }
    
    m_dataList[3] = 3;
    if (GlobalData::shared()->alliance_territory_index == 3) {
        territoryOpenVec[3] = 1;
    }
    else
        territoryOpenVec[3] = 0;
    
    if (m_warehouseData->count() == 0) {//仓库未放置状态，后台未返回任何数据，前台自行生成显示数据
        AllianceTerritoryInfo* info = new AllianceTerritoryInfo();
        info->m_territoryType = tile_wareHouse;
        info->m_allianceAbbr = GlobalData::shared()->playerInfo.allianceInfo.shortName;
        info->m_territoryPtIndex = 0;
        if (m_territoryActive == true) {
            info->m_territoryActive = 1;
        }
        else {
            info->m_territoryActive = 0;
            info->m_lockFlag = false;
        }
        info->m_territoryName = _lang("115364");
        m_warehouseData->addObject(info);
        CC_SAFE_RELEASE(info);
    }

    if (GlobalData::shared()->alliance_territory_banner_switch == 1) {
        m_dataList[4] = 4;
        if (GlobalData::shared()->alliance_territory_index == 4) {
            territoryOpenVec[4] = 1;
        }
        else
            territoryOpenVec[4] = 0;
        
        if (m_bannerData->count() == 0) {//国旗未放置状态，后台未返回任何数据，前台自行生成显示数据
            AllianceTerritoryInfo* info = new AllianceTerritoryInfo();
            info->m_territoryType = tile_banner;
            info->m_allianceAbbr = GlobalData::shared()->playerInfo.allianceInfo.shortName;
            info->m_territoryPtIndex = 0;
            info->m_banner = "";
            if (m_territoryActive == true) {
                info->m_territoryActive = 1;
            }
            else {
                info->m_territoryActive = 0;
                info->m_lockFlag = false;
            }
            info->m_territoryName = _lang("115534");
            m_bannerData->addObject(info);
            CC_SAFE_RELEASE(info);
        }
    }
    
    switch (GlobalData::shared()->alliance_territory_index) {
        case 0:
            m_showData->addObjectsFromArray(m_territoryData);
            break;
        case 1:
            m_showData->addObjectsFromArray(m_resourceData);
            break;
        case 2:
            m_showData->addObjectsFromArray(m_towerData);
            break;
        case 3:
            m_showData->addObjectsFromArray(m_warehouseData);
            break;
        case 4:
            m_showData->addObjectsFromArray(m_bannerData);
            break;
        default:
            break;
    }
    
    m_tabView->reloadData();
}

//放置或收回后对领地面板进行刷新
void AllianceTerritoryView::refresh(CCObject* param)
{
    int index = -1;
    if(param==NULL || (dynamic_cast<CCInteger*>(param)==NULL)){
        index = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(param);
        index = intObj->getValue();
    }
    
    if (index == 0) {
        m_showData->removeAllObjects();
        m_showData->addObjectsFromArray(m_territoryData);
    }
    else if (index == 1) {
        CCCommonUtils::flyHint("", "", _lang("E100008"));
        m_showData->removeAllObjects();
//        m_showData->addObjectsFromArray(m_resourceData);//fusheng 不开放超级矿
    }
    else if (index == 2) {
        m_showData->removeAllObjects();
        m_showData->addObjectsFromArray(m_towerData);
    }
    else if (index == 3) {
        m_showData->removeAllObjects();
        m_showData->addObjectsFromArray(m_warehouseData);
    }
    else if (index == 4) {
        m_showData->removeAllObjects();
        m_showData->addObjectsFromArray(m_bannerData);
    }
    else if (index == -1) {
        m_showData->removeAllObjects();
    }
    
    m_tabView->reloadData();
}

void AllianceTerritoryView::removeLoadingAni(CCObject* p)
{
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}
        
void AllianceTerritoryView::addLoadingAni(){
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
    if (CCCommonUtils::isIosAndroidPad()) {
        m_loadingIcon->setScale(1.6);
    }
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon,1000000);
    }
}

CCSize AllianceTerritoryView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(m_dataList.size()<=0){
        return CCSizeZero;
    }
    
    if (idx < m_dataList.size()) {
        if (idx == 0) {
            if(territoryOpenVec[idx]==1){
                int size = m_territoryTotalNum;
                int num = (size-1)/2+1;
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 800 * num + 320);
                } else {
                    return CCSize(640, 160+num*420);
                }
            }
            else
            {
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 180);
                } else {
                    return CCSize(640, 90);
                }
            }
        }
        
        if (idx == 1) {
            territoryOpenVec[1] = 0;//fusheng 不开放超级矿
            if(territoryOpenVec[idx]==1){
                int size = m_resourceTotalNum;
                int num = (size-1)/2+1;
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 800 * num + 320);
                } else {
                    return CCSize(640, 160+num*420);
                }
            }
            else
            {
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 180);
                } else {
                    return CCSize(640, 90);
                }
            }
        }
        
        if (idx == 2) {
            if (territoryOpenVec[idx]==1) {
                int size = m_towerTotalNum;
                int num = (size-1)/2+1;
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 800 * num + 320);
                } else {
                    return CCSize(640, 160+num*420);
                }
            }
            else
            {
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 180);
                } else {
                    return CCSize(640, 90);
                }
            }
        }
        if (idx == 3) {
            if (territoryOpenVec[idx]==1) {
                int size = m_warehouseTotalNum;
                int num = (size-1)/2+1;
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 800 * num + 320);
                } else {
                    return CCSize(640, 160+num*420);
                }
            }
            else
            {
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 180);
                } else {
                    return CCSize(640, 90);
                }
            }
        }
        if (idx == 4) {
            if (territoryOpenVec[idx]==1) {
                int size = m_bannerTotalNum;
                int num = (size-1)/2+1;
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 800 * num + 320);
                } else {
                    return CCSize(640, 160+num*420);
                }
            }
            else
            {
                if (CCCommonUtils::isIosAndroidPad()) {
                    return CCSize(1536, 180);
                } else {
                    return CCSize(640, 90);
                }
            }
        }
    }
    else
        return CCSizeZero;
}

CCSize AllianceTerritoryView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 180);
    } else {
        return CCSize(640, 90);
    }
}
CCTableViewCell* AllianceTerritoryView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(m_dataList.size()<=0){
        return NULL;
    }
    
    AllianceTerritoryInfoCell* cell = (AllianceTerritoryInfoCell*)table->dequeueCell();
    if (idx < m_dataList.size()) {
        if (cell) {
            territoryOpenVec[1] = 0;//fusheng 不开放超级矿
            cell->setData(idx, territoryOpenVec[idx]);
        }
        else {
            if (idx == 0) {
                cell = AllianceTerritoryInfoCell::create(idx, territoryOpenVec[idx], m_isInTheWorld);
            }
            else if (idx == 1) {
                territoryOpenVec[1] = 0;//fusheng 不开放超级矿
                cell = AllianceTerritoryInfoCell::create(idx, territoryOpenVec[idx], m_territoryActive);
            }
            else if (idx == 2) {
                cell = AllianceTerritoryInfoCell::create(idx, territoryOpenVec[idx], m_territoryActive);
            }
            else if (idx == 3) {
                cell = AllianceTerritoryInfoCell::create(idx, territoryOpenVec[idx], m_territoryActive);
            }
            else if (idx == 4) {
                cell = AllianceTerritoryInfoCell::create(idx, territoryOpenVec[idx], m_territoryActive);
            }
        }
        
        if (territoryOpenVec[idx] == 1) {
            int m_totalNum = 0;
            m_totalNum = m_showData->count();
            
            int size = (m_totalNum-1)/2+1;
            for (int i = 0; i < size; i++) {
                AllianceTerritoryInfo* info1 = (AllianceTerritoryInfo*)m_showData->objectAtIndex(i*2);
                if (i*2+1>=m_showData->count()) {
                    cell->bind(m_totalNum,i,m_targetIndex,info1,NULL,info1->m_territoryType);
                }
                else {
                    AllianceTerritoryInfo* info2 = (AllianceTerritoryInfo*)m_showData->objectAtIndex(i*2+1);
                    cell->bind(m_totalNum,i,m_targetIndex,info1,info2,info1->m_territoryType);
                }
            }
        }
    }
    
    return cell;
}
ssize_t AllianceTerritoryView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_dataList.size() / numPerRow);
    return num;
}
void AllianceTerritoryView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void AllianceTerritoryView::onEnter()
{
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    CCNode::onEnter();
    if(m_showData->count()<=0 && GlobalData::shared()->playerInfo.isInAlliance()){
        AllianceTerritoryShowCommand* cmd = new AllianceTerritoryShowCommand();
        //cmd->setCallback(CCCallFuncO::create(this,callfuncO_selector(AllianceTerritoryView::updateInfo), NULL));
        //cmd->setFailCallback(CCCallFuncO::create(this,callfuncO_selector(AllianceTerritoryView::removeLoadingAni), NULL));
        cmd->sendAndRelease();
        addLoadingAni();
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceTerritoryView::updateInfo), ALLIANCE_TERRITORY_DATA_BACK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceTerritoryView::removeLoadingAni), ALLIANCE_TERRITORY_DATA_BACKFAIL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceTerritoryView::refresh), TERRITORY_INFO_CELL_CLICK, NULL);
}

void AllianceTerritoryView::onExit()
{
    territoryOpenVec.clear();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TERRITORY_INFO_CELL_CLICK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_TERRITORY_DATA_BACK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_TERRITORY_DATA_BACKFAIL);
    CCNode::onExit();
}

//void AllianceTerritoryView::scrollViewDidScroll(CCScrollView* view){
//    float mindy = m_tabView->minContainerOffset().y;
//    float dy = m_tabView->getContentOffset().y;
//    if(dy<mindy){
//        m_tabView->setContentOffset(ccp(0, mindy));
//    }
//}

AllianceTerritoryView::~AllianceTerritoryView()
{
}

void AllianceTerritoryView::onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45208");
//    CCCommonUtils::showHelpShiftFAQ("45203");
//    HelpshiftCocos2dx::showFAQSection("468");
}

SEL_CCControlHandler AllianceTerritoryView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", AllianceTerritoryView::onTipBtnClick);
    return NULL;
}

bool AllianceTerritoryView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    return false;
}

///////AllilanceTerritoryInfoCell

AllianceTerritoryInfoCell* AllianceTerritoryInfoCell::create(int index, int type, bool tipFlag)//index：0为哨塔 1为超级矿 2为箭塔 type：0为未打开 1为打开
{
    auto ret = new AllianceTerritoryInfoCell(tipFlag,index,type,true,0,0);
    if (ret && ret->init(index, type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceTerritoryInfoCell::init(int index, int type)
{
    bool ret = true;
    CCBLoadFile("AllianceTerritoryInfoCell",this,this);
    setData(index, type);
    return ret;
}

void AllianceTerritoryInfoCell::setData(int index, int type)
{
    m_curY = 0;
    m_index = index;
    m_type = type;
    
    if (m_index == 0) {
        m_titleLabel->setString(_lang("115392"));
        if (m_tipFlag == true) {
            m_tipLabel->setString(_lang("115304"));
        }
        else {
           m_tipLabel->setString(_lang("115303"));
        }
    }
    else if (m_index == 1) {
        m_titleLabel->setString(_lang("115373"));
        if (m_tipFlag == true) {
            m_tipLabel->setString(_lang("115374"));
        }
        else {
            m_tipLabel->setString(_lang("115394"));
        }
    }
    else if (m_index == 2) {
        m_titleLabel->setString(_lang_1("115406", ""));
        if (m_tipFlag == true) {
            m_tipLabel->setString(_lang("115404"));
        }
        else {
            m_tipLabel->setString(_lang("115405"));
        }
    }
    else if (m_index == 3) {
        m_titleLabel->setString(_lang("115364"));
        if (m_tipFlag == true) {
            m_tipLabel->setString(_lang("115444"));
        }
        else {
            m_tipLabel->setString(_lang("115445"));
        }
    }
    else if (m_index == 4) {
        m_titleLabel->setString(_lang("115534"));
        if (m_tipFlag == true) {
            m_tipLabel->setString(_lang_1("115524", CC_ITOA(WorldController::getInstance()->m_territoryBannerCost)));
        }
        else {
            m_tipLabel->setString(_lang("115523"));
        }
    }
    
    if (m_type==0) {
        m_jiantouSpr->setRotation(0);
        m_tipNode->setVisible(false);
    }
    else {
        m_tipNode->setVisible(true);
        m_jiantouSpr->setRotation(90);
    }
    m_mainNode->setPositionY(90);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_mainNode->setPositionY(150);
    }
    m_listNode->removeAllChildren();
}

void AllianceTerritoryInfoCell::onEnter() {
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCNode::onEnter();
}

void AllianceTerritoryInfoCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AllianceTerritoryInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_myTouchNode", CCNode*, m_myTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jiantouSpr", CCSprite*, m_jiantouSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipLabel", CCLabelIF*, this->m_tipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode", CCNode*, m_tipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBg", CCScale9Sprite*, m_tipBg);
    return false;
}

SEL_CCControlHandler AllianceTerritoryInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool AllianceTerritoryInfoCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_myTouchNode, pTouch)) {
        m_startY = pTouch->getLocation().y;
        return true;
    }
    return false;
}

void AllianceTerritoryInfoCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
//    int mv = pTouch->getLocation().y - m_startY;
//    if (abs(mv)>20) {
//        return;
//    }
//    //    if (!m_canTouch) {
//    //        return;
//    //    }
    if (m_type==0) {
        m_type = 1;
        onClickOpen(90);
        for (int i = 0; i < territoryOpenVec.size(); i++) {
            if (i != m_index) {
                territoryOpenVec[i] = 0;
            }
        }
        territoryOpenVec[m_index] = 1;
        GlobalData::shared()->alliance_territory_index = m_index;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TERRITORY_INFO_CELL_CLICK, CCInteger::create(m_index));
    }
    else {
        m_type = 0;
        onClickOpen(0);
        territoryOpenVec[m_index] = 0;
        GlobalData::shared()->alliance_territory_index = -1;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TERRITORY_INFO_CELL_CLICK, CCInteger::create(-1));
    }
}

void AllianceTerritoryInfoCell::onClickOpen(float rat)
{
    CCActionInterval * rotateto = CCRotateTo::create(0.2, rat);
    m_jiantouSpr->runAction(rotateto);
}

void AllianceTerritoryInfoCell::onResetCanTouch()
{
    m_canTouch = true;
}

void AllianceTerritoryInfoCell::bind(int num, int index, unsigned int target, AllianceTerritoryInfo* info1, AllianceTerritoryInfo* info2, WorldCityType type)
{
    auto cell = AllianceTerritoryCell::create(num, index, target, info1, info2, type);
    cell->setPosition(ccp(0, m_curY));
    if (CCCommonUtils::isIosAndroidPad())
    {
        cell->setPositionX(86);
    }
    m_listNode->addChild(cell);
    if (!CCCommonUtils::isIosAndroidPad()) {
        m_curY -= 420;
        m_mainNode->setPositionY(160+abs(m_curY));
    } else {
        m_curY -= 800;
        m_mainNode->setPositionY(290+abs(m_curY));
    }
}

/////////////
AllianceTerritoryCell* AllianceTerritoryCell::create(int num, int index, unsigned int target, AllianceTerritoryInfo* info1, AllianceTerritoryInfo* info2, WorldCityType type)
{
    auto ret = new AllianceTerritoryCell(num,index,target,info1,info2,type);
    if (ret && ret->init(info1,info2)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceTerritoryCell::init(AllianceTerritoryInfo* info1, AllianceTerritoryInfo* info2)
{
    bool ret = true;
    
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });

    CCBLoadFile("AllianceTerritoryCell",this,this);
    
    m_ptIndex1 = -1;
    m_ptIndex2 = -1;
    m_territoryId1 = "";
    m_territoryId2 = "";
    m_territoryName1 = "";
    m_territoryName2 = "";
    m_territoryCount1 = 0;
    m_territoryCount2 = 0;
    m_territoryResourceType1 = info1->m_territoryResourceType;
    if (info2 != NULL) {
        m_territoryResourceType2 = info2->m_territoryResourceType;
    }
    setData(m_index,info1,info2,m_type);
    return ret;
}

void AllianceTerritoryCell::setData(int index,AllianceTerritoryInfo* info1, AllianceTerritoryInfo* info2, WorldCityType type)
{
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    m_index = index;
    m_type = m_type;
    
    m_tNode1->setVisible(true);
    m_tNode2->setVisible(true);
    
    //左边
    if (info1->m_territoryActive == 0) {//锁定状态
        m_lockNode1->setVisible(info1->m_lockFlag);
        m_territoryName1 = info1->m_territoryName;
        m_territoryCount1 = info1->m_territoryCount;
        m_placeBtn1->setVisible(false);
        m_infoBgPlace1->setVisible(true);
        m_statUnplaceLabel1->setVisible(false);
        m_lockTipLabel1->setVisible(true);
        if (info1->m_territoryType == Tile_allianceArea) {
            m_lockTipLabel1->setString(_lang("115359"));
        }
        else if (info1->m_territoryType == tile_superMine) {
            m_lockTipLabel1->setString(_lang("115393"));
        }
        else if (info1->m_territoryType == tile_tower) {
            if (info1->m_lockFlag == true) {
                m_lockTipLabel1->setString(_lang("115359"));
            }
            else
                m_lockTipLabel1->setString(_lang("115393"));
        }
        else if (info1->m_territoryType == tile_wareHouse) {
            m_lockTipLabel1->setString(_lang("115393"));
        }
        else if (info1->m_territoryType == tile_banner) {
            m_lockTipLabel1->setString(_lang("115393"));
        }
//        m_lockTipLabel1->setColor({198,190,178});
        m_infoBgPlace1->setPositionY(m_infoBgPlace1->getPositionY() + 24);
        m_lockTipLabel1->setPositionY(m_lockTipLabel1->getPositionY() + 24);
        int labelLen = m_lockTipLabel1->getContentSize().width * m_lockTipLabel1->getOriginScaleX();
        m_lockNode1->setPositionX(m_lockTipLabel1->getPositionX() + labelLen/2 + 30);
        m_lockNode1->setPositionY(m_lockNode1->getPositionY() + 24);
        m_statBgPlace1->setVisible(false);
        m_coordIcon1->setVisible(false);
        m_nameLabel1->setVisible(true);
        m_nameLabel1->setString(info1->m_territoryName);
        m_placeLabel1->setVisible(false);
        m_statPlaceLabel1->setVisible(false);
        m_defLabel1->setVisible(false);
        m_coordLabel1->setVisible(false);
        m_underlineSpr1->setVisible(false);
        
        //图片置灰
        
        auto& children = m_bgNode1->getChildren();
        for (auto child : children)
        {
            CCSprite* tmpSpr = dynamic_cast<CCSprite*>(child);
            if (tmpSpr) {
                CCCommonUtils::setSpriteGray(tmpSpr, true);
            }
        }
//        auto arrSpr = m_bgNode1->getChildren();
//        if (arrSpr) {
//            for (int i=0; i<arrSpr->count(); i++) {
//                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(arrSpr->objectAtIndex(i));
//                if (tmpSpr) {
//                    CCCommonUtils::setSpriteGray(tmpSpr, true);
//                }
//            }
//        }
        
        if (info1->m_territoryType == Tile_allianceArea) {//////////哨塔图片置灰
            auto pic = CCLoadSprite::createSprite("territory_building_lv1.png");
            if (pic) {
                CCCommonUtils::setSpriteGray(pic, true);
                m_picNode1->addChild(pic);
            }
        }
        else if (info1->m_territoryType == tile_superMine) {//////////超级矿图片置灰
            std::string fileName = "";
            switch (info1->m_territoryResourceType) {
                case Food:
                    fileName = "territory_resource_food.png";
                    break;
                case Wood:
                    fileName = "territory_resource_wood.png";
                    break;
                case Iron:
                    fileName = "territory_resource_iron.png";
                    break;
                case Stone:
                    fileName = "territory_resource_stone.png";
                    break;
                default:
                    break;
            }
            auto pic = CCLoadSprite::createSprite(fileName.c_str());
            if (pic) {
                CCCommonUtils::setSpriteGray(pic, true);
                m_picNode1->addChild(pic);
            }
        }
        else if (info1->m_territoryType == tile_tower) {//箭塔
            auto pic = CCLoadSprite::createSprite("territory_tower0.png");
            if (pic) {
                CCCommonUtils::setSpriteGray(pic, true);
                m_picNode1->addChild(pic);
            }
        }
        else if (info1->m_territoryType == tile_wareHouse) {//仓库
            auto pic = CCLoadSprite::createSprite("territory_warehouse0.png");
            if (pic) {
                CCCommonUtils::setSpriteGray(pic, true);
                m_picNode1->addChild(pic);
            }
        }
        else if (info1->m_territoryType == tile_banner) {//国旗////////////////////////
            auto pic = CCLoadSprite::createSprite("territory_tower0.png");
            if (pic) {
                CCCommonUtils::setSpriteGray(pic, true);
                m_picNode1->addChild(pic);
            }
        }
    }
    else {
        m_lockNode1->setVisible(false);
        m_picNode1->removeAllChildren();
        std::string picStr = "";
        if (info1->m_territoryType == Tile_allianceArea) {
            picStr = "territory_building_lv1.png";
        }
        else if (info1->m_territoryType == tile_superMine) {
            switch (info1->m_territoryResourceType) {
                case Food:
                    picStr = "territory_resource_food.png";
                    break;
                case Wood:
                    picStr = "territory_resource_wood.png";
                    break;
                case Iron:
                    picStr = "territory_resource_iron.png";
                    break;
                case Stone:
                    picStr = "territory_resource_stone.png";
                    break;
                default:
                    break;
            }
        }
        else if (info1->m_territoryType == tile_tower) {
            picStr = "territory_tower0.png";
        }
        else if (info1->m_territoryType == tile_wareHouse) {
             picStr = "territory_warehouse0.png";
        }
        else if (info1->m_territoryType == tile_banner) {
            picStr = "territory_tower0.png";/////////////////
        }
        auto pic = CCLoadSprite::createSprite(picStr.c_str());
        if (pic) {
            m_picNode1->addChild(pic);
        }
        if (info1->m_territoryPtIndex == 0) {
            m_territoryId1 = info1->m_territoryId;
            m_placeBtn1->setVisible(true);
            if (GlobalData::shared()->playerInfo.allianceInfo.rank<4) {
                m_placeBtn1->setEnabled(false);
            }
            if (!CCCommonUtils::isShowFlagForChinese() && m_type == tile_banner) {
                m_placeBtn1->setVisible(false);//如果是大陆地区的，检测是否可以显示
            }
            m_infoBgPlace1->setVisible(true);
            m_lockTipLabel1->setVisible(false);
            m_statUnplaceLabel1->setVisible(true);
            if (info1->m_territoryType == Tile_allianceArea) {
                m_statUnplaceLabel1->setString(_lang("115384"));
            }
            else if (info1->m_territoryType == tile_superMine) {
                m_statUnplaceLabel1->setString(_lang("115383"));
            }
            else if (info1->m_territoryType == tile_tower) {
                m_statUnplaceLabel1->setString(_lang("115411"));
            }
            else if (info1->m_territoryType == tile_wareHouse) {
                m_statUnplaceLabel1->setString(_lang("115446"));
            }
            else if (info1->m_territoryType == tile_banner) {
                m_statUnplaceLabel1->setString(_lang("115535"));
            }
//            m_statUnplaceLabel1->setColor({198,190,178});
            m_infoBgPlace1->setPositionY(m_infoBgPlace1->getPositionY() + 24);
            m_statUnplaceLabel1->setPositionY(m_statUnplaceLabel1->getPositionY() + 24);
            m_statBgPlace1->setVisible(false);
            m_coordIcon1->setVisible(false);
            m_nameLabel1->setVisible(true);
            std::string tempName = "";
            tempName = info1->m_territoryName;
            m_nameLabel1->setString(tempName);
//            m_nameLabel1->setColor({198,190,178});;
            m_placeLabel1->setVisible(true);
            m_placeLabel1->setString(_lang("115302"));
//            m_placeLabel1->setColor({198,190,178});
            m_statPlaceLabel1->setVisible(false);
            m_defLabel1->setVisible(false);
            m_coordLabel1->setVisible(false);
            m_underlineSpr1->setVisible(false);
        }
        else {
            m_placeBtn1->setVisible(false);
            m_infoBgPlace1->setVisible(true);
            m_statBgPlace1->setVisible(true);
            m_coordIcon1->setVisible(true);
            m_nameLabel1->setVisible(true);
            std::string tempName = "";
            tempName = info1->m_territoryName;
            m_nameLabel1->setString(tempName);
//            m_nameLabel1->setColor({198,190,178});
            m_placeLabel1->setVisible(false);
            m_statPlaceLabel1->setVisible(true);
            m_defLabel1->setVisible(true);
            if (info1->m_territoryType == Tile_allianceArea) {
                m_defLabel1->setString(_lang_1("115345", CC_CMDITOA(info1->m_territoryDef).c_str()));
            }
            else if (info1->m_territoryType == tile_superMine) {
                std::string resStr = _lang("115382");
                if (info1->m_territoryState == 3 || info1->m_territoryState == 4 || info1->m_territoryState == 6) {
                    resStr.append(CC_CMDITOA(info1->m_territoryRes));
                    m_defLabel1->setString(resStr.c_str());
                }
                else
                    m_defLabel1->setString(_lang_1("115345", CC_CMDITOA(info1->m_territoryDef).c_str()));
            }
            else if (info1->m_territoryType == tile_tower) {
                m_defLabel1->setString(_lang_1("115345", CC_CMDITOA(info1->m_territoryDef).c_str()));
            }
            else if (info1->m_territoryType == tile_wareHouse) {
                std::string resStr = _lang("115449");
                if (info1->m_territoryState == 3) {
                    resStr.append(CC_CMDITOA(info1->m_totalResStorage*1.0));
                    m_defLabel1->setString(resStr.c_str());
                }
                else
                    m_defLabel1->setString(_lang_1("115345", CC_CMDITOA(info1->m_territoryDef).c_str()));
            }
            else if (info1->m_territoryType == tile_banner) {
                m_defLabel1->setString(_lang_1("115345", CC_CMDITOA(info1->m_territoryDef).c_str()));
            }
//            m_defLabel1->setColor({198,190,178});
            m_coordLabel1->setVisible(true);
            m_ptIndex1 = info1->m_territoryPtIndex;
            std::string coordStr = "X:";
            CCPoint tPoint = WorldController::getInstance()->getPointByIndex(info1->m_territoryPtIndex);
            coordStr.append(CC_ITOA(tPoint.x));
            coordStr.append(" ");
            coordStr.append("Y:");
            coordStr.append(CC_ITOA(tPoint.y));
            m_coordLabel1->setString(coordStr);
//            m_coordLabel1->setColor(ccWHITE);
            m_statUnplaceLabel1->setVisible(false);
            m_underlineSpr1->setVisible(true);
            
            switch (info1->m_territoryState) {
                case 0: {
                    m_statPlaceLabel1->setVisible(true);
                    m_statPlaceLabel1->setString(_lang("115305"));
                }
                    break;
                case 1: {
                    m_statPlaceLabel1->setVisible(true);
                    m_statPlaceLabel1->setString(_lang("115306"));
                }
                    break;
                case 2: {
                    m_statPlaceLabel1->setVisible(true);
                    if (info1->m_territoryType == Tile_allianceArea) {
                        m_statPlaceLabel1->setString(_lang("115307"));
                    }
                }
                    break;
                case 3: {
                    m_statPlaceLabel1->setVisible(true);
                    if (info1->m_territoryType == Tile_allianceArea) {
                        m_statPlaceLabel1->setString(_lang("115308"));
                    }
                    else if (info1->m_territoryType == tile_superMine) {
                        m_statPlaceLabel1->setString(_lang("115381"));
                    }
                    else if (info1->m_territoryType == tile_tower) {
                        m_statPlaceLabel1->setString(_lang("115409"));
                    }
                    else if (info1->m_territoryType == tile_wareHouse) {
                        m_statPlaceLabel1->setString(_lang("115448"));
                    }
                    else if (info1->m_territoryType == tile_banner) {
                        m_statPlaceLabel1->setString(_lang("115308"));
                    }
                }
                    break;
                case 4: {
                    m_statPlaceLabel1->setVisible(true);
                    if (info1->m_territoryType == Tile_allianceArea) {
                        m_statPlaceLabel1->setString(_lang("115309"));
                    }
                    else if (info1->m_territoryType == tile_superMine) {
                        m_statPlaceLabel1->setString(_lang("115381"));
                    }
                    else if (info1->m_territoryType == tile_banner) {
                        m_statPlaceLabel1->setString(_lang("115309"));
                    }
                }
                    break;
                case 5: {
                    m_statPlaceLabel1->setVisible(true);
                    if (info1->m_territoryType == Tile_allianceArea) {
                        m_statPlaceLabel1->setString(_lang("115310"));
                    }
                    else if (info1->m_territoryType == tile_banner) {
                        m_statPlaceLabel1->setString(_lang("115310"));
                    }
                }
                    break;
                case 6: {
                    m_statPlaceLabel1->setVisible(true);
                    if (info1->m_territoryType == Tile_allianceArea) {
                        m_statPlaceLabel1->setString(_lang("115311"));
                    }
                    else if (info1->m_territoryType == tile_superMine) {
                        m_statPlaceLabel1->setString(_lang("115380"));
                    }
                    else if (info1->m_territoryType == tile_banner) {
                        m_statPlaceLabel1->setString(_lang("115311"));
                    }
                }
                    break;
                default:
                    break;
            }
            m_statPlaceLabel1->setColor({255,0,0});
        }
    }
    
    //右边
    if (m_index*2+1 >= m_totalNum) {
        this->m_tNode2->setVisible(false);
    }
    else {
        if(info2->m_territoryActive == 0){
            m_lockNode2->setVisible(info2->m_lockFlag);
            m_territoryName2 = info2->m_territoryName;
            m_territoryCount2 = info2->m_territoryCount;
            m_placeBtn2->setVisible(false);
            m_infoBgPlace2->setVisible(true);
            m_statUnplaceLabel2->setVisible(false);
            m_lockTipLabel2->setVisible(true);
            if (info2->m_territoryType == Tile_allianceArea) {
                m_lockTipLabel2->setString(_lang("115359"));
            }
            else if (info2->m_territoryType == tile_superMine) {
                m_lockTipLabel2->setString(_lang("115393"));
            }
            else if (info2->m_territoryType == tile_tower) {
                if (info2->m_lockFlag == true) {
                    m_lockTipLabel2->setString(_lang("115359"));
                }
                else
                    m_lockTipLabel2->setString(_lang("115393"));
            }
            else if (info2->m_territoryType == tile_wareHouse) {
                m_lockTipLabel2->setString(_lang("115393"));
            }
            else if (info2->m_territoryType == tile_banner) {
                m_lockTipLabel2->setString(_lang("115393"));
            }
//            m_lockTipLabel2->setColor({198,190,178});
            m_infoBgPlace2->setPositionY(m_infoBgPlace2->getPositionY() + 24);
            m_lockTipLabel2->setPositionY(m_lockTipLabel2->getPositionY() + 24);
            int labelLen = m_lockTipLabel2->getContentSize().width * m_lockTipLabel2->getOriginScaleX();
            m_lockNode2->setPositionX(m_lockTipLabel2->getPositionX() + labelLen/2 + 30);
            m_lockNode2->setPositionY(m_lockNode2->getPositionY() + 24);
            m_statBgPlace2->setVisible(false);
            m_coordIcon2->setVisible(false);
            m_nameLabel2->setVisible(true);
            m_nameLabel2->setString(info2->m_territoryName);
            m_placeLabel2->setVisible(false);
            m_statPlaceLabel2->setVisible(false);
            m_defLabel2->setVisible(false);
            m_coordLabel2->setVisible(false);
            m_underlineSpr2->setVisible(false);

            //图片置灰
//            auto arrSpr = m_bgNode2->getChildren();
//            if (arrSpr) {
//                for (int i=0; i<arrSpr->count(); i++) {
//                    CCSprite* tmpSpr = dynamic_cast<CCSprite*>(arrSpr->objectAtIndex(i));
//                    if (tmpSpr) {
//                        CCCommonUtils::setSpriteGray(tmpSpr, true);
//                    }
//                }
//            }
            auto& children = m_bgNode2->getChildren();
            for (auto child : children)
            {
                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(child);
                if (tmpSpr) {
                    CCCommonUtils::setSpriteGray(tmpSpr, true);
                }
            }
            
            if (info2->m_territoryType == Tile_allianceArea) {//////////哨塔图片置灰
                auto pic = CCLoadSprite::createSprite("territory_building_lv1.png");
                if (pic) {
                    CCCommonUtils::setSpriteGray(pic, true);
                    m_picNode2->addChild(pic);
                }
            }
            else if (info2->m_territoryType == tile_superMine) {//////////超级矿图片置灰
                std::string fileName = "";
                switch (info2->m_territoryResourceType) {
                    case Food:
                        fileName = "territory_resource_food.png";
                        break;
                    case Wood:
                        fileName = "territory_resource_wood.png";
                        break;
                    case Iron:
                        fileName = "territory_resource_iron.png";
                        break;
                    case Stone:
                        fileName = "territory_resource_stone.png";
                        break;
                    default:
                        break;
                }
                auto pic = CCLoadSprite::createSprite(fileName.c_str());
                if (pic) {
                    CCCommonUtils::setSpriteGray(pic, true);
                    m_picNode2->addChild(pic);
                }
            }
            else if (info2->m_territoryType == tile_tower) {//箭塔
                auto pic = CCLoadSprite::createSprite("territory_tower0.png");
                if (pic) {
                    CCCommonUtils::setSpriteGray(pic, true);
                    m_picNode2->addChild(pic);
                }
            }
            else if (info2->m_territoryType == tile_wareHouse) {//仓库
                auto pic = CCLoadSprite::createSprite("territory_warehouse0.png");
                if (pic) {
                    CCCommonUtils::setSpriteGray(pic, true);
                    m_picNode2->addChild(pic);
                }
            }
            else if (info2->m_territoryType == tile_banner) {//国旗///////////////////////
                auto pic = CCLoadSprite::createSprite("territory_tower0.png");
                if (pic) {
                    CCCommonUtils::setSpriteGray(pic, true);
                    m_picNode2->addChild(pic);
                }
            }
        }else{
            m_lockNode2->setVisible(false);
            m_picNode2->removeAllChildren();
            std::string picStr = "";
            if (info2->m_territoryType == Tile_allianceArea) {
                picStr = "territory_building_lv1.png";
            }
            else if (info2->m_territoryType == tile_superMine) {
                switch (info2->m_territoryResourceType) {
                    case Food:
                        picStr = "territory_resource_food.png";
                        break;
                    case Wood:
                        picStr = "territory_resource_wood.png";
                        break;
                    case Iron:
                        picStr = "territory_resource_iron.png";
                        break;
                    case Stone:
                        picStr = "territory_resource_stone.png";
                        break;
                    default:
                        break;
                }
            }
            else if (info2->m_territoryType == tile_tower) {
                picStr = "territory_tower0.png";
            }
            else if (info2->m_territoryType == tile_wareHouse) {
                picStr = "territory_warehouse0.png";
            }
            else if (info2->m_territoryType == tile_banner) {//////////////////
                picStr = "territory_tower0.png";
            }
            auto pic = CCLoadSprite::createSprite(picStr.c_str());
            if (pic) {
                m_picNode2->addChild(pic);
            }
            if (info2->m_territoryPtIndex == 0) {
                m_territoryId2 = info2->m_territoryId;
                m_placeBtn2->setVisible(true);
                if (GlobalData::shared()->playerInfo.allianceInfo.rank<4) {
                    m_placeBtn2->setEnabled(false);
                }
                if (m_placeBtn2->isVisible() && !CCCommonUtils::isShowFlagForChinese() && m_type == tile_banner) {
                    m_placeBtn2->setVisible(false);//不能改国旗
                }
                m_infoBgPlace2->setVisible(true);
                m_lockTipLabel2->setVisible(false);
                m_statUnplaceLabel2->setVisible(true);
                if (info2->m_territoryType == Tile_allianceArea) {
                    m_statUnplaceLabel2->setString(_lang("115384"));
                }
                else if (info2->m_territoryType == tile_superMine) {
                    m_statUnplaceLabel2->setString(_lang("115383"));
                }
                else if (info2->m_territoryType == tile_tower) {
                    m_statUnplaceLabel2->setString(_lang("115411"));
                }
                else if (info2->m_territoryType == tile_wareHouse) {
                    m_statUnplaceLabel2->setString(_lang("115446"));
                }
                else if (info2->m_territoryType == tile_banner) {
                    m_statUnplaceLabel2->setString(_lang("115535"));
                }
//                m_statUnplaceLabel2->setColor({198,190,178});
                m_infoBgPlace2->setPositionY(m_infoBgPlace2->getPositionY() + 24);
                m_statUnplaceLabel2->setPositionY(m_statUnplaceLabel2->getPositionY() + 24);
                m_statBgPlace2->setVisible(false);
                m_coordIcon2->setVisible(false);
                m_nameLabel2->setVisible(true);
                std::string tempName = "";
                tempName = info2->m_territoryName;
                m_nameLabel2->setString(tempName);
//                m_nameLabel2->setColor({198,190,178});
                m_placeLabel2->setVisible(true);
                m_placeLabel2->setString(_lang("115302"));
//                m_placeLabel2->setColor({198,190,178});
                m_statPlaceLabel2->setVisible(false);
                m_defLabel2->setVisible(false);
                m_coordLabel2->setVisible(false);
                m_underlineSpr2->setVisible(false);
            }
            else {
                m_placeBtn2->setVisible(false);
                m_infoBgPlace2->setVisible(true);
                m_statBgPlace2->setVisible(true);
                m_coordIcon2->setVisible(true);
                m_nameLabel2->setVisible(true);
                std::string tempName = "";
                tempName = info2->m_territoryName;
                m_nameLabel2->setString(tempName);
//                m_nameLabel2->setColor({198,190,178});
                m_placeLabel2->setVisible(false);
                m_statPlaceLabel2->setVisible(true);
                m_defLabel2->setVisible(true);
                if (info2->m_territoryType == Tile_allianceArea) {
                    m_defLabel2->setString(_lang_1("115345", CC_CMDITOA(info2->m_territoryDef).c_str()));
                }
                else if (info2->m_territoryType == tile_superMine) {
                    std::string resStr = _lang("115382");
                    if (info2->m_territoryState == 3 || info2->m_territoryState == 4 || info2->m_territoryState == 6) {
                        resStr.append(CC_CMDITOA(info2->m_territoryRes));
                        m_defLabel2->setString(resStr.c_str());
                    }
                    else
                        m_defLabel2->setString(_lang_1("115345", CC_CMDITOA(info2->m_territoryDef).c_str()));
                    
                }
                else if (info2->m_territoryType == tile_tower) {
                    m_defLabel2->setString(_lang_1("115345", CC_CMDITOA(info2->m_territoryDef).c_str()));
                }
                else if (info2->m_territoryType == tile_wareHouse) {
                    std::string resStr = _lang("115449");
                    if (info2->m_territoryState == 3) {
                        resStr.append(CC_CMDITOA(info2->m_totalResStorage*1.0));
                        m_defLabel2->setString(resStr.c_str());
                    }
                    else
                        m_defLabel2->setString(_lang_1("115345", CC_CMDITOA(info2->m_territoryDef).c_str()));
                }
                else if (info2->m_territoryType == tile_banner) {
                    m_defLabel2->setString(_lang_1("115345", CC_CMDITOA(info2->m_territoryDef).c_str()));
                }
//                m_defLabel2->setColor({198,190,178});
                m_coordLabel2->setVisible(true);
                m_ptIndex2 = info2->m_territoryPtIndex;
                std::string coordStr = "X:";
                CCPoint tPoint = WorldController::getInstance()->getPointByIndex(info2->m_territoryPtIndex);
                coordStr.append(CC_ITOA(tPoint.x));
                coordStr.append(" ");
                coordStr.append("Y:");
                coordStr.append(CC_ITOA(tPoint.y));
                m_coordLabel2->setString(coordStr);
//                m_coordLabel2->setColor(ccWHITE);
                m_statUnplaceLabel2->setVisible(false);
                m_underlineSpr2->setVisible(true);
                
                switch (info2->m_territoryState) {
                    case 0: {
                        m_statPlaceLabel2->setVisible(true);
                        m_statPlaceLabel2->setString(_lang("115305"));
                    }
                        break;
                    case 1: {
                        m_statPlaceLabel2->setVisible(true);
                        m_statPlaceLabel2->setString(_lang("115306"));
                    }
                        break;
                    case 2: {
                        m_statPlaceLabel2->setVisible(true);
                        if (info2->m_territoryType == Tile_allianceArea) {
                            m_statPlaceLabel2->setString(_lang("115307"));
                        }
                        else if (info2->m_territoryType == tile_banner) {
                            m_statPlaceLabel2->setString(_lang("115307"));
                        }
                    }
                        break;
                    case 3: {
                        m_statPlaceLabel2->setVisible(true);
                        if (info2->m_territoryType == Tile_allianceArea) {
                            m_statPlaceLabel2->setString(_lang("115308"));
                        }
                        else if (info2->m_territoryType == tile_superMine) {
                            m_statPlaceLabel2->setString(_lang("115381"));
                        }
                        else if (info2->m_territoryType == tile_tower) {
                            m_statPlaceLabel2->setString(_lang("115409"));
                        }
                        else if (info2->m_territoryType == tile_wareHouse) {
                            m_statPlaceLabel2->setString(_lang("115448"));
                        }
                        else if (info2->m_territoryType == tile_banner) {
                            m_statPlaceLabel2->setString(_lang("115308"));
                        }
                    }
                        break;
                    case 4: {
                        m_statPlaceLabel2->setVisible(true);
                        if (info2->m_territoryType == Tile_allianceArea) {
                            m_statPlaceLabel2->setString(_lang("115309"));
                        }
                        else if (info2->m_territoryType == tile_superMine) {
                            m_statPlaceLabel2->setString(_lang("115381"));
                        }
                        else if (info2->m_territoryType == tile_banner) {
                            m_statPlaceLabel2->setString(_lang("115309"));
                        }
                    }
                        break;
                    case 5: {
                        m_statPlaceLabel2->setVisible(true);
                        if (info2->m_territoryType == Tile_allianceArea) {
                            m_statPlaceLabel2->setString(_lang("115310"));
                        }
                        else if (info2->m_territoryType == tile_banner) {
                            m_statPlaceLabel2->setString(_lang("115310"));
                        }
                    }
                        break;
                    case 6: {
                        m_statPlaceLabel2->setVisible(true);
                        if (info2->m_territoryType == Tile_allianceArea) {
                            m_statPlaceLabel2->setString(_lang("115311"));
                        }
                        else if (info2->m_territoryType == tile_superMine) {
                            m_statPlaceLabel2->setString(_lang("115380"));
                        }
                        else if (info2->m_territoryType == tile_banner) {
                            m_statPlaceLabel2->setString(_lang("115311"));
                        }
                    }
                        break;
                    default:
                        break;
                }
                m_statPlaceLabel2->setColor({255,0,0});
            }
        }
    }
}

void AllianceTerritoryCell::onEnter() {
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);//
}

void AllianceTerritoryCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler AllianceTerritoryCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnPlaceBtnClick1", AllianceTerritoryCell::OnPlaceBtnClick1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnPlaceBtnClick2", AllianceTerritoryCell::OnPlaceBtnClick2);
    return NULL;
}

void AllianceTerritoryCell::OnPlaceBtnClick1(CCObject * pSender, Control::EventType pCCControlEvent)
{
    auto tPoint = WorldController::getPointByIndex(m_targetIndex);
    std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
    int isFirstPop = CCUserDefault::sharedUserDefault()->getIntegerForKey(isFirstPopKey.c_str(),0);
    
    if(GlobalData::shared()->playerInfo.isInSelfServer()){
        if (m_type == tile_banner) {
            PopupViewController::getInstance()->addPopupInView(TerritoryBannerChoseView::create(m_targetIndex));
        }
        else {
            WorldController::getInstance()->buildAllianceTerritory = 1;
            WorldController::getInstance()->allianceTerritoryIndex = m_targetIndex;
            WorldController::getInstance()->allianceTerritoryId = m_territoryId1;
            WorldController::getInstance()->openTargetIndex = m_targetIndex;
            WorldController::getInstance()->allianceTerritoryType = m_type;
            WorldController::getInstance()->allianceTerritoryResourceType = m_territoryResourceType1;
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                WorldMapView::instance()->gotoTilePoint(tPoint,true);
            }else{
                if (isFirstPop==0) {
                    WorldController::getInstance()->firstInWorld = true;
                    isFirstPop = 1;
                    CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), isFirstPop);
                    CCUserDefault::sharedUserDefault()->flush();
                }
                
                SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, m_targetIndex);
            }
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
}
void AllianceTerritoryCell::OnPlaceBtnClick2(CCObject * pSender, Control::EventType pCCControlEvent)
{
    auto tPoint = WorldController::getPointByIndex(m_targetIndex);
    std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
    int isFirstPop = CCUserDefault::sharedUserDefault()->getIntegerForKey(isFirstPopKey.c_str(),0);
    
    if(GlobalData::shared()->playerInfo.isInSelfServer()){
        if (m_type == tile_banner) {
            PopupViewController::getInstance()->addPopupInView(TerritoryBannerChoseView::create(m_targetIndex));
        }
        else {
            WorldController::getInstance()->buildAllianceTerritory = 1;
            WorldController::getInstance()->allianceTerritoryIndex = m_targetIndex;
            WorldController::getInstance()->allianceTerritoryId = m_territoryId2;
            WorldController::getInstance()->openTargetIndex = m_targetIndex;
            WorldController::getInstance()->allianceTerritoryType = m_type;
            WorldController::getInstance()->allianceTerritoryResourceType = m_territoryResourceType2;
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                WorldMapView::instance()->gotoTilePoint(tPoint,true);
            }else{
                if (isFirstPop==0) {
                    WorldController::getInstance()->firstInWorld = true;
                    isFirstPop = 1;
                    CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), isFirstPop);
                    CCUserDefault::sharedUserDefault()->flush();
                }
                
                SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, m_targetIndex);
            }
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
}

bool AllianceTerritoryCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_hintBGNode,pTouch)) {
        return true;
    }
    return false;
}
void AllianceTerritoryCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
void AllianceTerritoryCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_hintBGNode, pTouch)) {
        if (isTouchInside(m_coordNode1, pTouch)) {
            if (m_coordLabel1->isVisible()) {
                int pos = m_ptIndex1;
                WorldController::getInstance()->openTargetIndex = pos;
                CCPoint pt = WorldController::getPointByIndex(pos);
                if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                    WorldMapView::instance()->gotoTilePoint(pt);
                }else{
                    int index = WorldController::getIndexByPoint(pt);
                    SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
                }
                PopupViewController::getInstance()->removeAllPopupView();
                
            }
        }
        
        if (isTouchInside(m_coordNode2, pTouch)) {
            if (m_coordLabel2->isVisible()) {
                int pos = m_ptIndex2;
                WorldController::getInstance()->openTargetIndex = pos;
                CCPoint pt = WorldController::getPointByIndex(pos);
                if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                    WorldMapView::instance()->gotoTilePoint(pt);
                }else{
                    int index = WorldController::getIndexByPoint(pt);
                    SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
                }
                PopupViewController::getInstance()->removeAllPopupView();
               
            }
        }
        
        if (isTouchInside(m_placeBtn1, pTouch)) {
            if (m_placeBtn1->isVisible() && GlobalData::shared()->playerInfo.allianceInfo.rank<4) {
                CCCommonUtils::flyHint("", "", _lang("115286"), 3, 0, true);
            }
        }
        
        if (isTouchInside(m_placeBtn2, pTouch)) {
            if (m_placeBtn2->isVisible() && GlobalData::shared()->playerInfo.allianceInfo.rank<4) {
                CCCommonUtils::flyHint("", "", _lang("115286"), 3, 0, true);
            }
        }
        
        if (isTouchInside(m_lockNode1, pTouch)) {
            if (m_lockNode1->isVisible()) {
                if (m_type == Tile_allianceArea) {
                    PopupViewController::getInstance()->addPopupView(TerritoryUnlockTipsView::create(m_territoryName1, m_territoryCount1));
                }
                else if (m_type == tile_superMine) {
                    CCCommonUtils::flyHint("", "", _lang("115375"), 3, 0, true);
                }
                else if (m_type == tile_tower) {
                    CCCommonUtils::flyHint("", "", _lang("115410"), 3, 0, true);
                }
            }
        }
        
        if (isTouchInside(m_lockNode2, pTouch)) {
            if (m_lockNode2->isVisible()) {
                if (m_type == Tile_allianceArea) {
                    PopupViewController::getInstance()->addPopupView(TerritoryUnlockTipsView::create(m_territoryName2, m_territoryCount2));
                }
                else if (m_type == tile_superMine) {
                    CCCommonUtils::flyHint("", "", _lang("115375"), 3, 0, true);
                }
                else if (m_type == tile_tower) {
                    CCCommonUtils::flyHint("", "", _lang("115410"), 3, 0, true);
                }
            }
        }
    }
}

bool AllianceTerritoryCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel1", CCLabelIF*, m_nameLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_placeLabel1", CCLabelIF*, m_placeLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statPlaceLabel1", CCLabelIF*, m_statPlaceLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defLabel1", CCLabelIF*, m_defLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordLabel1", CCLabelIF*, m_coordLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statUnplaceLabel1", CCLabelIF*, m_statUnplaceLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockTipLabel1", CCLabelIF*, m_lockTipLabel1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel2", CCLabelIF*, m_nameLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_placeLabel2", CCLabelIF*, m_placeLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statPlaceLabel2", CCLabelIF*, m_statPlaceLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defLabel2", CCLabelIF*, m_defLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordLabel2", CCLabelIF*, m_coordLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statUnplaceLabel2", CCLabelIF*, m_statUnplaceLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockTipLabel2", CCLabelIF*, m_lockTipLabel2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBgPlace1", CCNode*, m_infoBgPlace1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statBgPlace1", CCNode*, m_statBgPlace1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordIcon1", CCSprite*, m_coordIcon1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr1", CCScale9Sprite*, m_underlineSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNode1", CCSprite*, m_lockNode1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBgPlace2", CCNode*, m_infoBgPlace2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statBgPlace2", CCNode*, m_statBgPlace2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordIcon2", CCSprite*, m_coordIcon2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr2", CCScale9Sprite*, m_underlineSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNode2", CCSprite*, m_lockNode2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tNode1", CCNode*, m_tNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tNode2", CCNode*, m_tNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordNode1", CCNode*, m_coordNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordNode2", CCNode*, m_coordNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintBGNode", CCNode*, m_hintBGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode1", CCNode*, m_bgNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode2", CCNode*, m_bgNode2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_placeBtn1", CCControlButton*, m_placeBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_placeBtn2", CCControlButton*, m_placeBtn2);
    return false;
}


