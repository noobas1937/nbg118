//
//  StoreMallCellTip.cpp
//  IF
//
//  Created by lifangkai on 14-12-1.
//
//

#include "StoreMallCellTip.h"
StoreMallCellTip* StoreMallCellTip::create(string msg,string title,float dy,CCTextAlignment align) {
    auto ret = new StoreMallCellTip();
    if (ret && ret->init(msg,title,dy,align)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool StoreMallCellTip::init(string msg,string title,float dy,CCTextAlignment align) {
    bool ret = false;
    if (PopupBaseView::init()) {
        _modelLayer->setOpacity(0);
        CCBLoadFile("StoreMallCellTip",this,this);
        m_titleName->setString(title);
                        
        
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            this->onClickCloseBtn(NULL, Control::EventType::TOUCH_DOWN);
        });
        vector<std::string> vector_item;
        vector<std::string> vector_number;
        CCCommonUtils::splitString(msg, "|", vector_item);
        int size =vector_item.size();
        int i=0;
        while (i<size) {
            vector_number.clear();
            string item =  vector_item[i];
            CCCommonUtils::splitString(item, ";", vector_number);
            int itemId =atoi(vector_number[0].c_str());
            int num = atoi(vector_number[1].c_str());
            StoreMallCellTipReward* cell = StoreMallCellTipReward::create(R_GOODS, itemId,num);
            m_rewardList->addChild(cell);
            int k= i%4;
            int j = i/4;
            cell->setPosition(k*113.5, -j*155);
            i++;
        }
        
        
        
        ret = true;
    }
    return ret;
}

void StoreMallCellTip::onEnter() {
    PopupBaseView::onEnter();
}

void StoreMallCellTip::onExit() {
    PopupBaseView::onExit();
}

SEL_CCControlHandler StoreMallCellTip::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", TipsView::onClickCloseBtn);
    return NULL;
}

bool StoreMallCellTip::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardList", CCNode*, m_rewardList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBG", CCScale9Sprite*, m_rewardBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleName", CCLabelIF*, m_titleName);
    return false;
}

void StoreMallCellTip::onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    PopupViewController::getInstance()->removePopupView(this);
}

StoreMallCellTipReward *StoreMallCellTipReward::create(int type,int value,int num,int color,bool gray){
    StoreMallCellTipReward *ret = new StoreMallCellTipReward(type,value,num,color,gray);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
CCNode* StoreMallCellTipReward::getPicNode(){
    return m_picNode;
}
void StoreMallCellTipReward::setData(int type,int value,int num,int color,bool gay){
    
    std::string nameStr;
    CCSprite icon;
    if(type == R_GOODS || type == R_GENERAL){
        nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
    }else{
        nameStr = CCCommonUtils::getResourceNameByType(type);
        
    }
    if(type == R_GOODS || type == R_GENERAL){
//        bool dPath = (type == R_GOODS)? true:false;
        auto icon  =(type == R_GOODS)? CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(value)).c_str(),true,CCLoadSpriteType_GOODS) : CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(value)).c_str());
        m_picNode->addChild(icon);
        icon->setAnchorPoint(ccp(0.5, 0.5));
        if(gay){
            icon->setColor({90,85,81});
        }
    }else{
        auto icon1 = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(type).c_str());
        m_picNode->addChild(icon1);
        icon1->setAnchorPoint(ccp(0.5, 0.5));
        if(gay){
            icon1->setColor({90,85,81});
        }
    }
    
    std::string str = CC_ITOA(num);
    
    this->m_nameText->setString(nameStr);
    this->m_numText->setString(str);
    if(gay){
        m_numText->setColor({90,85,81});
        m_nameText->setColor({90,85,81});
    }
    
}

void StoreMallCellTipReward::onEnter(){
    CCNode::onEnter();
}

void StoreMallCellTipReward::onExit(){
    CCNode::onExit();
}

bool StoreMallCellTipReward::init(){
    auto bg = CCBLoadFile("StoreMallCellRewardTip", this, this);
    this->setContentSize(bg->getContentSize());
    this->setData(m_type,m_value,m_num,m_color,m_gay);
    return true;
}

bool StoreMallCellTipReward::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numText", CCLabelIF*, this->m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picNode", CCNode*, this->m_picNode);
    
    return false;
}

SEL_CCControlHandler StoreMallCellTipReward::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}