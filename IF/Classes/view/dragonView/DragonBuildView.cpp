//
//  DragonBuildView.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonBuildView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "BuildMoreInfoView.h"
#include "ToolController.h"
#include "StoreView.h"
#include "UseResToolView.h"
#include "ArmyController.h"
#include "TipsView.h"
#include "BuildUpgradeView.h"
#include "QueueController.h"
#include "TroopsController.h"

DragonBuildView* DragonBuildView::create(int buildId){
    DragonBuildView* ret = new DragonBuildView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonBuildView::init(int buildId)
{
    if (!ArcPopupBaseView::init(TYPE_POS_TO_UP)) {
        return false;
    }
    m_qid = QID_MAX;
    m_buildId = buildId;
    auto tmpCCB = CCBLoadFile("ResBuildView02",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    m_upBtnMsgLabel->setString(_lang("102105").c_str());//more info
    m_btnMsgLabel->setString(_lang("102121").c_str());//destroy
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    string key = CC_ITOA(m_buildId);
    int qid = QueueController::getInstance()->getQueueQidByKey(key);
    if (qid > 0) {
        m_qid = qid;
    }
    
    updateInfo();
    
    m_openNum=0;
    
    vector<CCNode*> tmpVec;
    CCCommonUtils::getRGBAProtocols(m_mainNode, &tmpVec);
    for (int i=0; i<tmpVec.size(); i++) {
        CCActionInterval * fadeIn = CCFadeIn::create(0.5);
        tmpVec[i]->runAction(fadeIn);
    }
    
    return true;
}

void DragonBuildView::updateInfo()
{
    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    m_nameLabel->setString(_lang(m_info.name));
    string title = _lang_1("102272", CC_ITOA(m_info.level));
    if(m_info.level>=GlobalData::shared()->MaxBuildLv) {
        title += _lang("102183");
    }
    m_lvLabel->setString(title);
    m_nextLvLabel->setString("");
    m_nextLabel->setString("");
    m_nextMsgLabel->setString(_lang(m_info.description));
    float fh = m_nextMsgLabel->getContentSize().height;
    if (fh>345) {
        m_nextMsgLabel->setFontSize(16);
    }
    
    int curX = 30;
    int curY = 0;
    int _itemH = 60;//110
    string addMsg = "";
    
    if (m_info.type == FUN_BUILD_ARROW_TOWER) {
        vector<string> vecItems;
        CCCommonUtils::splitString(m_info.information, "|", vecItems);
        string tmpAtk = "";
        string tmpSpd = "";
        if (m_info.level < vecItems.size()) {
            string curMsg = vecItems[m_info.level];
            vector<string> cellItems;
            CCCommonUtils::splitString(curMsg, ";", cellItems);
            if (cellItems.size()>=3) {
                string atkStr = cellItems[1];
                string spdStr = cellItems[2];
                
                vector<string> tinyItem1;
                CCCommonUtils::splitString(atkStr, ",", tinyItem1);
                if (tinyItem1.size()==2) {
                    tmpAtk = _lang_1(tinyItem1[0],CC_CMDITOA(atoi(tinyItem1[1].c_str())).c_str()  );
                }
                
                vector<string> tinyItem2;
                CCCommonUtils::splitString(spdStr, ",", tinyItem2);
                if (tinyItem2.size()==2) {
                    tmpSpd = _lang_1(tinyItem2[0],CC_CMDITOA(atoi(tinyItem2[1].c_str())).c_str()  );
                }
            }
        }
        
        auto item2 = BuildResCell::create(Wood, _lang("102260"), 100, "attack_icon.png", tmpSpd); //攻速提升
        item2->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        auto item1 = BuildResCell::create(Wood, _lang("102259"), 100, "attack_icon.png", tmpAtk); //攻击提升
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        curY -= 20;
    }
    else if (m_info.type == FUN_BUILD_WAR) {
        auto label = CCLabelIF::create(_lang("108648").c_str(), 24, CCSizeMake(600, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        label->setColor(ccWHITE);
        curY += 100;
        int labelW = label->getContentSize().width*label->getOriginScaleX();
        label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
        m_scrollView->addChild(label);
        curY += _itemH;
        curY -= 20;
    }
    else {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        if (qInfo.type == TYPE_BUILDING) {
            if (m_info.state == FUN_BUILD_CREATE) {
                m_btnMsgLabel->setString(_lang("135002"));
            }else {
                m_btnMsgLabel->setString(_lang("135000"));
                float fw = m_btnMsgLabel->getContentSize().width;
                if (fw>300) {//防爆框
                    m_btnMsgLabel->setScaleX(300.0/fw);
                }
            }
        }else if (qInfo.type == TYPE_SCIENCE) {
            m_btnMsgLabel->setString(_lang("135003"));
        }else if (qInfo.type == TYPE_HOSPITAL) {
            m_btnMsgLabel->setString(_lang("135004"));
        }else {
            m_btnMsgLabel->setString(_lang("135001"));
            if (m_info.type == FUN_BUILD_FORT) {
                m_btnMsgLabel->setString(_lang("135002"));
            }
        }
    }
}

void DragonBuildView::onEnter()
{
    CCNode::onEnter();
}

void DragonBuildView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler DragonBuildView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoreClick", DragonBuildView::onMoreClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDestroyClick", DragonBuildView::onDestroyClick);
    return NULL;
}

bool DragonBuildView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, this->m_instantBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_destroyNode", CCNode*, this->m_destroyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreNode", CCNode*, this->m_moreNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLabel", CCLabelIF*, this->m_nextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLvLabel", CCLabelIF*, this->m_nextLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextMsgLabel", CCLabelIF*, this->m_nextMsgLabel);
    return false;
}

void DragonBuildView::onMoreClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(BuildMoreInfoView::create(m_buildId));
}

void DragonBuildView::onDestroyClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    if (m_qid != QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        if (qInfo.finishTime <= GlobalData::shared()->getWorldTime()) {
            onResetDesBtn();
        }else {
            string tmpMsg = "";
            if (qInfo.type == TYPE_BUILDING) {
                if (m_info.state == FUN_BUILD_CREATE) {
                    tmpMsg = _lang("135007");
                }else {
                    tmpMsg = _lang("135005");
                }
            }else if (qInfo.type == TYPE_SCIENCE) {
                tmpMsg = _lang("135008");
            }else if (qInfo.type == TYPE_HOSPITAL) {
                tmpMsg = _lang("135009");
            }else {
                tmpMsg = _lang("135006");
                if (m_info.type == FUN_BUILD_FORT) {
                    tmpMsg = _lang("135007");
                }
            }
            YesNoDialog::show( tmpMsg.c_str() , CCCallFunc::create(this, callfunc_selector(DragonBuildView::onFirstCancelQueue)));
        }
    }else {
        if (m_info.state == FUN_BUILD_NORMAL && m_info.type == FUN_BUILD_MAIN) {
            PopupViewController::getInstance()->removeAllPopupView();
            FunBuildController::getInstance()->OpenMoveBuild();
        }else {
            YesNoDialog::show( _lang("102182").c_str() , CCCallFunc::create(this, callfunc_selector(DragonBuildView::onYesDestroy)));
        }
    }
}

void DragonBuildView::onYesDestroy()
{
    if (FunBuildController::getInstance()->startDestroyBuild(m_buildId) )
    {
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

void DragonBuildView::onFirstCancelQueue()
{
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    if (qInfo.finishTime <= GlobalData::shared()->getWorldTime()) {
        onResetDesBtn();
    }else {
        YesNoDialog::show( _lang("102195").c_str() , CCCallFunc::create(this, callfunc_selector(DragonBuildView::onLastCancelQueue)));
    }
}

void DragonBuildView::onLastCancelQueue()
{
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    if (qInfo.finishTime <= GlobalData::shared()->getWorldTime()) {
        onResetDesBtn();
    }else {
        QueueController::getInstance()->startCancelQueue(m_qid);
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void DragonBuildView::onResetDesBtn()
{
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    string tmpMsg = "102290";
    if (qInfo.type == TYPE_BUILDING) {
        tmpMsg = "103679";
    }else if (qInfo.type == TYPE_SCIENCE) {
        tmpMsg = "102289";
    }else if (qInfo.type == TYPE_HOSPITAL) {
        tmpMsg = "103677";
    }else if (qInfo.type == TYPE_FORT) {
        tmpMsg = "103675";
    }else if (qInfo.type == TYPE_FORCE || qInfo.type == TYPE_RIDE_SOLDIER || qInfo.type == TYPE_BOW_SOLDIER || qInfo.type == TYPE_CAR_SOLDIER) {
        tmpMsg = "103673";
    }
    CCCommonUtils::flyHint("", "", _lang(tmpMsg));//提示该队列已经完成
    
    m_qid = QID_MAX;
    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    if (m_info.limitNum <= 1) {
        m_destroyNode->setVisible(false);
        m_instantBtn->setEnabled(false);
        m_moreNode->setPositionX(0);
    }
    else {
        if (m_info.state != FUN_BUILD_NORMAL) {
            m_instantBtn->setEnabled(false);
        }
    }
    m_btnMsgLabel->setString(_lang("102121").c_str());
}