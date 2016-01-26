//
//  ActivityRewardView.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#include "ActivityRewardView.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "AllianceFightBulletinCommand.h"

ActivityRewardView *ActivityRewardView::create(CCArray* data,int type){
    ActivityRewardView *ret = new ActivityRewardView(data,type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ActivityRewardView::onEnter(){
    switch (m_type) {
        case 0:
            m_titleTxt->setString(_lang("105821"));
            break;
        case 1:
            m_titleTxt->setString(_lang("105822"));
            break;
        case 2:
            m_titleTxt->setString(_lang("133026"));
            break;
        case 3:
            m_titleTxt->setString(_lang("133062"));
            break;
        case 4:
            m_titleTxt->setString(_lang("138004"));
            break;
        case 5:
            m_titleTxt->setString(_lang("138005"));
            break;
        case 6:
            m_titleTxt->setString(_lang("105843"));
            break;
        case 7:
            m_titleTxt->setString(_lang("105821"));
            break;
        case 8:
            m_titleTxt->setString(_lang("105821"));
            break;
        default:
            m_titleTxt->setString(_lang("105822"));
            break;
    }

    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}

void ActivityRewardView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool ActivityRewardView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_viewBg, pTouch)){
        m_needclose = true;
    }
    return true;
}

void ActivityRewardView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(m_needclose)
    {
        m_needclose = false;
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void ActivityRewardView::onCloseClick(CCObject *pSender)
{
    m_needclose = false;
    PopupViewController::getInstance()->removePopupView(this);
}

bool ActivityRewardView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto node = CCBLoadFile("ActivityRewardView", this, this);
        this->setContentSize(node->getContentSize());
        
        this->setModelLayerOpacity(0);

        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(-50);
        m_infoList->addChild(m_tabView);
        
        m_tabView->reloadData();
        ret = true;
    }
    return ret;
}


bool ActivityRewardView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    return false;
}

SEL_CCControlHandler ActivityRewardView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

cocos2d::SEL_MenuHandler ActivityRewardView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCloseClick", ActivityRewardView::onCloseClick);
    return NULL;
}

CCSize ActivityRewardView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    auto oneDic = _dict(m_data->objectAtIndex(idx));
    CCArray* arr = dynamic_cast<CCArray*>(oneDic->objectForKey("reward"));
    if(arr && arr->count()>0){
        return CCSize(600.0, arr->count()*85+20+20);
    }
    return CCSizeZero;
}

CCSize ActivityRewardView::cellSizeForTable(CCTableView *table)
{
    return CCSize(600.0, 109);
}

CCTableViewCell* ActivityRewardView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    ActivityRankReward* cell = (ActivityRankReward*)table->dequeueCell();
    
    auto oneDic = _dict(m_data->objectAtIndex(idx));
    CCArray* arr = dynamic_cast<CCArray*>(oneDic->objectForKey("reward"));
    std::string tempStr = _lang_1("105828",CC_ITOA((long)(idx+1)));
    if (m_type==5 || m_type==7 || m_type==8) {
        //todo
        int min = oneDic->valueForKey("min")->intValue();
        int max = oneDic->valueForKey("max")->intValue();
        if(min!=max){
            string t("");
            t.append(CC_ITOA(min));
            t.append("~");
            t.append(CC_ITOA(max));
            
            tempStr = _lang_1("105828", t.c_str());
        }else{
            tempStr = _lang_1("105828",CC_ITOA(min));
        }
    } else if (m_type == 6)
    {
     tempStr = "";
    }
    if(idx < m_data->count()){
        if(cell){
            cell->setData(arr,tempStr,true,m_type);
        }else{
            cell = ActivityRankReward::create(arr,tempStr,true,m_type);
        }
    }
    return cell;
}

ssize_t ActivityRewardView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void ActivityRewardView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

//

ActivityRewardCell *ActivityRewardCell::create(AllianceFightInfo* info,int index){
    ActivityRewardCell *ret = new ActivityRewardCell(info,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ActivityRewardCell::onEnter(){
    CCNode::onEnter();
}

void ActivityRewardCell::onExit(){
    CCNode::onExit();
}

bool ActivityRewardCell::init(){
    auto node = CCBLoadFile("AllianceWarResultCell", this, this);
    this->setContentSize(node->getContentSize());
    setData(m_info,m_index);
    return true;
}

void ActivityRewardCell::setData(AllianceFightInfo* info,int index){
    m_info = info;
    m_index = index;
    std::string uid = GlobalData::shared()->playerInfo.allianceInfo.uid;
    int flag = m_index%2==0;
    m_bg1->setVisible(flag);
    m_bg2->setVisible(!flag);
}

bool ActivityRewardCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg1", CCScale9Sprite*, this->m_bg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg2", CCScale9Sprite*, this->m_bg2);
    return false;
}

SEL_CCControlHandler ActivityRewardCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}