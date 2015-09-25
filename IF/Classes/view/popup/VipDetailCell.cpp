//
//  VipDetailCell.cpp
//  IF
//
//  Created by chenliang on 14-3-18.
//
//

#include "VipDetailCell.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "UseToolView.h"
#include "VipConstants.h"
#include "VipUtil.h"


const float VipDetailCell::CELL_WIDTH = 320;
const float VipDetailCell::CELL_HEIGHT = 428;

static const float INFO_AREA_HEIGHT = 287;

static const int ATTRIBUTE_COUNT = 14;

static const int CONTAINER_TAG = 999;

VipDetailCell* VipDetailCell::create(int index)
{
    auto ret = new VipDetailCell(index);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool VipDetailCell::init()
{
    bool ret = true;
   // CCBLoadFile("VipDetailCell",this,this);
    
    m_listNode = CCNode::create();
    this->addChild(m_listNode);
//    m_addVipPointsBtnLabel->setString(_lang("103003").c_str());
//    m_activateVipBtnLabel->setString(_lang("103004").c_str());
//    
//    m_infoScrollView = CCScrollView::create();
//    m_infoScrollView->setDirection(kCCScrollViewDirectionVertical);
//    m_infoScrollView->setPosition(CCPoint(0, 95));
//    m_infoScrollView->setAnchorPoint(CCPoint(0,0));
//    m_infoScrollView->setViewSize(CCSize(CELL_WIDTH, 287));
//    addChild(m_infoScrollView);
    setData(m_index);
    return ret;
}

void VipDetailCell::onEnter() {
    CCNode::onEnter();
}

void VipDetailCell::onExit() {
        CCNode::onExit();
}

SEL_CCControlHandler VipDetailCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    
    return NULL;
}

float VipDetailCell::getHG(){

    return this->getContentSize().height;
}

void VipDetailCell::setData(int index)
{

    int itemID = 7000+index;
    int nextItemID = 7000+index+1;
    //string a[9]={"speedup","wood","food","iron","silver","march","attack","defense","health"};
    //string a[6]={"speedup","wood","march","attack","defense","health"};
    string a[10]={"hurt","attack","defense","health","troop","march","monster","body","speedup","wood"};
    int length = 10;
    int j = 0;
    for (int i =0; i<length; i++) {
        string key = a[i];
        bool isCreateCell = true;
        int value = VipUtil::getVipItemValueByKey(CC_ITOA(itemID), key);
        int nextvalue = VipUtil::getVipItemValueByKey(CC_ITOA(nextItemID), key);
        if(nextvalue<=0){
            isCreateCell = false;
        }
        if(isCreateCell){
            VipEffectCell* cell = VipEffectCell::create(key,value,key,nextvalue);
            m_listNode->addChild(cell);
            float height = cell->getHG();
            cell->setPositionY(-height*j);
            j++;
        }
    }
    m_listNode->setPositionY(135*j);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_listNode->setPositionY(276 * j);
    }
    this->setContentSize(CCSize(614, 135*j));
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCSize(1470, 276 * j));
    }


}


bool VipDetailCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftText", CCLabelIF*, m_leftText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightText", CCLabelIF*, m_rightText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftPicNode", CCNode*, m_leftPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightPicNode", CCNode*, m_rightPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftPicNode", CCNode*, m_leftPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightPicNode", CCNode*, m_rightPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    
    return false;
}


VipEffectCell* VipEffectCell::create(string type1,int value1,string type2,int value2)
{
    auto ret = new VipEffectCell(type1,value1,type2,value2);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool VipEffectCell::init()
{
    bool ret = true;
    auto bg = CCBLoadFile("VipEffectCell",this,this);
    this->setContentSize(bg->getContentSize());
    setData(m_type1,m_value1,m_type2,m_value2);
    return ret;
}

void VipEffectCell::onEnter() {
    CCNode::onEnter();
}

void VipEffectCell::onExit() {
        CCNode::onExit();
}

SEL_CCControlHandler VipEffectCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    
    return NULL;
}

float VipEffectCell::getHG(){
    
    return this->getContentSize().height;
}

void VipEffectCell::setData(string type1,int value1,string type2,int value2)
{
    string pic = "";
    if (type1=="speedup") {
//        this->m_showNode->setVisible(true);
//        this->m_hideNode->setVisible(false);
        m_leftText->setString(_lang_1("103034",""));
        m_rightText->setString(_lang_1("103034",""));
        pic = "skill2_650403.png";
    }else if (type1=="wood") {
        m_leftText->setString(_lang_1("103035",""));
        m_rightText->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (type1=="food") {
        m_leftText->setString(_lang_1("103035",""));
        m_rightText->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (type1=="iron") {
        m_leftText->setString(_lang_1("103035",""));
        m_rightText->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (type1=="silver") {
        m_leftText->setString(_lang_1("103035",""));
        m_rightText->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (type1=="attack") {
        m_leftText->setString(_lang_1("103037",""));
        m_rightText->setString(_lang_1("103037",""));
        pic = "skill2_606201.png";
    }else if (type1=="defense") {
        m_leftText->setString(_lang_1("103038",""));
        m_rightText->setString(_lang_1("103038",""));
        pic = "skill2_606202.png";
    }else if (type1=="march") {
        m_leftText->setString(_lang_1("103036",""));
        m_rightText->setString(_lang_1("103036",""));
        pic = "skill2_650000.png";
    }else if (type1=="health") {
        m_leftText->setString(_lang_1("103039",""));
        m_rightText->setString(_lang_1("103039",""));
        pic = "skill2_652205.png";
    }else if (type1=="hurt") {
        m_leftText->setString(_lang_1("103040",""));
        m_rightText->setString(_lang_1("103040",""));
        pic = "skill2_650205.png";
    }else if (type1=="troop") {
        m_leftText->setString(_lang_1("103041",""));
        m_rightText->setString(_lang_1("103041",""));
        pic = "skill2_603103.png";
    }else if (type1=="monster") {
        m_leftText->setString(_lang_1("103042",""));
        m_rightText->setString(_lang_1("103042",""));
        pic = "skill2_650400.png";
    }else if (type1=="body") {
        m_leftText->setString(_lang_1("103043",""));
        m_rightText->setString(_lang_1("103043",""));
        pic = "skill2_650405.png";
    }
    if (value1 == value2)
    {
        m_rightAddFlag->setVisible(false);
    }
    CCSprite* head = CCLoadSprite::createSprite(pic.c_str());
    CCSprite* head1 = CCLoadSprite::createSprite(pic.c_str());
    m_leftPicNode->addChild(head);
    m_rightPicNode->addChild(head1);
    float scale = 94/head->getContentSize().width;
    scale = scale>1.0?1.0:scale;
    head->setScale(scale);
    head1->setScale(scale);
    if(value1==0){
        head->setColor({90,85,81});
        m_leftText->setColor({90,85,81});
        m_leftValueText->setColor({173,44,35});
    }
    string value1str = "";

    string value2str = "";
    if (type1=="troop") {
        value1str.append("+");
        value1str.append(CC_ITOA(value1));
        value2str.append("+");
        value2str.append(CC_ITOA(value2));
        m_leftValueText->setString(value1str);
        m_rightValueText->setString(value2str);
        
    }else if (type1=="speedup") {
        
        value1str.append(CC_ITOA(value1));
        value1str.append(_lang("103033"));
        value2str.append(CC_ITOA(value2));
        value2str.append(_lang("103033"));
        m_leftValueText->setString(value1str);
        m_rightValueText->setString(value2str);
        
    }else{
        value1str.append("+");
        value1str.append(CC_ITOA(value1));
        value1str.append("%");
        
        value2str.append("+");
        value2str.append(CC_ITOA(value2));
        value2str.append("%");
       
        m_leftValueText->setString(value1str);
        m_rightValueText->setString(value2str);
    }
}


bool VipEffectCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightValueText", CCLabelIF*, m_rightValueText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftValueText", CCLabelIF*, m_leftValueText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftText", CCLabelIF*, m_leftText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightText", CCLabelIF*, m_rightText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftPicNode", CCNode*, m_leftPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightPicNode", CCNode*, m_rightPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showNode1", CCNode*, m_showNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showNode2", CCNode*, m_showNode2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightAddFlag", CCSprite*, m_rightAddFlag);
    return false;
}