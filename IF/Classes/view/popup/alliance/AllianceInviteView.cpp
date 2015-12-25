//
//  AllianceInviteView.cpp
//  IF
//
//  Created by ganxiaohua on 14-12-31.
//
//

#include "AllianceInviteView.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "../../../Ext/CCDevice.h"
#include "GetAllianceMembersCommand.h"
#include "AllianceManagerFunView.h"
#include "SearchUserAllianceCommand.h"
#include "ChatController.h"
#include "GameController.h"
#include "UIComponent.h"
#include "InvitesAllianceCommand.h"
#include "WorldSearchUserCommand.h"
#include "KingdomExecCommand.h"
#include "ManagerKingdomView.h"
#include "PopupViewController.h"
#include "KingsGiftCommand.h"
#include "SoundController.h"
#include "LuaController.h"

AllianceInviteView *AllianceInviteView::create(int officeId, int panelType, std::string giftId, std::string giftDialogId){
    AllianceInviteView *ret = new AllianceInviteView(officeId,panelType,giftId, giftDialogId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceInviteView::onEnter(){
    PopupBaseView::onEnter();
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    if(m_panelType == KINGSGIFT){
        std::string titleStr = _lang_1("110130", _lang(m_dialogId).c_str());
        this->setTitleName(titleStr);
    }else{
        this->setTitleName(_lang("115178"));
    }
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInviteView::removeOneData), MSG_REMOVE_ONE_INVITE_DATA, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInviteView::refresh), MSG_REFRESH_INVITE_DATA, NULL);
}

void AllianceInviteView::onExit(){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REMOVE_ONE_INVITE_DATA);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFRESH_INVITE_DATA);
    PopupBaseView::onExit();
    if(m_backTo>0){
        LuaController::getInstance()->backToExchange(m_backTo);
    }
}

bool AllianceInviteView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_moreTxt->setString("");
    m_startPos = pTouch->getLocation();
    m_tabPos = m_tabView->getContentOffset();
    if(isTouchInside(m_infoList, pTouch)){
        return true;
    }
    return false;
}

void AllianceInviteView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_tabView->getContentOffset().y > 40){
        m_tabView->setBounceable(false);
    }
    float offsetY = m_tabView->getContentOffset().y;
//    if(m_total > m_data->count() && offsetY>0){
//        if(offsetY>40)
//            m_moreTxt->setString(_lang("105318").c_str());
//        else
//            m_moreTxt->setString(_lang("115189").c_str());
//    }
}

void AllianceInviteView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    if(m_tabView->getContentOffset().y >=0 && fabs(pos.y - m_startPos.y) >40){
        m_tabView->setBounceable(false);
        m_tabView->setTouchEnabled(false);
        m_tabView->stopAllActions();
        m_tabView->unscheduleAllSelectors();
        m_tabView->getContainer()->unscheduleAllSelectors();
        m_tabView->getContainer()->stopAllActions();
        m_prevNum = m_data->count();
        m_currMinOffsetY = m_tabView->minContainerOffset().y;
        m_currOffsetY = m_tabView->getContentOffset().y;
        this->callMore();
    }
    m_moreTxt->setString("");
}

void AllianceInviteView::refresh(CCObject* param){
    m_tabView->setTouchEnabled(true);
    CCPoint pos = m_tabView->getContentOffset();
    float dy = m_tabView->getContentOffset().y;
    m_tabView->reloadData();
    if(!m_first){
        m_tabView->reloadData();
        m_newMinOffsetY = m_tabView->minContainerOffset().y;
        if(dy>m_tabView->maxContainerOffset().y){
            dy = m_tabView->maxContainerOffset().y;
        }
        if(dy<m_newMinOffsetY){
            dy = m_newMinOffsetY;
        }
        if(m_currMinOffsetY<0&&m_newMinOffsetY<0){
            float dy1 = m_newMinOffsetY-m_currMinOffsetY+m_currOffsetY;
            if(dy1>0){
                dy1 = 0;
            }
            m_tabView->setContentOffset(ccp(0,dy1));
        }
        
    }
    if(m_data->count()<=0){
        if(m_officeId!=0){
            //m_tipTxt->setString(_lang("115204"));
        }else{
            if(m_searchKey!=""){
                m_tipTxt->setString(_lang("115204"));
            }else{
                m_tipTxt->setString(_lang("115273"));
            }
        }

    }else{
        m_tipTxt->setString("");
    }
    m_first = false;
}

void AllianceInviteView::callMore(){
    m_tabView->setBounceable(true);
    if(m_officeId!=0 || m_panelType == KINGSGIFT){
        int nextPage = ceil(m_data->count()/8.0) + 1;
        WorldSearchUserCommand* cmd = new WorldSearchUserCommand(m_searchKey,nextPage);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInviteView::updateData), NULL));
        cmd->sendAndRelease();
    }else{
        if(m_data->count()>=m_total && m_total!=0) return ;
        int nextPage = ceil(m_data->count()/8.0) + 1;
        SearchUserAllianceCommand* cmd = new SearchUserAllianceCommand(m_searchKey,m_lang,nextPage);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInviteView::updateData), NULL));
        cmd->sendAndRelease();
    }
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_waitInterface = GameController::getInstance()->showWaitInterface(m_waitNode);
}

bool AllianceInviteView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(307, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        setCleanFunction([this](){
            if (false == m_bReleaseTextureAfterRemove) return;
            
            CCLoadSprite::doResourceByCommonIndex(307, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
        });
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//        int curHight = -500;
//        while (curHight<maxHight) {
//            auto bg = CCLoadSprite::createSprite("technology_09.png");
//            bg->setAnchorPoint(ccp(0, 1));
//            bg->setPosition(ccp(0, curHight));
//            curHight += bg->getContentSize().height;
//            tBatchNode->addChild(bg);
//        }
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            tBatchNode->setScaleX(1536.0 / 640.0);
//        }
//        this->addChild(tBatchNode);
        auto tmpCCB = CCBLoadFile("AllianceInviteView", this, this);
        this->setContentSize(tmpCCB->getContentSize());
        int prevH = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prevH;
        if (CCCommonUtils::isIosAndroidPad())
        {
            add = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height+add));
        m_infoList->setPositionY(m_infoList->getPositionY()-add);
//        m_moreTxt->setPositionY(m_moreTxt->getPositionY()-add);
        m_bottom_node->setPositionY(-add);
        m_viewBg->setVisible(false);
        
        m_data = CCArray::create();
        
        m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_tabView->setMultiColTableViewDelegate(this);
        m_infoList->addChild(m_tabView);
        
        auto spriteText = CCLoadSprite::createScale9Sprite("nb_alliance_search_bg.png");
        if (!CCCommonUtils::isIosAndroidPad()) {
            m_inputName = CCEditBox::create(CCSizeMake(420,44),spriteText);
        } else {
            auto s = m_searchNode->getContentSize();
            m_inputName = CCEditBox::create(s, spriteText);
        }
        m_inputName->setMaxLength(20);
        m_inputName->setFontSize(24);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_inputName->setFontSize(52);
            m_inputName->setAnchorPoint(CCPointZero);
        }
        m_inputName->setFontColor({196, 207, 255});
        m_inputName->setReturnType(kKeyboardReturnTypeDone);
        m_inputName->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
        m_inputName->setText("");
        m_inputName->setPlaceHolder("");
        m_searchNode->addChild(m_inputName);

        CCCommonUtils::setButtonTitle(onInviteBtn, _lang("115270").c_str());
        CCCommonUtils::setButtonTitle(m_searchBtn, _lang("115269").c_str());
        m_goldTxt->setString(CC_ITOA(GlobalData::shared()->invite_cost_k1));
        m_titleTxt->setString(_lang("115178"));
        
        m_allianeId = "";
        string aLang = "115601";
        if(m_panelType==OFFICIAL || INVITE || KINGSGIFT || ITEM_GIFT || ITEM_DONATE){
            if(GlobalData::shared()->playerInfo.isInAlliance()){
                m_allianeId = GlobalData::shared()->playerInfo.allianceInfo.uid;
                aLang = GlobalData::shared()->playerInfo.allianceInfo.language;
            }
        }
        CCDictionary* dic_all = LocalController::shared()->shared()->DBXMLManager()->getGroupByKey("language");
        int num = dic_all->count();
        m_lang = "en";
        int idx = 9500;
        for (int i=0; i<num; ++i) {
            CCDictionary* dic_one = _dict(dic_all->objectForKey(CC_ITOA(idx+i)));
            string mark = dic_one->valueForKey("mark")->getCString();
            string lang_ma = dic_one->valueForKey("lang_ma")->getCString();
            string lang_id = dic_one->valueForKey("lang_id")->getCString();
            if(mark!="" && lang_id!="" && lang_id==aLang){
                m_lang = lang_ma;
                break;
            }
        }
        m_currentPage = 1;
        m_total = 0;
        m_uids = "";
        m_searchKey = "";
        m_first = true;
        m_waitInterface = NULL;
        if(m_officeId!=0){
            m_inviteNode->setVisible(false);
            m_titleTxt->setPositionX(320);
            string strTitle = _lang("110064");
            strTitle.append(":");
            strTitle.append(CCCommonUtils::getNameById(CC_ITOA(m_officeId)));
            m_titleTxt->setString(strTitle);
        }
        if(m_officeId!=0 || m_panelType == KINGSGIFT){
            if(m_allianeId!=""){
                GetAllianceMembersCommand * command = new GetAllianceMembersCommand();
                command->putParam("allianceId", CCString::create(m_allianeId));
                command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInviteView::getAllianceMemberData), NULL));
                command->sendAndRelease();
            }
        }else{
            if(m_panelType == ITEM_GIFT || m_panelType == ITEM_DONATE){
                if(m_allianeId!=""){
                    GetAllianceMembersCommand * command = new GetAllianceMembersCommand();
                    command->putParam("allianceId", CCString::create(m_allianeId));
                    command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInviteView::getAllianceMemberData), NULL));
                    command->sendAndRelease();
                }else{
                    
                }
            }else{
                this->callMore();
            }
        }
        if(m_panelType == KINGSGIFT){
            m_inviteNode->setVisible(false);
            m_titleTxt->setPositionX(320);
            std::string titleStr = _lang_1("110130", _lang(m_dialogId).c_str());
            m_titleTxt->setString(titleStr);
        }else if(m_panelType == ITEM_GIFT){
            m_inviteNode->setVisible(false);
            m_titleTxt->setPositionX(320);
            std::string titleStr = _lang("101380");
            m_titleTxt->setString(titleStr);
        }else if(m_panelType == ITEM_DONATE){
            m_inviteNode->setVisible(false);
            m_titleTxt->setPositionX(320);
            std::string titleStr = _lang("101000");
            m_titleTxt->setString(titleStr);
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            m_titleTxt->setMaxScaleXByWidth(700);
        } else {
            m_titleTxt->setMaxScaleXByWidth(300);
        }
        
        ret = true;
    }
    return ret;
}

void AllianceInviteView::getAllianceMemberData(CCObject* param){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_data->removeAllObjects();
    NetResult* result = dynamic_cast<NetResult*>(param);
    CCDictionary* params = _dict(result->getData());
    CCArray*  members = dynamic_cast<CCArray*>(params->objectForKey("list"));
    m_uids = "";
    if(members!=NULL){
        for (int i=0; i < members->count(); i++) {
            CCDictionary* member = (CCDictionary*)members->objectAtIndex(i);
            string uid = member->valueForKey("uid")->getCString();
            if(m_panelType == ITEM_GIFT || m_panelType == ITEM_DONATE){
                if(uid.compare(GlobalData::shared()->playerInfo.uid) == 0)
                    continue;
            }
            int rank = member->valueForKey("rank")->intValue();
            string name = member->valueForKey("name")->getCString();
            string pic = member->valueForKey("pic")->getCString();
            unsigned long power = member->valueForKey("power")->longValue();
            int kills = member->valueForKey("kills")->intValue();
            bool onLine = member->valueForKey("online")->boolValue();
            int pointId = member->valueForKey("pointId")->intValue();
            int mainCityLv = member->valueForKey("mainCityLv")->intValue();
            float offLineTime =  member->valueForKey("offLineTime")->floatValue();
            string lang = member->valueForKey("lang")->getCString();
            string abbr = GlobalData::shared()->playerInfo.allianceInfo.shortName;
            if(pic==""||pic=="0"){
                pic = "g044_middle.png";
            }else{
                pic.append("_middle.png");
            }
            int userLv = member->valueForKey("level")->intValue();
            
            AllianceInfoMember* infoMember = new AllianceInfoMember();
            infoMember->setRank(rank);
            infoMember->setName(name);
            infoMember->setPoints(power);
            infoMember->setKills(kills);
            infoMember->setUid(uid);
            infoMember->setOffLineTime(offLineTime);
            infoMember->setPointId(pointId);
            infoMember->setHead(member->valueForKey("pic")->getCString());
            infoMember->setPic(pic);
            infoMember->setOnline(onLine);
            infoMember->setMainCityLv(mainCityLv);
            infoMember->setPower(power);
            infoMember->setIsManager(false);
            infoMember->setIsTitle(false);
            infoMember->setLang(lang);
            infoMember->setAbbr(abbr);
            infoMember->setUserLevel(userLv);
            m_data->addObject(infoMember);
            infoMember->release();
        }
    }
    refresh(NULL);
}

void AllianceInviteView::updateData(CCObject* param){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    if(m_searchKey!=""){
        m_data->removeAllObjects();
    }
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    CCArray* members = NULL;
    if ((dic && dic->objectForKey("list"))) {
        members = dynamic_cast<CCArray*>(dic->objectForKey("list"));
    }else{
     members = dynamic_cast<CCArray*>(result->getData());
    }
    if(m_searchKey==""){
        m_total = dic->valueForKey("total")->intValue();
    }else{
        m_uids = "";
    }
    if(members!=NULL){
        for (int i=0; i < members->count(); i++) {
            CCDictionary* member = (CCDictionary*)members->objectAtIndex(i);
            string uid = member->valueForKey("uid")->getCString();
            if(m_uids.find(uid.c_str())<m_uids.length() && m_searchKey==""){
                //有相同的，不要
                continue;
            }
            if((m_panelType == ITEM_GIFT || m_panelType == ITEM_DONATE) && uid == GlobalData::shared()->playerInfo.uid){
                continue;
            }
            
            if(m_searchKey==""){
                m_uids.append(",");
                m_uids.append(uid);
            }
            int rank = member->valueForKey("rank")->intValue();
            string name = member->valueForKey("name")->getCString();
            string pic = member->valueForKey("pic")->getCString();
            unsigned long power = member->valueForKey("power")->longValue();
            int kills = member->valueForKey("kills")->intValue();
            bool onLine = member->valueForKey("online")->boolValue();
            int pointId = member->valueForKey("pointId")->intValue();
            int mainCityLv = member->valueForKey("mainCityLv")->intValue();
            float offLineTime =  member->valueForKey("offLineTime")->floatValue();
            string lang = member->valueForKey("lang")->getCString();
            string abbr = member->valueForKey("abbr")->getCString();
            if(pic==""||pic=="0"){
                pic = "g044_middle.png";
            }else{
                pic.append("_middle.png");
            }
            int userLv = member->valueForKey("level")->intValue();
            
            AllianceInfoMember* infoMember = new AllianceInfoMember();
            infoMember->setRank(rank);
            infoMember->setName(name);
            infoMember->setPoints(power);
            infoMember->setKills(kills);
            infoMember->setUid(uid);
            infoMember->setOffLineTime(offLineTime);
            infoMember->setPointId(pointId);
            infoMember->setHead(member->valueForKey("pic")->getCString());
            infoMember->setPic(pic);
            infoMember->setOnline(onLine);
            infoMember->setMainCityLv(mainCityLv);
            infoMember->setPower(power);
            infoMember->setIsManager(false);
            infoMember->setIsTitle(false);
            infoMember->setLang(lang);
            infoMember->setAbbr(abbr);
            infoMember->setUserLevel(userLv);
            m_data->addObject(infoMember);
            infoMember->release();
        }
    }
    refresh(NULL);
}

void AllianceInviteView::removeOneData(CCObject* param){
    CCString* str = dynamic_cast<CCString*>(param);
    if(str){
        string uuid = str->getCString();
        int num = m_data->count();
        for (int i=0; i<num; i++) {
            AllianceInfoMember* infoM = dynamic_cast<AllianceInfoMember*>(m_data->objectAtIndex(i));
            if (infoM && infoM->getUid()==uuid) {
                m_data->removeObjectAtIndex(i);
                break;
            }
        }
    }
}

bool AllianceInviteView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"onInviteBtn", CCControlButton*, this->onInviteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchBtn", CCControlButton*, this->m_searchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchNode", CCNode*, this->m_searchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_inviteNode", CCNode*, this->m_inviteNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goldTxt", CCLabelIF*, this->m_goldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moreTxt", CCLabelIF*, this->m_moreTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_waitNode", CCNode*, this->m_waitNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottom_node", CCNode*, this->m_bottom_node);
    return false;
}

SEL_CCControlHandler AllianceInviteView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSearchClick", AllianceInviteView::onSearchClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInviteClick", AllianceInviteView::onInviteClick);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeClick", AllianceInviteView::onChangeClick);
    return NULL;
}

void AllianceInviteView::onSearchClick(CCObject *pSender, CCControlEvent event){
    m_searchKey = m_inputName->getText();

    if(m_officeId!=0 || (m_panelType == KINGSGIFT || m_panelType == ITEM_GIFT || m_panelType == ITEM_DONATE)){
        WorldSearchUserCommand* cmd = new WorldSearchUserCommand(m_searchKey,1);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInviteView::updateData), NULL));
        cmd->sendAndRelease();
    }else{
        SearchUserAllianceCommand* cmd = new SearchUserAllianceCommand(m_searchKey,m_lang,1);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInviteView::updateData), NULL));
        cmd->sendAndRelease();
    }
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_waitInterface = GameController::getInstance()->showWaitInterface(m_waitNode);
}

void AllianceInviteView::onInviteClick(CCObject *pSender, CCControlEvent event){
    if(GlobalData::shared()->playerInfo.allianceInfo.rank>=4){
        YesNoDialog::showButtonAndGold(_lang("115263").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceInviteView::inviteFun)),_lang("105302").c_str(),GlobalData::shared()->invite_cost_k1);
    }else{
        CCCommonUtils::flyText(_lang("115272"));
    }
}

void AllianceInviteView::inviteFun(){
    if(GlobalData::shared()->invite_cost_k1>GlobalData::shared()->playerInfo.gold){
        YesNoDialog::gotoPayTips();
        return ;
    }
    string str = _lang_1("115261",GlobalData::shared()->playerInfo.allianceInfo.name.c_str());
    ChatController::getInstance()->sendCountryChat(str.c_str(), CHAT_COUNTRY, CHAT_TYPE_INVITE);
}

void AllianceInviteView::onChangeClick(CCObject *pSender, CCControlEvent event){
    SearchUserAllianceCommand* cmd = new SearchUserAllianceCommand("",m_lang,1);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInviteView::updateData), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void AllianceInviteView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize AllianceInviteView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(706, 480);
    }
    return CCSize(320, 220);
}

CCSize AllianceInviteView::tableCellSizeForIndex(CCTableView *table, ssize_t idx){
    
    if(idx >= m_data->count()){
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(706, 120);
        } else {
            return CCSize(320, 60);
        }
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(706, 480);
    } else {
        return CCSize(320, 220);
    }
}

CCTableViewCell* AllianceInviteView::gridAtIndex(CCMultiColTableView *table, unsigned int idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    AllianceUserCell* cell = (AllianceUserCell*)table->dequeueGrid();
    AllianceInfoMember* info = dynamic_cast<AllianceInfoMember*>(m_data->objectAtIndex(idx));
    if(cell==NULL){
        cell = AllianceUserCell::create();
    }
    cell->setData(info,m_infoList,m_officeId,m_panelType,m_giftId,m_dialogId);
    return cell;
}

ssize_t AllianceInviteView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_data->count();
    num = num/2 + (num%2==0?0:1) + 1;
    return num;
}

unsigned int AllianceInviteView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 2;
}

//
bool AllianceUserCell::init()
{
    bool ret = true;
    
    if (CCNode::init()) {
//        setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(307, false);
//            CCLoadSprite::doResourceByCommonIndex(7, false);
//        });

        m_ccbNode = CCBLoadFile("NewAllianceMemberCell", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_ccbNode->setPositionX(m_ccbNode->getPositionX() + 10);
        }
        CCSize size = m_ccbNode->getContentSize();
        setContentSize(size);
        std::string lang = cocos2d::extension::CCDevice::getLanguage();
//        if (lang == "en" || lang == "en-US")
//        {
//            m_powerTxt->setFntFile("pve_fnt_title.fnt");
//        }
        m_inviteId = "";
        
        m_powerTxt->setFntFile(getNBFont(NB_FONT_Bold_Border));
        m_powerValue->setFntFile(getNBFont(NB_FONT_Bold_Border));
        
        return true;
    }
    
    return ret;
}

void AllianceUserCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceUserCell::playInviteAni), MSG_PLAY_REMOVE_INVITE_ANI, NULL);
}

void AllianceUserCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PLAY_REMOVE_INVITE_ANI);
    CCNode::onExit();
}

bool AllianceUserCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickNode1, pTouch) || isTouchInside(m_clickNode2, pTouch)){
        return true;
    }
    return false;
}

void AllianceUserCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(fabs(m_touchPos.y - end.y)<40 && isTouchInside(m_clickArea, pTouch)){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        if(m_panelType == KINGSGIFT){
            if(false){//是否已经发了国王奖励
                CCCommonUtils::flyHint("", "", _lang("110117"));
            }else{
                std::string str = _lang_2("110118", m_info->getName().c_str(), _lang(m_dialogId).c_str());
                YesNoDialog::showVariableTitle(str.c_str(),CCCallFunc::create(this, callfunc_selector(AllianceUserCell::inviteCallBack)),_lang("confirm").c_str());
            }
        }else{
            if(m_officeId==0){
                if(m_panelType==ITEM_GIFT){
                    PopupViewController::getInstance()->goBackPopupView();
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("msg.user.choose",CCString::create(m_info->getName()));
                }else if(m_panelType == ITEM_DONATE){
                    auto iter= GlobalData::shared()->goldExchangeList.find(m_giftId);
                    if(iter!=GlobalData::shared()->goldExchangeList.end()){
                        if(LuaController::getInstance()->canSendExchange(m_giftId, m_info->getUid(), m_info->getName(),atoi((*iter).second->type.c_str()),true)){
                            std::string showTip = _lang_2("101002",_lang((*iter).second->name).c_str(), m_info->getName().c_str());
                            showTip += "\n";
                            showTip += _lang("101003");
                            YesNoDialog::showVariableTitle(showTip.c_str(),CCCallFunc::create(this, callfunc_selector(AllianceUserCell::sureToDonate)),_lang("confirm").c_str());
                        }
                    }
                }else{
                    if(GlobalData::shared()->playerInfo.allianceInfo.rank>=4){
                        YesNoDialog::showVariableTitle(_lang("115271").c_str(),CCCallFunc::create(this, callfunc_selector(AllianceUserCell::inviteCallBack)),_lang("confirm").c_str());
                    }else{
                        CCCommonUtils::flyText(_lang("115272"));
                    }
                }
            }else{
                string officeName = CCCommonUtils::getNameById(CC_ITOA(m_officeId));
                YesNoDialog::showVariableTitle(_lang_2("110083",m_info->getName().c_str(),officeName.c_str()),CCCallFunc::create(this, callfunc_selector(AllianceUserCell::inviteCallBack)),_lang("confirm").c_str());
            }
        }
    }
}

void AllianceUserCell::playInviteAni(CCObject* param){
    CCString* str = dynamic_cast<CCString*>(param);
    if (str==NULL) return;
    string uid = str->getCString();
    if(m_info && uid==m_info->getUid()){
        m_ccbNode->stopAllActions();
        CCSequence* sc = CCSequence::create(CCScaleTo::create(0.4, 0.03),CCCallFunc::create(this, callfunc_selector(AllianceUserCell::postionMSG)),NULL);
        m_ccbNode->runAction(sc);
    }
}

void AllianceUserCell::postionMSG(){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESH_INVITE_DATA);
}
void AllianceUserCell::sureToDonate(){
    if(!m_info){
        return;
    }
    if(m_panelType == ITEM_DONATE){
        PopupViewController::getInstance()->removeAllPopupView();
        LuaController::getInstance()->callPaymentToFriend(m_giftId, m_info->getUid(),m_info->getName());
    }
}
void AllianceUserCell::inviteCallBack(){
    if(!m_info){
        return;
    }
    if(m_panelType == KINGSGIFT){
        KingsGiftPostCommand *cmd = new KingsGiftPostCommand(m_info->getUid(), m_giftId, m_info->getName(), _lang(m_dialogId));
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceUserCell::sendGiftSuccess), NULL));
        cmd->sendAndRelease();
    }else{
        if(m_officeId==0){
            m_inviteId = m_info->getUid();
            InvitesAllianceCommand* cmd = new InvitesAllianceCommand(m_info->getUid());
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceUserCell::inviteSuccess), NULL));
            cmd->sendAndRelease();
        }else{
            KingdomExecCommand* cmd = new KingdomExecCommand();
            cmd->putParam("type", CCInteger::create(APPOINT));
            cmd->putParam("uid", CCString::create(m_info->getUid()));
            cmd->putParam("posId", CCString::create(CC_ITOA(m_officeId)));
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceUserCell::appointedSuccess), NULL));
            cmd->sendAndRelease();
        }
    }
    GameController::getInstance()->showWaitInterface();
}

void AllianceUserCell::appointedSuccess(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if(result==NULL) return;
    CCDictionary* dict = dynamic_cast<CCDictionary*>(result->getData());
    if(dict){
        if(!dict->objectForKey("success")){
            PopupViewController::getInstance()->removeAllPopupView();
            PopupViewController::getInstance()->addPopupInView(ManagerKingdomView::create());
        }else{
            CCCommonUtils::flyHint("", "", _lang_1("110094",""));
        }
    }
}

void AllianceUserCell::sendGiftSuccess(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if(result==NULL) return;
    CCDictionary* dict = dynamic_cast<CCDictionary*>(result->getData());
    if(dict==NULL || dict->objectForKey("errorCode")) return;
}

void AllianceUserCell::inviteSuccess(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if(result==NULL) return;
    CCDictionary* dict = dynamic_cast<CCDictionary*>(result->getData());
    if(dict==NULL || dict->objectForKey("errorCode") || m_info == NULL) return;
    CCCommonUtils::flyHint("", "", _lang("115259"));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAY_REMOVE_INVITE_ANI,CCString::create(m_inviteId));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_ONE_INVITE_DATA,CCString::create(m_inviteId));
}

void AllianceUserCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

SEL_CCControlHandler AllianceUserCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

void AllianceUserCell::setData(AllianceInfoMember* member,CCNode* clickArea,int officeId, int panelType,std::string giftId,std::string dialogId)
{
    m_dialogId = dialogId;
    m_info = member;
    m_clickArea = clickArea;
    m_officeId = officeId;
    m_panelType = panelType;
    m_giftId = giftId;
    if(m_info==NULL) return ;
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(307, true);

    m_ccbNode->setScale(1.0);
    m_headIcon->removeAllChildrenWithCleanup(true);
    CCSprite* spr = CCLoadSprite::createSprite(m_info->getPic().c_str());
    spr->setAnchorPoint({0.5, 0});
    spr->setScale(.5);
//    spr->setScale(0.8);
//    if (CCCommonUtils::isIosAndroidPad()) {
//        spr->setScale(1);
//    }
    m_headIcon->addChild(spr);
    m_nameTxt->setString(m_info->getName().c_str());
//    m_powerTxt->setString(_lang("102163").c_str());
    string lv = "LV."; lv.append(CC_ITOA(m_info->getUserLevel()));
    m_powerTxt->setString(lv);
    m_powerValue->setString(CC_CMDITOAL(m_info->getPower()));
//    if(m_info->getOnline()){
//        m_powerValue->setColor({255,235,191});
//        std::string lang = cocos2d::extension::CCDevice::getLanguage();
//        if (lang=="en") {
//            m_powerTxt->setColor({255,255,255});
//        }else{
//            m_powerTxt->setColor({239,211,0});
//        }
//    }else{
//        m_powerTxt->setColor({172,172,172});
//        m_powerValue->setColor({172,172,172});
//    }
    
    m_titleFlag->setVisible(false);
//    m_flagBg->initWithSpriteFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Alliance_Flag_01.png")->getCString()));
    if(officeId!=0 || m_panelType == KINGSGIFT){
        m_offLineTime->setString(m_info->getAbbr());
    }else{
        m_offLineTime->setString(CCCommonUtils::getLanguageFNByLocalSN(m_info->getLang()).c_str());
    }
    
//    m_offLineTime->setColor({111,161,70});
//    m_offLineTime->setPositionY(-27);
    
    bool showShine = false;
    if(m_panelType==ITEM_DONATE){
        if(LuaController::getInstance()->canSendExchange(m_giftId, m_info->getUid(), m_info->getName()) == false){
            showShine = true;
            
        }else{
            showShine=false;
        }
    }
    int iindex = 0;
    while (iindex<2) {
        this->m_nodeHide[iindex]->setVisible(showShine);
        ++iindex;
    }
}

bool AllianceUserCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headIcon", CCNode*, this->m_headIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_offLineTime", CCLabelIF*, this->m_offLineTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerValue", CCLabelIF*, this->m_powerValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleFlag", CCSprite*, this->m_titleFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagBg", CCSprite*, this->m_flagBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode1", CCNode*, this->m_clickNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode2", CCNode*, this->m_clickNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeHide1", CCNode*, this->m_nodeHide[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeHide2", CCNode*, this->m_nodeHide[1]);
    return false;
}
