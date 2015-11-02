//
//  FireSoldierView.cpp
//  LastEmpire
//
//  Created by zhangjun on 2015-03-19 10:33:42.
//
//

#include "FireSoldierView.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"
#include "fireandcomman.h"

//FireSoldierView::FireSoldierView(TreatInfo *info):m_info(info)
//{
//    CCLOGFUNC();
//}

FireSoldierView::~FireSoldierView()
{
    CCLOGFUNC();
}
FireSoldierView* FireSoldierView::create(TreatInfo *info)
{
    FireSoldierView *pRet = new FireSoldierView(info);
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
bool FireSoldierView::init()
{
    if (!PopupBaseView::init()) {
        CCLOG("!PopupBaseView::init()");
        return false;
    }
    bool bRet = false;
    setIsHDPanel(true);
    if (CCBLoadFile("FireSoldierView.ccbi", this, this))
    {
        m_totalNum=m_info->dead;
        m_fireNum=0;
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(m_info->armyId);
        std::string nameStr = it->second.getName();
        std::string fireStr=_lang_1("102377",nameStr.c_str());
        m_fireLabel->setString(fireStr.c_str());//_lang_1("102377",nameStr.c_str())
//        m_numMaxText->setString("m_numMaxText");
        m_fireBtn->setTitleForState(_lang("102135"), CCControlStateNormal);
        
        //滑动条
        //fusheng d
//        auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
//        m_sliderBg->setInsetBottom(5);
//        m_sliderBg->setInsetLeft(5);
//        m_sliderBg->setInsetRight(5);
//        m_sliderBg->setInsetTop(5);
//        m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
//        m_sliderBg->setPosition(ccp(304/2, 25));
//        m_sliderBg->setContentSize(CCSize(304,18));
        
//        auto proSp = CCLoadSprite::createSprite("huadongtiao4.png");
//        auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
//        
//        m_fireSlider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
        m_fireSlider = NBSlider::create("nb_bar_bg.png", "nb_bar_pro.png", "nb_cursor_icon.png",NBSlider::TextureResType::PLIST);
        m_fireSlider->setCapInsets(Rect(8, 1, 30, 13));
        float sw = m_sliderContainer->getContentSize().width;
        m_fireSlider->setContentSize(Size(sw,15));
        m_fireSlider->setMinimumValue(0.0f);
        m_fireSlider->setMaximumValue(1.0f);
        m_fireSlider->setValue(0);
//        m_fireSlider->setProgressScaleX(300/proSp->getContentSize().width);
        m_fireSlider->setTag(1);
        m_fireSlider->setLimitMoveValue(25);
//        m_fireSlider->setTouchPriority(Touch_Default);//Touch_Popup//fusheng d
//        m_fireSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(FireSoldierView::sliderCallBack), CCControlEventValueChanged);
        m_fireSlider->addEventListener(CC_CALLBACK_2(FireSoldierView::sliderCallBack, this));
        m_sliderContainer->addChild(m_fireSlider);
        
        auto editSize = m_editBoxNode->getContentSize();
        auto editpic =CCLoadSprite::createScale9Sprite("frame_3.png");
        editpic->setContentSize(editSize);
        editpic->setInsetBottom(1);
        editpic->setInsetTop(1);
        editpic->setInsetRight(1);
        editpic->setInsetLeft(1);
        m_editBox = CCEditBox::create(editSize,editpic );
        m_editBox->setInputMode(kEditBoxInputModeNumeric);
        m_editBox->setText("0");
        m_editBox->setDelegate(this);
//        m_editBox->setTouchPriority(Touch_Default);
        m_editBox->setMaxLength(7);
        m_editBox->setReturnType(kKeyboardReturnTypeDone);
        m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
        m_editBoxNode->addChild(m_editBox);

        refreshNum();
        
        bRet = true;
    }
    return bRet;
}
void FireSoldierView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FireSoldierView::refreshNum), ARMY_NUM_CHANGE, NULL);
}
void FireSoldierView::onExit()
{
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ARMY_NUM_CHANGE);
    CCNode::onExit();
}

SEL_CCControlHandler FireSoldierView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", FireSoldierView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", FireSoldierView::onSubClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFireBtnClick", FireSoldierView::onFireBtnClick);
    return NULL;
}

SEL_MenuHandler FireSoldierView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CallFuncN FireSoldierView::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool FireSoldierView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBoxNode", CCNode*,this->m_editBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderContainer", CCNode*,this->m_sliderContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCNode*,this->m_sprBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*,this->m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*,this->m_addBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBtn", CCControlButton*,this->m_subBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numMaxText", CCLabelIF*,this->m_numMaxText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireBtn", CCControlButton*,this->m_fireBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireLabel", CCLabelIF*,this->m_fireLabel);
    return false;
}
#pragma mark UI
//ccb
void FireSoldierView::onAddClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    float value  = m_fireSlider->getValue() * m_totalNum+1;
    if(m_totalNum<=0){
        m_fireSlider->setValue(0.0);
    }else
        m_fireSlider->setValue(1.0f * value / m_totalNum);
}

void FireSoldierView::onSubClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    float value  = m_fireSlider->getValue() * m_totalNum-1;
    if(m_totalNum<=0){
        m_fireSlider->setValue(0.0);
    }else
        m_fireSlider->setValue(1.0f * value / m_totalNum);
}
void FireSoldierView::onFireBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    CCLOG("m_fireNum：%d",m_fireNum);
    YesNoDialog::show(_lang("102376").c_str() , CCCallFunc::create(this, callfunc_selector(FireSoldierView::goToDismissAction)));
}
//ccTouch
bool FireSoldierView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_sprBG, pTouch)) {
        return false;
    }
    PopupViewController::getInstance()->removePopupView(this);
    return true;
}

void FireSoldierView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
//slider
void FireSoldierView::sliderCallBack(Ref *pSender, NBSlider::EventType type){
    m_fireNum = m_fireSlider->getValue() * m_totalNum;
    CCLOG("%d",m_fireNum);
    refreshNum();
}
//editBox
void FireSoldierView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    std::string temp  =text;
    size_t pos = temp.find_first_not_of("1234567890");
    while (pos!=std::string::npos) {
        temp.replace(pos, 1, "");
        pos = temp.find_first_not_of("1234567890");
    }
    int a = atoi(temp.c_str());
    std::string aaaa = CC_CMDITOA(a);
    editBox->setText(aaaa.c_str());
}
void FireSoldierView::editBoxReturn(CCEditBox *editBox){
    string numStr = m_editBox->getText();
    if(!numStr.empty()){
        size_t pos = numStr.find(",");
        while(pos!=string::npos){
            numStr.replace(pos, 1, "");
            pos=numStr.find(",");
        }
    }
    int num = atoi(numStr.c_str());
    if(num>m_totalNum){
        num = m_totalNum;
        
    }else if(num<0){
        num=0;
    }
    m_fireNum = num;
    if(m_totalNum<=0){
        m_fireSlider->setValue(0.0);
    }else
        m_fireSlider->setValue(1.0f * num / m_totalNum);
    if(m_totalNum<=0){
//        m_fireSlider->setTouchEnabled(false);//fusheng edit
        m_fireSlider->setEnabled(false);
    }else{
//        m_fireSlider->setTouchEnabled(true);//fusheng edit
         m_fireSlider->setEnabled(true);
    }
}
#pragma mark UI-end

void FireSoldierView::refreshNum(CCObject* p){
    CCLOG("m_info->dead:%d",m_info->dead);
    m_totalNum=m_info->dead;
    if(m_fireNum>m_totalNum){
        m_fireNum = m_totalNum;
    }
    m_editBox->setText(CC_CMDITOA(m_fireNum).c_str());
    if(m_fireNum>0){
        m_fireBtn->setEnabled(true);
    }else{
        m_fireBtn->setEnabled(false);
    }
    string maxStr = "/"+CC_CMDITOA(m_totalNum);
    m_numMaxText->setString(maxStr);
    if(m_totalNum<=0){
//        m_fireSlider->setTouchEnabled(false);//fusheng edit
         m_fireSlider->setEnabled(false);
    }else{
//        m_fireSlider->setTouchEnabled(true);//fusheng edit
        m_fireSlider->setEnabled(true);

    }
}

void FireSoldierView::goToDismissAction(){

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("resetTreatNum");
    
    fireandcomman *fire = new fireandcomman(m_info->armyId, m_fireNum,HOSPITAL_FIRE);
    fire->sendAndRelease();

    m_fireNum = 0;
    m_fireSlider->setValue(1.0f * m_fireNum);
    PopupViewController::getInstance()->goBackPopupView();    
    
}

