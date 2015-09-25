//
//  AchievementNewPopUpView.cpp
//  IF
//
//  Created by lifangkai on 14-7-8.
//
//

#include "AchievementNewPopUpView.h"

//
//  AchievementPopUpView.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-19.
//
//

#include "AchievementNewPopUpView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "AchievementDetailPopUpView.h"
#include "SoundController.h"
#include "DropRdCCB.h"
#include "QuestRewardCommand.h"
#include "GoldExchangeView.h"
#include "LuaController.h"
#include "CCMathUtils.h"

AchievementNewPopUpView *AchievementNewPopUpView::create(){
    AchievementNewPopUpView *ret = new AchievementNewPopUpView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

AchievementNewPopUpView::~AchievementNewPopUpView(){

}
void AchievementNewPopUpView::onEnter(){
    PopupBaseView::onEnter();
    CCLoadSprite::doResourceByCommonIndex(101, true);
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchievementNewPopUpView::refreshView), QUEST_STATE_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchievementNewPopUpView::onPlayDropRD), MSG_PLAY_DROP_RD, NULL);
    this->refreshView(NULL);
}

void AchievementNewPopUpView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_STATE_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PLAY_DROP_RD);
    PopupBaseView::onExit();
}

bool AchievementNewPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        if (CCCommonUtils::isIosAndroidPad())
        {
             std::string _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_101_HD;
            if (CCFileUtils::sharedFileUtils()->isFileExist(_path)) {
                _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_HD;
                CCLoadSprite::doResourceByPathIndex(_path, 101, true);
            }
            
        }
        
        CCLoadSprite::doResourceByCommonIndex(101, true);
        
        setCleanFunction([](){
            if (CCCommonUtils::isIosAndroidPad())
            {
                std::string _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_101_HD;
                if (CCFileUtils::sharedFileUtils()->isFileExist(_path)) {
                    _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_HD;
                    CCLoadSprite::doResourceByPathIndex(_path, 101, false);
                }
            }
            
            CCLoadSprite::doResourceByCommonIndex(101, false);
        });
        auto bg = CCBLoadFile("QuestViewNewCCB", this, this);
        this->setContentSize(bg->getContentSize());
        setTitleName(_lang("107500"));
        
        int preHeight = this->m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        
        int dh = m_viewBg->getContentSize().height - preHeight;
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        m_listContainer->setPositionY(m_listContainer->getPositionY()-dh);
        
        m_data = CCArray::create();
        m_cells = CCArray::create();
        m_waitInterface=NULL;
        m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setDelegate(this);
        m_tabView->setTouchPriority(2);
        m_listContainer->addChild(m_tabView);
        CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());
        
        m_recommendinfo = NULL;
        m_guideAutoSt = 0;
        ret = true;
    }
    return ret;
}

void AchievementNewPopUpView::onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    auto dropCCB1 = DropRdCCB::create(R_FOOD);
//    this->addChild(dropCCB1);
//    return;
    
    if (QuestController::getInstance()->isGettingReward) {
        return;
    }
    
    if(m_recommendinfo->state!=COMPLETE || m_recommendinfo->curValue<m_recommendinfo->maxValue){
        PopupViewController::getInstance()->removeAllPopupView();
        QuestController::getInstance()->goToQuestTarget(m_recommendinfo);
        return;
    }
    
    m_btnReward->setEnabled(false);
    auto dropCCB = DropRdCCB::create(r_type);
    if (CCCommonUtils::isIosAndroidPad())
    {
        dropCCB->setScale(2);
    }
    this->addChild(dropCCB);
    m_waitInterface= GameController::getInstance()->showWaitInterface(m_btnReward);
    QuestController::getInstance()->getReward(m_recommendinfo->itemId, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("AC_reward"));
}
void AchievementNewPopUpView::onGoInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_recommendinfo==NULL){
        return;
    }
    PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_recommendinfo));
}
void AchievementNewPopUpView::refreshView(CCObject * obj){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_data->removeAllObjects();
    m_cells->removeAllObjects();
    CCArray* arr = CCArray::create();
    setRecommendTaskInfo();
    map<std::string, QuestInfo*>::iterator it;
    for(it = QuestController::getInstance()->currentAchList.begin();it!=QuestController::getInstance()->currentAchList.end();it++){
        if(m_recommendinfo!=NULL){
            if(m_recommendinfo->itemId==it->second->itemId){
                continue;
            }
        }
        
        if(it->second->type==2){
            if(it->second->order2<=0){
                continue;
            }
            arr->addObject(it->second);
            //break;
        }
    }
    int num = arr->count()-1;
    for(int j=0;j<num;j++){
        for(int i=0;i<num-j;i++)
        {
            QuestInfo* quest1 = (QuestInfo*)arr->objectAtIndex(i);
            QuestInfo* quest2 = (QuestInfo*)arr->objectAtIndex(i+1);
            if(quest1->order2 > quest2->order2 || (quest1->state == ACCEPT && quest2->state != ACCEPT))//未接受的在前
            {
                arr->swap(i, i+1);
            }
        }
    }
    int leng = arr->count();
//    leng = leng>4?4:leng;
    leng = leng>3?3:leng;
    for(int i=0;i<leng;i++ ){
        m_data->addObject(arr->objectAtIndex(i)); 
        
    }
    
    if(m_data->count()<=0){
        m_noQuestText->setString(_lang("107530"));
        m_noQuestText->setVisible(true);
    }else{
        m_noQuestText->setVisible(false);
    }
    bool ishasChild = false;
    CCPoint pos;
    if(m_tabView->getContainer()->getChildrenCount()==0){
        ishasChild = true;
    }else{
        pos = m_tabView->getContentOffset();
    }
    m_tabView->reloadData();
    if(ishasChild){
    }else{

        CCPoint minPt = m_tabView->minContainerOffset();
        CCPoint maxPt = m_tabView->maxContainerOffset();
        if (pos.y > 0) {
            return;
        }
        if (pos.y > maxPt.y) {
            pos.y = maxPt.y;
        }
        else if (pos.y < minPt.y) {
            pos.y = minPt.y;
        }
        m_tabView->setContentOffset(pos);
    }
}
int AchievementNewPopUpView::getContentOffSety(){
    int index = 0;
    int num = m_data->count()-1;
    for(int j=0;j<num;j++){
        QuestInfo* quest1 = (QuestInfo*)m_data->objectAtIndex(j);
        if(quest1->state==COMPLETE){
            break;
        }
        index++;
        j++;

    }
   
    return index;
}
void AchievementNewPopUpView::setRecommendTaskInfo(float dt)
{
//    m_recommendinfo = NULL;
//    m_recommendinfo = QuestController::getInstance()->getRecommendQuest(true);
    
    QuestInfo* curRecommendinfo = QuestController::getInstance()->getRecommendQuest(true);
    if (curRecommendinfo != m_recommendinfo) {
        bool stTmp = true;
        if (m_recommendinfo!=NULL) {
            stTmp = false;
            this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(AchievementNewPopUpView::updateRecommendTask));
            this->getAnimationManager()->runAnimationsForSequenceNamed("Refresh");
        }
        m_recommendinfo = curRecommendinfo;
        if (stTmp) {
            updateRecommendTask();
        }
    }
}

void AchievementNewPopUpView::updateRecommendTask()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    m_nameTxt->setString(_lang("107528").c_str());
    if(m_recommendinfo){
        QuestInfo* quest = m_recommendinfo;
        m_btnReward->setEnabled(false);
        if(m_recommendinfo->state==COMPLETE){
            m_btnReward->setEnabled(true);
            CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());
        }else{
            m_btnReward->setEnabled(true);
            CCCommonUtils::setButtonTitle(m_btnReward, _lang("2000214").c_str());
        }
        if(quest->curValue>=quest->maxValue){
            m_btnReward->setEnabled(true);
            CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());
        }
        m_descTxt->setString(quest->name);
        std::string str = CC_CMDITOA(quest->curValue>quest->maxValue?quest->maxValue:quest->curValue);
        str.append("/");
        str.append(CC_CMDITOA(quest->maxValue));
        m_progressTxt->setString(str);
        int max = quest->maxValue;
        max = MAX(max, quest->curValue);
        float scale = quest->curValue*1.0/max;
        scale = MIN(scale,1);
        m_progress->setScaleX(scale);
        m_headNode->removeAllChildren();
        CCSprite* pic1 = NULL;
        if(m_recommendinfo->icon1!=""){
            std::string _picname = m_recommendinfo->icon1;
            if (CCCommonUtils::isIosAndroidPad())
            {
                std::vector<std::string> _strvet;
                CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
                _picname = _strvet[0] + "_hd." + _strvet[1];
                if (!CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str()))
                {
                    _picname = m_recommendinfo->icon1;
                    m_headNode->setScale(2.f);
                }
            }
            pic1 = CCLoadSprite::createSprite(_picname.c_str());
            m_headNode->addChild(pic1);
            pic1->setAnchorPoint(ccp(0.5, 0.5));
        }
        if(m_recommendinfo->icon2!=""){
            std::string _picname = m_recommendinfo->icon2;
            if (CCCommonUtils::isIosAndroidPad())
            {
                std::vector<std::string> _strvet;
                CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
                _picname = _strvet[0] + "_hd." + _strvet[1];
                if (!CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str()))
                {
                    _picname = m_recommendinfo->icon2;
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
        
        string rewardIcon = "";
        r_type = -1;
        double r_num = 0;
        if (m_recommendinfo->rewardshow == "wood") {
            rewardIcon = "item400.png";
            r_type = R_WOOD;
        }
        else if (m_recommendinfo->rewardshow == "food") {
            rewardIcon = "item403.png";
            r_type = R_FOOD;
        }
        else if (m_recommendinfo->rewardshow == "iron") {
            rewardIcon = "item402.png";
            r_type = R_IRON;
        }
        else if (m_recommendinfo->rewardshow == "stone") {
            rewardIcon = "item401.png";
            r_type = R_STONE;
        }
        else if (m_recommendinfo->rewardshow == "gold") {
            rewardIcon = "item406.png";
            r_type = R_GOLD;
        }
        m_rewardIcon->removeAllChildren();
        bool isGold = false;
        for (int i=0; i<m_recommendinfo->reward->count(); i++) {
            auto dic = dynamic_cast<CCDictionary*>(m_recommendinfo->reward->objectAtIndex(i));
            int type = dic->valueForKey("type")->intValue();
            if (type == r_type) {
                isGold = true;
                r_num = dic->valueForKey("value")->intValue();
                break;
            }
        }
        if (isGold) {
            auto rIcon = CCLoadSprite::createSprite(rewardIcon.c_str());
            rIcon->setAnchorPoint(ccp(1, 0));
            m_rewardIcon->addChild(rIcon);
            string tmpstr = "+";
            tmpstr = tmpstr + CC_ITOA_K(r_num);
            m_rewardNum->setString(tmpstr.c_str());
            m_rewardNum->setFntFile("pve_fnt_boss.fnt");
        }
    }
}

void AchievementNewPopUpView::onPlayDropRD(CCObject* obj)
{
    CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
    if (intObj) {
        int tr_type = intObj->getValue();
        if (tr_type>-1) {
            auto dropCCB = DropRdCCB::create(tr_type);
            if (CCCommonUtils::isIosAndroidPad())
            {
                dropCCB->setScale(2.f);
            }
            this->addChild(dropCCB);
        }
    }
}

bool AchievementNewPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progressTxt", CCLabelIF*, this->m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_noQuestText", CCLabelIF*, this->m_noQuestText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goInfoBtn", CCControlButton*, this->m_goInfoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnReward", CCControlButton*, this->m_btnReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_recommandBG", CCNode*, this->m_recommandBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardIcon", CCNode*, this->m_rewardIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardNum", CCLabelIFBMFont*, this->m_rewardNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dropNode", CCNode*, this->m_dropNode);
    return false;
}


SEL_CCControlHandler AchievementNewPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoInfoBtnClick", AchievementNewPopUpView::onGoInfoBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardBtnClick", AchievementNewPopUpView::onRewardBtnClick);
    return NULL;
}
bool AchievementNewPopUpView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(m_recommandBG,pTouch)){
        return true;
    }
    return false;
}
void AchievementNewPopUpView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (fabs(m_touchPoint.y - pTouch->getLocation().y)> 30) {
        return;
    }
    if(m_recommendinfo==NULL){
        return;
    }
    if (isTouchInside(m_recommandBG,pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        if (CCCommonUtils::isIosAndroidPad())
        {
            PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_recommendinfo), true, false, false, true);
        }
        else
        {
            PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_recommendinfo));
        }
    }
}

CCSize AchievementNewPopUpView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 370);
    }
    
    return CCSize(620, 210);
}

CCSize AchievementNewPopUpView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 370);
    }
    
    return CCSize(620, 210);
}

CCTableViewCell* AchievementNewPopUpView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    AchievementNewTaskTableCell* cell = (AchievementNewTaskTableCell*)table->dequeueCell();
    QuestInfo* info = (QuestInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info,idx);
        }else{
            cell = AchievementNewTaskTableCell::create(info,idx);
            m_cells->addObject(cell);
        }
    }
    return cell;
}

ssize_t AchievementNewPopUpView::numberOfCellsInTableView(CCTableView *table){
    int count = m_data->count();
    return count;
}

void AchievementNewPopUpView::scrollViewDidScroll(CCScrollView* view){
    float mindy = m_tabView->minContainerOffset().y;
    float dy = m_tabView->getContentOffset().y;
    if(dy<mindy){
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}

CCNode * AchievementNewPopUpView::getGuideNode(string _key)
{
    if (_key=="AC_back") {
        return UIComponent::getInstance()->m_popupReturnBtn;
    }
    else if (_key=="AC_reward") {
        
        if(m_recommendinfo&&m_recommendinfo->state==COMPLETE){
            m_guideAutoSt = 0;
            return m_btnReward;
        }else {
            if (m_guideAutoSt==0) {
                m_guideAutoSt = 1;
                autoGuideNext();
//                scheduleOnce(schedule_selector(AchievementNewPopUpView::autoGuideNext), 0.1);
            }
            return NULL;
        }
    }
    else if (_key=="AC_normal") {
        return m_listContainer;
    }
    return NULL;
}

void AchievementNewPopUpView::autoGuideNext(float dt)
{
    if (m_guideAutoSt==1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("AC_reward"));
    }
}

//class AchievementNewTaskTableCell
AchievementNewTaskTableCell* AchievementNewTaskTableCell::create(QuestInfo *info, int index)
{
    AchievementNewTaskTableCell* ret = new AchievementNewTaskTableCell();
    if (ret && ret->init(info, index))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AchievementNewTaskTableCell::init(QuestInfo *info, int index)
{
    this->setData(info, index);
    return true;
}

void AchievementNewTaskTableCell::setData(QuestInfo *info, int index)
{
    if (mCell == NULL)
    {
        mCell = AchievementNewTaskCell::create(info, index);
        addChild(mCell);
    }
    else
    {
        mCell->setData(info, index);
    }
}

CCNode* AchievementNewTaskTableCell::getGuideNode()
{
    return mCell->getGuideNode();
}

//class AchievementNewTaskCell
AchievementNewTaskCell *AchievementNewTaskCell::create(QuestInfo* info,int index){
    AchievementNewTaskCell *ret = new AchievementNewTaskCell(info,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool AchievementNewTaskCell::init(){
    auto bg = CCBLoadFile("QuestCellTask", this, this);
    this->setContentSize(bg->getContentSize());
    CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());
    m_waitInterface = NULL;
    this->setData(m_info,m_index);
    return true;
}

CCNode* AchievementNewTaskCell::getGuideNode()
{
    if (m_btnReward && m_btnReward->isVisible() && m_btnReward->isEnabled()) {
        return m_btnReward;
    }
    return NULL;
}
void AchievementNewTaskCell::setData(QuestInfo* info,int index){
    m_info = info;
    if (m_playAnimation)
    {
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(AchievementNewTaskCell::animationCallBack));
        this->getAnimationManager()->runAnimationsForSequenceNamed("2");
    }
    else
    {
        animationCallBack();
    }
}

void AchievementNewTaskCell::animationCallBack()
{
    m_playAnimation = false;
    this->getAnimationManager()->runAnimationsForSequenceNamed("1");
    
    m_rewardClick = false;
    
    m_picHead->removeAllChildren();
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    if(m_info->state==COMPLETE){
        m_btnGoTo->setVisible(false);
        m_btnReward->setVisible(true);
        //   m_btnLabel->setVisible(true);
        if(m_info->itemId == "2900101" || m_info->itemId == "2900102" || m_info->itemId == "2900103"){
            if (m_info->itemId == "2900102") {
                CCCommonUtils::setButtonTitle(m_btnReward, _lang("133109").c_str());//领奖按钮显示【完成】，dialog=133109
            }else{
                CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());//rewardbutton文字变回原来的reward
            }
        }
    }else{
        m_btnGoTo->setVisible(true);
        m_btnReward->setVisible(false);
        //  m_btnLabel->setVisible(false);
    }
    if(m_info->curValue>=m_info->maxValue){
        m_btnReward->setVisible(true);
        m_btnGoTo->setVisible(false);
    }
    m_nameTxt->setString(m_info->name);
    //    std::string str = _lang("107526")+CC_CMDITOA(m_info->curValue>m_info->maxValue?m_info->maxValue:m_info->curValue);
    std::string str = CC_CMDITOA(m_info->curValue>m_info->maxValue?m_info->maxValue:m_info->curValue);
    str.append("/");
    str.append(CC_CMDITOA(m_info->maxValue));
    m_titleTxt->setString(_lang("107529"));
    //    m_descTxt->setString(str);
    CCSprite* pic1 =NULL;
    if(m_info->icon1!=""){
        std::string _picname = m_info->icon1;
        if (CCCommonUtils::isIosAndroidPad())
        {
            std::vector<std::string> _strvet;
            CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
            _picname = _strvet[0] + "_hd." + _strvet[1];
            auto __frame =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str());
            if (!__frame)
            {
                _picname = m_info->icon1;
                m_picHead->setScale(2.f);
            }
        }

        pic1 = CCLoadSprite::createSprite(_picname.c_str());
        m_picHead->addChild(pic1);
    }
    if(m_info->icon2!=""){
        std::string _picname = m_info->icon2;
        if (CCCommonUtils::isIosAndroidPad())
        {
            std::vector<std::string> _strvet;
            CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
            _picname = _strvet[0] + "_hd." + _strvet[1];
            auto __frame =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str());
            if (!__frame)
            {
                _picname = m_info->icon2;
                m_picHead->setScale(2.f);
            }
        }
        CCSprite* pic = CCLoadSprite::createSprite(_picname.c_str());
        m_picHead->addChild(pic);
        if(pic1!=NULL){
            pic->cocos2d::CCNode::setPosition(pic1->getContentSize().width/2-pic->getContentSize().width/2, -(pic1->getContentSize().height/2-pic->getContentSize().height/2));
        }
    }
    
    float pro = m_info->curValue*1.0/m_info->maxValue;
    pro = pro>1?1:pro;
    m_progress->setScaleX(pro);
    m_progressTxt->setString(str);
    
    if ((m_info->itemId == "2900101" || m_info->itemId == "2900102" || m_info->itemId == "2900103") && m_info->state == ACCEPT) {
        CCCommonUtils::setButtonTitle(m_btnReward, _lang("2000214").c_str());
        //        m_rewardClick = true;
        m_btnGoTo->setEnabled(false);
        m_btnGoTo->setVisible(false);
        m_btnReward->setVisible(true);
    }
}

void AchievementNewTaskCell::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 3, false);
}

void AchievementNewTaskCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AchievementNewTaskCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
  //  CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnLabel", CCLabelIF*, this->m_btnLabel);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnReward", CCControlButton*, this->m_btnReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnGoTo", CCControlButton*, this->m_btnGoTo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cellBG", CCNode*, this->m_cellBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picHead", CCNode*, this->m_picHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progressTxt", CCLabelIF*, this->m_progressTxt);
    return false;
}

SEL_CCControlHandler AchievementNewTaskCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoToBtnClick", AchievementNewTaskCell::onGoToBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardBtnClick", AchievementNewTaskCell::onRewardBtnClick);
    // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoToBtnClick1", AchievementNewTaskCell::onGoToBtnClick1);
    return NULL;
}
void AchievementNewTaskCell::onGoToBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    //SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_info));
    
}
bool AchievementNewTaskCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(m_cellBG,pTouch)){
     return true;
    }
    return false;
}
void AchievementNewTaskCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (fabs(m_touchPoint.y - pTouch->getLocation().y)> 30) {
        return;
    }
    if (isTouchInside(m_cellBG,pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_info));
    }

}

void AchievementNewTaskCell::onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if ((m_info->itemId == "2900102" || m_info->itemId == "2900101") && m_info->state == ACCEPT)
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
        CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());//rewardbutton文字变回原来的reward
        return;
    }
    
    if (m_info->itemId == "2900103" && m_info->state == ACCEPT) {
        PopupViewController::getInstance()->addPopupView(GoldExchangeView::create(),false);
        return;
    }
    
    if (m_rewardClick||QuestController::getInstance()->isGettingReward) {
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
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification( MSG_PLAY_DROP_RD,CCInteger::create(r_type) );
    }
    
    m_rewardClick=true;
    m_playAnimation=true;
    m_waitInterface=GameController::getInstance()->showWaitInterface(m_btnReward);
    QuestController::getInstance()->getReward(m_info->itemId);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("AC_reward"));
    
}