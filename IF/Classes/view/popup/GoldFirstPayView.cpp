//
//  GoldFirstPayView.cpp
//  IF
//
//  Created by lifangkai on 14-10-17.
//
//

#include "GoldFirstPayView.h"
#include "PayController.h"
#include "DataRecordCommand.h"
#include "ToolController.h"
#include "ToolInfo.h"
#include "FreeListView.h"
#include "PopupViewController.h"
#include "PaymentCommand.h"
#include "GoldExchangeMoreListView.h"
#include "EquipmentController.h"

GoldFirstPayView* GoldFirstPayView::create(int parentType){
    GoldFirstPayView* ret = new GoldFirstPayView(parentType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldFirstPayView::init()
{
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    
    CCBLoadFile("GoldFirstPayView",this,this);

    this->setContentSize(CCSizeMake(640, 480));
    
    
    m_titleLabel->setString(_lang("101227").c_str());

  //  m_titleLabel->setString(_lang(m_dataItem->name).c_str());
    //  CCCommonUtils::setButtonTitle(m_moreBtn, _lang("102162").c_str());
    m_moreLabel->setString(_lang("101228").c_str());

    m_scrollView = CCScrollView::create(m_listNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Default);
    m_listNode->addChild(m_scrollView);
    
    
    


    m_scrollView->setTouchEnabled(false);

    int curY = 0;
    CCArray* arr = GlobalData::shared()->payRewardInfo.firstPayReward;
    if (arr!= NULL) {
        CCObject* obj;
        CCARRAY_FOREACH(arr, obj){
            auto dic = _dict(obj);
            int type = dic->valueForKey("type")->intValue();
            auto dict1 = _dict(dic->objectForKey("value"));
            int num = 0;
            if(type==7){
                type = dict1->valueForKey("id")->intValue();
                num = dict1->valueForKey("num")->intValue();
            }else{
                num = dic->valueForKey("num")->intValue();
            }
            auto cell = GoldFirstPayRewardCell::create(type,num,0);
            cell->setPosition(ccp(0, curY));
            m_scrollView->addChild(cell);
            curY += 60;
        }
    }
    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - curY));
    
    return true;
}

void GoldFirstPayView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void GoldFirstPayView::onExit()
{
    setTouchEnabled(false);
    GlobalData::shared()->isBind = false;
    CCNode::onExit();
}

SEL_CCControlHandler GoldFirstPayView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{

    return NULL;
}

bool GoldFirstPayView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreLabel", CCLabelIF*, this->m_moreLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchMoreNode", CCNode*, this->m_touchMoreNode);

    return false;
}

bool GoldFirstPayView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    return false;
}
void GoldFirstPayView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {

}
void GoldFirstPayView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

}

void GoldFirstPayView::setParentScrollView(CCScrollView* parentScrollView)
{
    m_parentScrollView = parentScrollView;
}

////////
GoldFirstPayRewardCell* GoldFirstPayRewardCell::create(int itemId, int num,int type)
{
    auto ret = new GoldFirstPayRewardCell();
    if (ret && ret->init(itemId,num,type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldFirstPayRewardCell::init(int itemId, int num,int type)
{

    CCBLoadFile("GoldFirstPayCell",this,this);
    string name ="";
    string picStr = "";
    if(type==0){
        if (itemId < 10) {// 资源
            picStr = CCCommonUtils::getResourceIconByType(itemId);
            name = CCCommonUtils::getResourceNameByType(itemId);
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(2).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 55,true);
            m_iconNode->addChild(iconBg);
            auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
            CCCommonUtils::setSpriteMaxSize(icon, 52, true);
            m_iconNode->addChild(icon);
            m_nameLabel->setString(name);
            m_numLabel->setString(CC_ITOA(num));
        }
        else {//道具
            CCCommonUtils::createGoodsIcon(itemId, m_iconNode, CCSize(55, 55),NULL,m_nameLabel);
            m_numLabel->setString(CC_ITOA(num));
//            auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
//            name = _lang(toolInfo.name);
//            picStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
//            
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 55,true);
//            m_iconNode->addChild(iconBg);
        }
    }else if(type == 1){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[itemId];
        name = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
        string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        auto iconBg = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 55, true);
        m_iconNode->addChild(iconBg);
        picStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
        auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, 52, true);
        m_iconNode->addChild(icon);
    }
    
    bool ret = true;
    return ret;
}

void GoldFirstPayRewardCell::onEnter() {
    CCNode::onEnter();
}

void GoldFirstPayRewardCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler GoldFirstPayRewardCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool GoldFirstPayRewardCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    return false;
}