//
//  CheckKingdomView.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-9.
//
//

#include "CheckKingdomView.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ToolController.h"
#include "UseSkillCommand.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "GeneralManager.h"
#include "FunBuildController.h"
#include "KingsGiftView.h"
#include "SoundController.h"

CheckKingdomView *CheckKingdomView::create(int serverId){
    CheckKingdomView *ret = new CheckKingdomView(serverId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CheckKingdomView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("110021"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void CheckKingdomView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool CheckKingdomView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPos = pTouch->getLocation();
    return true;
}

void CheckKingdomView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(GlobalData::shared()->playerInfo.isInSelfServer() && isTouchInside(m_giftArea, pTouch)){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupInView(KingsGiftView::create());
    }else if(isTouchInside(m_kingNode, pTouch)){
        OfficeInfo* info = NULL;
        for (int i=0; i<m_data->count(); i++) {
            OfficeInfo* temp = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i));
            string oid = CC_ITOA(temp->id);
            if(oid==KINGDOM_KING_ID){
                info = temp;
                break;
            }
        }
        if(info){
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            PopupViewController::getInstance()->addPopupView(KingdomRemoveAppointView::create(info,false));
        }
    }
}

bool CheckKingdomView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto tbg = CCLoadSprite::loadResource("technology_09.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
        int curHight = -500;
        if (CCCommonUtils::isIosAndroidPad()) {
            curHight = -1200;
        }
        while (curHight<maxHight) {
            auto bg = CCLoadSprite::createSprite("technology_09.png");
            bg->setAnchorPoint(ccp(0, 1));
            bg->setPosition(ccp(0, curHight));
            if (CCCommonUtils::isIosAndroidPad()) {
                bg->setScaleX(2.4);
            }
            curHight += bg->getContentSize().height;
            tBatchNode->addChild(bg);
        }
        this->addChild(tBatchNode);
        
        CCLoadSprite::doResourceByCommonIndex(307, true);
        CCLoadSprite::doResourceByCommonIndex(310, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(307, false);
            CCLoadSprite::doResourceByCommonIndex(310, false);
        });
        auto tmpCCB = CCBLoadFile("CheckKingdomView", this, this);
        if(!GlobalData::shared()->playerInfo.isInSelfServer()){
            m_giftNode->setVisible(false);
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else {
            this->setContentSize(tmpCCB->getContentSize());
            int prevH = m_viewBg->getContentSize().height;
            changeBGHeight(m_viewBg);
            int add = m_viewBg->getContentSize().height - prevH;
            m_scNode->setContentSize(CCSize(m_scNode->getContentSize().width,m_scNode->getContentSize().height+add));
            m_scNode->setPositionY(m_scNode->getPositionY()-add);
        }
        
        m_viewBg->setVisible(false);
        m_data = CCArray::create();
        m_officalsData = CCArray::create();
        m_servantData = CCArray::create();

        m_scrollView = CCScrollView::create(m_scNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(Touch_Popup_Item);
        m_scNode->addChild(m_scrollView);
        
        if (CCCommonUtils::isIosAndroidPad()) {
            float h1 = m_officalsNode->getPositionY();
            m_officalsNode->removeFromParent();
            m_officalsNode->setPositionY(m_officalsNode->getPositionY() + 900);
            m_scrollView->addChild(m_officalsNode);
            
            float h2 = m_servantNode->getPositionY();
            m_servantNode->removeFromParent();
            m_servantNode->setPositionY(m_servantNode->getPositionY() + 900);
            m_scrollView->addChild(m_servantNode);
            m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,2100));
            m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - 2100));
        }
        else {
            m_officalsNode->removeFromParent();
            m_officalsNode->setPositionY(m_officalsNode->getPositionY() + 690);
            m_scrollView->addChild(m_officalsNode);
            
            m_servantNode->removeFromParent();
            m_servantNode->setPositionY(m_servantNode->getPositionY() + 690);
            m_scrollView->addChild(m_servantNode);
            m_scrollView->setContentSize(CCSize(m_scNode->getContentSize().width,1050));
            m_scrollView->setContentOffset(ccp(0, m_scNode->getContentSize().height - 1050));
        }
        
        
        m_officialsTxt1->setString(_lang("110026"));
        m_officialsTxt2->setString(_lang("110027"));
        
        CCDictElement *element;
        auto dictSK = LocalController::shared()->DBXMLManager()->getGroupByKey("office");
        CCDICT_FOREACH(dictSK, element){
            std::string id = element->getStrKey();
            OfficeInfo* info = OfficeInfo::create();
            auto tempDic = _dict(element->getObject());
            info->parse(tempDic);
            m_data->addObject(info);
        }
        for (int i=0;i<8; i++) {
            OfficeInfo* info = NULL;
            if(m_data->count()>9){
                info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i+1));
            }
            KingdomOfficalsCell* ocell = KingdomOfficalsCell::create(info,m_scNode,false);
            if(i>3){
                if (CCCommonUtils::isIosAndroidPad()) {
                    ocell->setPosition(ccp((i-4)*350, -500));
                }
                else
                    ocell->setPosition(ccp((i-4)*154, -390));
            }else{
                if (CCCommonUtils::isIosAndroidPad()) {
                    ocell->setPosition(ccp(i*350, 0));
                }
                else
                    ocell->setPosition(ccp(i*154, -150));
            }
            m_oNode->addChild(ocell);
        }
        
        for (int i=0;i<8; i++) {
            OfficeInfo* info = NULL;
            if(m_data->count()==17){
                info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(i+9));
            }
            KingdomServantCell* ocell = KingdomServantCell::create(info,m_scNode,false);
            if(i>3){
                if (CCCommonUtils::isIosAndroidPad()) {
                    ocell->setPosition(ccp((i-4)*350, -420));
                }
                else
                    ocell->setPosition(ccp((i-4)*154, -295));
            }else{
                if (CCCommonUtils::isIosAndroidPad()) {
                    ocell->setPosition(ccp(i*350, 0));
                }
                else
                    ocell->setPosition(ccp(i*154, -95));
            }
            m_sNode->addChild(ocell);
        }
    
        KingdomOfficersCommand* cmd = new KingdomOfficersCommand();
        cmd->putParam("serverId", CCInteger::create(GlobalData::shared()->playerInfo.currentServerId));
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CheckKingdomView::getOfficersData), NULL));
        cmd->sendAndRelease();
        ret = true;
    }
    return ret;
}

void CheckKingdomView::getOfficersData(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* officers = dynamic_cast<CCArray*>(dic->objectForKey("officers"));
        string kingdomName = dic->valueForKey("kingdomName")->getCString();
        if(kingdomName==""){
            kingdomName = string("#") + CC_ITOA(GlobalData::shared()->playerInfo.currentServerId);
        }
        //kingdomName = (kingdomName + "#" + CC_ITOA(GlobalData::shared()->playerInfo.currentServerId));
        m_kingTxt->setString(_lang_1("110023",kingdomName.c_str()));
        int num = officers->count();
        for (int i=0; i<num; i++) {
            auto officerInfo = _dict(officers->objectAtIndex(i));
            int posId = officerInfo->valueForKey("positionId")->intValue();
            int oNum = m_data->count();
            for (int j=0; j<oNum; j++) {
                OfficeInfo* info = dynamic_cast<OfficeInfo*>(m_data->objectAtIndex(j));
                if (posId==info->id) {
                    info->playerName = officerInfo->valueForKey("name")->getCString();
                    info->uid = officerInfo->valueForKey("uid")->getCString();
                    info->playerIcon = officerInfo->valueForKey("pic")->getCString();
                    info->chNameFlag = officerInfo->valueForKey("chNameFlag")->boolValue();
                    info->time = officerInfo->valueForKey("time")->doubleValue();
                    info->allianceName = officerInfo->valueForKey("alliancename")->getCString();
                    info->picVer = officerInfo->valueForKey("picVer")->intValue();
                    string temp = "";
                    if(info->id==216000){
                        if(info->uid==""){
                            temp = _lang("110077");
                            m_playerTxt->setString(_lang_1("110025",temp.c_str()));
                        }else{
                            //temp = _lang_1("110025",info->playerName.c_str());
                            temp = info->playerName;
                            if (temp.length()>19) {
                                temp = CCCommonUtils::subStrByUtf8(temp,0,19);
                                temp.append("...");
                            }
                            m_playerTxt->setString(temp.c_str());
                            m_kingIcon = info->playerIcon;
                            asyDelayLoad(NULL);
                        }
                        if(info->allianceName==""){
                            m_allianceTxt->setString("");
                        }else{
                            //m_allianceTxt->setString(_lang_1("110024",info->allianceName.c_str()));
                            m_allianceTxt->setString(info->allianceName.c_str());
                        }
                    }
                }
            }
        }
        this->refreshView();
    }
}

void CheckKingdomView::refreshView(){
    auto& arr = m_oNode->getChildren();
    for(auto child :arr)
    {
        KingdomOfficalsCell* officals = dynamic_cast< KingdomOfficalsCell*>(child);
        if (officals!=NULL) {
            officals->refreshView();
        }
    }
    
    auto& arr2 = m_sNode->getChildren();
    for(auto child2 :arr2)
    {
        KingdomServantCell* servant = dynamic_cast< KingdomServantCell*>(child2);
        if (servant!=NULL) {
            servant->refreshView();
        }
    }
//    int num = arr->count();
//    for (int i=0; i<num; i++) {
//        KingdomOfficalsCell* officals = dynamic_cast< KingdomOfficalsCell*>(arr->objectAtIndex(i));
//        if (officals!=NULL) {
//            officals->refreshView();
//        }
//    }
//    arr = m_sNode->getChildren();
//    num = arr->count();
//    for (int i=0; i<num; i++) {
//        KingdomServantCell* servant = dynamic_cast< KingdomServantCell*>(arr->objectAtIndex(i));
//        if (servant!=NULL) {
//            servant->refreshView();
//        }
//    }
}

void CheckKingdomView::asyDelayLoad(CCObject* p){
    m_iconNode->removeAllChildrenWithCleanup(true);
    auto pic = CCLoadSprite::createSprite(CCString::createWithFormat("%s_middle.png",m_kingIcon.c_str())->getCString());
    m_iconNode->addChild(pic);
}

bool CheckKingdomView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_officalsNode", CCNode*, this->m_officalsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_servantNode", CCNode*, this->m_servantNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingTxt", CCLabelIFTTF*, this->m_kingTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceTxt", CCLabelIFTTF*, this->m_allianceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerTxt", CCLabelIFTTF*, this->m_playerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_officialsTxt1", CCLabelIF*, this->m_officialsTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_officialsTxt2", CCLabelIF*, this->m_officialsTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scNode", CCNode*, this->m_scNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingNode", CCNode*, this->m_kingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_oNode", CCNode*, this->m_oNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sNode", CCNode*, this->m_sNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_giftNode", CCNode*, this->m_giftNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_giftArea", CCNode*, this->m_giftArea);

    return false;
}

SEL_CCControlHandler CheckKingdomView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUseClick", CheckKingdomView::onUseClick);
    return NULL;
}
