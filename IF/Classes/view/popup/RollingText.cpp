//
//  RollingText.cpp
//  IF
//
//  Created by 李锐奇 on 13-12-23.
//
//

#include "RollingText.h"
using namespace std;

const int speed = 2;

RollingText* RollingText::create(CCArray* arr, int w,const ccColor3B& color){
    RollingText* ret = new RollingText(arr, w,color);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void RollingText::onEnter(){
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(RollingText::enterFrame), this, 0.02,kCCRepeatForever, 0.0f, false);
}

void RollingText::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(RollingText::enterFrame), this);
    CCNode::onExit();
}

void RollingText::setFontColor(const ccColor3B& color){
    m_colText = color;
}

bool RollingText::init(){
    m_clipNode = CCClipNode::create(m_width, 25);
    m_clipNode->setAnchorPoint(ccp(0, 0.5));
    this->addChild(m_clipNode);
    enterFrame(0.0);
    return true;
}

void RollingText::enterFrame(float t){
    int totalCount = m_arr->count();
    if(totalCount == 0){
        return;
    }
    if(m_clipNode->getChildren().size() == 0){
        int total = 0;
        while(m_currentIndex < m_arr->count()){
            std::string str = dynamic_cast<CCString*>(m_arr->objectAtIndex(m_currentIndex))->getCString();
            auto render = RollRender::create(str,m_colText);
            m_clipNode->addChild(render);
            render->setPositionX(total);
            total += render->getW() + 5;
            m_currentIndex++;
        }
        if(total > m_width){
            m_scroll = true;
        }
    }else if(m_scroll){
        int i = 0;
        while(i < m_clipNode->getChildren().size()){
            m_currentIndex = m_currentIndex % totalCount;
            auto render = dynamic_cast<RollRender*>(m_clipNode->getChildren().at(i));
            render->setPositionX(render->getPositionX() - speed);
            if(render->getPositionX() < -render->getW()){
                m_clipNode->removeChild(render);
            }else{
                if(i == m_clipNode->getChildren().size() - 1){
                    if(render->getPositionX() + render->getW() < m_width){
                        std::string str = dynamic_cast<CCString*>(m_arr->objectAtIndex(m_currentIndex))->getCString();
                        auto addRender = RollRender::create(str,m_colText);
                        addRender->setPositionX(render->getPositionX() + render->getW() + 70);
                        m_clipNode->addChild(addRender);
                        m_currentIndex++;
                    }
                }
                i++;
            }
        }
    }
}

//------------------------------

RollRender* RollRender::create(std::string str,const ccColor3B& color){
    RollRender* ret = new RollRender(str,color);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void RollRender::onEnter(){
    CCNode::onEnter();
}

void RollRender::onExit(){
    CCNode::onExit();
}

bool RollRender::init(){
    m_text = CCLabelIF::create();
    m_text->setFontSize(20);
    m_text->setColor(m_colText);
    m_text->setAnchorPoint(ccp(0, 0));
    this->addChild(m_text);
    m_text->setString(m_str.c_str());
    return true;
}

int RollRender::getW(){
    int w = this->m_text->getContentSize().width * this->m_text->getOriginScaleX();
    return w;
}