//
//  CDkeyPopUpView.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-27.
//
//

#include "CDkeyPopUpView.h"
#include "PopupViewController.h"
#include "CDkeyRewardCommand.h"

bool CDkeyPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init())
    {
        setIsHDPanel(true);
        auto tmpCCB = CCBLoadFile("CDkey", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        
        auto sprite9 = CCLoadSprite::createScale9Sprite("UI_chat_text.png");
        sprite9->setInsetBottom(2);
        sprite9->setInsetLeft(2);
        sprite9->setInsetRight(2);
        sprite9->setInsetRight(2);
        m_editBox = CCEditBox::create(CCSizeMake(510,48),sprite9);
        m_editBox->setMaxLength(30);
        m_editBox->setFontSize(22);
        m_editBox->setFontColor(ccBLACK);
        m_editBox->setReturnType(kKeyboardReturnTypeDone);
        m_editBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
        m_editNode->addChild(m_editBox);
        m_editBox->setPlaceHolder(_lang("101273").c_str());
        CCCommonUtils::setButtonTitle(m_btnOk, _lang("101274").c_str());
        m_btnOk->setSwallowsTouches(false);
        m_titleTxt->setString(_lang("101272"));
        m_descTxt->setString(_lang("101280"));
        this->schedule(schedule_selector(CDkeyPopUpView::checkCanUse));
        ret = true;
    }
    return ret;
    
}

void CDkeyPopUpView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void CDkeyPopUpView::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

void CDkeyPopUpView::onBtnOkClick(CCObject *pSender, CCControlEvent event){
    string code = m_editBox->getText();
    if (code=="") return ;
    CDkeyRewardCommand* cmd = new CDkeyRewardCommand(code);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CDkeyPopUpView::getRewardCallBack), NULL));
    cmd->sendAndRelease();
}

void CDkeyPopUpView::getRewardCallBack(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    string helpInfo = "";
    if(dic){
        int result = -1;
        if(dic->objectForKey("result")){
            result = dic->valueForKey("result")->intValue();
        }
        string tip = "";
        switch (result) {
            case 0:
                tip = _lang("101279");//"兑换成功";
                break;
            case 1:
                tip = _lang("101276");//"无效";
                break;
            case 2:
                tip = _lang("101278");//"领过了";
                break;
            case 3:
                tip = _lang("101277");//"已经使用";
                break;
            case 4:
                tip = _lang("101299");//"无效";
                break;
            case 5:
                tip = _lang("113013");//"你不可使用这批激活码";
                break;
        }
        if(tip!=""){
            CCCommonUtils::flyHint("", "", tip);
        }
    }
    this->closeSelf();
}

bool CDkeyPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_background", CCScale9Sprite*, this->m_background);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, this->m_btnOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, this->m_editNode);
    return true;
}

SEL_CCControlHandler CDkeyPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnOkClick", CDkeyPopUpView::onBtnOkClick);
    return NULL;
}

bool CDkeyPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void CDkeyPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_background, pTouch)) {
        if(isTouchInside(m_btnOk, pTouch) && m_btnOk->isEnabled()){
            onBtnOkClick(NULL,Control::EventType::TOUCH_DOWN);
        }
        //todo
    }else{
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void CDkeyPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

CDkeyPopUpView::~CDkeyPopUpView()
{

}

CDkeyPopUpView::CDkeyPopUpView(){
    
}

void CDkeyPopUpView::checkCanUse(float _time){
    string code = m_editBox->getText();
    m_btnOk->setEnabled(code!="");
}
