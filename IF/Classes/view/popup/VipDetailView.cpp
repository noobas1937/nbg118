//
//  VipDetailView.cpp
//  IF
//
//  Created by chenliang on 14-3-18.
//
//

#include "VipDetailView.h"

#include "SoundController.h"
#include "PopupViewController.h"
#include "VipDetailCell.h"
#include "LocalController.h"
#include "VipUtil.h"
#include "StoreView.h"
#include "TipsView.h"
#include "ParticleController.h"
#include "UseToolView.h"
#include "ToolController.h"
#include "HFHeadImgNode.h"

//#define VIP_PROGRESS_BAR_WIDTH (CCCommonUtils::isIosAndroidPad()? 1113 : 540)
#define VIP_PROGRESS_BAR_WIDTH 540

const float VipDetailView::CELL_MARGIN = 20;
const int VipDetailView::MAX_CELL_NEEDED = 3;

VipDetailView::VipDetailView():m_touchState(0)
{
}

bool VipDetailView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLvText", CCLabelIFBMFont*, this->m_vipLvText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLeftTimeText", CCLabelIF*, this->m_vipLeftTimeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginDayText", CCLabelIF*, this->m_loginDayText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_currentVIPText", CCLabelIFBMFont*, this->m_currentVIPText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_NextVIPText", CCLabelIFBMFont*, this->m_NextVIPText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressBarNode", CCNode*, this->m_progressBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipAnimNode", CCNode*, this->m_vipAnimNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picHeadNode", CCNode*, this->m_picHeadNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBG", CCScale9Sprite*, this->m_listBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pageFlag", CCSprite*, this->m_pageFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLastPointNumText", CCLabelIF*, this->m_vipLastPointNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipNextPointNumText", CCLabelIF*, this->m_vipNextPointNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipCurrentPointNumText", CCLabelIF*, this->m_vipCurrentPointNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desText", CCLabelIF*, this->m_desText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblVIP1", CCLabelIFBMFont*, this->m_lblVIP[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblVIP2", CCLabelIFBMFont*, this->m_lblVIP[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblVIP3", CCLabelIFBMFont*, this->m_lblVIP[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblVIP4", CCLabelIFBMFont*, this->m_lblVIP[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprVip", CCSprite*, this->m_sprVip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprActive1", CCSprite*, this->m_sprActive[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprActive2", CCSprite*, this->m_sprActive[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barTouchNode", CCNode*, this->m_barTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBar1", CCSprite*, this->m_sprBar[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBar2", CCSprite*, this->m_sprBar[1]);
    return true;
}

SEL_CCControlHandler VipDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", VipDetailView::onTipBtnClick);
    return NULL;
}
void VipDetailView::onTipBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    //CCCommonUtils::flyText("VIP Rule");
    string content = _lang("103031")+"\n"+"\n"+"\n"+"\n"+_lang("103032");
    PopupViewController::getInstance()->addPopupView(TipsVipView::create(_lang("103030"),content));
    
}
void VipDetailView::onAddPointBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    // PopupViewController::getInstance()->addPopupInView(StoreView::create());
    auto dict = CCDictionary::create();
    PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_VIP_PIC,dict,"103002"));
    
}
void VipDetailView::onActivityPointBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    auto dict = CCDictionary::create();
    PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_VIP_ACTIVITY,dict,"103004"));
    //PopupViewController::getInstance()->addPopupInView(StoreView::create());
}

VipDetailView* VipDetailView::create(){
    VipDetailView* ret = new VipDetailView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}

void VipDetailView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(VipDetailView::setVipProgress), VIP_REFRESH, NULL);
    setTitleName(_lang("103000"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2,false);
    this->schedule(schedule_selector(VipDetailView::onTimer),1.0);
    setVipProgress(NULL);
}

void VipDetailView::onExit(){
    setTouchEnabled(false);
    this->unschedule(schedule_selector(VipDetailView::onTimer));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, VIP_REFRESH);
    PopupBaseView::onExit();
}


bool VipDetailView::init(){
    
    bool ret = false;
    if(!PopupBaseView::init())
    {
       // CCBLoadFile("VipMain", this, this);
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(105, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(105, false);
    });

    m_ismoving = false;
    m_movedh = 0;
    m_data = CCArray::create();
    m_listNode = CCNode::create();
    auto bg = CCBLoadFile("VipMain", this, this);
    this->setContentSize(bg->getContentSize());
    setTitleName(_lang("103018"));
    int preHeight = this->m_buildBG->getContentSize().height;
    
    m_NextVIPText->setFntFile("pve_fnt_title.fnt");
    m_vipLvText->setFntFile("pve_fnt_title.fnt");
//    m_vipLeftTimeText->setFntFile("pve_fnt_title.fnt");
    m_currentVIPText->setFntFile("pve_fnt_title.fnt");
    int index = 0;
    while (index<4) {
        m_lblVIP[index]->setFntFile("pve_fnt_title.fnt");
        ++index;
    }
    m_lblVIP[0]->setString(_lang("103000").c_str());
    m_lblVIP[1]->setPositionX(m_lblVIP[0]->getPositionX() + m_lblVIP[0]->getContentSize().width * m_lblVIP[0]->getOriginScaleY());
    m_lblVIP[2]->setString(_lang("103000").c_str());
    m_loginDayText->setString(_lang("103044"));
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_buildBG->setContentSize(CCDirector::sharedDirector()->getWinSize());
        dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
   // this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height +dh));
    this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
    this->m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, m_listBG->getContentSize().height + dh));
    this->m_downNode->setPositionY(m_downNode->getPositionY() - dh);
    
//    m_tableView = CCTableView::create(this, CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height));
//    //tableView->setDelegate(this);
//    m_tableView->setDirection(kCCScrollViewDirectionHorizontal);
//    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    //m_tableView->setTouchPriority(Touch_Popup);
    //m_listContainer->addChild(m_tableView);
   // m_tableView->setPositionY(200);

    //m_listContainer->addChild(m_nodeDisplay);
   // m_nodeDisplay->setAnchorPoint(ccp(0,1.0));
    listContentSize =m_listContainer->getContentSize();
   // m_nodeDisplay->addChild(m_listNode);
    m_nodeprogressBar = CCClipNode::create(CCSize(VIP_PROGRESS_BAR_WIDTH,116));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_progressBarNode->setContentSize(CCSize(1113, 208));
    }
    m_nodeprogressBar->setAnchorPoint(ccp(0,0.5));
    //m_nodeprogressBar->setPositionY(29);
    this->m_progressBarNode->addChild(m_nodeprogressBar);
    m_sprBar[2] = CCLoadSprite::createSprite("vip_tianchng.png");
    m_sprBar[2]->setAnchorPoint(ccp(0.0, 0.5));
    m_sprBar[2]->setPositionY(29);
    m_nodeprogressBar->addChild(m_sprBar[2]);
    m_sprBar[3] = CCLoadSprite::createSprite("vip_tianchng.png");
    m_sprBar[3]->setFlipX(true);
    m_sprBar[3]->setAnchorPoint(ccp(0.0, 0.5));
    m_sprBar[3]->setPositionX(VIP_PROGRESS_BAR_WIDTH*0.5);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_sprBar[3]->setPositionX(1113 * 0.5);
        float scaleX = 1113 * 1.0 / VIP_PROGRESS_BAR_WIDTH;
        float scaleY = 208 * 1.0 / 116;
        m_sprBar[2]->setScaleX(scaleX);
        m_sprBar[2]->setScaleY(scaleY);
        m_sprBar[3]->setScaleX(scaleX);
        m_sprBar[3]->setScaleY(scaleY);
    }
    m_sprBar[3]->setPositionY(29);
    m_nodeprogressBar->addChild(m_sprBar[3]);
//    if (CCCommonUtils::isIosAndroidPad()) {
//        m_nodeprogressBar->setScaleX(1113 * 1.0 / VIP_PROGRESS_BAR_WIDTH);
//        m_nodeprogressBar->setScaleY(208 * 1.0 / 116);
//    }
    
    int w = m_listContainer->getContentSize().width;
    int h = m_listContainer->getContentSize().height;
    m_scroll = CCScrollView::create(CCSize(w, h));
    m_scroll->setAnchorPoint(ccp(0, 1));
    m_scroll->setTouchPriority(1);
    m_scroll->setDirection(kCCScrollViewDirectionVertical);
    m_listContainer->addChild(m_scroll);
    m_scroll->addChild(m_listNode);
    
    ret = true;
    refresh();
    return ret;
}

void VipDetailView::refresh(){
    m_picHeadNode->removeAllChildren();
    if (GlobalData::shared()->playerInfo.isUseCustomPic()){
        auto headImgNode = HFHeadImgNode::create();
        headImgNode->initHeadImgUrl2(m_picHeadNode, GlobalData::shared()->playerInfo.getCustomPicUrl(), 1.0f, 130, true);
    }else{
        CCSprite* head = CCLoadSprite::createSprite(GlobalData::shared()->playerInfo.getPic().c_str());
        m_picHeadNode->addChild(head);
        CCCommonUtils::setSpriteMaxSize(head, 130,true);
    }
    auto &playerInfo = GlobalData::shared()->playerInfo;
    if(playerInfo.vipEndTime>GlobalData::shared()->getWorldTime()){//vip有效,倒计时
        this->unschedule(schedule_selector(VipDetailView::onTimer));
        this->schedule(schedule_selector(VipDetailView::onTimer),1.0);
        onTimer(1.0);
    }else{
        m_vipLeftTimeText->setString(_lang("103011").c_str());
    }
    
    setVipProgress(NULL);
}
void VipDetailView::setVipProgress(CCObject* obj){
    auto &playerInfo = GlobalData::shared()->playerInfo;
    m_desText->setString(_lang_1("103045", CC_ITOA(playerInfo.nextDayLoginVipReward)));
    
    int vipLv = VipUtil::getVipLevel(playerInfo.vipPoints);
    m_vipLvText->setString(_lang_1("103001", CC_ITOA(vipLv)));
    if(vipLv>9){
        m_vipLevel = 9;
    }else{
        m_vipLevel = vipLv;
    }
    int currLvPoint = VipUtil::getVipItemValueByKey(CC_ITOA(m_vipLevel+7000-1), "point");
    int nextLvPoint = VipUtil::getVipItemValueByKey(CC_ITOA(m_vipLevel+7000), "point");
    m_vipLastPointNumText->setString("("+CC_CMDITOA(currLvPoint)+")");
    m_vipNextPointNumText->setString("("+CC_CMDITOA(nextLvPoint)+")");
    m_vipCurrentPointNumText->setString(CC_CMDITOA(playerInfo.vipPoints));
    
    this->m_currentVIPText->setString(CC_ITOA(m_vipLevel));
    this->m_lblVIP[1]->setString(CC_ITOA(m_vipLevel));
    this->m_NextVIPText->setString(CC_ITOA(m_vipLevel+1));
    this->m_lblVIP[3]->setString(CC_ITOA(m_vipLevel+1));
    m_lblVIP[2]->setPositionX(m_lblVIP[3]->getPositionX() - m_lblVIP[3]->getContentSize().width * m_lblVIP[3]->getOriginScaleY());
    
    m_listNode->removeChildByTag(1000);
    VipDetailCell* cell = VipDetailCell::create(m_vipLevel-1);
    m_listNode->addChild(cell, 0, 1000);
    this->m_listNode->setPositionY(-135);
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->m_listNode->setPositionY(-270);
    }
    int hg =cell->getHG();
    m_scroll->setContentSize(CCSize(m_listContainer->getContentSize().width, cell->getHG()));
    m_scroll->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (cell->getHG())));
    float dh = listContentSize.height-cell->getHG();
    int maxY = 0;
    maxY = maxY>dh?maxY:dh;
    m_minPoint.y = 0;
    m_maxPoint.y = maxY;
    refreshPageFlag();
    
    int point = playerInfo.vipPoints-currLvPoint;
    int needPoint =nextLvPoint-currLvPoint;
    if(needPoint==0){
        needPoint = 1.0;
    }
    float sacle =point/(needPoint*1.0);
    sacle = sacle>1.0?1.0:sacle;
    float total = VIP_PROGRESS_BAR_WIDTH * sacle;
    if (CCCommonUtils::isIosAndroidPad()) {
        total = 1113 * sacle;
    }
    m_nodeprogressBar->setContentSize(CCSize(total, 58));
//    if(playerInfo.vipPoints < nextLvPoint  && playerInfo.vipPoints > currLvPoint){
//        m_sprVip->setVisible(true);
//        m_vipCurrentPointNumText->setVisible(true);
        m_sprVip->setPositionX(m_progressBarNode->getPositionX() + total);
        m_vipCurrentPointNumText->setPositionX(m_sprVip->getPositionX());
//    }
    
    setBtnState();
}
void VipDetailView::onAddVipAnim(){
     m_vipAnimNode->removeAllChildren();
    ParticleController::initParticle();
    
//    auto newBatch = ParticleController::createParticleBatch();
    auto praticle1 = ParticleController::createParticle("VIPUI_1");
    praticle1->setScale(0.65);
    m_vipAnimNode->addChild(praticle1);
    auto praticle2 = ParticleController::createParticle("VIPUI_2");
    praticle2->setScale(0.65);
    m_vipAnimNode->addChild(praticle2);
    auto praticle3= ParticleController::createParticle("VIPUI_3");
    praticle3->setScale(0.65);
    m_vipAnimNode->addChild(praticle3);
    auto praticle4 = ParticleController::createParticle("VIPUI_4");
    praticle4->setScale(0.65);
    m_vipAnimNode->addChild(praticle4);
//    int dx = 10;
//    m_vipAnimNode->addChild(newBatch);
//    praticle1->setPosition(ccp(dx, 0));
//    praticle1->setAnchorPoint(ccp(0, 0));
//    praticle2->setPosition(ccp(dx, 0));
//    praticle2->setAnchorPoint(ccp(0, 0));
//    praticle3->setPosition(ccp(dx, 0));
//    praticle3->setAnchorPoint(ccp(0, 0));
//    praticle4->setPosition(ccp(dx, 0));
//    praticle4->setAnchorPoint(ccp(0, 0));

}
void VipDetailView::setBtnState(){
    m_sprActive[1]->stopAllActions();
    int dtime = GlobalData::shared()->playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
    if(dtime>0){
        int index = 0;
        while (index<2) {
            CCCommonUtils::setSpriteGray(m_sprActive[index], false);
            ++index;
        }
        CCActionInterval * rotateto1 = CCRotateTo::create(20, 720);
        CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
        CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
        m_sprActive[1]->runAction(rotateForever);
        //CCCommonUtils::setButtonTitle(m_activityPointBtn, _lang("103004").c_str());
        onAddVipAnim();
//        m_vipLeftTimeText->setColor({255,255,255});
//        m_vipLvText->setColor({255,255,255});
    }else{
        int index = 0;
        while (index<2) {
            CCCommonUtils::setSpriteGray(m_sprActive[index], true);
            ++index;
        }
        m_vipAnimNode->removeAllChildren();
        m_sprActive[1]->setRotation(30);
//        m_vipLeftTimeText->setColor({255,0,0});
    }
}
void VipDetailView::onTimer(float dt){

    int dtime = GlobalData::shared()->playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
    if(dtime<0){
        this->unschedule(schedule_selector(VipDetailView::onTimer));
        m_vipLeftTimeText->setString(_lang("103011").c_str());
        setBtnState();
    }else{
        string timestr = getTimeStr(dtime);
        m_vipLeftTimeText->setString(timestr.c_str());
    }
}
string VipDetailView::getTimeStr(int secs){
    int day = secs/(24*3600);
    secs = secs%(24*3600);
    int hour = secs/3600;
    secs = secs%(3600);
    int min = secs/60;
    secs = secs%(60);
    int sec = secs;
    std::string dayStr = _lang("105592");
    std::string hourStr = _lang("105591");
    std::string minStr = _lang("105590");
    string timeStr = "";
    if(day > 0){
//        timeStr = CC_ITOA(day)+_lang("103005")+" ";
        timeStr = CC_ITOA(day) + std::string(" d ");
    }
    if(hour<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(hour));
    }else{
        timeStr.append(CC_ITOA(hour));
    }
    timeStr.append(":");
    if(min<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(min));
    }else{
        timeStr.append(CC_ITOA(min));
    }
    timeStr.append(":");
    if(sec<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(sec));
    }else{
        timeStr.append(CC_ITOA(sec));
    }
    
    return timeStr;
}
CCSize VipDetailView::tableCellSizeForIndex(CCTableView *table, ssize_t idx) {
    return CCSize(280, 282);
};
CCSize VipDetailView::cellSizeForTable(CCTableView *table) {
    return CCSize(280, 280);
};

CCTableViewCell* VipDetailView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    //CCTableViewCell AchievementNewPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    int count = m_data->count();
    if(idx >= m_data->count()){
        return NULL;
    }
    VipDetailCell* cell = (VipDetailCell*)table->dequeueCell();
    //QuestInfo* info = (QuestInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(idx);
        }else{
            cell = VipDetailCell::create(idx);
          //  m_cells->addObject(cell);
        }
    }
    return cell;
}
ssize_t VipDetailView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

bool VipDetailView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!m_ismoving) {
        if(isTouchInside(m_sprActive[0], pTouch)){
            int index = 0;
            while (index<2) {
                m_sprActive[index]->setColor(ccGRAY);
                ++index;
            }
            m_touchState = 1;
            return true;
        }else if(isTouchInside(m_barTouchNode, pTouch)){
            int index = 0;
            while (index<4) {
                m_sprBar[index]->setColor(ccGRAY);
                ++index;
            }
            m_touchState = 2;
            return true;
        }
    }
    m_touchState = 0;
    m_touchPoint = pTouch->getLocation();
    if (!m_ismoving&&isTouchInside(m_listContainer,pTouch)){
       // m_ListPoint = m_listNode->getPosition();
        return true;
    }
    return false;
}
void VipDetailView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_touchState == 1){
        int index = 0;
        while (index<2) {
            m_sprActive[index]->setColor(ccWHITE);
            ++index;
        }
        onActivityPointBtnClick(NULL, Control::EventType::TOUCH_DOWN);
    }else if(m_touchState==2){
        int index = 0;
        while (index<4) {
            m_sprBar[index]->setColor(ccWHITE);
            //CCCommonUtils::setSpriteGray(m_sprBar[index], false);
            ++index;
        }
        onAddPointBtnClick(NULL, Control::EventType::TOUCH_DOWN);
    }else{
        if(m_ismoveX&&fabs(m_touchPoint.x - pTouch->getLocation().x)>10){
            int dx =-m_touchPoint.x +pTouch->getLocation().x;
            if(dx<0){
                if(m_vipLevel>=9){
                    m_ismoving = false;
                    m_ismoveX = false;
                    m_scroll->setTouchEnabled(true);
                }else{
                    m_vipLevel++;
                    PlayComeInFromRightAnim();
                }
                
            }else{
                if(m_vipLevel<=1){
                    m_ismoving = false;
                    m_ismoveX = false;
                    m_scroll->setTouchEnabled(true);
              
                }else{
                    m_vipLevel--;
                    PlayComeInFromLeftAnim();
                }
                
            }
        }else{
            m_ismoving = false;
            m_ismoveX = false;
            m_scroll->setTouchEnabled(true);
        }
    }
    m_touchState = 0;
   // m_ismoving = false;
}
void VipDetailView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_touchState == 1 || m_touchState == 2)
        return;
    
    float dx =pTouch->getLocation().x-m_touchPoint.x;
    float dy =pTouch->getLocation().y-m_touchPoint.y;
    if(fabs(dx)<10&&fabs(dy)<10){
        return;
    }
    
    if(fabs(dy)>=fabs(dx)){//竖向
        m_ismoveX = false;
       // m_tableView->setTouchEnabled(false);
//        float desY = m_ListPoint.y+dy;
//        if(desY<=m_minPoint.y){
//            desY = m_minPoint.y;
//        }
//        if(desY>=m_maxPoint.y){
//            desY = m_maxPoint.y;
//        }
//         m_listNode->setPositionY(desY);
    }else{
         m_ismoving = true;
            m_ismoveX = true;
        m_scroll->setTouchEnabled(false);

        if(dx<0){
//            if(m_vipLevel>=9){
//                m_ismoving = false;
//                m_ismoveX = false;
//                return;
//            }
//            m_vipLevel++;
        }
        if(dx>0){
//            if(m_vipLevel<=1){
//                m_ismoving = false;
//                m_ismoveX = false;
//                return;
//            }
//            m_vipLevel--;
        }
         //m_listNode->setTouchEnabled(true);
//        float desX = m_ListPoint.x+dx;
//        if(desX<=m_minPoint.x){
//            desX = m_minPoint.x;
//        }
//        if(desX>=m_maxPoint.x){
//            desX = m_maxPoint.x;
//        }
       // m_listNode->setPositionY(desX);
//         m_listNode->setPositionX(desX);
       // return;
    }
}
void VipDetailView::PlayComeInFromRightAnim(){
    PlayComeOutFromRightAnim();
    VipDetailCell* cell = VipDetailCell::create(m_vipLevel-1);
    m_listNode->addChild(cell, 100, 1001);
   // cell->setPositionY(listContentSize.height-cell->getHG());
    float dh = listContentSize.height-cell->getHG();
    int maxY = 0;
    maxY = maxY>dh?maxY:dh;
    m_minPoint.y = 0;
    m_maxPoint.y = maxY;
    
    float x = cell->getPositionX();
    float y = cell->getPositionY();
    cell->setPositionX(this->getPositionX()+cell->getContentSize().width);
    CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
    cell->runAction(CCSequence::create(
                                       CCEaseSineOut::create(userMove)
                                       , CCCallFunc::create(this, callfunc_selector(VipDetailView::ComeInAnimFinish))
                                       , NULL
                                       ));

}
void VipDetailView::ComeInAnimFinish(){
    
    
}
void VipDetailView::PlayComeOutFromRightAnim(){
    auto outNode = m_listNode->getChildByTag(1000);
    float x = outNode->getPositionX()-outNode->getContentSize().width;
    float y = outNode->getPositionY();
    //this->setPositionX(this->getPositionX()+this->getContentSize().width);
    CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
    outNode->runAction(CCSequence::create(
                                                  CCEaseSineOut::create(userMove)
                                                  , CCCallFunc::create(this, callfunc_selector(VipDetailView::ComeOutAnimFinish))
                                                  , NULL
                                                  ));
}
void VipDetailView::ComeOutAnimFinish(){
    VipDetailCell* outNode = dynamic_cast<VipDetailCell*>(m_listNode->getChildByTag(1001));
    
    m_listNode->removeChildByTag(1000);
    outNode->setTag(1000);
    if(m_movedh>0){
        outNode->setPositionY(outNode->getPositionY()-m_movedh);
        m_movedh = 0;
    }
    m_ismoving = false;
    m_scroll->setTouchEnabled(true);
    refreshPageFlag();
    m_scroll->setContentSize(CCSize(m_listContainer->getContentSize().width, outNode->getHG()));
    m_scroll->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (outNode->getHG())));
    
}
void VipDetailView::PlayComeInFromLeftAnim(){
    VipDetailCell* outNode = dynamic_cast<VipDetailCell*>(m_listNode->getChildByTag(1000));
    PlayComeOutFromLeftAnim();
    VipDetailCell* cell = VipDetailCell::create(m_vipLevel-1);
    m_listNode->addChild(cell, 100, 1001);
    //cell->setPositionY(listContentSize.height-cell->getHG());
    float dh = listContentSize.height-cell->getHG();
    int maxY = 0;
    maxY = maxY>dh?maxY:dh;
    m_minPoint.y = 0;
    m_maxPoint.y = maxY;
    if(cell->getHG()<outNode->getHG()){
        m_movedh =(outNode->getHG()-cell->getHG());
        cell->setPositionY(cell->getPositionY()+m_movedh);
    }
    float x = cell->getPositionX();
    float y = cell->getPositionY();
    cell->setPositionX(this->getPositionX()-cell->getContentSize().width);
    CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
    cell->runAction(CCSequence::create(
                                       CCEaseSineOut::create(userMove)
                                       , CCCallFunc::create(this, callfunc_selector(VipDetailView::ComeInAnimFinish))
                                       , NULL
                                       ));
    
}
void VipDetailView::PlayComeOutFromLeftAnim(){
    auto outNode = m_listNode->getChildByTag(1000);
    float x = outNode->getPositionX()+outNode->getContentSize().width;
    float y = outNode->getPositionY();
    //this->setPositionX(this->getPositionX()+this->getContentSize().width);
    CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
    outNode->runAction(CCSequence::create(
                                          CCEaseSineOut::create(userMove)
                                          , CCCallFunc::create(this, callfunc_selector(VipDetailView::ComeOutAnimFinish))
                                          , NULL
                                          ));
}
void VipDetailView::refreshPageFlag(){

    this->m_currentVIPText->setString(CC_ITOA(m_vipLevel));
    this->m_NextVIPText->setString(CC_ITOA(m_vipLevel+1));
    this->m_pageFlag->setPositionX(-114+29*(m_vipLevel-1));

}
