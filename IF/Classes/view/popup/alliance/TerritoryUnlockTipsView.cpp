//
//  TerritoryUnlockTipsView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/6/1.
//
//

#include "TerritoryUnlockTipsView.h"
#include "ScienceController.h"
TerritoryUnlockTipsView* TerritoryUnlockTipsView::create(std::string name, int count) {
    auto ret = new TerritoryUnlockTipsView(name,count);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TerritoryUnlockTipsView::init() {
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(504, false);
        });
        
        auto ccb = CCBLoadFile("TerritoryUnlockTipsView",this,this);
        this->setContentSize(ccb->getContentSize());
        
        m_titleLabel->setString(m_name);
        m_tipLabel->setString(_lang("115398"));
        
        m_currentNum = GlobalData::shared()->playerInfo.allianceInfo.currentNum;
        m_totalForce = GlobalData::shared()->playerInfo.allianceInfo.totalForce;
        m_scienceLevel = ScienceController::getInstance()->getTotalAllScPoints();
        
        CCDictElement *element;
        auto dictEffect = LocalController::shared()->DBXMLManager()->getGroupByKey("territory");
        CCDICT_FOREACH(dictEffect, element){
            std::string territoryId = element->getStrKey();
            int countTemp = atoi(territoryId.c_str());
            if (countTemp == m_count+6000) {///////////
                std::string condition1 = CCCommonUtils::getPropById(territoryId, "condition1");
                std::string condition2 = CCCommonUtils::getPropById(territoryId, "condition2");
                std::string condition3 = "1";
                if (m_count != 1 && m_count != 2) {
                    condition3 = CCCommonUtils::getPropById(territoryId, "condition3");
                }
                
                m_lock1Label->setString(_lang_1("115395", condition1.c_str()));
                m_lock2Label->setString(_lang_1("115396", condition2.c_str()));
                m_lock3Label->setString(_lang_1("115397", condition3.c_str()));
                
                std::string strTemp = "";
                double temp = 0;
                double s = 0.0f;
                strTemp = CC_ITOA(m_currentNum);
                strTemp.append("/");
                strTemp.append(condition1);
                m_progressTxt1->setString(strTemp.c_str());
                temp = atoi(condition1.c_str());
                if (temp != 0) {
                    s = m_currentNum / temp;
                    s = MAX(0.0f, s);
                    s = MIN(1.0f, s);
//                    m_bar1->setScaleX(s);
//                    m_lbar1->setVisible(s>0.00f);
//                    m_rbar1->setVisible(s>0.99f);
                    m_progress1->setPreferredSize(CCSize(248 * s, 20));
                    m_progress1->setVisible(s > 0.01);
                }
                
                strTemp = CC_ITOA(m_totalForce);
                strTemp.append("/");
                strTemp.append(condition2);
                m_progressTxt2->setString(strTemp.c_str());
                temp = atoi(condition2.c_str());
                if (temp != 0) {
                    s = m_totalForce / temp;
                    s = MAX(0.0f, s);
                    s = MIN(1.0f, s);
//                    m_bar2->setScaleX(s);
//                    m_lbar2->setVisible(s>0.00f);
//                    m_rbar2->setVisible(s>0.99f);
                    m_progress2->setPreferredSize(CCSize(248 * s, 20));
                    m_progress2->setVisible(s > 0.01);
                }
                
                strTemp = CC_ITOA(m_scienceLevel);
                strTemp.append("/");
                strTemp.append(condition3);
                m_progressTxt3->setString(strTemp.c_str());
                temp = atoi(condition3.c_str());
                if (temp != 0) {
                    s = m_scienceLevel / temp;
                    s = MAX(0.0f, s);
                    s = MIN(1.0f, s);
//                    m_bar3->setScaleX(s);
//                    m_lbar3->setVisible(s>0.00f);
//                    m_rbar3->setVisible(s>0.99f);
                    m_progress3->setPreferredSize(CCSize(248 * s, 20));
                    m_progress3->setVisible(s > 0.01);
                }
                
                std::string picStr = "";
                picStr = "alliance_currentnum.png";
                auto pic1 = CCLoadSprite::createSprite(picStr.c_str());
                CCCommonUtils::setSpriteMaxSize(pic1, 80);
                pic1->setAnchorPoint({.5, 0});
                m_icon1->addChild(pic1);

                picStr = "alliance_totalforce.png";
                auto pic2 = CCLoadSprite::createSprite(picStr.c_str());
                CCCommonUtils::setSpriteMaxSize(pic2, 80);
                pic2->setAnchorPoint({.5, 0});
                m_icon2->addChild(pic2);
                
                picStr = "allianceScience.png";
                auto pic3 = CCLoadSprite::createSprite(picStr.c_str());
                CCCommonUtils::setSpriteMaxSize(pic3, 80);
                pic3->setAnchorPoint({.5, 0});
                m_icon3->addChild(pic3);
                
                if (m_count == 1 || m_count == 2) {
                     m_node3->setVisible(false);
                }
                
                break;
            }
        }

        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            // detect touch inside panel
            if (!isTouchInside(m_touchNode, pTouch)) {
                closeSelf();
            }
        });
        
        return true;
        
    }
    return false;
}

bool TerritoryUnlockTipsView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipLabel", CCLabelIF*, this->m_tipLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode)
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon1", CCNode*, this->m_icon1)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lock1Label", CCLabelIF*, this->m_lock1Label)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress1", CCScale9Sprite*, this->m_progress1)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt1", CCLabelIF*, this->m_progressTxt1)
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon2", CCNode*, this->m_icon2)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lock2Label", CCLabelIF*, this->m_lock2Label)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress2", CCScale9Sprite*, this->m_progress2)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt2", CCLabelIF*, this->m_progressTxt2)
  
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node3", CCNode*, this->m_node3)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon3", CCNode*, this->m_icon3)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lock3Label", CCLabelIF*, this->m_lock3Label)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress3", CCScale9Sprite*, this->m_progress3)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt3", CCLabelIF*, this->m_progressTxt3)
    
    return false;
}

SEL_CCControlHandler TerritoryUnlockTipsView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;}

void TerritoryUnlockTipsView::onEnter() {
    PopupBaseView::onEnter();
//    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
//    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);//
}

void TerritoryUnlockTipsView::onExit() {
//    setTouchEnabled(false);
    PopupBaseView::onExit();
}