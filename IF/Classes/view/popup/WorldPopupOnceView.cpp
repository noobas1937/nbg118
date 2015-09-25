//
//  WorldPopupOnceView.cpp
//  IF
//
//  Created by wangchaoyi on 15/3/23.
//
//

#include "WorldPopupOnceView.h"

WorldPopupOnceView::WorldPopupOnceView(){
    
}

WorldPopupOnceView::~WorldPopupOnceView(){
    
}
const float numPerRow = 1.0;
WorldPopupOnceView* WorldPopupOnceView::create(){
    WorldPopupOnceView* ret = new WorldPopupOnceView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool WorldPopupOnceView::init(){
    if(PopupBaseView::init()){

        CCLoadSprite::doResourceByCommonIndex(501, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(501, false);
        });
        
        setIsHDPanel(true);
        auto tmpCCB = CCBLoadFile("WorldPopupOnceView",this,this,true);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());

        m_infoLabel1->setString(_lang("108880"));
        m_infoLabel2->setString(_lang("108881"));
        m_infoLabel3->setString(_lang("108882"));
        m_infoLabel4->setString(_lang("108883"));
        m_infoLabel5->setString(_lang("108884"));
        m_infoLabel6->setString(_lang("108885"));
        m_infoLabel7->setString(_lang("108886"));
        m_infoLabel8->setString(_lang("108887"));
        m_infoLabel9->setString(_lang("108888"));
        m_infoLabel10->setString(_lang("108889"));
        m_infoLabel11->setString(_lang("108890"));
        m_infoLabel12->setString(_lang("108891"));
        m_infoLabel13->setString(_lang("108892"));
        m_infoLabel14->setString(_lang("108893"));
        
        int temp = m_infoLabel2->getContentSize().height * m_infoLabel2->getOriginScaleY() - 23;
        m_infoList->setPositionY(m_infoList->getPosition().y - temp);
        
        CCSprite* pic1;
        string mpic1 = "guide_ziyuan.png";
        pic1 = CCLoadSprite::createSprite(mpic1.c_str());
        pic1->setAnchorPoint(ccp(0,0));
        m_spriteNode1->addChild(pic1);
        
        CCSprite* pic2;
        string mpic2 = "guide_guaiwu.png";
        pic2 = CCLoadSprite::createSprite(mpic2.c_str());
        pic2->setAnchorPoint(ccp(0,0));
        m_spriteNode2->addChild(pic2);
        
        CCSprite* pic3;
        string mpic3 = "guide_difang.png";
        pic3 = CCLoadSprite::createSprite(mpic3.c_str());
        pic3->setAnchorPoint(ccp(0,0));
        m_spriteNode3->addChild(pic3);
        
        CCSprite* pic4;
        string mpic4 = "guide_lianmeng.png";
        pic4 = CCLoadSprite::createSprite(mpic4.c_str());
        pic4->setAnchorPoint(ccp(0,0));
        m_spriteNode4->addChild(pic4);
        
        int height = 0;
        int h1 = 0;
        int h2 = 0;
        int h3 = 0;
        int h4 = 0;
        CCPoint p1, p2;
        int h;
        int dy = 20;

        height += 270;
        p1 = m_infoLabel4->getPosition();
        h = m_infoLabel4->getContentSize().height * m_infoLabel4->getOriginScaleY();
        height += h;
        m_infoLabel5->setPositionY(p1.y - h - dy);
        height += dy;
        
        //第二块
        p1 = m_infoNode1->getPosition();
        h = m_infoLabel5->getContentSize().height * m_infoLabel5->getOriginScaleY();
        height += h;
        height += dy;
        h1 = height;
        m_infoNode2->setPositionY(p1.y - height);

        
        height += 270;
        p1 = m_infoLabel7->getPosition();
        h = m_infoLabel7->getContentSize().height * m_infoLabel7->getOriginScaleY();
        height += h;
        m_infoLabel8->setPositionY(p1.y - h - dy);
        height += dy;
        
        //第三块
        p1 = m_infoNode2->getPosition();
        h = m_infoLabel8->getContentSize().height * m_infoLabel8->getOriginScaleY();
        height += h;
        height += dy;
        h2 = height - h1;
        m_infoNode3->setPositionY(p1.y - height);

        height += 270;
        p1 = m_infoLabel10->getPosition();
        h = m_infoLabel10->getContentSize().height * m_infoLabel10->getOriginScaleY();
        height += h;
        m_infoLabel11->setPositionY(p1.y - h - dy);
        height += dy;
        
        //第四块
        p1 = m_infoNode3->getPosition();
        h = m_infoLabel11->getContentSize().height * m_infoLabel11->getOriginScaleY();
        height += h;
        height += dy;
        h3 = height - h2 - h1;
        m_infoNode4->setPositionY(p1.y - height);
        
        height += 270;
        p1 = m_infoLabel13->getPosition();
        h = m_infoLabel13->getContentSize().height * m_infoLabel13->getOriginScaleY();
        height += h;
        m_infoLabel14->setPositionY(p1.y - h - dy);
        height += dy;
        
        h = m_infoLabel14->getContentSize().height * m_infoLabel14->getOriginScaleY();
        height += h;
        height += dy;
        h4 = height - h3 - h2 - h1;
        
        m_scrollView = CCScrollView::create(m_infoList->getContentSize());
        m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width, height));
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setAnchorPoint(ccp(0, 0));
        m_infoList->removeAllChildren();
        m_infoList->addChild(m_scrollView);
        m_scrollView->setPosition(0, 0);

        m_scrollView->addChild(m_infoNode1);
        m_scrollView->addChild(m_infoNode2);
        m_scrollView->addChild(m_infoNode3);
        m_scrollView->addChild(m_infoNode4);
        m_infoNode4->setPositionY(h4);
        m_infoNode3->setPositionY(h4+h3);
        m_infoNode2->setPositionY(h4+h3+h2);
        m_infoNode1->setPositionY(height);
        m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));

        return true;
    }
    return false;
}

void WorldPopupOnceView::onEnter(){
    PopupBaseView::onEnter();
}

void WorldPopupOnceView::onExit(){
    PopupBaseView::onExit();
}

bool WorldPopupOnceView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel1", CCLabelIF*, m_infoLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel2", CCLabelIF*, m_infoLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCNode*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel3", CCLabelIF*, m_infoLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel4", CCLabelIF*, m_infoLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel5", CCLabelIF*, m_infoLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel6", CCLabelIF*, m_infoLabel6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel7", CCLabelIF*, m_infoLabel7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel8", CCLabelIF*, m_infoLabel8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel9", CCLabelIF*, m_infoLabel9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel10", CCLabelIF*, m_infoLabel10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel11", CCLabelIF*, m_infoLabel11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel12", CCLabelIF*, m_infoLabel12);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel13", CCLabelIF*, m_infoLabel13);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel14", CCLabelIF*, m_infoLabel14);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spriteNode1", CCNode*, m_spriteNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spriteNode2", CCNode*, m_spriteNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spriteNode3", CCNode*, m_spriteNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spriteNode4", CCNode*, m_spriteNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode1", CCNode*, m_infoNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode2", CCNode*, m_infoNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode3", CCNode*, m_infoNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode4", CCNode*, m_infoNode4);
    return false;
}

SEL_CCControlHandler WorldPopupOnceView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", WorldPopupOnceView::onCloseClick);
    return NULL;
}

void WorldPopupOnceView::onCloseClick(cocos2d::CCObject *pSender, CCControlEvent event){
    closeSelf();
}

bool WorldPopupOnceView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //m_touchPoint = pTouch->getLocation();
    if (isTouchInside(m_infoList,pTouch)){
        return true;
    }
    return false;
}
void WorldPopupOnceView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(!isTouchInside(m_infoList, pTouch)){
    }
}