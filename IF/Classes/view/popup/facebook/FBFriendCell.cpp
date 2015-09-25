//
//  FBFriendCell.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-19.
//
//

#include "FBFriendCell.h"
#include "LoadWebImg.h"
#include "FBUtilies.h"
#include "PopupViewController.h"
#include "SocialController.h"
#include "ParticleController.h"
#include "FacebookInviteView.h"
#include "FBMineHelpCommand.h"
#include "CCShake.h"
#include "BaoZangView.h"
#include "ActivityController.h"

bool FBFriendCell::init()
{
    bool ret = true;
    
    if (CCTableViewCell::init()) {
        m_ccbNode = CCBLoadFile("FBFriendCell", this, this);
        this->setContentSize(m_ccbNode->getContentSize());
        m_eventNode->setVisible(false);
        m_helpTxt->setString("!");
        m_helpNode->setVisible(false);
        return true;
    }
    
    return ret;
}

void FBFriendCell::setData(FBFriendInfo* info,CCNode* clickArea)
{
    m_info = info;
    m_clickArea = clickArea;
    m_icon->removeAllChildrenWithCleanup(true);
    m_nameTxt->setString("");
    m_txt1->setString("");
    m_txt2->setString("");
    m_defIcon->setVisible(false);
    m_helpNode->setVisible(info->help==1);
    if(m_info!=NULL && !m_info->add){
        m_defIcon->setVisible(true);
        LoadWebImg* img = LoadWebImg::create();
        img->setLoadURL(m_info->url.c_str());
        img->setScale(1.5);
        m_icon->addChild(img);
        string name = m_info->name;
        if(name.length()>9){
            name = CCCommonUtils::subStrByUtf8(name, 0, 8);
            name.append("...");
        }
        m_nameTxt->setString(name.c_str());
    }else if(m_info!=NULL && m_info->add){
        CCSprite* spr = CCLoadSprite::createSprite("icon_facebook_invite.png");
        m_icon->addChild(spr);
        m_nameTxt->setString("");
        m_txt1->setString(_lang("107034"));
        m_txt2->setString(_lang("107035"));
        
    }
}

void FBFriendCell::onEnter(){
    CCNode::onEnter();
   
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FBFriendCell::refreshHelpData), FB_REFRESH_FRIEND_DATA, NULL);
}

void FBFriendCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FB_REFRESH_FRIEND_DATA);
    CCNode::onExit();
}

void FBFriendCell::onGoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    
}

SEL_CCControlHandler FBFriendCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    
    return NULL;
}

bool FBFriendCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eventNode", CCNode*, this->m_eventNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCScale9Sprite*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eventTxt", CCLabelIF*, this->m_eventTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defIcon", CCNode*, this->m_defIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNode", CCNode*, this->m_helpNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpTxt", CCLabelIF*, this->m_helpTxt);
    return false;
}

bool FBFriendCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickNode, pTouch)){
        m_clickNode->setColor(ccc3(128,128,128));
        return true;
    }
    return false;
}

void FBFriendCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(BaoZangView::getTipFlag()) return ;
    CCPoint pos = pTouch->getLocation();
    m_clickNode->setColor(ccc3(255,255,255));
    string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if(isTouchInside(m_clickNode, pTouch)){
        if (_fbUid=="" || !FBUtilies::fbIsLogin()){
            CCCommonUtils::flyHint("", "", _lang("107072"));
            return ;
        }
    }
    if (isTouchInside(m_clickNode, pTouch) && isTouchInside(m_clickArea, pTouch) && m_info && m_info->add && fabs(pos.x - m_touchPos.x) <30) {
        //PopupViewController::getInstance()->addPopupInView(FacebookInviteView::create());
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(FB_OPEN_INVITE_VIEW);
    }else if(isTouchInside(m_clickNode, pTouch) && isTouchInside(m_clickArea, pTouch)  && fabs(pos.x - m_touchPos.x) <30){
        if(m_info->help==1){
            string m_facebookName = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERNAME, "");
            FBMineHelpCommand* cmd = new FBMineHelpCommand(m_info->server,m_info->gUid,m_facebookName);
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(FBFriendCell::helpSuccess), NULL));
            cmd->sendAndRelease();
            m_info->help = 0;
            refreshHelpData(NULL);
            if (ActivityController::getInstance()->baozangHelpArr && ActivityController::getInstance()->baozangHelpArr->count()>0) {
                int num = ActivityController::getInstance()->baozangHelpArr->count();
                for (int i=0; i<num; i++) {
                    auto oneInfo = dynamic_cast<CCDictionary*>(ActivityController::getInstance()->baozangHelpArr->objectAtIndex(i));
                    if (oneInfo) {
                        string fId = oneInfo->valueForKey("fId")->getCString();
                        if (fId==m_info->id) {
                            oneInfo->setObject(CCString::create("0"), "help");
                        }
                    }
                }
            }
        }else{
            CCCommonUtils::flyHint("", "", _lang_1("107079",m_info->name.c_str()));
        }
    }
}

void FBFriendCell::helpSuccess(CCObject* param){
    CCCommonUtils::flyHint("", "", _lang_1("107074",m_info->name.c_str()));
}

void FBFriendCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void FBFriendCell::refreshHelpData(CCObject* param){
    m_helpNode->setVisible(m_info->help==1);
}
