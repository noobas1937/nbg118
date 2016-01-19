//
//  DetectMailPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-4-15.
//
//

#include "DetectMailPopUpView.h"
#include "BattleReportMailPopUpView.h"
#include "PopupViewController.h"
#include "RoleInfoView.h"
#include "WorldController.h"
#include "SceneController.h"
#include "MailController.h"
#include "GeneralHeadPic.h"
#include "MailPopUpView.h"
#include "WorldMapView.h"
#include "WorldController.h"
#include "FunBuildController.h"
#include "GeneralHeadPic.h"
#include "GeneralManager.h"
#include "RewardController.h"
#include "GeneralSkillListPopUpView.h"
#include "MailBattleDetailView.h"
#include "YesNoDialog.h"
#include "MailSaveCommand.h"
#include "ResourceMailPopUpView.h"
#include "OccupyMailPopUpView.h"
#include "PlayerInfoBtnPopUpView.h"
#include "CountryChatCommand.h"
#include "ChatServiceCocos2dx.h"
#include "UIComponent.h"
#include "MailWritePopUpView.h"
const static int cellGapY = 10;
const static int cellHdGapY = 24;

DetectMailPopUpView *DetectMailPopUpView::create(MailInfo *info){
    DetectMailPopUpView *ret = new DetectMailPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DetectMailPopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade,true);//simon;
    UIComponent::getInstance()->hideReturnBtn();
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DetectMailPopUpView::refreshView), MAIL_CONTENT_READ, NULL);
//    setTitleName(m_info->fromName.c_str());
}

void DetectMailPopUpView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_CONTENT_READ);
    PopupBaseView::onExit();
}

bool DetectMailPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        m_totalH = 0;
        m_abilityId = "";
        setIsHDPanel(true);
        setMailUuid(m_info->uid);
        auto cf = CCLoadSprite::getSF("Mail_BG1.png");
//        auto cf = CCLoadSprite::getSF("Mail_diban.png");
        if (cf==NULL) {
            CCLoadSprite::doResourceByCommonIndex(105, true);
            CCLoadSprite::doResourceByCommonIndex(6, true);
            CCLoadSprite::doResourceByCommonIndex(204, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(105, false);
                CCLoadSprite::doResourceByCommonIndex(6, false);
                CCLoadSprite::doResourceByCommonIndex(204, false);
            });
        }
        else {
            CCLoadSprite::doResourceByCommonIndex(105, true);
            CCLoadSprite::doResourceByCommonIndex(204, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(105, false);
                CCLoadSprite::doResourceByCommonIndex(204, false);
            });
        }
        auto bg = CCBLoadFile("NEW_DetectViewCCB", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else
            this->setContentSize(bg->getContentSize());

        m_shareBtn->setVisible(false);
        m_listNode = CCNode::create();
        m_mailTitle->setString("");
        m_timeText->setString("");
        m_titleText->setString(m_info->fromName.c_str());
        this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());
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

        this->m_mailTitle->setFntFile("Arial_Bold_Regular.fnt");
        this->m_timeText->setFntFile("Arial_Bold_Regular.fnt");
        
        m_scrollView = CCScrollView::create(m_listContainer->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setAnchorPoint(ccp(0, 0));
        m_listContainer->addChild(m_scrollView);
        m_scrollView->addChild(m_listNode);

        if(!m_info->isReadContent){
            getData();
        }else{
            refreshView();
        }
        ret = true;
    }
    return ret;
}
bool DetectMailPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(this->m_listContainer, pTouch)){
        return true;
    }
    if (isTouchInside(this->m_returnSpr, pTouch)) {
        return true;
    }
    return false;
}

void DetectMailPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void DetectMailPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    AutoSafeRef tmp(this);
    
    if (isTouchInside(this->m_returnSpr, pTouch)) {
        PopupViewController::getInstance()->goBackPopupView();
    }
    if(!isTouchInside(this->m_listContainer, pTouch)){
        return;
    }
    int d = pTouch->getStartLocation().y - pTouch->getLocation().y;
    d = abs(d);
    if(d > 20){
        return;
    }

    auto& arr = m_listNode->getChildren();
    int i = 0;
 
    for(auto child:arr)
    {
        DetectPlayerInfoCell *playerCell = dynamic_cast<DetectPlayerInfoCell*>(child);
        if(playerCell
           && isTouchInside(playerCell, pTouch)){
            playerCell->cellTouch(pTouch);
        }
    }
 
//    while(arr && i < arr->count()){
//            DetectPlayerInfoCell *playerCell = dynamic_cast<DetectPlayerInfoCell*>(arr->objectAtIndex(i));
//            if(playerCell
//               && isTouchInside(playerCell, pTouch)){
//                playerCell->cellTouch(pTouch);
//            }
//
//        i++;
//    }
}
void DetectMailPopUpView::refreshView(CCObject* p){
    if(m_info->save==0){
        m_addSaveBtn->setHighlighted(false);
    }else{
        m_addSaveBtn->setHighlighted(true);
    }
    if (m_info->isFromChat) {
        m_addSaveBtn->setVisible(false);
        m_deleteBtn->setVisible(false);
        m_shareBtn->setVisible(false);
    }
    
    if(m_info->type==MAIL_DETECT){
        auto cell = DetectedByPlayerCell::create(m_info);
        m_totalH = 0;
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH -= 936;
        }
        else
            m_totalH -= 390;
        cell->setPosition(0, m_totalH);
        this->m_listNode->addChild(cell);
        m_mailTitle->setString(m_info->title);
        m_shareBtn->setVisible(false);
        
        
        if(m_info->pointType == Trebuchet){
            std::string strType=_lang("110081");
            m_mailTitle->setString(_lang_1("140181", strType.c_str()));
        }
        else if (m_info->pointType == Throne){
            std::string strType=_lang("110172");
            m_mailTitle->setString(_lang_1("140181", strType.c_str()));
        }
        
        
    }else{
        if (!m_info->isFromChat) {
            m_shareBtn->setVisible(true);
        }
        addPlayerInfo();
        addResourceInfo();
        addCityDefence();
        addSelfHeroInfo();
        addAllianceHelpInfo();
        if(m_info->detectReport->objectForKey("fort") || m_info->detectReport->objectForKey("tower") || m_info->detectReport->objectForKey("defence")){
           addFortInfo();
        }
        if(m_info->detectReport->objectForKey("fort") || m_info->detectReport->objectForKey("tower") || m_info->detectReport->objectForKey("defence")){
            addTowerInfo();
        }
        addScienceInfo();
        addTalentInfo();
        addAbilityInfo();

        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH -= 48;
        }
        else
            m_totalH -= 20;
        
        std::string nameStr = m_info->detectReport->valueForKey("name")->getCString();
        
        

        int pos = m_info->detectReport->valueForKey("pointId")->intValue();
        CCPoint pt = WorldController::getPointByIndex(pos);
//        if (m_info && m_info->serverType>=SERVER_BATTLE_FIELD) {
//            pt = WorldController::getPointByMapTypeAndIndex(pos,(MapType)m_info->serverType);
//        } simon
//        m_mailTitle->setString(_lang_2("105525", CC_ITOA(pt.x), CC_ITOA(pt.y)));
        
        
        if(m_info->pointType == Trebuchet){
            std::string strType=_lang("110081");
            m_mailTitle->setString(_lang_1("140180", strType.c_str()));
        }
        else if (m_info->pointType == Throne){
            std::string strType=_lang("110172");
            m_mailTitle->setString(_lang_1("140180", strType.c_str()));
        }
        else{
            m_mailTitle->setString(_lang("105527"));
        }
        
        
    }

    m_totalH *= -1;
    auto& arr = m_scrollView->getContainer()->getChildren();
    for(auto child:arr)
    {
        auto node = dynamic_cast<CCNode*>(child);
        node->setPositionY(node->getPositionY() + m_totalH);
    }    
//    int i = 0;
//    while( arr && i < arr->count()){
//        auto node = dynamic_cast<CCNode*>(arr->objectAtIndex(i++));
//        node->setPositionY(node->getPositionY() + m_totalH);
//    }
    m_totalH += 3;
    m_scrollView->setContentSize(CCSize(m_listContainer->getContentSize().width, m_totalH));
    m_scrollView->setContentOffset(ccp(0, -m_totalH + m_listContainer->getContentSize().height));
}
void DetectMailPopUpView::addPlayerInfo(){
    CCDictionary *dict = dynamic_cast<CCDictionary*>(m_info->detectReport->objectForKey("user"));
    if(dict == NULL){
        return;
    }
    if (dict->objectForKey("serverType")) {
        m_info->serverType = dict->valueForKey("serverType")->intValue();
    }
    auto cell = DetectPlayerInfoCell::create(dict,m_info->pointType,m_info);
    m_totalH = 0;
    if (CCCommonUtils::isIosAndroidPad()) {
        m_totalH -= 936;
    }
    else
        m_totalH -= 390;
    cell->setPosition(0, m_totalH);
    this->m_listNode->addChild(cell);
}
void DetectMailPopUpView::addResourceInfo(){
    CCDictionary *dict = dynamic_cast<CCDictionary*>(m_info->detectReport->objectForKey("resource"));
    if(dict == NULL){
        return;
    }
    bool isResourceShieldState = false;
    if(dict->objectForKey("isResourceShieldState")){
        isResourceShieldState = dict->valueForKey("isResourceShieldState")->boolValue();
    }

    CCArray *arr = RewardController::getInstance()->getRewardArr(dict);

    int rowNum = arr->count();
    DetectBgCell* bgCell = DetectBgCell::create(_lang("114127").c_str());
    if(rowNum>2){
        int bgHeight = (rowNum-2)*45+bgCell->getBgCellHeight();
        if (CCCommonUtils::isIosAndroidPad()) {
            bgHeight = (rowNum-2)*108+bgCell->getBgCellHeight();
        }
        bgCell->setBgHeight(bgHeight);
    }

    if (CCCommonUtils::isIosAndroidPad()) {
        m_totalH -= cellHdGapY;
    }
    else
        m_totalH -= cellGapY;
    m_totalH -= bgCell->getBgCellHeight();
    bgCell->setPosition(0, m_totalH);
    this->m_listNode->addChild(bgCell);
    bgCell->setTitle(_lang("114126"), "", _lang("114127"));
    
    int i = 0;
    CCObject *obj;
    CCARRAY_FOREACH(arr, obj){
        CCDictionary *reward = dynamic_cast<CCDictionary*>(obj);
        int value = atoi(reward->valueForKey("value")->getCString());
        int type = atoi(reward->valueForKey("type")->getCString());
        int notCollectNum = atoi(reward->valueForKey("notCollect")->getCString());
    
        auto cell = DetectRewardPreviewCell::create(type, notCollectNum);
        cell->setAnchorPoint(ccp(0, 0));
        int posX = 60;
        if (CCCommonUtils::isIosAndroidPad()) {
            posX = 144;
            cell->setPosition(ccp(posX,  96+i*108));
        }
        else
            cell->setPosition(ccp(posX,  40+i*45));
        bgCell->m_bg->addChild(cell);
        
        i++;
        if(rowNum==1){
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(posX,  96+48));
            }
            else
                cell->setPosition(ccp(posX,  40+20));
        }
        string numStr = CC_CMDITOA(value);
        numStr.append(" +");
        auto label = CCLabelIF::create(numStr.c_str());
        label->setFntFile("Arial_Bold_Regular.fnt");
        if (CCCommonUtils::isIosAndroidPad()) {
            label->setFontSize(label->getFontSize()*2);
        }
        bgCell->m_bg->addChild(label);
        label->setColor(ccc3(85,62,31));
        
        if (CCCommonUtils::isIosAndroidPad()) {
            label->setPosition(cell->getPosition() + ccp(720, 0));
        }
        else
            label->setPosition(cell->getPosition() + ccp(300, 0));
        label->setAnchorPoint(ccp(1, 0.5));
    }
    if(isResourceShieldState){
        if (CCCommonUtils::isIosAndroidPad()) {
            auto cover = CCLayerColor::create(ccc4(0, 0, 0, 127), 1500, rowNum * 108 + 25);
            this->m_listNode->addChild(cover);
            cover->setPosition(bgCell->getPosition() + ccp(0, 0));
            auto label = CCLabelIF::create(_lang("103766").c_str());
            label->setFntFile("Arial_Bold_Regular.fnt");
            label->setDimensions(CCSize(1440, 0));
            this->m_listNode->addChild(label);
            label->setPosition(cover->getPosition() + cover->getContentSize() / 2);
        }
        else {
            auto cover = CCLayerColor::create(ccc4(0, 0, 0, 127), 625, rowNum * 45 + 25);
            this->m_listNode->addChild(cover);
            cover->setPosition(bgCell->getPosition() + ccp(0, 0));
            auto label = CCLabelIF::create(_lang("103766").c_str());
            label->setFntFile("Arial_Bold_Regular.fnt");
            label->setDimensions(CCSize(600, 0));
            this->m_listNode->addChild(label);
            label->setPosition(cover->getPosition() + cover->getContentSize() / 2);
        }
    }
}

void DetectMailPopUpView::addCityDefence(){
    CCDictionary *dict = dynamic_cast<CCDictionary*>(m_info->detectReport->objectForKey("resource"));
    if(dict == NULL){
        return;
    }
    if(dict->objectForKey("cityDefValue")){
        int cityDefence = dict->valueForKey("cityDefValue")->intValue();
        int max = dict->valueForKey("cityDefMax")->intValue();
        DetectBgCell* bgCell = DetectBgCell::create(_lang("102206").c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH -= cellHdGapY;
        }
        else
            m_totalH -= cellGapY;
        m_totalH -= bgCell->getBgCellHeight();
        bgCell->setPosition(0, m_totalH);
        this->m_listNode->addChild(bgCell);
        bgCell->setNumTitle(_lang("102206"), std::string(CC_CMDITOA(cityDefence)) + "/" + CC_CMDITOA(max));
        resetCellBG(bgCell);
//        bgCell->m_kuangbg->setVisible(false);
    }
}

void DetectMailPopUpView::addAbilityInfo(){
    if(m_info->detectReport->objectForKey("ability")){
        CCArray* arr = dynamic_cast<CCArray*>(m_info->detectReport->objectForKey("ability"));
        int i = 0;
        if(arr && arr->count()>0){
            DetectBgCell* bgCell = DetectBgCell::create(_lang("105533").c_str());
            if (CCCommonUtils::isIosAndroidPad()) {
                m_totalH -= cellHdGapY;
            }
            else
                m_totalH -=cellGapY;
            m_totalH -= bgCell->getBgCellHeight();
            bgCell->setPosition(0, m_totalH);
            
            this->m_listNode->addChild(bgCell);
            MailItemCell* cell;
            std::string name = "";
            std::string icon = "";
            int i = 0;
            int cellH = 145;
            if (CCCommonUtils::isIosAndroidPad()) {
                cellH = 348;
            }
            if(arr->count()==0){
                resetCellBG(bgCell);
                //                bgCell->m_kuangbg->setVisible(false);
            }else if(arr->count()==1){
                auto dic = _dict(arr->objectAtIndex(0));
                name = CCCommonUtils::getNameById(dic->valueForKey("id")->getCString());
                icon = CCCommonUtils::getIcon(dic->valueForKey("id")->getCString());
                cell = MailItemCell::create(name,icon,dic->valueForKey("id")->getCString());
                cell->m_detectReport = m_info->detectReport;
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(623,0);
                }
                else
                    cell->setPosition(262, 0);
                bgCell->m_bg->addChild(cell);
                i = 1;
            }else if(arr->count()==2){
                auto dic1 = _dict(arr->objectAtIndex(0));
                name = CCCommonUtils::getNameById(dic1->valueForKey("id")->getCString());
                icon = CCCommonUtils::getIcon(dic1->valueForKey("id")->getCString());
                cell = MailItemCell::create(name,icon,dic1->valueForKey("id")->getCString());
                cell->m_detectReport = m_info->detectReport;
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(362,0);
                }
                else
                    cell->setPosition(151, 0);
                bgCell->m_bg->addChild(cell);
                
                auto dic2 = _dict(arr->objectAtIndex(1));
                name = CCCommonUtils::getNameById(dic2->valueForKey("id")->getCString());
                icon = CCCommonUtils::getIcon(dic2->valueForKey("id")->getCString());
                cell = MailItemCell::create(name,icon,dic2->valueForKey("id")->getCString());
                cell->m_detectReport = m_info->detectReport;
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(922, 0);
                }
                else
                    cell->setPosition(384.0, 0);
                bgCell->m_bg->addChild(cell);
                i = 2;
            }else{
                int numPerRow = 2;
                int cellY = 0;
                int col = 0;
                CCObject *obj;
                CCARRAY_FOREACH(arr, obj){
                    col = i%2;
                    auto dic = _dict(arr->objectAtIndex(i));
                    name = CCCommonUtils::getNameById(dic->valueForKey("id")->getCString());
                    icon = CCCommonUtils::getIcon(dic->valueForKey("id")->getCString());
                    cell = MailItemCell::create(name,icon,dic->valueForKey("id")->getCString());
                    cell->m_detectReport = m_info->detectReport;
                    if (CCCommonUtils::isIosAndroidPad()) {
                        if(col==0){
                            cell->setPosition(362, cellY);
                        }else{
                            cell->setPosition(922, cellY);
                        }
                    }
                    else {
                        if(col==0){
                            cell->setPosition(151, cellY);
                        }else{
                            cell->setPosition(384, cellY);
                        }
                    }
                    
                    bgCell->m_bg->addChild(cell);
                    if (arr->count() % numPerRow == 0) {
                        i++;
                        if(i % numPerRow == 0){
                            cellY += cellH;
                        }
                    }
                    else if (arr->count() % numPerRow == 1) {
                        if(i % numPerRow == 0){
                            cellY += cellH;
                        }
                        i++;
                    }
                }
            }
            int addH = cellH * int((arr->count() + 1) / 2);
            if(addH > 0){
                addH -= 75;
            }
            bgCell->m_bg->setContentSize(CCSize(bgCell->m_bg->getContentSize().width, bgCell->m_bg->getContentSize().height+addH));
            bgCell->m_bg->setPositionY(bgCell->m_bg->getPositionY()-addH);
            //            bgCell->m_kuangbg->setContentSize(CCSize(bgCell->m_kuangbg->getContentSize().width, bgCell->m_kuangbg->getContentSize().height+addH));
            //            bgCell->m_kuangbg->setPositionY(bgCell->m_kuangbg->getPositionY()-addH);
            m_totalH -= addH;
        }
    }
}

void DetectMailPopUpView::addSelfHeroInfo(){
    if(m_info->detectReport->objectForKey("defence")){
        auto defence = _dict(m_info->detectReport->objectForKey("defence"));
        CCArray* arr = NULL;
        if(defence){
            arr = dynamic_cast<CCArray*>(defence->objectForKey("arms"));
        }
        CCObject *obj;
        int totalNum = defence->valueForKey("total")->intValue();
        std::string titleStr = _lang("105530") + "(~" + CC_CMDITOA(totalNum) + ")";
        DetectBgCell* bgCell = DetectBgCell::create(titleStr);
        std::string numStr = CC_CMDITOA(totalNum);
        bool about = false;
        if(defence->objectForKey("about") && defence->valueForKey("about")->boolValue()){
            about = true;
            numStr = std::string("~") + CC_CMDITOA(totalNum);
        }
        bgCell->setNumTitle(_lang("105530"), numStr);
        int rowNum = arr->count();
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH -= cellHdGapY;
        }
        else
            m_totalH-=cellGapY;
        bgCell->setBgHeight(bgCell->getBgCellHeight()+60);
        if(rowNum>1){
            if (CCCommonUtils::isIosAndroidPad()) {
                int bgHeight = (rowNum-1)*264+bgCell->getBgCellHeight();
                bgCell->setBgHeight(bgHeight);
            }
            else {
                int bgHeight = (rowNum-1)*110+bgCell->getBgCellHeight();
                bgCell->setBgHeight(bgHeight);
            }
        }
        m_totalH -= bgCell->getBgCellHeight();
        bgCell->setPosition(0, m_totalH);
        if(rowNum == 0){
            resetCellBG(bgCell);
//            bgCell->m_kuangbg->setVisible(false);
        }

        this->m_listNode->addChild(bgCell);
        int i = 0;
        int numPerRow = 1;
        int cellH = 37;
        if (CCCommonUtils::isIosAndroidPad()) {
            cellH = 89;
        }
        int cellY = bgCell->m_bg->getContentSize().height-40;
        CCARRAY_FOREACH(arr, obj){
            if(i % numPerRow == 0){
                cellY -= cellH;
            }
            auto dict = dynamic_cast<CCDictionary*>(obj);
            if(dict){
                std::string id = dict->valueForKey("id")->getCString();
                int num = dict->valueForKey("num")->intValue();
                string name = CCCommonUtils::getNameById(id);
                auto cell = DetectArmyCell::create(id, num,true);
                cell->setAnchorPoint(ccp(0, 0));
                int posX = 0;
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(ccp(posX,  96+i*264));
                }
                else
                    cell->setPosition(ccp(posX,  40+i*110));
                bgCell->m_bg->addChild(cell);
                i++;
            }
        }
    }
    if (!m_info->detectReport->objectForKey("defence") && (m_info->pointType == Tile_allianceArea || m_info->pointType == tile_banner)) {////////
        int totalNum = 0;
        std::string titleStr = _lang("105530") + "(~" + CC_CMDITOA(totalNum) + ")";
        DetectBgCell* bgCell = DetectBgCell::create(titleStr);
        std::string numStr = std::string("~") + CC_CMDITOA(totalNum);
        bgCell->setNumTitle(_lang("105530"), numStr);
        int rowNum = 0;
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH-=cellHdGapY;
        }
        else
            m_totalH-=cellGapY;
        bgCell->setBgHeight(bgCell->getBgCellHeight()+60);
        m_totalH -= bgCell->getBgCellHeight();
        bgCell->setPosition(0, m_totalH);
        resetCellBG(bgCell);
//        bgCell->m_kuangbg->setVisible(false);
        this->m_listNode->addChild(bgCell);
    }
}

void DetectMailPopUpView::resetCellBG(DetectBgCell *bgCell){
    int bgCellH = bgCell->getBgCellHeight();
    bgCell->m_bg->setContentSize(CCSize(bgCell->m_bg->getContentSize().width, bgCell->m_titleBG->getContentSize().height + 10));
    int bgCellH1 = bgCell->getBgCellHeight();
    bgCell->m_bg->setPositionY(bgCell->m_bg->getPositionY() + bgCellH - bgCellH1);
    m_totalH += bgCellH - bgCellH1;
}

void DetectMailPopUpView::addFortInfo(){
    if(m_info->detectReport->objectForKey("fort")){
        auto fort = dynamic_cast<CCDictionary*>(m_info->detectReport->objectForKey("fort"));
        auto arr = dynamic_cast<CCArray*>(fort->objectForKey("forts"));
        CCObject *obj;
        int rowNum = arr->count();
        int total = fort->valueForKey("total")->intValue();
        std::string title = "";
        bool about = false;
        if(fort->objectForKey("about") && fort->valueForKey("about")->boolValue()){
            about = true;
            title.append("~");
        }
        title.append(CC_CMDITOA(total));
        DetectBgCell* bgCell = DetectBgCell::create(title);
        bgCell->setNumTitle(_lang("105529"), title);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH-=cellHdGapY;
        }
        else
            m_totalH-=cellGapY;
        if(rowNum>1){
            int bgHeight = (rowNum-1)*45+bgCell->getBgCellHeight()-40;
            if (CCCommonUtils::isIosAndroidPad()) {
                bgHeight = (rowNum-1)*108+bgCell->getBgCellHeight()-96;
            }
            bgCell->setBgHeight(bgHeight);
        }
        m_totalH -= bgCell->getBgCellHeight();
        bgCell->setPosition(0, m_totalH);
        if(rowNum == 0){
            resetCellBG(bgCell);
//            bgCell->m_kuangbg->setVisible(false);
        }

        this->m_listNode->addChild(bgCell);
        int i = 0;
        int numPerRow = 1;
        int cellH = 37;
        if (CCCommonUtils::isIosAndroidPad()) {
            cellH = 89;
        }
        int cellY = bgCell->m_bg->getContentSize().height-40;
        CCARRAY_FOREACH(arr, obj){
            if(i % numPerRow == 0){
                cellY -= cellH;
            }
            auto dict = dynamic_cast<CCDictionary*>(obj);
            if(dict){
                std::string id = dict->valueForKey("id")->getCString();
                int num = dict->valueForKey("num")->intValue();
                auto cell = OccupyLossPreviewCell::create(CCCommonUtils::getNameById(id), num, about);
                cell->setAnchorPoint(ccp(0, 0));
                int posX = 60;
                if (CCCommonUtils::isIosAndroidPad()) {
                    posX = 144;
                    cell->setPosition(ccp(posX,  96+i*108));
                }
                else
                    cell->setPosition(ccp(posX,  40+i*45));
                bgCell->m_bg->addChild(cell);
                if(rowNum==1){
                    if (CCCommonUtils::isIosAndroidPad()) {
                        cell->setPosition(ccp(posX,  96+48));
                    }
                    else
                        cell->setPosition(ccp(posX,  40+20));
                }
                i++;
            }
        }
    }
}

void DetectMailPopUpView::addTowerInfo(){
    if(m_info->detectReport->objectForKey("tower")){
        auto arr = dynamic_cast<CCArray*>(m_info->detectReport->objectForKey("tower"));
        
        std::string title = _lang("102016");
        DetectBgCell* bgCell = DetectBgCell::create(title);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH -= cellHdGapY;
        }
        else
            m_totalH -=cellGapY;
        m_totalH -= bgCell->getBgCellHeight();
        bgCell->setPosition(0, m_totalH);
        this->m_listNode->addChild(bgCell);
        if(arr && arr->count()>0){
            if(arr->count()==1){
                auto dic = _dict(arr->objectAtIndex(0));
                int level = dic->valueForKey("level")->intValue();
                std::string lv = _lang("108634");
                if(dic->objectForKey("about") && dic->valueForKey("about")->boolValue()){
                    lv.append("~");
                }
                lv.append(CC_CMDITOA(level));
                MailItemCell* cell = MailItemCell::create(lv,"pic417000_2.png");
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(623, 0);
                }
                else
                    cell->setPosition(262, 0);
                bgCell->m_bg->addChild(cell);
            }else{
                auto dic1 = _dict(arr->objectAtIndex(0));
                int level = dic1->valueForKey("level")->intValue();
                int star = dic1->valueForKey("star")->intValue();
                std::string lv = _lang("108634");
                if(dic1->objectForKey("about") && dic1->valueForKey("about")->boolValue()){
                    lv.append("~");
                }
                if (star >= 1) {
                    lv=_lang("160001");
                    lv.append(CC_CMDITOA(star));
                }else{
                    lv.append(CC_CMDITOA(level));
                }
                MailItemCell* cell = MailItemCell::create(lv,"pic417000_2.png");
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(362, 0);
                }
                else
                    cell->setPosition(151, 0);
                bgCell->m_bg->addChild(cell);
                
                auto dic2 = _dict(arr->objectAtIndex(1));
                level = dic2->valueForKey("level")->intValue();
                star = dic2->valueForKey("star")->intValue();
                if (star >= 1) {
                    lv =_lang("160001");
                    if(dic2->objectForKey("about") && dic2->valueForKey("about")->boolValue()){
                        lv.append("~");
                    }
                    lv.append(CC_CMDITOA(star));
                }else{
                    lv = _lang("108634");
                    if(dic2->objectForKey("about") && dic2->valueForKey("about")->boolValue()){
                        lv.append("~");
                    }
                    lv.append(CC_CMDITOA(level));
                }
                cell = MailItemCell::create(lv,"pic417000_2.png");
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(922, 0);
                }
                else
                    cell->setPosition(384.0, 0);
                bgCell->m_bg->addChild(cell);
            }
        }else{
            auto tower = dynamic_cast<CCDictionary*>(m_info->detectReport->objectForKey("tower"));
            if(tower){
                int level = tower->valueForKey("level")->intValue();
                std::string lv = _lang("108634");
                if(tower->objectForKey("about") && tower->valueForKey("about")->boolValue()){
                    lv.append("~");
                }
                lv.append(CC_CMDITOA(level));
                MailItemCell* cell = MailItemCell::create(lv,"pic417000_2.png");
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(629, 0);
                }
                else
                    cell->setPosition(262, 0);
                bgCell->m_bg->addChild(cell);
            }
        }
        int addH = 60;
        bgCell->m_bg->setContentSize(CCSize(bgCell->m_bg->getContentSize().width, bgCell->m_bg->getContentSize().height+addH));
        bgCell->m_bg->setPositionY(bgCell->m_bg->getPositionY()-addH);
//        bgCell->m_kuangbg->setContentSize(CCSize(bgCell->m_kuangbg->getContentSize().width, bgCell->m_kuangbg->getContentSize().height+addH));
//        bgCell->m_kuangbg->setPositionY(bgCell->m_kuangbg->getPositionY()-addH);
        m_totalH -= addH;
    }
}

void DetectMailPopUpView::addScienceInfo(){
    if(m_info->detectReport->objectForKey("science")){
        auto arr = dynamic_cast<CCArray*>(m_info->detectReport->objectForKey("science"));
        if(arr && arr->count() > 0){
            DetectBgCell* bgCell = DetectBgCell::create(_lang("105532").c_str());
            if (CCCommonUtils::isIosAndroidPad()) {
                m_totalH -= cellHdGapY;
            }
            else
                m_totalH -=cellGapY;
            m_totalH -= bgCell->getBgCellHeight();
            bgCell->setPosition(0, m_totalH);

            this->m_listNode->addChild(bgCell);
            MailItemCell* cell;
            std::string name = "";
            std::string id = "";
            int i = 0;
            int cellH = 145;
            if (CCCommonUtils::isIosAndroidPad()) {
                cellH = 348;
            }
            if(arr->count()==0){
                resetCellBG(bgCell);
//                bgCell->m_kuangbg->setVisible(false);ƒ
            }else if(arr->count()==1){
                auto dic = _dict(arr->objectAtIndex(0));
                id = dic->valueForKey("id")->getCString();
                name = CCCommonUtils::getNameById(id);
                cell = MailItemCell::create(name,CCCommonUtils::getIcon(id).c_str());
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(524, 0);
                }
                else
                    cell->setPosition(629, 0);
                bgCell->m_bg->addChild(cell);
                i = 1;
            }else if(arr->count()==2){
                auto dic1 = _dict(arr->objectAtIndex(0));
                id = dic1->valueForKey("id")->getCString();
                name = CCCommonUtils::getNameById(id);
                cell = MailItemCell::create(name,CCCommonUtils::getIcon(id).c_str());
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(362, 0);
                }
                else
                    cell->setPosition(151, 0);
                bgCell->m_bg->addChild(cell);
                
                auto dic2 = _dict(arr->objectAtIndex(1));
                id = dic2->valueForKey("id")->getCString();
                name = CCCommonUtils::getNameById(id);
                cell = MailItemCell::create(name,CCCommonUtils::getIcon(id).c_str());
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(922, 0);
                }
                else
                    cell->setPosition(384.0, 0);
                bgCell->m_bg->addChild(cell);
                i = 2;
            }else{
  
                int numPerRow = 2;
                int cellY = 0;
                int col = 0;
                CCObject *obj;
                CCARRAY_FOREACH(arr, obj){
                    col = i%2;
                    auto dic = _dict(arr->objectAtIndex(i));
                    id = dic->valueForKey("id")->getCString();
                    name = CCCommonUtils::getNameById(id);
                    cell = MailItemCell::create(name,CCCommonUtils::getIcon(id));
                    if (CCCommonUtils::isIosAndroidPad()) {
                        if(col==0){
                            cell->setPosition(362, cellY);
                        }else{
                            cell->setPosition(922, cellY);
                        }
                    }
                    else {
                        if(col==0){
                            cell->setPosition(151, cellY);
                        }else{
                            cell->setPosition(384, cellY);
                        }
                    }

                    bgCell->m_bg->addChild(cell);
                    i++;
                    if(i % numPerRow == 0){
                        cellY += cellH;
                    }
                }
            }
            int addH = cellH * int((arr->count() + 1) / 2);
            if(addH > 0){
                addH -= 75;
            }
            bgCell->m_bg->setContentSize(CCSize(bgCell->m_bg->getContentSize().width, bgCell->m_bg->getContentSize().height+addH));
            bgCell->m_bg->setPositionY(bgCell->m_bg->getPositionY()-addH);
//            bgCell->m_kuangbg->setContentSize(CCSize(bgCell->m_kuangbg->getContentSize().width, bgCell->m_kuangbg->getContentSize().height+addH));
//            bgCell->m_kuangbg->setPositionY(bgCell->m_kuangbg->getPositionY()-addH);
            m_totalH -= addH;
        }
    }
}

void DetectMailPopUpView::addTalentInfo(){
    if(m_info->detectReport->objectForKey("talent")){
        auto arr = dynamic_cast<CCArray*>(m_info->detectReport->objectForKey("talent"));
        if(arr && arr->count() > 0){
            auto bg = CCLoadSprite::createScale9Sprite("rect_1.png");
            this->m_scrollView->addChild(bg);
            bg->setAnchorPoint(ccp(0, 1));
            bg->setPosition(ccp(30, m_totalH));
            int startY = m_totalH;

            int i = 0;
            float w = this->m_listContainer->getContentSize().width;
            int numPerRow = 2;
            int cellH = 115;
            if (CCCommonUtils::isIosAndroidPad()) {
                cellH = 276;
            }
            while(i < arr->count()){
                if(i % numPerRow == 0){
                    m_totalH -= cellH;
                }

                CCDictionary *d = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
                auto cell = DetectSiceneInfoCell::create(d);
                cell->setAnchorPoint(ccp(0.5, 1));
                int posX = (i % numPerRow == 0 ? w / 4 - 50 : w * 3 / 4 - 50);
                if (CCCommonUtils::isIosAndroidPad()) {
                    posX *= 2.4;
                }
                cell->setPosition(ccp(posX, m_totalH));
                this->m_listNode->addChild(cell);
                i++;
                
            }
            if (CCCommonUtils::isIosAndroidPad()) {
                bg->setContentSize(CCSize(1474, startY - m_totalH + 5));
            }
            else
                bg->setContentSize(CCSize(614, startY - m_totalH + 5));
            m_totalH -= 30;
        }
    }
}

void DetectMailPopUpView::addAllianceHelpInfo(){
    if(m_info->detectReport->objectForKey("rein_about_detail")){
        if(!m_info->detectReport->objectForKey("rein_about")){
            CCDictionary *dict = CCDictionary::create();
            CCArray *arr = dynamic_cast<CCArray*>(m_info->detectReport->objectForKey("rein_about_detail"));
            bool about = false;
            int total = 0;
            CCObject *obj;
            CCARRAY_FOREACH(arr, obj) {
                CCArray *soldierArr = dynamic_cast<CCArray*>(obj);
                CCObject *soldierObj;
                CCARRAY_FOREACH(soldierArr, soldierObj){
                    CCDictionary *soldier =_dict(soldierObj);
                    about = soldier->valueForKey("about")->boolValue();
                    total += soldier->valueForKey("count")->intValue();
                }
            }
            dict->setObject(CCString::create(CC_ITOA(about)), "about");
            dict->setObject(CCString::create(CC_ITOA(total)), "total");
            m_info->detectReport->setObject(dict, "rein_about");
        }
    }
    if(m_info->detectReport->objectForKey("rein_about")){
        auto dict = _dict(m_info->detectReport->objectForKey("rein_about"));
        int total = dict->valueForKey("total")->intValue();
        bool about = dict->valueForKey("about")->boolValue();
        
        std::string title = "";
        DetectBgCell* bgCell = DetectBgCell::create(title);
        std::string numStr = CC_CMDITOA(total);
        if(about){
            numStr = std::string("~") + CC_CMDITOA(total);
        }
        bgCell->setNumTitle(_lang("105531"), numStr);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_totalH -= cellHdGapY;
        }
        else
            m_totalH -= cellGapY;
        m_totalH -= bgCell->getBgCellHeight();
        bgCell->setPosition(0, m_totalH);

        this->m_listNode->addChild(bgCell);
        
        CCArray *arr = NULL;
        CCArray *arr1 = NULL;
        if(m_info->detectReport->objectForKey("rein_detail")){
            arr = dynamic_cast<CCArray*>(m_info->detectReport->objectForKey("rein_detail"));
        }
        if(m_info->detectReport->objectForKey("rein_about_detail")){
            arr1 = dynamic_cast<CCArray*>(m_info->detectReport->objectForKey("rein_about_detail"));
        }
        int addH = 0;

        if(arr && arr->count() > 0){
            addH = 0;
            int index = 0;
            if (CCCommonUtils::isIosAndroidPad()) {
                addH -= 268;
                m_totalH += 268;
            }
            else {
                addH -= 112;
                m_totalH += 112;
            }
            while (index < arr->count()) {
                auto player = _dict(arr->objectAtIndex(index));
                CCArray *soldier = NULL;
                if(arr1 != NULL){
                    soldier = dynamic_cast<CCArray*>(arr1->objectAtIndex(index));
                }
                auto cell = AllianceHelpCell::create(player, soldier);
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setScale(2.4);
                    cell->setPosition(ccp(0, m_totalH));
                }
                else
                    cell->setPosition(ccp(0, m_totalH));
                this->m_listNode->addChild(cell);
                m_totalH -= cell->getH();
                addH += cell->getH();
                index++;
            }
        }else{
            resetCellBG(bgCell);
//            bgCell->m_kuangbg->setVisible(false);
        }
        bgCell->m_bg->setContentSize(CCSize(bgCell->m_bg->getContentSize().width, bgCell->m_bg->getContentSize().height+addH));
        bgCell->m_bg->setPositionY(bgCell->m_bg->getPositionY() - addH);
//        bgCell->m_kuangbg->setContentSize(CCSize(bgCell->m_kuangbg->getContentSize().width, bgCell->m_kuangbg->getContentSize().height+addH));
//        bgCell->m_kuangbg->setPositionY(bgCell->m_kuangbg->getPositionY()-addH);
    }
}

CCLabelIF *DetectMailPopUpView::createLabel(int x, int y, std::string str, int fontSize){
    CCLabelIF *ret = CCLabelIF::create(str.c_str());
    ret->setFontSize(fontSize);
    ret->setPosition(ccp(x, y));
    return ret;
}

void DetectMailPopUpView::getData(){
    MailController::getInstance()->readMailFromServer(m_info->uid, CC_ITOA(m_info->type));
}

bool DetectMailPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailTitle", CCLabelIF*, this->m_mailTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shareBtn", CCControlButton*, this->m_shareBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnBtn", CCControlButton*, this->m_returnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnSpr", CCSprite*, this->m_returnSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_writeBtn", CCControlButton*, this->m_writeBtn);
    
    return false;
}

SEL_CCControlHandler DetectMailPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", DetectMailPopUpView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", DetectMailPopUpView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShareClick", DetectMailPopUpView::onShareClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturnClick", DetectMailPopUpView::onReturnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteClick", DetectMailPopUpView::onWriteClick);
    
    return NULL;
}
void DetectMailPopUpView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
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
void DetectMailPopUpView::onDeleteClick(CCObject *pSender, CCControlEvent event){
    if(m_info->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(DetectMailPopUpView::onOkDeleteMail)),true);

}

void DetectMailPopUpView::onShareClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent) {
    double gapTime = m_info->shareTime -GlobalData::shared()->getWorldTime();
    if (gapTime < 0) {
        CCLOG("MAIL TEST");
        CCDictionary *dict = dynamic_cast<CCDictionary*>(m_info->detectReport->objectForKey("user"));
        string  description1 = _lang("115297");
        string dialog = "115297";
        CCArray* msgArr = CCArray::create();
        if(dict){
            std::string nameStr = dict->valueForKey("name")->getCString();
            if(dict->objectForKey("abbr")){
                std::string asn = dict->valueForKey("abbr")->getCString();
                if(asn != ""){
                    nameStr = std::string("(") + asn + ")" + dict->valueForKey("name")->getCString();
                }
            }
            description1 = _lang_1("115297",nameStr.c_str());
            dialog = "115297";
            msgArr->addObject(CCString::create(nameStr));
        }
//        CountryChatCommand* cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND, description1.c_str(), CHAT_TYPE_DETECT_SHARE, "", dialog.c_str(), msgArr);
//        cmd->putParam("reportUid", CCString::create(m_info->uid));
//        cmd->sendAndRelease(); simon
        double shareTime = GlobalData::shared()->getWorldTime();
        shareTime = shareTime + 10*60 - 1;
        m_info->shareTime = shareTime;
    }else{
        int gap = gapTime/60 +1;
        if(gap<=0) gap = 1;
        CCCommonUtils::flyHint("", "", _lang_1("115276", CC_ITOA(gap)));
    }
}

void DetectMailPopUpView::onReturnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void DetectMailPopUpView::onWriteClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
}

void DetectMailPopUpView::onOkDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    PopupViewController::getInstance()->goBackPopupView();
}
//------------

DetectedByPlayerCell *DetectedByPlayerCell::create(MailInfo *dict){
    DetectedByPlayerCell *ret = new DetectedByPlayerCell(dict);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void DetectedByPlayerCell::cellTouch(CCTouch *pTouch){
    
}
void DetectedByPlayerCell::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
}

void DetectedByPlayerCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}







bool DetectedByPlayerCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(isTouchInside(this->m_headBG, pTouch)&&m_info->fromUid!=""&&m_info->fromName!=""){
        m_startPoint = pTouch->getLocation();
        return true;
    }
    return false;
}

void DetectedByPlayerCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(fabs(pTouch->getLocation().y - m_startPoint.y)>20||fabs(pTouch->getLocation().x - m_startPoint.x)>20){
        return;
    }
    if (isTouchInside(m_headBG,pTouch)) {
        if(m_info->uid==GlobalData::shared()->playerInfo.uid){
            PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(&GlobalData::shared()->playerInfo,1));
            return;
        }
        PlayerInfoBtnPopUpView* pop =PlayerInfoBtnPopUpView::create();
        pop->InitCommonBtns(m_info->fromName, m_info->fromUid);
        PopupViewController::getInstance()->addPopupView(pop, false);
    }
}
bool DetectedByPlayerCell::init(){
    
    auto bg = CCBLoadFile("NEW_DetectedByPlayerCell", this, this);
    this->setContentSize(bg->getContentSize());

    m_battlePicNode->removeAllChildrenWithCleanup(true);
    auto battlePic = CCLoadSprite::createSprite("Mail_zhencha_succeed.png");
    this->m_battlePicNode->addChild(battlePic);
    string picPath = m_info->pic;
    int picVer = m_info->picVer;
    string uid = m_info->fromUid;
    if(picPath==""){
        picPath = "g044.png";
    }else{
        picPath+=".png";
    }
    auto pic = CCLoadSprite::createSprite(picPath.c_str());
    float scale  = 70/pic->getContentSize().width;
    if(scale>1.0){
        scale = 1.0;
    }
    pic->setScale(scale);

//    this->m_playHeadNode->addChild(pic);
//    if (CCCommonUtils::isUseCustomPic(picVer))
//    {
//        m_headImgNode = HFHeadImgNode::create();
//        m_headImgNode->initHeadImgUrl2(m_playHeadNode, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 70, true);
//    }

    ////////融合底图
    auto sizeLayer = CCSize(80, 80);
    m_selfModelLayer = CCRenderTexture::create(sizeLayer.width, sizeLayer.height);
    m_selfModelLayer->setAnchorPoint(ccp(0.5, 0.5));
    ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
    auto spr = CCLoadSprite::createSprite("Mail_headBack.png");
    spr->setScale(1);
    spr->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
    auto bgCircle = CCLoadSprite::createSprite("Mail_head_backBattle.png");
    bgCircle->setBlendFunc(cbf);
    bgCircle->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
    pic->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 3));
    //    pic->removeFromParent();
    m_selfModelLayer->begin();
    spr->visit();
    pic->visit();
    bgCircle->visit();
    m_selfModelLayer->end();
    m_playHeadNode->addChild(m_selfModelLayer);
    if (CCCommonUtils::isUseCustomPic(picVer) && uid != "")
    {
        m_headImgNode = HFHeadImgNode::create();
        string backImg = "Mail_headBack.png";
        string renderImg = "Mail_head_backBattle.png";
//        m_headImgNode->initHeadImgUrl3(m_playHeadNode, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 74, true, ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 2), sizeLayer, backImg, renderImg);
    }
    
    if (m_info->detecterName != "") {
        this->m_nameText->setString(m_info->detecterName.c_str());
    }
    else {
        this->m_nameText->setString(m_info->detecterName.c_str());
        this->m_tipText->setPositionY(m_nameText->getPositionY());
    }
    
    this->m_tipText->setFntFile("Arial_Bold_Regular.fnt");
//    this->m_nameText->setFntFile("Arial_Bold_Regular.fnt");
    this->m_tipText->setColor(ccc3(85,62,31));
    this->m_tipText->setString(m_info->contents.c_str());
    return true;
}

bool DetectedByPlayerCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playHeadNode", CCNode*, this->m_playHeadNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipText", CCLabelIF*, this->m_tipText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerTitle", CCLabelIF*, this->m_playerTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headBG", CCSprite*, this->m_headBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, this->m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_PicNode", CCNode*, this->m_PicNode);
    return false;
}
//------------
//------------

DetectPlayerInfoCell *DetectPlayerInfoCell::create(CCDictionary *dict,int pointType,MailInfo* info){
    DetectPlayerInfoCell *ret = new DetectPlayerInfoCell(dict,pointType,info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void DetectPlayerInfoCell::cellTouch(CCTouch *pTouch){
    if(isTouchInside(m_posBG, pTouch)){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::stopReturnToChat();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//simon        ChatServiceCocos2dx::stopReturnToChat();
#endif
        int pos = m_dict->valueForKey("pointId")->intValue();
        WorldController::getInstance()->openTargetIndex = pos;
        CCPoint pt = WorldController::getPointByIndex(pos);
//        if (m_info && m_info->serverType>=SERVER_BATTLE_FIELD) {
//            pt = WorldController::getPointByMapTypeAndIndex(pos,(MapType)m_info->serverType);
//            if (GlobalData::shared()->serverType<=SERVER_TEST) {
//                YesNoDialog::show(_lang("140195").c_str(), NULL);
//                return ;
//            }
//        } simon
        if (m_dict->objectForKey("serverId")) {
            GlobalData::shared()->playerInfo.currentServerId = m_dict->valueForKey("serverId")->intValue();
        }
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
        }
        PopupViewController::getInstance()->removeAllPopupView();
        //zym 2015.12.11
       // PopupViewController::getInstance()->forceClearAll(true); simon
    }
}
bool DetectPlayerInfoCell::init(){

    CCLoadSprite::doResourceByCommonIndex(504, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
    });

    auto bg = CCBLoadFile("NEW_DetectPlayerCell", this, this);
    this->setContentSize(bg->getContentSize());
    m_towerTipTitle->setFntFile("Arial_Bold_Regular.fnt");
    m_playPoint->setFntFile("Arial_Bold_Regular.fnt");
    m_failText->setFntFile("Arial_Bold_Regular.fnt");
    if(m_dict!=NULL){
        std::string picUrl = "";
        std::string titile = "";
        switch (m_pointType) {
            case Throne:{
                titile = _lang("105708");
                picUrl = "Mail_darksoil.png";
                break;
            }
            case Trebuchet:{
                titile = _lang("105709");
                picUrl = "Mail_darksoil.png";
                break;
            }
            case Tile_allianceArea:{
                titile = _lang("115356");
                picUrl = "Mail_darksoil.png";
                break;
            }
            case tile_banner:{
                titile = _lang("115539");
                picUrl = "Mail_darksoil.png";
                break;
            }
            default:{
//                titile = _lang("105527");
                picUrl = "Mail_zhencha_succeed.png";
                break;
            }
        }
        m_battlePicNode->removeAllChildren();
        auto battlePic = CCLoadSprite::createSprite(picUrl.c_str());
        this->m_battlePicNode->addChild(battlePic);
        if (m_dict->objectForKey("noDef") && m_dict->valueForKey("noDef")->intValue() == 1) {//哨塔没有防守者的情况
            string picPath = "";
            if (m_dict->objectForKey("level")) {
                string picPath = "";
                if (m_pointType == Tile_allianceArea) {
                    picPath = "territory_building_lv0";
                }
                else if (m_pointType == tile_banner) {
                    picPath = "territory_tower0";
                }
                picPath.append(".png");
                auto pic = CCLoadSprite::createSprite(picPath.c_str());
                if (pic) {
                    float scale  = 70/pic->getContentSize().width;
                    if(scale>1.0){
                        scale = 1.0;
                    }
                    pic->setScale(scale);
//                    this->m_playHeadNode->addChild(pic);
                    
                    ////////融合底图
                    auto sizeLayer = CCSize(80, 80);
                    m_selfModelLayer = CCRenderTexture::create(sizeLayer.width, sizeLayer.height);
                    m_selfModelLayer->setAnchorPoint(ccp(0.5, 0.5));
                    ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
                    auto spr = CCLoadSprite::createSprite("Mail_headBack.png");
                    spr->setScale(1);
                    spr->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
                    auto bgCircle = CCLoadSprite::createSprite("Mail_head_backBattle.png");
                    bgCircle->setBlendFunc(cbf);
                    bgCircle->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
                    pic->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
                    //    pic->removeFromParent();
                    m_selfModelLayer->begin();
                    spr->visit();
                    pic->visit();
                    bgCircle->visit();
                    m_selfModelLayer->end();
                    m_playHeadNode->addChild(m_selfModelLayer);
                }
            }
            std::string nameStr = "";
            if(m_dict->objectForKey("abbr")){
                std::string asn = m_dict->valueForKey("abbr")->getCString();
                if(asn != ""){
                    nameStr.append("(");
                    nameStr.append(asn);
                    nameStr.append(")");
                }
            }
            std::string nameTemp = "";
            if (m_dict->objectForKey("name")) {
                nameTemp = m_dict->valueForKey("name")->getCString();
            }
            
            if (nameTemp != "") {
                nameStr.append(nameTemp);
            }
            else {
                if (m_info->pointType == Tile_allianceArea) {
                    if (m_dict->objectForKey("count")) {
                        int num = m_dict->valueForKey("count")->intValue();
                        nameStr.append(_lang_1("115312", CC_ITOA(num)));
                    }
                    else
                        nameStr.append(_lang_1("115312", CC_ITOA(1)));
                }
                else if (m_info->pointType == tile_banner) {
                    nameStr.append(_lang("115534"));
                }
            }
            
            this->m_playName->setString(nameStr.c_str());
            if(m_dict->objectForKey("pointId")){
                int pos = m_dict->valueForKey("pointId")->intValue();
                CCPoint pt = WorldController::getPointByIndex(pos);
//                if (m_info && m_info->serverType>=SERVER_BATTLE_FIELD) {
//                    pt = WorldController::getPointByMapTypeAndIndex(pos,(MapType)m_info->serverType);
//                }
//                std::string nameStrpos = string("X:") + CC_ITOA(pt.x)+"  "+string("Y:") + CC_ITOA(pt.y);
//                m_playPoint->setString(nameStrpos); simon
            }
            
//            this->m_playerTitle->setString(titile);
            this->m_failNode->setVisible(false);
            this->m_palyInfoNode->setVisible(true);
            this->m_playName->setVisible(true);
//            this->m_playLv->setVisible(true);
            this->m_playPoint->setVisible(true);
            this->m_towerTipTitle->setVisible(true);
            this->m_failText->setVisible(false);
            this->m_towerTipTitle->setString(_lang("105587").c_str());
        }
        else {
            string picPath = m_dict->valueForKey("pic")->getCString();
            int picVer = 0;
            if (m_dict->objectForKey("picVer")) {
                picVer = m_dict->valueForKey("picVer")->intValue();
            }
            string uid = m_dict->valueForKey("uid")->getCString();
            if(picPath==""){
                picPath = "g044.png";
            }else{
                picPath+=".png";
            }
            auto pic = CCLoadSprite::createSprite(picPath.c_str());
            float scale  = 70/pic->getContentSize().width;
            if(scale>1.0){
                scale = 1.0;
            }
            pic->setScale(scale);
//            this->m_playHeadNode->addChild(pic);
//            if (CCCommonUtils::isUseCustomPic(picVer))
//            {
//                m_headImgNode = HFHeadImgNode::create();
//                m_headImgNode->initHeadImgUrl2(m_playHeadNode, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 90, true);
//            }
            
            ////////融合底图
            auto sizeLayer = CCSize(80, 80);
            m_selfModelLayer = CCRenderTexture::create(sizeLayer.width, sizeLayer.height);
            m_selfModelLayer->setAnchorPoint(ccp(0.5, 0.5));
            ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
            auto spr = CCLoadSprite::createSprite("Mail_headBack.png");
            spr->setScale(1);
            spr->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
            auto bgCircle = CCLoadSprite::createSprite("Mail_head_backBattle.png");
            bgCircle->setBlendFunc(cbf);
            bgCircle->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
            pic->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 3));
            //    pic->removeFromParent();
            m_selfModelLayer->begin();
            spr->visit();
            pic->visit();
            bgCircle->visit();
            m_selfModelLayer->end();
            m_playHeadNode->addChild(m_selfModelLayer);
            if (CCCommonUtils::isUseCustomPic(picVer) && uid != "")
            {
                m_headImgNode = HFHeadImgNode::create();
                string backImg = "Mail_headBack.png";
                string renderImg = "Mail_head_backBattle.png";
//                m_headImgNode->initHeadImgUrl3(m_playHeadNode, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 74, true, ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 2), sizeLayer, backImg, renderImg);
            }
            
            std::string nameStr = m_dict->valueForKey("name")->getCString();
            if(m_dict->objectForKey("abbr")){
                std::string asn = m_dict->valueForKey("abbr")->getCString();
                if(asn != ""){
                    nameStr = std::string("(") + asn + ")" + m_dict->valueForKey("name")->getCString();
                }
            }
            this->m_playName->setString(nameStr.c_str());
            if(m_dict->objectForKey("lv")){
                int level = m_dict->valueForKey("lv")->intValue();
                std::string nameStr = string("Lv.") + CC_ITOA(level);
//                this->m_playLv->setString(nameStr);
            }
            if(m_dict->objectForKey("pointId")){
                int pos = m_dict->valueForKey("pointId")->intValue();
                CCPoint pt = WorldController::getPointByIndex(pos);
//                if (m_info && m_info->serverType>=SERVER_BATTLE_FIELD) {
//                    pt = WorldController::getPointByMapTypeAndIndex(pos,(MapType)m_info->serverType);
//                }
//                std::string nameStrpos = string("X:") + CC_ITOA(pt.x)+"  "+string("Y:") + CC_ITOA(pt.y);
//                m_playPoint->setString(nameStrpos); simon
            }
//            this->m_playerTitle->setString(titile);
            this->m_failNode->setVisible(false);
            this->m_palyInfoNode->setVisible(true);
            this->m_playName->setVisible(true);
//            this->m_playLv->setVisible(true);
            this->m_playPoint->setVisible(true);
            this->m_towerTipTitle->setVisible(true);
            this->m_failText->setVisible(false);
            this->m_towerTipTitle->setString(_lang("105587").c_str());
        }
    }else{
        this->m_failNode->setVisible(true);
        this->m_palyInfoNode->setVisible(false);
        this->m_playName->setVisible(false);
//        this->m_playLv->setVisible(false);
        this->m_playPoint->setVisible(false);
        this->m_failText->setVisible(true);
        this->m_failText->setString(_lang("114005").c_str());
    }
    
    this->m_towerTipTitle->setVisible(false);//去掉升级瞭望塔提示
    
    return true;
}

bool DetectPlayerInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playHeadNode", CCNode*, this->m_playHeadNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playPoint", CCLabelIF*, this->m_playPoint);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playName", Label*, this->m_playName);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playLv", CCLabelIF*, this->m_playLv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_failNode", CCNode*, this->m_failNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_palyInfoNode", CCNode*, this->m_palyInfoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG", CCNode*, this->m_posBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_failText", CCLabelIF*, this->m_failText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerTitle", CCLabelIF*, this->m_playerTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_towerTipTitle", CCLabelIF*, this->m_towerTipTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, this->m_battlePicNode);
    return false;
}
//------------

DetectGeneralInfoCell *DetectGeneralInfoCell::create(CCDictionary *dict){
    DetectGeneralInfoCell *ret = new DetectGeneralInfoCell(dict);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DetectGeneralInfoCell::init(){
    auto bg = CCBLoadFile("DetectGeneralInfoCellCCB", this, this);
    this->setContentSize(bg->getContentSize());
    std::string generalId = m_dict->valueForKey("genId")->getCString();
    int color = m_dict->valueForKey("color")->intValue();
    this->m_headContainer->addChild(GeneralHeadPic::create(generalId, color));
    this->m_nameText->setString(CCCommonUtils::getNameById(generalId).c_str());
    if(m_dict->objectForKey("level")){
        int level = m_dict->valueForKey("level")->intValue();
        this->m_levelText->setString(CC_ITOA(level));
        this->m_levelText->setColor(CCCommonUtils::getItemColor(color));
    }
    if(m_dict->objectForKey("num")){
        int num = m_dict->valueForKey("num")->intValue();
        auto icon = CCLoadSprite::createSprite(GeneralManager::getInstance()->getGeneralArmyIcon(generalId).c_str());
        icon->setScale(28/icon->getContentSize().width);
        this->m_armyIconContainer->addChild(icon);
        this->m_numText->setString(CC_CMDITOA(num));
    }

    return true;
}

bool DetectGeneralInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headContainer", CCNode*, this->m_headContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armyIconContainer", CCNode*, this->m_armyIconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelText", CCLabelIF*, this->m_levelText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText", CCLabelIF*, this->m_numText);
    return false;
}

//------------

DetectSiceneInfoCell *DetectSiceneInfoCell::create(CCDictionary *dict){
    DetectSiceneInfoCell *ret = new DetectSiceneInfoCell(dict);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DetectSiceneInfoCell::init(){
    auto bg = CCBLoadFile("DetectSiceneInfoCellCCB", this, this);
    this->setContentSize(bg->getContentSize());

    std::string sid = m_dict->valueForKey("id")->getCString();
    int level = m_dict->valueForKey("level")->intValue();
    std::string str = CCCommonUtils::getNameById(sid) + " LV " + CC_ITOA(level);
    this->m_nameText->setString(str.c_str());
    
    auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getIcon(sid).c_str());
    sprite->setScale(68/sprite->getContentSize().width);
    this->m_iconContainer->addChild(sprite);
    return true;
}

bool DetectSiceneInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconContainer", CCNode*, this->m_iconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    return false;
}

//--
DetectBgCell *DetectBgCell::create(std::string title){
    DetectBgCell *ret = new DetectBgCell(title);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DetectBgCell::init(){
    auto node = CCBLoadFile("NEW_DetectMailView", this, this);
    this->setContentSize(node->getContentSize());
    this->m_title->setString(m_titleStr.c_str());
    
    return true;
}

int DetectBgCell::getBgCellHeight(){
    int maxH = m_bg->getContentSize().height;
    return maxH;
}

void DetectBgCell::setBgHeight(float bgH){
    if(bgH > m_bg->getContentSize().height){
        int gap = bgH - m_bg->getContentSize().height;
        m_bg->setContentSize(CCSize(m_bg->getContentSize().width,bgH));
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            m_kuangbg->setContentSize(CCSize(m_kuangbg->getContentSize().width, m_kuangbg->getContentSize().height+gap / 2.0));
//        } else {
//            m_kuangbg->setContentSize(CCSize(m_kuangbg->getContentSize().width, m_kuangbg->getContentSize().height+gap));
//        }
        m_titleBG->setPositionY(m_titleBG->getPositionY()+gap);
        m_title->setPositionY(m_title->getPositionY()+gap);
        m_title1->setPositionY(m_title1->getPositionY()+gap);
        m_title2->setPositionY(m_title2->getPositionY()+gap);
    }
}

void DetectBgCell::setNumTitle(std::string str1, std::string str2){
    this->m_title->setString("");
    this->m_title1->setString(str1);
    this->m_title2->setString(str2);
}

void DetectBgCell::setTitle(std::string str1, std::string str2, std::string str3){
    this->m_title->setString(str1);
    this->m_title1->setString(str2);
    this->m_title2->setString(str3);
}

bool DetectBgCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleBG", CCScale9Sprite*, this->m_titleBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, this->m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title2", CCLabelIF*, this->m_title2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title1", CCLabelIF*, this->m_title1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head", CCNode*, this->m_head);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kuangbg", CCNode*, this->m_kuangbg);
    return false;
}

//--
MailItemCell *MailItemCell::create(std::string title,std::string icon,std::string id){
    MailItemCell *ret = new MailItemCell(title,icon,id);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailItemCell::init(){
    CCLoadSprite::doResourceByCommonIndex(5, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(5, false);
        
    });

    auto node = CCBLoadFile("MailItemRender", this, this);
    this->setContentSize(node->getContentSize());
    this->m_nameTxt->setString(m_titleStr.c_str());
    m_nameTxt->setFntFile("Arial_Bold_Regular.fnt");
    CCSprite* spr = CCLoadSprite::createSprite(m_iconPath.c_str());
    CCCommonUtils::setSpriteMaxSize(spr, 85);
    this->m_icon->addChild(spr);
    m_detectReport = NULL;
    m_generalInfo = NULL;
    if(m_id == ""){
        m_btn->setTouchEnabled(false);
    }
    return true;
}

void MailItemCell::clickAbilityHandle(CCObject *pSender, CCControlEvent event){
    if(m_id=="") return ;
    GeneralInfo *info = NULL;
    if(m_generalInfo!=NULL){
        info = m_generalInfo;
    }else if(m_detectReport!=NULL){
        info = new GeneralInfo(m_detectReport);
    }else{
        info = new GeneralInfo();
        info->setXMLGeneralInfo("240000");
    }
    MailBattleDetailView* view = dynamic_cast<MailBattleDetailView*>(PopupViewController::getInstance()->getCurrentPopupView());
    
    PopupViewController::getInstance()->addPopupInView(GeneralSkillListPopUpView::create(info,m_id));
    if(view){
        PopupViewController::getInstance()->removePopupView(view);
    }
}

bool MailItemCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCControlButton*, this->m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    return false;
}

SEL_CCControlHandler MailItemCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickAbilityHandle", MailItemCell::clickAbilityHandle);
    
    return NULL;
}

DetectRewardPreviewCell *DetectRewardPreviewCell::create(int type, int value, int num){
    DetectRewardPreviewCell *ret = new DetectRewardPreviewCell(type, value, num);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DetectRewardPreviewCell::setData(int type, int value, int num){
    m_type = type;
    m_value = value;
    m_num = num;
    refreshView();
}

bool DetectRewardPreviewCell::init(){
    refreshView();
    
    return true;
}

void DetectRewardPreviewCell::refreshView(){
    this->removeAllChildren();
    if(m_type == R_GOODS){
        if (CCCommonUtils::isIosAndroidPad())
        {
            CCCommonUtils::createGoodsIcon(m_value, this, CCSize(64, 64));
        } else {
            CCCommonUtils::createGoodsIcon(m_value, this, CCSize(37,37));
        }
//        auto& toolInfo = ToolController::getInstance()->getToolInfoById(m_value);
//        int color = toolInfo.color;
//        if (color>=0) {
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 46);
//            this->addChild(iconBg);
//        }
    }else{
        auto icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(m_type, m_value).c_str());
        this->addChild(icon);
        icon->setAnchorPoint(ccp(0.5, 0.5));
        if (CCCommonUtils::isIosAndroidPad())
        {
            CCCommonUtils::setSpriteMaxSize(icon, 64, true);
        } else {
            CCCommonUtils::setSpriteMaxSize(icon, 37);
        }
    }
    
    std::string namestr = RewardController::getInstance()->getNameByType(m_type, m_value);
    auto label = CCLabelIF::create();
    label->setFntFile("Arial_Bold_Regular.fnt");
    label->setFontSize(22);
    label->setColor(ccc3(130, 99, 56));
    label->setString(namestr.c_str());
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(40, 0);
    if (CCCommonUtils::isIosAndroidPad()) {
        label->setFontSize(53);
        label->setPositionX(96);
    }
    this->addChild(label);
    
    std::string numstr = "";
    if(m_type == R_GOODS){
        numstr.append(CC_CMDITOA(m_num));
    }else{
        numstr.append(CC_CMDITOA(m_value));
    }
    auto label1 = CCLabelIF::create();
    label1->setFntFile("Arial_Bold_Regular.fnt");
    label1->setFontSize(22);
    label1->setColor(ccc3(127, 35, 29));
    label1->setString(numstr.c_str());
    label1->setAnchorPoint(ccp(1.0, 0.5));
    label1->setPosition(560, 0);
    if (CCCommonUtils::isIosAndroidPad())
    {
        label1->setFontSize(53);
        label1->setPositionX(1350);
    }
    this->addChild(label1);
}

//-----

AllianceHelpCell *AllianceHelpCell::create(CCDictionary *dict, CCArray *arr){
    AllianceHelpCell *ret = new AllianceHelpCell();
    if(ret && ret->init(dict, arr)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

int AllianceHelpCell::getH(){
    if (CCCommonUtils::isIosAndroidPad()) {
        m_totalH *= 2.4;
    }
    return m_totalH;
}

bool AllianceHelpCell::init(CCDictionary *dict, CCArray *arr){
    m_totalH = 0;
    //基本信息
    std::string icon = dict->valueForKey("pic")->getCString();
    if(icon==""){
        icon = "g044";
    }
    auto pic = CCLoadSprite::createSprite((icon + ".png").c_str());
    float scale  = 70/pic->getContentSize().width;
    if(scale>1.0){
        scale = 1.0;
    }
    pic->setScale(scale);
    ////////融合底图
    auto sizeLayer = CCSize(80, 80);
    m_selfModelLayer = CCRenderTexture::create(sizeLayer.width, sizeLayer.height);
    m_selfModelLayer->setAnchorPoint(ccp(0.5, 0.5));
    ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
    auto spr = CCLoadSprite::createSprite("Mail_headBack.png");
    spr->setScale(1);
    spr->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
    auto bgCircle = CCLoadSprite::createSprite("Mail_head_backBattle.png");
    bgCircle->setBlendFunc(cbf);
    bgCircle->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
    pic->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 3));
    //    pic->removeFromParent();
    m_selfModelLayer->begin();
    spr->visit();
    pic->visit();
    bgCircle->visit();
    m_selfModelLayer->end();

    m_selfModelLayer->setPosition(320, -65);
    this->addChild(m_selfModelLayer);
    
    auto nameLabel = CCLabelIFTTF::create();
    nameLabel->setColor(ccc3(47, 22, 0));
    nameLabel->setFontSize(24);
    
    string levelstr="1";
    if(dict->valueForKey("level")&&dict->valueForKey("level")->getCString())
    {
        levelstr = dict->valueForKey("level")->getCString();
    }
    std::string tempStr = "";
    tempStr.append("Lv.").append(levelstr).append("  ");
    std::string nameStr = "";
    if(dict->objectForKey("abbr")){
        std::string asn = dict->valueForKey("abbr")->getCString();
        if(asn != ""){
            nameStr.append("(").append(asn).append(")");
        }
    }
    if (dict->objectForKey("name")) {
        std::string name = dict->valueForKey("name")->getCString();
        if (name != "") {
            nameStr.append(name);
        }
    }
    tempStr.append(nameStr);
    nameLabel->setString(tempStr.c_str());
    this->addChild(nameLabel);
    nameLabel->setAnchorPoint(ccp(0.5, 0.5));
    nameLabel->setPosition(ccp(m_selfModelLayer->getPositionX(), m_selfModelLayer->getPositionY() - 65));
    
    m_totalH += 160;
    
    if(arr){
        //部队信息
        auto line = CCLoadSprite::createScale9Sprite("Mail_line_02.png");
        this->addChild(line);
        line->setPreferredSize(CCSize(560, 2));
        line->setAnchorPoint(ccp(0, 0.5));
        line->setPosition(ccp(40, -m_totalH));
        m_totalH += 25;
    
        int index = 0;
        
        CCObject *obj;
        int i = 0;
        int numPerRow = 1;
        int cellH = 37;
        int cellY = this->getContentSize().height-40;
        CCARRAY_FOREACH(arr, obj){
            if(i % numPerRow == 0){
                cellY -= cellH;
            }
            auto dict = dynamic_cast<CCDictionary*>(obj);
            if(dict){
                std::string id = dict->valueForKey("armyId")->getCString();
                int num = dict->valueForKey("count")->intValue();
                bool about = dict->valueForKey("about")->boolValue();
                auto cell = DetectArmyCell::create(id, num,true);
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setScale(0.42);
                }
                cell->setAnchorPoint(ccp(0, 0));
                int posX = 0;
                m_totalH += 110;
                cell->setPosition(ccp(posX,  -m_totalH));
                this->addChild(cell);
                i++;
            }
        }
        
//        while (index < arr->count()) {
//            CCDictionary *soldierDict = _dict(arr->objectAtIndex(index++));
//            std::string armyId = soldierDict->valueForKey("armyId")->getCString();
//            int num = soldierDict->valueForKey("count")->intValue();
//            bool about = soldierDict->valueForKey("about")->boolValue();
//            std::string numStr = "";
//            if(about){
//                numStr = std::string("~") + CC_CMDITOA(num);
//            }else{
//                numStr = CC_CMDITOA(num);
//            }
//            
//            auto armyNameLabel = CCLabelIF::create(CCCommonUtils::getNameById(armyId).c_str());
//            armyNameLabel->setColor(ccc3(91, 74, 59));
//            armyNameLabel->setFontSize(22);
//            auto numLabel = CCLabelIF::create(numStr.c_str());
//            numLabel->setColor(ccc3(91, 74, 59));
//            numLabel->setFontSize(22);
//            this->addChild(armyNameLabel);
//            this->addChild(numLabel);
//            armyNameLabel->setAnchorPoint(ccp(0, 1));
//            armyNameLabel->setPosition(ccp(sprite->getPositionX(), -m_totalH));
//            numLabel->setAnchorPoint(ccp(1, 1));
//            numLabel->setPosition(ccp(550, armyNameLabel->getPositionY()));
//            m_totalH += 25;
//        }
        m_totalH += 5;
    }
    
    return true;
}

bool AllianceHelpCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvText", CCLabelIF*, this->m_lvText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    return false;
}