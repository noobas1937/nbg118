//
//  ActivityRankReward.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-3.
//
//

#include "ActivityRankReward.h"
#include "RewardController.h"

ActivityRankReward *ActivityRankReward::create(CCArray* datas,std::string title,bool flag,int type, string cell_ccb){
    ActivityRankReward *ret = new ActivityRankReward(datas,title,flag,type,cell_ccb);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityRankReward::init(){
    bool ret = false;
    if(CCNode::init()){
        m_lable = CCLabelIF::create(m_title.c_str(), 22);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_lable->setFontSize(44);
            m_lable->setScale(0.5);
        }
        m_lable->setAnchorPoint(ccp(0, 0.5));
        m_lable->setColor(ccRED);
        this->addChild(m_lable,100000);
        
        m_container = CCNode::create();
        this->addChild(m_container);

        setData(m_datas,m_title,m_flag,m_type);
        ret = true;
    }
    return ret;
}

void ActivityRankReward::setData(CCArray* datas,std::string title,bool flag,int type){
    m_datas = datas;
    m_title = title;
    m_flag = flag;
    m_type = type;
    int num = m_datas->count();
    m_container->removeAllChildrenWithCleanup(true);
    m_lable->setString(m_title);
    if(flag){
        m_lable->setPositionY(m_datas->count()*85);
    }
//    for(int i=0;i<num;i++){
//        auto dic = _dict(m_datas->objectAtIndex(i));
//        ActivityRankRewardCell* cell = ActivityRankRewardCell::create(dic);
//        if(flag){
//            cell->setPosition(ccp(0, i*85));
//        }else{
//            cell->setPosition(ccp(0, -i*85 - 65-20));
//        }
//        m_container->addChild(cell);
//    }
    if (m_type==0 || m_type==1 || m_type==2||m_type==3 ||m_type==4 || m_type==5) {
        for(int i=0;i<num;i++){
            auto dic = _dict(m_datas->objectAtIndex(i));
            ActivityRankRewardCell* cell = ActivityRankRewardCell::create(dic,m_cell_ccb);
            cell->setPosition(ccp(0, -i*85 + m_datas->count()*85 - 65 -20));
            m_container->addChild(cell);
        }
    }else{
        for(int i=0;i<num;i++){
            auto dic = _dict(m_datas->objectAtIndex(i));
            ActivityRankRewardCell* cell = ActivityRankRewardCell::create(dic,m_cell_ccb);
            if(flag){
                cell->setPosition(ccp(0, i*85));
            }else{
                cell->setPosition(ccp(0, -i*85 - 65-20));
            }
            m_container->addChild(cell);
        }
    }
}

int ActivityRankReward::getTotalH(){
    return m_datas->count()*85+20;
}

ActivityRankRewardCell *ActivityRankRewardCell::create(CCDictionary* info, string cell_ccb){
    ActivityRankRewardCell *ret = new ActivityRankRewardCell(info, cell_ccb);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ActivityRankRewardCell::onEnter(){
    CCNode::onEnter();
}

void ActivityRankRewardCell::onExit(){
    CCNode::onExit();
}

bool ActivityRankRewardCell::init(){
    auto bg = CCBLoadFile(m_cell_ccb.c_str(), this, this);
    this->setContentSize(bg->getContentSize());
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_nameTxt->setScale(0.5);
        m_nameTxt->setFontSize(m_nameTxt->getFontSize() * 2);
        m_nameTxt->setDimensions(CCSize(540, 120));
        m_numTxt->setScale(0.5);
        m_numTxt->setFontSize(m_numTxt->getFontSize() * 2);
    }
    
    std::string name = "";
    std::string icon = "";
    int type = m_info->valueForKey("type")->intValue();
    int value = m_info->valueForKey("value")->intValue();
    if(type==R_GOODS){
        auto valueObj = _dict(m_info->objectForKey("value"));
        int id = valueObj->valueForKey("id")->intValue();
        name = RewardController::getInstance()->getNameByType(type,id);
        icon = RewardController::getInstance()->getPicByType(type,id);
        value = valueObj->valueForKey("num")->intValue();
    }else if(type==R_EFFECT){
        name = CCCommonUtils::getNameById(CC_ITOA(value));
        if (name=="") {
            switch (value) {
                case 502600:
                    name = _lang("138074");
                    break;
                case 502601:
                    name = _lang("138075");
                    break;
                case 502602:
                    name = _lang("138076");
                    break;
            }
        }
        icon = CCCommonUtils::getIcon(CC_ITOA(value));
        value = 1;
    }else{
        name = RewardController::getInstance()->getNameByType(type,value);
        icon = RewardController::getInstance()->getPicByType(type,value);
    }
    m_nameTxt->setString(name);
    name = "X";
    name.append(CC_CMDITOA(value));
    if (type==R_EFFECT) {
        name = "";
    }
    m_numTxt->setString(name);
    
    CCSprite* spr = CCLoadSprite::createSprite(icon.c_str());
    CCCommonUtils::setSpriteMaxSize(spr, 45);
    m_iconNode->addChild(spr);
    return true;
}


bool ActivityRankRewardCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    return false;
}

SEL_CCControlHandler ActivityRankRewardCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}