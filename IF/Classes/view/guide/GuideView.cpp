//
//  GuideView.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-6.
//
//

#include "GuideView.h"
#include "NewPlotView.h"
#include "GuideController.h"
#include "QuestController.h"
#include "CCLoadSprite.h"
#include "GuidePlotView.h"
#include "ParticleController.h"

bool GuideView::init(){
    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0,0));
    m_rect = CCRect(0, 0, 0, 0);
    _modelLayer = NULL;
    _arrowAni = NULL;
    isMoving = false;
    _containerNode = CCNode::create();
    this->addChild(_containerNode);
        
    m_flyContainer = CCNode::create();
    this->addChild(m_flyContainer);
    
    m_particleContainer = CCNode::create();
    this->addChild(m_particleContainer);
    
    if (true) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        m_gotoNode = CCNode::create();
        
        m_gotoEndBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("world_coordinate_bg.png"));
        m_gotoEndBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GuideView::gotoEnd), CCControlEventTouchUpInside);
        m_gotoEndBtn->setPreferredSize(CCSize(329, 80));
        m_gotoEndBtn->setPosition(ccp(-150, 0));
        m_gotoEndBtn->setTouchPriority(-1);
        m_gotoNode->addChild(m_gotoEndBtn);
        
        auto sp1 = CCLoadSprite::createSprite("UI_jiantou01.png");
        sp1->setPosition(ccp(0, 10));
        m_gotoNode->addChild(sp1);
        auto sp2 = CCLoadSprite::createSprite("UI_jiantou02.png");
        sp2->setPosition(ccp(0, -20));
        m_gotoNode->addChild(sp2);
        
        auto label = CCLabelIF::create(_lang("106031").c_str());
        label->setAlignment(kCCTextAlignmentCenter);
        label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        label->setAnchorPoint(ccp(0.5, 0.5));
        label->setPosition(ccp(-150, 0));
        m_gotoNode->addChild(label);
        
        m_gotoNode->setPosition(ccp(winSize.width-50, winSize.height-100));
    }
    return true;
}

void GuideView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuideView::playPlotOver), PLOT_PLAY_OVER, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuideView::removeGuideView), POPUP_VIEW_CLOSE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuideView::removeGuideView), CLEAR_ARROW, NULL);
    setKeyboardEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//    CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GuideView::update), this, 0.02, 10, 0, false);
}

void GuideView::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
//    CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(this);
    setKeyboardEnabled(false);
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PLOT_PLAY_OVER);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, POPUP_VIEW_CLOSE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CLEAR_ARROW);
    CCNode::onExit();
}

void GuideView::removeGuideView(CCObject* p){
    if(_arrowAni){
        _arrowAni->stop();
        _arrowAni->removeFromParent();
    }
}

void GuideView::gotoEnd(cocos2d::CCObject *pTarget, CCControlEvent touchEvent){
    GuideController::share()->IsSkip = true;
    GuideController::share()->setGuide("3070700");
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_BATTLE_END);
//    GuideController::share()->setGuide("3010100");
}
void GuideView::showGoToBtn()
{
    m_gotoNode->removeFromParent();
    if (true || CCCommonUtils::isTestPlatformAndServer("guide_skip_1")) {
        string curGuideId = GuideController::share()->getCurGuideID();
        if (curGuideId == "3070100"||curGuideId == "3070200"||curGuideId == "3070300"||curGuideId == "3070400"||curGuideId == "3070500"||curGuideId == "3070600") {
            this->addChild(m_gotoNode);
            m_gotoNode->setVisible(true);
        }
    }
}

void GuideView::setArrow(CCPoint p, int w, int h, int d, bool fly){
    if(_arrowAni == NULL){
        _arrowAni = ArrowAni::create();
        _arrowAni->setVisible(false);
        _containerNode->addChild(_arrowAni);
    }
    _arrowAni->play(p, w, h, d);
    if(_arrowAni->getParent()==NULL){
        _containerNode->addChild(_arrowAni);
    }
    _arrowAni->setVisible(true);
    if(d == 0){
        _arrowAni->setVisible(false);
    }
}

void GuideView::doGuide(GuideInfo* guide){
    
}

void GuideView::doPlot(std::string did, bool cover){
    if (did == "") {
        return;
    }
    if(cover && _modelLayer && _modelLayer->getParent() == NULL){
        this->addChild(_modelLayer,1);
    }
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto spr = NewPlotView::create(did);
    spr->setPositionX((size.width-spr->getContentSize().width)/2);
    _containerNode->addChild(spr);
    
    showGoToBtn();
}

void GuideView::doPlot1(std::string did, bool cover){
    if (did == "") {
        return;
    }
    if(cover && _modelLayer && _modelLayer->getParent() == NULL){
        this->addChild(_modelLayer,1);
    }
    auto spr = GuidePlotView::create(did);
    _containerNode->addChild(spr);
    if (did == "3070710") {
        setModelLayerOpacity(0);
    }else if (did == "3110113") {
        spr->setPosition(spr->getPosition()+ccp(0, 350));
    }else if (did == "3110114") {
        spr->setPosition(spr->getPosition()+ccp(0, 210));
    }
    
    showGoToBtn();
}


CCNode *GuideView::getGuideNode(){
    auto& children = _containerNode->getChildren();
    if(children.size() > 0){
        NewPlotView *plot = dynamic_cast<NewPlotView*>(children.at(0));
        if(plot){
            return plot->getGuideNode();
        }
    }
    return NULL;
}

void GuideView::playPlotOver(CCObject* param){
    if(_modelLayer){
        _modelLayer->removeFromParent();
    }
        
    if(_arrowAni){
        _arrowAni->stop();
        _arrowAni->removeFromParent();
    }
    _containerNode->removeAllChildrenWithCleanup(true);
    GuideController::share()->next();
}

void GuideView::addToFlyContainer(CCNode* node){
    this->m_flyContainer->addChild(node);
}

void GuideView::clearFlyContainer(){
    this->m_flyContainer->removeAllChildren();
}

void GuideView::setModelLayerOpacity(int num)
{
    if(_modelLayer == NULL){
        auto size = this->getParent()->getContentSize();
        _modelLayer = CCRenderTexture::create(size.width, size.height);
        ccBlendFunc ccb1 = {GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
        _modelLayer->getSprite()->setBlendFunc(ccb1);
        _modelLayer->clear(0.0f, 0.0f, 0.0f, 0.6f);
        _modelLayer->setPosition(ccp(size.width/2, size.height/2));
    }
    if(_modelLayer->getParent() == NULL){
        this->addChild(_modelLayer, -1);
    }
    _modelLayer->removeAllChildren();
    _modelLayer->getSprite()->setOpacity(num);
    
    showGoToBtn();
}

void GuideView::addCover(CCRect& rect, int type){
    if(_modelLayer == NULL){
        auto size = this->getParent()->getContentSize();
        _modelLayer = CCRenderTexture::create(size.width, size.height);
        ccBlendFunc ccb1 = {GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
        _modelLayer->getSprite()->setBlendFunc(ccb1);
        _modelLayer->clear(0.0f, 0.0f, 0.0f, 0.6f);//0.0f, 0.0f, 0.0f, 0.7f
        _modelLayer->setPosition(ccp(size.width/2, size.height/2));
    }
    m_rect = rect;
    if(m_rect.size.width == 0){
        m_rect.size.width = 1;
    }
    if(m_rect.size.height == 0){
        m_rect.size.height = 1;
    }

    if(_modelLayer->getParent() == NULL){
        this->addChild(_modelLayer, -1);
    }
    _modelLayer->removeAllChildren();
    CCLayerColor* layer = CCLayerColor::create();
    layer->setOpacity(255);
    layer->setColor(ccc3(255, 255, 255));
    layer->setContentSize(m_rect.size);
    layer->setPosition(m_rect.origin);
    ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
    layer->setBlendFunc(cbf);
    _modelLayer->begin();
    layer->visit();
    _modelLayer->end();
//    if(_containerNode->getChildrenCount() == 0){
//        _modelLayer->getSprite()->setOpacity(180);
//    }
    _modelLayer->getSprite()->setOpacity(150);
    
    showGoToBtn();
    
    if (type == 0) {
        return;
    }
    
    string tmpStart = "ShowFire_";
    int maxP = m_rect.size.width/3;
    for (int i=1; i<=5; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(), CCPointZero,maxP);
        particle->setPosVar(ccp(m_rect.size.width/2, 0));
//        particle->setTotalParticles(m_rect.size.width/3);
        particle->setPosition(ccp(m_rect.origin.x+m_rect.size.width/2, m_rect.origin.y));
        addParticleToBatch(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(),CCPointZero,maxP);
        particle1->setPosVar(ccp(m_rect.size.width/2, 0));
        particle1->setPosition(ccp(m_rect.origin.x+m_rect.size.width/2, m_rect.origin.y+m_rect.size.height-7.5));
        addParticleToBatch(particle1);
    }
    
    tmpStart = "ShowFireUp_";
    maxP = m_rect.size.height/3;
    for (int i=1; i<=5; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(),CCPointZero,maxP);
        particle->setPosVar(ccp(0, m_rect.size.height/2));
        particle->setPosition(ccp(m_rect.origin.x, m_rect.origin.y+m_rect.size.height/2));
        addParticleToBatch(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString(),CCPointZero,maxP);
        particle1->setPosVar(ccp(0, m_rect.size.height/2));
        particle1->setPosition(ccp(m_rect.origin.x+m_rect.size.width, m_rect.origin.y+m_rect.size.height/2));
        addParticleToBatch(particle1);
    }
}

void GuideView::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_particleContainer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void GuideView::update(float t){
    if(_modelLayer && _modelLayer->isVisible() && _modelLayer->getParent()){
        int opacity = _modelLayer->getSprite()->getOpacity();
        if(opacity < 255){
//            _modelLayer->getSprite()->setOpacity(opacity + 20 > 255 ? 255 : opacity + 20);
            return;
        }
    }
}

void GuideView::removeCover(){
    if(_modelLayer){
        _modelLayer->removeAllChildren();
        _modelLayer->removeFromParent();
    }
    if(_arrowAni){
        _arrowAni->stop();
        _arrowAni->removeFromParent();
    }
    _containerNode->removeAllChildrenWithCleanup(true);
    m_rect.setRect(0, 0, 0, 0);
}

void GuideView::removeModelLayer()
{
    if(_modelLayer){
        _modelLayer->removeAllChildren();
        _modelLayer->removeFromParent();
        _modelLayer = NULL;
    }
}

bool GuideView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isMoving){
        return true;
    }

    if(_modelLayer == NULL || !_modelLayer->isVisible() || _modelLayer->getParent() == NULL){
        return false;
    }
    auto node = GuideController::share()->m_node;
    int ly = pTouch->getStartLocation().y;
    int lx = pTouch->getStartLocation().x;
 
    if(node && node->getParent()){
        CCPoint touchLocation = this->getParent()->convertToNodeSpace(pTouch->getLocation());
        lx = touchLocation.x;
        ly = touchLocation.y;
    }
    CCPoint pt = ccp(lx, ly);
    if(m_rect.containsPoint(pt)){
        return false;
    }
    return true;
}

void GuideView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void GuideView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
//    if(_arrowAni && _arrowAni->getParent()){
//        _arrowAni->showStar(pTouch->getLocation());
//    }
}
void GuideView::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* unused_event)
{
    unused_event->stopPropagation();
}

void GuideView::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event)
{
    unused_event->stopPropagation();
}
