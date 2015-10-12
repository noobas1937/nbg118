//
//  IFShakeLayer.cpp
//  IF
//
//  Created by wangdianzhen on 15/8/18.
//
//

#include "IFShakeLayer.h"
#include "WorldMapView.h"
#include "DynamicTiledMap.h"

//214 255 214 358
void IFShakeLayer::onGotoPoint(CCObject* pSender,CCControlEvent event){
    WorldController::getInstance()->openMarchDeploy(m_tileIndex,0);
}
void IFShakeLayer::onClose(CCObject* pSender,CCControlEvent event){
    this->closeSelf();
}
IFShakeLayer::IFShakeLayer(int tileIndex,int monsterId){
    m_tileIndex = tileIndex;
    m_monsterId = monsterId;
    m_tilePoint = WorldController::getPointByIndex(m_tileIndex);
}
 IFShakeLayer* IFShakeLayer::create(int tileIndex,int monsterId){
     IFShakeLayer* pRet = new IFShakeLayer(tileIndex, monsterId);
     if (pRet && pRet->init()) {
         pRet->autorelease();
     }
     else{
     CC_SAFE_DELETE(pRet);
     }
     return pRet;
}
 void IFShakeLayer::onEnter(){
     CCNode::onEnter();
     setTouchEnabled(true);
}
 void IFShakeLayer::onExit(){
     setTouchEnabled(false);
     CCNode::onExit();
     
}
bool IFShakeLayer::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(206, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(206, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERBUST);
    });
    
    auto bg = CCBLoadFile("shakeLayer",this,this);
    this->setContentSize(bg->getContentSize());
    initLayerInfo();
    m_gotoBtn->setSwallowsTouches(false);
    m_closeBtn->setSwallowsTouches(false);
    return true;
}

SEL_CCControlHandler IFShakeLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGotoBtnClick", IFShakeLayer::onGotoPoint);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", IFShakeLayer::onClose);
    return NULL;
}

bool IFShakeLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLb", CCLabelIF*, this->m_titleLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subTitleLb", CCLabelIF*, this->m_subTitleLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLb", CCLabelIF*, this->m_infoLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_disLb", CCLabelIF*, this->m_disLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pointLb", CCLabelIF*, this->m_pointLb);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shakeSp", CCSprite*, this->m_shakeSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toushSp", CCSprite*, this->m_toushSp);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoBtn", CCControlButton*, this->m_gotoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightNode", CCNode*, this->m_rightNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    
    
    return false;
}
void IFShakeLayer::refreshInfo(int index,int monsterId){
    m_tileIndex = index;
    m_monsterId = monsterId;
    m_tilePoint  = WorldController::getPointByIndex(index);
    initLayerInfo(true);
}

bool IFShakeLayer::onTouchBegan(CCTouch* pTouch,CCEvent* pEvent){
    if (!isTouchInside(m_touchNode, pTouch)) {
        this->closeSelf();
    }
    return true;
}
void IFShakeLayer::onTouchEnded(CCTouch* pTouch,CCEvent* pEvent){
    if(isTouchInside(m_toushSp, pTouch)){
        this->closeSelf();
        WorldMapView::instance()->gotoTilePoint(m_tilePoint);
    }
    if(isTouchInside(m_gotoBtn, pTouch)){
        this->closeSelf();
        onGotoPoint(nullptr, CCControlEvent::TOUCH_DOWN);
    }
    if(isTouchInside(m_closeBtn, pTouch)){
        this->closeSelf();
    }
    
}
void IFShakeLayer::initLayerInfo(bool needAnimation/* = false*/){
//    needAnimation = true;
    if(needAnimation){
        m_picNode->setPosition(0, 255);
        m_rightNode->setPosition(200, 0);
    }
    m_titleLb->setString("");
    string tempStr = string("LV. ")+ CCCommonUtils::getPropById(CC_ITOA(m_monsterId), "level") + CCCommonUtils::getNameById(CC_ITOA(m_monsterId));
    m_subTitleLb->setString(tempStr.c_str());
    
    std::string description = CCCommonUtils::getPropById(CC_ITOA(m_monsterId), "description");
    std::vector<std::string> vector;
    CCCommonUtils::splitString(description, ",", vector);
    if(description != ""){
        int index = rand() % vector.size();
        if(index >= vector.size()){
            index = vector.size() - 1;
        }
        m_infoLb->setString(_lang(vector[index]));
    }
    auto vs = ccpSub(WorldMapView::instance()->m_map->getViewPointByTilePoint(m_tilePoint), WorldMapView::instance()->selfViewPoint);
    m_disLb->setString(_lang_1("113105", CC_ITOA((vs.getLength() / 100))));
    
    m_pointLb->setString(_lang_2("113104", CC_ITOA(m_tilePoint.x), CC_ITOA(m_tilePoint.y)));
    CCCommonUtils::setButtonTitle(m_gotoBtn, _lang("108723").c_str());
    string picName = CCCommonUtils::getPropById(CC_ITOA(m_monsterId), "monster") + string("_bust.png");
    m_picNode->removeAllChildrenWithCleanup(true);
    auto monsterSp  = CCLoadSprite::createSprite(picName.c_str(),true,CCLoadSpriteType_MONSTERLAYERBUST);
    monsterSp->setAnchorPoint(CCPoint(0.5, 0));
    m_picNode->addChild(monsterSp);
    m_shakeSp->stopAllActions();
    
    CCRotateBy * roaction = CCRotateBy::create(0.1f,60);
    CCRepeat * r1 = CCRepeat::create(CCSequence::create(roaction,roaction->reverse(),nullptr),4);
    CCDelayTime * d = CCDelayTime::create(5.0f);
    m_shakeSp->runAction(CCRepeatForever::create(CCSequence::create(r1,d,nullptr)));
    
    if(needAnimation){
        CCMoveTo* moveTo = CCMoveTo::create(0.3, ccp(300, 255));
        CCMoveTo* back = CCMoveTo::create(0.1, ccp(214, 255));
        
//        CCEaseExponentialIn * ease  = CCEaseExponentialIn::create(CCSequence::create(moveTo,back,NULL));
        m_picNode->runAction(CCSequence::create(moveTo,back,NULL));
        
        CCMoveTo* moveTo2 = CCMoveTo::create(0.3, ccp(-60, 0));
        CCMoveTo* back2 = CCMoveTo::create(0.1, ccp(0, 0));
//        CCEaseExponentialIn * ease2  = CCEaseExponentialIn::create(CCSequence::create(moveTo2,back2,NULL));
        m_rightNode->runAction(CCSequence::create(moveTo2,back2,NULL));
    }
    
}