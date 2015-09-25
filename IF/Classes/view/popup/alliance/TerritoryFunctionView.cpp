//
//  TerritoryFunctionView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/5/19.
//
//

#include "TerritoryFunctionView.h"
#include "PopupViewController.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "WorldController.h"
#include "ParticleController.h"
TerritoryFunctionView* TerritoryFunctionView::create(int stat){
    TerritoryFunctionView* ret = new TerritoryFunctionView(stat);
    if(ret && ret->init(stat)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool TerritoryFunctionView::init(int stat)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    auto tmpCCB = CCBLoadFile("TerritoryFunctionView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    int oldBgHeight = m_viewBg->getContentSize().height;
    changeBGHeight(m_viewBg);
    int newBgHeight = m_viewBg->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    if (CCCommonUtils::isIosAndroidPad()) {
        addHeight = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    
    m_descriptionText->setString(_lang("115372"));
    int textH = m_descriptionText->getContentSize().height * m_descriptionText->getOriginScaleY();
    m_descriptionText->setPositionY(m_descriptionText->getPositionY() - textH / 2);
    if (!CCCommonUtils::isIosAndroidPad()) {
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight-20);//
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight + addHeight - textH + 60));
        
        m_viewBg2_py=m_infoList->getPositionY()+m_infoList->getContentSize().height+20;
        m_viewBg2->setPositionY(m_viewBg2_py);//350
    } else {
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height - (textH - 49) + addHeight));
        m_infoList->setPositionY(m_infoList->getPositionY() - addHeight);
        m_viewBg2->setPositionY(m_viewBg2->getPositionY() - (textH - 49));
    }
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);

    m_stat = stat;
    int i = 0;
    CCDictElement *element;
    auto dictEffect = LocalController::shared()->DBXMLManager()->getGroupByKey("territory_effect");
    CCDICT_FOREACH(dictEffect, element){
        std::string effectId = element->getStrKey();
        std::string buffStr = CCCommonUtils::getPropById(effectId, "k2");
        std::string descriptionStr = CCCommonUtils::getPropById(effectId, "description");
        std::string titleStr = CCCommonUtils::getPropById(effectId, "title");
        TerritoryFunctionCell* cell = TerritoryFunctionCell::create(buffStr, descriptionStr, titleStr, effectId, m_stat);
        cell->setAnchorPoint(ccp(0.5, 0.5));
        cell->setPosition(ccp(0,i*160));
        if (CCCommonUtils::isIosAndroidPad()) {
            cell->setPositionY(i * 265);
        }
        m_scrollView->addChild(cell);
        i++;
    }

    this->setTitleName(_lang("115371"));
    
    int contentHeight = i*160;
    if (CCCommonUtils::isIosAndroidPad()) {
        contentHeight = i * 265;
    }
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,contentHeight));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - contentHeight));
    
    m_scrollH = contentHeight;
    return true;
}

void TerritoryFunctionView::onEnter()
{
    CCNode::onEnter();
}

void TerritoryFunctionView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler TerritoryFunctionView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool TerritoryFunctionView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descriptionText", CCLabelIF*, this->m_descriptionText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg2", CCNode*, this->m_viewBg2);
    return false;
}

TerritoryFunctionCell* TerritoryFunctionCell::create(std::string buff, std::string description, std::string title, std::string index, int stat)
{
    auto ret = new TerritoryFunctionCell(buff,description,title,index,stat);
    if (ret && ret->init(buff,description,title,index,stat)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TerritoryFunctionCell::init(std::string buff, std::string description, std::string title, std::string index, int stat)
{
    CCLoadSprite::doResourceByCommonIndex(504, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
    });
    bool ret = true;
    CCBLoadFile("TerritoryFunctionCell",this,this);
    setData(buff,description,title,index,stat);
    return ret;
}

void TerritoryFunctionCell::setData(std::string buff, std::string description, std::string title, std::string index, int stat)
{
    m_buff = buff;
    m_description = description;
    m_title = title;
    m_index = index;
    m_stat = stat;
    m_picNode->removeAllChildren();
    
    m_nameLabel->setString(_lang(m_title));
    m_desLabel->setString(_lang_1(m_description, m_buff.c_str()));
    std::string picStr = "";
    if (m_index == "45000") {
        picStr = "gather_buff.png";
    }
    else if (m_index == "45001") {
        picStr = "resource_buff.png";
    }
    else if (m_index == "45002") {
        picStr = "storage_buff.png";
    }
    else if (m_index == "45003") {
        picStr = "def_buff.png";
    }
    auto pic = CCLoadSprite::createSprite(picStr.c_str());
    if (pic) {
        if (m_stat == 0 || m_stat == 1) {
            CCCommonUtils::setSpriteGray(pic, true);
        }
        m_picNode->addChild(pic);
    }
}

void TerritoryFunctionCell::onEnter() {
    CCNode::onEnter();
}

void TerritoryFunctionCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler TerritoryFunctionCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool TerritoryFunctionCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget,const char *pMemberVariableName,cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    return false;
}