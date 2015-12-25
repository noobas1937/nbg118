//
//  AllRankListPopUpView.cpp
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#include "AllRankListPopUpView.h"
#include "PopupViewController.h"
#include "CCSafeNotificationCenter.h"
#include "MailWritePopUpView.h"
#include "UIComponent.h"
#include "SettingPopUpView.h"
#include "YesNoDialog.h"
#include "DataRecordCommand.h"
#include "AllianceRankView.h"
#include "PlayerRankListView.h"
#include "PlatformUtil.h"
#include "AchievementController.h"

void AllRankListPopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("108100"));
}

void AllRankListPopUpView::onExit(){
    PopupBaseView::onExit();
}

AllRankListPopUpView* AllRankListPopUpView::create(){
    AllRankListPopUpView* ret = new AllRankListPopUpView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllRankListPopUpView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(8, true);

    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
    });

    auto bg = CCBLoadFile("AllRankListView", this, this);
    this->setContentSize(bg->getContentSize());
    setTitleName(_lang("105513"));
    int preHeight = this->m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height+dh));
    m_infoList->setPositionY(m_infoList->getPositionY()-dh);
//    m_IOSNode->setPositionY(m_IOSNode->getPositionY() - dh / 2);
//    int totalH = 0;
//    while (totalH < CCDirector::sharedDirector()->getWinSize().height) {
//        auto pic = CCLoadSprite::createSprite("technology_09.png");
//        m_totalNode->addChild(pic);
//        pic->setAnchorPoint(ccp(0, 0));
//        pic->setPosition(ccp(0, totalH));
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            pic->setScale(2.4);
//        }
//        totalH += pic->getContentSize().height;
//    }
    m_totalNode->setPositionY(m_totalNode->getPositionY() - dh);

    if(GlobalData::shared()->analyticID == "market_global"){
        CCCommonUtils::setButtonTitle(m_gameCenterBtn, _lang("137051").c_str());
        m_hintText->setString(_lang("137052").c_str());
        m_facebookIcon->setDisplayFrame(CCLoadSprite::loadResource("icon_google-play.png"));
    }else{
        CCCommonUtils::setButtonTitle(m_gameCenterBtn, _lang("111083").c_str());
        m_hintText->setString(_lang("111084").c_str());
    }
    refresh();

    return true;
}

void AllRankListPopUpView::refresh(){
    int totalH = m_infoList->getContentSize().height;
    int start = (totalH-750)/2;
    if (CCCommonUtils::isIosAndroidPad())
    {
        start = (totalH-1850)/2;
    }
    if(GlobalData::shared()->analyticID == "AppStore" || GlobalData::shared()->analyticID == "market_global"){
        addIosShowBtn();
        auto bg = AllRankListCell::create(3);
        m_infoList->addChild(bg);
        int h = CCDirector::sharedDirector()->getWinSize().height;
        int totalH = h - 80;
        int upAndDowH = totalH >= 900 ? 852 : 852 - 80;
        if (CCCommonUtils::isIosAndroidPad())
        {
            bg->setScale(2.2f);
            int totalH = h - 154;
            upAndDowH = totalH >= 900 ? 2048 : 2048 - 154;
        }
        int dy = totalH - upAndDowH;
        m_IOSNode->setPositionY(m_IOSNode->getPositionY() - dy / 2);
        bg->setPositionY(dy / 2);
        if (CCCommonUtils::isIosAndroidPad())
        {
             bg->setPosition(ccp(100,100));
        }
    }else{
        m_IOSNode->setVisible(false);
        auto bg = AllRankListCell::create(2);
        m_infoList->addChild(bg);
        
        bg->setPositionY(start);
    
        auto bg1 = AllRankListCell::create(1);
        m_infoList->addChild(bg1);
        bg1->setPositionY(305+start);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            bg->setScale(2.2f);
            bg1->setScale(2.2f);
            bg1->setPositionY(305*2.2+start);
        }
    }
}

void AllRankListPopUpView::addIosShowBtn(){
    m_IOSNode->setVisible(true);
}

bool AllRankListPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_IOSNode", CCNode*, this->m_IOSNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gameCenterBtn", CCControlButton*, this->m_gameCenterBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIFTTF*, this->m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_facebookIcon", CCSprite*, this->m_facebookIcon);

    return true;
}

SEL_CCControlHandler AllRankListPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGameCenterClick", AllRankListPopUpView::onGameCenterClick);

    return NULL;
}

void AllRankListPopUpView::onGameCenterClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->analyticID == "market_global"){
        AchievementController::getInstance()->doOpenGooglePlay();
        return;
    }
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        PlatformUtilies::goto_IOSGameCenter();
    #endif
}


bool AllRankListPopUpView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return false;
}
void AllRankListPopUpView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return;
}
void AllRankListPopUpView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

}

AllRankListCell* AllRankListCell::create(int parentType){
    AllRankListCell* ret = new AllRankListCell(parentType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllRankListCell::init()
{
    CCBLoadFile("AllRankListCell",this,this);
    //当前联盟排行2条,玩家排行4条..为了展示全,所以暂定玩家高度4,联盟2
    CCSize nodeSize = m_listNode->getContentSize();
    int cellH = 70;
    int cellTotalH = 0;
    if(m_parentType==1){
        cellTotalH = cellH * 4;
    }else if(m_parentType==2){
        cellTotalH = cellH * 2;
    }else if(m_parentType == 3){
        int h = CCDirector::sharedDirector()->getWinSize().height;
        int totalH = h - 80;
        if(totalH >= 900){
            cellTotalH = cellH * 6;
        }else{
            cellTotalH = cellH * 5;
        }
    }
    float changeH = nodeSize.height - cellTotalH;
    m_listNode->setContentSize(CCSize(nodeSize.width, cellTotalH));
//    m_nodeTop->setPositionY(m_nodeTop->getPositionY()-changeH);
    
    m_top1->setPositionY(m_top1->getPositionY() - changeH);
    m_top2->setPositionY(m_top2->getPositionY() - changeH);
    m_bg1->setContentSize(m_bg1->getContentSize() + Size(0, -changeH));
    m_bg2->setContentSize(m_bg2->getContentSize() + Size(0, -changeH));
    
    int totalAddH = -changeH;
    int num = 0;
    while (totalAddH > 0) {
        auto sprite = CCLoadSprite::createSprite("Ranking_tiaomu_bg9.png");
        sprite->setAnchorPoint(ccp(0.5, 0));
        sprite->setPositionY(num * sprite->getContentSize().height);
        totalAddH -= sprite->getContentSize().height;
        m_bgContainer->addChild(sprite);
        num++;
    }
    
    m_scrollView = CCScrollView::create(m_listNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Default);
    m_listNode->addChild(m_scrollView);
    
    string name = "";
    string pic = "";
    int curY = 0;
    int count = 0;
    if(m_parentType==1 || m_parentType == 3){//个人排行榜
        auto cell3 = AllRankListSmallCell::create(4);
        cell3->setPosition(ccp(0, curY));
        m_scrollView->addChild(cell3);
        curY += cellH;
        count+=1;
        
        name = _lang("108101")+" "+_lang("108100");
        pic = "Ranking_icon.png";
        auto cell2 = AllRankListSmallCell::create(3);
        cell2->setPosition(ccp(0, curY));
        m_scrollView->addChild(cell2);
        curY += cellH;
        count+=1;
        auto cell1 = AllRankListSmallCell::create(2);
        cell1->setPosition(ccp(0, curY));
        m_scrollView->addChild(cell1);
        curY += cellH;
        count+=1;
        auto cell = AllRankListSmallCell::create(1);
        cell->setPosition(ccp(0, curY));
        m_scrollView->addChild(cell);
        curY += cellH;
        count+=1;
    }
    if(m_parentType==2 || m_parentType == 3){
        auto cell1 = AllRankListSmallCell::create(21);
        cell1->setPosition(ccp(0, curY));
        m_scrollView->addChild(cell1);
        curY += cellH;
        count+=1;
        name = _lang("108102")+" "+_lang("108100");
        pic = "Allance_flay.png";
        auto cell = AllRankListSmallCell::create(20);
        cell->setPosition(ccp(0, curY));
        m_scrollView->addChild(cell);
        curY += cellH;
        count+=1;

    }
    if(m_parentType == 3){
        pic = "Ranking_icon.png";
        name = _lang("108100");
    }
    if(count>3){
        m_moreBtn->setVisible(true);
    }else{
        m_moreBtn->setVisible(true);
    }
    m_nameLabel->setString(name);
    auto picspr = CCLoadSprite::createSprite(pic.c_str());
    picspr->setScale(100.0 / picspr->getContentSize().height);
//    if(m_parentType==2){
//        picspr->setScale(0.8);
//    }else if(m_parentType==1){
//        picspr->setPositionY(14);
//    }
    m_headNode->addChild(picspr);
    m_scrollView->setContentSize(CCSize(nodeSize.width,curY));
    m_scrollView->setContentOffset(ccp(0, cellTotalH - curY));
    return true;
}

void AllRankListCell::onEnter()
{
    CCNode::onEnter();
}

void AllRankListCell::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler AllRankListCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
     CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoreBtnClick", AllRankListCell::onMoreBtnClick);
    return NULL;
}

bool AllRankListCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreBtn", CCControlButton*, this->m_moreBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeTop", CCNode*, this->m_nodeTop);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeContent", CCNode*, this->m_nodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_top1", CCNode*, this->m_top1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_top2", CCNode*, this->m_top2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, this->m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCScale9Sprite*, this->m_bg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg2", CCScale9Sprite*, this->m_bg2);

    return false;
}
void AllRankListCell::onMoreBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    CCPoint pos = m_scrollView->getContentOffset();
    CCPoint minPos = m_scrollView->minContainerOffset();
    CCPoint maxPos = m_scrollView->maxContainerOffset();
    pos.y+=70;
    if(pos.y>maxPos.y){
        pos.y = maxPos.y;
    }
    if(pos.y <minPos.y){
        pos.y= minPos.y;
    }
    m_scrollView->setContentOffset(pos,0.5);
}
bool AllRankListCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    return false;
}
void AllRankListCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
}
void AllRankListCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}

void AllRankListCell::setParentScrollView(CCScrollView* parentScrollView)
{
}

////////
AllRankListSmallCell* AllRankListSmallCell::create(int type)
{
    auto ret = new AllRankListSmallCell(type);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllRankListSmallCell::init()
{
    CCBLoadFile("AllRankListSmallCell",this,this);
    string name ="";
    string picStr = "";
    m_cellBG2->setVisible(false);
    if(m_type==1){
        name = _lang("108101")+" "+ _lang("108103");
    }else if(m_type==2){
        name = _lang("108101")+" "+ _lang("108108");
    }else if(m_type==3){
        name = _lang("108101")+" "+ _lang("108110");
    }else if(m_type==4){
        name = _lang("108101")+" "+ _lang("108112");
    }else if(m_type==20){
        name = _lang("108102")+" "+_lang("102163")+" "+_lang("108100");
    }else if(m_type==21){
        name = _lang("108102")+" "+_lang("108108");
    }
    m_nameLabel->setString(name);

    bool ret = true;
    return ret;
}

void AllRankListSmallCell::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void AllRankListSmallCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler AllRankListSmallCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool AllRankListSmallCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBG", CCScale9Sprite*, m_cellBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBG0", CCScale9Sprite*, m_cellBG0);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBG1", CCScale9Sprite*, m_cellBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBG2", CCScale9Sprite*, m_cellBG2);
    return false;
}
bool AllRankListSmallCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (isTouchInside(m_cellBG, pTouch)) {
        m_startPoint = pTouch->getStartLocation();
        return true;
    }
    return false;
}
void AllRankListSmallCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {

      if (isTouchInside(m_cellBG, pTouch)) {
          CCPoint endPoint = pTouch->getLocation();
          if(fabs(endPoint.y-m_startPoint.y)>20||fabs(endPoint.x-m_startPoint.x)>20){
              return;
          }
          switch (m_type) {
              case 1:
              case 2:
              case 3:
              case 4:
                  PopupViewController::getInstance()->addPopupInView(PlayerRankListView::create(m_type));
                  break;
              case 20:
              case 21:
                  PopupViewController::getInstance()->addPopupInView(AllianceRankView::create(m_type-19));
              default:
                  break;
          }
      }

    
}
void AllRankListSmallCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    //    if (isTouchInside(m_listNode, pTouch)) {
    //        if (m_parentScrollView) {
    //            m_parentScrollView->setTouchEnabled(false);
    //        }
    //        m_scrollView->onTouchMoved(pTouch, pEvent);
    //    }
    //    else {
    //        if (m_parentScrollView) {
    //            m_parentScrollView->setTouchEnabled(true);
    //        }
    //        m_parentScrollView->onTouchMoved(pTouch, pEvent);
    //    }
}
