//
//  ChestUseView.cpp
//  LastEmpire
//
//  Created by zhangjun on 2015-03-24 17:28:19.
//
//

#include "ChestUseView.h"
#include "PopupViewController.h"
#include "ChestUseViewCell.h"
#include "RewardController.h"
#include "ActivityController.h"

ChestUseView::~ChestUseView()
{
    CCLOGFUNC();
}
ChestUseView* ChestUseView::create(CCDictionary *dict)
{
    ChestUseView *pRet = new ChestUseView(dict);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}
bool ChestUseView::init()
{
    if (!PopupBaseView::init()) {
        CCLOG("!PopupBaseView::init()");
        return false;
    }
    setIsHDPanel(true);
    bool bRet = true;
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    /*Load CCB*/
    CCBLoadFile("ChestUseView.ccbi", this, this);
    
    if (m_dataDict&&m_dataDict->objectForKey("itemEffectObj")) {
        auto effectObj = _dict(m_dataDict->objectForKey("itemEffectObj"));
        m_dataArr = dynamic_cast<CCArray*>(effectObj->objectForKey("reward"));
        m_saveDataArr = CCArray::create();
        m_saveDataArr->retain();
        m_dataDict->retain();
    }
    if (m_dataArr) {
        //吧经验放到最前 防止使用itemId:202006图片乱位
        for (int i=0; i<m_dataArr->count(); i++) {
            CCDictionary *rewardDict=_dict(m_dataArr->objectAtIndex(i));
            int type = rewardDict->valueForKey("type")->intValue();
            if (type==6) {
                m_dataArr->removeObjectAtIndex(i);
                m_dataArr->insertObject(rewardDict, 0);
                break;
            }
        }
        //合并装备
        m_eqNumDict=CCDictionary::create();
        int num=1;
        for (int i=0; i<m_dataArr->count(); i++) {
            CCDictionary *rewardDict=_dict(m_dataArr->objectAtIndex(i));
            CCDictionary *valueDict=_dict(rewardDict->objectForKey("value"));
            int itemId=valueDict->valueForKey("itemId")->intValue();
            for (int j=i+1; j<m_dataArr->count(); j++) {
                CCDictionary *rewardDict=_dict(m_dataArr->objectAtIndex(j));
                CCDictionary *valueDict=_dict(rewardDict->objectForKey("value"));
                int tempId=valueDict->valueForKey("itemId")->intValue();
                if (itemId==tempId&&itemId!=0) {
                    m_saveDataArr->addObject(m_dataArr->objectAtIndex(j));
                    m_dataArr->removeObjectAtIndex(j--);
                    num++;
                }
            }
            m_eqNumDict->setObject(CCInteger::create(num), CC_ITOA(itemId));
        }
        m_eqNumDict->retain();
        //合并装备
        int arrCount = m_dataArr->count();
        switch (arrCount) {
            case 1:
                m_bgNode->getChildByTag(2)->setVisible(false);
                m_bgNode->getChildByTag(3)->setVisible(false);
                m_bgNode->getChildByTag(4)->setVisible(false);
                m_bgNode->getChildByTag(5)->setVisible(false);
                m_bgNode->getChildByTag(6)->setPositionY(m_bgNode->getChildByTag(5)->getPositionY()+245);
                m_tableNode->setContentSize(CCSize(500, 56));
                m_sprBG->setContentSize(CCSize(612, 157));
//                m_titleLabel->setPositionY(m_titleLabel->getPositionY()-80);
                break;
            case 2:
                m_bgNode->getChildByTag(2)->setVisible(false);
                m_bgNode->getChildByTag(3)->setPositionY(m_bgNode->getChildByTag(3)->getPositionY()+35);
                m_bgNode->getChildByTag(4)->setVisible(false);
                m_bgNode->getChildByTag(5)->setVisible(false);
                m_bgNode->getChildByTag(6)->setPositionY(m_bgNode->getChildByTag(5)->getPositionY()+175);
                m_tableNode->setContentSize(CCSize(500, 112));
                m_sprBG->setContentSize(CCSize(612, 227));
//                m_titleLabel->setPositionY(m_titleLabel->getPositionY()-60);
                break;
            case 3:
                m_bgNode->getChildByTag(4)->setVisible(false);
                m_bgNode->getChildByTag(5)->setVisible(false);
                m_bgNode->getChildByTag(6)->setPositionY(m_bgNode->getChildByTag(5)->getPositionY()+140);
                m_tableNode->setContentSize(CCSize(500, 168));
                m_sprBG->setContentSize(CCSize(612, 262));
                m_titleLabel->setPositionY(m_titleLabel->getPositionY()+5);
                m_tableNode->setPositionY(m_tableNode->getPositionY()+10);
                break;
            case 4:
                m_bgNode->getChildByTag(5)->setVisible(false);
                m_bgNode->getChildByTag(6)->setPositionY(m_bgNode->getChildByTag(5)->getPositionY()+70);
                m_tableNode->setContentSize(CCSize(500, 224));
                m_sprBG->setContentSize(CCSize(612, 332));
//                m_titleLabel->setPositionY(m_titleLabel->getPositionY()-20);
                break;
                
            default:
                break;
        }
    }
    
    m_titleLabel->setString(_lang("107502").c_str());
    m_tabView = CCTableView::create(this, m_tableNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setDelegate(this);
    m_tableNode->addChild(m_tabView);
    
    return bRet;
}
void ChestUseView::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);//Touch_Default
}
void ChestUseView::onExit()
{
    if(m_dataArr){
        if (m_saveDataArr&&m_saveDataArr->count()>0) {
            m_dataArr->addObjectsFromArray(m_saveDataArr);
        }
        std::string rewardStr = RewardController::getInstance()->retReward(m_dataArr);//同步数据
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
        CC_SAFE_RELEASE(m_dataDict);
        CC_SAFE_RELEASE(m_eqNumDict);
    }
    setTouchEnabled(false);
//    m_dataArr->release();
    CCNode::onExit();
}

SEL_CCControlHandler ChestUseView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_MenuHandler ChestUseView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CallFuncN ChestUseView::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool ChestUseView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCNode*,this->m_sprBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*,this->m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tableNode", CCNode*,this->m_tableNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*,this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*,this->m_bgNode);
    return false;
}
//ccTouch
bool ChestUseView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_sprBG, pTouch)) {
        return false;
    }
    PopupViewController::getInstance()->removePopupView(this);
    return true;
}

void ChestUseView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

#pragma mark tabelView
void ChestUseView::scrollViewDidScroll(CCScrollView* view)
{
//    float offsetY = m_tabView->getContentOffset().y;
}
void ChestUseView::tableCellTouched(cocos2d::extension::TableView* table, CCTableViewCell* cell){
    CCLOGFUNC();
}
CCSize ChestUseView::cellSizeForTable(CCTableView *table)
{
    return CCSize(500, 56);
}

CCTableViewCell* ChestUseView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    CCLOGFUNC();
    ChestUseViewCell* cell = (ChestUseViewCell*)table->dequeueCell();
    if(!cell){
        cell = ChestUseViewCell::create();
    }
//    m_dataDict->writeToFile("/Users/zhangjun/Desktop/dictf/dt");
    if(m_dataArr&&m_dataArr->objectAtIndex(idx)){
        std::string nameStr = RewardController::getInstance()->getRewardNameInArrayByIndex(m_dataArr, idx);
        int countStr = RewardController::getInstance()->getRewardCountInArrayByIndex(m_dataArr, idx);
        
        CCDictionary *rewardDict=_dict(m_dataArr->objectAtIndex(idx));
        CCDictionary *valueDict=_dict(rewardDict->objectForKey("value"));
        int itemId=valueDict->valueForKey("itemId")->intValue();
        int itemType=rewardDict->valueForKey("type")->intValue();
        int rewardAdd=valueDict->valueForKey("rewardAdd")->intValue();
        
        string idStr = valueDict->valueForKey("itemId")->getCString();
        string nameId = CCCommonUtils::getPropById(idStr, "name");
        if(idStr=="200705" || idStr =="200719"){
            string para2 = valueDict->valueForKey("para2")->getCString();
            nameStr = _lang_1(nameId.c_str(), para2.c_str());
        }
        
        if (itemType==14) {//针对装备得到数量
//            countStr=m_eqNumDict->valueForKey(CC_ITOA(itemId))->intValue();
            CCInteger* pInteger1 = (CCInteger*)m_eqNumDict->objectForKey(CC_ITOA(itemId));
            if (pInteger1) {
                countStr=pInteger1->getValue();
            }
        }

        cell->setData(itemId,nameStr,countStr,itemType);
    }
    return cell;
}
ssize_t ChestUseView::numberOfCellsInTableView(CCTableView *table){
    if (m_dataArr) {
        int count = m_dataArr->count();
//        CCLOG("count:%d",count);
        return count;
    }
    return 1;
}
CCSize ChestUseView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    return CCSize(500, 56);
}
#pragma mark tabelView-end

///ChestRDView
ChestRDView::~ChestRDView()
{
    CCLOGFUNC();
}
ChestRDView* ChestRDView::create(CCDictionary *dict)
{
    ChestRDView *pRet = new ChestRDView(dict);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}
bool ChestRDView::init()
{
    if (!PopupBaseView::init()) {
        CCLOG("!PopupBaseView::init()");
        return false;
    }
    bool bRet = true;
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    setIsHDPanel(true);
    /*Load CCB*/
    CCBLoadFile("ChestUseView.ccbi", this, this);
    
    int day = 0;
    if (m_dataDict&&m_dataDict->objectForKey("itemEffectObj")) {
        day = m_dataDict->valueForKey("day")->intValue();
        auto effectObj = _dict(m_dataDict->objectForKey("itemEffectObj"));
        m_dataArr = dynamic_cast<CCArray*>(effectObj->objectForKey("reward"));
        m_saveDataArr = CCArray::create();
        m_saveDataArr->retain();
        m_dataDict->retain();
    }
    if (m_dataArr) {
        int arrCount = m_dataArr->count();
        switch (arrCount) {
            case 1:
                m_bgNode->getChildByTag(2)->setVisible(false);
                m_bgNode->getChildByTag(3)->setVisible(false);
                m_bgNode->getChildByTag(4)->setVisible(false);
                m_bgNode->getChildByTag(5)->setVisible(false);
                m_bgNode->getChildByTag(6)->setPositionY(m_bgNode->getChildByTag(5)->getPositionY()+245);
                m_tableNode->setContentSize(CCSize(500, 56));
//                if (CCCommonUtils::isIosAndroidPad())
//                {
//                    m_tableNode->setContentSize(CCSize(900, 101.8));
//                }
                m_sprBG->setContentSize(CCSize(612, 157));
                break;
            case 2:
                m_bgNode->getChildByTag(2)->setVisible(false);
                m_bgNode->getChildByTag(3)->setPositionY(m_bgNode->getChildByTag(3)->getPositionY()+35);
                m_bgNode->getChildByTag(4)->setVisible(false);
                m_bgNode->getChildByTag(5)->setVisible(false);
                m_bgNode->getChildByTag(6)->setPositionY(m_bgNode->getChildByTag(5)->getPositionY()+175);
                m_tableNode->setContentSize(CCSize(500, 112));
//                if (CCCommonUtils::isIosAndroidPad())
//                {
//                    m_tableNode->setContentSize(CCSize(900, 101.8*2));
//                }
                m_sprBG->setContentSize(CCSize(612, 227));
                break;
            case 3:
                m_bgNode->getChildByTag(4)->setVisible(false);
                m_bgNode->getChildByTag(5)->setVisible(false);
                m_bgNode->getChildByTag(6)->setPositionY(m_bgNode->getChildByTag(5)->getPositionY()+140);
                m_tableNode->setContentSize(CCSize(500, 168));
                m_sprBG->setContentSize(CCSize(612, 262));
                m_titleLabel->setPositionY(m_titleLabel->getPositionY()+5);
                m_tableNode->setPositionY(m_tableNode->getPositionY()+10);
//                if (CCCommonUtils::isIosAndroidPad())
//                {
//                    m_tableNode->setContentSize(CCSize(900, 101.8*3));
//                }
                break;
            case 4:
                m_bgNode->getChildByTag(5)->setVisible(false);
                m_bgNode->getChildByTag(6)->setPositionY(m_bgNode->getChildByTag(5)->getPositionY()+70);
                m_tableNode->setContentSize(CCSize(500, 224));
                m_sprBG->setContentSize(CCSize(612, 332));
//                if (CCCommonUtils::isIosAndroidPad())
//                {
//                    m_tableNode->setContentSize(CCSize(900, 101.8*4));
//                }
                break;
            case 5:
                m_bgNode->getChildByTag(6)->setPositionY(m_bgNode->getChildByTag(5)->getPositionY());
                m_tableNode->setContentSize(CCSize(500, 280));
                m_sprBG->setContentSize(CCSize(612, 402));
//                if (CCCommonUtils::isIosAndroidPad())
//                {
//                    m_tableNode->setContentSize(CCSize(900, 101.8*5));
//                }
                break;
                
            default:
                break;
        }
    }
    
    if (m_subNode) {
        int num = m_dataArr->count();
        int cellH = 70;
        m_subNode->setPositionY(m_subNode->getPositionY()-(5-num)*cellH/2);
        m_bgNode->setPositionY(m_bgNode->getPositionY()-(5-num)*cellH/2);
        m_sprBG->setPositionY(m_sprBG->getPositionY()-(5-num)*cellH/2);
    }
    
    m_titleLabel->setString(_lang("101303").c_str());
    if (day <= PortActController::getInstance()->m_newLoginDay) {
        m_titleLabel->setString(_lang("115216").c_str());
    }
    if (day == 99999) {
        m_titleLabel->setString(_lang("106051").c_str());
    }
    
    m_tabView = CCTableView::create(this, m_tableNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setDelegate(this);
    m_tableNode->addChild(m_tabView);
    
    return bRet;
}
void ChestRDView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);//Touch_Default
}
void ChestRDView::onExit()
{
   // CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

SEL_CCControlHandler ChestRDView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_MenuHandler ChestRDView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CallFuncN ChestRDView::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool ChestRDView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCNode*,this->m_sprBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*,this->m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tableNode", CCNode*,this->m_tableNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*,this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*,this->m_bgNode);
    return false;
}
//ccTouch
bool ChestRDView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_sprBG, pTouch)) {
        return false;
    }
    PopupViewController::getInstance()->removePopupView(this);
    return true;
}

void ChestRDView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

#pragma mark tabelView
void ChestRDView::scrollViewDidScroll(CCScrollView* view)
{
}
void ChestRDView::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    CCLOGFUNC();
}
CCSize ChestRDView::cellSizeForTable(CCTableView *table)
{
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        return CCSize(900, 101.8);
//    }
    return CCSize(500, 56);
}

CCTableViewCell* ChestRDView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    CCLOGFUNC();
    ChestUseViewCell* cell = (ChestUseViewCell*)table->dequeueCell();
    if(!cell){
        cell = ChestUseViewCell::create();
    }
    if(m_dataArr&&m_dataArr->objectAtIndex(idx)){
        CCDictionary *rewardDict=_dict(m_dataArr->objectAtIndex(idx));
        CCDictionary *valueDict=_dict(rewardDict->objectForKey("value"));
        int itemId=valueDict->valueForKey("id")->intValue();
        int itemType=rewardDict->valueForKey("type")->intValue();
        int rewardAdd=valueDict->valueForKey("num")->intValue();
        std::string nameStr = RewardController::getInstance()->getNameByType(itemType, itemId);
        cell->setData(itemId,nameStr,rewardAdd,itemType);
    }
    return cell;
}
ssize_t ChestRDView::numberOfCellsInTableView(CCTableView *table){
    if (m_dataArr) {
        int count = m_dataArr->count();
        return count;
    }
    return 1;
}
CCSize ChestRDView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(900, 101.8);
    }
    return CCSize(500, 56);
}
#pragma mark tabelView-end