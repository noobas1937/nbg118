//
//  ActivityView.cpp
//  IF
//
//  Created by xxrdsg on 15-6-3.
//
//

#include "ActivityView.h"
#include "LuaController.h"
#include "ActivityExcView.h"
#include "PopupViewController.h"

ActivityView* ActivityView::create(ActivityEventObj * obj)
{
    auto ret = new ActivityView(obj);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    if (m_obj->type != 6) return false;
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCLoadSprite::doResourceByCommonIndex(506, true);
    auto size = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(CCSize(640, 852));
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setContentSize(CCSize(1536, 2048));
    }
    CCBLoadFile("ActivityView", this, this);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - (size.height - 2048));
        m_listNode->setPositionY(m_listNode->getPositionY() - (size.height - 2048));
        m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + size.height - 2048));
    } else {
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - (size.height - 852));
        m_listNode->setPositionY(m_listNode->getPositionY() - (size.height - 852));
        m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + size.height - 852));
    }

    if (m_obj->exchange == "1" || m_obj->exchange == "2") {
        m_bottomNode->setVisible(true);
    } else {
        m_bottomNode->setVisible(false);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_listNode->setPositionY(m_listNode->getPositionY() - 178);
            m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + 178));
        } else {
            m_listNode->setPositionY(m_listNode->getPositionY() - 92);
            m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + 92));
        }
    }
    m_titleLabel->setString(m_obj->name);
    m_infoLabel->setString(_lang(m_obj->story));
    if (m_infoLabel->getContentSize().height * m_infoLabel->getOriginScaleY() <= m_listNode->getContentSize().height) {
        m_labelNode->getParent()->removeChild(m_labelNode, false);
        m_labelNode->setPosition(ccp(0, m_listNode->getContentSize().height / 2 - m_infoLabel->getContentSize().height * m_infoLabel->getOriginScaleY() / 2));
        m_listNode->addChild(m_labelNode);
    } else {
        auto scrollView = CCScrollView::create(m_listNode->getContentSize());
        scrollView->setDirection(kCCScrollViewDirectionVertical);
        scrollView->setAnchorPoint(ccp(0, 0));
        scrollView->setContentSize(CCSize(m_listNode->getContentSize().width, m_infoLabel->getContentSize().height * m_infoLabel->getOriginScaleY()));
        m_labelNode->getParent()->removeChild(m_labelNode, false);
        m_labelNode->setPosition(ccp(0, 0));
        scrollView->addChild(m_labelNode);
        scrollView->setPosition(ccp(0, 0));
        m_listNode->addChild(scrollView);
        scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - scrollView->getContentSize().height));
    }
    
    CCCommonUtils::setButtonTitle(m_rwdBtn, _lang("133076").c_str());
    
    
    
    bool st = false;
    if (LuaController::getInstance()->checkActLuaValid(m_obj))
    {
        CCNode* node = LuaController::getInstance()->createActivityAdCell(m_obj);
        if (node && node->getChildrenCount()) {
            m_picNode->addChild(node);
            if (CCCommonUtils::isIosAndroidPad())
            {
                float w = (1536.0 - 1156.0) / 2.0;
                float h = 645;
                node->setContentSize(CCSize(0, 0));
                node->setScaleX(1156.0 / 640.0);
                node->setScaleY(645.0 / 357.0);
                auto layer1 = CCModelLayerColor::create();
                layer1->setAnchorPoint(ccp(0, 0));
                layer1->setContentSize(CCSize(w, h));
                layer1->setPosition(-w, 0);
                layer1->setOpacity(255);
                layer1->setColor(ccBLACK);
//                layer1->setTouchPriority(-999);
                m_picNode->addChild(layer1);
                
                layer1 = CCModelLayerColor::create();
                layer1->setAnchorPoint(CCPointZero);
                layer1->setPosition(ccp(1156, 0));
                layer1->setContentSize(CCSize(w, h));
                layer1->setOpacity(255);
                layer1->setColor(ccBLACK);
//                layer1->setTouchPriority(-999);
                m_picNode->addChild(layer1);
            }
        } else {
            st = true;
        }
    }
    else
    {
        st = true;
    }
    
    if (st) {
        auto nameTTF = CCLabelIF::create(m_obj->name.c_str());
        auto infoTTF = CCLabelIF::create(m_obj->desc.c_str());
        auto bgImg = CCLoadSprite::createSprite("activity_ad_beiyong.png");
        
        nameTTF->setPosition(ccp(16,245));
        nameTTF->setAnchorPoint(ccp(0, 1));
        nameTTF->setFontSize(36);
        nameTTF->setColor({255,219,117});
        nameTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
        nameTTF->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        nameTTF->setDimensions(CCSize(0, 0));
        
        infoTTF->setPosition(ccp(16,201));
        infoTTF->setAnchorPoint(ccp(0, 1));
        infoTTF->setColor({0, 249, 0});
        infoTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
        infoTTF->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        infoTTF->setDimensions(CCSize(300, 0));
        
        
        bgImg->setAnchorPoint(ccp(0, 0));
        bgImg->setPosition(ccp(0, 0));
        auto node = CCNode::create();
        node->addChild(bgImg);
        node->addChild(nameTTF);
        node->addChild(infoTTF);
        m_picNode->addChild(node);
        if (CCCommonUtils::isIosAndroidPad()) {
            float w = (1536.0 - 1156.0) / 2.0;
            float h = 645;
            node->setContentSize(CCSize(0, 0));
            node->setScaleX(1156.0 / 640.0);
            node->setScaleY(645.0 / 357.0);
            auto layer1 = CCModelLayerColor::create();
            layer1->setAnchorPoint(ccp(0, 0));
            layer1->setContentSize(CCSize(w, h));
            layer1->setPosition(-w, 0);
            layer1->setOpacity(255);
            layer1->setColor(ccBLACK);
            //                layer1->setTouchPriority(-999);
            m_picNode->addChild(layer1);
            
            layer1 = CCModelLayerColor::create();
            layer1->setAnchorPoint(CCPointZero);
            layer1->setPosition(ccp(1156, 0));
            layer1->setContentSize(CCSize(w, h));
            layer1->setOpacity(255);
            layer1->setColor(ccBLACK);
            //                layer1->setTouchPriority(-999);
            m_picNode->addChild(layer1);
        }
    }
    
    if (m_obj->exchange == "1" || m_obj->exchange == "2") {
        if (ActivityController::getInstance()->m_exc2Data.find(m_obj->id) == ActivityController::getInstance()->m_exc2Data.end()) {
            ActivityController::getInstance()->startGetAct6Data();
        } else {
            if (!ActivityController::getInstance()->checkIsToday(ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshTime)) {
                ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshTime = GlobalData::shared()->getTimeStamp();
                ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refresh = 0;
            }
        }
    }

    return true;
}

void ActivityView::onEnter()
{
    CCNode::onEnter();
    setTitleName(m_obj->name);
}

void ActivityView::onExit()
{
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCNode::onExit();
}

bool ActivityView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCLayerColor*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_labelNode", CCNode*, this->m_labelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, this->m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCLayerColor*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdBtn", CCControlButton*, this->m_rwdBtn);
    return false;
}

SEL_CCControlHandler ActivityView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRwdBtn", ActivityView::onClickRwdBtn);
    return NULL;
}

void ActivityView::onClickRwdBtn(CCObject *pSender, CCControlEvent event)
{
//    if (m_obj->exchange != "1" && m_obj->exchange != "2") {
//        return;
//    }
    if (m_obj->exchange == "2") {
        PopupViewController::getInstance()->addPopupView(ActivityExcNewView::create(m_obj));
    }
}
