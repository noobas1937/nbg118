//
//  ArcScrollView.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-12.
//
//

#include "ArcScrollView.h"
#include "PopupViewController.h"
#include "CCMathUtils.h"
#include "SpriteSheetAni.h"
#include "ParticleController.h"
#include "CCGraySprite.h"

static  int radius = 1000;
static  int cellw2 = 85;

ArcScrollView* ArcScrollView::create(CCArray* array,int level,int pos){
    ArcScrollView* ret = new ArcScrollView(array,level,pos);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ArcScrollView::~ArcScrollView(){
    m_cells->removeAllObjects();
    m_data->removeAllObjects();
    if(m_clickArea){
        m_clickArea->removeFromParentAndCleanup(true);
    }
    if(m_touchArea){
        m_touchArea->removeFromParentAndCleanup(true);
    }
}

void ArcScrollView::setShowPos(int pos){
    m_pos = pos;
    m_pos = m_data->count() - 1 - m_pos;
    if(m_pos<0) m_pos = 0;
    if(m_pos>m_data->count() - 1) m_pos = m_data->count() - 1;
    m_scrollView->getContainer()->stopAllActions();
    m_scrollView->unscheduleAllSelectors();
    CCPoint initPos;
    if(m_num%2==1){
        initPos = ccp((m_num/2-m_pos)*m_cellW + m_cellW/2.0, 0);
    }else{
        initPos = ccp((m_num/2-m_pos)*m_cellW + m_cellW/2.0, 0);
    }
    m_scrollView->setContentOffset(initPos);
}

bool ArcScrollView::init()
{
    if (!CCNode::init()) {
        return false;
    }
    
    int sizeW = 640;
    int sizeH = 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        radius = 800;
        this->setScale(1536/640);
    }
    this->setContentSize(CCSize(sizeW,sizeH));
    CCLoadSprite::doLoadResourceAsync(COMMON_PATH, CCCallFuncO::create(this, callfuncO_selector(ArcScrollView::asyLoad), NULL), 4);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
    });
    //
//    CCNode* bg2 = CCLoadSprite::createSprite("arcScrollView_bg2.png");
//    bg2->setPosition(ccp(-283,0));
//    this->addChild(bg2,-10);
    
    m_touchArea = CCLoadSprite::createScale9Sprite("rect.png");//touch999999
    int maxH = 300;
    m_touchArea->setContentSize(CCSize(640,maxH));
    m_touchArea->setPosition(ccp(-28,-250));
    m_touchArea->setOpacity(255);
    m_touchArea->setVisible(false);
    this->addChild(m_touchArea,-1000);
    
    m_num = m_data->count();
    m_cellW = 170;
    m_scrollView = ScrollTableView::create(CCSize(MAX(m_cellW*m_num,sizeH),sizeH),m_cellW,m_num);
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
//    m_scrollView->setDirection(kCCScrollViewDirectionHorizontal);
    m_scrollView->setDelegate(this);
    m_scrollView->setTouchPriority(1);
    m_scrollView->touchArea = m_touchArea;
   
    if(m_num%2==0){
        m_scrollView->setPosition(ccp(-m_cellW*(m_num+1)/2.0, -400) );
    }else{
        m_scrollView->setPosition(ccp( -m_cellW*m_num/2.0, -400));//-50 - 120
    }
 
    if(m_level==1 || m_num<=1){
        m_scrollView->setTouchEnabled(false);
    }else{
        m_scrollView->setTouchEnabled(true);
    }
    this->addChild(m_scrollView);
    if(m_level!=1){
        m_clickArea = CCLoadSprite::createScale9Sprite("rect999999.png");
        m_clickArea->setContentSize(CCSize(m_cellW, 270));
        m_clickArea->setPosition(ccp(0, -270));
        m_clickArea->setOpacity(35);
        m_clickArea->setVisible(false);
        this->addChild(m_clickArea);
        m_scrollView->clickArea = m_clickArea;
        
        CCNode* cellBg = CCLoadSprite::createSprite("arrow.png");
        cellBg->setPosition(ccp(0,-170));
        this->addChild(cellBg);
    }
        
    m_isMoving = false;
    m_data->reverseObjects();
    m_pos = m_data->count() - 1 - m_pos;
    if(m_pos<0) m_pos = 0;
    if(m_pos>m_data->count() - 1) m_pos = m_data->count() - 1;
    
    m_cells = CCArray::create();
    
    m_touchEnd = false;
    //this->scheduleOnce(schedule_selector(ArcScrollView::updateInfo), 0.02);
    return true;
}

void ArcScrollView::asyLoad(CCObject* p){
    int sizeW = 640;
    int sizeH = 852;
    m_circle = CCLoadSprite::createSprite("tongyong_fenge_1.png");
    m_circle->setScaleX(1.2);
    m_circle->setAnchorPoint(ccp(0, 0));
    m_circle->setPosition(ccp(-20,-200));
//    m_circle->setOpacity(0);
    this->addChild(m_circle);
    
    auto tmpCircle2 = CCLoadSprite::createSprite("tongyong_fenge_1.png");
    tmpCircle2->setFlippedX(true);
    tmpCircle2->setScaleX(1.2);
    tmpCircle2->setAnchorPoint(ccp(1, 0));
    tmpCircle2->setPosition(ccp(20,-200));
//    tmpCircle2->setOpacity(0);
    this->addChild(tmpCircle2);
    
    if(m_level==1){
        m_circle->runAction(CCFadeIn::create(0.25));
    }else{
        m_circle->runAction(CCFadeIn::create(0.08));
    }
}

bool ArcScrollView::getAniState()
{
    if (m_scrollView) {
        return m_scrollView->m_aniStatus;
    }
    return true;
}

void ArcScrollView::moveOneCell(bool up){
    m_scrollView->moveOneCell(up);
}

bool ArcScrollView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_touchArea, pTouch)){
        m_isInTouch = true;
    }else{
        m_isInTouch = false;
    }
    m_touchEnd = false;
    return true;
}

void ArcScrollView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    m_touchEnd = true;
}

void ArcScrollView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    m_touchEnd = false;
}

void ArcScrollView::updateInfo()
{
    m_scrollView->unscheduleAllSelectors();
    m_scrollView->getContainer()->stopAllActions();
    m_scrollView->getContainer()->removeAllChildrenWithCleanup(true);
    CCPoint circle = ccp(0,700);//ccp(-283,0)
    
//    auto spr = CCLoadSprite::createSprite("donate_icon.png");
//    spr->setPosition(ccp(0,200));
//    this->addChild(spr);
    
    CCPoint worldPoint = this->convertToWorldSpace(circle);//世界圆心坐标
    for(int i=0;i<m_num;i++){
        ArcInfo* info = (ArcInfo*)m_data->objectAtIndex(i);
        ArcScrollCell* cell  = ArcScrollCell::create(info,m_clickArea,worldPoint,m_level);
        m_scrollView->addChild(cell,i==m_pos?100:0);
        cell->setAnchorPoint(ccp(0.5, 0.5));
        m_cells->addObject(cell);
        if (m_level==1) {
            cell->setPosition(ccp(i*m_cellW, 0));
            cell->setScale(0);
            CCActionInterval * scaleto = CCScaleTo ::create(0.2, 1);
            CCActionInterval * easeElasticIn= CCEaseElasticIn::create(scaleto);
            cell->runAction(easeElasticIn);
        }else{
            CCSequence* seq;
            if(i==m_pos){
                cell->setPosition(ccp(i*m_cellW,0));
                cell->setScale(0.1);
                cell->playAni(0.1);
                seq = CCSequence::create(CCScaleTo::create(0.1, 1),NULL);
            }else{
                cell->setPosition(ccp(m_pos*m_cellW, 0));
                cell->setScale(0.001);
                seq = CCSequence::create(CCDelayTime::create(0.15),CCScaleTo::create(0.03, 1),CCMoveTo::create(0.15, ccp(i*m_cellW, 0)),NULL);
            }
            cell->runAction(seq);
        }

    }
    CCPoint initPos;
    if(m_num%2==1){
        initPos = ccp((m_num/2-m_pos)*m_cellW + m_cellW/2.0, 0);
    }else{
        initPos = ccp((m_num/2-m_pos)*m_cellW + m_cellW/2.0, 0);
    }
    m_scrollView->setContentOffset(initPos);
    m_scrollView->initPos = initPos;
    m_scrollView->worldPoint = worldPoint;
    m_scrollView->minX = (m_num/2-(m_data->count() - 1))*m_cellW;
}

void ArcScrollView::onEnter()
{
    CCNode::onEnter();
    if(m_cells->count()<=0){
        this->updateInfo();
    }
    m_isInTouch = false;
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ArcScrollView::getMsgFun), MSG_ARC_CELL_CLICK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ArcScrollView::touchViewFun), MSG_TOUCH_ARC_VIEW, NULL);
}

void ArcScrollView::onExit()
{
    m_isInTouch = false;
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ARC_CELL_CLICK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOUCH_ARC_VIEW);
    CCNode::onExit();
}

void ArcScrollView::callback() {
    if (m_callbackTarget && m_callbackFunc) {
        (m_callbackTarget->*m_callbackFunc)();
    }
}

CCNode* ArcScrollView::getShowAreaByPos(int pos){
    CCNode* showAreaNode = NULL;
    for (int i=0; i<m_cells->count(); i++) {
        ArcScrollCell* cell  = (ArcScrollCell*)m_cells->objectAtIndex(i);
        if(cell->m_info->m_index==pos){
            showAreaNode = cell->m_showArea;
            break;
        }
    }
    return showAreaNode;
}

void ArcScrollView::scrollViewDidScroll(CCScrollView* view){
    if(!m_touchEnd) return ;
    int lastIndex = m_currentIndex;
    for (int i=0; i<m_cells->count(); i++) {
        ArcScrollCell* cell  = (ArcScrollCell*)m_cells->objectAtIndex(i);
        if(cell->checkValide()){
            m_currentIndex = cell->m_info->m_index;
            if(lastIndex!=m_currentIndex){
                this->callback();   
            }
            break;
        }
    }
}

void ArcScrollView::setCallback(cocos2d::CCObject *target, SEL_CallFunc func) {
    m_callbackTarget = target;
    m_callbackFunc = func;
}

void ArcScrollView::getMsgFun(CCObject* param){
    CCInteger* intObj = dynamic_cast<CCInteger*>(param);
    //CCLOG("m_type=%d",intObj->getValue());
    m_currentIndex = intObj->getValue();
    this->callback();

}

void ArcScrollView::touchViewFun(CCObject* param){
    m_isInTouch = (dynamic_cast<CCBool*>(param))->getValue();
}

ArcScrollCell* ArcScrollCell::create(ArcInfo* info,CCNode* clickNode,CCPoint worldPoint,int level)
{
    auto ret = new ArcScrollCell(info,clickNode,worldPoint,level);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ArcScrollCell::playAni(float delayTime){
}

void ArcScrollCell::playFinish(){
    this->m_haveAni = false;
}

bool ArcScrollCell::checkValide(){
    //if(m_info->isLock) return false;
    bool flag = false;
    //CCPoint center = ccp(37,473);//圆心的世界坐标
    CCPoint center = m_worldPoint;//ccp(35,477);//圆心的世界坐标
    CCPoint pos = ccp(85,105);
    CCPoint p2 = m_buttonNode->convertToWorldSpace(pos);
    float alphaW = center.x - p2.x;
    if(alphaW>cellw2 || alphaW<-cellw2){
        flag = false;
    }else{
        flag = true;
    }

    return flag;
}

int  ArcScrollCell::getWorldGapY(){
    CCPoint center = m_worldPoint;
    CCPoint pos = ccp(85,105);
    CCPoint p2 = m_buttonNode->convertToWorldSpace(pos);
    return abs((int)(center.x - p2.x));
}

void ArcScrollCell::setData(ArcInfo* info,CCNode* node,CCPoint worldPoint){
    m_clickNode = node;
    m_info = info;
    m_worldPoint = worldPoint;
}

void ArcScrollCell::onEnter() {
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void ArcScrollCell::onExit() {
    CCNode::onExit();
    setTouchEnabled(false);
}


bool ArcScrollCell::init()
{
    CCBLoadFile("ArcScrollCell",this,this);
    this->setContentSize(CCSize(170,210));//250,132
    m_lockIcon = NULL;
    if(m_info->m_srcIndex!=-1){
        CCLoadSprite::doResourceByCommonIndex(m_info->m_srcIndex, true);
    }
    int sIndex = m_info->m_srcIndex;
    setCleanFunction([sIndex](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        if(sIndex!=-1){
            CCLoadSprite::doResourceByCommonIndex(sIndex, false);   
        }
    });
    
    if(m_info->m_id!=""){
        string num = m_info->m_id.substr(m_info->m_id.size()-2);
//        m_LvSpr = CCCommonUtils::getRomanSprite(atoi(num.c_str())+1);
//        m_LvSpr->setAnchorPoint(ccp(0.5, 0.5));
//        this->m_button->addChild(m_LvSpr,1002);
//        m_LvSpr->setPosition(ccp(66,22));
        
        auto lvSpr1 = CCCommonUtils::getRomanSprite(atoi(num.c_str())+1, 1);
        m_lockLvNode->addChild(lvSpr1);
        auto lvSpr2 = CCCommonUtils::getRomanSprite(atoi(num.c_str())+1);
        m_LvNode->addChild(lvSpr2);
    }
    
    m_buttonTxt->setString(m_info->m_title);
    m_buttonLockTxt->setString(m_info->m_title);
    m_icon = CCLoadSprite::createSprite(m_info->m_icon.c_str());
    
    if(m_info->isLock)
    {
        CCCommonUtils::setSpriteGray(m_button, true);
        m_lockNode->setVisible(true);
        m_txtNode->setVisible(false);
        CCCommonUtils::setSpriteGray(m_icon,true);
        
//        m_lockIcon = CCLoadSprite::createSprite("iron_lock.png");
//        m_lockIcon->setPosition(ccp(66,66));
//        CCCommonUtils::setSpriteMaxSize(m_lockIcon, 40, true);
//        m_button->addChild(m_lockIcon,1001);
//        CCCommonUtils::setSpriteGray(m_lockIcon,true);
//        if(m_LvSpr){
//            CCCommonUtils::setSpriteGray(m_LvSpr,true);
//        }
    }else{
        m_lockNode->setVisible(false);
        m_txtNode->setVisible(true);
    }
    
    m_maxIconScale = 1;
//    CCCommonUtils::setSpriteMaxSize(m_icon, m_info->maxIconSize, true);
    CCCommonUtils::setSpriteMaxSize(m_icon, 150, true);
    m_maxIconScale = m_icon->getScale();
    m_icon->setPosition(ccp(84,120));
    this->m_button->addChild(m_icon,1000);
    
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->m_buttonTxt->setDimensions(CCSize(300, 0));
    }
    
    m_haveAni = false;
    
    return true;
}

void ArcScrollCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    m_isDraging = true;
}

bool ArcScrollCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_isDraging = false;
    if(isTouchInside(m_button, pTouch)){
        return true;
    }
    return false;
}

void ArcScrollCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(m_isDraging) return ;
    if((m_level==1 && isTouchInside(m_button, pTouch))||(isTouchInside(m_button, pTouch) && isTouchInside(m_clickNode, pTouch))){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ARC_CELL_CLICK,CCInteger::create(m_info->m_index));
        if(m_level==1) return ;
        string tmpStart = "UIGlow_";
        int count = 3;
        for (int i = 1; i <= count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(66, 66));
            m_button->addChild(particle,10000);
        }
    }
}

void ArcScrollCell::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    CCPoint center = m_worldPoint;//圆心的世界坐标
    CCPoint pos = ccp(85,105);
    CCPoint p2 = m_buttonNode->convertToWorldSpace(pos);
    float value = (p2.x-center.x)/radius;
    float moveY = radius*cos(asin(value));
    currentWorldPoint = ccp(p2.x, center.y-moveY);//当前点的世界坐标
    CCPoint local = m_buttonNode->convertToNodeSpace(currentWorldPoint);
    m_button->setPosition(ccp(local.x,local.y));
    m_txtNode->setPosition(local);
    m_showArea->setPosition(local);
    m_lockNode->setPosition(local);
    if(m_level!=1 && false){
        float alphaW = center.x - p2.x;
        float alpValue;
        float scale = 0;
        if(alphaW>cellw2 || alphaW<-cellw2){
            float alp = abs(int(alphaW))/320.0;
            if(alp>1) alp = 1;
            alpValue= (1-alp)*255;
            scale = 1;//0.9*(1-alp*0.35);
            this->m_button->setOpacity(alpValue);
            this->m_buttonTxt->setOpacity(alpValue);
            this->m_icon->setOpacity(alpValue);

            if(m_lockIcon){
                this->m_lockIcon->setOpacity(alpValue);
            }
            if(this->m_LvSpr){
                this->m_LvSpr->setOpacity(alpValue);
                this->m_LvSpr->setScale(scale);
            }
            this->m_button->setScale(scale);
            this->m_buttonTxt->setScale(scale);
            this->m_icon->setScale(scale*m_maxIconScale);
        }else{
            alpValue = 255;
            scale = 1;
            this->m_button->setOpacity(alpValue);
            this->m_buttonTxt->setOpacity(alpValue);
            if(m_lockIcon){
                this->m_lockIcon->setOpacity(alpValue);
            }
            if(this->m_LvSpr){
                this->m_LvSpr->setOpacity(255);
                this->m_LvSpr->setScale(scale);
            }
            this->m_button->setScale(scale);
            this->m_buttonTxt->setScale(scale);
            if(!m_haveAni){
                this->m_icon->setScale(scale*m_maxIconScale);
                this->m_icon->setOpacity(255);
            }
        }
    }
    CCNode::visit(renderer,parentTransform,parentFlags);
//    this->draw();
}

CCPoint ArcScrollCell::getBottomPoint(){
    CCPoint bpos = m_button->getPosition();
    CCPoint pos = m_buttonNode->convertToWorldSpace(ccp(bpos.x-cellw2,bpos.y));
    return pos;
}

CCPoint ArcScrollCell::getUpPoint(){
    CCPoint bpos = m_button->getPosition();
    CCPoint pos = m_buttonNode->convertToWorldSpace(ccp(bpos.x+cellw2,bpos.y));
    return pos;
}

bool ArcScrollCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonTxt", CCLabelIF*, this->m_buttonTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button", CCSprite*, this->m_button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonNode", CCNode*, this->m_buttonNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtNode", CCNode*, this->m_txtNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showArea", CCNode*, this->m_showArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_LvNode", CCNode*, this->m_LvNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNode", CCNode*, this->m_lockNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockLvNode", CCNode*, this->m_lockLvNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonLockTxt", CCLabelIF*, this->m_buttonLockTxt);
    return false;
}
