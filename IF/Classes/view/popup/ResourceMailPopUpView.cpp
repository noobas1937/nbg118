//
//  ResourceMailPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-26.
//
//

#include "ResourceMailPopUpView.h"
#include "PopupViewController.h"
#include "MailController.h"
#include "RewardController.h"
#include "MailPopUpView.h"
#include "WorldController.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "MailSaveCommand.h"
#include "ChatServiceCocos2dx.h"

ResourceMailPopUpView *ResourceMailPopUpView::create(MailInfo *info){
    ResourceMailPopUpView *ret = new ResourceMailPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ResourceMailPopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ResourceMailPopUpView::refresh), MAIL_CONTENT_READ, NULL);
    setTitleName(m_info->fromName.c_str());
}

void ResourceMailPopUpView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_CONTENT_READ);
    this->m_battlePicNode->removeAllChildren();
    PopupBaseView::onExit();
}

bool ResourceMailPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setMailUuid(m_info->uid);
        auto bg = CCBLoadFile("ResourceMailCCB", this, this);
        this->setContentSize(bg->getContentSize());
//        CCLoadSprite::doResourceByCommonIndex(6, true);
//        setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(6, false);
//            
//        });
        setTitleName(_lang("105513"));
        this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());
        m_titleText->setString(_lang("105516").c_str());
        m_rewardTitle->setString(_lang("104904").c_str());
        int preHeight = this->m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int dh = m_buildBG->getContentSize().height - preHeight;
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
        
        m_listNode = CCNode::create();
        m_scrollView = CCScrollView::create(m_listContainer->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setAnchorPoint(ccp(0, 0));
        m_listContainer->addChild(m_scrollView);
        m_scrollView->addChild(m_listNode);
       // m_tabView = NULL;
        m_downNode->setPositionY(m_downNode->getPositionY() - dh);
        m_deleteBtnTitle->setString(_lang("108523").c_str());
        m_playerTitle->setString(_lang("105577").c_str());
        this->m_totalNode->removeChild(m_playerNode);
        this->m_totalNode->removeChild(m_rewardNode);
        m_totalH = 0;
        m_listNode->addChild(m_playerNode);
        m_playerNode->setAnchorPoint(ccp(0,0));
        m_playerNode->setPositionX(0);
        m_listNode->addChild(m_rewardNode);
        m_rewardNode->setAnchorPoint(ccp(0,0));
        m_rewardNode->setPositionX(0);
        m_totalH-=444;
        m_playerNode->setPositionY(m_totalH);
        m_totalH-=186;
        m_rewardNode->setPositionY(m_totalH);
        auto battlePic = CCLoadSprite::createSprite("Mail_caiji.png");
        this->m_battlePicNode->addChild(battlePic);
        if(!m_info->isReadContent){
            getData();
        }else{
            refresh();
            this->m_listNode->setPositionY(-m_totalH);
            m_scrollView->setContentSize(CCSize(m_listContainer->getContentSize().width, -m_totalH));
            m_scrollView->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (-m_totalH)));
        }
        CCPoint pos = WorldController::getPointByIndex(m_info->pointId);
        std::string strPos = "X:";
        //_lang("108534");
        strPos.append(CC_ITOA(pos.x));
        strPos.append(" ");
        strPos.append("Y:");
        strPos.append(CC_ITOA(pos.y));
        m_posTxt->setString(strPos.c_str());
       // m_btnPos->setOpacity(200);
        
        ret = true;
    }
    return ret;
}

bool ResourceMailPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardTitle", CCLabelIF*, this->m_rewardTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerTitle", CCLabelIF*, this->m_playerTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descriptionText", CCLabelIF*, this->m_descriptionText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battlePicNode", CCNode*, this->m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerNode", CCNode*, this->m_playerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_posTxt", CCLabelIF*, this->m_posTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardCellNode", CCNode*, this->m_rewardCellNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_posBG", CCNode*, this->m_posBG);
   // CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnPos", CCControlButton*, this->m_btnPos);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_deleteBtnTitle", CCLabelIF*, this->m_deleteBtnTitle);

//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_savespr", CCSprite*, this->m_savespr);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_unsavespr", CCSprite*, this->m_unsavespr);
    return false;
}

void ResourceMailPopUpView::onDeleteClick(CCObject *pSender, CCControlEvent event){
    if(m_info->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(ResourceMailPopUpView::onOkDeleteMail)),true);
}
void ResourceMailPopUpView::onOkDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    PopupViewController::getInstance()->goBackPopupView();

}
SEL_CCControlHandler ResourceMailPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", ResourceMailPopUpView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPosClick", ResourceMailPopUpView::onPosClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", ResourceMailPopUpView::onAddSaveClick);
    return NULL;
}
bool ResourceMailPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(this->m_listContainer, pTouch)){
        return true;
    }
    return false;
}

void ResourceMailPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void ResourceMailPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(this->m_listContainer, pTouch)){
        return;
    }
    int d = pTouch->getStartLocation().y - pTouch->getLocation().y;
    d = abs(d);
    if(d > 20){
        return;
    }
    if(isTouchInside(this->m_posBG, pTouch)){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::stopReturnToChat();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//simon        ChatServiceCocos2dx::stopReturnToChat();
#endif
        int pos = m_info->pointId;
        WorldController::getInstance()->openTargetIndex = pos;
        CCPoint pt = WorldController::getPointByIndex(pos);
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
        }
        //zym 2015.12.11
//        PopupViewController::getInstance()->forceClearAll(true);
        PopupViewController::getInstance()->removeAllPopupView();
        return;
    }
}
void ResourceMailPopUpView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
}

cocos2d::CCSize ResourceMailPopUpView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(600, 85);
}

CCTableViewCell* ResourceMailPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_info->reward->count()){
        return NULL;
    }
    CCDictionary *dict = dynamic_cast<CCDictionary*>(m_info->reward->objectAtIndex(idx));
    ResourceRewardPreviewCell* cell = (ResourceRewardPreviewCell*)table->dequeueGrid();
    int type = dict->valueForKey("t")->intValue();
    int value = dict->valueForKey("v")->intValue();
    if(idx < m_info->reward->count()){
        if(cell){
            cell->setData(type, value);
        }else{
            cell = ResourceRewardPreviewCell::create(type, value);
        }
    }
    return cell;
}

ssize_t ResourceMailPopUpView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_info->reward->count();
}

ssize_t ResourceMailPopUpView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}
void ResourceMailPopUpView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
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
        //CCCommonUtils::setButtonTitle(m_addSaveBtn, "save");
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        // CCCommonUtils::setButtonTitle(m_addSaveBtn, "unsave");
       // m_saveBtnTitle->setString(_lang("105573").c_str());
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
}
void ResourceMailPopUpView::onPosClick(CCObject *pSender, CCControlEvent event){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//simon    ChatServiceCocos2dx::stopReturnToChat();
#endif
    int pos = m_info->pointId;
    WorldController::getInstance()->openTargetIndex = pos;
    CCPoint pt = WorldController::getPointByIndex(pos);
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    //zym 2015.12.11
//    PopupViewController::getInstance()->forceClearAll(true);
    PopupViewController::getInstance()->removeAllPopupView();
}

void ResourceMailPopUpView::refresh(CCObject * p){
    if(m_info->save==0){
        //CCCommonUtils::setButtonTitle(m_addSaveBtn, "save");
        //        m_saveBtnTitle->setString(_lang("105574").c_str());
        m_addSaveBtn->setVisible(false);
        m_unSaveBtn->setVisible(true);
    }else{
        m_addSaveBtn->setVisible(true);
        m_unSaveBtn->setVisible(false);
    }
//    if(m_tabView == NULL){
//        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
//        m_tabView->setDirection(kCCScrollViewDirectionVertical);
//        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
//        m_tabView->setMultiColTableViewDelegate(this);
//        m_tabView->setTouchPriority(1);
//        m_listContainer->addChild(m_tabView);
//    }
//    m_tabView->setTouchEnabled(m_info->reward->count() > 1);
//    m_tabView->reloadData();
    int count =m_info->reward->count();
    if(count> 0){
        auto dict = _dict(m_info->reward->objectAtIndex(0));
        int type = dict->valueForKey("t")->intValue();
        std::string nameStr = string("Lv.") + CC_ITOA(m_info->resLevel) + " " + CCCommonUtils::getResourceProductByType(type);
        m_nameText->setString(nameStr.c_str());
        m_descriptionText->setString(_lang_1("105517", nameStr.c_str()));
        
        for (int i = 0;i<count; i++) {
            CCDictionary *dict = dynamic_cast<CCDictionary*>(m_info->reward->objectAtIndex(i));
            int type = dict->valueForKey("t")->intValue();
            int value = dict->valueForKey("v")->intValue();
            ResourceRewardPreviewCell*  cell = ResourceRewardPreviewCell::create(type, value);
            this->m_rewardCellNode->addChild(cell);
            cell->setPositionY(-30-i*50);
            
        }
        
    }else{
        this->m_rewardNode->setVisible(false);
    }
    CCPoint pos = WorldController::getPointByIndex(m_info->pointId);
    std::string strPos = _lang("108534");
    strPos.append(CC_ITOA(pos.x));
    strPos.append(",");
    strPos.append(CC_ITOA(pos.y));
    m_posTxt->setString(strPos.c_str());
    m_posBG->setPositionX(m_nameText->getPositionX()+m_nameText->getContentSize().width+10);
    
}

void ResourceMailPopUpView::getData(){
    MailController::getInstance()->readMailFromServer(m_info->uid, CC_ITOA(m_info->type));
}

void ResourceMailPopUpView::doWhenClose(){

}

//-----------

ResourceRewardPreviewCell *ResourceRewardPreviewCell::create(int type, int num){
    ResourceRewardPreviewCell *ret = new ResourceRewardPreviewCell(type, num);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ResourceRewardPreviewCell::setData(int type, int num){
    m_type = type;
    m_num = num;
    refreshView();
}

bool ResourceRewardPreviewCell::init(){
    refreshView();
    
    return true;
}

void ResourceRewardPreviewCell::refreshView(){
    this->removeAllChildren();
    
//    auto bg = CCLoadSprite::createSprite("UI_frame_ziyuan.png");
//    bg->setPosition(ccp(15, 5));
//    bg->setAnchorPoint(ccp(0, 0));
//    this->addChild(bg);
//    
//    auto bg2 = CCLoadSprite::createSprite("icon_kuang_ziyuan.png");
//    bg2->setPosition(ccp(0, -8));
//    bg2->setAnchorPoint(ccp(0, 0));
//    this->addChild(bg2);
    
    auto icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(m_type, m_num).c_str());
    this->addChild(icon);
    icon->setAnchorPoint(ccp(0.5, 0.5));
    icon->setPosition(ccp(0, 30));
    CCCommonUtils::setSpriteMaxSize(icon, 50);
    
    std::string namestr = RewardController::getInstance()->getNameByType(m_type);
    auto label = CCLabelIF::create();
    label->setFontSize(22);
    label->setColor(ccc3(130, 99, 56));
    label->setString(namestr.c_str());
    label->setAnchorPoint(ccp(0, 0.5));
    //int width =bg->getContentSize().width/2;
    label->setPosition(40, icon->getPositionY());
    this->addChild(label);
    
    std::string numstr = " + ";
    numstr.append(CC_CMDITOA(m_num));
    auto label1 = CCLabelIF::create();
    label1->setFontSize(22);
    label1->setColor(ccc3(127, 35, 29));
    label1->setString(numstr.c_str());
    label1->setAnchorPoint(ccp(1.0, 0.5));
    label1->setPosition(435, icon->getPositionY());
    this->addChild(label1);
}