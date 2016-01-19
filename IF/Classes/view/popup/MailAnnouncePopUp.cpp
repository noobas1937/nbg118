//
//  MailAnnouncePopUp.cpp
//  IF
//
//  Created by lifangkai on 14-7-24.
//
//

#include "MailAnnouncePopUp.h"
#include "PopupViewController.h"
#include "MailController.h"
#include "RewardController.h"
#include "MailPopUpView.h"
#include "GeneralHeadPic.h"
#include "WorldController.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "MailSaveCommand.h"
#include "MailGetRewardCommand.h"
#include "../../Ext/CCDevice.h"
#include "SoundController.h"
#include "EquipmentController.h"
#include "MailWritePopUpView.h"
#include "UIComponent.h"
#include "FunBuildController.h"
#include "LuaController.h"

MailAnnouncePopUp *MailAnnouncePopUp::create(MailInfo *info){
    MailAnnouncePopUp *ret = new MailAnnouncePopUp(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MailAnnouncePopUp::onEnter(){
    PopupBaseView::onEnter();
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade,true);//simon;
    UIComponent::getInstance()->hideReturnBtn();
    //CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(OccupyMailPopUpView::refresh), MAIL_CONTENT_READ, NULL);
     setTouchEnabled(true);
}

void MailAnnouncePopUp::onExit(){
   // CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_CONTENT_READ);
     setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool MailAnnouncePopUp::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        setMailUuid(m_info->uid);
//        auto cf = CCLoadSprite::getSF("Mail_diban.png");
        auto cf = CCLoadSprite::getSF("Mail_BG1.png");
        if (cf==NULL) {
            CCLoadSprite::doResourceByCommonIndex(6, true); //simon
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(6, false); //simon
                CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
            });
        }
        else {
            setCleanFunction([](){
                CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
            });
        }
        auto bg = CCBLoadFile("NEW_MailAnnounce", this, this);
        if(bg==NULL)
        {
            CCLOGFUNC("bg==NULL");
        }
        else
        {
            CCLOGFUNC("bg!=NULL");
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
            int extH = getExtendHeight();
            this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + extH));
            m_downNode->setPositionY(m_downNode->getPositionY() - extH);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
            auto tbg = CCLoadSprite::loadResource("Mail_diban.png");
            auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
            auto picBg1 = CCLoadSprite::createSprite("Mail_diban.png");
            picBg1->setAnchorPoint(ccp(0, 0));
            picBg1->setPosition(ccp(0, 0));
            picBg1->setScaleX(2.4);
            tBatchNode->addChild(picBg1);
            int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
            int curHeight = picBg1->getContentSize().height;
            while(curHeight < maxHeight)
            {
                auto picBg2 = CCLoadSprite::createSprite("Mail_diban.png");
                picBg2->setAnchorPoint(ccp(0, 0));
                picBg2->setPosition(ccp(0, curHeight));
                picBg2->setScaleX(2.4);
                tBatchNode->addChild(picBg2);
                curHeight += picBg2->getContentSize().height;
            }
            m_bgNode->addChild(tBatchNode);
        }
        else {
            this->setContentSize(bg->getContentSize());
            int extH = getExtendHeight();
            this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + extH));
            m_downNode->setPositionY(m_downNode->getPositionY() - extH);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
            auto tbg = CCLoadSprite::loadResource("Mail_diban.png");
            auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
            auto picBg1 = CCLoadSprite::createSprite("Mail_diban.png");
            picBg1->setAnchorPoint(ccp(0, 0));
            picBg1->setPosition(ccp(0, 0));
            tBatchNode->addChild(picBg1);
            int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
            int curHeight = picBg1->getContentSize().height;
            while(curHeight < maxHeight)
            {
                auto picBg2 = CCLoadSprite::createSprite("Mail_diban.png");
                picBg2->setAnchorPoint(ccp(0, 0));
                picBg2->setPosition(ccp(0, curHeight));
                tBatchNode->addChild(picBg2);
                curHeight += picBg2->getContentSize().height;
            }
            m_bgNode->addChild(tBatchNode);
        }
        
        m_updateBtn->setEffectStr("");
        m_listNode = CCNode::create();
//        setTitleName(_lang("102996"));
        m_mailTitle->setString(_lang("102996"));
        CCLOGFUNCF("m_info->createTime %d",m_info->createTime);
        this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());
        m_titleText->setString(_lang_1("114107", m_info->version.c_str()));
        m_titleText->setFntFile("Arial_Bold_Regular.fnt");
        m_timeText->setFntFile("Arial_Bold_Regular.fnt");
        m_updateContent->setFntFile("Arial_Bold_Regular.fnt");
        m_rewardTitleText->setString(_lang("114109"));
        m_updateTitle->setString(_lang("114108"));
        
        m_scrollView = CCScrollView::create(m_listContainer->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setAnchorPoint(ccp(0, 0));
        m_listContainer->addChild(m_scrollView);

        
        this->m_totalNode->removeChild(this->m_moveNode);
        m_scrollView->addChild(m_listNode);
        
        m_updateContentRich= CCRichLabelTTF::create("", "Helvetica", m_updateContent->getFontSize(),m_updateContent->getDimensions(),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
        m_updateContent->getParent()->addChild(m_updateContentRich);
        m_updateContentRich->setPosition(m_updateContent->getPosition());
        m_updateContentRich->setAnchorPoint(m_updateContent->getAnchorPoint());
        m_updateContent->removeFromParent();
        m_updateContent=NULL;

        
        m_totalHg = 0;
        m_listNode->addChild(m_moveNode);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_moveNode->setPosition(768,m_totalHg);
        }
        else
            m_moveNode->setPosition(320,m_totalHg);
        if(!m_info->isReadContent){
            getData();
        }else{
            refresh();
            this->m_listNode->setPositionY(-m_totalHg);
            m_scrollView->setContentSize(CCSize(m_listContainer->getContentSize().width, -m_totalHg));
            m_scrollView->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (-m_totalHg)));
        }
        ret = true;
    }
    return ret;
}

void MailAnnouncePopUp::onUpdateBtnClick(CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    if(m_versionNum==2){
        if(m_info->mbLevel>0 && m_info->mbLevel >= FunBuildController::getInstance()->getMainCityLv())
        {
            CCCommonUtils::flyText(_lang_1("105784", CC_ITOA(m_info->mbLevel)));
            return;
        }
        
        this->m_updateBtn->setVisible(false);
        if(m_sprAnim){
            m_sprAnim->setVisible(false);
        }
        SoundController::sharedSound()->playEffects(Music_Sfx_button_click_reward);
        MailGetRewardCommand* cmd = new MailGetRewardCommand(m_info->uid, m_info->type);
        m_info->rewardStatus = 1;
        cmd->sendAndRelease();
        refreshRewardStatus();

    }else if(m_versionNum==1){
        GameController::getInstance()->updateVersion();
    }
}

bool MailAnnouncePopUp::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_mailTitle", CCLabelIF*, this->m_mailTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_contentText", CCLabelIF*, this->m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardTitleText", CCLabelIF*, this->m_rewardTitleText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_deleteBtnTitle", CCLabelIF*, this->m_deleteBtnTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ListStartNode", CCNode*, this->m_ListStartNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnBtn", CCControlButton*, this->m_returnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnSpr", CCSprite*, this->m_returnSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_writeBtn", CCControlButton*, this->m_writeBtn);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_updateBtn", CCControlButton*, this->m_updateBtn);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_updateContent", CCLabelIFTTF*, this->m_updateContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_updateTitle", CCLabelIF*, this->m_updateTitle);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipText", CCLabelIF*, this->m_tipText);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listBG", CCScale9Sprite*, this->m_listBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kuangBG", CCScale9Sprite*, this->m_kuangBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveDownListNode", CCNode*, this->m_moveDownListNode);
    
    return false;
}
bool MailAnnouncePopUp::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_returnSpr, pTouch)) {
        return true;
    }
    return false;
}
void MailAnnouncePopUp::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void MailAnnouncePopUp::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_returnSpr, pTouch)) {
        PopupViewController::getInstance()->goBackPopupView();
    }
}
void MailAnnouncePopUp::onDeleteClick(CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    
    if(m_versionNum==2&&m_info->rewardStatus==0&&m_info->rewardId!=""){
        CCCommonUtils::flyHint("", "", _lang("105512"));
        return;
    }
    if(m_info->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(MailAnnouncePopUp::onOKDeleteMail)),true);
    
}
void MailAnnouncePopUp::onAddSaveClick(CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    
    if(m_info->save ==1){
        MailCancelSaveCommand *cmd = new MailCancelSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
    }else{
        if(MailController::getInstance()->isMailFull(SAVEMAIL)){
            CCCommonUtils::flyText("full");
            return;
        }
        MailSaveCommand *cmd = new MailSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
    }
    if(m_info->save==0){
        m_addSaveBtn->setHighlighted(false);
    }else{
        m_addSaveBtn->setHighlighted(true);
    }
}
void MailAnnouncePopUp::onReturnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void MailAnnouncePopUp::onWriteClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
}
void MailAnnouncePopUp::onOKDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    PopupViewController::getInstance()->goBackPopupView();
    
}
SEL_CCControlHandler MailAnnouncePopUp::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MailAnnouncePopUp::onDeleteClick);
  //  CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnPosClick", MailAnnouncePopUp::onBtnPosClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MailAnnouncePopUp::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturnClick", MailAnnouncePopUp::onReturnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteClick", MailAnnouncePopUp::onWriteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUpdateBtnClick", MailAnnouncePopUp::onUpdateBtnClick);
    return NULL;
}

void MailAnnouncePopUp::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
}

cocos2d::CCSize MailAnnouncePopUp::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(200, 160);
}

CCTableViewCell* MailAnnouncePopUp::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
   // if(idx >= m_info->occupyGeneral->count()){
        return NULL;
    //}
//    CCDictionary *dict = dynamic_cast<CCDictionary*>(m_info->occupyGeneral->objectAtIndex(idx));
//    OccupyMailGeneralCell* cell = (OccupyMailGeneralCell*)table->dequeueGrid();
//    std::string generalId = dict->valueForKey("genId")->getCString();
//    int level = dict->valueForKey("level")->intValue();
//    int color = dict->valueForKey("color")->intValue();
//    int num = dict->valueForKey("armyNum")->intValue();
//    if(idx < m_info->occupyGeneral->count()){
//        if(cell){
//            cell->setData(generalId, num, color, level);
//        }else{
//            cell = OccupyMailGeneralCell::create(generalId, num, color, level);
//        }
//    }
//    return cell;
}

ssize_t MailAnnouncePopUp::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    if(m_info->occupyGeneral == NULL){
        return 0;
    }
    int num = 0;
    return num;
}

unsigned int MailAnnouncePopUp::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 2;
}
void MailAnnouncePopUp::refreshRewardStatus(){
    m_totalHg = 0;
    m_listNode->removeAllChildren();
    m_listNode->addChild(m_moveNode);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_moveNode->setPosition(768, m_totalHg);
    }
    else
        m_moveNode->setPosition(320,m_totalHg);

    refresh(true);
    this->m_listNode->setPositionY(-m_totalHg);
    m_scrollView->setContentSize(CCSize(m_listContainer->getContentSize().width, -m_totalHg));
    m_scrollView->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (-m_totalHg)));

}

void MailAnnouncePopUp::refresh(bool isrefresh){
    if(m_info->save==0){
        m_addSaveBtn->setHighlighted(false);
    }else{
        m_addSaveBtn->setHighlighted(true);
    }
    m_versionNum = 0;
    string appversion =cocos2d::extension::CCDevice::getVersionName();
    vector<std::string> appvector;
    CCCommonUtils::splitString(appversion, ".", appvector);
    if(appvector.size()<3){
        return;
    }
    int j=0;
    int apptotal;
    apptotal =atoi(appvector[0].c_str())*10000+atoi(appvector[1].c_str())*100+atoi(appvector[2].c_str())*1;
    
    vector<std::string> appvector1;
    
    CCCommonUtils::splitString(m_info->version, ".", appvector1);
    if(appvector1.size()<3){
        return;
    }
    int apptotalnew;
    apptotalnew =atoi(appvector1[0].c_str())*10000+atoi(appvector1[1].c_str())*100+atoi(appvector1[2].c_str())*1;
    if(apptotalnew>apptotal){
        bool isUrlNull =  GlobalData::shared()->downloadUrl.empty() || GlobalData::shared()->downloadUrl=="null";
        
        if (isUrlNull){
            m_updateBtn->setVisible(false);
        }
        CCCommonUtils::setButtonTitle(m_updateBtn, _lang("102999").c_str());
        //        m_tipText->setVisible(false);
        m_versionNum = 1;
    }else if(m_info->rewardStatus==0&&m_info->rewardId != ""){
        CCCommonUtils::setButtonTitle(m_updateBtn, _lang("105572").c_str());
        m_versionNum = 2;
    }else{
        m_updateBtn->setVisible(false);
        
    }
    
    string content = m_info->contents;
    content+="                  ";
    if(m_info->mbLevel>0 && m_info->mbLevel >= FunBuildController::getInstance()->getMainCityLv()){
        string str =_lang_1("105784",CC_ITOA(m_info->mbLevel));
        content=str+"\r\n "+"\r\n"+content;
    }
//    string tmpStr = "fubin1 \r\n fubin2 \r\n fubin3 \r\n\r\n fubin4 \r\n fubin5";
    while(true)   {
        string::size_type   pos(0);
        if(   (pos=content.find("\r"))!=string::npos   )
            content.replace(pos,2,"\n ");
        else   break;
    }
//    content = "2Open Rankings\n \n-Check the Rankings in the Lord Interface\n-Unlock the Individual Power and Alliance Power Rankings\n \n-More Rankings coming soon\n \n\nNew Event\n \n-Unlocked new round of activity of 7 stages for 10 days (New server opening soon)\n \n-Complete 3 targets in each stage to get Target Rewards\n \n-Top 100 ranking Lords in each stage to get Stage Ranking Rewards\n \n-Top 100 Lords during the whole event will get Superpower Lord Ranking Rewards\n \n\nBalance Modifications\n-Reduce the attack of World Monsters\n \n-Lower the Science and research condition for Senior Alliance\n \n\nOther Optimization\n \n-Optimized some displaying matters on the Alliance Military Intelligence Interface\n \n-Added detailed Power status in My Details\n \n-Added Construction Power display in the Construction's More Information                  ";
    
    
    string tempstr=content;
    bool hasLink = false;
    std::string findString1 = "<a href=\"";
    std::string findString2 = "\">";
    std::string findString3 = "</a>";
    int posStart = tempstr.find(findString1);
    int posMiddle = tempstr.find(findString2);
    int posEnd= tempstr.find(findString3);
    int index = 0;
    std::string showStr = "";
    while(posStart!=string::npos && posMiddle!=string::npos && posEnd!=string::npos){
        hasLink=true;
        std::string subStr1 = tempstr.substr(0,posStart);
        posStart = posStart + findString1.length();
        std::string subStr2 = tempstr.substr(posStart,posMiddle - posStart);
        m_linkUrls.push_back(subStr2);
        posStart = posMiddle + findString2.length();
        std::string subStr3 = tempstr.substr(posStart,posEnd - posStart);
        posStart = posEnd + findString3.length();
        tempstr= tempstr.substr(posStart);
        showStr += subStr1 + "[u][color=ff0000ff][link bg=00000000 bg_click=00000000]"+subStr3 +"[/link][/color][/u]";
        ++index;
        posStart = tempstr.find(findString1);
        posMiddle = tempstr.find(findString2);
        posEnd= tempstr.find(findString3);
    }
    if(hasLink==false){
        tempstr ="[color=ff380800]" + tempstr + "[/color]";
        m_updateContentRich->setString(tempstr.c_str());
    }else{
        if(!tempstr.empty()){
            showStr += tempstr;
        }
        showStr = "[color=ff380800]" + showStr  + "[/color]" + "\n";
        m_updateContentRich->setString(showStr.c_str());
        while (index>0) {
            m_updateContentRich->setLinkTarget(index-1, CCCallFuncO::create(this, callfuncO_selector(MailAnnouncePopUp::onLinkClicked), NULL));
            --index;
        }
    }

//    m_updateContent->setString(content.c_str());
    
//    m_tipText->setString(_lang("114106"));
    //m_updateContent->setContentSize(CCSize(m_updateContent->getContentSize().width, m_updateContent->getContentSize().height+20));
//    m_tipText->setVisible(false);
    int w = this->m_listContainer->getContentSize().width;
    if(m_info->rewardId != ""){
        vector<std::string> vector1;
        vector<std::string> vector;
        
        CCCommonUtils::splitString(m_info->rewardId, "|", vector);
        int i = 0;
        bool gray = (m_info->rewardStatus!=0);
        int dh = 0;
        
        int startcellx = 0;
        int start = 0;
        
        while(i < vector.size()){
            std::string rewardStr = vector[i];
            vector1.clear();
            CCCommonUtils::splitString(rewardStr, ",", vector1);
            int type = 0;
            int value = 0;
            int num = 0;
            if(vector1.size()>=3){
                type = RewardController::getInstance()->getTypeByName(vector1[0]);
                value = atoi(vector1[1].c_str());
                num = atoi(vector1[2].c_str());
            }

            std::string nameStr;
            std::string picStr;
            auto cellNode = CCNode::create();
            m_ListStartNode->addChild(cellNode);
            if (CCCommonUtils::isIosAndroidPad()) {
                cellNode->setScale(2.4);
            }
            auto icon0  = CCLoadSprite::createSprite("icon_kuang.png");
            icon0->setPositionX(57);
            icon0->setPositionY(57);
            cellNode->addChild(icon0);
            icon0->setScale(98/icon0->getContentSize().width);
            float sacle = 1.0;
            if(type == R_GOODS){
                CCCommonUtils::createGoodsIcon(value, icon0, CCSize(114, 114));
//                cellNode->setPosition(ccp(59, 49));
//                icon0->setPositionX(0);
//                icon0->setPositionY(0);
                nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
                if(m_info->rewardStatus==1 && icon0->getChildByTag(GOODS_ICON_TAG)){
                    CCSprite *icon = dynamic_cast<CCSprite*>(icon0->getChildByTag(GOODS_ICON_TAG));
                    if(icon){
                        icon->setColor({90,85,81});
                    }
                }
            }else if(type == R_GENERAL || type == R_EQUIP){
                nameStr = CCCommonUtils::getNameById(CC_ITOA(value));
                picStr =CCCommonUtils::getIcon(CC_ITOA(value));
                //float sacle = 1.0;
                if(picStr==".png"){
                    picStr = "no_iconFlag.png";
                    sacle = 94/98;
                }
                Sprite* icon = nullptr;
                if (type == R_EQUIP) {
                    icon = CCLoadSprite::createSprite(picStr.c_str(), true, CCLoadSpriteType_EQUIP);
                } else {
                    icon = CCLoadSprite::createSprite(picStr.c_str());
                }
                sacle = 94/icon->getContentSize().width;
                cellNode->addChild(icon,1);
                icon->setScale(sacle);
                icon->setPositionX(10+57);
                icon->setPositionY(57);
                if(m_info->rewardStatus==1){
                    icon->setColor({90,85,81});
                }
                
                string colorBg = "";
                /*if (type == R_GOODS) {
                    auto iter = ToolController::getInstance()->m_toolInfos.find(value);
                    if(iter != ToolController::getInstance()->m_toolInfos.end()){
                        ToolInfo& info = ToolController::getInstance()->getToolInfoById(value);
                        nameStr = info.getName();
                        colorBg = CCCommonUtils::getToolBgByColor(info.color);
                    }
                }else */if (type == R_EQUIP) {
                    auto equipIter = EquipmentController::getInstance()->EquipmentInfoMap.find(value);
                    if(equipIter != EquipmentController::getInstance()->EquipmentInfoMap.end()){
                        int color = EquipmentController::getInstance()->EquipmentInfoMap[value].color;
                        colorBg = CCCommonUtils::getToolBgByColor(color);
                    }
                }
                
                if (colorBg != "") {
                    auto iconBG = CCLoadSprite::createSprite(colorBg.c_str());
                    CCCommonUtils::setSpriteMaxSize(iconBG, 85);
                    cellNode->addChild(iconBG,0);
                    iconBG->setPosition(icon->getPosition());
                }
            }else{
                nameStr = CCCommonUtils::getResourceNameByType(type);
                picStr =CCCommonUtils::getResourceIconByType(type);

                if(picStr==".png"){
                    picStr = "no_iconFlag.png";
                    sacle = 94/98;
                }
                
                auto icon  = CCLoadSprite::createSprite(picStr.c_str());
                sacle = 94/icon->getContentSize().width;
                cellNode->addChild(icon);
                icon->setScale(sacle);
                icon->setPositionX(10+57);
                icon->setPositionY(57);
                if(m_info->rewardStatus==1){
                    icon->setColor({90,85,81});
                }
               
            }

            auto label = CCLabelIF::create();
            label->setFntFile("Arial_Bold_Regular.fnt");
            label->setFontSize(22);
            label->setColor({125,98,63});

            label->setString(nameStr);
            label->setAnchorPoint(ccp(0, 0.5));
            cellNode->addChild(label);
            auto label1 = CCLabelIF::create();
            label1->setFntFile("Arial_Bold_Regular.fnt");
            label1->setFontSize(20);
            label1->setColor({156,18,0});
            label1->setAnchorPoint(ccp(1.0, 0.5));
            
            label1->setString(CC_CMDITOA(num));
            label1->setAnchorPoint(ccp(0, 0));
            cellNode->addChild(label1);
            if(m_info->rewardStatus==1){
                label->setColor({90,85,81});
                label1->setColor({90,85,81});
                
            }
            if(type == R_GOODS){
                if (CCCommonUtils::isIosAndroidPad()) {
                    label->setPosition(110, 80);
                    label1->setPosition(600, 20);
                    cellNode->setPositionY(-i*288);
                }
                else {
                    label->setPosition(110, 80);
                    label1->setPosition(600, 20);
                    cellNode->setPositionY(-i*120);
                }
            }else{
                if (CCCommonUtils::isIosAndroidPad()) {
                    label->setPosition(110, 80);
                    label1->setPosition(600, 20);
                    cellNode->setPositionY(-i*288);
                }
                else {
                    label->setPosition(110, 80);
                    label1->setPosition(600, 20);
                    cellNode->setPositionY(-i*120);
                }
            }

            i++;
        }
        if(!isrefresh){
            if (CCCommonUtils::isIosAndroidPad()) {
                int listbgH = m_listBG->getContentSize().height;
                m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, listbgH+(i-1)*288));
                m_moveDownListNode->setPositionY(m_moveDownListNode->getPositionY() - (i-1)*288);
            }
            else {
                int listbgH = m_listBG->getContentSize().height;
                m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, listbgH+(i-1)*120));
                m_moveDownListNode->setPositionY(m_moveDownListNode->getPositionY() - (i-1)*120);
            }
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalHg-= (i-1)*288;
        }
        else {
            m_totalHg-= (i-1)*120;
        }
    }
//    if(m_versionNum==2){
//        m_sprAnim = CCLoadSprite::createSprite("Mail_btn00.png");
//        m_updateBtn->getParent()->addChild(m_sprAnim);
//        m_sprAnim->setScaleX((m_updateBtn->getPreferredSize().width)/m_sprAnim->getContentSize().width);
//        m_sprAnim->setScaleY((m_updateBtn->getPreferredSize().height)/m_sprAnim->getContentSize().height);
//        ccBlendFunc cbf = {GL_ONE,GL_ONE};
//        m_sprAnim->setBlendFunc(cbf);
//        m_sprAnim->setPosition(m_updateBtn->getPosition());
//        m_sprAnim->setOpacity(0);
//        m_sprAnim->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(1,80),CCFadeTo::create(1,0),NULL)));
//    }
    if (CCCommonUtils::isIosAndroidPad()) {
        m_totalHg-=730;
    }
    else {
        m_totalHg-=304;
    }
    m_totalHg-= m_updateContentRich->getContentSize().height;
    // auto head = CCLoadSprite::createSprite(pic.c_str());
    //startY = 100;
//    if(m_info->occupyGeneral->count()>0){
//        CCDictionary *dict = dynamic_cast<CCDictionary*>(m_info->occupyGeneral->objectAtIndex(0));
//        this->m_failNode->setVisible(false);
//        //m_picHeadNode->setVisible(false);
//        this->m_armsName->setString(_lang("108557"));
//        this->m_armsNun->setString(_lang("105558"));
//        this->m_kuangBG->setVisible(true);
//        this->m_kuangBG->setPositionY(this->m_kuangBG->getPositionY()+startY);
//        this->m_armsName->setPositionY(this->m_armsName->getPositionY()+startY);
//        this->m_armsNun->setPositionY(this->m_armsNun->getPositionY()+startY);
//        this->m_armsListNode->setPositionY(this->m_armsListNode->getPositionY()+startY);
//        
//        CCObject *obj = NULL;
//        int i = 0;
//        int posX = 0;
//        CCARRAY_FOREACH(m_info->occupyGeneral, obj){
//            CCDictionary *reward = dynamic_cast<CCDictionary*>(obj);
//            string id = reward->valueForKey("armyId")->getCString();
//            string armname =CCCommonUtils::getNameById(id);
//            int num = atoi(reward->valueForKey("armyNum")->getCString());
//            auto cell = OccupyLossPreviewCell::create(armname, num);
//            cell->setAnchorPoint(ccp(0, 0));
//            
//            cell->setPosition(ccp(posX,  -20-i*40));
//            m_armsListNode->addChild(cell);
//            i++;
//        }
}

void MailAnnouncePopUp::onLinkClicked(CCObject *ccObj) {
    CCLOG("on link clicked");
    if(ccObj){
        CCInteger *integer = dynamic_cast<CCInteger*>(ccObj);
        if(integer){
            int index = integer->getValue();
            if(index < m_linkUrls.size()){
                std::string url = m_linkUrls.at(index);
                if(!url.empty()){
                    url = LuaController::getInstance()->encodeUrl(url);
                    CCLOG("link url [%s]",url.c_str());
                    GameController::getInstance()->goTurntoUrl(url);
                }
            }
        }
    }
}



void MailAnnouncePopUp::getData(){
    MailController::getInstance()->readMailFromServer(m_info->uid, CC_ITOA(m_info->type));
}