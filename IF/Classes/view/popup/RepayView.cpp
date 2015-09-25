
#include "RepayView.h"
#include "ActivityController.h"
#include "ToolController.h"
#include "EquipmentController.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "CCClipNode.h"
#include "TipsView.h"
#include "UIComponent.h"

#define REPAY_PROGRESS_BAR_STEP_LENGTH 112
#define REPAY_PROGRESS_BAR_STEP 5
#define REPAY_PROGRESS_BAR_ICON_Y 90
#define REPAY_PROGRESS_BAR_ICON_X 0
#define REPAY_CELL_W 640
#define REPAY_CELL_H 150
#define REPAY_ITEM_CELL_H 95
#define REPAY_REWARD_MAX_COUNT 6

#define REPAY_CLIP_NODE_TAG 10000
#define REPAY_CLIP_NODE_BAR_TAG 10001

#define MSG_REPAY_VIEW_SHOW_DES "msg.repay.view.show.des"

RepayView* RepayView::create(){
    RepayView* ret = new RepayView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
RepayView::RepayView():m_bInit(false),m_tabRewards(NULL),m_isTouchMove(false){
    
}
RepayView::~RepayView(){
    this->unschedule(schedule_selector(RepayView::onTimer));
}
void RepayView::onEnter(){
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RepayView::refreshData), MSG_REPAY_INFO_INIT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RepayView::showDes), MSG_REPAY_VIEW_SHOW_DES, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    PopupBaseView::onEnter();
}
void RepayView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REPAY_INFO_INIT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REPAY_VIEW_SHOW_DES);
    PopupBaseView::onExit();
}
bool RepayView::init(){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(305, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    m_dataCount = RepayController::getInstance()->repayRewards.size();
    setTitleName("");
    auto tmpCCB = CCBLoadFile("RepayView",this,this);
    setContentSize(tmpCCB->getContentSize());
    auto size = CCDirector::sharedDirector()->getWinSize();
    float extH = getExtendHeight();
    float add =  size.height - 852;
    if (CCCommonUtils::isIosAndroidPad()) {
        add = size.height - 2048;
        add = add / 2.4;
    }
    m_bgNode->setPositionY(m_bgNode->getPositionY()-extH);
    auto tbg = CCLoadSprite::loadResource("Bottom frame04.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = ceil(size.height*1.0/90);
    for (int j = 0; j<maxHight; j++)
    {
        for (int i=0; i<8; i++) {
            auto bg = CCLoadSprite::createSprite("Bottom frame04.png");
            bg->setAnchorPoint(ccp(0, 0));
            bg->setPosition(ccp(i*90, j*90));
            tBatchNode->addChild(bg);
        }
    }
    m_bgNode->addChild(tBatchNode);
    CCSize nodeRewardSize = m_nodeRewards->getContentSize();
    nodeRewardSize.height += extH;
    m_nodeRewards->setContentSize(nodeRewardSize);
    m_nodeRewards->setPositionY(m_nodeRewards->getPositionY()-extH);
    m_tabRewards = CCTableView::create(this, nodeRewardSize);
    m_tabRewards->setDirection(kCCScrollViewDirectionVertical);
    m_tabRewards->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabRewards->setTouchPriority(Touch_Default);
    m_tabRewards->setDelegate(this);
    m_nodeRewards->addChild(m_tabRewards);
    
//    m_scrollPoints = CCScrollView::create(m_nodePoints->getContentSize());
//    m_scrollPoints->setDirection(kCCScrollViewDirectionHorizontal);
//    m_scrollPoints->setTouchEnabled(false);
//    m_nodePoints->addChild(m_scrollPoints);
//    m_scrollPoints->setPosition(ccp(0, 0));
    auto clipNode = CCClipNode::create(m_nodePoints->getContentSize().width, m_nodePoints->getContentSize().height);
    clipNode->setAnchorPoint(CCPointZero);
    m_nodePoints->addChild(clipNode);
    clipNode->setTag(REPAY_CLIP_NODE_TAG);
    CCNode *node = CCNode::create();
    clipNode->addChild(node);
    node->setTag(REPAY_CLIP_NODE_TAG);
    
    auto clipNode1 = CCClipNode::create(m_nodePoints->getContentSize().width - 45, m_nodePoints->getContentSize().height);
    m_nodePoints->addChild(clipNode1);
    clipNode1->setPosition(ccp(23,0));
    clipNode1->setTag(REPAY_CLIP_NODE_BAR_TAG);
    auto progress = CCLoadSprite::createScale9Sprite("01_03.png");
    progress->setAnchorPoint(ccp(0, 0.5));
    progress->setTag(REPAY_CLIP_NODE_BAR_TAG);
    clipNode1->addChild(progress);
    progress->setPreferredSize(CCSize(1, 26));
    progress->setPosition(ccp(2.5, 60));
    setTitleName(_lang("133246"));
//    m_lblTitle->setString(_lang("133246"));
//    setButtonTitle(m_btnGoBuy, _lang("101221").c_str());
    m_lblPointTitle->setString(_lang("133247"));
    m_lblRewardTitle->setString(_lang("105848"));
    m_lblPointOrgW = m_lblPointTitle->getContentSize().width * m_lblPointTitle->getOriginScaleX();
    m_lblPointOrgX = m_lblPointTitle->getPositionX();
    onTimer(0);
    this->schedule(schedule_selector(RepayView::onTimer),1.0);
    initPointNode(true);
    
    CCPoint offset = m_tabRewards->getContentOffset();
    int newPoint = RepayController::getInstance()->getNewPayPoint();
    int lv = RepayController::getInstance()->getLVIndexByPoint(newPoint) - 3;
    float changeH = 0;
    while (lv>-1) {
        changeH += tableCellSizeForIndex(m_tabRewards, lv).height;
        lv--;
    }
    offset.y += changeH;
    m_tabRewards->setContentOffset(offset);
    return true;
}
void RepayView::showDes(CCObject *ccObj){
    if(m_isTouchMove==true){
        m_desNode->setVisible(false);
        return;
    }
    if(!ccObj){
        m_desNode->setVisible(false);
        return;
    }
    CCArray *arr = dynamic_cast<CCArray*>(ccObj);
    if(!arr || arr->count()< 4){
        m_desNode->setVisible(false);
        return;
    }
    std::string name = (dynamic_cast<CCString*>(arr->objectAtIndex(0)))->getCString();
    std::string des = (dynamic_cast<CCString*>(arr->objectAtIndex(1)))->getCString();
    int posx = (dynamic_cast<CCInteger*>(arr->objectAtIndex(2)))->getValue();
    int posy= (dynamic_cast<CCInteger*>(arr->objectAtIndex(3)))->getValue();
    CCLabelIF* lbl1 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(1));
    CCLabelIF* lbl2 = dynamic_cast<CCLabelIF*>(m_desNode->getChildByTag(2));
    if(lbl1 && lbl2){
        lbl1->setString(des);
        lbl2->setString(name);
    }
    CCPoint point = m_desNode->getParent()->convertToNodeSpace(ccp(posx,posy));
//    auto size = CCDirector::sharedDirector()->getWinSize();
    auto desSize = m_desNode->getContentSize();
    m_desNode->setPosition(ccp(point.x, point.y));
    m_desNode->setVisible(true);
    CC_SAFE_RELEASE(arr);
}
void RepayView::refreshData(CCObject *ccObj){
    m_dataCount = RepayController::getInstance()->repayRewards.size();
    m_tabRewards->reloadData();
    initPointNode(false);
}
void RepayView::onTimer(float dt){
    double endT = RepayController::getInstance()->getEndTime();
    int lastTime = GlobalData::shared()->changeTime(endT) - GlobalData::shared()->getWorldTime();
    if(lastTime>0){
        m_lblTime->setString(CC_SECTOA(lastTime));
    }else{
        PopupViewController::getInstance()->removePopupView(this);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REPAY_INFO_INIT);
    }
}
bool RepayView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTime", CCLabelIF*, this->m_lblTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle", CCLabelIF*, this->m_lblTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblPointTitle", CCLabelIF*, this->m_lblPointTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblPointNum", CCLabelIF*, this->m_lblPointNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodePoints", CCNode*, this->m_nodePoints);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblDes", CCLabelIF*, this->m_lblDes);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnGoBuy", CCControlButton*, this->m_btnGoBuy);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblRewardTitle", CCLabelIF*, this->m_lblRewardTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeRewards", CCNode*, this->m_nodeRewards);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, this->m_desNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollPoints", CCScrollView*, this->m_scrollPoints);
    return false;
}

SEL_CCControlHandler RepayView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoToPayClick", RepayView::onGoToPayClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipClick", RepayView::onTipClick);
    return NULL;
}
bool RepayView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(m_desNode->isVisible()){
        m_desNode->setVisible(false);
    }
    m_isTouchMove=false;
    return true;
}
void RepayView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) < 20){
        m_isTouchMove=false;
    }else{
        m_isTouchMove=true;
    }
}
void RepayView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}
void RepayView::onGoToPayClick(CCObject * pSender, Control::EventType pCCControlEvent){
    UIComponent::getInstance()->showGoldBuyPanel();
}
void RepayView::onTipClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    RepayController::getInstance()->addPoint(RepayController::getInstance()->getNewPayPoint() + 1000);
//    return;
    string showTip = _lang("133249");
    PopupViewController::getInstance()->addPopupView(TipsView::create(showTip));
}
CCSize RepayView::tableCellSizeForIndex(CCTableView *table, ssize_t idx){
    if(idx >= (m_dataCount-1)){
        return CCSizeZero;
    }
    RepayRewardStruct rStruct = RepayController::getInstance()->repayRewards.at(idx+1);
    int rowCount = ceil(rStruct.rewards.size() * 1.0 /(REPAY_REWARD_MAX_COUNT * 1.0));
    int cellH = (rowCount-1)*(REPAY_ITEM_CELL_H+5) + REPAY_CELL_H;
    return CCSize(REPAY_CELL_W, cellH);
}
CCSize RepayView::cellSizeForTable(CCTableView *table){
    return CCSize(REPAY_CELL_W, REPAY_CELL_H);
}
CCTableViewCell* RepayView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    if(idx >= (m_dataCount-1)){
        return NULL;
    }
    RepayCell* cell = (RepayCell*)table->dequeueCell();
    if(cell){
        cell->setData(idx+1);
    }else{
        cell = RepayCell::create(idx+1,m_nodeRewards);
    }
    return cell;
}
ssize_t RepayView::numberOfCellsInTableView(CCTableView *table){
    return m_dataCount-1;
}
void RepayView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){
    
}
void RepayView::scrollViewDidScroll(CCScrollView* view){
    float mindy = m_tabRewards->minContainerOffset().y-20;
    float dy = m_tabRewards->getContentOffset().y;
    if(dy<mindy){
        m_tabRewards->setContentOffset(ccp(0, mindy));
    }
}
void RepayView::initPointNode(bool bTween){
    auto clipNode = m_nodePoints->getChildByTag(REPAY_CLIP_NODE_TAG);
    auto containerNode = clipNode->getChildByTag(REPAY_CLIP_NODE_TAG);
    containerNode->removeAllChildren();
    containerNode->setPositionX(0);
    auto clipNode2 = m_nodePoints->getChildByTag(REPAY_CLIP_NODE_BAR_TAG);
    CCScale9Sprite *progress = dynamic_cast<CCScale9Sprite*>(clipNode2->getChildByTag(REPAY_CLIP_NODE_BAR_TAG));
    progress->setPositionX(2.5);
    int startIndex = 0;
    int endIndex = 0;
    int oldPoint = RepayController::getInstance()->getOldPayPoint();
    int newPoint = RepayController::getInstance()->getNewPayPoint();
    int lastLVIndex = RepayController::getInstance()->getLVIndexByPoint(oldPoint) - 1;
    lastLVIndex = MAX(lastLVIndex,0);
    int nextLVIndex = RepayController::getInstance()->getLVIndexByPoint(newPoint);
    nextLVIndex = MAX(nextLVIndex,1);
    int curLVIndex = nextLVIndex - 1;
    int needPoint = RepayController::getInstance()->repayRewards.at(nextLVIndex).point - newPoint;
    string desStr = "";
    if(needPoint>0){
        desStr = _lang_1("133248", CC_CMDITOA(needPoint).c_str());
    }else{
        desStr = _lang("133265");
    }
    desStr.append("\n");
    desStr.append(_lang("101017"));
    
    m_lblDes->setString(desStr);
    m_lblPointNum->setString(CC_CMDITOA(newPoint));
    float tmpW2 = m_lblPointNum->getContentSize().width * m_lblPointNum->getOriginScaleX();
    float tmpMove = (m_lblPointOrgW - tmpW2) * 0.5;
    m_lblPointTitle->setPositionX(m_lblPointOrgX + tmpMove);
    m_lblPointNum->setPositionX(m_lblPointTitle->getPositionX()+10);
    
    bool bScaleProgressBar = false;
    bool bMoveBox = false;
    int showPoint = 0;
    int showIndex0 = 0;
    int showIndex1 = 0;
    int showIndex2 = 0;
    float progressStartScale = 0.0f;
    float progressEndScale = 0.0f;
    int moveOffset = 0;
    float progressEndScale1 = 0.0f;
    int totalStep = REPAY_PROGRESS_BAR_STEP;
    if(bTween==false || RepayController::getInstance()->getChangeLV()==false){
        progress->setPreferredSize(CCSize(REPAY_PROGRESS_BAR_STEP*REPAY_PROGRESS_BAR_STEP_LENGTH, 26));
        endIndex = nextLVIndex+1;
        if(endIndex<REPAY_PROGRESS_BAR_STEP){
            endIndex = REPAY_PROGRESS_BAR_STEP;
        }else if(endIndex>m_dataCount-1){
            endIndex = m_dataCount - 1;
        }
        showPoint = newPoint;
        startIndex = endIndex - REPAY_PROGRESS_BAR_STEP;
        showIndex0 = curLVIndex;
        showIndex1 = nextLVIndex;
        showIndex2 = curLVIndex;
        bScaleProgressBar=false;
        bMoveBox=false;
    }else{
        showPoint = oldPoint;
        endIndex = nextLVIndex+1;
        if(nextLVIndex<REPAY_PROGRESS_BAR_STEP || nextLVIndex>=(m_dataCount-1)){
            progress->setPreferredSize(CCSize(REPAY_PROGRESS_BAR_STEP*REPAY_PROGRESS_BAR_STEP_LENGTH, 26));
            
            if(endIndex<REPAY_PROGRESS_BAR_STEP){
                endIndex = REPAY_PROGRESS_BAR_STEP;
            }else if(endIndex>m_dataCount-1){
                endIndex = m_dataCount - 1;
            }
            startIndex = endIndex - REPAY_PROGRESS_BAR_STEP;
            auto minP = RepayController::getInstance()->repayRewards.at(startIndex).point;
            if(oldPoint>minP){
                showIndex0 = lastLVIndex;
                showIndex1 = lastLVIndex+1;
                showIndex2 = lastLVIndex;
            }else{
                showIndex0 = startIndex;
                showIndex1 = startIndex;
                showIndex2=startIndex;
            }
            
            auto value1 = RepayController::getInstance()->repayRewards.at(curLVIndex);
            auto value2 = RepayController::getInstance()->repayRewards.at(nextLVIndex);
            float extValue = newPoint - value1.point;
            float extTotal = value2.point - value1.point;
            float extScale = 0;
            if(extTotal!=0){
                extScale = extValue/extTotal;
            }
            
            extScale += curLVIndex-startIndex;
            progressEndScale = extScale/REPAY_PROGRESS_BAR_STEP;
            bScaleProgressBar=true;
            bMoveBox=false;
        }else{
            totalStep = REPAY_PROGRESS_BAR_STEP+2;
            progress->setPreferredSize(CCSize(totalStep*REPAY_PROGRESS_BAR_STEP_LENGTH, 26));
            
            endIndex = nextLVIndex+1;
            if(endIndex<totalStep){
                endIndex = totalStep;
            }else if(endIndex>m_dataCount-1){
                endIndex = m_dataCount - 1;
            }
            
            startIndex = endIndex - totalStep;
            auto minP = RepayController::getInstance()->repayRewards.at(startIndex).point;
            if(oldPoint>minP){
                showIndex0 = lastLVIndex;
                showIndex1 = lastLVIndex+1;
                showIndex2 = lastLVIndex;
            }else{
                showIndex0 = startIndex;
                showIndex1 = startIndex;
                showIndex2=startIndex;
            }
            if(curLVIndex>=(endIndex-2)){
                progressEndScale = REPAY_PROGRESS_BAR_STEP*1.0 / totalStep;
                auto value1 = RepayController::getInstance()->repayRewards.at(curLVIndex);
                auto value2 = RepayController::getInstance()->repayRewards.at(nextLVIndex);
                float extValue = newPoint - value1.point;
                float extTotal = value2.point - value1.point;
                float extScale = 0;
                if(extTotal!=0){
                    extScale = extValue/extTotal;
                }
                extScale += curLVIndex-startIndex;
                progressEndScale1 = extScale/totalStep;
            }else{
                auto value1 = RepayController::getInstance()->repayRewards.at(curLVIndex);
                auto value2 = RepayController::getInstance()->repayRewards.at(nextLVIndex);
                float extValue = newPoint - value1.point;
                float extTotal = value2.point - value1.point;
                float extScale = 0;
                if(extTotal!=0){
                    extScale = extValue/extTotal;
                }
                extScale += curLVIndex-startIndex;
                progressEndScale = extScale/totalStep;
                progressEndScale1=progressEndScale;
            }
            bScaleProgressBar=true;
            moveOffset = -2 * REPAY_PROGRESS_BAR_STEP_LENGTH;
            bMoveBox=true;
        }
    }
    int offsetX = 20;
    for (int index = startIndex; index<=endIndex; ++index) {
        if(index > 0){
            auto sprite = CCLoadSprite::createSprite("item302.png");
            containerNode->addChild(sprite);
            sprite->setPosition(ccp(offsetX, 110));
            sprite->setTag(index);
            if(index>4){
                sprite->setScale(0.6);
            }else{
                sprite->setScale(0.5);
            }
            if((index-REPAY_PROGRESS_BAR_STEP-startIndex)>0){
                sprite->setScale(0);
            }
        }
        auto value = RepayController::getInstance()->repayRewards.at(index);
        auto label = CCLabelIF::create("", 20);
        label->setColor(ccc3(255, 236, 214));
        label->setString(CC_CMDITOA(value.point));
        containerNode->addChild(label);
        label->setPosition(ccp(offsetX,20));
        offsetX += REPAY_PROGRESS_BAR_STEP_LENGTH;
    }
    auto value1 = RepayController::getInstance()->repayRewards.at(showIndex0);
    auto value2 = RepayController::getInstance()->repayRewards.at(showIndex1);
    float extValue = showPoint - value1.point;
    float extTotal = value2.point - value1.point;
    float extScale = 0;
    if(extTotal!=0){
        extScale = extValue/extTotal;
    }
    extScale += showIndex2-startIndex;
    progressStartScale = extScale/totalStep;
    progress->setScaleX(progressStartScale);
    
    float time = 0;
    if(bScaleProgressBar && progressEndScale>0){
        time = 0.5 + (progressEndScale - progressStartScale) * 1;
        progress->runAction(CCSequence::create(CCDelayTime::create(0.5),CCScaleTo::create(time, progressEndScale,1),NULL));
    }
    if(bMoveBox && progressEndScale1>0 && moveOffset!=0){
        float delayT = time+0.5;
        CCPoint pos = containerNode->getPosition();
        containerNode->runAction(CCSequence::create(CCDelayTime::create(delayT),CCMoveTo::create(0.5, ccp(pos.x+moveOffset, pos.y)),NULL));
        auto box1 = containerNode->getChildByTag(endIndex-1);
        if(box1){
            box1->runAction(CCSequence::create(CCDelayTime::create(delayT+0.1),CCScaleTo::create(0.2, 0.6),NULL));
            box1->runAction(CCSequence::create(CCDelayTime::create(delayT+0.1),CCFadeIn::create(0.2),NULL));
        }
        auto box2 = containerNode->getChildByTag(endIndex);
        if(box2){
            box2->runAction(CCSequence::create(CCDelayTime::create(delayT+0.05),CCScaleTo::create(0.05, 0.2),CCDelayTime::create(0.2),CCScaleTo::create(0.2, 0.6),NULL));
            box2->runAction(CCSequence::create(CCDelayTime::create(delayT+0.3),CCFadeIn::create(0.2),NULL));
        }
        pos = progress->getPosition();
        float scaleT = (progressEndScale1 - progressEndScale) * 1;
        if(scaleT>0){
            progress->runAction(CCSequence::create(CCDelayTime::create(delayT),CCMoveTo::create(0.5, ccp(pos.x+moveOffset, pos.y)),CCDelayTime::create(0.5),CCScaleTo::create(scaleT, progressEndScale1, 1),NULL));
        }else{
            progress->runAction(CCSequence::create(CCDelayTime::create(delayT),CCMoveTo::create(0.5, ccp(pos.x+moveOffset, pos.y)),NULL));
        }
    }
    RepayController::getInstance()->resetChangeLV();
    if(m_bInit==false){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REPAY_INFO_INIT);
        m_bInit=true;
    }
}
//
// RepayCell
//
RepayCell* RepayCell::create(int index, CCNode* touchNode){
    RepayCell* ret = new RepayCell();
    if(ret && ret->init(index,touchNode)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
RepayCell::RepayCell():m_touchNode(NULL),m_index(-1){
    
}
void RepayCell::onEnter(){
    CCTableViewCell::onEnter();
}
void RepayCell::onExit(){
    CCTableViewCell::onExit();
}
bool RepayCell::init(int index, CCNode* touchNode){
    if (!CCTableViewCell::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("RepayCell",this,this);
    setContentSize(tmpCCB->getContentSize());
    m_touchNode = touchNode;
    mOrgSize = m_nodeReward->getContentSize();
    setData(index);
    return true;
}
bool RepayCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeReward", CCNode*, this->m_nodeReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCScale9Sprite*, this->m_sprBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle", CCLabelIF*, this->m_lblTitle);
    return false;
}
void RepayCell::setData(int index){
    m_nodeReward->removeAllChildren();
    m_index = index;
    if(m_index > (RepayController::getInstance()->repayRewards.size()-1))
        return;
    auto rStruct = RepayController::getInstance()->repayRewards.at(m_index);
    
    int max = rStruct.rewards.size();
    int rowCount = ceil(max*1.0/REPAY_REWARD_MAX_COUNT);
    int addH = (rowCount - 1) * (REPAY_ITEM_CELL_H+5);
    m_nodeReward->setContentSize(CCSize(mOrgSize.width, mOrgSize.height + addH));
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    setContentSize(CCSize(winSize.width, mOrgSize.height + addH + 50));
    m_sprBG->setCapInsets(CCRect(30, 25, 10, 10));
    m_sprBG->setPreferredSize(CCSize(winSize.width-20, mOrgSize.height + addH + 45));
    
    m_sprBG->setPositionX(10);
    m_lblTitle->setPositionX(m_sprBG->getPositionX() + 15);
    m_lblTitle->setPositionY(m_nodeReward->getPositionY() + mOrgSize.height + addH);
    m_nodeReward->setPositionX(m_lblTitle->getPositionX());
    
    int startX = 0;
    int startY = mOrgSize.height + addH - 100;
    for (int curIndex = 0; curIndex<max; curIndex++) {
        auto tmpStruct = rStruct.rewards.at(curIndex);
        int col = curIndex%REPAY_REWARD_MAX_COUNT;
        int row = floor(curIndex*1.0/REPAY_REWARD_MAX_COUNT);
        float tmpX = startX + col*(REPAY_ITEM_CELL_H+5);
        float tmpY = startY - row*(REPAY_ITEM_CELL_H+5);
        
        auto item = RepayItemCell::create(tmpStruct.itemid, tmpStruct.type, tmpStruct.count);
        m_nodeReward->addChild(item);
        item->setPosition(ccp(tmpX, tmpY));
    }
    string title = _lang_2("133258", CC_ITOA(index), CC_CMDITOA(rStruct.point).c_str());
    if(title.empty()){
        m_lblTitle->setString(std::string("reward ") + CC_ITOA(index));
    }else{
        m_lblTitle->setString(title);
    }
}


//
// RepayItemCell
//
RepayItemCell* RepayItemCell::create(std::string itemid,int type, int num){
    RepayItemCell* ret = new RepayItemCell();
    if(ret && ret->init(itemid,type,num)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
RepayItemCell::RepayItemCell(){
    _ignoreAnchorPointForPosition = false;
    setAnchorPoint(Vec2(0,0));
}
bool RepayItemCell::init(std::string itemid,int type, int num){
    if (!CCNode::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("RepayItemCell",this,this);
    setContentSize(tmpCCB->getContentSize());
    m_nodeIcon->removeAllChildren();
    m_lblNum->setString(CC_CMDITOA(num));
    switch (type) {
        case R_GOODS:{
            int tID =atoi(itemid.c_str());
            auto &info = ToolController::getInstance()->getToolInfoById(tID);
            toolName = info.getName();
            toolDes = _lang(info.des.c_str());
            CCCommonUtils::createGoodsIcon(tID, m_nodeIcon, CCSize(75, 75));
            break;
        }
        case R_EQUIP:{
            int eID = atoi(itemid.c_str());
            auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[eID];
            string ebgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
            auto ebgIcon = CCLoadSprite::createSprite(ebgPath.c_str());
            CCCommonUtils::setSpriteMaxSize(ebgIcon, 75, true);
            m_nodeIcon->addChild(ebgIcon);
            string epicStr = CCCommonUtils::getIcon(itemid);
            auto eicon = CCLoadSprite::createSprite(epicStr.c_str(),true,CCLoadSpriteType_EQUIP);
            CCCommonUtils::setSpriteMaxSize(eicon, 70, true);
            m_nodeIcon->addChild(eicon);
            toolName = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
            toolDes = _lang(eInfo.des);
            break;
        }
        default:{
            int oID = atoi(itemid.c_str());
            auto pic = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(oID).c_str());
            if(pic){
                CCSprite* iconBg = NULL;
                if(oID == Gold){
                    iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(BLUE).c_str());
                }else{
                    iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(WHITE).c_str());
                }
                CCCommonUtils::setSpriteMaxSize(iconBg, 75,true);
                m_nodeIcon->addChild(iconBg);
                CCCommonUtils::setSpriteMaxSize(pic, 60,true);
                m_nodeIcon->addChild(pic);
            }
            toolName = CCCommonUtils::getResourceNameByType(oID);
            toolDes = "";
            break;
        }
    }
    return true;
}
bool RepayItemCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeIcon", CCNode*, this->m_nodeIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblNum", CCLabelIF*, this->m_lblNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCSprite*, this->m_sprBG);
    return false;
}
void RepayItemCell::onEnter(){
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, true);
    CCNode::onEnter();
}
void RepayItemCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}
bool RepayItemCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_sprBG, pTouch)){
        return true;
    }
    return false;
}
void RepayItemCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void RepayItemCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_sprBG, pTouch)){
        CCArray *array = CCArray::create();
        array->addObject(CCString::create(toolName));
        array->addObject(CCString::create(toolDes));
        CCPoint point = m_sprBG->getParent()->convertToWorldSpace(m_sprBG->getPosition());
        array->addObject(CCInteger::create(point.x));
        array->addObject(CCInteger::create(point.y));
        array->retain();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REPAY_VIEW_SHOW_DES,array);
    }
}

//RepayIcon

RepayIcon* RepayIcon::create(){
    RepayIcon* ret = new RepayIcon();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
RepayIcon::RepayIcon(){
    
}
RepayIcon::~RepayIcon(){
    this->unschedule(schedule_selector(RepayIcon::onTimer));
}
bool RepayIcon::init(){
    if (!CCNode::init()) {
        return false;
    }
    auto tmpCCB = CCBLoadFile("RepayIcon",this,this);
    setContentSize(tmpCCB->getContentSize());
    onTimer(0);
    this->schedule(schedule_selector(RepayIcon::onTimer),1.0);
    return true;
}
bool RepayIcon::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniNode", CCNode*, this->m_aniNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTime", CCLabelIF*, this->m_lblTime);
    return false;
}
void RepayIcon::showPartical(bool bshow){
    if(m_aniNode){
        if(bshow && m_aniNode->getChildrenCount()==0){
            auto particle1 = ParticleController::createParticle("Rose_3");
            m_aniNode->addChild(particle1);
            auto particle2 = ParticleController::createParticle("VIPGlow_3");
            m_aniNode->addChild(particle2);
        }else if(!bshow){
            m_aniNode->removeAllChildren();
        }
    }
}
void RepayIcon::onTimer(float u){
    double endT = RepayController::getInstance()->getEndTime();
    int lastTime = GlobalData::shared()->changeTime(endT) - GlobalData::shared()->getWorldTime();
    if(lastTime>0){
        m_lblTime->setString(CC_SECTOA(lastTime));
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REPAY_INFO_INIT);
    }
}