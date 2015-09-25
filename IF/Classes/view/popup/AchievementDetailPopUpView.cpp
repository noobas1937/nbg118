//
//  AchievementDetailPopUpView.cpp
//  IF
//
//  Created by lifangkai on 14-7-9.
//
//

#include "AchievementDetailPopUpView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "SoundController.h"
#include "DropRdCCB.h"
#include "QuestRewardCommand.h"
#include "GoldExchangeView.h"
#include "EquipmentController.h"

AchievementDetailPopUpView *AchievementDetailPopUpView::create(QuestInfo* quest){
    AchievementDetailPopUpView *ret = new AchievementDetailPopUpView(quest);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AchievementDetailPopUpView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchievementDetailPopUpView::refreshView), QUEST_STATE_UPDATE, NULL);
    this->refreshView(NULL);
}

void AchievementDetailPopUpView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_STATE_UPDATE);
    PopupBaseView::onExit();
}

bool AchievementDetailPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto bg = CCBLoadFile("AchievementInfoPopUpView", this, this);
        this->setContentSize(bg->getContentSize());
        setTitleName(_lang("107500"));
        setCleanFunction([](){
            CCLoadSprite::doResourceByGeneralIndex(101, false);
            CCLoadSprite::doResourceByGeneralIndex(102, false);
        });
        int preHeight = this->m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int dh = m_buildBG->getContentSize().height - preHeight;
        
        if (dh>0) {
            int num = dh/177+1;
            for (int i=0; i<num; i++) {
                CCSprite* pic1 = CCLoadSprite::createSprite("brown_bg4.png");
                //m_leftBianNode->addChild(pic1);
                pic1->setAnchorPoint(ccp(0.5, 1.0));
                pic1->setPositionY((i-1)*170);
                CCSprite* pic2 = CCLoadSprite::createSprite("brown_bg4.png");
             //   m_rightBianNode->addChild(pic2);
                pic2->setAnchorPoint(ccp(0.5, 1.0));
                pic2->setPositionY((i-1)*177);
                
            }
            m_rewardListNode->setContentSize(CCSize(m_rewardListNode->getContentSize().width,m_rewardListNode->getContentSize().height+dh));
        }
        this->m_downNode->setPositionY(m_downNode->getPositionY() - dh);
        //this->m_listNode->setPositionY(m_listNode->getPositionY() - dh);
        //this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
      //  this->m_rewardListBG->setContentSize(CCSize(m_rewardListBG->getContentSize().width, m_rewardListBG->getContentSize().height + dh));
        //this->m_rewardListBG->setPositionY(m_rewardListBG->getPositionY() - dh);
        //this->m_rewardListNode->setContentSize(CCSize(m_rewardListNode->getContentSize().width, m_rewardListNode->getContentSize().height + dh));
        m_data = CCArray::create();
        m_cells = CCArray::create();
     //   m_rewardListBG->setPositionY(m_rewardListBG->getPositionY()-10);
        m_nameTxt->setString(m_info->name);
        m_descTxt->setString(m_info->description);
        std::string str = CC_CMDITOA(m_info->curValue>m_info->maxValue?m_info->maxValue:m_info->curValue);
        str.append("/");
        str.append(CC_CMDITOA(m_info->maxValue));
        m_progressTxt->setString(str);
        m_rewardTitle->setString(_lang("107516"));
        int max = m_info->maxValue;
        max = MAX(max, m_info->curValue);
        float scale = m_info->curValue*1.0/max;
        scale = MIN(scale,1);
        m_progress->setScaleX(scale);
        
        m_tabView = CCMultiColTableView::create(this, m_rewardListNode->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(Touch_Popup);
        m_tabView->setPositionX(-CCDirector::sharedDirector()->getWinSize().width/2+m_rewardListNode->getContentSize().width/2);
        m_rewardListNode->addChild(m_tabView);

        refreshView(NULL);
        ret = true;
    }
    return ret;
}
void AchievementDetailPopUpView::onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if((m_info->itemId == "2900101" || m_info->itemId == "2900102") && m_info->state == ACCEPT)
    {
        int para = 1;
        if (m_info->itemId == "2900102") {//facebook like
            para = 2;
            GameController::getInstance()->gotoFaceBookLike();
        }
        else {//google 5星评价
            para = 1;
            GameController::getInstance()->updateVersion();
        }
        FSTaskCommand * cmd = new FSTaskCommand(para);
        cmd->sendAndRelease();
        if (m_info->itemId == "2900102") {
            CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("133109").c_str());//领奖按钮显示【完成】，dialog=133109
        }else{
            CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("107516").c_str());//rewardbutton文字变回原来的reward
        }

//        SoundController::sharedSound()->playEffects(Music_building_upgrade);
        PopupViewController::getInstance()->goBackPopupView();
        return;
    }
    
    if (m_info->itemId == "2900103" && m_info->state == ACCEPT) {
//        PopupViewController::getInstance()->goBackPopupView();
        PopupViewController::getInstance()->addPopupView(GoldExchangeView::create(),false);
        return;
    }
    
    int r_type = -1;
    if (m_info->rewardshow == "wood") {
        r_type = R_WOOD;
    }else if (m_info->rewardshow == "food") {
        r_type = R_FOOD;
    }else if (m_info->rewardshow == "iron") {
        r_type = R_IRON;
    }else if (m_info->rewardshow == "stone") {
        r_type = R_STONE;
    }else if (m_info->rewardshow == "gold") {
        r_type = R_GOLD;
    }
    if (r_type>-1) {
        auto dropCCB = DropRdCCB::create(r_type);
        if (CCCommonUtils::isIosAndroidPad())
        {
            dropCCB->setScale(2.f);
        }
        this->addChild(dropCCB);
    }
    
//    SoundController::sharedSound()->playEffects(Music_building_upgrade);
    QuestController::getInstance()->getReward(m_info->itemId);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("AC_reward"));
    PopupViewController::getInstance()->goBackPopupView();
}
void AchievementDetailPopUpView::refreshView(CCObject* obj){
    if(m_info->type==1){
        this->m_titleTxt->setString(_lang("107528"));
//        CCSprite* pic = CCLoadSprite::createSprite("general_102_1.png");
//        m_headNode->addChild(pic);
//        pic->setAnchorPoint(ccp(0.5, 0.5));
//        pic->setScaleX(-0.7);
//        pic->setScaleY(0.7);
//         pic->setPositionY(-30);
    }else if(m_info->type==2){
//        CCSprite* pic = CCLoadSprite::createSprite("general_102_2.png");
//        m_headNode->addChild(pic);
//        pic->setScale(0.7);
//        pic->setFlipX(true);
//        pic->setAnchorPoint(ccp(0.5, 0.5));
//        pic->setPositionY(-20);
        this->m_titleTxt->setString(_lang("107529"));
    }
    CCSprite* pic1 = NULL;
    if(m_info->icon1!=""){
        std::string _picname = m_info->icon1;
        if (CCCommonUtils::isIosAndroidPad())
        {
            std::vector<std::string> _strvet;
            CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
            _picname = _strvet[0] + "_hd." + _strvet[1];
            if (!CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str()))
            {
                _picname = m_info->icon1;
                m_headNode->setScale(2.f);
            }
        }

         pic1 = CCLoadSprite::createSprite(_picname.c_str());

        m_headNode->addChild(pic1);
        pic1->setAnchorPoint(ccp(0.5, 0.5));
    }
    if(m_info->icon2!=""){
        std::string _picname = m_info->icon2;
        if (CCCommonUtils::isIosAndroidPad())
        {
            std::vector<std::string> _strvet;
            CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
            _picname = _strvet[0] + "_hd." + _strvet[1];
            if (!CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str()))
            {
                _picname = m_info->icon2;
                m_headNode->setScale(2.f);
            }
        }

        CCSprite* pic = CCLoadSprite::createSprite(_picname.c_str());
        m_headNode->addChild(pic);
        
        pic->setAnchorPoint(ccp(0.5, 0.5));
        if(pic1!=NULL){
            pic->cocos2d::CCNode::setPosition(pic1->getContentSize().width/2-pic->getContentSize().width/2, -(pic1->getContentSize().height/2-pic->getContentSize().height/2));
        }
    }
//    }else if(m_info->type==3){
//        CCSprite* pic = CCLoadSprite::createSprite("general_102_3.png");
//        m_headNode->addChild(pic);
//        pic->setScaleX(-0.8);
//        pic->setScaleY(0.8);
//        pic->setPositionY(-20);
//        pic->setPositionX(20);
//        pic->setAnchorPoint(ccp(0.5, 0.5));
//    }else if(m_info->type==4){
//        CCSprite* pic = CCLoadSprite::createSprite("general_102_4.png");
//        m_headNode->addChild(pic);
//        pic->setScaleX(-0.8);
//        pic->setScaleY(0.8);
//        pic->setPositionY(-20);
//        pic->setAnchorPoint(ccp(0.5, 0.5));
//    }
    CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("107516").c_str());
    if(m_info->state==COMPLETE){
        m_rewardBtn->setEnabled(true);
    }else{
        m_rewardBtn->setEnabled(false);
    }
    if(m_info->curValue>=m_info->maxValue){
        m_rewardBtn->setEnabled(true);
    }
    
    m_tabView->reloadData();
    
    if (m_info->itemId == "2900101" || m_info->itemId == "2900102" || m_info->itemId == "2900103") {
        m_rewardBtn->setEnabled(true);
        if (m_info->state == ACCEPT) {
            CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("2000214").c_str());
        }else if (m_info->itemId == "2900102" && m_info->state == COMPLETE) {
            CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("133109").c_str());//领奖按钮显示【完成】，dialog=133109
        }
    }
}

bool AchievementDetailPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progressTxt", CCLabelIF*, this->m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameText", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardTitle", CCLabelIF*, this->m_rewardTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardBtn", CCControlButton*, this->m_rewardBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
   // CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardListNode", CCNode*, this->m_rewardListNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardListBG", CCScale9Sprite*, this->m_rewardListBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listNode", CCNode*, this->m_listNode);
   // CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_leftBianNode", CCNode*, this->m_leftBianNode);
   // CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rightBianNode", CCNode*, this->m_rightBianNode);
    
    return false;
}


SEL_CCControlHandler AchievementDetailPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardBtnClick", AchievementDetailPopUpView::onRewardBtnClick);
    return NULL;
}

void AchievementDetailPopUpView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize AchievementDetailPopUpView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad())
    {
        
        return CCSize(1536, 138);
    }
    
    return CCSize(240, 70);
}

CCTableViewCell* AchievementDetailPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    int num = m_info->reward->count();
    if(idx >= num){
        return NULL;
    }
    AchievementDetailRewardCell* cell = (AchievementDetailRewardCell*)table->dequeueGrid();
    if(idx < num){
        if(cell){
            cell->setData(m_info, idx);
        }else{
            cell = AchievementDetailRewardCell::create(m_info,idx);
            m_cells->addObject(cell);
        }
    }
    return cell;
}

ssize_t AchievementDetailPopUpView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    if(m_info->reward==NULL){
        return 0;
    }
    int num = m_info->reward->count();
    return num;
    //return m_data->count();
}

unsigned int AchievementDetailPopUpView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

AchievementDetailRewardCell *AchievementDetailRewardCell::create(QuestInfo* info,int index){
    AchievementDetailRewardCell *ret = new AchievementDetailRewardCell(info,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AchievementDetailRewardCell::setData(QuestInfo* info, int index){
    m_info = info;
    m_index = index;
    int num = m_info->reward->count();
    auto dic = dynamic_cast<CCDictionary*>(m_info->reward->objectAtIndex(m_index));
    int type = dic->valueForKey("type")->intValue();
    
    if (type == R_EQUIP) {
        int itemId = 0;
        auto dict = _dict(dic->objectForKey("value"));
        if (dict && dict->objectForKey("itemId")) {
            itemId = dict->valueForKey("itemId")->intValue();
        }
        if (itemId == 0) {
            return;
        }
        
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[itemId];
        string name = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
        string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        auto iconBg = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 60, true);
        m_picNode->addChild(iconBg);
        string picStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
        auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, 55, true);
        m_picNode->addChild(icon);
        m_nameText->setString(name);
        m_numText->setString("1");
    }else {
        int value = 0;
        if (type == R_GOODS) {
            int itemId = 0;
            auto dict = _dict(dic->objectForKey("value"));
            if (dict && dict->objectForKey("itemId")) {
                itemId = dict->valueForKey("itemId")->intValue();
            }
            if (itemId == 0) {
                return;
            }
            value = itemId;

        }else {
            value = dic->valueForKey("value")->intValue();
        }
        
        auto icon = CCLoadSprite::createScale9Sprite(RewardController::getInstance()->getPicByType(type, value).c_str());
        m_picNode->addChild(icon);
        icon->setAnchorPoint(ccp(0.5, 0.5));
        std::string str = CC_ITOA(value);
        this->m_nameText->setString(RewardController::getInstance()->getNameByType(type, value));
        this->m_numText->setString(str);
    }
}

void AchievementDetailRewardCell::onEnter(){
    CCNode::onEnter();
}

void AchievementDetailRewardCell::onExit(){
    CCNode::onExit();
}

bool AchievementDetailRewardCell::init(){
    auto bg = CCBLoadFile("QuestCellInfoTaskReward", this, this);
    this->setContentSize(bg->getContentSize());
    //  this->m_btnLabel->setString(_lang("107516"));
    // this->m_rewardTitle->setString(_lang("107501"));
    this->setData(m_info, m_index);
    return true;
}

bool AchievementDetailRewardCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numText", CCLabelIF*, this->m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picNode", CCNode*, this->m_picNode);
  // CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btn", CCControlButton*, this->m_btn);
    return false;
}

SEL_CCControlHandler AchievementDetailRewardCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}