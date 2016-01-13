 
#include "CommonInclude.h"
#include "LongJingBuyView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "YesNoDialog.h"
#include "GoldExchangeView.h"
#include "UseResToolView.h"
#include "SoundController.h"
#include "ToolController.h"
#include "AllianceManager.h"

LongJingBuyView::~LongJingBuyView(void){
}

LongJingBuyView::LongJingBuyView():m_effNodePos(CCPointZero),m_effNodeSize(CCSizeZero),m_waitInterface(NULL),m_onClose(NULL),m_numCurrent(1),m_numAll(1){
}

LongJingBuyView* LongJingBuyView::show(const char *url,const char* title,const char* desc,int price,int color,CCCallFuncO * func,CCPoint startPos,int priceType,long cCount){
    LongJingBuyView* dialog = new LongJingBuyView();
    dialog->init(url,title,desc,price,color,priceType,cCount);
    dialog->setYesCallback(func);
    dialog->setStartPos(startPos);
    PopupViewController::getInstance()->addPopupView(dialog, false);
    dialog->release();
    return dialog;
}

bool LongJingBuyView::init(const char* url, const char* title,const char* desc,int price,int color,int priceType,long cCount){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    bool bRet=false;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    if (CCBLoadFile("LongJingBuyView.ccbi", this, this)) {
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        int maxWidth = m_subNode->getContentSize().width;
        int maxHeight = m_subNode->getContentSize().height;
        //label可滑动，放在scrollview上
        CCLabelIF* label = CCLabelIF::create();
        label->setDimensions(CCSize(maxWidth, 0));
        label->setString(desc);
        label->setColor(ccc3(169, 132, 71));
        label->setFontSize(24);
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            label->setFontSize(40);
//        }
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        int totalHeight = label->getContentSize().height * label->getOriginScaleY();
        if(totalHeight > maxHeight){
            CCScrollView* scroll = CCScrollView::create(CCSize(maxWidth, maxHeight));
            scroll->setContentSize(CCSize(maxWidth, totalHeight));
            scroll->addChild(label);
            m_subNode->addChild(scroll);
            scroll->setPosition(CCPointZero);
            scroll->setDirection(kCCScrollViewDirectionVertical);
            scroll->setAnchorPoint(ccp(0, 0));
            scroll->setContentOffset(ccp(0, maxHeight-totalHeight));
        }else{
            m_subNode->addChild(label);
            label->setAnchorPoint(ccp(0, 1));
            label->setPosition(ccp(0,maxHeight));
        }
        auto iconSize = m_sprIconBG->getContentSize();
        iconSize.width -= 20;
        iconSize.height -= 20;
        float scale = 1;
        if(color>=0){
            auto picBG = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
            auto picSize = picBG->getContentSize();
            float scale = MIN(iconSize.width/picSize.width,iconSize.height/picSize.height);
            picBG->setScale(scale);
//            if (CCCommonUtils::isIosAndroidPad())
//            {
//                picBG->setScale(2.f);
//            }
            m_nodeIcon->addChild(picBG);
            picBG->setPosition(m_sprIconBG->getPosition());
        }
        auto pic = CCLoadSprite::createSprite(url,true,CCLoadSpriteType_GOODS);
        auto picSize = pic->getContentSize();
        scale = MIN(iconSize.width/picSize.width,iconSize.height/picSize.height);
        pic->setScale(scale);
        m_nodeIcon->addChild(pic);
        pic->setPosition(m_sprIconBG->getPosition());
        m_lblTitle->setString(title);
        m_lblDesc->setString(_lang("104919"));
        if (priceType == -1)
        {
            m_lblDesc->setString(_lang_1("115817", title));
        }
//        m_btnOk->setTouchPriority(-1);
        m_btnOk->setTouchPriority(1);
        
        m_costNode->removeAllChildren();
        if (priceType == -1) {
            auto costIcon = CCLoadSprite::createSprite("Contribution_icon1.png");
            costIcon->setScale(0.7);
            if (costIcon) {
                m_costNode->addChild(costIcon);
            }
            int dy = 14;
            if (CCCommonUtils::isIosAndroidPad())
            {
                dy = 24;
            }
            m_costNode->setPositionY(m_costNode->getPositionY() - dy - 5);
            m_costNum->setPositionY(m_costNum->getPositionY() - dy);
            auto btnLabel = CCLabelIF::create();
            btnLabel->setString(_lang("102148"));
            btnLabel->setColor(ccWHITE);
            btnLabel->setFontSize(24);
            if (CCCommonUtils::isIosAndroidPad())
            {
                btnLabel->setFontSize(48);
            }
            btnLabel->setAnchorPoint(ccp(0.5, 0.5));
            btnLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
            btnLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
            m_costNode->getParent()->addChild(btnLabel);
            btnLabel->setPositionY(btnLabel->getPositionY() + dy);
            btnLabel->setZOrder(1);
            m_costNode->setZOrder(2);
        }
        else if(priceType < WorldResource_Max){
            auto costIcon = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(priceType).c_str());
            if(costIcon){
                m_costNode->addChild(costIcon);
            }
        }else{
            CCNode *priceNode = CCNode::create();
            auto priceSpr = CCLoadSprite::createSprite("Items_icon_kuang2.png");
            priceNode->addChild(priceSpr);
            auto priceSize = priceSpr->getContentSize();
            CCCommonUtils::createGoodsIcon(priceType, priceNode,priceSize-CCSize(20, 20));
            float scale = 38.0f/priceSize.width;
            priceNode->setScale(scale);
            priceNode->setPositionY(-5);
            m_costNode->addChild(priceNode);
        }
        
        m_sprBG->setVisible(false);
        
        m_price = price;
        m_priceType = priceType;
        
        setCostString();
        if(cCount>0){
            CCScale9Sprite* numBG = CCLoadSprite::createScale9Sprite("BG_quatnity.png");
            numBG->setColor(CCCommonUtils::getItemColor(color));
            numBG->setOpacity(200);
            CCLabelBMFont* numIF = CCLabelBMFont::create(CC_ITOA_K(cCount), "pve_fnt_boss.fnt");
            CCSize numSize = numIF->getContentSize();
            const float constScale = 0.3;
            CCSize defSize = m_sprIconBG->getContentSize();
            float scale = defSize.height*constScale/numSize.height;
            if((numSize.width * scale) > defSize.width){
                scale = defSize.width/numSize.width;
            }
            numIF->setScale(scale);
            numSize.height *= scale;
            numBG->setPreferredSize(CCSize(defSize.width, defSize.height*constScale));
            m_nodeIcon->addChild(numBG);
            m_nodeIcon->addChild(numIF);
            numIF->setPositionY(defSize.height * constScale * 0.5);
            numBG->setPosition(numIF->getPosition());
        }
        bRet=true;
    }
    return bRet;
}
void LongJingBuyView::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LongJingBuyView::removeWaitInter), MSG_ALLIANCE_BUY_FAIL, NULL);
    if (getParent() && (getParent()->getChildByTag(BUILDINGVIEW_TAG) || getParent()->getChildByTag(YESNODLG_TAG))){
    }else{
        setTag(YESNODLG_TAG);
    }
    showDialog();
}
void LongJingBuyView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_BUY_FAIL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK);
    setTouchEnabled(false);
   
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
    CCNode::onExit();
}

void LongJingBuyView::removeWaitInter(CCObject* obj)
{
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = nullptr;
    }
}
void LongJingBuyView::showDialog(){
    CCPoint endPos = m_nodeIcon->getPosition();
    float endScale = m_nodeIcon->getScale();
    CCSize nodeSize = CCSizeMake(150, 150);
    m_nodeIcon->setScale(0.8);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_nodeIcon->setScale(0.8 * endScale);
    }
    m_startPos = m_nodeIcon->getParent()->convertToNodeSpace(m_startPos);
    if (m_priceType != -1) {
        m_startPos.x += nodeSize.width * 0.4;
        m_startPos.y += nodeSize.height * 0.4;
    }
    m_nodeIcon->setPosition(m_startPos);
    m_nodeIcon->runAction(CCSequence::create(CCEaseExponentialOut::create(CCMoveTo::create(0.5, endPos)),NULL));
    m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(0.25, endScale),NULL));
    m_nodeIcon->runAction(CCSequence::create(CCDelayTime::create(0.15),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::showBG)),NULL));
}
void LongJingBuyView::showBG(){
    m_sprBG->setVisible(true);
    m_sprBG->runAction(CCSequence::create(CCFadeIn::create(0.1),NULL));
}
void LongJingBuyView::closeDialog(){
    if(m_sprBG){
        m_sprBG->stopAllActions();
        m_nodeIcon->stopAllActions();
        closeSelf();
        if(m_onClose){
            m_onClose->execute();
            m_onClose = NULL;
        }
    }
}
bool LongJingBuyView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return false;
    }
    m_sprBG->getParent()->runAction(CCSequence::create(CCScaleTo::create(0.25, 0),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::closeDialog)),NULL));
//    m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(0.25, 0),NULL));
    return true;
}

void LongJingBuyView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}
#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler LongJingBuyView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler LongJingBuyView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", LongJingBuyView::keypressedBtnOk);
    return NULL;
}
bool LongJingBuyView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, this->m_btnOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costNum", CCLabelIF*, this->m_costNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costNode", CCNode*, this->m_costNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeIcon", CCNode*, this->m_nodeIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprIconBG", CCSprite*, this->m_sprIconBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblDesc", CCLabelIF*, this->m_lblDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*, this->m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle", CCLabelIF*, this->m_lblTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCNode*, this->m_sprBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    return false;
}

#pragma mark -
#pragma mark Control Events
void LongJingBuyView::onOkBuyResource(){
    if(m_priceType < WorldResource_Max && m_priceType >= 0){
        if(m_priceType == Gold){
            PopupViewController::getInstance()->addPopupView(GoldExchangeView::create(),false);
        }else{
            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(m_priceType));
        }
    }
}
void LongJingBuyView::keypressedBtnOk(CCObject * pSender, CCControlEvent pCCControlEvent) {
    if(m_waitInterface){
        return;
    }
    CCLOG("---------keypressedBtnOk--------------");
    int price = m_price * m_numCurrent;
    if(m_priceType == -1) {
        if(GlobalData::shared()->playerInfo.allianceInfo.alliancepoint < price){
            CCCommonUtils::flyHint("", "", _lang("115827"));
            return;
        }
    }
    else if(m_priceType<WorldResource_Max){
        if (!CCCommonUtils::isEnoughResourceByType(m_priceType,price)) {
            if(m_priceType < WorldResource_Max && m_priceType >= 0){
                if(m_priceType == Gold){
                    this->closeSelf();
                    YesNoDialog::gotoPayTips();
                    if(m_onClose){
                        m_onClose->execute();
                        m_onClose = NULL;
                    }
                }else if (m_priceType == LongJing){
                    std::string res =  CCCommonUtils::getResourceNameByType(m_priceType);
                    std::string showString = _lang_1("111656",res.c_str());
                    YesNoDialog *dialog = YesNoDialog::showYesDialog(showString.c_str(),false,NULL,false);
                    dialog->setYesButtonTitle(_lang("101274").c_str());//确定
                    closeDialog();
                }
                else{
                    std::string res =  CCCommonUtils::getResourceNameByType(m_priceType);
                    std::string showString = _lang_2("104943",res.c_str(), res.c_str());
                    YesNoDialog *dialog = YesNoDialog::showYesDialog(showString.c_str(),false,CCCallFunc::create(this, callfunc_selector(LongJingBuyView::onOkBuyResource)),false);
                    dialog->setYesButtonTitle(_lang_1("104944",res.c_str()));
                    closeDialog();
                }
            }
            return;
        }
        
    }else{
        auto &tInfo = ToolController::getInstance()->getToolInfoById(m_priceType);
        if(tInfo.getCNT() < price){
            YesNoDialog::showYesDialog(_lang_2("104957", tInfo.getName().c_str(), m_lblTitle->getString().c_str()));
            closeDialog();
            return;
        }
    }
    
    if(m_numCurrent>1){
        std::string des = m_lblTitle->getString() + std::string(" x ") + CC_ITOA(m_numCurrent);
        std::string showString = _lang_1("104954", des.c_str());
        if (m_priceType == -1) {
            showString = _lang_1("115817", des.c_str());
        }
        YesNoDialog::showYesDialog(showString.c_str(),false,CCCallFunc::create(this, callfunc_selector(LongJingBuyView::onOKBuy)),false);
    }else{
        onOKBuy();
    }
}
void LongJingBuyView::onOKBuy(){
    if (m_onYes) {
        m_waitInterface=GameController::getInstance()->showWaitInterface(this);
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LongJingBuyView::playBuyEffect), MSG_BUY_CONFIRM_OK, NULL);
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LongJingBuyView::playBuyWithoutEffect), MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN, NULL);
        
        m_onYes->setObject(CCInteger::create(m_numCurrent));
        m_onYes->execute();
    }
}
void LongJingBuyView::playBuyWithoutEffect(CCObject *ccObj){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK);
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
    hideEffect();
}
void LongJingBuyView::playBuyEffect(CCObject *ccObj){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK);
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
    if(!m_sprBG){
        hideEffect();
        return;
    }
    m_sprBG->setVisible(false);
    CCPoint centrePos = m_sprBG->getPosition();
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(1.4, 1 * 1.9),NULL));
    } else {
        m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(1.4, 1),NULL));
    }
    m_nodeIcon->runAction(CCSequence::create(CCEaseExponentialOut::create(CCMoveTo::create(1, centrePos)),NULL));
    m_nodeIcon->runAction(CCSequence::create(CCDelayTime::create(0.8),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::showEffect)),NULL));
    m_nodeIcon->runAction(CCSequence::create(CCDelayTime::create(2.5),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::flyIcon)),NULL));
    
    
}
void LongJingBuyView::showEffect(){
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_secret);
    CCPoint centrePos = m_sprBG->getPosition();
    auto particle = ParticleController::createParticle("MallBag_1");
    m_sprBG->getParent()->addChild(particle,m_nodeIcon->getZOrder()-1);
    particle->setPosition(centrePos);
    auto particle1 = ParticleController::createParticle("MallBag_2");
    m_sprBG->getParent()->addChild(particle1,m_nodeIcon->getZOrder()-1);
    particle1->setPosition(centrePos);
    auto particle2 = ParticleController::createParticle("MallBag_3");
    m_sprBG->getParent()->addChild(particle2,m_nodeIcon->getZOrder()-1);
    particle2->setPosition(centrePos);
}
void LongJingBuyView::flyIcon(){
    m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(0.3, 0),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::hideEffect)),NULL));
}
void LongJingBuyView::hideEffect(){
        closeDialog();
}

void LongJingBuyView::setCostString(){
    int costVal = m_price * m_numCurrent;
    m_costNum->setString(CC_CMDITOA(costVal));
    if (m_priceType == -1) {
        if (GlobalData::shared()->playerInfo.allianceInfo.alliancepoint < costVal) {
            m_costNum->setColor(ccRED);
        } else {
            m_costNum->setColor({255, 225, 0});
        }
    }
    else if(m_priceType<WorldResource_Max){
        if (!CCCommonUtils::isEnoughResourceByType(m_priceType,costVal)){
            m_costNum->setColor(ccRED);
        }else{
            m_costNum->setColor(ccWHITE);
        }
    }else{
        auto &info = ToolController::getInstance()->getToolInfoById(m_priceType);
        if (info.getCNT()<costVal) {
            m_costNum->setColor(ccRED);
        }else{
            m_costNum->setColor(ccWHITE);
        }
    }
}
