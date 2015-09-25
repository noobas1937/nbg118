//
//  HowToPlayCell.cpp
//  COK
//
//  Created by zhaohf on 14-7-24.
//
//

#include "HowToPlayCell.h"
#include "CCCommonUtils.h"

HowToPlayCell* HowToPlayCell::create(int idx,CCDictionary* _htpGroup){
    HowToPlayCell* ret = new HowToPlayCell();
    if(ret && ret->init(idx,_htpGroup)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool HowToPlayCell::init(int idx,CCDictionary* _htpGroup)
{
    bool ret = true;
    CCBLoadFile("TIPS_how_to_play_cell",this,this);
    m_contextText->setDimensions(CCSize(460,NULL));
    this->setContentSize(CCSizeMake(640, 130));
    m_htpGroup = _htpGroup;
    setData(idx);
    return ret;
}

void HowToPlayCell::setData(int idx)
{
    //unsigned long ulc = clock();
    int _id = 9300 + idx;
    CCDictionary* dic = _dict(m_htpGroup->objectForKey(CC_ITOA(_id)));
    std::string content = dic->valueForKey("content")->getCString();
    std::string level = dic->valueForKey("level")->getCString();
    //CCLOG("dic->valueForKey()->getCString() %lu",clock() - ulc);
    if (level == "1"){
        //ulc = clock();
        m_titleText->setString(_lang(content).c_str());
        //CCLOG("setString(_lang(title)) %lu",clock() - ulc);
        m_titleNode->setVisible(true);
        m_contextNode->setVisible(false);
        //CCLOG("set title %lu",clock()-ulc);
    }
    else if (level == "2"){
        //ulc = clock();
        m_contextText->setString(_lang(content).c_str());
        m_contextNode->setVisible(true);
        m_titleNode->setVisible(false);
        //CCLOG("setString(_lang(content)) %lu",clock() - ulc);
        //m_bg交替显示
        CCDictionary* cnt_dic;
        std::string cnt_level;
        int cnt = idx-1;
        int i;
        for(i=0; cnt>=0; i++) {
            //ulc = clock();
            cnt_dic = _dict(m_htpGroup->objectForKey(CC_ITOA(9300+cnt)));
            cnt_level = cnt_dic->valueForKey("level")->getCString();
            //CCLOG("交替显示 cnt_dic->valueForKey(level)->getCString() %lu",clock() - ulc);
            cnt--;
            if(cnt_level == "1")
                break;
        }
        if(i%2 == 0) {
            m_bg->setVisible(false);
        }
        else {
            m_bg->setVisible(true);
        }
        //CCLOG("set text %lu",clock()-ulc);
        //ulc = clock();
        CCSize bgSize = m_bg->getContentSize();
        CCSize textSize = m_contextText->getContentSize();
        //CCLOG("bgSize textSize %lu",clock()-ulc);
        std::string image = dic->valueForKey("image")->getCString();
        if(image !="") {
            //ulc = clock();
            image.append(".png");
            CCSprite* m_image = CCLoadSprite::createSprite(image.c_str());
            //CCLOG("createSprite(image.c_str()) %lu",clock()-ulc);
            CCSize imgSize = m_image->getContentSize();
            m_image->setTag(0);
            m_image->setAnchorPoint(ccp(0.5,0));
            m_image->setPosition(ccp(320,35));
            m_image->setVisible(true);
            m_contextNode->removeChildByTag(0);
            m_contextNode->addChild(m_image);
            m_contextText->setPositionY(imgSize.height + 55);
            m_bg->setContentSize(CCSize(bgSize.width, imgSize.height + textSize.height + 60));
            //CCLOG("set img %lu",clock()-ulc);
        }
        else {
            //ulc = clock();
            m_bg->setContentSize(CCSize(bgSize.width,textSize.height+40));
            m_contextText->setPositionY(20);
            m_contextNode->removeChildByTag(0);
            //CCLOG("no image setBg contextPos rmNode %lu",clock()-ulc);
        }
    }
}

void HowToPlayCell::onEnter(){
    CCNode::onEnter();
   
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void HowToPlayCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler HowToPlayCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}

bool HowToPlayCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, this->m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contextNode", CCNode*, this->m_contextNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIFTTF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contextText", CCLabelIFTTF*, this->m_contextText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_line", CCScale9Sprite*, this->m_line);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_image", CCScale9Sprite*, this->m_image);
    return false;
}

bool HowToPlayCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {

    return false;
}

void HowToPlayCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {

}

