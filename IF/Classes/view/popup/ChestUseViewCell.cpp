//
//  ChestUseViewCell.cpp
//  LastEmpire
//
//  Created by zhangjun on 2015-03-24 18:37:40.
//
//

#include "ChestUseViewCell.h"
#include "ToolController.h"

ChestUseViewCell::ChestUseViewCell()
{
    CCLOGFUNC();
}

ChestUseViewCell::~ChestUseViewCell()
{
    CCLOGFUNC();
}
ChestUseViewCell* ChestUseViewCell::create()
{
    ChestUseViewCell *pRet = new ChestUseViewCell();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}
bool ChestUseViewCell::init()
{
    bool bRet = false;
    if (CCNode::init())
    {
        /*Load CCB*/
        CCBLoadFile("ChestUseViewCell.ccbi", this, this);
        bRet = true;
    }
    return bRet;
}
void ChestUseViewCell::onEnter()
{
    CCNode::onEnter();
}
void ChestUseViewCell::onExit()
{
    CCNode::onExit();
}
		
SEL_CCControlHandler ChestUseViewCell::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_MenuHandler ChestUseViewCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CallFuncN ChestUseViewCell::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool ChestUseViewCell::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spr9Bg", CCScale9Sprite*,this->m_spr9Bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprChestBg", CCSprite*,this->m_sprChestBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeChest", CCNode*,this->m_nodeChest);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_labCount", Label*,this->m_labCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_labDes", Label*,this->m_labDes);
    return false;
}

void ChestUseViewCell::setData(int itemId,string nameStr,int countStr,int itemType){
    m_labDes->setString(nameStr.c_str());
    string str=" +"+CC_CMDITOA(countStr);
    m_labCount->setString(str.c_str());
    if (itemType==R_GOODS) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
        if (pic) {
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 40,true);
            m_nodeChest->addChild(iconBg);
            CCCommonUtils::setSpriteMaxSize(pic, 40,true);
            m_nodeChest->addChild(pic);
        }
    }else if (itemType==14){
        std::string picStr = CCCommonUtils::getPropById(CC_ITOA(itemId), "icon");
        picStr+=".png";
        auto pic = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
        if (pic) {
            std::string colorStr = CCCommonUtils::getPropById(CC_ITOA(itemId), "color");
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(atoi(colorStr.c_str())).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, 40,true);
            m_nodeChest->addChild(iconBg);
            CCCommonUtils::setSpriteMaxSize(pic, 40,true);
            m_nodeChest->addChild(pic);
        }
        string name=CCCommonUtils::getPropById(CC_ITOA(itemId), "name");
        m_labDes->setString(_lang(name).c_str());
//        string str=" +"+CC_CMDITOA(countStr);
//        m_labCount->setString(str.c_str());
    }else{
        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(itemType).c_str(),true,CCLoadSpriteType_DEFAULT);
        if (pic) {
            CCCommonUtils::setSpriteMaxSize(pic, 40,true);
            m_nodeChest->addChild(pic);
        }
    }
    
    
}

