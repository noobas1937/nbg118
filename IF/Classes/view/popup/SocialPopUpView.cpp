//
//  SocialPopUpView.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-11.
//
//

#include "SocialPopUpView.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "UIComponent.h"
#include "GameController.h"
#include "../../Ext/CCDevice.h"
#include "FBUtilies.h"

static const char* btnIcon[7] = {"icon_facebook.png","icon_Twitter.png","icon_youtube.png","icon_vk_general.png","social_kr.png","icon_google.png","icon_337.png"};
static const char* btnLang[7] = {"101265","101266","101267","101297","","101268","101269"};
static const char* btnURL[7] = {"https://www.facebook.com/DragonClans.Game","https://twitter.com/ClashOfKingsCOK","http://www.youtube.com/user/ClashofKingsGame","http://vk.com/clashofking","http://cafe.naver.com/clashofkings","https://plus.google.com/communities/102750039774058615160","http://forum.337.com/en/forum-3760-1.html"};

bool SocialPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_background", CCScale9Sprite*, this->m_background);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    return true;
}

SEL_CCControlHandler SocialPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}

SocialPopUpView::~SocialPopUpView()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_REMOVE,CCString::create(CC_ITOA(PVT_alliance)));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFriendsList);
}

SocialPopUpView:: SocialPopUpView(){
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SocialPopUpView::test), MSG_FBFriendsList, NULL);
}

void SocialPopUpView::test(CCObject* param){
    CCLOG("fb call back");
}

CCSize SocialPopUpView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536,220);
    }
    return CCSize(640,110);
}

CCSize SocialPopUpView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536,220);
    }
    return CCSize(640,110);
}

CCTableViewCell* SocialPopUpView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_datas->count()){
        return NULL;
    }
    SocialBtnCell* cell = (SocialBtnCell*)table->dequeueCell();
    CCInteger* intObj = dynamic_cast<CCInteger*>(m_datas->objectAtIndex(idx));
    if(cell==NULL){
        cell = SocialBtnCell::create();
    }
    if (intObj) {
        cell->setData(intObj->getValue());
    }
    return cell;
}

ssize_t SocialPopUpView::numberOfCellsInTableView(CCTableView *table)
{
    return m_datas->count();
}

bool SocialPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init())
    {
        setIsHDPanel(true);
        auto tmpCCB = CCBLoadFile("SocialView", this, this);
        this->setContentSize(tmpCCB->getContentSize());

        int oldBgHeight = m_background->getContentSize().height;
        changeBGHeight(m_background);
        int newBgHeight = m_background->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
        std::string language = LocalController::shared()->getLanguageFileName();
        m_datas = CCArray::create();
        if(language=="ko"){
            for (int i=0; i<5; i++) {
                m_datas->addObject(CCInteger::create(i));
            }
        }else if(GlobalData::shared()->analyticID == "tstore" || GlobalData::shared()->analyticID == "amazon" || GlobalData::shared()->analyticID == "AppStore"){
            for (int i=0; i<4; i++) {
                m_datas->addObject(CCInteger::create(i));
            }
        }else{
            for (int i=0; i<7; i++) {
                if (i==4) {
                    continue;//不是ko，4都不显示
                }
                m_datas->addObject(CCInteger::create(i));
            }
        }
        //fusheng 暂时只显示facebook
        m_datas->removeAllObjects();
        m_datas->addObject(CCInteger::create(0));

        
        //fusheng end
        
        m_tableView = CCTableView::create(this, m_infoList->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setAnchorPoint(CCPoint(0,0));
        m_tableView->setDelegate(this);
        
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_infoList->addChild(m_tableView);
        
        ret = true;
    }
    return ret;
    
}

void SocialPopUpView::onEnter(){
    CCNode::onEnter();
    this->setTitleName(_lang("101264"));
}

void SocialPopUpView::onExit(){
    CCNode::onExit();
}

bool SocialBtnCell::init()
{
    bool ret = true;
    
    if (CCTableViewCell::init()) {
        CCNode* node = CCBLoadFile("SocialBtnCell", this, this);
        CCSize size = node->getContentSize();
        setContentSize(size);
        if(GlobalData::shared()->analyticID == "cn1" || GlobalData::shared()->isXiaoMiPlatForm()){
            CCCommonUtils::setButtonTitle(m_goBtn, "赞");
        }else{
            CCCommonUtils::setButtonTitle(m_goBtn, _lang("101270").c_str());
        }
        return true;
    }
    
    return ret;
}

void SocialBtnCell::setData(int index)
{
    m_index = index;
    m_icon->removeAllChildrenWithCleanup(true);
    std::string language = LocalController::shared()->getLanguageFileName();
    if(m_index==4 && language=="ko"){
        m_descTxt->setString("한국 유저 팬까페에 가입하세요！");
    }else{
        m_descTxt->setString(_lang(btnLang[m_index]));
    }
    std::string bicon = btnIcon[m_index];
    if(m_index==3 && language=="ru"){
        bicon = "icon_vk_ru.png";
    }
    CCSprite* logo = CCLoadSprite::createSprite(bicon.c_str());
    CCCommonUtils::setSpriteMaxSize(logo , 60);
    m_icon->addChild(logo);
}

void SocialBtnCell::onEnter(){
    CCNode::onEnter();
}

void SocialBtnCell::onExit(){
    CCNode::onExit();
}

void SocialBtnCell::onGoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    cocos2d::extension::CCDevice::updateVersion(btnURL[m_index]);
}

SEL_CCControlHandler SocialBtnCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoBtnClick", SocialBtnCell::onGoBtnClick);
    return NULL;
}

bool SocialBtnCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goBtn", CCControlButton*, this->m_goBtn);
    return false;
}
