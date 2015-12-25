//
//  MedalNewView.cpp
//  IF
//
//  Created by chenfubi on 15/4/3.
//
//

#include "MedalNewView.h"
#include "AchievementController.h"
#include "AchievementNewView.h"
#include "PopupViewController.h"
//#include "TipsView.h"
//#include "PopupViewController.h"
MedalNewView* MedalNewView::create()
{
	MedalNewView* ret = new MedalNewView();
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

bool MedalNewView::init()
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
        
        auto ccb = CCBLoadFile("MedalNewView", this, this);
        this->setContentSize(ccb->getContentSize());
        
        int extH = getExtendHeight();
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//        int width = CCDirector::sharedDirector()->getWinSize().width;
//        int curHight = -maxHight/2.0f;
//        while (curHight<maxHight) {
//            auto bg = CCLoadSprite::createSprite("technology_09.png");
//            bg->setAnchorPoint(ccp(0, 1));
//            bg->setPosition(ccp(-width/2.0f, curHight));
//            curHight += bg->getContentSize().height;
//            bg->runAction(CCFadeIn::create(0.5));
//            tBatchNode->addChild(bg);
//        }
//        m_viewNode->addChild(tBatchNode);
        m_viewNode->setPositionY(m_viewNode->getPositionY()-extH);
//        m_waitInterface = GameController::getInstance()->showWaitInterface(m_listNode);
        MedalNewNode* medalNode = MedalNewNode::create(medalType_Other);
        medalNode->setPosition(ccp(-320, 350));
        m_listNode->addChild(medalNode);
        
		bRet = true;
	}while(0);
	return bRet;
}

void MedalNewView::getData(cocos2d::CCObject *pObj)
{
    MedalNewNode* medalNode = MedalNewNode::create(medalType_Other);
    medalNode->setPosition(ccp(-320, 350));
    m_listNode->addChild(medalNode);
}

void MedalNewView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("137402"));
}

void MedalNewView::onExit()
{
    CCNode::onExit();
}

bool MedalNewView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewNode", CCNode*, this->m_viewNode);
    return false;
}

SEL_CCControlHandler MedalNewView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoogleClick", AchievementPopUpView::onGoogleClick);
    return NULL;
}


//CLASS MedalNewNode
#define TABLEVEIW_TAG 999
static string selectMedalId = "";
MedalNewNode* MedalNewNode::create(int type/*=0*/)
{
    MedalNewNode* ret = new MedalNewNode();
    if (ret && ret->init(type))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MedalNewNode::init(int type)
{
	bool bRet = false;
	do
	{
        CCLoadSprite::doResourceByCommonIndex(310, true);
        
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(310, false);
        });
        
        m_type = type;
        float heght = type==0?0:100;
        auto ccb = CCBLoadFile("MedalNewNode",this,this);
        this->setContentSize(ccb->getContentSize());
        ccb->setAnchorPoint(ccp(0, 1));
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            ccb->setAnchorPoint(CCPointZero);
//        }
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        int extH = winSize.height - DEF_HEIGHT;
        if (CCCommonUtils::isIosAndroidPad()) {
            extH = winSize.height - DEF_HEIGHT_HD;
        }
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_listNode->setContentSize(CCSizeMake(1536, m_listNode->getContentSize().height + heght + extH));
        }
        else
            m_listNode->setContentSize(CCSizeMake(640, m_listNode->getContentSize().height + heght + extH));
        m_rdMsgLabel->setString(_lang("137405"));
        
        m_data = CCArray::create();
        selectMedalId = "";
        
        CCTableView* tableView = CCTableView::create(this, m_listNode->getContentSize());
        tableView->setDirection(kCCScrollViewDirectionVertical);
        tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        tableView->setTouchPriority(Touch_Default);
        tableView->setTag(TABLEVEIW_TAG);
        m_listNode->addChild(tableView);
		bRet = true;
	}while(0);
	return bRet;
}

CCSize MedalNewNode::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSizeMake(1536, 456);
    }
    return CCSizeMake(640, 228);
}

CCSize MedalNewNode::cellSizeForTable(  cocos2d::extension::TableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSizeMake(1536, 456);
    }
    return CCSizeMake(640, 228);
}

CCTableViewCell* MedalNewNode::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    MedalNewNodeCell* cell = (MedalNewNodeCell*)table->dequeueCell();
    int index = idx*3;
    CCArray* a = CCArray::create();
    for (int i = 0; i<3; i++)
    {
        if ((index+i) < m_data->count())
        {
            a->addObject(m_data->objectAtIndex(index+i));
        }
    }
    if (cell == NULL)
    {
        cell = MedalNewNodeCell::create(a, m_type);
    }
    else
    {
        cell->setData(a, m_type);
    }
    return cell;
}

ssize_t MedalNewNode::numberOfCellsInTableView(  cocos2d::extension::TableView *table)
{
    return ceil(m_data->count()/3.0f);
}

void MedalNewNode::reloadData(cocos2d::CCObject *pObj)
{
    CCTableView* tableView = dynamic_cast<CCTableView*>(m_listNode->getChildByTag(TABLEVEIW_TAG));
    if (tableView)
    {
        tableView->reloadData();
    }
    
}

void MedalNewNode::onEnter()
{
    CCNode::onEnter();
    CCLoadSprite::doResourceByCommonIndex(503, true);
    selectMedalId = "";
    if (m_type == medalType_Other)
    {
        m_data = AchievementController::getInstance()->getOtherMedalInfo();
    }
    else
    {
        m_data = AchievementController::getInstance()->getMyMedalInfo();
    }
    reloadData(NULL);
}

void MedalNewNode::onExit()
{
    CCLoadSprite::doResourceByCommonIndex(503, false);
    if (selectMedalId!="" && selectMedalId!=GlobalData::shared()->medalid)
    {
        GlobalData::shared()->medalid = selectMedalId;
        AchievementController::getInstance()->changePlayerMedalId();
    }
    CCNode::onExit();
}

bool MedalNewNode::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rdMsgLabel", CCLabelIF*, this->m_rdMsgLabel);
    
    return false;
}

SEL_CCControlHandler MedalNewNode::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoogleClick", AchievementPopUpView::onGoogleClick);
    return NULL;
}

//CLASS MedalNewNodeCell
MedalNewNodeCell* MedalNewNodeCell::create(CCArray* a, int type)
{
    MedalNewNodeCell* ret = new MedalNewNodeCell();
    if (ret && ret->init(a, type))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MedalNewNodeCell::init(CCArray* a, int type)
{
    bool bRet = false;
    do {
        this->setContentSize(CCSizeMake(640, 208));
        if (CCCommonUtils::isIosAndroidPad())
        {
            this->setContentSize(CCSizeMake(1536, 456));
        }
        CCSprite* bg0 = CCLoadSprite::createSprite("KIng_picture_05.png");
        CCSprite* bg1 = CCLoadSprite::createSprite("KIng_picture_05.png");
        bg0->setPosition(ccp(160, 20));
        bg1->setPosition(ccp(480, 20));
        if (CCCommonUtils::isIosAndroidPad()) {
            bg0->setAnchorPoint(CCPointZero);
            bg1->setAnchorPoint(CCPointZero);
            bg0->setPosition(CCPoint(0, 0));
            bg1->setPosition(ccp(768, 0));
            float scalex = 768 * 1.0 / 320;
            bg0->setScaleX(scalex);
            bg1->setScaleX(scalex);
            float scaley = 2;
            bg0->setScaleY(scaley);
            bg1->setScaleY(scaley);
        }
        bg0->setZOrder(0);
        bg1->setZOrder(0);
        addChild(bg0);
        addChild(bg1);
        setData(a, type);
        bRet = true;
    } while (0);
    return bRet;
}

void MedalNewNodeCell::setData(CCArray* a, int type)
{
    string medalId = "";
    for (int i = 0; i < 3; i++)
    {
        if (i < a->count())
        {
            medalId = dynamic_cast<CCString*>(a->objectAtIndex(i))->getCString();
        }
        if (getChildByTag(i) == nullptr)
        {
            MedalCell* cell = MedalCell::create("", type);
            cell->setTag(i);
            cell->setPosition(ccp(210*i, 0));
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(490 * i + 90, 0));
            }
            cell->setZOrder(100);
            addChild(cell);
        }
        if (i < a->count())
        {
            dynamic_cast<MedalCell*>(getChildByTag(i))->setData(medalId, type);
            getChildByTag(i)->setVisible(true);
        }
        else
        {
            getChildByTag(i)->setVisible(false);
        }
    }
}

void MedalNewNodeCell::onEnter()
{
    CCNode::onEnter();
}

void MedalNewNodeCell::onExit()
{
    CCNode::onExit();
}

//CLASS MedalCell
#include "ParticleController.h"
MedalCell* MedalCell::create(string medalId, int type)
{
    MedalCell* ret = new MedalCell();
    if (ret && ret->init(medalId, type))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MedalCell::init(string medalId, int type)
{
    bool bRet = false;
    do {
        auto ccb = CCBLoadFile("MedalCell", this, this);
        this->setContentSize(ccb->getContentSize());
        m_rectSp->setVisible(false);
        bRet = true;
    } while (0);
    return bRet;
}

void MedalCell::setData(string medalId, int type)
{
    m_type = type;
    m_rectSp->setVisible(false);
    if (type == medalType_My)
    {
        m_medalInfo = &AchievementController::getInstance()->m_myMedalInfos[medalId];
        if (m_medalInfo->medalId == atoi(GlobalData::shared()->medalid.c_str()))
        {
            m_rectSp->setVisible(true);
        }
    }
    else
    {
        m_medalInfo = &AchievementController::getInstance()->m_otherMedalInfos[medalId];
    }
    if (m_medalInfo == nullptr)
    {
        return;
    }
    
    string frameName = m_medalInfo->icon_main;
    if (CCLoadSprite::getSF(frameName.c_str()) == NULL)
    {
        frameName = "medal_1.png";
    }
    m_medalIcon->setDisplayFrame(CCLoadSprite::loadResource(frameName.c_str()));
    if (m_medalInfo->hasGet)
    {
        CCCommonUtils::setSpriteGray(m_medalIcon, false);
        m_nameTTF->setString(m_medalInfo->name);
        if (!CCCommonUtils::isIosAndroidPad())
        {
            if (m_nameTTF->getContentSize().height>60) {
                m_nameTTF->setPositionY(m_nameTTF->getPositionY()-10);
            }
        }
        addParticle();
    }
    else
    {
        CCCommonUtils::setSpriteGray(m_medalIcon, true);
        m_nameTTF->setString(_lang("137406"));
    }
    CCSprite* starSp = nullptr;
    string nameStr = "";
    if (m_medalInfo->level > 5)
    {
        m_starNode->setVisible(false);
        m_speStarNode->setVisible(true);
        string speStarStr = "X";
        speStarStr.append(CC_ITOA(m_medalInfo->level));
        m_speStarTTF->setString(speStarStr);
        return;
    }
    m_speStarNode->setVisible(false);
    m_starNode->setVisible(true);
    for (int i = 0; i < 5; i++)
    {
        nameStr = (i<m_medalInfo->level&&m_medalInfo->hasGet)?"icon_xingxing01.png":"icon_xingxing02.png";
        starSp = (CCSprite*)m_starNode->getChildByTag(i);
        starSp->setDisplayFrame(CCLoadSprite::loadResource(nameStr.c_str()));
    }
}

void MedalCell::addParticle()
{
    m_touchNode->removeAllChildren();
    if (!m_medalInfo || !m_medalInfo->hasGet)
    {
        return;
    }
    map<int, vector<float>> colorMap;
    for (int i=1; i<=2; i++) {
        colorMap.clear();
        AchievementController::getInstance()->getMedalIconColor(CC_ITOA(m_medalInfo->medalId), colorMap);
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Achievement_%d",i)->getCString());
        particle->setAnchorPoint(ccp(0.5, 0.5));
        particle->setPosition(ccp(m_touchNode->getContentSize().width/2.0f, m_touchNode->getContentSize().height/2.0f));
        vector<float> v = colorMap[i];
        ccColor4F color = {v.at(0), v.at(1), v.at(2), v.at(3)};
        particle->setStartColor(color);
        color = {v.at(4), v.at(5), v.at(6), v.at(7)};
        particle->setEndColor(color);
        particle->setScale(1.3f);
        if (CCCommonUtils::isIosAndroidPad()) {
            particle->setScale(2.6f);
        }
        m_touchNode->addChild(particle);
    }
}

CCNode *MedalCell::getParentTouchNode(){
    auto pop = dynamic_cast<AchievementNewView*>(PopupViewController::getInstance()->getCurrentPopupView());
    if(pop){
        return pop->m_medalNodeView->m_listNode;
    }
    return NULL;
}

bool MedalCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    auto node = getParentTouchNode();
    if(node == NULL){
        return false;
    }
    return isTouchInside(m_touchNode, pTouch)&&isTouchInside(node, pTouch);
}

void MedalCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) > 10)
    {
        return;
    }
    
    string info = "";
    if (m_medalInfo->hasGet)
    {
        info = _lang_1("137407", m_medalInfo->name.c_str());
        if (m_type == medalType_My)
        {
            selectMedalId = CC_ITOA(m_medalInfo->medalId);
        }
        if (m_type == medalType_Other)
        {
            string langKey = CCCommonUtils::getPropById(m_medalInfo->achievement, "type");
            info = _lang_1("137408", _lang(langKey).c_str());
        }
        if (m_type == medalType_My)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("MEDALCELL_TOUCH");
            m_rectSp->setVisible(true);
        }
    }
    else
    {
        string langKey = CCCommonUtils::getPropById(m_medalInfo->achievement, "type");
        info = _lang_1("137408", _lang(langKey).c_str());
    }
    
    CCCommonUtils::flyHint("", "", info);
//    PopupViewController::getInstance()->addPopupView(TipsView::create(info));
}

void MedalCell::touchCallBack(cocos2d::CCObject *pObj)
{
    m_rectSp->setVisible(false);
}

void MedalCell::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MedalCell::touchCallBack), "MEDALCELL_TOUCH", NULL);
    addParticle();
}

void MedalCell::onExit()
{
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "MEDALCELL_TOUCH");
    CCNode::onExit();
}

bool MedalCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_medalIcon", CCSprite*, this->m_medalIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTTF", CCLabelIF*, this->m_nameTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speStarTTF", CCLabelIF*, this->m_speStarTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speStarNode", CCNode*, this->m_speStarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_starNode", CCNode*, this->m_starNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rectSp", CCSprite*, this->m_rectSp);
    
    return false;
}

SEL_CCControlHandler MedalCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    return NULL;
}
