//
//  EquipmentListView.cpp
//  IF
//
//  Created by fubin on 14-12-18.
//
//

#include "EquipmentListView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "EquipmentController.h"
#include "EquipmentInfo.h"
#include "EquipmentCreateView.h"
#include "QueueController.h"
#include "FunBuildController.h"
#include "GuideController.h"

//bool antiCompareEqui(int item1, int item2);

EquipmentListView* EquipmentListView::create(int site){
    EquipmentListView* ret = new EquipmentListView();
    if(ret && ret->init(site)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentListView::init(int site)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(306, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(306, false);
    });
    
    setIsHDPanel(true);
    m_weapType = site;
    auto bg = CCBLoadFile("EquipmentListView",this,this);
        setContentSize(bg->getContentSize());
        changeBGHeight(m_buildBG);
        int extH = getExtendHeight();
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + extH));
        m_infoList->setPositionY(m_infoList->getPositionY() - extH);
        m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
        
        auto tbg = CCLoadSprite::loadResource("BG_equip_top.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        auto pic3Left = CCLoadSprite::createSprite("BG_equip_bottom.png");
        pic3Left->setAnchorPoint(ccp(0, 0));
        pic3Left->setPosition(ccp(0, 0));
        tBatchNode->addChild(pic3Left);
        auto pic3Right = CCLoadSprite::createSprite("BG_equip_bottom.png");
        pic3Right->setScaleX(-1);
        pic3Right->setAnchorPoint(ccp(1, 0));
        pic3Right->setPosition(ccp(317, 0));
        tBatchNode->addChild(pic3Right);
        int maxHeight = CCDirector::sharedDirector()->getWinSize().height - 86;
    if (CCCommonUtils::isIosAndroidPad()) {
        maxHeight = 852 - 60;
    }
        int curHeight = pic3Left->getContentSize().height;
        while(curHeight < maxHeight)
        {
            auto pic2Left = CCLoadSprite::createSprite("BG_equip_middle.png");
            pic2Left->setAnchorPoint(ccp(0, 0));
            pic2Left->setPosition(ccp(0, curHeight));
            tBatchNode->addChild(pic2Left);
            auto pic2Right = CCLoadSprite::createSprite("BG_equip_middle.png");
            pic2Right->setScaleX(-1);
            pic2Right->setAnchorPoint(ccp(1, 0));
            pic2Right->setPosition(ccp(317, curHeight));
            tBatchNode->addChild(pic2Right);
            curHeight += pic2Left->getContentSize().height;
        }
        auto pic1Left = CCLoadSprite::createSprite("BG_equip_top.png");
        curHeight = CCDirector::sharedDirector()->getWinSize().height - 86 - pic1Left->getContentSize().height;
    if (CCCommonUtils::isIosAndroidPad()) {
        curHeight = 852 - 60 - pic1Left->getContentSize().height;
    }
        pic1Left->setPosition(ccp(0, curHeight));
        pic1Left->cocos2d::CCNode::setAnchorPoint(ccp(0, 0));
        tBatchNode->addChild(pic1Left);
        auto pic1Right = CCLoadSprite::createSprite("BG_equip_top.png");
        pic1Right->setScaleX(-1);
        pic1Right->setPosition(ccp(317, curHeight));
        pic1Right->setAnchorPoint(ccp(1, 0));
        tBatchNode->addChild(pic1Right);
        tBatchNode->setPosition(0, 0);
        m_bgNode->addChild(tBatchNode);
//    }
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
    miny = 0;
    offsetPt = ccp(0,0);
    
    setTitleName(_lang("119008"));
    
    
//    m_data = EquipmentController::getInstance()->SelEquipmentsByType(m_weapType, -1);
//    m_weapType = -1;
//    m_mateType = -1;

    return true;
}

void EquipmentListView::onEnter(){
    CCNode::onEnter();
    UIComponent::getInstance()->showEquipOrBagBtn(1);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentListView::refreshData), EQUIP_SEL_DATA_REFRESH, NULL);
    EquipmentController::getInstance()->CheckSelEquipMate(m_weapType, -1);
    m_isForgeWork = isForgeWork();
    m_data = SelEquipmentsByPlayerLv();
    
    string curGuideId = GuideController::share()->getCurGuideID();
    if (curGuideId == "3110500" || curGuideId == "3110600") {
        bool rtST = false;
        vector<int>::iterator it = m_data.begin();
        for (int i=0; i<m_data.size(); i++)
        {
            if (m_data[i] == 1001015) {
                m_data.erase(it);
                rtST = true;
                break;
            }
            it++;
        }
        if (rtST) {
            m_data.insert(m_data.begin(), 1001015);
        }
    }
    generateData();
}

void EquipmentListView::onExit(){
    offsetPt  = m_tabView->getContentOffset();
    UIComponent::getInstance()->showEquipOrBagBtn(0);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_SEL_DATA_REFRESH);
    CCNode::onExit();
}

CCNode* EquipmentListView::getGuideNode(string _key)
{
    if (_key == "FR2_1")
    {
        EquipmentListCell* cell = dynamic_cast<EquipmentListCell*>(m_tabView->cellAtIndex(0));
        if (cell && cell->m_forgeBtn->isVisible() && cell->m_itemId == 1001015) {
            return cell->m_forgeBtn;
        }
        return NULL;
    }
    return NULL;
}

bool EquipmentListView::isForgeWork()
{
    bool ret = false;
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
    if (tqid != QID_MAX)
        ret = true;
    return ret;
}

bool EquipmentListView::antiCompareEqui(int item1, int item2)
{
    auto& info1 = EquipmentController::getInstance()->EquipmentInfoMap[item1];
    auto& info2 = EquipmentController::getInstance()->EquipmentInfoMap[item2];
    if (info1.level < info2.level) {
        return true;
    }
    else if (info1.level > info2.level) {
        return false;
    }
    else {
        if (info1.site < info2.site) {
            return true;
        }
        else {
            return false;
        }
    }
}

vector<int> EquipmentListView::SelEquipmentsByPlayerLv()
{
    vector<int> retVec;
    vector<int> tmp3;
    vector<int> tmp2;
    vector<int> tmp1 = EquipmentController::getInstance()->SelEquipmentsByType(m_weapType, -1);
    int maxLv = 9999;
    bool isInGuide = GuideController::share()->isInTutorial();
    for (int i = 0; i < tmp1.size(); ++i) {
        auto& info = EquipmentController::getInstance()->EquipmentInfoMap[tmp1[i]];
        if (info.showType != "no") {
            
            if (info.level <= GlobalData::shared()->playerInfo.level) {
                retVec.push_back(tmp1[i]);
            }
            else {
                if (!isInGuide) {
                    if (maxLv > info.level) {
                        retVec.clear();
                        maxLv = info.level;
                        retVec.push_back(tmp1[i]);
                    }else {
                        retVec.push_back(tmp1[i]);
                    }
                }
            }
//            if (info.canCrt) {
//                tmp3.push_back(tmp1[i]);
//            } else {
//                tmp2.push_back(tmp1[i]);
//            }
        }
    }
    
    return retVec;
    
//    sort(tmp2.begin(), tmp2.end(), antiCompareEqui);
    for (int i = 0; i < tmp3.size(); ++i) {
        retVec.push_back(tmp3[i]);
    }
    for (int i = 0; i < tmp2.size(); ++i) {
        retVec.push_back(tmp2[i]);
    }
    return retVec;
}

bool EquipmentListView::isLevelFirst(int idx)
{
    bool st = false;
    if(m_data.size()) {
        if (idx == 0) {
            st = true;
        } else {
            auto& info1 = EquipmentController::getInstance()->EquipmentInfoMap[m_data[idx - 1]];
            auto& info2 = EquipmentController::getInstance()->EquipmentInfoMap[m_data[idx]];
            if (info1.level != info2.level) {
                st = true;
            } else {
                st = false;
            }
        }
    }
    return st;
}

bool EquipmentListView::isInForging(int idx)
{
//    return true;
    bool st = false;
    if (m_data.size()) {
        int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
        if (tqid != QID_MAX) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
            int tItemId = QueueController::getInstance()->getItemId(qInfo.key);
            int mItemId = m_data[idx];
            if (tItemId / 10 == mItemId / 10) {
                st = true;
            }
        }
    }
    return st;
}

void EquipmentListView::refreshData(CCObject* p)
{
    CCDictionary* dict = dynamic_cast<CCDictionary*>(p);
    if (dict) {
        if (dict->objectForKey("weapId")) {
            m_weapType = dict->valueForKey("weapId")->intValue();
        }
        if (dict->objectForKey("mateId")) {
            m_mateType = dict->valueForKey("mateId")->intValue();
        }
    }
    
    int weapId = -1;
    int mateId = -1;
    switch (m_weapType) {
        case 1://武器
            weapId = 0;
            break;
        case 2://头
            weapId = 1;
            break;
        case 3://衣服
            weapId = 2;
            break;
        case 4://裤子
            weapId = 3;
            break;
        case 5://鞋子
            weapId = 4;
            break;
        case 6://戒指
            weapId = 5;
            break;
        default:
            break;
    }
    
    switch (m_mateType) {
        case 1://枫木
            mateId = 201010;
            break;
        case 2://水银
            mateId = 201020;
            break;
        case 3://骨头
            mateId = 201030;
            break;
        case 4://布料
            mateId = 201040;
            break;
        case 5://硫磺
            mateId = 201050;
            break;
        case 6://青铜
            mateId = 201060;
            break;
        case 7://琥珀
            mateId = 201070;
            break;
        case 8://兽角
            mateId = 201080;
            break;
        case 9://原油
            mateId = 201090;
            break;
        default:
            break;
    }
    m_data = EquipmentController::getInstance()->SelEquipmentsByType(weapId, mateId);
    generateData();
}

void EquipmentListView::generateData()
{
    m_tabView->reloadData();
    float mincurry = m_tabView->minContainerOffset().y;
    if(miny != 0) {
        offsetPt.y+=(-miny+mincurry);
        m_tabView->setContentOffset(offsetPt);
    }
    miny = mincurry;
}

SEL_CCControlHandler EquipmentListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool EquipmentListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    return false;
}

CCSize EquipmentListView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    float cellHeight = calListCellHeight(table,idx);
    
    return CCSizeMake(620, cellHeight);
}

bool canBeSupportedWithBmpFont(std::string language)
{
    int length = language.length();
    
    if(language.find("zh_TW") < length ||
       language.find("zh_CN") < length ||
       language.find("zh-Hans") < length ||
       language.find("ko")< length ||
       language.find("ja")< length ||
       language.find("ru")< length ||
       language.find("th")< length ||
       language.find("pt")< length ||
       language.find("ar")< length ||
       language.find("nl")< length ||
       language.find("fr")< length ||
       language.find("it")< length ||
       language.find("pl")< length ||
       language.find("tr")< length){
        return false;
    }
    else
    {
        return true;
    }
}

float EquipmentListView::calListCellHeight(CCTableView *table,int idx)
{
    int itemId = m_data[idx];
    bool isLvFirst = false;
    bool isInForg = isInForging(idx);
    float cellHeight = 0.0f;
//    EquipmentListCell* cell = EquipmentListCell::create(itemId, isLvFirst, isInForg, m_isForgeWork, &cellHeight);
//    
//    return cellHeight;
    
    auto& info = EquipmentController::getInstance()->EquipmentInfoMap[itemId];
    
    float posY = 0.0f;
    if(isInForg || info.canCrt)
    {
        posY = -60.f;
    }
    else
    {
        posY = -93.f;
    }
    
    float offSetY = 8.0f;
    float offSetY2 = 0.0f;
    float singleLineHeight = 0.0f;
    std::string language = LocalController::shared()->getLanguageFileName();
    if (!canBeSupportedWithBmpFont(language))
    {
        offSetY = 8.0f;
    }
    else if(language == "de" || language == "es" || language == "id")
    {
        offSetY = -15.0f;
    }
    else
    {
        offSetY = -15.0f;
        offSetY2 = -5.0f;
        singleLineHeight = (CCLabelIF::create(_lang("101205").c_str(), 22))->getContentSize().height;
    }
    
    string tmpInfo = "";
    CCLabelIF* tmpLb = nullptr;
    for (int i=1; i <= 4; i++) {
        if (info.showDias.find(i) != info.showDias.end()) {
            string dialog = info.showDias[i];
            if (dialog != "" && info.values.find(i) != info.values.end()) {
                tmpInfo = _lang(dialog)+" +"+CC_ITOA_MONEY(info.values[i])+"%";
                tmpLb = CCLabelIF::create(tmpInfo.c_str(), 22);
                tmpLb->setDimensions(CCSize(280, 0));
                tmpLb->setMaxScaleXByWidth(450);
                // 多行
                float tmpLbHeight = tmpLb->getContentSize().height;
                if(singleLineHeight>0.0f && tmpLbHeight > singleLineHeight)
                {
                    posY -= tmpLbHeight + offSetY;
                }
                else
                {
                    posY -= tmpLbHeight + offSetY2;
                }
            }
        }
    }
    
    if (isInForg) {
        cellHeight = -posY+70;
    }
    else
    {
        cellHeight = -posY+15;
    }
    
//    int dd = cellHeight;
//    CCLOG("cell height fubin %d", dd);
    
    return cellHeight;
}

CCSize EquipmentListView::cellSizeForTable(CCTableView *table)
{
    return CCSize(620, 280);
}

CCTableViewCell* EquipmentListView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if (idx >= m_data.size()) {
        return NULL;
    }
    
    bool isLvFirst = false;
    bool isInForg = isInForging(idx);
    int itemId = m_data[idx];
    
    EquipmentListCell* cell = (EquipmentListCell*)table->dequeueCell();
    if (cell) {
        cell->setData(itemId, isLvFirst, isInForg, m_isForgeWork);
    } else {
        cell = EquipmentListCell::create(itemId, isLvFirst, isInForg, m_isForgeWork);
    }
    
    return cell;
}

ssize_t EquipmentListView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data.size();
}

//class cell
EquipmentListCell* EquipmentListCell::create(int itemIds, bool isLvFirst, bool isInForging, bool isForgeWork)
{
    auto ret = new EquipmentListCell();
    if (ret && ret->init(itemIds, isLvFirst, isInForging, isForgeWork)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentListCell::init(int itemIds, bool isLvFirst, bool isInForging, bool isForgeWork)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(306, true);
    CCBLoadFile("EquipmentListCell",this,this);
    setContentSize(CCSize(620, 280));
    setData(itemIds, isLvFirst, isInForging, isForgeWork);
    return ret;
}

void EquipmentListCell::setData(int itemId, bool isLvFirst, bool isInForging, bool isForgeWork)
{
    m_itemId = itemId;
    m_isLvFirst = isLvFirst;
    m_isInForging = isInForging;
    m_isForgeWork = isForgeWork;
    m_picNode->removeAllChildren();
    m_nameLabel->setString("");
    m_levelLabel->setString("");
    m_infoLabel1->setString("");
    m_infoLabel2->setString("");
    m_infoLabel3->setString("");
    m_infoLabel4->setString("");

    setLabelTag();
    
    auto& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (!m_isInForging && info.canCrt)
    {
        m_forgeBtn->setVisible(true);
        m_viewBtn->setVisible(false);
        CCCommonUtils::setButtonTitle(m_forgeBtn, _lang("119006").c_str());
    }
    else
    {
        m_viewBtn->setVisible(true);
        m_forgeBtn->setVisible(false);
        CCCommonUtils::setButtonTitle(m_viewBtn, _lang("119011").c_str());
    }
    
    // 只有不在锻造状态，并且材料不足时，才显示“材料不足”的提示
    if(m_isInForging)
    {
        m_noEnoughMaterialLb->setVisible(false);
    }
    else
    {
        if(info.canCrt)
        {
            m_noEnoughMaterialLb->setVisible(false);
        }
        else
        {
            m_noEnoughMaterialLb->setVisible(true);
            m_noEnoughMaterialLb->setString(_lang("119077"));
            m_noEnoughMaterialLb->setMaxScaleXByWidth(450.0);
            m_noEnoughMaterialLb->setColor(ccc3(120,24,1));
        }
    }
    
    m_nameLabel->setString(_lang(info.name));
    string s("Lv: ");
    s += CC_ITOA(info.level);
    m_levelLabel->setString(s);
    m_levelLabel->setPositionX(m_nameLabel->getPositionX() + m_nameLabel->getContentSize().width * m_nameLabel->getOriginScaleX() + 20);
    string picStr = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    auto icon = CCLoadSprite::createSprite(picStr.c_str(), true,CCLoadSpriteType_EQUIP);
    CCCommonUtils::setSpriteMaxSize(icon, 100, true);
    m_picNode->addChild(icon);
    
    float posY = 0.0f;
    if(m_noEnoughMaterialLb->isVisible())
    {
        posY = -93.f;
    }
    else
    {
        posY = -60.f;
    }
    
    float offSetY = 8.0f;
    float offSetY2 = 0.0f;
    float singleLineHeight = 0.0f;
    std::string language = LocalController::shared()->getLanguageFileName();
    if (!m_nameLabel->canBeSupportedWithBmpFont(language))
    {
        offSetY = 8.0f;
    }
    else if(language == "de" || language == "es" || language == "id")
    {
        offSetY = -15.0f;
    }
    else
    {
        offSetY = -15.0f;
        offSetY2 = -5.0f;

        singleLineHeight = (CCLabelIF::create(_lang("101205").c_str(), 22))->getContentSize().height;
    }
    
    string tmpInfo = "";
    CCLabelIF* tmpLb = nullptr;
    for (int i=1; i <= 4; i++) {
        if (info.showDias.find(i) != info.showDias.end()) {
            string dialog = info.showDias[i];
            if (dialog != "" && info.values.find(i) != info.values.end()) {
                tmpInfo = _lang(dialog)+" +"+CC_ITOA_MONEY(info.values[i])+"%";
                tmpLb = (CCLabelIF*)(m_mainNode->getChildByTag(i));
                tmpLb->setString(tmpInfo);
                tmpLb->setMaxScaleXByWidth(450);
                tmpLb->setPositionY(posY);
                // 多行
                if(singleLineHeight>0.0f && tmpLb->getContentSize().height > singleLineHeight)
                {
                    posY -= tmpLb->getContentSize().height + offSetY;
                }
                else
                {
                    posY -= tmpLb->getContentSize().height + offSetY2;
                }
            }
        }
    }
    
    if (m_isInForging) {
        m_proNode->setVisible(true);
        m_mainNode->setPositionY(-posY+70);
        
        auto pos = m_progrossBar->getPosition();
        m_barClipNode = CCClipNode::create(m_progrossBar->getContentSize().width, m_progrossBar->getContentSize().height);
        m_progrossBar->getParent()->addChild(m_barClipNode);
        m_barClipNode->setAnchorPoint(ccp(0, 0));
        m_progrossBar->retain();
        m_progrossBar->removeFromParent();
        m_barClipNode->addChild(m_progrossBar);
        m_progrossBar->setPosition(0, 0);
        CC_SAFE_RELEASE_NULL(m_progrossBar);
        m_barClipNode->setPosition(pos);
        update(1.0f);
    }
    else
    {
        m_proNode->setVisible(false);
        m_mainNode->setPositionY(-posY+15);
    }
    m_picNode->setPositionY(-m_mainNode->getPositionY()/2);
    
//    int dd = m_mainNode->getPositionY();
//    CCLOG("cell height fubin ***** %d", dd);
}

void EquipmentListCell::setLabelTag()
{
    m_infoLabel1->setTag(1);
    m_infoLabel2->setTag(2);
    m_infoLabel3->setTag(3);
    m_infoLabel4->setTag(4);
}

void EquipmentListCell::onEnter() {
    CCNode::onEnter();
    if (m_isInForging) {
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    }
}

void EquipmentListCell::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCNode::onExit();
}

void EquipmentListCell::refreashData(CCObject* obj)
{
}

void EquipmentListCell::update(float time)
{
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
    auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
    int tItemId = QueueController::getInstance()->getItemId(qInfo.key);
    int mItemId = m_itemId;
    if (tItemId / 10 != mItemId / 10) {
        m_proNode->setVisible(false);
        CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
        return;
    }
    int tmpCurTime = GlobalData::shared()->getWorldTime();
    if (qInfo.startTime == 0) {
        qInfo.startTime = tmpCurTime;
    }
    int sumTime = qInfo.finishTime - qInfo.startTime;
    int leftTime = qInfo.finishTime - tmpCurTime;
    leftTime = MAX(0, leftTime);
    sumTime = MAX(1, sumTime);
    if (qInfo.finishTime <= 0) {
        leftTime = 0;
        sumTime = 1;
    }
    float pro = leftTime *1.0 / sumTime;
    pro = pro > 1? 1 : pro;
//    m_progrossBar->setScaleX(1 - pro);
    m_timeText->setString(CC_SECTOA(leftTime));
    
    int totalW = 262;
    float w = totalW - pro * totalW;
    m_barClipNode->setContentSize(CCSize(w, m_barClipNode->getContentSize().height));
    if (leftTime <= 0) {
        m_proNode->setVisible(false);
    }
}

void EquipmentListCell::onForgeBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupInView(EquipmentCreateView::create(m_itemId));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("FR2_1"));
}

void EquipmentListCell::onViewBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupInView(EquipmentCreateView::create(m_itemId));
}

SEL_CCControlHandler EquipmentListCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onForgeBtnClick", EquipmentListCell::onForgeBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onViewBtnClick", EquipmentListCell::onViewBtnClick);
    return NULL;
}

bool EquipmentListCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelLabel", CCLabelIF*, m_levelLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel1", CCLabelIF*, m_infoLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel2", CCLabelIF*, m_infoLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel3", CCLabelIF*, m_infoLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel4", CCLabelIF*, m_infoLabel4);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headLvLabel", CCLabelIF*, m_headLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topSpr", CCSprite*, m_topSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomSpr", CCSprite*, m_bottomSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgSpr", CCScale9Sprite*, m_bgSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgSpr2", CCScale9Sprite*, m_bgSpr2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headLvBg", CCSprite*, m_headLvBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode2", CCNode*, m_mainNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forgeBtn", CCControlButton*, m_forgeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBtn", CCControlButton*, m_viewBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_proNode", CCNode*, m_proNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressBG", CCScale9Sprite*, m_progressBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progrossBar", CCScale9Sprite*, m_progrossBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noEnoughMaterialLb", CCLabelIF*, m_noEnoughMaterialLb);
    return false;
}

/////
SelTipView* SelTipView::create(int weapType, int mateType) {
    auto ret = new SelTipView();
    if (ret && ret->init(weapType, mateType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SelTipView::init(int weapType, int mateType) {
    bool ret = false;
    if (PopupBaseView::init()) {
        m_weapType = weapType;
        m_mateType = mateType;
        
        CCBLoadFile("SelTipView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        m_titleLabel->setString(_lang("101315"));
        
        if (m_weapType!=-1) {
            m_weapOk->setPosition(m_weapNodes[m_weapType+1]->getPosition());
        }
        else {
            m_weapOk->setPosition(m_weapNodes[m_weapNodes.size()]->getPosition());
        }
        
        if (m_mateType!=-1) {
            m_mateOK->setPosition(m_mateNodes[m_mateType+1]->getPosition());
        }
        else {
            m_mateOK->setPosition(m_mateNodes[m_mateNodes.size()]->getPosition());
        }
        
        ret = true;
    }
    return ret;
}

void SelTipView::onEnter() {
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void SelTipView::onExit() {
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

SEL_CCControlHandler SelTipView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOKBtn", SelTipView::onClickOKBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCancelBtn", SelTipView::onClickCancelBtn);
    return NULL;
}

bool SelTipView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_weapNode",10) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 10, strlen(pMemberVariableName) - 10);
        int idx = atoi(index);
        m_weapNodes[idx] = pNode;
        return true;
    }
    if (pTarget == this && strncmp(pMemberVariableName, "m_mateNode",10) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 10, strlen(pMemberVariableName) - 10);
        int idx = atoi(index);
        m_mateNodes[idx] = pNode;
        return true;
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn", CCControlButton*, m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weapOk", CCSprite*, m_weapOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateOK", CCSprite*, m_mateOK);
    return false;
}

void SelTipView::onClickOKBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    auto dict = CCDictionary::create();
    dict->setObject(CCString::create(CC_ITOA(m_weapType)), "weapId");
    dict->setObject(CCString::create(CC_ITOA(m_mateType)), "mateId");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_SEL_DATA_REFRESH,dict);
    PopupViewController::getInstance()->removePopupView(this);
}

void SelTipView::onClickCancelBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

bool SelTipView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void SelTipView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    map<int, CCNode*>::iterator it = m_weapNodes.begin();
    for (; it!=m_weapNodes.end(); it++) {
        if (isTouchInside(it->second, pTouch)) {
            m_weapType = it->first;
            m_weapOk->setPosition(it->second->getPosition());
            return;
        }
    }
    
    map<int, CCNode*>::iterator mait = m_mateNodes.begin();
    for (; mait!=m_mateNodes.end(); mait++) {
        if (isTouchInside(mait->second, pTouch)) {
            m_mateType = mait->first;
            m_mateOK->setPosition(mait->second->getPosition());
            return;
        }
    }
}