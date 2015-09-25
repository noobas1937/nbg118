//
//  EvaluationView.cpp
//  IF
//
//  Created by chenfubi on 15/3/12.
//
//

#include "EvaluationView.h"
#include "PopupViewController.h"
#include "../../Ext/CCDevice.h"


EvaluationView* EvaluationView::create()
{
	EvaluationView* ret = new EvaluationView();
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

bool EvaluationView::init()
{
	bool bRet = false;
	do
	{
        
        if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
        auto ccb = CCBLoadFile("EvaluationView.ccbi", this, this);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        m_infoNode->setPositionY(winSize.height/2 - ccb->getContentSize().height/2);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_infoNode->setPositionX(winSize.width / 2.0 - ccb->getContentSize().width / 2.0);
        }
        
        m_titileTTF->setString(_lang("113989"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        m_infoTTF->setString(_lang("113990"));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        m_infoTTF->setString(_lang("113991"));
#endif
        m_btnTTF0->setString(_lang("113992"));
        m_btnTTF1->setString(_lang("113993"));
        m_btnTTF2->setString(_lang("113994"));
        
		
		bRet = true;
	}while(0);
	return bRet;
}

void EvaluationView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void EvaluationView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool EvaluationView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void EvaluationView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) > (CCCommonUtils::isIosAndroidPad()? 20 : 10))
    {
        return;
    }
    if (!isTouchInside(m_touchNode, pTouch))
    {
        PopupViewController::getInstance()->removePopupView(this);
        return;
    }
    if (isTouchInside(m_touchNode0, pTouch))
    {
        onBtnClick(0);
    }
    else if (isTouchInside(m_touchNode1, pTouch))
    {
        onBtnClick(1);
    }
    else if (isTouchInside(m_touchNode2, pTouch))
    {
        onBtnClick(2);
    }
}

void EvaluationView::onBtnClick(int type)
{
    switch (type)
    {
        case 0:
            sendCmd();
            break;
            
        case 2:
            cocos2d::extension::CCDevice::updateVersion(GlobalData::shared()->downloadUrl.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
#endif
            GlobalData::shared()->isOpenEvaluate = false;
            break;
            
        case 1:
        default:
            break;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

void EvaluationView::sendCmd()
{
    EvaluationCmd* cmd = new EvaluationCmd();
    cmd->sendAndRelease();
    GlobalData::shared()->isOpenEvaluate = false;
}

bool EvaluationView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titileTTF", CCLabelIF*, m_titileTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF", CCLabelIF*, m_infoTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTTF0", CCLabelIF*, m_btnTTF0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTTF1", CCLabelIF*, m_btnTTF1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTTF2", CCLabelIF*, m_btnTTF2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode0", CCNode*, m_touchNode0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode1", CCNode*, m_touchNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode2", CCNode*, m_touchNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, m_infoNode);
    return false;
}

SEL_CCControlHandler EvaluationView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBuyBtnClick", LotteryActView::onBuyBtnClick);
    return NULL;
}
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//https://play.google.com/store/apps/details?id=air.com.elextech.happyfarm
//itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=

bool EvaluationCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(EVALUATEIONCMD) != 0)
    {
        return false;
    }
    return true;
}
