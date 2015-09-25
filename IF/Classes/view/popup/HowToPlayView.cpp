//
//  HowToPlayView.cpp
//  COK
//
//  Created by zhaohf on 14-7-24.
//
//

#include "HowToPlayView.h"
#include "CCCommonUtils.h"
#include "HowToPlayCell.h"

HowToPlayView* HowToPlayView::create(){
    CCLOG("HTP***");
    unsigned long ulc = clock();
    HowToPlayView* ret = new HowToPlayView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    CCLOG("HTP created%lu",clock()-ulc);
    return ret;
}

bool HowToPlayView::init()
{
    //CCLOG("******HTP Begin**********");
    if (!PopupBaseView::init()) {
        return false;
    }

    m_htpGroup = LocalController::shared()->shared()->DBXMLManager()->getGroupByKey("howtoPlay");
    m_htpCount = m_htpGroup->count();
    m_cellSize = CCArray::create();
    
    auto tmpCCB = CCBLoadFile("TIPS_how_to_play",this,this);
    winsize = CCDirector::sharedDirector()->getWinSize();
    tmpCCB->setContentSize(winsize+CCSize(0,-140));
    m_bg->setContentSize(CCSize(winsize.width+2,winsize.height-140));
    setContentSize(winsize);
    m_list->setContentSize(CCSizeMake(m_list->getContentSize().width, winsize.height - 400));
    //m_list->setContentSize(CCSizeMake(640, 100));
    m_list->setPositionY(m_list->getContentSize().height - 120);
    m_bottom->setPositionY(-122);
    //m_label = CCLabelIFTTF::create("",24);
    //m_label->setDimensions(CCSize(460, NULL));
    
    m_tabView = CCTableView::create(this, m_list->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setTag(123);
    m_list->addChild(m_tabView);
    
    //动画
    //CCActionInterval * fadein = CCFadeIn::create(2);
    //m_bg->runAction(fadein);

    //m_bottom->setVisible(false);
    //m_list->setVisible(false);
    m_bg->setPreferredSize(CCSize(640,240));
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(HowToPlayView::HTPAnim), this, 0.1, kCCRepeatForever, 0.5f, false);
    return true;
}
void HowToPlayView::onEnter()
{
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,false);
    CCNode::onEnter();
    setTitleName(_lang("101216"));
}

void HowToPlayView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}


SEL_CCControlHandler HowToPlayView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", HowToPlayView::onCloseClick);

    return NULL;
}

bool HowToPlayView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_list", CCNode*, this->m_list);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottom", CCSprite*, this->m_bottom);
    return false;
}
void HowToPlayView::onCloseClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    closeSelf();
}

void HowToPlayView::scrollViewDidScroll(CCScrollView* view)
{
//    float curY = m_tabView->getContentOffset().y;
//    float minY = m_tabView->minContainerOffset().y;
//    if ((minY-curY) > 150) {
//        if (_lastPosY > curY) {//向下滑动
//            if(_canRequest){
//                _canRequest = false;
//                scheduleOnce(schedule_selector(ChatView::reSetRequest), 5.0f);
//                CCLOG("request data from server");
//                ChatController::getInstance()->sendRequestChat(type);
//            }
//        }
//    }
//    _lastPosY = curY;
}



//void HowToPlayView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
//{
//
//}

CCSize HowToPlayView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    //unsigned long ulc = clock();
    float height = 0;
    if (m_cellSize->count() > idx) {
        auto _height = dynamic_cast<CCInteger*>(m_cellSize->objectAtIndex(idx));
        if (_height) {
            //CCLOG("id:%d setSize array %lu",idx+9300,clock() - ulc);
            return CCSize(640,_height->getValue());
        }
    }
    int _id = 9300 + idx;
    CCDictionary* dic = _dict(m_htpGroup->objectForKey(CC_ITOA(_id)));
    std::string content = dic->valueForKey("content")->getCString();
    std::string level = dic->valueForKey("level")->getCString();
    
    if (level == "1"){
        m_cellSize->addObject(CCInteger::create(90));
        //CCLOG("CCArray->add");
        //CCLOG("id:%d title setSize %lu",_id,clock() - ulc);
        return CCSize(640,90);
    }
    else if (level =="2") {
        //ulc = clock();
        //std::string s = _lang(content);
        //CCLOG(":::::_lang(content) %lu",clock() - ulc);
        //m_label->setString(s.c_str());
        //CCLOG(":::::setString(s.c_str()) %lu",clock() - ulc);
        height = ceil(_lang(content).length()/38.3)*28.5;
        //CCSize labelSize = m_label->getContentSize();
        //CCLOG("Size:%f length:%lu L:%f H;%f \n%s",labelSize.height,_lang(content).length(),ceil(_lang(content).length()/38.3),height,_lang(content).c_str());
        std::string image = dic->valueForKey("image")->getCString();
        if(image !="") {
            image.append(".png");
            //CCLOG("append() %lu",clock() - ulc);
            //ulc = clock();
            CCSize imgSize = CCLoadSprite::createSprite(image.c_str())->getContentSize();
//            CCSize imgSize = CCLoadSprite::getSpriteSize(COMMON_PATH, 3, image.c_str());
            //CCLOG("get img size %lu",clock() - ulc);
            //float _height = pImage->getContentSize().height + labelSize.height + 90;
            float _height = height + imgSize.height + 90;
            //CCLOG("id:%d img setSize %lu",_id,clock() - ulc);
            m_cellSize->addObject(CCInteger::create(_height));
            //CCLOG("CCArray->add");
            return CCSize(640, _height);
        }
        //m_cellSize->addObject(CCInteger::create(labelSize.height+40)) ;
        m_cellSize->addObject(CCInteger::create(height+40)) ;
        //CCLOG("CCArray->add");
        //CCLOG("id:%d text setSize %lu",_id,clock() - ulc);
        //return CCSize(640,labelSize.height+40);
            return CCSize(640,height+40);
    }
    //CCLOG("id:%d time setSize null%lu",_id,clock() - ulc);
    return CCSize(640,0);
}

cocos2d::CCSize HowToPlayView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 80);
}

CCTableViewCell* HowToPlayView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    //unsigned long ulc = clock();
    if (idx >= m_htpCount) {
        return NULL;
    }
    if(idx == m_htpCount-1) {
        m_bottom->setVisible(false);
    }else {
        m_bottom->setVisible(true);
    }
    HowToPlayCell* cell = (HowToPlayCell*)table->dequeueCell();
    if(cell){
        //CCLOG("setData()");
        cell->setData(idx);
    }else{
        //CCLOG("create cell");
        cell = HowToPlayCell::create(idx,m_htpGroup);
    }
    //CCLOG("id:%d time setData %lu",idx+9300,clock() - ulc);
    return cell;
}

ssize_t HowToPlayView::numberOfCellsInTableView(CCTableView *table)
{
    return m_htpCount;
};

bool HowToPlayView::onTouchBegan(CCTouch* touch, CCEvent* event)
{
    //CCLOG("ccTouchBegan");
    return true;
}

void HowToPlayView::onTouchMoved(CCTouch* touch, CCEvent* event){
    //CCLOG("onTouchMoved");
}

void HowToPlayView::onTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCLOG("onTouchEnded");
    //获取离开屏幕时对应的坐标
    CCPoint point = touch->getLocation();
    if(point.y<140) {
        closeSelf();
    }
}

void HowToPlayView::HTPAnim(float dt){
    float h_bg = m_bg->getPreferredSize().height + 100;
    float h_list = m_list->getPositionY() - 100;
    float h_bottom = m_bottom->getPositionY() - 100;
    m_bg->setPreferredSize(CCSize(640,h_bg));
    m_list->setPositionY(h_list);
    m_bottom->setPositionY(h_bottom);
        if(h_bg >= 850){
            m_bg->setPreferredSize(CCSize(640,CCDirector::sharedDirector()->getWinSize().height - 140));
            m_list->setPositionY(-120);
            m_bottom->setPositionY(280 - winsize.height);
            //m_list->setVisible(true);
            //m_bottom->setVisible(true);
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(HowToPlayView::HTPAnim), this);
        }
}
