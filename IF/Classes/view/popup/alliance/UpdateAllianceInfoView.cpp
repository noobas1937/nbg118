//
//  UpdateAllianceInfoView.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-9.
//
//

#include "UpdateAllianceInfoView.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "GuideController.h"
#include "PopupViewController.h"
#include "CreateAllianceView.h"
#include "JoinAllianceView.h"
#include "ChangeAllianceAttributesCommand.h"
#include "CheckAllianceNameCommand.h"
#include "CheckAllianceTagCommand.h"
#include "AllianceCheckView.h"
#include "AllianceManager.h"
#include "AllianceSetRankNameCommand.h"
#include "checkAllianceRestrictcommamd.h"

#define OPENAGAIN 20
const float nb_cell_width = 662;
UpdateAllianceInfoView *UpdateAllianceInfoView::create(int open){
    UpdateAllianceInfoView *ret = new UpdateAllianceInfoView();
    if(ret && ret->init(open)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void UpdateAllianceInfoView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("115032"));
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UpdateAllianceInfoView::updatePosition), MSG_ALLIACNE_FUN_POSITION, NULL);
}

void UpdateAllianceInfoView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIACNE_FUN_POSITION);
    PopupBaseView::onExit();
}
bool UpdateAllianceInfoView::init(int open){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto node = CCBLoadFile("UpdateAllianceInfoView", this, this);
        this->setContentSize(node->getContentSize());
        int preHeight = this->m_background->getContentSize().height;
        changeBGMaxHeight(m_background);
//        changeBGHeight(m_background);
        int dh = m_background->getContentSize().height - preHeight;
        if (CCCommonUtils::isIosAndroidPad()) {
            dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        this->m_funNode->setContentSize(CCSize(m_funNode->getContentSize().width, m_funNode->getContentSize().height + dh));
        m_funNode->setPositionY(m_funNode->getPositionY()-dh);
        
        m_scrollView = CCScrollView::create(m_funNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_funNode->addChild(m_scrollView);
        
        m_cell = CCArray::create();
        if (AllianceManager::getInstance()->checkRight(CHANGE_SLOGAN, GlobalData::shared()->playerInfo.allianceInfo.rank)) {
            AllianceChangeFunCell* cell = AllianceChangeFunCell::create(_lang("115046"), &GlobalData::shared()->playerInfo.allianceInfo,0);
            m_scrollView->addChild(cell);
            m_cell->addObject(cell);
        }
        if (AllianceManager::getInstance()->checkRight(OPEN_RECRUITMENT, GlobalData::shared()->playerInfo.allianceInfo.rank)) {
            AllianceChangeFunCell* cell2 = AllianceChangeFunCell::create(_lang("115047"), &GlobalData::shared()->playerInfo.allianceInfo,1);
            m_scrollView->addChild(cell2);
            m_cell->addObject(cell2);
        }
        /////TODO  开关控制联盟要求
//        if(GlobalData::shared()->alliance_fun_on3_k5)
//        {
//            if (AllianceManager::getInstance()->checkRight(RESTRICTION, GlobalData::shared()->playerInfo.allianceInfo.rank)) {
//                AllianceChangeFunCell* conneededcell = AllianceChangeFunCell::create(_lang("115508"), &GlobalData::shared()->playerInfo.allianceInfo,7);
//                m_scrollView->addChild(conneededcell);
//                m_cell->addObject(conneededcell);
//            }
//        }
        
        /////////
        if (AllianceManager::getInstance()->checkRight(CHANGE_NAME, GlobalData::shared()->playerInfo.allianceInfo.rank)) {
            AllianceChangeFunCell* cell3 = AllianceChangeFunCell::create(_lang("115048"), &GlobalData::shared()->playerInfo.allianceInfo,2);
            m_scrollView->addChild(cell3);
            m_cell->addObject(cell3);
        }
        if (AllianceManager::getInstance()->checkRight(CHANGE_BRIEF_NAME, GlobalData::shared()->playerInfo.allianceInfo.rank)) {
            AllianceChangeFunCell* cell4= AllianceChangeFunCell::create(_lang("115058"), &GlobalData::shared()->playerInfo.allianceInfo,3);
            m_scrollView->addChild(cell4);
            m_cell->addObject(cell4);
        }
        if (AllianceManager::getInstance()->checkRight(SHOW_ALLIANCE_LANGUAGE, GlobalData::shared()->playerInfo.allianceInfo.rank)) {
            AllianceChangeFunCell* cell5= AllianceChangeFunCell::create(_lang("115231"), &GlobalData::shared()->playerInfo.allianceInfo,4);
            m_scrollView->addChild(cell5);
            m_cell->addObject(cell5);
        }
        if (AllianceManager::getInstance()->checkRight(CHANGE_RANK_TITLE, GlobalData::shared()->playerInfo.allianceInfo.rank)) {
            AllianceChangeFunCell* cell6= AllianceChangeFunCell::create(_lang("115119"), &GlobalData::shared()->playerInfo.allianceInfo,5);
            m_scrollView->addChild(cell6);
            m_cell->addObject(cell6);
        }
        if (GlobalData::shared()->playerInfo.allianceInfo.rank >= 5) {
            AllianceChangeFunCell* cell7= AllianceChangeFunCell::create(_lang("115313"), &GlobalData::shared()->playerInfo.allianceInfo,6);
            m_scrollView->addChild(cell7);
            m_cell->addObject(cell7);
        }
        
        this->updatePosition(NULL);
        
        if(open != -1){
            int num = m_cell->count();
            for(int i=0; i<num; ++i){
                auto cell = (AllianceChangeFunCell*)m_cell->objectAtIndex(i);
                if(open == cell->m_index){
                    cell->open();
                    updatePosition(CCInteger::create(open));
                }
            }
            
        }

        ret = true;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch *touch, Event *event)
    {
        if (isTouchInside(m_nbTouchNodeForChangeFlag, touch)) {
            return true;
        }
        return false;
    };
    listener->onTouchMoved = [this](Touch *touch, Event *event){};
    listener->onTouchEnded = [this](Touch *touch, Event *event){
        if (isTouchInside(m_nbTouchNodeForChangeFlag, touch)) {
            CCCommonUtils::flyHint("", "", _lang("E100008"));
            
            return;
            
            
            
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    string lv = "Lv.";
    lv.append(CC_CMDITOAL(GlobalData::shared()->playerInfo.allianceInfo.level).c_str());
    m_lvTxt->setString(lv);

    
    
    m_nb_bg1->setColor({25,30,44});
    m_nb_bg2->setColor({38,44,70});
    
    
    return ret;
}

void UpdateAllianceInfoView::updatePosition(CCObject* data){
    int index = 0;
    if(data==NULL || (dynamic_cast<CCInteger*>(data)==NULL)){
        index = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(data);
        index = intObj->getValue();
    }
    int num = m_cell->count();
    int offy = 0;
    if(index==5 || index==6){
        offy = 600;
    }else if(index==4){
        offy = 500;
    }else if(index==OPENAGAIN){
        offy = 380;
    }else if(index!=-1){
        offy = 200;
    }
    int totalH = 110*num + offy;   //--sun 这个高度决定scrollView的高度
    int totalH2 = totalH;
    for (int i=0; i<num; i++) {
        AllianceChangeFunCell* cell = (AllianceChangeFunCell*)m_cell->objectAtIndex(i);
        cell->setPosition(ccp(0, totalH));
        if(cell->m_index!=index){
            if(index != OPENAGAIN){
                 cell->reset();
            }
        }
        totalH -= (cell->m_bg->getContentSize().height + 12);
    }
    m_scrollView->setContentSize(CCSize(m_funNode->getContentSize().width,totalH2));
    if (index==5 || index==6) {
        m_scrollView->setContentOffset(ccp(0, m_funNode->getContentSize().height - totalH2 + 520));
        m_scrollView->setTouchEnabled(false);
    }else if(index==OPENAGAIN){
        m_scrollView->setContentOffset(ccp(0, m_funNode->getContentSize().height - totalH2));
        m_scrollView->setTouchEnabled(false);
    }else if(index==4){
        m_scrollView->setContentOffset(ccp(0, m_funNode->getContentSize().height - totalH2 + 110*(num-2) - 110));
        m_scrollView->setTouchEnabled(false);
    }else if(index == -1){
        m_scrollView->setContentOffset(ccp(0, m_funNode->getContentSize().height - totalH2));
        m_scrollView->setTouchEnabled(true);
    }
    else
    {
        m_scrollView->setContentOffset(ccp(0, m_funNode->getContentSize().height - totalH2));
        m_scrollView->setTouchEnabled(false);

    }
}

bool UpdateAllianceInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nb_bg1", CCLayerColor*, this->m_nb_bg1);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nb_bg2", CCLayerColor*, this->m_nb_bg2);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvTxt", CCLabelIF*, this->m_lvTxt);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIcon", CCNode*, this->m_allianceIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nbTouchNodeForChangeFlag", CCNode*, this->m_nbTouchNodeForChangeFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funNode", CCNode*, this->m_funNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_background", CCScale9Sprite*, this->m_background);
    return false;
}

SEL_CCControlHandler UpdateAllianceInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickClose", UpdateAllianceInfoView::onClickClose);
    return NULL;
}

void UpdateAllianceInfoView::onClickClose(CCObject * pSender, Control::EventType pCCControlEvent) {
    this->closeSelf();
    //PopupViewController::getInstance()->addPopupInView(CreateAllianceView::create(11));
}

AllianceChangeFunCell *AllianceChangeFunCell::create(std::string title,AllianceInfo* info,int index){
    AllianceChangeFunCell *ret = new AllianceChangeFunCell(title,info,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceChangeFunCell::init(){
    CCBLoadFile("UpdateAllianceInfoCell", this, this);
    this->setContentSize(m_bg->getContentSize());
    this->m_nameTxt->setString(m_titleStr.c_str());
    CCCommonUtils::setButtonTitle(m_btnEdit, _lang("115034").c_str());
    CCCommonUtils::setButtonTitle(m_btnSave, _lang("115035").c_str());
    
    m_btnEdit->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);

    m_btnSave->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    
     m_modifyBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    
     m_modifyAbbrBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    
     m_rankBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);

 
    //announce
    m_announceTxt->setString(m_info->intro.c_str());
    m_touchTxt1->setString(_lang("115053"));
    m_touchTxt2->setString(_lang("115052"));
    m_tipResuitTxt->setString(_lang("115051"));

    //name
    std::string str =_lang("115065");
    m_nameTip1->setString(_lang("115065"));
    m_nameTip2->setString(_lang("115010"));
    str =_lang("115061");
    m_modifyNameTxt->setString(_lang("115061"));
    m_nameGoldTxt->setString(CC_ITOA(GlobalData::shared()->alliance_cost_k1));

    //abbr
    m_abbrTip1->setString(_lang("115059"));
    m_abbrTip2->setString(_lang("115060"));
    CCCommonUtils::setButtonTitle(m_modifyAbbrBtn, _lang("115061").c_str());
    
    //flag
    m_flagTitle1->setString(_lang("115117"));
    m_flagTitle2->setString(_lang("115118"));
    CCCommonUtils::setButtonTitle(m_modifyFlagBtn, _lang("115061").c_str());
    m_data = CCArray::create();

    m_tabView = CCMultiColTableView::create(this, m_flagList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup);
    m_flagList->addChild(m_tabView);
    m_tabView->reloadData();
    m_tabView->setTouchEnabled(false);
    
    
    
    std::string iconStr = "changeAnnounce.png";
    switch (m_index) {
        case 0:
            iconStr = "changeAnnounce.png";
            break;
        case 1:
            iconStr = "changeRecurit.png";
            break;
        case 2:
            iconStr = "changeAllianceName.png";
            break;
        case 3:
            iconStr = "modifyAbbr.png";
            break;
        case 4:{
            iconStr = "allianceLanguage.png";//language
            //lang
            this->scheduleOnce(schedule_selector(AllianceChangeFunCell::showTabView),0.0);
            m_allianceLangIntr->setString(_lang("115238").c_str());
        }
            break;
        case 5:
            iconStr = "allianceFlag.png";
            break;
        case 6:
            iconStr = "allianceFlag.png";
            break;
        case 7:
            iconStr = "Alliance_Restriction.png";//--sun 更换旗帜
            break;
    }
    CCSprite* spr = CCLoadSprite::createSprite(iconStr.c_str());
    m_itemcon->addChild(spr);
    CCCommonUtils::setSpriteMaxSize(spr, 80);
    
    m_btnSave->setEnabled(false);
    m_open = false;
    m_clickTime = 0;
    m_lastname = "";
    return true;
}
void AllianceChangeFunCell::showTabView(float t){
    LangList* langList = LangList::create(m_langList);
    m_langList->addChild(langList);
}
void AllianceChangeFunCell::cellEnlarge(){
    m_bg->setContentSize(CCSize(nb_cell_width,580));   //380
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_bg->setContentSize(CCSize(1500, 1190));
    }
    m_strictNode->setVisible(true);
    auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    m_castlelevel = CCEditBox::create(CCSizeMake(200,40),spriteText);
    if (CCCommonUtils::isIosAndroidPad())
    {
        spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
        m_castlelevel = CCEditBox::create(CCSizeMake(400,60),spriteText);
    }
    m_castlelevel->setMaxLength(2);
    m_castlelevel->setFontSize(26);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_castlelevel->setFontSize(40);
    }
    m_castlelevel->setFontColor(ccBLACK);
    m_castlelevel->setDelegate(this);
    m_castlelevel->setInputMode(kEditBoxInputModeNumeric);
    m_castlelevel->setReturnType(kKeyboardReturnTypeDone);
    m_castlelevel->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    std::string str = CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.castleRestriction);
    m_castlelevel->setText(str.c_str());
    m_castleLevelNode->addChild(m_castlelevel);
    auto tempText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    m_fightability = CCEditBox::create(CCSizeMake(200,40),tempText);
    if (CCCommonUtils::isIosAndroidPad())
    {
        tempText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
        m_fightability = CCEditBox::create(CCSizeMake(400,60),tempText);
    }
    m_fightability->setMaxLength(8);
    m_fightability->setFontSize(26);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_fightability->setFontSize(40);
    }
    m_fightability->setFontColor(ccBLACK);
    m_fightability->setDelegate(this);
    m_fightability->setInputMode(kEditBoxInputModeNumeric);
    m_fightability->setReturnType(kKeyboardReturnTypeDone);
    m_fightability->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    str = CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.powerRestriction);
    m_fightability->setText(str.c_str());
    m_FightLevelNode->addChild(m_fightability);
    m_castleText->setString(_lang("115509").c_str());
    m_FightText->setString(_lang("115510").c_str());
    m_ResTitle->setString(_lang("115508").c_str());
    CCSize CS = m_ResTitle->getContentSize();
    int longnum = m_ResTitle->getPositionX() - m_leftBar->getPositionX() - CS.width/2 - 10;
    CCSize csNew(longnum,m_leftBar->getContentSize().height);
    m_leftBar->setContentSize(csNew);
    int a =m_ResTitle->getPositionX();
    int b =m_rightBar->getPositionX();
    longnum = m_rightBar->getPositionX() - m_ResTitle->getPositionX() - CS.width/2 - 10;
    CCSize csNewright(longnum,m_leftBar->getContentSize().height);
    m_rightBar->setContentSize(csNewright);
    m_DesText->setString(_lang("115517"));  //--sun ziyuantihuan
    m_strictBtn->setEnabled(true);
    CCCommonUtils::setButtonTitle(m_strictBtn, _lang("115035").c_str());    //--sun 要换成保存的标签
    m_strictBtn->setEnabled(false);
    //checkAllianceRestrictChange(0);

}
void AllianceChangeFunCell::open(){
    m_open = true;
    m_arrow->setRotation(90);
    m_bg->setContentSize(CCSize(nb_cell_width,300));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_bg->setContentSize(CCSize(1500, 650));
    }
    switch (m_index) {
        case 0:
            m_funNode1->setVisible(true);
            break;
        case 1:
            m_funNode2->setVisible(true);
            if(m_info->recruit==1){
                m_touchBtn1->setPositionX(316);
                if (CCCommonUtils::isIosAndroidPad())
                {
                    m_touchBtn1->setPositionX(743);
                }
                m_touchTxt1->setColor({136,125,112});
                m_touchTxt2->setColor({255,216,124});
                m_touchTxt1->setVisible(false);
                m_touchTxt2->setVisible(true);
            }else{
                m_touchBtn1->setPositionX(189);
                if (CCCommonUtils::isIosAndroidPad())
                {
                    m_touchBtn1->setPositionX(530);
                }
                m_touchTxt1->setColor({255,216,124});
                m_touchTxt2->setColor({136,125,112});
                m_touchTxt1->setVisible(true);
                m_touchTxt2->setVisible(false);
                cellEnlarge();
                 this->unschedule(schedule_selector(AllianceChangeFunCell::checkAllianceRestrictChange));  //---1
                this->schedule(schedule_selector(AllianceChangeFunCell::checkAllianceRestrictChange));
            }
            break;
        case 2:
        {
            m_funNameNode->setVisible(true);
            auto sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
            m_nameAndAbbrEditBox = CCEditBox::create(CCSizeMake(250,40),sprite9);
            if (CCCommonUtils::isIosAndroidPad())
            {
                sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_nameAndAbbrEditBox = CCEditBox::create(CCSizeMake(700,80),sprite9);
            }
            m_nameAndAbbrEditBox->setMaxLength(20);
            m_nameAndAbbrEditBox->setFontSize(24);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_nameAndAbbrEditBox->setFontSize(48);
            }
            m_nameAndAbbrEditBox->setFontColor(ccBLACK);
            m_nameAndAbbrEditBox->setReturnType(kKeyboardReturnTypeDone);
            m_nameAndAbbrEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
            m_nameAndAbbrEditBox->setText("");
            m_inputNameNode->addChild(m_nameAndAbbrEditBox);
            m_modifyBtn->setEnabled(false);
            m_nameOKNode->getChildByTag(0)->setVisible(false);
            m_nameOKNode->getChildByTag(1)->setVisible(false);
            this->schedule(schedule_selector(AllianceChangeFunCell::checkAllianceNameAndAbbr));
        }
            break;
        case 3:
        {
            m_funAbbrNode->setVisible(true);
            auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
            m_nameAndAbbrEditBox = CCEditBox::create(CCSizeMake(300,40),spriteText);
            if (CCCommonUtils::isIosAndroidPad()) {
                spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_nameAndAbbrEditBox = CCEditBox::create(CCSizeMake(700,80),spriteText);
            }
            m_nameAndAbbrEditBox->setMaxLength(3);
            m_nameAndAbbrEditBox->setFontSize(24);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_nameAndAbbrEditBox->setFontSize(48);
            }
            m_nameAndAbbrEditBox->setFontColor(ccBLACK);
            m_nameAndAbbrEditBox->setReturnType(kKeyboardReturnTypeDone);
            m_nameAndAbbrEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
            m_nameAndAbbrEditBox->setText("");
            m_inputAbbrNode->addChild(m_nameAndAbbrEditBox);
            m_modifyBtn->setEnabled(false);
            if(m_info->abbrRename==0){
                CCCommonUtils::setButtonTitle(m_modifyAbbrBtn, _lang("115062").c_str());
                m_modifyAbbrTxt->setString("");
                m_abbrGoldTxt->setString("");
                m_brifGold->setVisible(false);
            }else{
                CCCommonUtils::setButtonTitle(m_modifyAbbrBtn, "");
                m_modifyAbbrTxt->setString(_lang("115061"));
                m_abbrGoldTxt->setString(CC_ITOA(GlobalData::shared()->alliance_cost_k2));
                m_brifGold->setVisible(true);
            }
            m_modifyAbbrBtn->setEnabled(false);
            m_abbrOKNode->getChildByTag(0)->setVisible(false);
            m_abbrOKNode->getChildByTag(1)->setVisible(false);
            this->schedule(schedule_selector(AllianceChangeFunCell::checkAllianceNameAndAbbr));
        }
            break;
        case 4:
        {
            m_funLangNode->setVisible(true);
            m_bg->setContentSize(CCSize(nb_cell_width,600));
            if (CCCommonUtils::isIosAndroidPad()) {
                m_bg->setContentSize(CCSize(1500, 1200));
            }
        }
            break;
        case 5:
        {
            m_funRankNode->setVisible(true);
            m_bg->setContentSize(CCSize(nb_cell_width,600));
            if (CCCommonUtils::isIosAndroidPad()) {
                m_bg->setContentSize(CCSize(1500, 950));
            }
            m_rankTip->setString(_lang("115122"));
            m_oldTitle->setString(_lang("115120"));
            m_newTitle->setString(_lang("115121"));
            CCCommonUtils::setButtonTitle(m_rankBtn, _lang("115061").c_str());
            string rankTitle = _lang("115104");
            if(GlobalData::shared()->playerInfo.allianceInfo.strRank5!=""){
                rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank5;
            }
            m_rank5Txt->setString(rankTitle.c_str());
            rankTitle = _lang("115103");
            if(GlobalData::shared()->playerInfo.allianceInfo.strRank4!=""){
                rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank4;
            }
            m_rank4Txt->setString(rankTitle.c_str());
            rankTitle = _lang("115102");
            if(GlobalData::shared()->playerInfo.allianceInfo.strRank3!=""){
                rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank3;
            }
            m_rank3Txt->setString(rankTitle.c_str());
            rankTitle = _lang("115101");
            if(GlobalData::shared()->playerInfo.allianceInfo.strRank2!=""){
                rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank2;
            }
            m_rank2Txt->setString(rankTitle.c_str());
            rankTitle = _lang("115100");
            if(GlobalData::shared()->playerInfo.allianceInfo.strRank1!=""){
                rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank1;
            }
            m_rank1Txt->setString(rankTitle.c_str());
            
            auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
            m_rank5EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
            if (CCCommonUtils::isIosAndroidPad())
            {
                spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_rank5EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
            }
            m_rank5EditBox->setMaxLength(10);
            m_rank5EditBox->setFontSize(24);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_rank5EditBox->setFontSize(40);
            }
            m_rank5EditBox->setFontColor(ccBLACK);
            m_rank5EditBox->setReturnType(kKeyboardReturnTypeDone);
            m_rank5EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
            m_rank5EditBox->setPlaceHolder("input text");
            m_inputRank5->addChild(m_rank5EditBox);
            
            spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
            m_rank4EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
            if (CCCommonUtils::isIosAndroidPad()) {
                spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_rank4EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
            }
            m_rank4EditBox->setMaxLength(10);
            m_rank4EditBox->setFontSize(24);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_rank4EditBox->setFontSize(40);
            }
            m_rank4EditBox->setFontColor(ccBLACK);
            m_rank4EditBox->setReturnType(kKeyboardReturnTypeDone);
            m_rank4EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
            m_rank4EditBox->setPlaceHolder("input text");
            m_inputRank4->addChild(m_rank4EditBox);
            
            spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
            m_rank3EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
            if (CCCommonUtils::isIosAndroidPad()) {
                spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_rank3EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
            }
            m_rank3EditBox->setMaxLength(10);
            m_rank3EditBox->setFontSize(24);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_rank3EditBox->setFontSize(40);
            }
            m_rank3EditBox->setFontColor(ccBLACK);
            m_rank3EditBox->setReturnType(kKeyboardReturnTypeDone);
            m_rank3EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
            m_rank3EditBox->setPlaceHolder("input text");
            m_inputRank3->addChild(m_rank3EditBox);
            
            spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
            m_rank2EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
            if (CCCommonUtils::isIosAndroidPad()) {
                spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_rank2EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
            }
            m_rank2EditBox->setMaxLength(10);
            m_rank2EditBox->setFontSize(24);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_rank2EditBox->setFontSize(40);
            }
            m_rank2EditBox->setFontColor(ccBLACK);
            m_rank2EditBox->setReturnType(kKeyboardReturnTypeDone);
            m_rank2EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
            m_rank2EditBox->setPlaceHolder("input text");
            m_inputRank2->addChild(m_rank2EditBox);
            
            spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
            m_rank1EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
            if (CCCommonUtils::isIosAndroidPad())
            {
                spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_rank1EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
            }
            m_rank1EditBox->setMaxLength(10);
            m_rank1EditBox->setFontSize(24);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_rank1EditBox->setFontSize(40);
            }
            m_rank1EditBox->setFontColor(ccBLACK);
            m_rank1EditBox->setReturnType(kKeyboardReturnTypeDone);
            m_rank1EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
            m_rank1EditBox->setPlaceHolder("input text");
            m_inputRank1->addChild(m_rank1EditBox);
            m_rankBtn->setEnabled(false);
            this->unschedule(schedule_selector(AllianceChangeFunCell::checkRankTitle));
            this->schedule(schedule_selector(AllianceChangeFunCell::checkRankTitle));
        }
            break;
        case 6:
        {
            m_funTNameNode->setVisible(true);
            m_lock1->setVisible(true);
            m_lock2->setVisible(true);
            m_lock3->setVisible(true);
            m_lock4->setVisible(true);
            m_lock5->setVisible(true);
            m_bg->setContentSize(CCSize(nb_cell_width,600));
            if (CCCommonUtils::isIosAndroidPad()) {
                m_bg->setContentSize(CCSize(1500, 930));
            }
            m_nameTip->setString(_lang("115122"));
            m_oldTNameTitle->setString(_lang("115120"));
            m_newTNameTitle->setString(_lang("115121"));
            CCCommonUtils::setButtonTitle(m_nameBtn, _lang("115061").c_str());//目前有五个，只开放一个，写死了5个，从1开始
            map<int, std::pair<std::string, std::string>>::iterator nameMapItr;
            nameMapItr = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap.begin();
            for (; nameMapItr != GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap.end(); nameMapItr++) {
                int count = nameMapItr->first;
                switch (count) {
                    case 1:
                        m_lock1->setVisible(false);
                        break;
                    case 2:
                        m_lock2->setVisible(false);
                        break;
                    case 3:
                        m_lock3->setVisible(false);
                        break;
                    case 4:
                        m_lock4->setVisible(false);
                        break;
                    case 5:
                        m_lock5->setVisible(false);
                        break;
                    default:
                        break;
                }
            }
            string nameTitle = _lang_1("115312", CC_ITOA(1));
            if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[1].second!=""){
                nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[1].second;
            }
            m_name1Txt->setString(nameTitle.c_str());
            nameTitle = _lang_1("115312", CC_ITOA(2));
            if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[2].second!=""){
                nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[2].second;
            }
            m_name2Txt->setString(nameTitle.c_str());
            m_lock2->setPositionX(m_name2Txt->getPositionX() +  m_name2Txt->getContentSize().width * m_name2Txt->getScaleX() * 0.5 + 25);
            nameTitle = _lang_1("115312", CC_ITOA(3));
            if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[3].second!=""){
                nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[3].second;
            }
            m_name3Txt->setString(nameTitle.c_str());
            m_lock3->setPositionX(m_name3Txt->getPositionX() +  m_name3Txt->getContentSize().width * m_name3Txt->getScaleX() * 0.5 + 25);
            nameTitle = _lang_1("115312", CC_ITOA(4));
            if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[4].second!=""){
                nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[4].second;
            }
            m_name4Txt->setString(nameTitle.c_str());
            m_lock4->setPositionX(m_name4Txt->getPositionX() +  m_name4Txt->getContentSize().width * m_name4Txt->getScaleX() * 0.5 + 25);
            nameTitle = _lang_1("115312", CC_ITOA(5));
            if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[5].second!=""){
                nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[5].second;
            }
            m_name5Txt->setString(nameTitle.c_str());
            m_lock5->setPositionX(m_name5Txt->getPositionX() +  m_name5Txt->getContentSize().width * m_name5Txt->getScaleX() * 0.5 + 25);
            
            if (m_lock1->isVisible()) {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                m_name1EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                    m_name1EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name1EditBox->setMaxLength(10);
                m_name1EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name1EditBox->setFontSize(40);
                }
                m_name1EditBox->setFontColor(ccBLACK);
                m_name1EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name1EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name1EditBox->setEnabled(false);
            }
            else {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_name1EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                    m_name1EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name1EditBox->setMaxLength(10);
                m_name1EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name1EditBox->setFontSize(40);
                }
                m_name1EditBox->setFontColor(ccBLACK);
                m_name1EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name1EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name1EditBox->setPlaceHolder("input text");
            }
            m_inputName1->addChild(m_name1EditBox);
            
            if (m_lock2->isVisible()) {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                m_name2EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                    m_name2EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name2EditBox->setMaxLength(10);
                m_name2EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name2EditBox->setFontSize(40);
                }
                m_name2EditBox->setFontColor(ccBLACK);
                m_name2EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name2EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name2EditBox->setEnabled(false);
            }
            else {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_name2EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                    m_name2EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name2EditBox->setMaxLength(10);
                m_name2EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name2EditBox->setFontSize(40);
                }
                m_name2EditBox->setFontColor(ccBLACK);
                m_name2EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name2EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name2EditBox->setPlaceHolder("input text");
            }
            m_inputName2->addChild(m_name2EditBox);
            
            if (m_lock3->isVisible()) {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                m_name3EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                    m_name3EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name3EditBox->setMaxLength(10);
                m_name3EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name3EditBox->setFontSize(40);
                }
                m_name3EditBox->setFontColor(ccBLACK);
                m_name3EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name3EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name3EditBox->setEnabled(false);
            }
            else {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_name3EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                    m_name3EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name3EditBox->setMaxLength(10);
                m_name3EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name3EditBox->setFontSize(40);
                }
                m_name3EditBox->setFontColor(ccBLACK);
                m_name3EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name3EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name3EditBox->setPlaceHolder("input text");
            }
            m_inputName3->addChild(m_name3EditBox);
            
            if (m_lock4->isVisible()) {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                m_name4EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                    m_name4EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name4EditBox->setMaxLength(10);
                m_name4EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name4EditBox->setFontSize(40);
                }
                m_name4EditBox->setFontColor(ccBLACK);
                m_name4EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name4EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name4EditBox->setEnabled(false);
            }
            else {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_name4EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                    m_name4EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name4EditBox->setMaxLength(10);
                m_name4EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name4EditBox->setFontSize(40);
                }
                m_name4EditBox->setFontColor(ccBLACK);
                m_name4EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name4EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name4EditBox->setPlaceHolder("input text");
            }
            m_inputName4->addChild(m_name4EditBox);
            
            if (m_lock5->isVisible()) {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                m_name5EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_text.png");
                    m_name5EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name5EditBox->setMaxLength(10);
                m_name5EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name5EditBox->setFontSize(40);
                }
                m_name5EditBox->setFontColor(ccBLACK);
                m_name5EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name5EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name5EditBox->setEnabled(false);
            }
            else {
                auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_name5EditBox = CCEditBox::create(CCSizeMake(200,40),spriteText);
                if (CCCommonUtils::isIosAndroidPad()) {
                    spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                    m_name5EditBox = CCEditBox::create(CCSizeMake(400,60),spriteText);
                }
                m_name5EditBox->setMaxLength(10);
                m_name5EditBox->setFontSize(24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    m_name5EditBox->setFontSize(40);
                }
                m_name5EditBox->setFontColor(ccBLACK);
                m_name5EditBox->setReturnType(kKeyboardReturnTypeDone);
                m_name5EditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
                m_name5EditBox->setPlaceHolder("input text");
            }
            m_inputName5->addChild(m_name5EditBox);
            
            m_nameBtn->setEnabled(false);
            this->unschedule(schedule_selector(AllianceChangeFunCell::checkTerritoryTitle));
            this->schedule(schedule_selector(AllianceChangeFunCell::checkTerritoryTitle));
        }
            break;
        case 7:
        {
            //--sun TODO 获取限制要求信息
            m_bg->setContentSize(CCSize(nb_cell_width,380));
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_bg->setContentSize(CCSize(1500, 1190));
            }
            m_strictNode->setVisible(true);
            auto spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
            m_castlelevel = CCEditBox::create(CCSizeMake(200,40),spriteText);
            if (CCCommonUtils::isIosAndroidPad())
            {
                spriteText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_castlelevel = CCEditBox::create(CCSizeMake(400,60),spriteText);
            }
            m_castlelevel->setMaxLength(2);
            m_castlelevel->setFontSize(26);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_castlelevel->setFontSize(40);
            }
            m_castlelevel->setFontColor(ccBLACK);
            m_castlelevel->setReturnType(kKeyboardReturnTypeDone);
            m_castlelevel->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
            std::string str = CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.castleRestriction);
            m_castlelevel->setText(str.c_str());
            m_castleLevelNode->addChild(m_castlelevel);
            auto tempText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
            m_fightability = CCEditBox::create(CCSizeMake(200,40),tempText);
            if (CCCommonUtils::isIosAndroidPad())
            {
                tempText = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
                m_fightability = CCEditBox::create(CCSizeMake(400,60),tempText);
            }
            m_fightability->setMaxLength(8);
            m_fightability->setFontSize(26);
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_fightability->setFontSize(40);
            }
            m_fightability->setFontColor(ccBLACK);
            m_fightability->setReturnType(kKeyboardReturnTypeDone);
            m_fightability->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
            str = CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.powerRestriction);
            m_fightability->setText(str.c_str());
            m_FightLevelNode->addChild(m_fightability);
            m_castleText->setString(_lang("115509").c_str());
            m_FightText->setString(_lang("115510").c_str());
            m_DesText->setString(_lang("115517"));  //--sun ziyuantihuan
            if(m_info->recruit == 1){
                m_strictBtn->setEnabled(true);
            }
            else{
                m_strictBtn->setEnabled(false);
            }
            CCCommonUtils::setButtonTitle(m_strictBtn, _lang("115035").c_str());    //--sun 要换成保存的标签
            //this->schedule(schedule_selector(AllianceChangeFunCell::checkAllianceRestrict));
            
        }
            break;
        default:
            break;
    }
    
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_FUN_POSITION,CCInteger::create(m_index));
}

void AllianceChangeFunCell::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void AllianceChangeFunCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

void AllianceChangeFunCell::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize AllianceChangeFunCell::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(120, 120);
}

CCTableViewCell* AllianceChangeFunCell::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    AllianceFlagCell* cell = (AllianceFlagCell*)table->dequeueGrid();
    CCString* info = (CCString*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info->getCString());
        }else{
            cell = AllianceFlagCell::create(info->getCString());
        }
    }
    return cell;
}

ssize_t AllianceChangeFunCell::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_data->count();
}

unsigned int AllianceChangeFunCell::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 4;
}

bool AllianceChangeFunCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_moveFlag = false;
    if(isTouchInside(m_bg, pTouch)){
        m_touchPoint = pTouch->getLocation();
        unsigned long currentTime = clock()/1000;
        int gap = currentTime - m_clickTime;
        if(m_index==1 && isTouchInside(m_touchBtnBg, pTouch) && gap<500){
            return false;
        }
        m_clickTime = currentTime;
        return true;
    }
    return false;
}

void AllianceChangeFunCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if(m_open && m_index==1 && isTouchInside(m_touchBtnBg, pTouch) && fabs(pTouch->getStartLocation().x-pTouch->getLocation().x)>30){
        CCPoint move = pTouch->getLocation();
        float gap = move.x - m_touchPoint.x;
        float moveX = m_touchBtn1->getPositionX() + gap;
        if(moveX>316){
            m_touchBtn1->setPositionX(316);
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_touchBtn1->setPositionX(743);
            }
        }else if(moveX<189){
            m_touchBtn1->setPositionX(189);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_touchBtn1->setPositionX(530);
            }
        }else{
            m_touchBtn1->setPositionX(moveX);
        }
        m_moveFlag = true;
    }
}

void AllianceChangeFunCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(fabs(m_touchPoint.y - pTouch->getLocation().y) > 30 && !m_moveFlag){
        return ;
    }
    if(isTouchInside(m_bg2, pTouch) && m_open){
        this->reset();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_FUN_POSITION);
        return ;
    }
    if(isTouchInside(m_bg, pTouch) && !m_open){
        open();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_FUN_POSITION,CCInteger::create(m_index));
    }
    if(m_moveFlag){
        CCPoint move = pTouch->getLocation();
        float gap = move.x - m_touchPoint.x;
        m_touchBtn1->setPositionX(m_touchBtn1->getPositionX() + gap);
        if (CCCommonUtils::isIosAndroidPad()) {
            if(m_touchBtn1->getPositionX()>637){
                //m_touchBtn1->runAction(CCMoveTo::create(0.3, CCPoint(316,-200)));
                m_touchBtn1->setPositionX(743);
                m_touchTxt1->setColor({136,125,112});
                m_touchTxt2->setColor({255,216,124});
                m_touchTxt1->setVisible(false);
                m_touchTxt2->setVisible(true);
                ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
                command->putParam("recruit", CCInteger::create(1));
                m_currAttr = 1;
                command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::attrSuccess), NULL));
                command->sendAndRelease();
            }else if(m_touchBtn1->getPositionX()<=637){
                m_touchBtn1->setPositionX(530);
                m_touchTxt1->setColor({255,216,124});
                m_touchTxt2->setColor({136,125,112});
                m_touchTxt1->setVisible(true);
                m_touchTxt2->setVisible(false);
                //m_touchBtn1->runAction(CCMoveTo::create(0.3, CCPoint(189,-200)));
                ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
                command->putParam("recruit", CCInteger::create(0));
                m_currAttr = 0;
                command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::attrSuccess), NULL));
                command->sendAndRelease();
            }
        } else {
            if(m_touchBtn1->getPositionX()>268){
                //m_touchBtn1->runAction(CCMoveTo::create(0.3, CCPoint(316,-200)));
                m_touchBtn1->setPositionX(316);
                m_touchTxt1->setColor({136,125,112});
                m_touchTxt2->setColor({255,216,124});
                m_touchTxt1->setVisible(false);
                m_touchTxt2->setVisible(true);
                ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
                command->putParam("recruit", CCInteger::create(1));
                m_currAttr = 1;
                command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::attrSuccess), NULL));
                command->sendAndRelease();
            }else if(m_touchBtn1->getPositionX()<=268){
                m_touchBtn1->setPositionX(189);
                m_touchTxt1->setColor({255,216,124});
                m_touchTxt2->setColor({136,125,112});
                m_touchTxt1->setVisible(true);
                m_touchTxt2->setVisible(false);
                //m_touchBtn1->runAction(CCMoveTo::create(0.3, CCPoint(189,-200)));
                ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
                command->putParam("recruit", CCInteger::create(0));
                m_currAttr = 0;
                command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::attrSuccess), NULL));
                command->sendAndRelease();
            }
        }
        if(m_currAttr == 0){
            cellEnlarge();
            this->unschedule(schedule_selector(AllianceChangeFunCell::checkAllianceRestrictChange));//---
            this->schedule(schedule_selector(AllianceChangeFunCell::checkAllianceRestrictChange));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_FUN_POSITION,CCInteger::create(OPENAGAIN));
        }
        else{
            m_bg->setContentSize(CCSize(nb_cell_width,300));   //380
            if (CCCommonUtils::isIosAndroidPad()) {
                m_bg->setContentSize(CCSize(1500, 650));
            }
            m_strictNode->setVisible(false);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_FUN_POSITION,CCInteger::create(OPENAGAIN));
        }
        m_moveFlag = false;
        return;
    }
    
    if(isTouchInside(m_touchBtnBg, pTouch) && !isTouchInside(m_touchBtn1, pTouch)){
        if (CCCommonUtils::isIosAndroidPad()) {
            if(m_touchBtn1->getPositionX()<637){
                //m_touchBtn1->runAction(CCMoveTo::create(0.3, CCPoint(316,-200)));
                m_touchBtn1->setPositionX(743);
                m_touchTxt1->setColor({136,125,112});
                m_touchTxt2->setColor({255,216,124});
                m_touchTxt1->setVisible(false);
                m_touchTxt2->setVisible(true);
                ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
                command->putParam("recruit", CCInteger::create(1));
                m_currAttr = 1;
                command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::attrSuccess), NULL));
                command->sendAndRelease();
            }else if(m_touchBtn1->getPositionX()>=637){
                m_touchBtn1->setPositionX(530);
                m_touchTxt1->setColor({255,216,124});
                m_touchTxt2->setColor({136,125,112});
                m_touchTxt1->setVisible(true);
                m_touchTxt2->setVisible(false);
                //m_touchBtn1->runAction(CCMoveTo::create(0.3, CCPoint(189,-200)));
                ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
                command->putParam("recruit", CCInteger::create(0));
                m_currAttr = 0;
                command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::attrSuccess), NULL));
                command->sendAndRelease();
            }
        } else {
            if(m_touchBtn1->getPositionX()<268){
                //m_touchBtn1->runAction(CCMoveTo::create(0.3, CCPoint(316,-200)));
                m_touchBtn1->setPositionX(316);
                m_touchTxt1->setColor({136,125,112});
                m_touchTxt2->setColor({255,216,124});
                m_touchTxt1->setVisible(false);
                m_touchTxt2->setVisible(true);
                ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
                command->putParam("recruit", CCInteger::create(1));
                m_currAttr = 1;
                command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::attrSuccess), NULL));
                command->sendAndRelease();
            }else if(m_touchBtn1->getPositionX()>=268){
                m_touchBtn1->setPositionX(189);
                m_touchTxt1->setColor({255,216,124});
                m_touchTxt2->setColor({136,125,112});
                m_touchTxt1->setVisible(true);
                m_touchTxt2->setVisible(false);
                //m_touchBtn1->runAction(CCMoveTo::create(0.3, CCPoint(189,-200)));
                ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
                command->putParam("recruit", CCInteger::create(0));
                m_currAttr = 0;
                command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::attrSuccess), NULL));
                command->sendAndRelease();
            }
        }
        if(m_currAttr== 0){
            cellEnlarge();
            this->unschedule(schedule_selector(AllianceChangeFunCell::checkAllianceRestrictChange));//----
            this->schedule(schedule_selector(AllianceChangeFunCell::checkAllianceRestrictChange));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_FUN_POSITION,CCInteger::create(OPENAGAIN));
        }
        else{
            m_bg->setContentSize(CCSize(nb_cell_width,300));   //380
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_bg->setContentSize(CCSize(1500, 650));
            }
            m_strictNode->setVisible(false);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_FUN_POSITION,CCInteger::create(OPENAGAIN));
        }
        m_moveFlag = false;
        return;
    }
    
    m_moveFlag = false;
}

void AllianceChangeFunCell::reset(){
    m_arrow->setRotation(0);
    m_open = false;
    m_bg->setContentSize(CCSize(nb_cell_width,96));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_bg->setContentSize(CCSize(1500, 160));
    }
    m_funNode1->setVisible(false);
    m_funNode2->setVisible(false);
    m_funNameNode->setVisible(false);
    m_funAbbrNode->setVisible(false);
    m_strictNode->setVisible(false);
    m_funFlagNode->setVisible(false);
    m_funLangNode->setVisible(false);
    m_funRankNode->setVisible(false);
    m_funNameNode->setVisible(false);
    m_funTNameNode->setVisible(false);
    m_strictNode->setVisible(false);
    m_lastname = "";
    if(m_nameAndAbbrEditBox){
        m_nameAndAbbrEditBox->setEnabled(false);
        m_nameAndAbbrEditBox->removeFromParentAndCleanup(true);
    }
    if(m_rank5EditBox){
        m_rank5EditBox->setEnabled(false);
        m_rank5EditBox->removeFromParentAndCleanup(true);
    }
    if(m_rank4EditBox){
        m_rank4EditBox->setEnabled(false);
        m_rank4EditBox->removeFromParentAndCleanup(true);
    }
    if(m_rank3EditBox){
        m_rank3EditBox->setEnabled(false);
        m_rank3EditBox->removeFromParentAndCleanup(true);
    }
    if(m_rank2EditBox){
        m_rank2EditBox->setEnabled(false);
        m_rank2EditBox->removeFromParentAndCleanup(true);
    }
    if(m_rank1EditBox){
        m_rank1EditBox->setEnabled(false);
        m_rank1EditBox->removeFromParentAndCleanup(true);
    }
    if(m_name5EditBox){
        m_name5EditBox->setEnabled(false);
        m_name5EditBox->removeFromParentAndCleanup(true);
    }
    if(m_name4EditBox){
        m_name4EditBox->setEnabled(false);
        m_name4EditBox->removeFromParentAndCleanup(true);
    }
    if(m_name3EditBox){
        m_name3EditBox->setEnabled(false);
        m_name3EditBox->removeFromParentAndCleanup(true);
    }
    if(m_name2EditBox){
        m_name2EditBox->setEnabled(false);
        m_name2EditBox->removeFromParentAndCleanup(true);
    }
    if(m_name1EditBox){
        m_name1EditBox->setEnabled(false);
        m_name1EditBox->removeFromParentAndCleanup(true);
    }
    m_inputAbbrNode->removeAllChildrenWithCleanup(true);
    m_castleLevelNode->removeAllChildrenWithCleanup(true);
    m_FightLevelNode->removeAllChildrenWithCleanup(true);
    m_inputNameNode->removeAllChildrenWithCleanup(true);
    this->unschedule(schedule_selector(AllianceChangeFunCell::checkAllianceNameAndAbbr));
    this->unschedule(schedule_selector(AllianceChangeFunCell::checkRankTitle));
    this->unschedule(schedule_selector(AllianceChangeFunCell::checkTerritoryTitle));//---
   
}



void AllianceChangeFunCell::attrSuccess(CCObject* p){
    m_info->recruit = m_currAttr;
    GlobalData::shared()->playerInfo.allianceInfo.recruit = m_currAttr;
    
}

void AllianceChangeFunCell::clickHandle(CCObject *pSender, CCControlEvent event){
    
}

void AllianceChangeFunCell::onSaveClick(CCObject * pSender, Control::EventType pCCControlEvent){
    m_btnEdit->setEnabled(true);
    m_btnSave->setEnabled(false);
    m_announceEditBox->setEnabled(false);
    m_announceTxt->setVisible(true);
    m_announceTxt->setString(m_announceEditBox->getText());
    ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
    command->putParam("intro", CCString::create(m_announceEditBox->getText()));
    command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::saveSuccess), NULL));
    command->sendAndRelease();
    m_announceNode->removeAllChildrenWithCleanup(true);
    m_announceEditBox = NULL;
}

void AllianceChangeFunCell::saveSuccess(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if (result==NULL || result->getData()==NULL) {
        return;
    }
    auto dic = _dict(result->getData());
    if(dic){
        string tempIntro = dic->valueForKey("intro")->getCString();
        GlobalData::shared()->playerInfo.allianceInfo.intro = tempIntro;
    }
}

void AllianceChangeFunCell::onEditClick(CCObject * pSender, Control::EventType pCCControlEvent){
    auto sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    m_announceEditBox = CCEditBox::create(CCSizeMake(578.0,40),sprite9);
    if (CCCommonUtils::isIosAndroidPad())
    {
        sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
        m_announceEditBox = CCEditBox::create(CCSizeMake(1500,60),sprite9);
    }
    m_announceEditBox->setMaxLength(199);
    m_announceEditBox->setFontSize(20);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_announceEditBox->setFontSize(40);
    }
    m_announceEditBox->setFontColor(ccBLACK);
    m_announceEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_announceEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_announceEditBox->setText(GlobalData::shared()->playerInfo.allianceInfo.intro.c_str());//
    m_announceNode->addChild(m_announceEditBox);
    m_btnEdit->setEnabled(false);
    m_btnSave->setEnabled(true);
    m_announceTxt->setVisible(false);
}

void AllianceChangeFunCell::onNameModifyClick(CCObject * pSender, Control::EventType pCCControlEvent){
    ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
    command->putParam("name", CCString::create(m_nameAndAbbrEditBox->getText()));
    command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::saveNameSuccess), NULL));
    command->sendAndRelease();
}

void AllianceChangeFunCell::saveNameSuccess(CCObject* p){
    m_modifyBtn->setEnabled(false);
    m_info->name = m_nameAndAbbrEditBox->getText();
    CCCommonUtils::flyHint("","",_lang("115084"));
    m_nameAndAbbrEditBox->setText("");
    m_nameOKNode->getChildByTag(0)->setVisible(false);
    m_nameOKNode->getChildByTag(1)->setVisible(false);
    m_nameOKNode->setVisible(false);
}

void AllianceChangeFunCell::onModifyAbbrClick(CCObject * pSender, Control::EventType pCCControlEvent){
    std::string changeAbbr = m_nameAndAbbrEditBox->getText();
    if(changeAbbr=="" || changeAbbr==m_info->shortName) return ;
    ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
    command->putParam("abbr", CCString::create(m_nameAndAbbrEditBox->getText()));
    command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::modifyAbbrSuccess), NULL));
    command->sendAndRelease();
}

void AllianceChangeFunCell::onStrictBtnclick(CCObject * pSender, CCControlEvent pCCControlEvent){
     CommandAllianceRestrict(0);
}

void AllianceChangeFunCell::strictSuccess(CCObject* p){
    CCCommonUtils::flyHint("","",_lang("115520"));  //--sun resource replace
    m_fightability->setText(settingFight.c_str());
    m_FightLevelNode->removeAllChildren();
    m_FightLevelNode->addChild(m_fightability);
    m_castlelevel->setText(settingcastle.c_str());
    m_castleLevelNode->removeAllChildren();
    m_castleLevelNode->addChild(m_castlelevel);
}

bool AllianceChangeFunCell::isNumStr(string str){
    if(str.length() == 0){
        return false;
    }
    for(int i = 0;i < str.length();i++)
    {
        if(str[i]<'0' || str[i]>'9')
            return false;
    }
    return true;
}

void AllianceChangeFunCell::modifyRankClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if (m_index==5) {
        string strRank5 = m_rank5EditBox->getText();
        string strRank4 = m_rank4EditBox->getText();
        string strRank3 = m_rank3EditBox->getText();
        string strRank2 = m_rank2EditBox->getText();
        string strRank1 = m_rank1EditBox->getText();
        if(strRank1=="" && strRank2=="" && strRank3=="" && strRank4=="" && strRank5==""){
            return ;
        }
        //al.rankname.set>"rankOne" "rankTwo" "rankThree" "rankFour" "rankFive" 参数可以不全设置
        AllianceSetRankNameCommand* cmd = new AllianceSetRankNameCommand();
        cmd->putParam("rankOne", CCString::create(strRank1));
        cmd->putParam("rankTwo", CCString::create(strRank2));
        cmd->putParam("rankThree", CCString::create(strRank3));
        cmd->putParam("rankFour", CCString::create(strRank4));
        cmd->putParam("rankFive", CCString::create(strRank5));
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::modifyRankTitleCallBack), NULL));
        cmd->sendAndRelease();
        m_rankBtn->setEnabled(false);
        this->unschedule(schedule_selector(AllianceChangeFunCell::checkRankTitle));
    }
}

void AllianceChangeFunCell::modifyTNameClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if (m_index==6) {
        string strName1 = m_name1EditBox->getText();
        string strName2 = m_name2EditBox->getText();
        string strName3 = m_name3EditBox->getText();
        string strName4 = m_name4EditBox->getText();
        string strName5 = m_name5EditBox->getText();
        if(strName1=="" && strName2=="" && strName3=="" && strName4=="" && strName5==""){
            return ;
        }
        //al.rankname.set>"rankOne" "rankTwo" "rankThree" "rankFour" "rankFive" 参数可以不全设置
        AllianceSetTerritoryNameCommand* cmd = new AllianceSetTerritoryNameCommand();
        cmd->putParam("nameOne", CCString::create(strName1));
        cmd->putParam("nameTwo", CCString::create(strName2));
        cmd->putParam("nameThree", CCString::create(strName3));
        cmd->putParam("nameFour", CCString::create(strName4));
        cmd->putParam("nameFive", CCString::create(strName5));
        cmd->putParam("uidOne", CCString::create(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[1].first));
        cmd->putParam("uidTwo", CCString::create(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[2].first));
        cmd->putParam("uidThree", CCString::create(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[3].first));
        cmd->putParam("uidFour", CCString::create(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[4].first));
        cmd->putParam("uidFive", CCString::create(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[5].first));
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::modifyTerritoryTitleCallBack), NULL));
        cmd->sendAndRelease();
        m_nameBtn->setEnabled(false);
        this->unschedule(schedule_selector(AllianceChangeFunCell::checkTerritoryTitle));
    }
}

void AllianceChangeFunCell::checkRankTitle(float t){
    string strRank5 = m_rank5EditBox->getText();
    string strRank4 = m_rank4EditBox->getText();
    string strRank3 = m_rank3EditBox->getText();
    string strRank2 = m_rank2EditBox->getText();
    string strRank1 = m_rank1EditBox->getText();
    m_rankBtn->setEnabled(false);
    bool flag = true;
    m_check1->setVisible(true);
    if(strRank1==""){
        m_check1->setVisible(false);
    }else if(strRank1!="" && strRank1.length()>=3 && strRank1.length()<=10){
        m_check1->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_check1->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    
    m_check2->setVisible(true);
    if(strRank2==""){
        m_check2->setVisible(false);
    }else if(strRank2!="" && strRank2.length()>=3 && strRank2.length()<=10){
        m_check2->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_check2->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    
    m_check3->setVisible(true);
    if(strRank3==""){
        m_check3->setVisible(false);
    }else if(strRank3!="" && strRank3.length()>=3 && strRank3.length()<=10){
        m_check3->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_check3->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    
    m_check4->setVisible(true);
    if(strRank4==""){
        m_check4->setVisible(false);
    }else if(strRank4!="" && strRank4.length()>=3 && strRank4.length()<=10){
        m_check4->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_check4->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    
    m_check5->setVisible(true);
    if(strRank5==""){
        m_check5->setVisible(false);
    }else if(strRank5!="" && strRank5.length()>=3 && strRank5.length()<=10){
        m_check5->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_check5->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    if(strRank1=="" && strRank2=="" && strRank3=="" && strRank4=="" && strRank5==""){
        return ;
    }
    m_rankBtn->setEnabled(flag);
}
void AllianceChangeFunCell::checkTerritoryTitle(float t){
    string strName1 = m_name1EditBox->getText();
    string strName2 = m_name2EditBox->getText();
    string strName3 = m_name3EditBox->getText();
    string strName4 = m_name4EditBox->getText();
    string strName5 = m_name5EditBox->getText();
    m_nameBtn->setEnabled(false);
    bool flag = true;
    m_tcheck1->setVisible(true);
    if(strName1==""){
        m_tcheck1->setVisible(false);
    }else if(strName1!="" && strName1.length()>=3 && strName1.length()<=10){
        m_tcheck1->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_tcheck1->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    
    m_tcheck2->setVisible(true);
    if(strName2==""){
        m_tcheck2->setVisible(false);
    }else if(strName2!="" && strName2.length()>=3 && strName2.length()<=10){
        m_tcheck2->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_tcheck2->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    
    m_tcheck3->setVisible(true);
    if(strName3==""){
        m_tcheck3->setVisible(false);
    }else if(strName3!="" && strName3.length()>=3 && strName3.length()<=10){
        m_tcheck3->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_tcheck3->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    
    m_tcheck4->setVisible(true);
    if(strName4==""){
        m_tcheck4->setVisible(false);
    }else if(strName4!="" && strName4.length()>=3 && strName4.length()<=10){
        m_tcheck4->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_tcheck4->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    
    m_tcheck5->setVisible(true);
    if(strName5==""){
        m_tcheck5->setVisible(false);
    }else if(strName5!="" && strName5.length()>=3 && strName5.length()<=10){
        m_tcheck5->initWithSpriteFrame(CCLoadSprite::loadResource("green_yes.png"));
    }else{
        m_tcheck5->initWithSpriteFrame(CCLoadSprite::loadResource("red_no.png"));
        flag = false;
    }
    if(strName1=="" && strName2=="" && strName3=="" && strName4=="" && strName5==""){
        return ;
    }
    m_nameBtn->setEnabled(flag);
}

void AllianceChangeFunCell::modifyRankTitleCallBack(CCObject* param){
    m_rankBtn->setEnabled(true);
    this->unschedule(schedule_selector(AllianceChangeFunCell::checkRankTitle));
    this->schedule(schedule_selector(AllianceChangeFunCell::checkRankTitle));
    NetResult* net = dynamic_cast<NetResult*>(param);
    if(net==NULL) return ;
    CCDictionary* dic = dynamic_cast<CCDictionary*>(net->getData());
    if(dic==NULL) return ;
    const CCString *pStr = dic->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText(pStr->getCString());
        return ;
    }
    CCCommonUtils::flyHint("", "", _lang("115123"));
    string strRank5 = m_rank5EditBox->getText();
    string strRank4 = m_rank4EditBox->getText();
    string strRank3 = m_rank3EditBox->getText();
    string strRank2 = m_rank2EditBox->getText();
    string strRank1 = m_rank1EditBox->getText();
    if(strRank1!=""){
       GlobalData::shared()->playerInfo.allianceInfo.strRank1 = strRank1;
    }
    if(strRank2!=""){
        GlobalData::shared()->playerInfo.allianceInfo.strRank2 = strRank2;
    }
    if(strRank3!=""){
        GlobalData::shared()->playerInfo.allianceInfo.strRank3 = strRank3;
    }
    if(strRank4!=""){
        GlobalData::shared()->playerInfo.allianceInfo.strRank4 = strRank4;
    }
    if(strRank5!=""){
        GlobalData::shared()->playerInfo.allianceInfo.strRank5 = strRank5;
    }
    m_rank1EditBox->setText("");
    m_rank2EditBox->setText("");
    m_rank3EditBox->setText("");
    m_rank4EditBox->setText("");
    m_rank5EditBox->setText("");
    
    string rankTitle = _lang("115104");
    if(GlobalData::shared()->playerInfo.allianceInfo.strRank5!=""){
        rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank5;
    }
    m_rank5Txt->setString(rankTitle.c_str());
    rankTitle = _lang("115103");
    if(GlobalData::shared()->playerInfo.allianceInfo.strRank4!=""){
        rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank4;
    }
    m_rank4Txt->setString(rankTitle.c_str());
    rankTitle = _lang("115102");
    if(GlobalData::shared()->playerInfo.allianceInfo.strRank3!=""){
        rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank3;
    }
    m_rank3Txt->setString(rankTitle.c_str());
    rankTitle = _lang("115101");
    if(GlobalData::shared()->playerInfo.allianceInfo.strRank2!=""){
        rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank2;
    }
    m_rank2Txt->setString(rankTitle.c_str());
    rankTitle = _lang("115100");
    if(GlobalData::shared()->playerInfo.allianceInfo.strRank1!=""){
        rankTitle = GlobalData::shared()->playerInfo.allianceInfo.strRank1;
    }
    m_rank1Txt->setString(rankTitle.c_str());
}
void AllianceChangeFunCell::modifyTerritoryTitleCallBack(CCObject* param){
    m_nameBtn->setEnabled(true);
    this->unschedule(schedule_selector(AllianceChangeFunCell::checkTerritoryTitle));
    this->schedule(schedule_selector(AllianceChangeFunCell::checkTerritoryTitle));
    NetResult* net = dynamic_cast<NetResult*>(param);
    if(net==NULL) return ;
    CCDictionary* dic = dynamic_cast<CCDictionary*>(net->getData());
    if(dic==NULL) return ;
    const CCString *pStr = dic->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText(pStr->getCString());
        return ;
    }
    CCCommonUtils::flyHint("", "", _lang("115352"));///////
    string strName1 = m_name1EditBox->getText();
    string strName2 = m_name2EditBox->getText();
    string strName3 = m_name3EditBox->getText();
    string strName4 = m_name4EditBox->getText();
    string strName5 = m_name5EditBox->getText();
    if(strName1!=""){
        GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[1].second = strName1;
    }
    if(strName2!=""){
        GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[2].second = strName2;
    }
    if(strName3!=""){
        GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[3].second = strName3;
    }
    if(strName4!=""){
        GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[4].second = strName4;
    }
    if(strName5!=""){
        GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[5].second = strName5;
    }
    m_name1EditBox->setText("");
    m_name2EditBox->setText("");
    m_name3EditBox->setText("");
    m_name4EditBox->setText("");
    m_name5EditBox->setText("");
    
    string nameTitle = _lang_1("115312", "1");
    if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[1].second!=""){
        nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[1].second;
    }
    m_name1Txt->setString(nameTitle.c_str());
    nameTitle = _lang_1("115312", "2");
    if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[2].second!=""){
        nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[2].second;
    }
    m_name2Txt->setString(nameTitle.c_str());
    nameTitle = _lang_1("115312", "3");
    if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[3].second!=""){
        nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[3].second;
    }
    m_name3Txt->setString(nameTitle.c_str());
    nameTitle = _lang_1("115312", "4");
    if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[4].second!=""){
        nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[4].second;
    }
    m_name4Txt->setString(nameTitle.c_str());
    nameTitle = _lang_1("115312", "5");
    if(GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[5].second!=""){
        nameTitle = GlobalData::shared()->playerInfo.allianceInfo.territoryNameMap[5].second;
    }
    m_name5Txt->setString(nameTitle.c_str());
}

void AllianceChangeFunCell::modifyAbbrSuccess(CCObject* p){
    CCCommonUtils::flyHint("","",_lang("115064"));
    CCCommonUtils::setButtonTitle(m_modifyAbbrBtn, "");
    m_info->shortName = m_nameAndAbbrEditBox->getText();
    m_modifyAbbrTxt->setString(_lang("115061"));
    m_abbrGoldTxt->setString(CC_ITOA(GlobalData::shared()->alliance_cost_k2));
    m_brifGold->setVisible(true);
    m_info->abbrRename = 1;
    m_nameAndAbbrEditBox->setText("");
    m_abbrOKNode->getChildByTag(0)->setVisible(false);
    m_abbrOKNode->getChildByTag(1)->setVisible(false);
    m_abbrOKNode->setVisible(false);
}



void AllianceChangeFunCell::CommandAllianceRestrict(float t){
    checkAllianceRestrictcommamd* command = new checkAllianceRestrictcommamd();
    command->putParam("castleRestriction", CCString::create(settingcastle));
    command->putParam("powerRestriction", CCString::create(settingFight));
    command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::onRestrictSuccess), NULL));
    command->sendAndRelease();
}
void AllianceChangeFunCell::checkAllianceNameAndAbbr(float t){
    if(!m_nameAndAbbrEditBox || !m_nameAndAbbrEditBox->getParent()) return ;
    std::string changeName = m_nameAndAbbrEditBox->getText();
    bool ok = false;
    if(m_index==2){
        m_nameOKNode->setVisible(true);
//        m_nameOKNode->getChildByTag(0)->setVisible(false);
//        m_nameOKNode->getChildByTag(1)->setVisible(false);
        if(changeName=="" || changeName.length()<3 || changeName==m_info->name){
            m_modifyBtn->setEnabled(false);
            if(changeName!=""){
                m_nameOKNode->getChildByTag(0)->setVisible(!ok);
                m_nameOKNode->getChildByTag(1)->setVisible(ok);
            }
            return ;
        }
        if(changeName==m_lastname) return ;
        m_lastname = changeName;
        CheckAllianceNameCommand* command = new CheckAllianceNameCommand();
        command->putParam("name", CCString::create(changeName));
        command->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::updateModifyBtnStatus), NULL));
        command->sendAndRelease();
    }else if(m_index==3){
        m_abbrOKNode->setVisible(true);
//        m_abbrOKNode->getChildByTag(0)->setVisible(false);
//        m_abbrOKNode->getChildByTag(1)->setVisible(false);
        if(changeName=="" || changeName==m_info->shortName){
            m_modifyAbbrBtn->setEnabled(false);
            if(changeName!=""){
                m_abbrOKNode->getChildByTag(0)->setVisible(!ok);
                m_abbrOKNode->getChildByTag(1)->setVisible(ok);
            }
            return ;
        }
        if(changeName==m_lastname) return ;
        m_lastname = changeName;
        CheckAllianceTagCommand* command = new CheckAllianceTagCommand();
        command->putParam("abbr", CCString::create(changeName));
        command->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceChangeFunCell::onCheckAllianceTag), NULL));
        command->sendAndRelease();
    }

    
}
void AllianceChangeFunCell::checkAllianceRestrictChange(float t){
    std::string castle = m_castlelevel->getText();
    std::string Fight = m_fightability->getText();
    std::string str1 = CC_ITOA(m_info->powerRestriction);
    std::string str2 = CC_ITOA(m_info->castleRestriction);
    if (str2 == castle && str1 == Fight) {
        m_strictBtn->setEnabled(false);
    }
    else{
        m_strictBtn->setEnabled(true);
    }


    }

void AllianceChangeFunCell::onRestrictSuccess(CCObject* data){
    m_info->castleRestriction  = std::atoi(settingcastle.c_str());
    m_info->powerRestriction = std::atol(settingFight.c_str());
    GlobalData::shared()->playerInfo.allianceInfo.castleRestriction = std::atoi(settingcastle.c_str());
    GlobalData::shared()->playerInfo.allianceInfo.powerRestriction = std::atol(settingFight.c_str());
    CCObject* p = NULL;
    strictSuccess(p);
}

void AllianceChangeFunCell::updateModifyBtnStatus(CCObject* data){
    m_nameOKNode->setVisible(true);
    NetResult* result = dynamic_cast<NetResult*>(data);
    
    CCString* stringData = dynamic_cast<CCString*>(result->getData());
    string current = m_nameAndAbbrEditBox->getText();
    bool ok = stringData->intValue() != 1 && current.length() >=3;
    m_nameOKNode->getChildByTag(0)->setVisible(!ok);
    m_nameOKNode->getChildByTag(1)->setVisible(ok);
    m_modifyBtn->setEnabled(ok);
}

void AllianceChangeFunCell::onCheckAllianceTag(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    CCString* stringData = dynamic_cast<CCString*>(result->getData());
    string current = m_nameAndAbbrEditBox->getText();
    bool ok = stringData->intValue() != 1 && current.length() >=3;
    m_abbrOKNode->getChildByTag(0)->setVisible(!ok);
    m_abbrOKNode->getChildByTag(1)->setVisible(ok);
    m_modifyAbbrBtn->setEnabled(ok);
}
void AllianceChangeFunCell::editBoxReturn(CCEditBox *editBox) {
    settingcastle = m_castlelevel->getText();
    settingFight = m_fightability->getText();
    if(settingcastle == "" || settingFight == ""){
        return;
    }
    if(!isNumStr(settingcastle) ){
        settingcastle = "0";
    }
    if(!isNumStr(settingFight)){
        settingFight = "0";
    }
    int MaxLevel = GlobalData::shared()->alliance_gift_k2;
    long MaxPower = GlobalData::shared()->alliance_gift_k3;
    if(std::atoi(settingcastle.c_str()) > MaxLevel){
        settingcastle = CC_ITOA(MaxLevel);
    }
    if(std::atoi(settingFight.c_str()) > MaxPower){
        settingFight = CC_ITOA(MaxPower);
    }
    m_castlelevel->setText(settingcastle.c_str());
    m_fightability->setText(settingFight.c_str());
}

void AllianceChangeFunCell::editBoxTextChanged(CCEditBox* editBox, const std::string& text){


}
void AllianceChangeFunCell::editBoxEditingDidBegin(CCEditBox* editBox)
{
    CCLOG("Begin");
}

bool AllianceChangeFunCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funNode", CCNode*, this->m_funNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg2", CCScale9Sprite*, this->m_bg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrow", CCSprite*, this->m_arrow);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funNode1", CCNode*, this->m_funNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnSave", CCControlButton*, this->m_btnSave);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnEdit", CCControlButton*, this->m_btnEdit);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_announceTxt", CCLabelIFTTF*, this->m_announceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_announceNode", CCNode*, this->m_announceNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funNode2", CCNode*, this->m_funNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchTxt1", CCLabelIF*, this->m_touchTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchTxt2", CCLabelIF*, this->m_touchTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipResuitTxt", CCLabelIF*, this->m_tipResuitTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtn1", CCSprite*, this->m_touchBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtnBg", CCScale9Sprite*, this->m_touchBtnBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemcon", CCNode*, this->m_itemcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTip1", CCLabelIF*, this->m_nameTip1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTip2", CCLabelIF*, this->m_nameTip2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputNameNode", CCNode*, this->m_inputNameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modifyBtn", CCControlButton*, this->m_modifyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funNameNode", CCNode*, this->m_funNameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modifyNameTxt", CCLabelIF*, this->m_modifyNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameGoldTxt", CCLabelIF*, this->m_nameGoldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameGold", CCNode*, this->m_nameGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameOKNode", CCNode*, this->m_nameOKNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_abbrTip1", CCLabelIF*, this->m_abbrTip1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_abbrTip2", CCLabelIF*, this->m_abbrTip2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputAbbrNode", CCNode*, this->m_inputAbbrNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_castleLevelNode", CCNode*, this->m_castleLevelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_FightLevelNode", CCNode*, this->m_FightLevelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputAbbrNode", CCNode*, this->m_inputAbbrNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_DesText", CCLabelIF*, this->m_DesText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_castleText", CCLabelIFTTF*, this->m_castleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_FightText", CCLabelIFTTF*, this->m_FightText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ResTitle", CCLabelIFTTF*, this->m_ResTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_strictBtn", CCControlButton*, this->m_strictBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBar", CCScale9Sprite*, this->m_leftBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBar", CCScale9Sprite*, this->m_rightBar);
    
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modifyAbbrBtn", CCControlButton*, this->m_modifyAbbrBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funAbbrNode", CCNode*, this->m_funAbbrNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_strictNode", CCNode*, this->m_strictNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modifyAbbrTxt", CCLabelIF*, this->m_modifyAbbrTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_abbrGoldTxt", CCLabelIF*, this->m_abbrGoldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_brifGold", CCSprite*, this->m_brifGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_abbrOKNode", CCNode*, this->m_abbrOKNode);
    
    //flag
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagTitle1", CCLabelIF*, this->m_flagTitle1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagTitle2", CCLabelIF*, this->m_flagTitle2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oldFlagNode", CCNode*, this->m_oldFlagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modifyFlagBtn", CCControlButton*, this->m_modifyFlagBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newFlagNode", CCNode*, this->m_newFlagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_modifyFlagTxt", CCLabelIF*, this->m_modifyFlagTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagGoldTxt", CCLabelIF*, this->m_flagGoldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagList", CCNode*, this->m_flagList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funFlagNode", CCNode*, this->m_funFlagNode);
    //lang
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funLangNode", CCNode*, this->m_funLangNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_langList", CCNode*, this->m_langList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceLangIntr", CCLabelIF*, this->m_allianceLangIntr);
    
    //rank title
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oldTitle", CCLabelIF*, this->m_oldTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newTitle", CCLabelIF*, this->m_newTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rank5Txt", CCLabelIFTTF*, this->m_rank5Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputRank5", CCNode*, this->m_inputRank5);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rank4Txt", CCLabelIFTTF*, this->m_rank4Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputRank4", CCNode*, this->m_inputRank4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rank3Txt", CCLabelIFTTF*, this->m_rank3Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputRank3", CCNode*, this->m_inputRank3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rank2Txt", CCLabelIFTTF*, this->m_rank2Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputRank2", CCNode*, this->m_inputRank2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rank1Txt", CCLabelIFTTF*, this->m_rank1Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputRank1", CCNode*, this->m_inputRank1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTip", CCLabelIF*, this->m_rankTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankBtn", CCControlButton*, this->m_rankBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funRankNode", CCNode*, this->m_funRankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_check1", CCSprite*, this->m_check1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_check2", CCSprite*, this->m_check2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_check3", CCSprite*, this->m_check3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_check4", CCSprite*, this->m_check4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_check5", CCSprite*, this->m_check5);
    
    //tname title
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oldTNameTitle", CCLabelIF*, this->m_oldTNameTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newTNameTitle", CCLabelIF*, this->m_newTNameTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name5Txt", CCLabelIFTTF*, this->m_name5Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputName5", CCNode*, this->m_inputName5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name4Txt", CCLabelIFTTF*, this->m_name4Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputName4", CCNode*, this->m_inputName4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name3Txt", CCLabelIFTTF*, this->m_name3Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputName3", CCNode*, this->m_inputName3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name2Txt", CCLabelIFTTF*, this->m_name2Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputName2", CCNode*, this->m_inputName2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1Txt", CCLabelIFTTF*, this->m_name1Txt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputName1", CCNode*, this->m_inputName1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTip", CCLabelIF*, this->m_nameTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameBtn", CCControlButton*, this->m_nameBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funTNameNode", CCNode*, this->m_funTNameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tcheck1", CCSprite*, this->m_tcheck1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tcheck2", CCSprite*, this->m_tcheck2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tcheck3", CCSprite*, this->m_tcheck3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tcheck4", CCSprite*, this->m_tcheck4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tcheck5", CCSprite*, this->m_tcheck5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lock1", CCSprite*, this->m_lock1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lock2", CCSprite*, this->m_lock2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lock3", CCSprite*, this->m_lock3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lock4", CCSprite*, this->m_lock4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lock5", CCSprite*, this->m_lock5);
    return false;
}

SEL_CCControlHandler AllianceChangeFunCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickHandle", AllianceChangeFunCell::clickHandle);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSaveClick", AllianceChangeFunCell::onSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEditClick", AllianceChangeFunCell::onEditClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "modifyNameClick", AllianceChangeFunCell::onNameModifyClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "modifyAbbrClick", AllianceChangeFunCell::onModifyAbbrClick);
     CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStrictBtnclick", AllianceChangeFunCell::onStrictBtnclick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "modifyRankClick", AllianceChangeFunCell::modifyRankClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "modifyTNameClick", AllianceChangeFunCell::modifyTNameClick);
    return NULL;
}


//
AllianceFlagCell *AllianceFlagCell::create(std::string icon){
    AllianceFlagCell *ret = new AllianceFlagCell(icon);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceFlagCell::init(){
    m_flagNode = CCLoadSprite::createSprite("icon_kuang.png");
    m_flagNode->setPositionX(115);
    this->addChild(m_flagNode);
    this->setContentSize(CCSize(114,114));
    setData(m_icon);
    return true;
}

void AllianceFlagCell::setData(std::string icon){
    m_flagNode->removeAllChildrenWithCleanup(true);
    std::string head = m_icon;
    head.append(".png");
    CCSprite* flag = CCLoadSprite::createSprite(head.c_str());
    flag->setPosition(ccp(57, 57));
    CCCommonUtils::setSpriteMaxSize(flag, 106);
    m_flagNode->addChild(flag);
}

void AllianceFlagCell::onEnter(){
    CCNode::onEnter();

    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void AllianceFlagCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AllianceFlagCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void AllianceFlagCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void AllianceFlagCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
//===================
LangList *LangList::create(CCNode* parent){
    LangList *ret = new LangList(parent);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void LangList::onEnter(){
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCNode::onEnter();
}
void LangList::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool LangList::init(){
    if (!CCNode::init()) {
        return false;
    }
    m_size = m_parent->getContentSize();
    this->setContentSize(m_parent->getContentSize());
    this->setAnchorPoint(CCPointZero);
    
    CCDictionary* dic_all = LocalController::shared()->shared()->DBXMLManager()->getGroupByKey("language");
    int idx = 9500;
    int num = dic_all->count();
    m_curCeleted = 999;
    string curLang = GlobalData::shared()->playerInfo.allianceInfo.language;
    int allLangIdx = 999;
    string allLangId = "";
    for (int i=0,j=0; i<num; ++i) {
        
        CCDictionary* dic_one = _dict(dic_all->objectForKey(CC_ITOA(idx+i)));
        //CCLOG("%s %s %d",dic_one->valueForKey("lang_ma")->getCString(), dic_one->valueForKey("lang_id")->getCString(), dic_one->valueForKey("mark")->intValue());
        
        if(dic_one->valueForKey("mark")){
            m_langVec.push_back(dic_one->valueForKey("lang_id")->getCString());
            if (curLang == m_langVec[j]) {
                m_curCeleted = j;
            }
            if(strcmp(dic_one->valueForKey("lang_ma")->getCString(), "")==0){
                allLangIdx = j;
                allLangId = dic_one->valueForKey("lang_id")->getCString();
            }
            ++j;
        }
    }
    if(m_curCeleted == 999){
        m_curCeleted = allLangIdx;
    }
    
    m_langCnt = m_langVec.size();
    float sizeH = ceil(m_langCnt/2.0)*70;
    if (CCCommonUtils::isIosAndroidPad()) {
        sizeH = ceil(m_langCnt / 2.0) * 140;
    }
    m_size = CCSize(m_size.width, sizeH);
    
    m_tabView = CCMultiColTableView::create(this, m_parent->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    this->addChild(m_tabView);
    m_tabView->reloadData();
    return true;
}
bool LangList::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_parent->getParent()->isVisible() && isTouchInside(m_parent, pTouch)) {
        return true;
    }else{
        return false;
    }
}
void LangList::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(fabs(pTouch->getStartLocation().y-pTouch->getLocation().y)>30){
        return;
    }
    int idx = 0;
    for (int i=0; i<m_langCnt; ++i) {
        m_cellVec[i]->setCelect(false);
        if (isTouchInside(m_cellVec[i], pTouch)) {
            idx = i;
        }
    }
    m_cellVec[idx]->setCelect(true);
    if (idx == m_langCnt - 1) {
        for (int i = 0; i < m_langCnt; ++i) {
            m_cellVec[i]->setCelect(true);
        }
    }
    ChangeAllianceAttributesCommand* command = new ChangeAllianceAttributesCommand();
    command->putParam("language", CCString::create(m_langVec[idx]));
    command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(LangList::changeLangSuccess), NULL));
    command->sendAndRelease();
}
void LangList::changeLangSuccess(CCObject* p){
    string lang = GlobalData::shared()->playerInfo.allianceInfo.language;
    string str = _lang_1("115239", _lang(lang).c_str());
    CCCommonUtils::flyHint("", "", str);
}
void LangList::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}
void LangList::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){

}
cocos2d::CCSize LangList::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return m_size;
}
CCTableViewCell* LangList::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= 1) return NULL;
    
    CCTableViewCell* cell = table->dequeueGrid();
    if(cell){
        
    }else {
        cell = new CCTableViewCell();
        cell->autorelease();
//        CCLOG("language %s", GlobalData::shared()->lang.c_str());
//        CCLOG("language %s", GlobalData::shared()->playerInfo.allianceInfo.language.c_str());

        for (int i=0; i<m_langCnt; ++i) {
            float x = (i%2)*m_size.width/2.0;
            float y = m_size.height - (i/2 + 1)*70;
            if (CCCommonUtils::isIosAndroidPad())
            {
                y = m_size.height - (i/2 + 1)*140;
            }
            bool celeted = false;
            if(i == m_curCeleted)
                celeted = true;
            if (m_curCeleted == m_langCnt - 1) {
                celeted = true;
            }
            LangListCell* node = LangListCell::create(m_langVec[i], celeted);
            node->setPosition(x, y);
            cell->addChild(node);
            m_cellVec.push_back(node);
        }
    }
    
    return cell;
}
ssize_t LangList::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return 1;
}
unsigned int LangList::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}
//cell
LangListCell *LangListCell::create(std::string lang, bool celected){
    LangListCell* ret = new LangListCell(lang, celected);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LangListCell::init(){
    if (!CCNode::init()) {
        return false;
    }
    
    this->setContentSize(CCSize(280, 70));
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setScale(2);
    }
    this->setAnchorPoint(CCPointZero);
    CCBLoadFile("UpdateAllianceLangCell", this, this);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_langText->setScale(0.5);
        m_langText->setFontSize(44);
    }
    setData(m_lang, m_celected);
    return true;
}
void LangListCell::setData(std::string lang, bool celected){
    m_celectNode->getChildByTag(1)->setVisible(celected);
//    m_celectNode->getChildByTag(0)->setVisible(!celected);
    m_langText->setString(_lang(lang));
}
bool LangListCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_langText", CCLabelIF*, this->m_langText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_celectNode", CCNode*, this->m_celectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clicktNode);
    return false;
}
SEL_CCControlHandler LangListCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
void LangListCell::setCelect(bool b){
//    m_celectNode->getChildByTag(0)->setVisible(!b);
    m_celectNode->getChildByTag(1)->setVisible(b);
}
