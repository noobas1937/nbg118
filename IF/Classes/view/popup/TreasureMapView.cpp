
#include "TreasureMapView.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "WorldCommand.h"
#include "CCMathUtils.h"
#include "YesNoDialog.h"
#include "TipsView.h"
#include "QueueController.h"

//#define TREASURE_MAP_NODE_WIDTH 450

TreasureMapView::TreasureMapView():mTotalPage(0),mPage(0),m_editOpen(false),mWaitInterface(NULL),mItemid(0),m_inited(false)/*,mMapNode(NULL),mClipNode(NULL)*/{
}
TreasureMapView::~TreasureMapView(){
    
}
TreasureMapView* TreasureMapView::create(int color,int itemid){
    TreasureMapView* ret = new TreasureMapView();
    if(ret && ret->init(color,itemid)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}
bool TreasureMapView::init(int color,int itemid){
    if(!PopupBaseView::init()){
        return false;
    }
    mItemid = itemid;
    mColor = color;
    CCLoadSprite::doResourceByCommonIndex(306, true);
    CCLoadSprite::doResourceByCommonIndex(508, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(306, false);
        CCLoadSprite::doResourceByCommonIndex(508, false);
    });
    
    auto bg = CCBLoadFile("TreasureMapView", this, this,true);
    this->setContentSize(bg->getContentSize());
    
//    mClipNode = CCClipNode::create(m_nodeList->getContentSize());
//    m_nodeList->addChild(mClipNode);
//    mMapNode = CCNode::create();
//    mClipNode->addChild(mMapNode);
    
    auto editSize = m_editBoxNode->getContentSize();
    auto editpic = CCLoadSprite::createScale9Sprite("treasure_shuru.png");
    editpic->setContentSize(editSize);
    editpic->setInsetBottom(1);
    editpic->setInsetTop(1);
    editpic->setInsetRight(1);
    editpic->setInsetLeft(1);
    m_editBox = CCEditBox::create(editSize,editpic);
    m_editBox->setFontColor(ccc3(226,175,102));
    m_editBox->setInputMode(kEditBoxInputModeNumeric);
    m_editBox->setText("0");
    m_editBox->setDelegate(this);
   // m_editBox->setTouchPriority(Touch_Default);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_editBoxNode->addChild(m_editBox);
    
    CCCommonUtils::setSpriteGray(m_marchIcon, true);
    setButtonTitle(m_btnDestroy, _lang("111507").c_str());
    refreshData(NULL);
    
    return true;
}
void TreasureMapView::addTreasureNode(int idx,float posX){
//    TreasureMapNode *node = TreasureMapNode::create(mData.at(idx));
//    mMapNode->addChild(node);
//    node->setTag(idx);
//    node->setPosition(ccp(posX, 0));
    if(m_inited==false){
        if(mData.size()==0){
            WorldController::getInstance()->sendGetTreasureCMD(mItemid);
            mWaitInterface = GameController::getInstance()->showWaitInterface(m_nodePos[4],0);
            CCSize size0 = m_nodePos[4]->getContentSize();
            CCPoint addP = CCPoint(size0.width * 0.5, size0.height*0.5);
            mWaitInterface->setPosition(mWaitInterface->getPosition()+addP);
            m_btnDestroy->setEnabled(false);
            m_btnPoint->setEnabled(false);
            m_editBox->setEnabled(false);
            m_nextBtn->setEnabled(false);
            m_preBtn->setEnabled(false);
            return;
        }
        
        if(ToolController::getInstance()->m_toolInfos.find(mItemid)!=ToolController::getInstance()->m_toolInfos.end()){
            if( ToolController::getInstance()->m_toolInfos[mItemid].getCNT() > mData.size()){
                WorldController::getInstance()->sendGetTreasureCMD(mItemid);
            }
        }
        m_inited=true;
    }
    auto mInfo = mData.at(mPage);
    CCPoint ccccp = WorldController::getPointByIndex(mInfo.targetId);
    string tmpCCP = CC_ITOA(ccccp.x) + string(",")+ CC_ITOA(ccccp.y);
    if(isMarch(mInfo.targetId)){
        m_btnPoint->setEnabled(false);
        m_btnPoint->setPositionY(m_marchIcon->getPositionY());
        m_marchIcon->setVisible(true);
        m_btnDestroy->setVisible(false);
        setButtonTitle(m_btnPoint, " ");
    }else{
        m_btnPoint->setEnabled(true);
        m_marchIcon->setVisible(false);
        m_btnPoint->setPositionY(m_btnDestroy->getPositionY() + 90);
        m_btnDestroy->setVisible(true);
        setButtonTitle(m_btnPoint, tmpCCP.c_str());
        
    }

    //init star
    int startTag = 5000;
    int index = 1;
    while(index<6){
        int tag  = startTag+index;
        if(index>mInfo.colour){
            if(m_nodeItem->getChildByTag(tag)){
                m_nodeItem->removeChildByTag(tag);
            }
        }else{
            if(!m_nodeItem->getChildByTag(tag)){
                CCSprite *star = CCLoadSprite::createSprite("treasuremap_star.png");
                star->setTag(tag);
                m_nodeItem->addChild(star);
                star->setPosition(m_starSpr[index-1]->getPosition());
            }
        }
        ++index;
    }
    //random item
    int randomPos[6];
    string tmpX = tmpCCP.substr(0,tmpCCP.find(","));
    string tmpY = tmpCCP.substr(tmpCCP.find(",")+1);
    string xStr = "50";
    string yStr = "50";
    if(tmpX.size()>1){
        xStr = tmpX.substr(tmpX.size()-2);
    }else{
        xStr = tmpX;
    }
    if(tmpY.size()>1){
        yStr = tmpY.substr(tmpY.size()-2);
    }else{
        yStr = tmpY;
    }
    
    int xTmp = atoi(xStr.c_str());
    int yTmp = atoi(yStr.c_str());
    float scalePosX = xTmp / 100.0f;
    float scalePosY = yTmp / 100.0f;
    CCSize size = m_nodePos[4]->getContentSize();
    randomPos[0] = size.width * scalePosX;
    randomPos[1] = size.height * scalePosY;
    m_sprPos->setPosition(m_nodePos[4]->getPosition() + ccp(randomPos[0], randomPos[1]));
    size = m_nodePos[0]->getContentSize();
    randomPos[0] = MAX(size.width * scalePosX*0.33, 30);
    randomPos[1] = MAX(size.height * scalePosY*0.33, 30);
    randomPos[2] = MAX(size.width * scalePosX,randomPos[0]+100);
    randomPos[3] = MAX(size.height * scalePosY,randomPos[1]+80);
    randomPos[4] = size.width * scalePosX*0.65;
    randomPos[5] = size.height * scalePosY*0.65;
    float totalScale = scalePosX+scalePosY;
    CCNode *parent1=NULL;
    CCNode *parent2=NULL;
    CCNode *parent3=NULL;
    CCNode *parent4=NULL;
    if(totalScale<1){
        parent1=m_nodePos[0];
        parent2=m_nodePos[1];
        parent3=m_nodePos[2];
        parent4=m_nodePos[3];
        if(totalScale<0.35){
            m_boxSpr[0]->setVisible(false);
            m_boxSpr[1]->setVisible(true);
        }else if(totalScale<0.7){
            m_boxSpr[0]->setVisible(true);
            m_boxSpr[1]->setVisible(false);
        }else{
            m_boxSpr[0]->setVisible(true);
            m_boxSpr[1]->setVisible(true);
        }
        m_boxSpr[3]->setVisible(true);
    }else{
        parent1=m_nodePos[1];
        parent2=m_nodePos[0];
        parent3=m_nodePos[3];
        parent4=m_nodePos[2];
        if(totalScale<1.2){
            m_boxSpr[0]->setVisible(false);
            m_boxSpr[1]->setVisible(true);
        }else if(totalScale<1.4){
            m_boxSpr[0]->setVisible(true);
            m_boxSpr[1]->setVisible(false);
        }else{
            m_boxSpr[0]->setVisible(true);
            m_boxSpr[1]->setVisible(true);
        }
        m_boxSpr[3]->setVisible(true);
    }
    m_mineSpr[0]->setPosition(parent1->getPosition()+ccp(randomPos[0],randomPos[1]));
    m_mineSpr[1]->setPosition(parent1->getPosition()+ccp(randomPos[2],randomPos[3]));
    m_mineSpr[2]->setPosition(parent2->getPosition()+ccp(randomPos[4],randomPos[5]));
    m_boxSpr[0]->setPosition(parent3->getPosition()+ccp(randomPos[0],randomPos[1]));
    m_boxSpr[1]->setPosition(parent3->getPosition()+ccp(randomPos[2],randomPos[3]));
    m_boxSpr[2]->setPosition(parent4->getPosition()+ccp(randomPos[4],randomPos[5]));
    string text = CC_ITOA(mPage+1) + string("/") + CC_ITOA(mTotalPage);
    m_editBox->setText(text.c_str());
}
void TreasureMapView::refreshData(cocos2d::CCObject *ccObj){
    mData.clear();
    if(mWaitInterface){
        mWaitInterface->remove();
        mWaitInterface=NULL;
        m_btnDestroy->setEnabled(true);
        m_btnPoint->setEnabled(true);
        m_editBox->setEnabled(true);
        m_nextBtn->setEnabled(true);
        m_preBtn->setEnabled(true);
    }
    
    WorldController::getInstance()->getTreasureList(mColor, mData);
    mTotalPage = mData.size();
//    mMapNode->removeAllChildren();
    mPage = MIN(mPage,mTotalPage-1);
    mPage = MAX(mPage, 0);
//    if(mPage==0){
//        m_preBtn->setEnabled(false);
//    }else{
//        addTreasureNode(mPage-1, -TREASURE_MAP_NODE_WIDTH);
//    }
    
    addTreasureNode(mPage,0);
    
//    if(mPage == (mTotalPage-1)){
//        m_nextBtn->setEnabled(false);
//    }else{
//        addTreasureNode(mPage+1,TREASURE_MAP_NODE_WIDTH);
//    }
}
void TreasureMapView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TreasureMapView::refreshData), MSG_TREASURE_MAP_INFO_CHANGE, NULL);
    
}
void TreasureMapView::onExit(){
    if (mWaitInterface) {
        mWaitInterface->remove();
        mWaitInterface=NULL;
    }
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TREASURE_MAP_INFO_CHANGE);
    PopupBaseView::onExit();
}
bool TreasureMapView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_preBtn", CCControlButton*, this->m_preBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextBtn", CCControlButton*, this->m_nextBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeTouch", CCNode*, this->m_nodeTouch);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBoxNode", CCNode*, this->m_editBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnDestroy", CCControlButton*, this->m_btnDestroy);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnPoint", CCControlButton*, this->m_btnPoint);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle", CCLabelIF*, this->m_lblTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprPos", CCSprite*, this->m_sprPos);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeItem", CCNode*, this->m_nodeItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeTip", CCNode*, this->m_nodeTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchIcon", CCSprite*, this->m_marchIcon);
    
    int index = 0;
    while (index<5) {
        string tmpStr = string("m_nodePos") + CC_ITOA(index+1);
         CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, tmpStr.c_str(), CCNode*, this->m_nodePos[index]);
        ++index;
    }
    index=0;
    while (index<5) {
        string tmpStr = string("m_starSpr") + CC_ITOA(index+1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, tmpStr.c_str(), CCSprite*, this->m_starSpr[index]);
        ++index;
    }
    index=0;
    while (index<3) {
        string tmpStr = string("m_mineSpr") + CC_ITOA(index+1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, tmpStr.c_str(), CCSprite*, this->m_mineSpr[index]);
        ++index;
    }
    index=0;
    while (index<3) {
        string tmpStr = string("m_boxSpr") + CC_ITOA(index+1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, tmpStr.c_str(), CCSprite*, this->m_boxSpr[index]);
        ++index;
    }
    return true;
}
SEL_CCControlHandler TreasureMapView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNextBtnClick", TreasureMapView::onNextBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPreBtnClick", TreasureMapView::onPreBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPointBtnClick", TreasureMapView::onPointBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDestroyBtnClick", TreasureMapView::onDestroyBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", TreasureMapView::onCloseBtnClick);
    return NULL;
}
void TreasureMapView::onNextBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    int newPage = mPage+1;
    if(newPage > (mTotalPage-1)){
        newPage = 0;
    }
//    CCPoint nodePos = mMapNode->getPosition();
//    mMapNode->runAction(CCMoveTo::create(0.5, ccp(nodePos.x-TREASURE_MAP_NODE_WIDTH, nodePos.y)));
    mPage = newPage;
//    if(mPage>0){
//        m_preBtn->setEnabled(true);
//    }
    addTreasureNode(mPage,0);
    
//    newPage++;
//    if(newPage >= (mTotalPage-1)){
//        m_nextBtn->setEnabled(false);
//        return;
//    }
//    CCNode *pageNode = mMapNode->getChildByTag(mPage);
//    if(pageNode){
//        addTreasureNode(newPage, pageNode->getPositionX() + TREASURE_MAP_NODE_WIDTH);
//    }
}
void TreasureMapView::onPreBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    int newPage = mPage-1;
    if(newPage < 0){
        newPage = mTotalPage-1;
    }
//    CCPoint nodePos = mMapNode->getPosition();
//    mMapNode->runAction(CCMoveTo::create(0.5, ccp(nodePos.x+TREASURE_MAP_NODE_WIDTH, nodePos.y)));
    mPage = newPage;
//    if(mPage<(mTotalPage-1)){
//        m_nextBtn->setEnabled(true);
//    }
    addTreasureNode(mPage,0);
    
//    newPage--;
//    if(newPage == 0 || newPage < 0){
//        m_preBtn->setEnabled(false);
//        return;
//    }
//    CCNode *pageNode = mMapNode->getChildByTag(mPage);
//    if(pageNode){
//        addTreasureNode(newPage, pageNode->getPositionX() - TREASURE_MAP_NODE_WIDTH);
//    }
}
bool TreasureMapView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_editOpen){
        m_editOpen = false;
        return false;
    }
    
    return true;
}
void TreasureMapView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}
void TreasureMapView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (!isTouchInside(m_nodeTouch, pTouch)) {
        closeSelf();
    }
    if(isTouchInside(m_nodeTip, pTouch)){
        onClickTip();
    }
}
void TreasureMapView::onDestroyBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    YesNoDialog::show(_lang("111501").c_str(),CCCallFunc::create(this, callfunc_selector(TreasureMapView::onDestroyMap)));
}
void TreasureMapView::onDestroyMap(){
    auto mInfo = mData.at(mPage);
    WorldTreasureMapDestroyCommand *cmd = new WorldTreasureMapDestroyCommand(mInfo.uuid);
    cmd->sendAndRelease();
}
void TreasureMapView::onPointBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    auto mInfo = mData.at(mPage);
    int pos = mInfo.targetId;
    WorldController::getInstance()->openTargetIndex = pos;
    CCPoint pt = WorldController::getPointByIndex(pos);
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    this->closeSelf();
    PopupViewController::getInstance()->removeAllPopupView();
}
void TreasureMapView::onClickTip(){
//    std::string tipB = _lang("111500");
//    PopupViewController::getInstance()->addPopupView(TipsView::create(tipB));
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45211");
}
void TreasureMapView::onCloseBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    this->closeSelf();
}
void TreasureMapView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    if(text.find("/")!=string::npos)
        return;
    
    std::string temp  =text;
    size_t pos = temp.find_first_not_of("1234567890");
    while (pos!=std::string::npos) {
        temp.replace(pos, 1, "");
        pos = temp.find_first_not_of("1234567890");
    }
    editBox->setText(temp.c_str());
    
}
void TreasureMapView::editBoxReturn(CCEditBox *editBox){
    string numStr = m_editBox->getText();
    if(!numStr.empty()){
        size_t pos = numStr.find("/");
        if(pos==string::npos){
            numStr.append("/");
            numStr.append(CC_ITOA(mTotalPage));
            
            int a = atoi(numStr.c_str());
            a = MIN(a, mTotalPage);
            a = MAX(1, a);
            mPage = a-1;
            addTreasureNode(mPage, 0);
        }
    }
}
void TreasureMapView::editBoxEditingDidBegin(CCEditBox *editBox){
    m_editOpen = true;
    string numStr = m_editBox->getText();
    if(!numStr.empty()){
        size_t pos = numStr.find("/");
        if(pos!=string::npos){
            string curP = numStr.substr(0,pos);
            m_editBox->setText(curP.c_str());
        }
    }
}
bool TreasureMapView::isMarch(unsigned int targetidx){
    auto iter = WorldController::getInstance()->m_marchInfo.begin();
    while (iter!=WorldController::getInstance()->m_marchInfo.end()) {
        auto marchInfo = (*iter).second;
        if(marchInfo.endPointIndex == targetidx && marchInfo.ownerType == PlayerSelf){
            unsigned int qid= QueueController::getInstance()->getMarchQueueQid((*iter).first);
            auto iter0 = GlobalData::shared()->allQueuesInfo.find(qid);
            if(iter0!=GlobalData::shared()->allQueuesInfo.end()){
                return true;
            }
        }
        ++iter;
    }
    return false;
}
//////////////////////////////////////////////////////////////////////
//
//  TreasureMapNode
//
//////////////////////////////////////////////////////////////////////
//TreasureMapNode::TreasureMapNode(){
//}
//TreasureMapNode::~TreasureMapNode(){
//    
//}
//TreasureMapNode* TreasureMapNode::create(const TreasureInfo &info){
//    TreasureMapNode* ret = new TreasureMapNode();
//    if(ret && ret->init(info)){
//        ret->autorelease();
//    }else{
//        CC_SAFE_DELETE(ret);
//    }
//    return ret;
//    
//}
//bool TreasureMapNode::init(const TreasureInfo &info){
//    if(!CCNode::init()){
//        return false;
//    }
//    auto bg = CCBLoadFile("TreasureMapNode", this, this,true);
//    this->setContentSize(bg->getContentSize());
//    mInfo = info;
//    CCPoint ccccp = WorldController::getPointByIndex(mInfo.targetId);
//    string tmpCCP = CC_ITOA(ccccp.x) + string(",")+ CC_ITOA(ccccp.y);
//    m_lblPoint->setString(tmpCCP);
//    return true;
//}
//bool TreasureMapNode::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblPoint", CCLabelIF*, this->m_lblPoint);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnDestroy", CCControlButton*, this->m_btnDestroy);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
//    return true;
//}
//SEL_CCControlHandler TreasureMapNode::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDestroyBtnClick", TreasureMapNode::onDestroyBtnClick);
//    return NULL;
//}
//void TreasureMapNode::onEnter(){
//    CCNode::onEnter();
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
//}
//void TreasureMapNode::onExit(){
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//    CCNode::onExit();
//}
//bool TreasureMapNode::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//    m_touchPoint = pTouch->getLocation();
//    if(isTouchInside(m_touchNode, pTouch)) {
//        return true;
//    }
//    return false;
//}
//void TreasureMapNode::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 10) {
//        return;
//    }
//    int pos = mInfo.targetId;
//    WorldController::getInstance()->openTargetIndex = pos;
//    CCPoint pt = WorldController::getPointByIndex(pos);
//    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
//        WorldMapView::instance()->gotoTilePoint(pt);
//    }else{
//        int index = WorldController::getIndexByPoint(pt);
//        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
//    }
//    PopupViewController::getInstance()->removeAllPopupView();
//}
//void TreasureMapNode::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//    
//}
//void TreasureMapNode::onDestroyBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
//    WorldTreasureMapDestroyCommand *cmd = new WorldTreasureMapDestroyCommand(mInfo.uuid);
//    cmd->sendAndRelease();
//}