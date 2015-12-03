//
//  ArcPopupBaseView.cpp
//  IF
//
//  Created by fubin on 14-5-14.
//
//

#include "ArcPopupBaseView.h"
#include "PopupViewController.h"

bool ArcPopupBaseView::init(int type) {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        showArcModel(type);
        ret = true;
    }
    return ret;
}

void ArcPopupBaseView::showArcModel(int type)
{
    _modelLayer->setOpacity(0);
    auto size = CCDirector::sharedDirector()->getWinSize();
    m_selfModelLayer = CCRenderTexture::create(size.width, size.height);
    m_selfModelLayer->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(m_selfModelLayer);
//    if (type == TYPE_POS_NONE) {
//        _modelLayer->setColor(Color3B(59, 65, 104));
//        _modelLayer->setOpacity(255);
//        return;
//    }
    float _wf = 0.31;
    float _hf = 0.55;
    if (type == TYPE_POS_UP || type == TYPE_POS_TO_UP) {
        _wf = 0.31;
        _hf = 0.63;
    }else if(type == TYPE_POS_MID_UP){
        _wf = 0.5;
//        _hf = 0.60;
        _hf = 0.7;
    }else if(type == TYPE_POS_MID){
        _wf = 0.5;
        _hf = 0.5;
    }
    
    ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
    auto spr = CCLoadSprite::createSprite("Circle_1.png");
    spr->setAnchorPoint(ccp(0.5, 0.5));
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float scale = winSize.height/960;
    spr->setScale(6*scale);
    spr->setBlendFunc(cbf);
    spr->setPosition(ccp(size.width*_wf, size.height*_hf));
    
    m_selfModelLayer->beginWithClear(0.15f, 0.22f, 0.34f, 1.0f);//0.35f, 0.68f, 1.0f, 1.0f
    spr->visit();
    m_selfModelLayer->end();
    setContentSize(size);
    
    if (type == TYPE_POS_TO_UP) {
        _wf = 0.31;
        _hf = 0.63;
        m_selfModelLayer->getSprite()->setOpacity(230);
    }
    else {
        m_selfModelLayer->getSprite()->setOpacity(0);
        m_selfModelLayer->getSprite()->runAction(CCFadeTo::create(0.25, 230));
    }
}

CCSafeObject<CCRenderTexture> ArcPopupBaseView::getArcModelLayer()
{
    return m_selfModelLayer;
}
