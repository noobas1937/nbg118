//
//  GoldExchangeSaleView.cpp
//  IF
//
//  Created by lifangkai on 14-9-10.
//
//

#include "GoldExchangeSaleView.h"
#include "PayController.h"
#include "DataRecordCommand.h"
#include "ToolController.h"
#include "ToolInfo.h"
#include "FreeListView.h"
#include "PopupViewController.h"
#include "PaymentCommand.h"
#include "GoldExchangeMoreListView.h"
#include "ParticleController.h"
#include "ButtonLightEffect.h"
#include "EquipmentController.h"
#include "LuaController.h"
#include "TipsView.h"
#include "RewardController.h"

GoldExchangeSaleView* GoldExchangeSaleView::create(GoldExchangeItem *goldExchangeItem, int parentType){
    GoldExchangeSaleView* ret = new GoldExchangeSaleView(goldExchangeItem, parentType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeSaleView::init()
{
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(102, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(102, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    /* d by ljf, 目前只提供了一个界面
    if(m_dataItem->popup_image=="thanksgiving"){
        CCBLoadFile("GoldThanksGivingSaleActView",this,this);
    } else if(m_dataItem->popup_image=="winter") {
        CCBLoadFile("GoldWinterSaleActView", this, this);
    } else if (m_dataItem->popup_image == "christmas"){
        CCBLoadFile("GoldChristmassSaleActView", this, this);
    }else if(m_dataItem->popup_image == "silent"){
        CCBLoadFile("GoldChristmasEveSaleActView", this, this);
    }else if(m_dataItem->popup_image == "newyear"){
        CCBLoadFile("GoldNewYearSaleActView", this, this);
    }else if(m_dataItem->popup_image == "throne" || m_dataItem->popup_image == "equip"|| m_dataItem->popup_image =="lover" || m_dataItem->popup_image == "chunjie"){
        std::string tmp = m_dataItem->popup_image;
        char aa = tmp.at(0);
        aa = std::toupper(aa);
        std::string subStr = aa + tmp.substr(1);
        subStr = "Gold"+subStr+"SaleActView";
        CCBLoadFile(subStr.c_str(), this, this, false);
    }else{
        CCBLoadFile("GoldExchangeSaleActView",this,this);
    }
    */
    CCBLoadFile("GoldExchangeSaleActView",this,this);
    this->setContentSize(CCSizeMake(640, 852));
    
//    m_titleLabel->setColor(ccc3(70,186,68));
    if(m_titleLabel){
        //m_titleLabel->setString(_lang("102148").c_str());
        //m_titleLabel->setString(_lang("101224").c_str());
        m_titleLabel->setString(_lang(m_dataItem->name).c_str());
        if(m_titleLabelBg)
        {
            m_titleLabelBg->setPreferredSize(CCSize(m_titleLabel->getContentSize().width + 15, m_titleLabelBg->getContentSize().height));
        }
    }
    m_getLabel->setString(_lang_1("115073",""));
    
    //m_getLabel1->setString(_lang_1("101237","")); //d by ljf, 新版界面未提供此控件
    m_timeLabel->setString("23:59:59");
  //  CCCommonUtils::setButtonTitle(m_moreBtn, _lang("102162").c_str());
    //m_moreLabel->setString(_lang("102162").c_str()); //d by ljf, 新版界面未提供此控件
    if(m_parentType==1){
        m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height+60 ));
    }
    m_scrollView = CCScrollView::create(m_listNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Default);
    m_listNode->addChild(m_scrollView);
   
    int extWidth = 0;
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
    //begin d by ljf
    //m_oldPriceLabel->setString(PayController::getInstance()->getDollarText(m_dataItem->oldprice,""));
    //end d by ljf
    //begin a by ljf,只保留小数点后两位
    string oldPrice = PayController::getInstance()->getDollarText(m_dataItem->oldprice,"");
    if (dollar.rfind(".") != string::npos)
        dollar = dollar.substr(0, dollar.rfind(".") + 3);
    if (oldPrice.rfind(".") != string::npos)
        oldPrice = oldPrice.substr(0, oldPrice.rfind(".") + 3);
    m_oldPriceLabel->setString(oldPrice);
    //end a by ljf
    m_newPriceLabel->setString(dollar);
    if(m_oldPriceLabel->getContentSize().width>90){
        extWidth = m_oldPriceLabel->getContentSize().width - 90;
    }
    if(m_newPriceLabel->getContentSize().width>90){
        extWidth += m_newPriceLabel->getContentSize().width - 90;
    }
    if(extWidth>0){
        CCSize costSize = m_costBtn->getContentSize();
        costSize.width += extWidth;
        m_costBtn->setPreferredSize(costSize);
    }
    if(m_btnGivePackage && m_nodeBuy){
        m_btnGivePackage->setPositionX(m_btnGivePackage->getPositionX()+extWidth * 0.5);
        if(m_dataItem->send_mail==false){
            m_btnGivePackage->setVisible(false);
            m_nodeBuy->setPositionX(0);
        }else{
            m_btnGivePackage->setVisible(true);
            m_nodeBuy->setPositionX(-35);
        }
    }
    string dollarPercent =  m_dataItem->percent+"%";
    m_percentLabel->setString(dollarPercent.c_str());
    //m_percentLabel->setFontSize(22.0);
    //m_percentLabel->setFontSize(22);
    //m_percentLabel->setScale(1.2); //d by ljf

    int tmpPercent = atoi(m_dataItem->gold_doller.c_str());
    string strPercent = CC_CMDITOA(tmpPercent);
    m_getGoldNumText->setString(strPercent.c_str());
    if(m_parentType==1){
 
        //m_moreNode->setVisible(false); //d by ljf, 新版无此
        m_scrollView->setTouchEnabled(true);
    }else{

        //m_moreNode->setVisible(true); //d by ljf, 新版无此
         m_scrollView->setTouchEnabled(false);
    }
    int curY = 0;
    int offX = 10;
    
    int cellSize = 90;
    if (m_dataItem->item != "") {
        vector<string> vector1;
        vector<string> vector2;
        CCCommonUtils::splitString(m_dataItem->item, "|", vector1);
        if(m_parentType==0){
            int j=vector1.size()-4;
            if(m_dataItem->popup_image=="thanksgiving"){
                j =vector1.size()-5;
            }
            if(j<0)
                j=0;
            while (j<vector1.size()) {
                vector2.clear();
                CCCommonUtils::splitString(vector1[j], ";", vector2);
                if (vector2.size()==2) {
                    auto cell = GoldExchangeSaleCell::create(atoi(vector2[0].c_str()), atoi(vector2[1].c_str()),m_dataItem->popup_image,0);
                    cell->setPosition(ccp(0 + offX, curY));
                    m_scrollView->cocos2d::CCNode::addChild(cell);
                    curY += cellSize;
                }
                j++;
            }
        
        }else{
            int i = 0;
            while (i < vector1.size()) {
                vector2.clear();
                CCCommonUtils::splitString(vector1[i], ";", vector2);
                if (vector2.size()==2) {
                    auto cell = GoldExchangeSaleCell::create(atoi(vector2[0].c_str()), atoi(vector2[1].c_str()),m_dataItem->popup_image,0);
                    cell->setPosition(ccp(0 + offX, curY));
                    m_scrollView->cocos2d::CCNode::addChild(cell);
                    curY += cellSize;
                }
                i++;
            }
        }
    }
    if (m_dataItem->equipment != "") {
        vector<string> vector1;
        vector<string> vector2;
        CCCommonUtils::splitString(m_dataItem->equipment, "|", vector1);
        if(m_parentType==0){
        }else{
            int i = 0;
            while (i < vector1.size()) {
                vector2.clear();
                CCCommonUtils::splitString(vector1[i], ";", vector2);
                if (vector2.size()==2) {
                    auto cell = GoldExchangeSaleCell::create(atoi(vector2[0].c_str()), atoi(vector2[1].c_str()),m_dataItem->popup_image,1);
                    cell->setPosition(ccp(0 + offX, curY));
                    m_scrollView->addChild(cell);
                    curY += cellSize;
                }
                i++;
            }
        }
    }
    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width, curY ));
    m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - curY ));
    
    
    
    m_soleOutSpr->setVisible(false);
    m_costBtn->setEnabled(true);
    if (m_dataItem->bought) {
        m_soleOutSpr->setVisible(true);
        m_costBtn->setEnabled(false);
    }
    
    onEnterFrame(0);
    if(m_dataItem->popup_image=="thanksgiving" || m_dataItem->popup_image=="winter"){
        m_moreSpr->setVisible(false); //d by ljf, 新版无此
        m_oldPriceLabel->setVisible(false);
        //m_getLabel1->setVisible(false); //d by ljf,
//        m_titleLabel->setVisible(false);
        m_soleOutSpr->setVisible(false);
        m_percentLabel->setScale(1.6);
    }
    m_animNode = CCNode::create();
    m_costBtn->getParent()->addChild(m_animNode);
    m_animNode->setPosition(m_costBtn->getPosition());
    CCSize btnSize = m_costBtn->getContentSize();
    ButtonLightEffect* btnEffect = ButtonLightEffect::create(btnSize);
    //m_animNode->addChild(btnEffect);
//    CCSprite* Spr = CCSprite::create();
//    m_animNode->addChild(Spr);
//    string curPath = "ButtonLight_%d.png";
//    CCCommonUtils::makeInternalEffectSpr(Spr, curPath, 12, 0.07,0,2.0);
//    //Spr->setContentSize(CCSizeMake(m_costBtn->getContentSize().width, 10));
//   // Spr->setPreferredSize(CCSizeMake(m_costBtn->getContentSize().width, m_costBtn->getContentSize().height));
//    Spr->setAnchorPoint(ccp(0.5, 0.5));
//    CCSize size = m_costBtn->getContentSize();
//    CCSize size1 = Spr->getContentSize();
//    float scalex = m_costBtn->getContentSize().width/Spr->getContentSize().width;
//    float scaley = 1.0;
//    if(m_dataItem->popup_image=="thanksgiving" || m_dataItem->popup_image=="winter"){
//        scaley = (m_costBtn->getContentSize().height)/Spr->getContentSize().height;
//        Spr->setPositionY(0);
//    }else{
//        scaley =(m_costBtn->getContentSize().height+10)/Spr->getContentSize().height;
//        Spr->setPositionY(3);
//    }
//    Spr->setScaleX(scalex);
//    Spr->setScaleY(scaley);
//    CCSize size2 = Spr->getContentSize();
//    
////    Spr->setPositionX(-size2.width*scalex/2);
//   // Spr->setPosition(ccp(-Spr->getContentSize().width/2, -Spr->getContentSize().height/2));
//    Spr->setOpacity(175);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if(m_dataItem->popup_image =="lover"){
        auto particle1 = CCParticleSystemQuad::create("particleImg/Petal_1.plist");
        auto particle2= CCParticleSystemQuad::create("particleImg/Petal_2.plist");
        addChild(particle1, -2);
        particle1->setPosition(ccp(winSize.width*0.5,winSize.height));
        addChild(particle2,-1);
        particle2->setPosition(particle1->getPosition());
    }
    return true;
}

void GoldExchangeSaleView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GoldExchangeSaleView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeSaleView::errorPayment), MSG_PAY_FAILED, NULL);
}

void GoldExchangeSaleView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PAY_FAILED);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GoldExchangeSaleView::onEnterFrame), this);
    setTouchEnabled(false);
    GlobalData::shared()->isBind = false;
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeSaleView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", GoldExchangeSaleView::onClickCostBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoreBtnClick", GoldExchangeSaleView::onMoreBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", GoldExchangeSaleView::onCloseBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGivePackageBtn", GoldExchangeSaleView::onClickGivePackageBtn);
    return NULL;
}

bool GoldExchangeSaleView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel); //标题
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_percentLabel", CCLabelBMFont*, this->m_percentLabel); //折扣
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getGoldNumText", CCLabelBMFont*, this->m_getGoldNumText);//钻石旁边的钱
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getGoldNumText", CCLabelIF*, this->m_getGoldNumText);//钻石旁边的钱
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oldPriceLabel", CCLabelIF*, this->m_oldPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newPriceLabel", CCLabelIF*, this->m_newPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getLabel", CCLabelIF*, this->m_getLabel); //底下按钮上的
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getLabel1", CCLabelIF*, this->m_getLabel1);//条幅上提示only once的
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreLabel", CCLabelIF*, this->m_moreLabel); //提示有更多的文本
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costBtn", CCControlButton*, this->m_costBtn); //底下按钮
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreBtn", CCControlButton*, this->m_moreBtn); //提示列表还有更多的按钮
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soleOutSpr", CCSprite*, this->m_soleOutSpr); //底下按钮上的sole out
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreSpr", CCSprite*, this->m_moreSpr); //提示向下滚动的箭头
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode); //覆盖整个界面，接受点击事件？
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchMoreNode", CCNode*, this->m_touchMoreNode); //覆盖列表区域，
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreNode", CCNode*, this->m_moreNode);//更多箭头的父结点
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeBuy", CCNode*, this->m_nodeBuy); //底下按钮的父结点
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnGivePackage", CCControlButton*, this->m_btnGivePackage); //礼物包按钮
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabelBg", CCScale9Sprite*, this->m_titleLabelBg);
    return false;
}

bool GoldExchangeSaleView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    if(m_parentType==1) {
        return true;
    }else if(isTouchInside(m_touchMoreNode, pTouch)){
        m_startPoint =pTouch->getLocation();
        return true;
    }
    return false;
}
void GoldExchangeSaleView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(m_parentType==1) {
      if (!isTouchInside(m_touchNode, pTouch)) {
          PopupViewController::getInstance()->removeLastPopupView();
      }
    }else{
        if (isTouchInside(m_touchMoreNode, pTouch)) {
            if(fabs(pTouch->getLocation().y - m_startPoint.y)<20)
                PopupViewController::getInstance()->addPopupView(GoldExchangeMoreListView::create(atoi(m_dataItem->id.c_str())));
        }
    }
    
    //    if (m_parentScrollView) {
    //        m_parentScrollView->setTouchEnabled(true);
    //    }
}
void GoldExchangeSaleView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
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

void GoldExchangeSaleView::setParentScrollView(CCScrollView* parentScrollView)
{
    m_parentScrollView = parentScrollView;
}
void GoldExchangeSaleView::onMoreBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    //PopupViewController::getInstance()->addPopupView(GoldExchangeMoreListView::create(atoi(m_dataItem->id.c_str())));
    
}
void GoldExchangeSaleView::onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PayController::getInstance()->callPayment(m_dataItem,"GoldExchangePopupView");
}
void GoldExchangeSaleView::onClickGivePackageBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    LuaController::toSelectUser(m_dataItem->id, true,1);
}
void GoldExchangeSaleView::onCloseBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_parentType==1) {
        PopupViewController::getInstance()->removeLastPopupView();
    }
}
void GoldExchangeSaleView::onEnterFrame(float dt)
{
    int curTime = GlobalData::shared()->getWorldTime();
    int lastTime = 0;
    if (m_dataItem->time>0) {
        int gapTime =(m_dataItem->end - curTime);
        int count =gapTime/(m_dataItem->time*3600);
        lastTime = m_dataItem->end - (m_dataItem->time*3600)*count-curTime;
    }else{
        lastTime = m_dataItem->end - curTime;
    }
    
    m_timeLabel->setString(CC_SECTOA(lastTime));
    if (m_dataItem->end - curTime<=0) {
//        ToolController::getInstance()->m_lastTime += 3600*24;
//        ToolController::getInstance()->m_canBuy = 1;
        m_soleOutSpr->setVisible(false);
        m_costBtn->setEnabled(true);
        PopupViewController::getInstance()->removeAllPopupView();
    }
}
void GoldExchangeSaleView::errorPayment(cocos2d::CCObject *obj){
    this->schedule(schedule_selector(GoldExchangeSaleView::onShowError),0.3,1,0);
}
void GoldExchangeSaleView::onShowError(float dt){
    PayController::getInstance()->showErrorDialog();
    this->unschedule(schedule_selector(GoldExchangeSaleView::onShowError));
}
////////
GoldExchangeSaleCell* GoldExchangeSaleCell::create(int itemId, int num,string popUpImage,int type)
{
    auto ret = new GoldExchangeSaleCell();
    if (ret && ret->init(itemId, num,popUpImage,type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeSaleCell::init(int itemId, int num,string popUpImage,int type)
{
    /* //d by ljf, 目前只有一个界面
    if(popUpImage=="thanksgiving" || popUpImage=="winter"){
        CCBLoadFile("RechargeSaleCell",this,this);
    } else {
        CCBLoadFile("GoldHalloweenSaleCell",this,this);
    }
    */
    CCBLoadFile("RechargeSaleCell",this,this);
    string name ="";
    string picStr = "";
    if(type == 0){
        if (itemId < 10) {// 资源
            picStr = CCCommonUtils::getResourceIconByType(itemId);
            name = CCCommonUtils::getResourceNameByType(itemId);
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(2).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 55,true);
            m_iconNode->addChild(iconBg);
            auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
            CCCommonUtils::setSpriteMaxSize(icon, 52, true);
            m_iconNode->addChild(icon);
            m_nameLabel->setString(name);
            m_numLabel->setString(CC_ITOA(num));
        }
        else {//道具
            CCCommonUtils::createGoodsIcon(itemId, m_iconNode, CCSize(55, 55),NULL,m_nameLabel);
            m_numLabel->setString(CC_ITOA(num));
//            auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
//            name = _lang(toolInfo.name);
//            picStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
//
//                auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//                CCCommonUtils::setSpriteMaxSize(iconBg, 55,true);
//                m_iconNode->addChild(iconBg);
        }
    }else if(type==1){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[itemId];
        name = _lang(eInfo.name);
        m_nameLabel->setString(name);
        m_numLabel->setString(CC_ITOA(num));
        string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        auto iconBg = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 55, true);
        m_iconNode->addChild(iconBg);
        picStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
        auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, 52, true);
        m_iconNode->addChild(icon);
    }
    
    bool ret = true;
    return ret;
}

void GoldExchangeSaleCell::onEnter() {
    CCNode::onEnter();
}

void GoldExchangeSaleCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeSaleCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool GoldExchangeSaleCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    return false;
}


//first pay cell
GoldExchangeFirstPayCell* GoldExchangeFirstPayCell::create(GoldExchangeItem *goldExchangeItem, int parentType){
    GoldExchangeFirstPayCell* ret = new GoldExchangeFirstPayCell(goldExchangeItem, parentType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeFirstPayCell::init()
{
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(102, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(102, false);
    });
    CCBLoadFile("GoldExchangeFirstPayActView",this,this);
   this->setContentSize(CCSizeMake(640, 852));
    m_titleLabel->setString(_lang("101232").c_str());
    m_titleLabel1->setString(_lang("101233").c_str());
    m_getLabel->setString(_lang_1("115073",""));
    m_buttonLabel->setString(_lang("101236"));
    m_timeLabel->setString("23:59:59");

 
    m_doubleLabel->setString(_lang("101235"));


//    m_scrollView = CCScrollView::create(m_listNode->getContentSize());
//    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
//    m_scrollView->setTouchPriority(Touch_Default);
//    m_listNode->addChild(m_scrollView);
    
    
    
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
    m_priceLabel3->setString(_lang_1("101245",dollar.c_str()));
    string dollarPercent =  CC_CMDITOA(1600);
    m_percentLabel->setString(dollarPercent.c_str());
    //m_percentLabel->setFontSize(22.0);
    //m_percentLabel->setFontSize(22);
    m_percentLabel->setScale(1.2);
    string getGOld =CC_CMDITOA(atoi(m_dataItem->gold_doller.c_str()));
    m_getGoldNumText->setString(getGOld.c_str());

//    m_scrollView->setTouchEnabled(false);


    m_animNode = CCNode::create();
    m_costBtn->getParent()->addChild(m_animNode);
    m_animNode->setPosition(m_costBtn->getPosition());
    CCSize btnSize = m_costBtn->getContentSize();
    ButtonLightEffect* btnEffect = ButtonLightEffect::create(btnSize);
    m_animNode->addChild(btnEffect);
//    CCSprite* Spr = CCSprite::create();
//    m_animNode->addChild(Spr);
//    string curPath = "ButtonLight_%d.png";
//    CCCommonUtils::makeInternalEffectSpr(Spr, curPath, 12, 0.07,0,2.0);
//    Spr->setAnchorPoint(ccp(0.5, 0.5));
//    CCSize size = m_costBtn->getContentSize();
//    CCSize size1 = Spr->getContentSize();
//    float scalex = m_costBtn->getContentSize().width/Spr->getContentSize().width;
//    float scaley = 1.0;
//
//    scaley = (m_costBtn->getContentSize().height)/Spr->getContentSize().height;
//    Spr->setPositionY(0);
//
//    Spr->setScaleX(scalex);
//    Spr->setScaleY(scaley);
//    CCSize size2 = Spr->getContentSize();
//    
//    Spr->setOpacity(175);
    return true;
}

void GoldExchangeFirstPayCell::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GoldExchangeFirstPayCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeFirstPayCell::errorPayment), MSG_PAY_FAILED, NULL);
}

void GoldExchangeFirstPayCell::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PAY_FAILED);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GoldExchangeFirstPayCell::onEnterFrame), this);
    setTouchEnabled(false);
    GlobalData::shared()->isBind = false;
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeFirstPayCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", GoldExchangeFirstPayCell::onClickCostBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoreBtnClick", GoldExchangeFirstPayCell::onMoreBtnClick);
    return NULL;
}

bool GoldExchangeFirstPayCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel1", CCLabelIF*, this->m_titleLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelBMFont*, this->m_titleLabel);

    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getGoldNumText", CCLabelBMFont*, this->m_getGoldNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_percentLabel", CCLabelIF*, this->m_percentLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getLabel", CCLabelIF*, this->m_getLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel3", CCLabelIF*, this->m_priceLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_doubleLabel", CCLabelIF*, this->m_doubleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonLabel", CCLabelIF*, this->m_buttonLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costBtn", CCControlButton*, this->m_costBtn);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);

    return false;
}

bool GoldExchangeFirstPayCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(m_parentType==1) {
        return true;
    }
    return false;
}
void GoldExchangeFirstPayCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(m_parentType==1) {
        if (!isTouchInside(m_touchNode, pTouch)) {
            PopupViewController::getInstance()->removeLastPopupView();
        }
    }
}
void GoldExchangeFirstPayCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

}

void GoldExchangeFirstPayCell::setParentScrollView(CCScrollView* parentScrollView)
{
    m_parentScrollView = parentScrollView;
}
void GoldExchangeFirstPayCell::onMoreBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    //PopupViewController::getInstance()->addPopupView(GoldExchangeMoreListView::create(atoi(m_dataItem->id.c_str())));
    
}
void GoldExchangeFirstPayCell::onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PayController::getInstance()->callPayment(m_dataItem);
}

void GoldExchangeFirstPayCell::onEnterFrame(float dt)
{
    int curTime = GlobalData::shared()->getWorldTime();
    int lastTime = 0;
    if (m_dataItem->time>0) {
        int gapTime =(m_dataItem->end - curTime);
        int count =gapTime/(m_dataItem->time*3600);
        lastTime = m_dataItem->end - (m_dataItem->time*3600)*count-curTime;
    }else{
        lastTime = m_dataItem->end - curTime;
    }
    
    m_timeLabel->setString(CC_SECTOA(lastTime));
    if (m_dataItem->end - curTime<=0) {
        //        ToolController::getInstance()->m_lastTime += 3600*24;
        //        ToolController::getInstance()->m_canBuy = 1;
        m_costBtn->setEnabled(true);
        PopupViewController::getInstance()->removeAllPopupView();
    }
}
void GoldExchangeFirstPayCell::errorPayment(cocos2d::CCObject *obj){
    this->schedule(schedule_selector(GoldExchangeFirstPayCell::onShowError),0.3,1,0);
}
void GoldExchangeFirstPayCell::onShowError(float dt){
    PayController::getInstance()->showErrorDialog();
    this->unschedule(schedule_selector(GoldExchangeFirstPayCell::onShowError));
}


////////////////////////////////////////////////////
MonthCardPayCell* MonthCardPayCell::create(GoldExchangeItem *goldExchangeItem){
    MonthCardPayCell* ret = new MonthCardPayCell(goldExchangeItem);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MonthCardPayCell::init(){
    CCBLoadFile("MonthCardSaleActView",this,this);
    this->setContentSize(CCSizeMake(540, 590));
    m_lblTitle->setString(_lang("101281"));
    m_moreLabel->setString(_lang("101287"));
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    refreshData();
    return true;
}
void MonthCardPayCell::refreshData(){
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
    setButtonTitle(m_costBtn, _lang_1("101245",dollar.c_str()));
    if(GlobalData::shared()->monthCardInfoList.find(m_dataItem->id)!=GlobalData::shared()->monthCardInfoList.end()){
        MonthCardInfo *card =GlobalData::shared()->monthCardInfoList[m_dataItem->id];
        m_percentLabel->setString(CC_ITOA(card->getCount()));
        m_desLabel->setString("D");
        CCArray *rewardArr  = card->rewards;
        CCDictionary* reward = NULL;
        CCDictionary* value = NULL;
        int maxCount = 2;
        int count = MIN(rewardArr->count(), maxCount);
        int type = 0;
        int iconW = 65;
        for (int i = 0; i < count; i++){
            m_nodeItem[i]->removeAllChildren();
            auto bg = CCLoadSprite::createSprite("Items_icon_kuang.png");
            m_nodeItem[i]->addChild(bg);
            CCCommonUtils::setSpriteMaxSize(bg, iconW,true);
            reward = _dict(rewardArr->objectAtIndex(i));
            type = reward->valueForKey("type")->intValue();
            if (type == R_GOODS){
                value = _dict(reward->objectForKey("value"));
                if (value){
                    int toolid = value->valueForKey("id")->intValue();
                    CCCommonUtils::createGoodsIcon(toolid, m_nodeItem[i], CCSize(iconW-10, iconW-10));
                    m_lblItem[i]->setString(std::string("x ") + value->valueForKey("num")->getCString());
                }
            }else{
                string nextIcon = RewardController::getInstance()->getPicByType(type,0);
                auto pic = CCLoadSprite::createSprite(nextIcon.c_str());
                m_nodeItem[i]->addChild(pic);
                CCCommonUtils::setSpriteMaxSize(pic, iconW-10,true);
                m_lblItem[i]->setString(std::string("x ") + reward->valueForKey("value")->getCString());
            }
        }
    }
    
}
void MonthCardPayCell::onEnter()
{
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(MonthCardPayCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    
}

void MonthCardPayCell::onExit()
{
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(MonthCardPayCell::onEnterFrame), this);
    CCNode::onExit();
}

SEL_CCControlHandler MonthCardPayCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", MonthCardPayCell::onClickCostBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", MonthCardPayCell::onCloseBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipClick", MonthCardPayCell::onTipClick);
    return NULL;
}

bool MonthCardPayCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_percentLabel", CCLabelBMFont*, this->m_percentLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelBMFont*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreLabel", CCLabelIF*, this->m_moreLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblItem1", CCLabelIF*, this->m_lblItem[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblItem2", CCLabelIF*, this->m_lblItem[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeItem1", CCNode*, this->m_nodeItem[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeItem2", CCNode*, this->m_nodeItem[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costBtn", CCControlButton*, this->m_costBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTip", CCControlButton*, this->m_btnTip);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle", CCLabelIF*, m_lblTitle);
    return false;
}
void MonthCardPayCell::onClickCostBtn(CCObject * pSender, CCControlEvent pCCControlEvent){
    if(m_dataItem==NULL){
        return;
    }
    PayController::getInstance()->callPayment(m_dataItem);
    PopupViewController::getInstance()->removeLastPopupView();
}
void MonthCardPayCell::onCloseBtnClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removeLastPopupView();
}
void MonthCardPayCell::onEnterFrame(float dt)
{
}
bool MonthCardPayCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    return true;
}
void MonthCardPayCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(!isTouchInside(this->m_touchNode, pTouch) ) {
        PopupViewController::getInstance()->removeLastPopupView();
    }
}
void MonthCardPayCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
void MonthCardPayCell::onTipClick(CCObject *pSender, CCControlEvent event){
    std::string tipB = _lang("101289");
    string doller = PayController::getInstance()->getDollarText(m_dataItem->dollar, m_dataItem->product_id);
    string showTip = _lang_1("101286",doller.c_str());
    
    if(!tipB.empty()){
        showTip.append("\n");
        showTip.append(tipB);
    }
    showTip.append("\n");
    showTip.append(_lang("101288"));
    PopupViewController::getInstance()->addPopupView(TipsView::create(showTip));
}
CCRect MonthCardPayCell::getCostBtnRect(){
    CCSize size = m_costBtn->getContentSize();
    CCPoint pos = m_costBtn->getPosition();
    pos = m_costBtn->getParent()->convertToWorldSpace(pos);
    pos = this->convertToNodeSpace(pos);
    return CCRect(pos.x, pos.y, size.width, size.height);
}