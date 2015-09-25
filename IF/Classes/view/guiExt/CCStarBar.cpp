//
//  CCStarBar.cpp
//  IF
//
//  Created by fubin on 14-8-26.
//
//

#include "CCStarBar.h"
#include "ParticleController.h"

CCStarBar* CCStarBar::createStarBar(int sumStar){
    CCStarBar *pRet = new CCStarBar();
    if (pRet && pRet->init(sumStar))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCStarBar::init(int sumStar)
{
    m_sumStar = sumStar;
    m_curStar = 0;
    m_pro = 0;
    int tmpX = 0;
    for (int i=0; i<m_sumStar; i++) {
        auto starBG = CCLoadSprite::createSprite("icon_xingxing02.png");
        starBG->setAnchorPoint(ccp(0.5, 0.5));
        starBG->setPosition(ccp(tmpX+starBG->getContentSize().width/2, +starBG->getContentSize().height/2));
        this->addChild(starBG);
        
        auto star = CCLoadSprite::createSprite("icon_xingxing01.png");
        star->setAnchorPoint(ccp(0, 0));
        auto cnode = CCNode::create();
        cnode->setAnchorPoint(ccp(0, 0));
        cnode->addChild(star);
        cnode->setPosition(ccp(tmpX, 0));
        this->addChild(cnode);
        
        m_starVec.push_back(star);
        m_nodeVec.push_back(cnode);
        cnode->setVisible(false);
        CCNode* partNode = CCNode::create();
        cnode->addChild(partNode, 3,99);
        
        tmpX += 65;
    }
    return true;
}
void CCStarBar::onEnter(){
    
    CCNode::onEnter();

}
void CCStarBar::onExit(){
    
    this->stopAllActions();
    CCNode::onExit();

}
void CCStarBar::setProgress(int star,int pro,bool islevelUp)
{
    m_blevel = islevelUp;
    if (star<=0) {
        return;
    }
    if(star==1&&pro==0){
        return;
    }
    ;
    //1.初始化star原始状态
    if(m_curStar==0){
        m_pro = pro;
        m_curStar = star;
        setInitAnim();
    }else if(m_curStar != star){//2.当前star升级（1、全满星2、未满星）
        m_pro = pro;
        m_curStar = star;
        setOneStarUpAnim();
    }else if(m_curStar == m_sumStar){
    
        if((m_pro!=pro)&&islevelUp){
            m_pro = pro;
            m_curStar = star;
            setOneStarUpAnim();
        }
    }

    
    
//    if (m_curStar != star) {
//        if(m_curStar>0||islevelUp){
//            m_bAddstar = true;
//        }else{
//            m_bAddstar = false;
//        }
//        m_curStar = star;
//        m_pro = pro;
//
//        setInitAnim();
//    }

}
void CCStarBar::setInitAnim(){

    for (int i=0; i<m_sumStar; i++)
    {
        auto partNode =m_nodeVec[i]->getChildByTag(99);
        partNode->removeAllChildren();
        m_starVec[i]->stopAllActions();
        m_starVec[i]->setOpacity(255);
        if((i)<m_curStar){
            m_nodeVec[i]->setVisible(true);
        }else{
            m_nodeVec[i]->setVisible(false);
        }
        if(m_blevel){
            m_nodeVec[i]->setVisible(true);
            auto praticle1 = ParticleController::createParticle("UIGlowLoop_1");
            partNode->addChild(praticle1);
            auto praticle2 = ParticleController::createParticle("UIGlowLoop_2");
            partNode->addChild(praticle2);
            praticle1->setPosition(25, 25);
            praticle2->setPosition(25, 25);

        }else if ((i+1) == m_curStar){
            m_nodeVec[i]->setVisible(true);
            auto praticle1 = ParticleController::createParticle("UIGlowLoop_1");
            partNode->addChild(praticle1);
            auto praticle2 = ParticleController::createParticle("UIGlowLoop_2");
            partNode->addChild(praticle2);
            praticle1->setPosition(25, 25);
            praticle2->setPosition(25, 25);
            CCActionInterval * rotateto1 = CCFadeOut::create(1);
            CCActionInterval * rotateto2 = CCFadeIn::create(1);
            CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,rotateto2,NULL);
            CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
            m_starVec[i]->runAction(rotateForever);
//            if (m_bAddstar) {
//                CCActionInterval * scaleAction = CCScaleTo::create(0.2,1.2);
//                CCActionInterval * scaleActionReverse = scaleAction->reverse();
//                scaleActionReverse->autorelease();
//                CCSequence* ation = CCSequence::create(scaleAction,scaleActionReverse,rotateForever,NULL);
//                m_starVec[i]->runAction(ation);
//            }else{
            
//            }

//    else {
//        if (m_pro != pro && star<=m_sumStar) {
//            m_pro = pro;
//            m_nodeVec[star-1]->setContentSize(CCSizeMake(50*pro, 50));
//
        }
    }
}
void CCStarBar::setOneStarUpAnim(){
    int i=2;
    if(m_blevel){
        i=1;
    }
    auto preStar = m_starVec[m_curStar-i];
    auto partNode = m_nodeVec[m_curStar-i]->getChildByTag(99);
    partNode->removeAllChildren();
    auto praticle1 = ParticleController::createParticle("UIGlow_1");
    partNode->addChild(praticle1);
    auto praticle2 = ParticleController::createParticle("UIGlow_2");
    partNode->addChild(praticle2);
    praticle1->setPosition(25, 25);
    praticle2->setPosition(25, 25);
    CCActionInterval * scaleAction = CCScaleTo::create(0.2,1.2);
    CCActionInterval * scaleActionReverse = CCScaleTo::create(0.2,1.0);
    CCSequence* ation = CCSequence::create(scaleAction,scaleActionReverse,CCCallFunc::create(this, callfunc_selector(CCStarBar::setOneStarUpAnimFinish)),NULL);
    preStar->runAction(ation);

}
void CCStarBar::setOneStarUpAnimFinish(){
//    auto partNode = m_nodeVec[m_curStar-2]->getChildByTag(99);
//    partNode->removeAllChildren();
    setInitAnim();
    
}
void CCStarBar::setAllStarUpAnim(){
    
    
    
    
}