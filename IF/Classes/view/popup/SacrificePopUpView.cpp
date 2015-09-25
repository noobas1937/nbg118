//
//  SacrificePopUpView.cpp
//  IF
//
//  Created by lifangkai on 14-11-19.
//
//

#include "SacrificePopUpView.h"
#include "SceneController.h"
#include "PopupViewController.h"
#include "CCTypesExt.h"
#include "CCCommonUtils.h"
#include "ChatController.h"
#include "CCSafeNotificationCenter.h"
#include "CCLabelIF.h"
#include "ChatCell.h"
#include "YesNoDialog.h"
#include "ChatLockCommand.h"
#include "ChatUnLockCommand.h"
#include "MailWritePopUpView.h"
#include "GetUserInfoCommand.h"
#include "../../Ext/CCDevice.h"
#include "RoleInfoView.h"
#include "UIComponent.h"
#include "CCEditText.h"
#include "InvitesAllianceCommand.h"
#include "InputFieldMultiLine.h"
#include "JoinAllianceView.h"
#include "AlertAddAllianceView.h"
#include "CheckAllianceInfoView.h"
#include "MailController.h"
#include "MailDeleteCommand.h"
#include "MailReadCommand.h"
#include "PlayerInfoBtnPopUpView.h"
#include "Utf8Utils.h"
#include "RewardController.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "SacrificeCommand.h"
#include "FunBuildController.h"
#include "TipsView.h"
#include "DropSacrificeAnim.h"
#include "CCShake.h"
#include "FBUtilies.h"
#include "ActivityController.h"

#define Sacrifice_Cell_Height 230
#define Sacrifice_Cell_HD_Height 450

SacrificePopUpView* SacrificePopUpView::create( ){
    SacrificePopUpView* ret = new SacrificePopUpView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SacrificePopUpView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_isInit = true;
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(6, false);
        
    });
    auto tmpCCB = CCBLoadFile("SacrificeCCB",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int preHeight = this->m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int dh = m_buildBG->getContentSize().height - preHeight;
        this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dh));
        
        auto battlePic = CCLoadSprite::createSprite("The-fountain_tupian.png");
        this->m_battlePicNode->addChild(battlePic);
    }

    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    
    m_infoList->addChild(m_tabView);
    int count = (m_buildBG->getContentSize().height)/100+1;
    for (int i=0; i<count; i++) {
        auto pic = CCLoadSprite::createSprite("technology_09.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            pic->setScaleX(2.51);
        }
        pic->setPositionY(-i*100);
        this->m_bgNode->addChild(pic);
    }
    m_getNumText->setVisible(false);
    addCommonAnim();
    getData();
    return true;
}
//首次打开面板引导
CCNode* SacrificePopUpView::getGuideNode(){
    auto & listArr = m_tabView->getContainer()->getChildren();
    for (auto child : listArr)
    {
        SacrificeCell* cell = dynamic_cast<SacrificeCell*>(child);
        if(cell){
            if(GlobalData::shared()->resourceInfo.lWood>GlobalData::shared()->resourceInfo.lFood){
                return cell->m_cellNode->getChildByTag(100);
            }else{
                return cell->m_cellNode->getChildByTag(100);
            }
            
        }
    }
//    CCObject* obj;
//    CCARRAY_FOREACH(listArr, obj){
//        SacrificeCell* cell = dynamic_cast<SacrificeCell*>(obj);
//        if(cell){
//            if(GlobalData::shared()->resourceInfo.lWood>GlobalData::shared()->resourceInfo.lFood){
//                return cell->m_cellNode->getChildByTag(100);
//            }else{
//                return cell->m_cellNode->getChildByTag(100);
//            }
//            
//        }
//    }
    return NULL;
}
void SacrificePopUpView::getData(){
    GameController::getInstance()->showWaitInterface();
    SacrificeGetCommand* cmd = new SacrificeGetCommand();
    cmd->sendAndRelease();
}
void SacrificePopUpView::refresh(CCObject* p){
    SacrificeInfo& info = GlobalData::shared()->sacrificeInfo;
    if(m_isInit&&info.isFirstFree==1){
        auto size = CCDirector::sharedDirector()->getWinSize();
        PopupViewController::getInstance()->addPopupView(TipsGuideView::create(_lang("102330"),size.height-700));
        m_isInit = false;
        
    }
    bool bInit = false;
    if(m_data->count()>0){
        bInit = true;
    }
    m_data->removeAllObjects();
    m_data->addObject(CCInteger::create(0));
    m_data->addObject(CCInteger::create(1));
    if(info.isSteelOpen==true){
        m_data->addObject(CCInteger::create(2));
    }
    float needH = m_data->count() * Sacrifice_Cell_Height;
    if (CCCommonUtils::isIosAndroidPad()) {
        needH = m_data->count() * Sacrifice_Cell_HD_Height;
    }
    if(m_infoList->getContentSize().height < needH){
        m_tabView->setTouchEnabled(true);
    }else{
        bInit = false;
        m_tabView->setTouchEnabled(false);
    }
    
    CCPoint tabOffset = m_tabView->getContentOffset();
    m_tabView->reloadData();
    if(bInit){
        m_tabView->setContentOffset(tabOffset);
    }
    string des="";
    if(info.canPray()){
        int leftFree = info.getLeftFreeCount();
        if(leftFree>0){
            des = _lang("102324");
            des+=":";
            des+=CC_CMDITOA(leftFree);
        }else{
            des = "";
        }
    }else{
        des = _lang("102325");
        if (CCCommonUtils::isIosAndroidPad()) {
            m_desText->setFontSize(45);
        }
        else
            m_desText->setFontSize(22);
        int leftFree = info.getLeftFreeCount();
        if(leftFree>0){
            CCSequence *seq = CCSequence::create(CCFadeIn::create(0.5),CCDelayTime::create(5),CCFadeOut::create(0.5),CCCallFunc::create(this, callfunc_selector(SacrificePopUpView::showNextTip)),NULL);
            m_desText->setAnchorPoint(ccp(0.5, 0.5));
            this->m_desText->setString(des);
            this->m_desText->runAction(seq);
            return;
        }
    }
    m_desText->setAnchorPoint(ccp(0.5, 0.5));
    this->m_desText->setString(des);
}
void SacrificePopUpView::showNextTip(){
    string des="";
    string text = m_desText->getString();
    if(text.compare(_lang("102325")) == 0){
        SacrificeInfo& info = GlobalData::shared()->sacrificeInfo;
        int leftFree = info.getLeftFreeCount();
        if(leftFree>0){
            des = _lang("102324");
            des+=":";
            des+=CC_CMDITOA(leftFree);
        }
    }else{
        des = _lang("102325");
    }
    if(!des.empty()){
        this->m_desText->setString(des);
        CCSequence *seq = CCSequence::create(CCFadeIn::create(0.5),CCDelayTime::create(5),CCFadeOut::create(0.5),CCCallFunc::create(this, callfunc_selector(SacrificePopUpView::showNextTip)),NULL);
        this->m_desText->runAction(seq);
    }
}
void SacrificePopUpView::refreashData(CCObject* obj){
    CCDictionary* dict = dynamic_cast<CCDictionary*>(obj);
    m_num = dict->valueForKey("num")->intValue();
    m_hint = dict->valueForKey("hint")->intValue();
    m_resource = dict->valueForKey("resource")->intValue();
    addRewardPoolAnim();
    m_getNumText->stopAllActions();
    m_getNumText->setVisible(true);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_getNumText->setScale(0.2);
    }
    else
        m_getNumText->setScale(0.1);
    m_getNumText->setString(CC_CMDITOA(m_num).c_str());
    m_getNumText->setOpacity(255);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(SacrificePopUpView::GetNumAnimEnd));

//    CCScaleTo* iconRotate = CCScaleTo::create(0.125f, 1.3);
//    CCScaleTo* iconRotate1 = CCScaleTo::create(0.125f, 0.9);
//    CCScaleTo* iconRotate2 = CCScaleTo::create(0.125f, 1.1);

    if (CCCommonUtils::isIosAndroidPad()) {
        CCScaleTo* iconRotate3 = CCScaleTo::create(0.2f, 2.4);
        CCActionInterval * delatTime = CCDelayTime::create(0.65);
        CCEaseSineIn* iconRotate4=  CCEaseSineIn::create((CCActionInterval*)(iconRotate3));
        CCSequence* action  =CCSequence::create(iconRotate4,NULL);
        m_getNumText->runAction(CCSequence::create(action, delatTime, funcall,NULL));
    }
    else {
        CCScaleTo* iconRotate3 = CCScaleTo::create(0.2f, 1.2);
        CCActionInterval * delatTime = CCDelayTime::create(0.65);
        CCEaseSineIn* iconRotate4=  CCEaseSineIn::create((CCActionInterval*)(iconRotate3));
        CCSequence* action  =CCSequence::create(iconRotate4,NULL);
        m_getNumText->runAction(CCSequence::create(action, delatTime, funcall,NULL));
    }
   
    m_getNumNode->removeAllChildren();
    for (int i=6; i<10; i++) {
        string path = CCString::createWithFormat("WishPoolReward_%s",CC_ITOA(i))->getCString();
        auto particle = ParticleController::createParticle(path);
        m_getNumNode->addChild(particle);
    }
    
    m_hintNode->removeAllChildren();
    if(m_hint>1){
        std::string numStr = "X2";
        string hintPath = "scrifice_hint_2.png";
        if(m_hint==5){
            numStr = "X5";
            hintPath = "scrifice_hint_5.png";
        }else if(m_hint==10){
            numStr = "X10";
            hintPath = "scrifice_hint_10.png";
        }
//        m_HintSpr = CCLoadSprite::createSprite(hintPath.c_str());
        m_HintSpr = CCLabelIFBMFont::create(numStr.c_str(), "pve_fnt_boss.fnt");
        m_hintNode->addChild(m_HintSpr);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_HintSpr->setScale(10);
            m_HintSpr->setOpacity(0);
            CCCallFunc * funcall1= CCCallFunc::create(this, callfunc_selector(SacrificePopUpView::EndHintAnim1));
            CCScaleTo* iconRotate1 = CCScaleTo::create(0.2f, 2.0);
            CCActionInterval * delatTime1 = CCDelayTime::create(0.25);
            m_HintSpr->runAction(CCSequence::create(delatTime1,funcall1,NULL));
        }
        else {
            m_HintSpr->setScale(5);
            m_HintSpr->setOpacity(0);
            CCCallFunc * funcall1= CCCallFunc::create(this, callfunc_selector(SacrificePopUpView::EndHintAnim1));
            CCScaleTo* iconRotate1 = CCScaleTo::create(0.2f, 1.0);
            CCActionInterval * delatTime1 = CCDelayTime::create(0.25);
            m_HintSpr->runAction(CCSequence::create(delatTime1,funcall1,NULL));
        }
    }

    refresh(NULL);
    if (FBUtilies::fbIsLogin() && m_hint>=5) {
        string key = "wishing_five";
        if (m_hint==10) {
            key = "wishing_ten";
        }
        CCArray* arr = CCArray::create();
        arr->addObject(CCString::create(CC_ITOA(m_hint)));
        ActivityController::getInstance()->facebookPostMethod(key,arr);
//        double lastTime = 0.0;
//        string timeStr = CCUserDefault::sharedUserDefault()->getStringForKey("wishingTimeRecord","");
//        if (timeStr!="") {
//            lastTime = atof(timeStr.c_str());
//        }
//        std:: map<string, CCDictionary*>::iterator it = GlobalData::shared()->shareFbmap.find(key);
//        auto ctime = GlobalData::shared()->getWorldTime();
//        if (it!=GlobalData::shared()->shareFbmap.end()) {
//            CCDictionary* dict = it->second;
//            if (dict) {
//                int on = dict->valueForKey("ON")->intValue();
//                double limit = dict->valueForKey("Limit")->doubleValue();
//                if (lastTime > 0) {
//                    lastTime = lastTime + limit;
//                }
//                string permission = dict->valueForKey("permissions")->getCString();
//                if (on==1  && ctime >= lastTime && FBUtilies::fbHasGranted("publish_actions")) {
//                    CCUserDefault::sharedUserDefault()->setStringForKey("wishingTimeRecord", CC_ITOA(ctime));
//                    CCUserDefault::sharedUserDefault()->flush();
//                    string name = _lang_1(dict->valueForKey("name")->getCString(),CC_ITOA(m_hint));
//                    string caption = _lang_1(dict->valueForKey("caption")->getCString(),CC_ITOA(m_hint));
//                    string linkDescription = _lang_1(dict->valueForKey("linkDescription")->getCString(),CC_ITOA(m_hint));
//                    string pictureUrl = dict->valueForKey("pictureUrl")->getCString();
//                    string link = dict->valueForKey("link")->getCString();
//                    string ref = dict->valueForKey("ref")->getCString();
//                    FBUtilies::fbPublishFeedDialog(name,caption,linkDescription,link,pictureUrl,1,ref);
//                }
//            }
//        }
    }
}

void SacrificePopUpView::EndHintAnim1(){
    if(m_HintSpr){
        m_HintSpr->stopAllActions();
        m_HintSpr->setOpacity(255);
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCallFunc * funcall1= CCCallFunc::create(this, callfunc_selector(SacrificePopUpView::EndHintAnim));
            CCScaleTo* iconRotate1 = CCScaleTo::create(0.2f, 2.0);
            CCActionInterval * delatTime1 = CCDelayTime::create(0.25);
            m_HintSpr->runAction(CCSequence::create(iconRotate1,delatTime1,funcall1,NULL));
        }
        else {
            CCCallFunc * funcall1= CCCallFunc::create(this, callfunc_selector(SacrificePopUpView::EndHintAnim));
            CCScaleTo* iconRotate1 = CCScaleTo::create(0.2f, 1.0);
            CCActionInterval * delatTime1 = CCDelayTime::create(0.25);
            m_HintSpr->runAction(CCSequence::create(iconRotate1,delatTime1,funcall1,NULL));
        }
    }
}
void SacrificePopUpView::EndHintAnim(){
    
    CCActionInterval * delatTime1 = CCDelayTime::create(0.05);
    CCCallFunc * funcall1= CCCallFunc::create(this, callfunc_selector(SacrificePopUpView::addShakeAnim));
    m_HintSpr->stopAllActions();
    m_HintSpr->runAction(CCSequence::create(delatTime1,funcall1,CCFadeOut::create(0.1),NULL));
    if(m_hint==5||m_hint==10){
        addScienceOutAnim();
        if(m_hint==10){
            addLvUptAnim();
        }
    }
//    m_getNumText->setVisible(false);
//    m_hintNode->removeAllChildren();
}
void SacrificePopUpView::addScienceOutAnim(){
    string tmpStart = "ScienceOut_";
    int count = 9;
    for (int i=1; i<count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPositionX(20);
        m_hintNode->addChild(particle);
    }
    
}
void SacrificePopUpView::addLvUptAnim(){
    auto praticle = ParticleController::createParticle(CCString::createWithFormat("LevelUp_%d",1)->getCString());
    m_hintNode->addChild(praticle);
    for (int i=3; i<8; i++) {
        auto praticle1 = ParticleController::createParticle(CCString::createWithFormat("LevelUp_%d",i)->getCString());
        m_hintNode->addChild(praticle1);
    }
}

void SacrificePopUpView::GetNumAnimEnd(){
//    m_getNumText->setOpacity(255);
//    CCFadeOut* fadeOut= CCFadeOut::create(0.2);
    m_getNumText->runAction(CCSequence::create(CCFadeOut::create(0.2),CCHide::create(),NULL));
    if(m_HintSpr){
        CCFadeOut* fadeOut1= CCFadeOut::create(0.1);
        m_HintSpr->runAction(fadeOut1);
    }
//     m_getNumText->setVisible(false);
//     m_hintNode->removeAllChildren();
}

void SacrificePopUpView::onEnter()
{
    CCNode::onEnter();
    setTitleName("");
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SacrificePopUpView::refresh), SACRIFICE_GET, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SacrificePopUpView::refreashData), SACRIFICE_BUY, NULL);

//    GuideController::share()->setGuide("3000100");
}

void SacrificePopUpView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SACRIFICE_GET);
       CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SACRIFICE_BUY);
    this->m_desText->stopAllActions();
    PopupBaseView::onExit();
}



SEL_CCControlHandler SacrificePopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", SacrificePopUpView::onTipBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MailMonsterListView::onDeleteClick);
    return NULL;
}

bool SacrificePopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintNode", CCNode*, this->m_hintNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getNumNode", CCNode*, this->m_getNumNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dropNode", CCNode*, this->m_dropNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftNode1", CCNode*, this->m_leftNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftNode2", CCNode*, this->m_leftNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightNode1", CCNode*, this->m_rightNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightNode2", CCNode*, this->m_rightNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middleNode", CCNode*, this->m_middleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middleRewardNode", CCNode*, this->m_middleRewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, this->m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desText", CCLabelIF*, this->m_desText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getNumText", CCLabelBMFont*, this->m_getNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    return false;
}
void SacrificePopUpView::addCommonAnim(){
    m_leftNode1->removeAllChildren();
    m_leftNode2->removeAllChildren();
    m_rightNode1->removeAllChildren();
    m_rightNode2->removeAllChildren();
    m_middleNode->removeAllChildren();
    auto particle = ParticleController::createParticle("WishPool_L");
    auto particle1 = ParticleController::createParticle("WishPool_L");
    m_leftNode1->addChild(particle);
    m_rightNode1->addChild(particle1);
    auto particle2 = ParticleController::createParticle("WishPool_R");
    auto particle3 = ParticleController::createParticle("WishPool_R");
    m_leftNode2->addChild(particle2);
    m_rightNode2->addChild(particle3);
    auto particle4 = ParticleController::createParticle("WishPool_M");
    m_middleNode->addChild(particle4);
    
}
void SacrificePopUpView::addRewardPoolAnim(){
    m_middleRewardNode->removeAllChildren();
    for (int i=1; i<6; i++) {
        string path = CCString::createWithFormat("WishPoolReward_%s",CC_ITOA(i))->getCString();
        auto particle = ParticleController::createParticle(path);
        m_middleRewardNode->addChild(particle);
    }
    addDropAnim();

}
void SacrificePopUpView::addDropAnim(){
    m_dropNode->removeAllChildren();
    auto anim = DropSacrificeAnim::create(m_resource, m_hint);
    m_dropNode->addChild(anim);
    if(m_resource == Silver){
        std::string show = CC_CMDITOA(m_num * m_hint);
        show = _lang_1("102364", show.c_str());
        CCCommonUtils::flyHintWithDefault("","",show.c_str());
    }
}
void SacrificePopUpView::addShakeAnim(){
//    CCShake* shake = CCShake::create(0.5, 5);
//    this->runAction(shake);
    CCScaleTo* iconRotate3 = CCScaleTo::create(0.1f, 1.03);
    CCEaseSineIn* iconRotate4=  CCEaseSineIn::create((CCActionInterval*)(iconRotate3));
    CCScaleTo* iconRotate1 = CCScaleTo::create(0.1f, 1.0);
    CCEaseSineIn* iconRotate2=  CCEaseSineIn::create((CCActionInterval*)(iconRotate1));
    CCSequence* action  =CCSequence::create(iconRotate4,iconRotate2,NULL);
    if(m_totalNode){
        m_totalNode->runAction(action);
    }else{
        this->runAction(action);
    }
}
void SacrificePopUpView::onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102327")));
}
void SacrificePopUpView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

CCSize SacrificePopUpView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        int height = Sacrifice_Cell_HD_Height;
        return CCSize(1496,height);
    }
    else {
        int height = Sacrifice_Cell_Height;
        return CCSize(604,height);
    }
}

cocos2d::CCSize SacrificePopUpView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1496, Sacrifice_Cell_HD_Height);
    }
    return CCSize(604, Sacrifice_Cell_Height);
}

CCTableViewCell* SacrificePopUpView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx>=m_data->count()){
        return NULL;
    }
    CCInteger* id = dynamic_cast<CCInteger*>(m_data->objectAtIndex(idx));
    
    SacrificeCell* cell = (SacrificeCell*)table->dequeueCell();
    if(cell){
        cell->setData(id->getValue());
    }else{
        cell = SacrificeCell::create(id->getValue());
    }
    return cell;
}

ssize_t SacrificePopUpView::numberOfCellsInTableView(CCTableView *table){
    int cellNum = 0;
    cellNum = m_data->count();
    return cellNum;
}


SacrificeCell* SacrificeCell::create(int type){
    SacrificeCell* ret = new SacrificeCell(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SacrificeCell::init()
{
    bool ret = true;
    m_cellNode = CCNode::create();
    this->addChild(m_cellNode);

    setData(m_type);
    return ret;
}

void SacrificeCell::setData(int type)
{

    m_type = type;
    this->m_cellNode->removeAllChildren();
    if(m_type==0){
        auto cell = SacrificeOneCell::create(Wood);
        m_cellNode->addChild(cell);
        cell->setTag(99);
        auto cell1 = SacrificeOneCell::create(Food);
        m_cellNode->addChild(cell1);
        if (CCCommonUtils::isIosAndroidPad()) {
            cell1->setPositionX(748);
        }
        else
            cell1->setPositionX(319);
        cell1->setTag(100);
    }else if(m_type==1){
        auto &info = GlobalData::shared()->sacrificeInfo;
        if(info.isSteelOpen==true){
            int mlv = FunBuildController::getInstance()->getMainCityLv();
            if (mlv>=FunBuildController::getInstance()->building_base_k3){
                auto cell = SacrificeOneCell::create(Iron);
                m_cellNode->addChild(cell);
                cell->setTag(99);
                if (mlv>=FunBuildController::getInstance()->building_base_k4) {
                    auto cell1 = SacrificeOneCell::create(Stone);
                    m_cellNode->addChild(cell1);
                    if (CCCommonUtils::isIosAndroidPad()) {
                        cell1->setPositionX(748);
                    }
                    else
                        cell1->setPositionX(319);
                    cell1->setTag(100);
                }else{
                    auto cell1 = SacrificeOneCell::create(Silver);
                    m_cellNode->addChild(cell1);
                    if (CCCommonUtils::isIosAndroidPad()) {
                        cell1->setPositionX(748);
                    }
                    else
                        cell1->setPositionX(319);
                    cell1->setTag(100);
                }
            }else{
                auto cell = SacrificeOneCell::create(Silver);
                m_cellNode->addChild(cell);
                cell->setTag(99);
                auto cell1 = SacrificeOneCell::create(Iron);
                m_cellNode->addChild(cell1);
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell1->setPositionX(748);
                }
                else
                    cell1->setPositionX(319);
                cell1->setTag(100);
            }
        }else{
            auto cell = SacrificeOneCell::create(Iron);
            m_cellNode->addChild(cell);
            cell->setTag(99);
            auto cell1 = SacrificeOneCell::create(Stone);
            m_cellNode->addChild(cell1);
            if (CCCommonUtils::isIosAndroidPad()) {
                cell1->setPositionX(748);
            }
            else
                cell1->setPositionX(319);
            cell1->setTag(100);
        }
    }else if(m_type==2){
        int mlv = FunBuildController::getInstance()->getMainCityLv();
        if (mlv>=FunBuildController::getInstance()->building_base_k4){
            auto cell = SacrificeOneCell::create(Silver);
            m_cellNode->addChild(cell);
            cell->setTag(99);
        }else{
            auto cell = SacrificeOneCell::create(Stone);
            m_cellNode->addChild(cell);
            cell->setTag(99);
        }
    }

}

void SacrificeCell::onEnter(){
    CCNode::onEnter();
}

void SacrificeCell::onExit(){
    CCNode::onExit();
}

SEL_CCControlHandler SacrificeCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteBtnClick", MailResourceCell::onDeleteBtnClick);
    return NULL;
}

bool SacrificeCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    return false;
}


//.victory kill
SacrificeOneCell* SacrificeOneCell::create(int type){
    SacrificeOneCell* ret = new SacrificeOneCell(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SacrificeOneCell::init()
{
    bool ret = true;
    CCBLoadFile("SacrificeCell",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCSizeMake(690, Sacrifice_Cell_HD_Height));
    }
    else
        this->setContentSize(CCSizeMake(290, Sacrifice_Cell_Height));
    setData(m_type);
    return ret;
}

void SacrificeOneCell::setData(int type)
{
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface= NULL;
    }
    m_type = type;
    m_clickText->setString(_lang("102323"));

    SacrificeInfo& info = GlobalData::shared()->sacrificeInfo;
    string picPath = "";
   int mlv = FunBuildController::getInstance()->getMainCityLv();

    int gold = 0;
    if(m_type==Wood){
        m_getNumText->setString(CC_CMDITOA(info.wood));
        picPath="The-fountain_icon1.png";
        gold = info.goldwood;
    }else if(m_type==Stone){
        m_getNumText->setString(CC_CMDITOA(info.silver));
        gold = info.goldsilver;
        picPath="The-fountain_icon4.png";
        if (mlv>=FunBuildController::getInstance()->building_base_k4) {
            m_openNode->setVisible(true);
            m_closeDesText->setVisible(false);
        }
        else {
            m_openNode->setVisible(false);
            m_closeDesText->setVisible(true);
            m_closeDesText->setString(_lang_1("102299", CC_ITOA(FunBuildController::getInstance()->building_base_k4)));
        }
    }else if(m_type==Iron){
        m_getNumText->setString(CC_CMDITOA(info.iron));
        gold = info.goldiron;
        picPath="The-fountain_icon3.png";
        if (mlv>=FunBuildController::getInstance()->building_base_k3) {
            m_openNode->setVisible(true);
            m_closeDesText->setVisible(false);
        }
        else {
            m_openNode->setVisible(false);
            m_closeDesText->setVisible(true);
            m_closeDesText->setString(_lang_1("102299", CC_ITOA(FunBuildController::getInstance()->building_base_k3)));
        }
    }else if(m_type==Food){
        m_getNumText->setString(CC_CMDITOA(info.food));
        gold = info.goldfood;
        picPath="The-fountain_icon2.png";
    }else if(m_type == Silver){
        m_getNumText->setString(CC_CMDITOA(info.steel));
        gold = info.goldsteel;
        picPath="The-fountain_icon5.png";
    }
    string costStr = _lang("102326");
    costStr.append(CC_CMDITOA(gold));
    m_costNumText->setString(costStr);
    if(info.canFreePray()){
        m_costNumText->setVisible(false);
        m_goldSpr->setVisible(false);
    }else{
        m_costNumText->setVisible(true);
        m_goldSpr->setVisible(true);
    }
    this->m_picNode->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(picPath.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        pic->setScale(2);
    }
    this->m_picNode->addChild(pic);

    if(info.canPray() == false ||(!m_openNode->isVisible())){
        m_graySpr->setVisible(true);
    }else{
        m_graySpr->setVisible(false);
    }
    m_layerColor->setVisible(false);
    if(info.isFirstFree==1){
        if(m_type==Wood){
            if(GlobalData::shared()->resourceInfo.lFood>GlobalData::shared()->resourceInfo.lWood){
                m_layerColor->setVisible(true);
                this->playScaleAction();
            }
        }else if(m_type==Food){
            if(GlobalData::shared()->resourceInfo.lFood<=GlobalData::shared()->resourceInfo.lWood){
                m_layerColor->setVisible(true);
                this->playScaleAction();
            }
        }
    }
   
}
void SacrificeOneCell::playScaleAction(){
    CCFadeTo* action = CCFadeTo::create(0.5,0);
    CCFadeTo* action1 = CCFadeTo::create(0.5,180);
    CCScaleTo* action2 = CCScaleTo::create(0.2,0.95);
    CCScaleTo* action3 = CCScaleTo::create(0.2,1.0);
    CCSequence* actionSq = CCSequence::create(action,action1,NULL);
    CCRepeatForever* repeatAction = CCRepeatForever::create(actionSq);
    m_layerColor->runAction(repeatAction);
}
void SacrificeOneCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
//    UIComponent::getInstance()->hidePopupBG();
}

void SacrificeOneCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler SacrificeOneCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    
    return NULL;
}

bool SacrificeOneCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_graySpr", CCScale9Sprite*, this->m_graySpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintBG", CCScale9Sprite*, this->m_hintBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_openNode", CCNode*, this->m_openNode);
  
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeDesText", CCLabelIF*, this->m_closeDesText);
  
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickText", CCLabelIF*, this->m_clickText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costNumText", CCLabelIF*, this->m_costNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getNumText", CCLabelIF*, this->m_getNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldSpr", CCSprite*, this->m_goldSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_layerColor", CCLayerColor*, this->m_layerColor);
    return false;
}


bool SacrificeOneCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    if(layer && layer->getChildByTag(19999)){
        FlyHint* flyHit = dynamic_cast<FlyHint*>(layer->getChildByTag(19999));
        if(flyHit){
            flyHit->stopAllActions();
            flyHit->removeSelf();
        }
    }
    
    m_touchPoint = pTouch->getLocation();
    SacrificeInfo& info = GlobalData::shared()->sacrificeInfo;
    if(info.canPray() == false||(!info.isDataReturn)){
        return false;
    }
    if(isTouchInside(m_hintBG, pTouch)&&m_openNode->isVisible()){
        m_totalNode->stopAllActions();
        m_totalNode->setScale(1.2);
        return true;
    }
    
    return false;
}

void SacrificeOneCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_totalNode->setScale(1.0);
    m_totalNode->stopAllActions();
    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 10) {
        return;
    }

    SacrificeInfo& info = GlobalData::shared()->sacrificeInfo;
    if(info.canPray() == false){
        return;
    }
    int gold =0;
    if(m_type==Wood){
        gold = info.goldwood;
    }else if(m_type==Stone){
        gold = info.goldsilver;
    }else if(m_type==Iron){
        gold = info.goldiron;
    }else if(m_type==Food){
        gold = info.goldfood;
    }else if(m_type == Silver){
        gold = info.goldsteel;
    }
    
    if(info.canFreePray()){
        gold=0;
        buyPray();
    }else{
        if(info.getLeftFreeCount()==0){
            YesNoDialog::showButtonAndGold(_lang("102329").c_str(), CCCallFunc::create(this, callfunc_selector(SacrificeOneCell::buyPray)), _lang("102148").c_str(), gold);
        }else{
            buyPray();
        }
    }
//    YesNoDialog::showButtonAndGold(_lang("104919").c_str(), CCCallFunc::create(this, callfunc_selector(SacrificeOneCell::buyPray)), _lang("102148").c_str(), gold);
}
void SacrificeOneCell::buyPray(){
    SacrificeInfo& info = GlobalData::shared()->sacrificeInfo;
    int num = 0;
    int gold =0;
    if(m_type==Wood){
        num=info.wood;
        gold = info.goldwood;
    }else if(m_type==Stone){
        num=info.silver;
        gold = info.goldsilver;
    }else if(m_type==Iron){
        num=info.iron;
        gold = info.goldiron;
    }else if(m_type==Food){
        num = info.food;
        gold = info.goldfood;
    }else if(m_type == Silver){
        num = info.steel;
        gold = info.goldsteel;
    }
 
    if(info.canFreePray() == true){
        gold=0;
    }
    if (gold > GlobalData::shared()->playerInfo.gold) {
        YesNoDialog::gotoPayTips();
        return;
    }
    m_waitInterface= GameController::getInstance()->showWaitInterface(m_hintBG);
    info.isDataReturn = false;
    SacrificeBuyCommand* cmd = new SacrificeBuyCommand(m_type,gold,num);
    cmd->sendAndRelease();

}

void SacrificeOneCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
