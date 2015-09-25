//
//  DragonManageView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/26.
//
//

#include "DragonManageView.h"
#include "PopupViewController.h"
#include "CommandBase.h"
#include "YesNoDialog.h"
#include "DragonBattlePlayerInfoCommand.h"
#include "DragonBattleAlliancePlayCommand.h"

DragonManageView *DragonManageView::create(){
    DragonManageView *ret = new DragonManageView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonManageView::onEnter(){
    this->setTitleName(_lang("140044"));
    PopupBaseView::onEnter();
}

void DragonManageView::onExit(){
    PopupBaseView::onExit();
}

bool DragonManageView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void DragonManageView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

bool DragonManageView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(506, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(506, false);
        });
        auto node = CCBLoadFile("DragonManageView", this, this);
        this->setContentSize(node->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        this->changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height + add));
        m_infoList->setPositionY(m_infoList->getPositionY() - add);
        
        m_txt1->setString(_lang_2("140045","3","5"));
        m_txt2->setString(_lang_1("140046","3"));
        m_txt3->setString(_lang("140047").c_str());
        m_txt4->setString(_lang("140038").c_str());
        m_txt5->setString(_lang("140048").c_str());
        
        m_data = CCArray::create();
        m_data->addObject(CCDictionary::create());
        m_data->addObject(CCDictionary::create());
        m_data->addObject(CCDictionary::create());
        m_data->addObject(CCDictionary::create());
        
        m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        m_tabView->reloadData();
        
        DragonBattlePlayerInfoCommand* cmd = new DragonBattlePlayerInfoCommand();
        cmd->sendAndRelease();
        
        ret = true;
    }
    return ret;
}

void DragonManageView::getData(CCObject *param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        
    }
}


bool DragonManageView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt4", CCLabelIF*, this->m_txt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt5", CCLabelIF*, this->m_txt5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleNode", CCNode*, this->m_titleNode);
    return false;
}

SEL_CCControlHandler DragonManageView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

void DragonManageView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize DragonManageView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(640, 146);
}

CCTableViewCell* DragonManageView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    DragonMemberCell* cell = (DragonMemberCell*)table->dequeueGrid();
    CCDictionary* dic = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData();
        }else{
            cell = DragonMemberCell::create();
        }
    }
    return cell;
}

ssize_t DragonManageView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_data->count();
}

unsigned int DragonManageView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

//
DragonMemberCell *DragonMemberCell::create(){
    DragonMemberCell *ret = new DragonMemberCell();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonMemberCell::setData(){
    m_txt1->setString(_lang("R2"));
    m_txt2->setString("test");
    m_txt3->setString("9999");
    CCCommonUtils::setButtonTitle(m_battleBtn, _lang("140049").c_str());
}

void DragonMemberCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
}

void DragonMemberCell::onExit(){
    setTouchEnabled(false);
   
    CCNode::onExit();
}

bool DragonMemberCell::init(){
    auto bg = CCBLoadFile("DragonMemberCell", this, this);
    this->setContentSize(bg->getContentSize());
    setData();
    return true;
}

bool DragonMemberCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

    return true;
}

void DragonMemberCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    
}

void DragonMemberCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

void DragonMemberCell::onClickBatlle(CCObject *pSender, CCControlEvent event){
    DragonBattleAlliancePlayCommand* cmd = new DragonBattleAlliancePlayCommand("",1);
    cmd->sendAndRelease();
}

bool DragonMemberCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleBtn", CCControlButton*, this->m_battleBtn);
    
    return false;
}

SEL_CCControlHandler DragonMemberCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onManageClick", DragonMemberCell::onClickBatlle);
    return NULL;
}