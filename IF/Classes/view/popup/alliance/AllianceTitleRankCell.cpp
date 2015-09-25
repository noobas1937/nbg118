//
//  AllianceTitleRankCell.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-25.
//
//

#include "AllianceTitleRankCell.h"

bool AllianceTitleRankCell::init()
{
    bool ret = true;
    
    if (CCNode::init()) {
        CCNode* node = CCBLoadFile("AllianceTitleRankCell", this, this);
        CCSize size = node->getContentSize();
        setContentSize(size);
        return true;
    }
    return ret;
}

void AllianceTitleRankCell::setData(int rank,std::string title,CCArray* array){
    m_rank = rank;
    m_title = title;
    m_data = array;
    m_rankTxt->setString(CC_ITOA(m_rank));
    m_titleTxt->setString(m_title);
}

void AllianceTitleRankCell::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void AllianceTitleRankCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AllianceTitleRankCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(isTouchInside(m_arrow, pTouch)){
        
    }
    return true;
}

void AllianceTitleRankCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void AllianceTitleRankCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

SEL_CCControlHandler AllianceTitleRankCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onManageBtnClick", AllianceTitleRankCell::onManageBtnClick);
    return NULL;
}

bool AllianceTitleRankCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrow", CCNode*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt", CCLabelIF*, this->m_rankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    return false;
}
