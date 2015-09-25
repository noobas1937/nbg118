//
//  AllianceCheckView.cpp
//  IF
//
//  Created by liweiyu on 14-8-26.
//
//

#include "AllianceCheckView.h"
#include "PopupViewController.h"
#include "CCLoadSprite.h"
#include "PlayerInfo.h"
#include "ParticleController.h"
#include "UpdateAllianceInfoView.h"
#include "CheckAllianceInfoView.h"
#include "JoinAllianceView.h"
#include "ChangeAllianceFlagView.h"
#include "AllianceFlagPar.h"
#include "ChangeAllianceAttributesCommand.h"
#include "AllianceDonateRankView.h"
#include "AllianceManager.h"
#include "GameController.h"
#include "SoundController.h"

std::string flags[15] = {
    "Blood",
    "Snow",
    "Warhammer",
    "Tree",
    "Cure",
    "Death",
    "Fire",
    "Ocean",
    "Sun",
    "Wolf",
    "Angel",
    "Cattle",
    "FireWolf",
    "Grail",
    "Knight"
};

AllianceCheckView *AllianceCheckView::create(){
    AllianceCheckView *ret = new AllianceCheckView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceCheckView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, true);
    touchAllow = true;
}

void AllianceCheckView::onExit(){
    setTouchEnabled(false);
    PopupViewController::getInstance()->removePopupView(this);
    PopupBaseView::onExit();
    touchAllow = true;
}

bool AllianceCheckView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        
        this->setModelLayerOpacity(200);
//        CCSize winsize = CCDirector::sharedDirector()->getWinSize();
        auto node = CCBLoadFile("AllianceCheckView", this, this);
        m_flagList->setVisible(false);
        m_flagList->setContentSize(CCSize(0,0));
//        node->setPositionY(node->getPositionY());
        this->setContentSize(node->getContentSize());
        //changeBGHeight(m_viewBg);
        m_icon->setZOrder(99);
        m_icon->removeAllChildrenWithCleanup(true);
        m_currentPic = GlobalData::shared()->playerInfo.allianceInfo.icon;
        AllianceFlagPar* flag = AllianceFlagPar::create(GlobalData::shared()->playerInfo.allianceInfo.getAllianceIcon().c_str());
        m_icon->addChild(flag);
        this->refresh();
        for(int i=0;i<m_buttonNodes.size();i++){
            m_buttonNodes[i]->setVisible(false);
        }
        flagView();
        ret = true;
    }
    return ret;
}

void AllianceCheckView::refresh(){
    m_buttonNodes.clear();
    m_buttonState.clear();
    m_node3->setVisible(false);
    m_node4->setVisible(false);
    m_node5->setVisible(false);
    m_node6->setVisible(false);
    m_node7->setVisible(false);
//    m_node8->setVisible(false);
    m_node9->setVisible(false);
//    m_nameTxt->setString(GlobalData::shared()->playerInfo.allianceInfo.name);
    
    //btn1
    CCSprite* spr = CCLoadSprite::createSprite("alliance_search.png");
    m_btnNode1->addChild(spr);
    m_nameText1->setString(_lang("115211").c_str());
    setButtonState(m_node1,ButtonSearch);
    //btn2
    spr = CCLoadSprite::createSprite("alliance_view.png");
    m_btnNode2->addChild(spr);
    m_nameText2->setString(_lang("115210").c_str());
    setButtonState(m_node2,ButtonCheck);
    //btn3
    spr = CCLoadSprite::createSprite("alliance_paihang.png");
    m_btnNode8->addChild(spr);
    m_nameText8->setString(_lang("115823").c_str());
    setButtonState(m_node8,ButtonPaihang);
    
    int rank = GlobalData::shared()->playerInfo.allianceInfo.rank;
//    if(AllianceManager::getInstance()->checkRight(CHANGE_SLOGAN, rank))
//    {
//        m_node3->setVisible(true);
//        spr = CCLoadSprite::createSprite("changeAnnounce.png");
//        m_btnNode3->addChild(spr);
//        m_nameText3->setString(_lang("115046").c_str());
//        setButtonState(m_node3,ButtonSlogan);
//    }
//    if(AllianceManager::getInstance()->checkRight(OPEN_RECRUITMENT, rank))
//    {
//        m_node4->setVisible(true);
//        spr = CCLoadSprite::createSprite("changeRecurit.png");
//        m_btnNode4->addChild(spr);
//        m_nameText4->setString(_lang("115047").c_str());
//        setButtonState(m_node4,ButtonOpen);
//    }
//    if(AllianceManager::getInstance()->checkRight(CHANGE_NAME, rank))
//    {
//        m_node5->setVisible(true);
//        spr = CCLoadSprite::createSprite("changeAllianceName.png");
//        m_btnNode5->addChild(spr);
//        m_nameText5->setString(_lang("115048").c_str());
//        setButtonState(m_node5,ButtonName);
//    }
//    if(AllianceManager::getInstance()->checkRight(CHANGE_BRIEF_NAME, rank))
//    {
//        m_node6->setVisible(true);
//        spr = CCLoadSprite::createSprite("modifyAbbr.png");
//        m_btnNode6->addChild(spr);
//        m_nameText6->setString(_lang("115058").c_str());
//        setButtonState(m_node6,ButtonBriName);
//    }
//    if(AllianceManager::getInstance()->checkRight(SHOW_ALLIANCE_LANGUAGE/*language*/, rank))
//    {
//        m_node7->setVisible(true);
//        spr = CCLoadSprite::createSprite("allianceLanguage.png");
//        m_btnNode7->addChild(spr);
//        m_nameText7->setString(_lang("115231").c_str());
//        setButtonState(m_node7,ButtonLang);
//    }
//    if(AllianceManager::getInstance()->checkRight(CHANGE_BANNER, rank))
//    {
//        m_node9->setVisible(true);
//        spr = CCLoadSprite::createSprite("alliance_changeFlag.png");
//        m_btnNode9->addChild(spr);
//        m_nameText9->setString(_lang("115049").c_str());
//        setButtonState(m_node9,ButtonJJ);
//    }

    if(AllianceManager::getInstance()->checkRight(SHOW_ALLIANCE_LANGUAGE, rank))
    {
        m_node5->setVisible(true);
        spr = CCLoadSprite::createSprite("changeAllianceName.png");
        m_btnNode5->addChild(spr);
        m_nameText5->setString(_lang("115250").c_str());
        setButtonState(m_node5,ButtonLang);
    }
    m_node6->setVisible(true);
    spr = CCLoadSprite::createSprite("alliance_changeFlag.png");
    m_btnNode6->addChild(spr);
    m_nameText6->setString(_lang("115256").c_str());
    setButtonState(m_node6,ButtonJJ);
    
    int num = m_buttonNodes.size();
    switch (num) {
        case 9://Rank == 5
        {
            m_buttonNodes[3]->setVisible(true);
            m_buttonNodes[4]->setVisible(true);
            m_buttonNodes[5]->setVisible(true);
            m_buttonNodes[6]->setVisible(true);
            m_buttonNodes[7]->setVisible(true);
            m_buttonNodes[8]->setVisible(true);
            m_buttonNodes[3]->setPosition(ccp(0, 242));
            m_buttonNodes[4]->setPosition(ccp(-146, 203));
            m_buttonNodes[5]->setPosition(ccp(-230, 62));
            m_buttonNodes[6]->setPosition(ccp(-230, -87));
            m_buttonNodes[7]->setPosition(ccp(-144, -228));
            m_buttonNodes[8]->setPosition(ccp(-0, -270));
        }
            break;
            
        case 6://Rank == 4
        {
            m_buttonNodes[3]->setVisible(true);
            m_buttonNodes[4]->setVisible(true);
            m_buttonNodes[5]->setVisible(true);
            m_buttonNodes[3]->setPosition(ccp(-191, 156));
            m_buttonNodes[4]->setPosition(ccp(-230, 9));
            m_buttonNodes[5]->setPosition(ccp(-195, -142));
        }
            break;
        
        default:
            break;
    }
}

void AllianceCheckView::setButtonState(CCNode* node,ButtonState state){
    m_buttonNodes.push_back(node);
    m_buttonState.push_back(state);
}

bool AllianceCheckView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode1", CCNode*, this->m_btnNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText1", CCLabelIF*, this->m_nameText1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode2", CCNode*, this->m_btnNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText2", CCLabelIF*, this->m_nameText2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node3", CCNode*, this->m_node3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode3", CCNode*, this->m_btnNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText3", CCLabelIF*, this->m_nameText3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node4", CCNode*, this->m_node4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode4", CCNode*, this->m_btnNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText4", CCLabelIF*, this->m_nameText4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node5", CCNode*, this->m_node5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode5", CCNode*, this->m_btnNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText5", CCLabelIF*, this->m_nameText5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node6", CCNode*, this->m_node6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode6", CCNode*, this->m_btnNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText6", CCLabelIF*, this->m_nameText6);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node7", CCNode*, this->m_node7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode7", CCNode*, this->m_btnNode7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText7", CCLabelIF*, this->m_nameText7);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node8", CCNode*, this->m_node8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode8", CCNode*, this->m_btnNode8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText8", CCLabelIF*, this->m_nameText8);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node9", CCNode*, this->m_node9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode9", CCNode*, this->m_btnNode9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText9", CCLabelIF*, this->m_nameText9);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick1", CCNode*, this->m_nodeClick1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick2", CCNode*, this->m_nodeClick2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick3", CCNode*, this->m_nodeClick3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick4", CCNode*, this->m_nodeClick4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick5", CCNode*, this->m_nodeClick5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick6", CCNode*, this->m_nodeClick6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick7", CCNode*, this->m_nodeClick7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick8", CCNode*, this->m_nodeClick8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeClick8", CCNode*, this->m_nodeClick9);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagList", CCNode*, this->m_flagList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnChange", CCControlButton*, this->m_btnChange);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCancle", CCControlButton*, this->m_btnCancle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldTxt", CCLabelIF*, this->m_goldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTxt", CCLabelIF*, this->m_btnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldSpr", CCSprite*, this->m_goldSpr);
    
    return false;
}

bool AllianceCheckView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    return true;
}

void AllianceCheckView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(!isTouchInside(m_clickNode, pTouch) && !isTouchInside(m_flagList, pTouch) && !isTouchInside(m_btnChange, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }else if(!m_flagList->isVisible()){
        int index = 99;
        int num = m_buttonNodes.size();
        for (int i=0; i<num; i++) {
            CCNode* clickNode = m_buttonNodes[i]->getChildByTag(88);
            if (isTouchInside(clickNode, pTouch)) {
                index = i;
                break;
            }
        }

        if (index!=99 && touchAllow) {
            touchAllow = false;
            switch (m_buttonState[index]) {
                case ButtonSlogan:
                {
                    PopupViewController::getInstance()->removePopupView(this);
                    PopupViewController::getInstance()->addPopupInView(UpdateAllianceInfoView::create(0));
                }
                    break;
                case ButtonOpen:
                {
                    PopupViewController::getInstance()->removePopupView(this);
                    PopupViewController::getInstance()->addPopupInView(UpdateAllianceInfoView::create(1));
                }
                    break;
                case ButtonName:
                {
                    PopupViewController::getInstance()->removePopupView(this);
                    PopupViewController::getInstance()->addPopupInView(UpdateAllianceInfoView::create(2));
                }
                    break;
                case ButtonBriName:
                {
                    PopupViewController::getInstance()->removePopupView(this);
                    PopupViewController::getInstance()->addPopupInView(UpdateAllianceInfoView::create(3));
                }
                    break;
                case ButtonLang:
                {
                    PopupViewController::getInstance()->removePopupView(this);
                    PopupViewController::getInstance()->addPopupInView(UpdateAllianceInfoView::create(-1));
                }
                    break;
                case ButtonJJ:
                {
//                    PopupViewController::getInstance()->addPopupView(ChangeAllianceFlagView::create());
                    for(int i=0;i<m_buttonNodes.size();i++){
                        m_buttonNodes[i]->setVisible(false);
                    }
                    flagView();

                }
                    break;
                case ButtonCheck:
                {
                    PopupViewController::getInstance()->removePopupView(this);
                    PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
                }
                    break;
                case ButtonSearch:
                {
                    PopupViewController::getInstance()->removePopupView(this);
                    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
                }
                    break;
                case ButtonPaihang:
                {
                    PopupViewController::getInstance()->removePopupView(this);
                    PopupViewController::getInstance()->addPopupInView(AllianceDonateRankView::create(0));
                    //排行榜
                }
                    break;
            }
        }
    }
}

SEL_CCControlHandler AllianceCheckView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeClick", AllianceCheckView::onChangeClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancleClick", AllianceCheckView::onCancleClick);
    return NULL;
}
void AllianceCheckView::onChangeClick(CCObject *pSender, CCControlEvent event){
    if(!AllianceManager::getInstance()->checkRight(CHANGE_BANNER, GlobalData::shared()->playerInfo.allianceInfo.rank)){
        CCCommonUtils::flyText(_lang("115257"));
    }else if(m_currentPic==GlobalData::shared()->playerInfo.allianceInfo.icon){
        CCCommonUtils::flyHint("", "", _lang("105228"));
    }else if(m_currentPic!=""){
        ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
        command->putParam("icon", CCString::create(m_currentPic));
        command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceCheckView::saveSuccess), NULL));
        command->sendAndRelease();
        GameController::getInstance()->showWaitInterface();
    }
}

void AllianceCheckView::onCancleClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceCheckView::saveSuccess(CCObject* p){
    GameController::getInstance()->removeWaitInterface();
    GlobalData::shared()->playerInfo.allianceInfo.icon = m_currentPic;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_ALLIANCE_ICON);
    PopupViewController::getInstance()->removePopupView(this);
}

void AllianceCheckView::flagView(){
    m_flagList->setVisible(true);
    m_flagList->setContentSize(CCSize(640,170));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_flagList->setContentSize(CCSize(1536, 430));
    }
    m_btnTxt->setString(_lang("115061"));
    m_goldTxt->setString(CC_ITOA(GlobalData::shared()->alliance_cost_k3));
    m_btnChange->setEnabled(false);

    m_tabView = CCTableView::create(this, m_flagList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionHorizontal);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setTag(123);
    m_flagList->addChild(m_tabView);
    m_tabView->reloadData();
    CCCommonUtils::setButtonTitle(m_btnCancle, _lang("115021").c_str());
}

void AllianceCheckView::tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    int idx = cell->getIdx();
    
    m_currentPic = flags[idx];
    m_icon->removeAllChildren();
    string str = flags[idx]+ ".png";
    AllianceFlagPar* flag = AllianceFlagPar::create(str.c_str());
    m_icon->addChild(flag);
    m_btnChange->setEnabled(m_currentPic!=GlobalData::shared()->playerInfo.allianceInfo.icon);
    if(table->cellAtIndex(currCelect))
        table->cellAtIndex(currCelect)->getChildByTag(456)->setVisible(false);
    cell->getChildByTag(456)->setVisible(true);
    currCelect = idx;
    //
    float cur = table->getContentOffset().x;
    float x = 640 - (idx+1)*142;
    float _x = -idx*142;
    if (CCCommonUtils::isIosAndroidPad()) {
        x = 1536 - (idx + 1) * 372;
        _x = -idx * 372;
    }
    if(cur > x){
        table->setContentOffsetInDuration(ccp(x,0), 0.3);
    }
    else if(cur < _x){
        table->setContentOffsetInDuration(ccp(_x,0), 0.3);
    }
}
cocos2d::CCSize AllianceCheckView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(372, 430);
    }
    return CCSize(142,170);
}
cocos2d::CCSize AllianceCheckView::cellSizeForTable(  cocos2d::extension::TableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(372, 430);
    }
    return CCSize(142,170);
}

ssize_t AllianceCheckView::numberOfCellsInTableView(  cocos2d::extension::TableView *table){
    return m_cellCnt;
}
CCTableViewCell* AllianceCheckView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    if (idx >= m_cellCnt) {
        return NULL;
    }
    FlagCell* cell = (FlagCell*)table->dequeueCell();
    if(cell){
        cell->setData(idx, currCelect);
    }else{
        cell = FlagCell::create(idx, currCelect);
    }
    return cell;
}

//cell
FlagCell* FlagCell::create(int idx,int celect){
    FlagCell* ret = new FlagCell();
    if(ret && ret->init(idx, celect)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FlagCell::init(int idx, int celect){

    bool ret = true;
    setData(idx, celect);
    return ret;
}

void FlagCell::setData(int idx,int celect){
    this->removeAllChildren();
    string str = flags[idx]+".png";
    CCSprite* spr = CCLoadSprite::createSprite(str.c_str());
    
    spr->setAnchorPoint(CCPointZero);
    spr->setScale(0.6);
    CCScale9Sprite* kuang = CCLoadSprite::createScale9Sprite("rect.png");
    kuang->setAnchorPoint(CCPointZero);
    kuang->setPreferredSize(CCSize(130,150));
    if (CCCommonUtils::isIosAndroidPad()) {
        spr->setScale(1.65);
        kuang->setPreferredSize(CCSize(350, 400));
    }
    this->addChild(kuang,1,456);
    this->addChild(spr);
    
    if(idx!=celect)
        kuang->setVisible(false);
}
