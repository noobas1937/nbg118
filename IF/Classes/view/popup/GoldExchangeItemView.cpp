//
//  GoldExchangeItemView.cpp
//  IF
//
//  Created by wangzhenlei on 13-12-17.
//
//

#include "GoldExchangeItemView.h"
#include "PayController.h"
#include "DataRecordCommand.h"
#include "PaymentCommand.h"
#include "GoldExchangeInfoCommand.h"
#include "RewardController.h"
#include "ToolController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "SoundController.h"

GoldExchangeItemView* GoldExchangeItemView::create(GoldExchangeItem *goldExchangeItem){
    GoldExchangeItemView* ret = new GoldExchangeItemView(goldExchangeItem);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeItemView::init()
{
    CCBLoadFile("GoldExchangeItemViewNew",this,this,false);
    if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
    {
        this->setContentSize(CCSizeMake(310*2.4, 210*1.8));
    }
    else
        this->setContentSize(CCSizeMake(310, 220));
    refreshData();
    return true;
}
void GoldExchangeItemView::refreshData(CCObject* p){
    // reset data by sale
    if(m_dataItem){
        m_currdataItem =m_dataItem;
        for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
            if(!it->second)
                continue;
            if(it->second->type == "1"||it->second->bought||it->second->popup_image=="new_recharge"){
                continue;
            }
            if(m_currdataItem->dollar==it->second->dollar){//同价格的礼包，用高级别的覆盖
                if(it->second->type == "2"||(it->second->type == "3"&&it->second->popup_image=="close")){//用popup值大的替换
                    if (it->second->popup>m_currdataItem->popup) {
                        m_currdataItem = it->second;
                    }
                }
            }
        }
        m_cellNode1->setVisible(false);
        m_cellNode2->setVisible(false);
        m_cellNode3->setVisible(false);
        m_iconNode1->removeAllChildren();
        m_iconNode2->removeAllChildren();
        m_iconNode3->removeAllChildren();
        m_addFlagText2->setString("+");
        m_addFlagText3->setString("+");
        
        int sprid =  atoi((m_dataItem->product_id.substr(m_dataItem->product_id.find("_")+1)).c_str());
        if(sprid>5){
            sprid = 5;
        }
        string strID = CC_ITOA(sprid);
        string boxName = "gift000" + strID + ".png";
        CCSprite* sprite = CCLoadSprite::createSprite(boxName.c_str());
        if(m_currdataItem->type == "2"){
            m_cellNode2->setVisible(true);
            string dollar = PayController::getInstance()->getDollarText(m_currdataItem->dollar,m_currdataItem->product_id);
            this->m_dollerNum2->setString(dollar.c_str());
            this->m_goldNum2->setString(CC_CMDITOA(atoi(m_currdataItem->gold_doller.c_str())-m_currdataItem->price).c_str());
            this->m_goldAddNum2->setString(CC_CMDITOA(m_currdataItem->price));
            m_desText2->setString(_lang("101238"));
            m_saleText2->setString(_lang("101224"));
            m_iconNode2->addChild(sprite);
        }else if(m_currdataItem->type == "3"){
            m_cellNode3->setVisible(true);
            string dollar = PayController::getInstance()->getDollarText(m_currdataItem->dollar,m_currdataItem->product_id);
            this->m_goldNum3->setString(CC_CMDITOA(atoi(m_currdataItem->gold_doller.c_str())-m_currdataItem->price).c_str());
            this->m_goldAddNum3->setString(CC_CMDITOA(m_currdataItem->price));
            this->m_dollerNum3->setString(dollar.c_str());
            m_desText3->setString(_lang("101237"));
            m_saleText3->setString(_lang("101224"));
             m_iconNode3->addChild(sprite);
        }else{
            if(m_currdataItem->price>0){
                m_cellNode2->setVisible(true);
                string dollar = PayController::getInstance()->getDollarText(m_currdataItem->dollar,m_currdataItem->product_id);
                this->m_dollerNum2->setString(dollar.c_str());
                this->m_goldNum2->setString(CC_CMDITOA(atoi(m_currdataItem->gold_doller.c_str())-m_currdataItem->price).c_str());
                this->m_goldAddNum2->setString(CC_CMDITOA(m_currdataItem->price));
                m_desText2->setString(_lang("101236"));
                m_saleText2->setString(_lang("101224"));
                m_iconNode2->addChild(sprite);
            }else{
                m_cellNode1->setVisible(true);
                string dollar = PayController::getInstance()->getDollarText(m_currdataItem->dollar,m_currdataItem->product_id);
                this->m_dollerNum1->setString(dollar.c_str());
                this->m_goldNum1->setString(CC_CMDITOA(atoi(m_currdataItem->gold_doller.c_str())).c_str());
                m_iconNode1->addChild(sprite);
            }
            
        }
    }else{
        m_cellNode2->setVisible(false);
        m_cellNode3->setVisible(false);
        m_iconNode1->removeAllChildren();
        m_iconNode2->removeAllChildren();
        m_iconNode3->removeAllChildren();
        m_cellNode1->setVisible(true);
        m_dollerNum1->setString(_lang("101026"));
        m_goldNum1->setString("");
        if(GlobalData::shared()->analyticID=="mycard"){
            CCSprite *sprite = CCLoadSprite::createSprite("mycard.png");
            m_iconNode1->addChild(sprite);
            sprite->setPositionX(60);
        }else if(GlobalData::shared()->analyticID=="gash"){
            CCSprite *sprite = CCLoadSprite::createSprite("cash.png");
            m_iconNode1->addChild(sprite);
            sprite->setPositionX(60);
        }
    }
}
void GoldExchangeItemView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeItemView::refreshData), PAYMENT_COMMAND_RETURN, NULL);
}

void GoldExchangeItemView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PAYMENT_COMMAND_RETURN);
    setTouchEnabled(false);
    GlobalData::shared()->isBind = false;
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeItemView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
   // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRecruitShowBtnClick", GeneralControlRender::onRecruitShowBtnClick);
    return NULL;
}

bool GoldExchangeItemView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum1", CCLabelBMFont*, this->m_goldNum1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dollerNum1", CCLabelIF*, this->m_dollerNum1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode1", CCNode*, this->m_iconNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellNode1", CCNode*, this->m_cellNode1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum2", CCLabelBMFont*, this->m_goldNum2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dollerNum2", CCLabelIF*, this->m_dollerNum2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saleText2", CCLabelIF*, this->m_saleText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode2", CCNode*, this->m_iconNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellNode2", CCNode*, this->m_cellNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldAddNum2", CCLabelIF*, this->m_goldAddNum2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum3", CCLabelBMFont*, this->m_goldNum3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dollerNum3", CCLabelIF*, this->m_dollerNum3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saleText3", CCLabelIF*, this->m_saleText3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode3", CCNode*, this->m_iconNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellNode3", CCNode*, this->m_cellNode3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addFlagText2", CCLabelIF*, m_addFlagText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desText2", CCLabelIF*, this->m_desText2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addFlagText3", CCLabelIF*, this->m_addFlagText3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desText3", CCLabelIF*, this->m_desText3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldAddNum3", CCLabelIF*, this->m_goldAddNum3);
    return false;
}

bool GoldExchangeItemView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchLocation = pTouch->getLocation();
    if(isTouchInside(this->m_touchNode, pTouch) ) {
        this->setScale(0.98);
//        int itemId = atoi(m_dataItem->id.c_str());
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        PayController::getInstance()->payGoogle(itemId-8999);
//#endif
//        CCArray* p = CCArray::create();
//        p->addObject(CCInteger::create(GOLD_BUTTON));
//        CCArray* d = CCArray::create();
//        d->addObject(CCInteger::create(itemId - 8889));
//
//        DataRecordCommand* cmd = new DataRecordCommand(OPEN_PANEL, p, d);
//        cmd->sendAndRelease();
        return true;
    }
    return false;
}
void GoldExchangeItemView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    this->setScale(1);
    if (fabs(m_touchLocation.y - pTouch->getLocation().y) > 30 || fabs(m_touchLocation.x - pTouch->getLocation().x) > 30) {
        return;
    }
    if(isTouchInside(this->m_touchNode, pTouch) ) {
        if(m_currdataItem){
            if(m_currdataItem->type=="3"&&m_currdataItem->isPayCallBack){
                return;
            }
            PayController::getInstance()->callPayment(m_currdataItem,"GoldExchangeItemView");
        }else{
            PayController::getInstance()->callPayment(NULL,"GoldExchangeItemView");
        }
    }
}
void GoldExchangeItemView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}


void GoldExchangeItemView::onSpeedBtnClick(CCObject* pSender,Control::EventType pCCControlEvent){
    
}

void GoldExchangeItemView::goToExchangeCommamand(){
//    GeneRecruitFinishCommand* cmd=new GeneRecruitFinishCommand(m_data.generalId,2);
//    cmd->sendAndRelease();
    
}

