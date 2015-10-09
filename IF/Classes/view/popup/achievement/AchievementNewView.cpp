//
//  AchievementNewView.cpp
//  IF
//
//  Created by chenfubi on 15/4/3.
//
//

#include "AchievementNewView.h"
#include "AchievementController.h"
#include "UIComponent.h"
#include "ActivityController.h"
#include "PopupViewController.h"

AchievementNewView* AchievementNewView::create(int openType/*=0*/)
{
	AchievementNewView* ret = new AchievementNewView();
    if (ret && ret->init(openType))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

AchievementNewView::~AchievementNewView(){
    m_medalNodeView->removeAllChildren();
    m_achieveNodeView->removeAllChildren();
};

bool AchievementNewView::init(int openType)
{
	bool bRet = false;
	do
	{
		if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
        auto ccb = CCBLoadFile("AchievementNewView", this, this);
        this->setContentSize(ccb->getContentSize());
        int extH = getExtendHeight();
        
        auto tbg = CCLoadSprite::loadResource("technology_09.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
        int width = CCDirector::sharedDirector()->getWinSize().width;
        int curHight = -maxHight/2.0f;
        while (curHight<maxHight) {
            auto bg = CCLoadSprite::createSprite("technology_09.png");
            bg->setAnchorPoint(ccp(0, 1));
            bg->setPosition(ccp(-width/2.0f, curHight));
            if (CCCommonUtils::isIosAndroidPad())
            {
                bg->setScaleX(1536 * 1.0 / 640);
            }
            curHight += bg->getContentSize().height;
            bg->runAction(CCFadeIn::create(0.5));
            tBatchNode->addChild(bg);
        }
        m_viewNode->addChild(tBatchNode);
        m_viewNode->setPositionY(m_viewNode->getPositionY()-extH);
        
        CCCommonUtils::setButtonTitle(m_achieveBtn, _lang("137401").c_str());
        CCCommonUtils::setButtonTitle(m_medalBtn, _lang("137402").c_str());
        m_achieveBtn->setEnabled(openType!=0);
        m_medalBtn->setEnabled(openType==0);
        
        m_achieveNodeView = AchieveNodeView::create();
        
        m_achieveNodeView->setPosition(ccp(-320, 250));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_achieveNodeView->setPosition(ccp(0, 0));
        }
        m_medalNodeView = MedalNewNode::create(0);
        
        m_medalNodeView->setPosition(ccp(-320, 250));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_medalNodeView->setPosition(ccp(-768, 1035));
        }
        if (openType != 0)
        {
            m_achieveNode->addChild(m_medalNodeView);
        }
        else
        {
            m_achieveNode->addChild(m_achieveNodeView);
        }
        
        m_medalNodeView->setVisible(openType!=0);
        m_achieveNodeView->setVisible(openType==0);
        
		bRet = true;
	}while(0);
	return bRet;
}

void AchievementNewView::onSelectBtnClick(cocos2d::CCObject *pObj, CCControlEvent pEvent)
{
    m_achieveNode->removeAllChildren();
    if (m_achieveBtn == pObj)
    {
        m_achieveBtn->setEnabled(false);
        m_medalBtn->setEnabled(true);
        m_achieveNode->addChild(m_achieveNodeView);
    }
    else if (m_medalBtn == pObj)
    {
        m_achieveBtn->setEnabled(true);
        m_medalBtn->setEnabled(false);
        m_achieveNode->addChild(m_medalNodeView);
    }
    m_achieveNodeView->setVisible(!m_achieveBtn->isEnabled());
    m_medalNodeView->setVisible(!m_medalBtn->isEnabled());
}

void AchievementNewView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showPopupView(UIPopupViewType_9);
    setTitleName(_lang("137400"));
}

void AchievementNewView::onExit()
{
    UIComponent::getInstance()->m_googlePlayNode->setVisible(false);
    CCNode::onExit();
}

bool AchievementNewView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_achieveNode", CCNode*, this->m_achieveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_achieveBtn", CCControlButton*, this->m_achieveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_medalBtn", CCControlButton*, this->m_medalBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewNode", CCNode*, this->m_viewNode);
    
    return false;
}


SEL_CCControlHandler AchievementNewView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSelectBtnClick", AchievementNewView::onSelectBtnClick);
    
    return NULL;
}


//CLASS AchieveNodeView
#define CELL_HEGHT 350
AchieveNodeView* AchieveNodeView::create()
{
    AchieveNodeView* ret = new AchieveNodeView();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AchieveNodeView::init()
{
    bool bRet = false;
    do {
        CCLoadSprite::doResourceByCommonIndex(305, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(305, false);
        });
        
//        auto ccb = CCBLoadFile("AchieveNodeView", this, this);
        this->setContentSize(CCSizeMake(640, 660));
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCSizeMake(1536, 1350));
            this->setAnchorPoint(ccp(0.5, 0.5));
        }
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        int extH =  winSize.height - DEF_HEIGHT;
        if (CCCommonUtils::isIosAndroidPad()) {
            extH = winSize.height - DEF_HEIGHT_HD;
        }
        m_listNode  = CCNode::create();
        m_listNode->setAnchorPoint(ccp(0, 1));
        m_listNode->setContentSize(CCSizeMake(640, 640 + extH));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_listNode->setContentSize(CCSize(1536, 1685 + extH));
            m_listNode->setPosition(ccp(0, 1350));
        }
        
        m_data = CCArray::create();
        
        m_tableView = CCTableView::create(this, m_listNode->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_listNode->addChild(m_tableView);
        
        addChild(m_listNode);
        if(AchievementController::getInstance()->isDataBack){
            refreshView(ccs("1"));
        }else{
            getData();
        }
        
        bRet = true;
    } while (0);
    return bRet;
}

void AchieveNodeView::getData()
{
    AchievementController::getInstance()->getDataFromServer();
}

void AchieveNodeView::refreshView(cocos2d::CCObject *pObj)
{
    int rewardIdx = -1;
    m_data = AchievementController::getInstance()->getVisbleAchievement();
    string id = "";
    AchievementInfo* info = NULL;
    for (int i = 0; i < m_data->count(); i++)
    {
        id = ((CCString*)(m_data->objectAtIndex(i)))->getCString();
        info = &AchievementController::getInstance()->m_infos[id];
        if (info->state == ACHIEVEMENT_COMPELETE)
        {
            rewardIdx = i;
            break;
        }
    }
    CCPoint p = m_tableView->getContentOffset();
    m_tableView->reloadData();
    
    if (rewardIdx>0 && pObj)
    {
        float offSetY = m_listNode->getContentSize().height;
        if (rewardIdx > 0)
        {
            if (CCCommonUtils::isIosAndroidPad()) {
                offSetY = (m_data->count()-rewardIdx) * 695;
            } else {
                offSetY = (m_data->count()-rewardIdx)*CELL_HEGHT;
            }
        }
        p.y = m_listNode->getContentSize().height - offSetY;
        m_tableView->setContentOffset(p);
    }
    else if (!pObj)
    {
        m_tableView->setContentOffset(p);
    }
}

CCSize AchieveNodeView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSizeMake(1536, 695);
    }
    return CCSizeMake(640, CELL_HEGHT);
}

CCSize AchieveNodeView::cellSizeForTable(  cocos2d::extension::TableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSizeMake(1536, 695);
    }
    return CCSizeMake(640, CELL_HEGHT);
}

CCTableViewCell* AchieveNodeView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return NULL;
    }
    std::string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    AchievementInfo* info = &AchievementController::getInstance()->m_infos[itemId];
    AchieveNodeViewCell* cell = (AchieveNodeViewCell*)table->dequeueCell();
    if (cell == NULL)
    {
        cell = AchieveNodeViewCell::create(info);
    }
    else
    {
        cell->setData(info);
    }
    return cell;
}

ssize_t AchieveNodeView::numberOfCellsInTableView(  cocos2d::extension::TableView *table)
{
    return m_data->count();
}

void AchieveNodeView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchieveNodeView::refreshView), ACHIEVEMENT_VISIBLE_STATE_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchieveNodeView::refreshView), ACHIEVEMENT_STATE_CHAGE, NULL);
}

void AchieveNodeView::onExit()
{
    CCNode::onExit();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ACHIEVEMENT_VISIBLE_STATE_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ACHIEVEMENT_STATE_CHAGE);
}

//ClASS AchieveNodeViewCell
#include "RewardController.h"
#include "CCStarBar.h"
#include "ParticleController.h"

AchieveNodeViewCell* AchieveNodeViewCell::create(AchievementInfo* info)
{
    AchieveNodeViewCell* ret = new AchieveNodeViewCell();
    if (ret && ret->init(info))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AchieveNodeViewCell::init(AchievementInfo* info)
{
    bool bRet = false;
    do {
        
        auto ccb = CCBLoadFile("AchieveNodeViewCell", this, this);
        this->setContentSize(ccb->getContentSize());
        CCCommonUtils::setButtonTitle(m_rewardbBtn, _lang("107516").c_str());
        m_waitInterface = NULL;
        setData(info);
        
        bRet = true;
    } while (0);
    return bRet;
}

void AchieveNodeViewCell::setData(AchievementInfo *info)
{
    m_achieveInfo = info;
    if (m_waitInterface != nullptr)
    {
        m_waitInterface->remove();
        m_waitInterface = nullptr;
    }
    int base = 137053+ m_achieveInfo->showOrder;
    m_titleLabel->setString(m_achieveInfo->name+" "+_lang(CC_ITOA(base)));
    m_rdMsgLabel->setString(m_achieveInfo->description);
    m_rewardbBtn->setVisible(false);
    m_proNode->setVisible(false);
    m_rewardNode0->setVisible(true);
    m_rewardNode1->setVisible(true);
    m_comLabel->setVisible(false);
    m_rewardbBtn->setEnabled(true);
    
    if (m_achieveInfo->state == ACHIEVEMENT_COMPELETE)
    {
        m_rewardbBtn->setVisible(true);
    }
    else if (m_achieveInfo->state == ACHIEVEMENT_NORMAL)
    {
        m_proNode->setVisible(true);
    }
    else if (m_achieveInfo->state == ACHIEVEMENT_REWARD_GET)
    {
        m_rewardNode0->setVisible(false);
        m_rewardNode1->setVisible(false);
        m_comLabel->setVisible(true);
        m_comLabel->setString(_lang("137404"));
    }
    
    std::string str = CC_CMDITOA(m_achieveInfo->currentNum > m_achieveInfo->totalNum ? m_achieveInfo->totalNum : m_achieveInfo->currentNum);
    str.append("/");
    str.append(CC_CMDITOA(m_achieveInfo->totalNum));
    float pro = m_achieveInfo->currentNum * 1.0 / m_achieveInfo->totalNum;
    pro = pro > 1 ? 1 : pro;
    m_progress->setScaleX(pro);
    m_progressTxt->setString(str);
    
    m_top2Node->getChildByTag(11)->setVisible(false);
    m_top2Node->getChildByTag(12)->setVisible(false);
    if (m_achieveInfo->state != ACHIEVEMENT_REWARD_GET)
    {
        CCArray* rewardArr = m_achieveInfo->reward;
        CCDictionary* dict = nullptr;
        for (int i = 0; i < rewardArr->count(); i++)
        {
            dict = dynamic_cast<CCDictionary*>(rewardArr->objectAtIndex(i));
            displayReward(dict, m_top2Node->getChildByTag(11+i));
            m_top2Node->getChildByTag(11+i)->setVisible(true);
        }
    }
    resetStarNode();
}

void AchieveNodeViewCell::resetStarNode()
{
    int showOrder = m_achieveInfo->showOrder;
    if ((m_achieveInfo->series==1|| m_achieveInfo->series==2) && showOrder>5)
    {
        if (showOrder==6 && (m_achieveInfo->state!=ACHIEVEMENT_REWARD_GET))
        {
            initNormalStarNode();
        }
        else
        {
            initSpeStarNode();
        }
    }
    else
    {
        initNormalStarNode();
    }
    
}

void AchieveNodeViewCell::initSpeStarNode()
{
    int showOrder = m_achieveInfo->showOrder;
    m_starNode->setVisible(false);
    m_speStarNode->setVisible(true);
    string speStarStr = "X";
    if (m_achieveInfo->state!=ACHIEVEMENT_REWARD_GET)
    {
        showOrder--;
    }
    speStarStr.append(CC_ITOA(showOrder));
    m_speStarTTF->setString(speStarStr);
}

void AchieveNodeViewCell::initNormalStarNode()
{
    int showOrder = m_achieveInfo->showOrder;
    m_starNode->setVisible(true);
    m_speStarNode->setVisible(false);
    CCSprite* starSp = nullptr;
    CCNode* partNode = m_starNode->getChildByTag(5);
    partNode->removeAllChildren();
    for (int i = 0; i < 5; i++)
    {
        starSp = dynamic_cast<CCSprite*>(m_starNode->getChildByTag(i));
        starSp->stopAllActions();
        starSp->setOpacity(255);
        if ((i<(showOrder-1)) || ((i==(showOrder-1)) && (m_achieveInfo->state==ACHIEVEMENT_REWARD_GET)))
        {
            starSp->setDisplayFrame(CCLoadSprite::loadResource("icon_xingxing01.png"));
        }
        else if ((i==(showOrder-1)) && (m_achieveInfo->state==ACHIEVEMENT_COMPELETE))
        {
            starSp->setDisplayFrame(CCLoadSprite::loadResource("icon_xingxing01.png"));
            auto praticle1 = ParticleController::createParticle("UIGlowLoop_1");
            praticle1->setAnchorPoint(ccp(0.5, 0.5));
            partNode->addChild(praticle1);
            auto praticle2 = ParticleController::createParticle("UIGlowLoop_2");
            praticle2->setAnchorPoint(ccp(0.5, 0.5));
            partNode->addChild(praticle2);
            CCActionInterval * rotateto1 = CCFadeOut::create(1);
            CCActionInterval * rotateto2 = CCFadeIn::create(1);
            CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,rotateto2,NULL);
            CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
            starSp->runAction(rotateForever);
            partNode->setPosition(starSp->getPosition());
        }
        else
        {
            starSp->setDisplayFrame(CCLoadSprite::loadResource("icon_xingxing02.png"));
        }
    }
}

void AchieveNodeViewCell::displayReward(cocos2d::CCDictionary *dict, CCNode* rewardNode)
{
    int type = dict->valueForKey("type")->intValue();
    std::string iconStr = "";
    int num = 0;
    int itemNameId = type;
    if(type == R_GOODS){
        auto typeDict = dynamic_cast<CCDictionary*>(dict->objectForKey("value"));
        num = typeDict->valueForKey("num")->intValue();
        std::string itemId = typeDict->valueForKey("id")->getCString();
        itemNameId = atoi(itemId.c_str());
        iconStr = RewardController::getInstance()->getPicByType(type, atoi(itemId.c_str())).c_str();
    }else{
        num = dict->valueForKey("value")->intValue();
        iconStr = RewardController::getInstance()->getPicByType(type, num).c_str();
    }
    dynamic_cast<CCSprite*>(rewardNode->getChildByTag(0))->setDisplayFrame(CCLoadSprite::loadResource(iconStr.c_str()));
    if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
    {
        CCCommonUtils::setSpriteMaxSize(dynamic_cast<CCSprite*>(rewardNode->getChildByTag(0)), 50*1.8);
    }
    else
        CCCommonUtils::setSpriteMaxSize(dynamic_cast<CCSprite*>(rewardNode->getChildByTag(0)), 50);
    
    string para = CCCommonUtils::getPropById(CC_ITOA(itemNameId), "para");
    num = num*atoi(para.c_str());
    std::string str = CC_ITOA(num);

//    string name = RewardController::getInstance()->getNameByType(type, itemNameId);
    string name = str + " " + _lang("107518");
//    name.append("  X").append(str.c_str());
    dynamic_cast<CCLabelIF*>(rewardNode->getChildByTag(1))->setString(name);
}

void AchieveNodeViewCell::onRewardClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    m_rewardbBtn->setEnabled(false);
    m_waitInterface=GameController::getInstance()->showWaitInterface(m_rewardbBtn);
    AchievementController::getInstance()->getReward(m_achieveInfo->currentItemId);
    int itemId = atoi(m_achieveInfo->currentItemId.c_str());
    if (m_achieveInfo && itemId>=9213 && itemId<=9228) {
        string param1 = CCCommonUtils::getPropById(m_achieveInfo->currentItemId, "para1");
        CCArray* arr = CCArray::create();
        if (itemId>=9213 && itemId<=9218) {
            arr->addObject(CCString::create("medal2"));
        }
        if (itemId>=9219 && itemId<=9223) {
            arr->addObject(CCString::create("medal3"));
        }
        if (itemId>=9224 && itemId<=9228) {
            arr->addObject(CCString::create("medal4"));
        }
        if (itemId>=9229 && itemId<=9233) {
            arr->addObject(CCString::create("medal5"));
        }
        if (itemId>=260020 && itemId<=260024) {
            arr->addObject(CCString::create("medal8"));
        }
        if (itemId>=260030 && itemId<=260034) {
            arr->addObject(CCString::create("medal9"));
        }
        if (itemId>=260060 && itemId<=260064) {
            arr->addObject(CCString::create("medal10"));
        }
        if (itemId>=260070 && itemId<=260074) {
            arr->addObject(CCString::create("medal11"));
        }
        arr->addObject(CCString::create(CC_ITOA(m_achieveInfo->showOrder)));
        arr->addObject(CCString::create(param1));
        ActivityController::getInstance()->facebookPostMethod("achievement",arr);
    }
}

bool AchieveNodeViewCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    auto node = getParentTouchNode();
    if(!node){
        return false;
    }
    m_canClick = isTouchInside(node, pTouch);
    return true;
}

CCNode *AchieveNodeViewCell::getParentTouchNode(){
    auto pop = dynamic_cast<AchievementNewView*>(PopupViewController::getInstance()->getCurrentPopupView());
    if(pop){
        return pop->m_achieveNodeView->m_listNode;
    }
    return NULL;
}

void AchieveNodeViewCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void AchieveNodeViewCell::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    resetStarNode();
}

void AchieveNodeViewCell::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AchieveNodeViewCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg0", CCScale9Sprite*, this->m_bg0);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg1", CCScale9Sprite*, this->m_bg1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg2", CCScale9Sprite*, this->m_bg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_top2Node", CCNode*, this->m_top2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_starNode", CCNode*, this->m_starNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_proNode", CCNode*, this->m_proNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speStarNode", CCNode*, this->m_speStarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speStarTTF", CCLabelIF*, this->m_speStarTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rdMsgLabel", CCLabelIF*, this->m_rdMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progressTxt", CCLabelIF*, this->m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardbBtn", CCControlButton*, this->m_rewardbBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_comLabel", CCLabelIF*, this->m_comLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardNode0", CCNode*, this->m_rewardNode0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardNode1", CCNode*, this->m_rewardNode1);
    
    return false;
}


SEL_CCControlHandler AchieveNodeViewCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoogleClick", AchievementNewView::onGoogleClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardClick", AchieveNodeViewCell::onRewardClick);
    
    return NULL;
}

