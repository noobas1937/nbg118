//
//  LianShengView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/6.
//
//

#include "LianShengView.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "WorldController.h"
#include "CKFStreakListCommand.h"
#include "SceneController.h"
#include "WorldMapView.h"

LianShengView *LianShengView::create(){
    LianShengView *ret = new LianShengView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void LianShengView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
     //   CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void LianShengView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    PopupBaseView::onExit();
}

bool LianShengView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void LianShengView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void LianShengView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>35){
        return;
    }
    if (!isTouchInside(m_viewBg, pTouch)) {
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool LianShengView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto node = CCBLoadFile("LianShengView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        this->setModelLayerOpacity(70);
        m_data = CCArray::create();
    
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        CKFStreakListCommand* cmd = new CKFStreakListCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(LianShengView::getServerData), NULL));
        cmd->sendAndRelease();
        
        ret = true;
    }
    return ret;
}

void LianShengView::getServerData(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = (CCArray*)dic->objectForKey("list");
        if(array){
            m_data->addObjectsFromArray(array);
        }
        m_tabView->reloadData();
    }
    if(m_data->count()>0){
        m_tip->setString("");
    }else{
        m_tip->setString(_lang("138130"));
        
    }
}

bool LianShengView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip", CCLabelIF*, this->m_tip);
    return false;
}

SEL_CCControlHandler LianShengView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

CCSize LianShengView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    return CCSize(640, 60);
}

CCSize LianShengView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 60);
}

CCTableViewCell* LianShengView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    LianShengCell* cell = (LianShengCell*)table->dequeueCell();
    CCDictionary* info = (CCDictionary*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = LianShengCell::create(info,m_infoList);
        }
    }
    return cell;
}

ssize_t LianShengView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void LianShengView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

//

LianShengCell *LianShengCell::create(CCDictionary* info,CCNode* clickArea){
    LianShengCell *ret = new LianShengCell(info,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void LianShengCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void LianShengCell::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool LianShengCell::init(){
    auto node = CCBLoadFile("LianShengCell", this, this);
    this->setContentSize(node->getContentSize());
    m_richLabel= CCRichLabelTTF::create("", "Helvetica", 22,CCSize(500,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    m_richLabel->setPositionY(30);
    m_richLabel->setPositionX(250);
    m_clickNode->addChild(m_richLabel);
    setData(m_info);
    return true;
}

void LianShengCell::setData(CCDictionary* info){
    m_info = info;
    if (m_info==NULL) {
        return;
    }
    string name = info->valueForKey("name")->getCString();
    string abbr = info->valueForKey("abbr")->getCString();
    string serverId = info->valueForKey("serverId")->getCString();
    string positionId = info->valueForKey("positionId")->getCString();
    int pointId = info->valueForKey("pointId")->intValue();
    int streakNum = info->valueForKey("streakNum")->intValue();
    CCPoint pos = WorldController::getInstance()->getPointByIndex(pointId);
    string temp = "";
    if (abbr!="") {
        temp.append("(");
        temp.append(abbr);
        temp.append(")");
    }
    temp.append(name);
    temp.append("#");
    temp.append(serverId);

    string tip = "";
    string posStr = "(";
    posStr.append(CC_ITOA(pos.x));
    posStr.append(",");
    posStr.append(CC_ITOA(pos.y));
    posStr.append(")");

    string t1 = "[color=ff7491BC]" + temp + "[/color]";
    string t2 = "[color=ffE37C45]" + string(CC_ITOA(streakNum)) + "[/color]";
    string t3 = "[u][color=ff80A26E][link bg=00000000 bg_click=ff80A26E]" + posStr + "[/link][/color][/u]";
    string infoStr = _lang_3("138127", t1.c_str(), t2.c_str(), t3.c_str());
    m_richLabel->setString(infoStr.c_str());
}

bool LianShengCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void LianShengCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>35 || m_info==NULL){
        return;
    }
    if (isTouchInside(m_clickNode, pTouch) && isTouchInside(m_clickArea, pTouch) && m_info->objectForKey("pointId")) {
        int pointId = m_info->valueForKey("pointId")->intValue();
        CCLOG("pointId=%d",pointId);
        CCPoint pt = WorldController::getPointByIndex(pointId);
        PopupViewController::getInstance()->removeAllPopupView();
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD && WorldMapView::instance()){
            WorldMapView::instance()->gotoTilePoint(pt);
            WorldMapView::instance()->openTilePanel(pointId);
        }else{
            WorldController::getInstance()->openTargetIndex = pointId;
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, pointId);
        }
    }
}

bool LianShengCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt", CCLabelIF*, this->m_txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    return false;
}

SEL_CCControlHandler LianShengCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}