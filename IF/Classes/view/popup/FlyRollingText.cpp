//
//  FlyRollingText.cpp
//  IF
//
//  Created by liweiyu on 14-9-16.
//
//

#include "FlyRollingText.h"
#include "SceneController.h"
#include "PopupBaseView.h"
#include "PopupViewController.h"

FlyRollingText *FlyRollingText::create(string text, float width, float fontSize, ccColor3B color, float speed, int repeat,string icon){
    FlyRollingText *ret = new FlyRollingText(text, width, fontSize, color, speed, repeat,icon);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void FlyRollingText::onEnter(){
    CCNode::onEnter();
}

void FlyRollingText::onExit(){
    CCNode::onExit();
}

bool FlyRollingText::init(){
    if (!CCNode::init()) {
        return false;
    }
    std::string content = "";
    int num = m_text.length();
    for (int i=0; i<num; i++) {
        std::string one = m_text.substr(i,1);
        if(one=="\n"){
        }else{
            content.append(one);
        }
    }
    
    m_contentNode = CCNode::create();
    m_contentNode->setAnchorPoint(CCPointZero);
    int offX = 0;
    if(m_icon!=""){
        CCSpriteFrame* cf = CCLoadSprite::getSF(m_icon.c_str());
        CCSprite* pic = NULL;
        if(cf==NULL){
            CCLoadSprite::doResourceByCommonIndex(208, true);//现在只显示国旗
            pic = CCLoadSprite::createSprite(m_icon.c_str());
            CCLoadSprite::doResourceByCommonIndex(208, false);
        }else{
            pic = CCLoadSprite::createSprite(m_icon.c_str());
        }
        pic->setAnchorPoint(CCPointZero);
        pic->setPosition(ccp(0,-4));
        CCCommonUtils::setSpriteMaxSize(pic, 80);
        m_contentNode->addChild(pic);
        offX = 80;
    }
    m_label = CCLabelIFTTF::create();
    m_label->setFontSize(m_fontSize);
    m_label->setColor(m_color);
    m_label->setAnchorPoint(CCPointZero);
    m_label->setPosition(ccp(offX, 0));//m_width
    m_label->setString(content.c_str());
    m_contentNode->addChild(m_label);
    
    m_textContentSize = m_label->getContentSize();
    m_textContentSize.width = m_textContentSize.width + offX;
    
    m_clipNode = CCClipNode::create(m_width, m_textContentSize.height);
    m_clipNode->setAnchorPoint(CCPointZero);
    m_clipNode->addChild(m_contentNode);
    m_clipNode->setPosition(ccp(-m_width/2,-m_textContentSize.height/2));
    
    this->addChild(m_clipNode);
    rollText();
    return true;
}

void FlyRollingText::rollText(){
    float t = (m_width+m_textContentSize.width)/m_speed;
    std::string language = LocalController::shared()->getLanguageFileName();
    CCActionInterval* one = NULL;
    if(language == "ar"){
        one = CCSequence::create(
                                 CCPlace::create(ccp(-m_textContentSize.width,0))
                                 ,CCMoveTo::create(t, ccp(m_width, 0))
                                 ,CCCallFunc::create(this, callfunc_selector(FlyRollingText::stopAction))
                                 ,NULL
                                 );
    }else{
        one = CCSequence::create(
                                 CCPlace::create(ccp(m_width, 0))
                                 ,CCMoveTo::create(t, ccp(-m_textContentSize.width,0))
                                 ,CCCallFunc::create(this, callfunc_selector(FlyRollingText::stopAction))
                                 ,NULL
                                 );
    }

    CCAction* loop = CCRepeatForever::create(one);
    m_contentNode->runAction(loop);
}

void FlyRollingText::stopAction(){
    m_repeatCnt++;
    if(m_repeat == 0){
        return;
    }
    else if(m_repeatCnt == m_repeat){
        m_contentNode->stopAllActions();
        this->removeFromParent();
    }
}

float FlyRollingText::getRollingTime(){
    if(m_repeat == 0)
        return 0;
    else
        return m_repeat*(m_width+m_textContentSize.width)/m_speed;
}
