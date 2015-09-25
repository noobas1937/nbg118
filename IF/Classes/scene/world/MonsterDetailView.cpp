//
//  MonsterDetailView.cpp
//  IF
//
//  Created by 邹 程 on 14-4-14.
//
//

#include "MonsterDetailView.h"
#include "WorldCommand.h"
#include "PopupViewController.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "UIComponent.h"
#include "BattleManager.h"
#include "MazeController.h"

bool MonsterDetailView::init(unsigned int index) {
    PopupBaseView::init();
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });
    setTitleName(_lang("108610"));
    auto bg = CCBLoadFile("TIPS_yerenyingdi",this,this);
    this->setContentSize(bg->getContentSize());
    m_tileIndex = index;
    
    m_titleText->setString(_lang("108655"));
    m_hintText->setString(_lang("108656"));

    requestDetail();
    schedule(schedule_selector(MonsterDetailView::updateViewTime), 1.0f);
    return true;
}

void MonsterDetailView::onEnter() {
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MonsterDetailView::updateView), MSG_MONSTER_UPDATE, nullptr);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void MonsterDetailView::onExit() {
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MONSTER_UPDATE);
    PopupBaseView::onExit();
}

void MonsterDetailView::updateView(cocos2d::CCObject *obj) {
    auto indexObj = dynamic_cast<CCInteger*>(obj);
    if (!indexObj) {
        return;
    }
    auto index = indexObj->getValue();
    if (index != m_tileIndex) {
        return;
    }
    requestDetail();
}

bool MonsterDetailView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void MonsterDetailView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void MonsterDetailView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_bg, pTouch)){
        this->closeSelf();
    }
}

void MonsterDetailView::addButton(){
    CCCommonUtils::setButtonTitle(m_button1, _lang_1("108669", CC_ITOA(GlobalData::shared()->worldConfig.monsterAttackTime[0])));
    CCCommonUtils::setButtonTitle(m_button2, _lang_1("108670", CC_ITOA(GlobalData::shared()->worldConfig.monsterAttackTime[1] / 60)));
    CCCommonUtils::setButtonTitle(m_button3, _lang_1("108671", CC_ITOA(GlobalData::shared()->worldConfig.monsterAttackTime[2] / 60)));
    CCCommonUtils::setButtonTitle(m_button4, _lang_1("108672", CC_ITOA(GlobalData::shared()->worldConfig.monsterAttackTime[3] / 60)));
}

void MonsterDetailView::setButtonState(){
    auto &info = WorldController::getInstance()->getCityInfos()[m_tileIndex];
    auto now = WorldController::getInstance()->getTime();
    double remain = (info.refreshStamp - now) / 1000;
    if(remain < GlobalData::shared()->worldConfig.monsterAttackTime[3] * 60){
        m_button4->setEnabled(false);
    }
    if(remain < GlobalData::shared()->worldConfig.monsterAttackTime[2] * 60){
        m_button3->setEnabled(false);
    }
    if(remain < GlobalData::shared()->worldConfig.monsterAttackTime[1] * 60){
        m_button2->setEnabled(false);
    }
    if(remain < GlobalData::shared()->worldConfig.monsterAttackTime[0] * 60){
        m_button1->setEnabled(false);
    }
}

void MonsterDetailView::updateViewTime(float dt) {
    if (m_isInUpdate) {
        return;
    }
    auto &info = WorldController::getInstance()->getCityInfos()[m_tileIndex];
    auto now = WorldController::getInstance()->getTime();
    double refreshTime = info.refreshStamp;
    if (now > refreshTime) {
        PopupViewController::getInstance()->removeAllPopupView();
        WorldMapView::instance()->m_map->updateDynamicMap(WorldController::getPointByIndex(m_tileIndex));
        return;
    }
    
    double remain = info.refreshStamp - now;
    string timeRemainStr = CCCommonUtils::timeLeftToCountDown((int)(remain/1000));
    m_progressLabel->setString(_lang_1("108657", timeRemainStr.c_str()));

    setButtonState();
    
    int index = info.parentCityIndex;
    auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
    auto it = selfMarch.find(index);
    if (it != selfMarch.end()) {
        this->closeSelf();
        PopupViewController::getInstance()->addPopupInView(MonsterAttackDetailView::create(index),false);
    }
}

void MonsterDetailView::requestDetail() {
    onDetailCallback(NULL);
}

void MonsterDetailView::onDetailCallback(cocos2d::CCObject *obj) {
    addButton();
    updateViewTime(0.0);
}

SEL_CCControlHandler MonsterDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickButton1", MonsterDetailView::onClickButton1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickButton2", MonsterDetailView::onClickButton2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickButton3", MonsterDetailView::onClickButton3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickButton4", MonsterDetailView::onClickButton4);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", MonsterDetailView::onCloseClick);

    return NULL;
}

bool MonsterDetailView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressLabel", CCLabelIF*, this->m_progressLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button1", CCControlButton*, this->m_button1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button2", CCControlButton*, this->m_button2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button3", CCControlButton*, this->m_button3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button4", CCControlButton*, this->m_button4);

    return false;
}

//void MonsterDetailView::onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    this->closeSelf();
//}

void MonsterDetailView::onClickButton1(CCObject * pSender, Control::EventType pCCControlEvent){
    openMarchPanel(1);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("TX_15"));
}

void MonsterDetailView::onClickButton2(CCObject * pSender, Control::EventType pCCControlEvent){
    openMarchPanel(2);
}

void MonsterDetailView::onClickButton3(CCObject * pSender, Control::EventType pCCControlEvent){
    openMarchPanel(3);
}

void MonsterDetailView::onClickButton4(CCObject * pSender, Control::EventType pCCControlEvent){
    openMarchPanel(4);
}

void MonsterDetailView::openMarchPanel(int index){
    WorldController::getInstance()->openMarchDeploy(m_tileIndex, index + 1);
    this->closeSelf();
}

CCNode* MonsterDetailView::getGuideNode(string _key)
{
    if (m_button1->isEnabled()) {
        return m_button1;
    }
    return NULL;
}

//---------
MonsterAttackDetailView *MonsterAttackDetailView::create(unsigned int idx){
    MonsterAttackDetailView *ret = new MonsterAttackDetailView();
    if(ret && ret->init(idx)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MonsterAttackDetailView::init(unsigned int idx){
    bool ret = false;
    if(PopupBaseView::init()){
        m_idx = idx;
        m_timeToNext = NULL;
        CCLoadSprite::doResourceByCommonIndex(8, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
        });
        setIsHDPanel(true);
        auto bg = CCBLoadFile("NewworldMonsterView",this,this);
        this->setContentSize(bg->getContentSize());
        int dy = 852 - CCDirector::sharedDirector()->getWinSize().height;
        if (CCCommonUtils::isIosAndroidPad())
        {
            dy = 2048 - CCDirector::sharedDirector()->getWinSize().height;
        }
        setTitleName(_lang("108655"));

        m_upNode->setPositionY(m_upNode->getPositionY() - dy);
        m_bgContainer->setPositionY(m_bgContainer->getPositionY() - dy / 2);
        CCCommonUtils::setButtonTitle(m_returnBtn, _lang("108660").c_str());
        if(WorldController::getInstance()->m_monsterAttackInfo->count() == 0 || !WorldController::getInstance()->isInfoTake){
            requestDetail();
        }else{
            initView();
        }
        ret = true;
    }
    return ret;
}

void MonsterAttackDetailView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MonsterAttackDetailView::onDataBack), MSG_WORLD_MONSTER_RESEARCH_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MonsterDetailView::updateView), MSG_MONSTER_UPDATE, nullptr);
}

void MonsterAttackDetailView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MONSTER_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WORLD_MONSTER_RESEARCH_CHANGE);
    PopupBaseView::onExit();
}

SEL_CCControlHandler MonsterAttackDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturnClick", MonsterAttackDetailView::onReturnClick);
    
    return NULL;
}

bool MonsterAttackDetailView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressLabel", CCLabelIF*, this->m_progressLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnBtn", CCControlButton*, this->m_returnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollContainer", CCNode*, this->m_scrollContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, this->m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cdProgress", CCScale9Sprite*, this->m_cdProgress)

    return false;
}

void MonsterAttackDetailView::onReturnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    YesNoDialog::show(_lang("108667").c_str(), CCCallFunc::create(this, callfunc_selector(MonsterAttackDetailView::confirmReturn)), 0);
}

void MonsterAttackDetailView::confirmReturn(){
    auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
    auto it = selfMarch.find(m_idx);
    if (it == selfMarch.end()) {
        // todo : show error
        return;
    }
    string uuid = it->second;
    
    if (uuid.empty()) {
        // todo : show error
        return;
    }
    
    auto dict = CCDictionary::create();
    dict->setObject(CCString::create(uuid), "marchId");
    WorldMapView::instance()->afterMarchCancel(dict);
    this->closeSelf();
}

void MonsterAttackDetailView::onDataBack(CCObject *obj){
    if(!obj){
        return;
    }
    m_tabView->reloadData();
    int count = WorldController::getInstance()->m_monsterAttackInfo->count();
    if(count % num_per_level == 0){
        refreshMaze();
    }else{
        MazeController::getInstance()->resetOpenIdx();
        refreshFlag();
    }
}

void MonsterAttackDetailView::onReportDetailClick(CCObject * pSender, Control::EventType pCCControlEvent){
    auto btn = dynamic_cast<CCControlButton *>(pSender);
    int tag = btn->getTag();
    if(tag >= WorldController::getInstance()->m_monsterAttackInfo->count()){
        return;
    }
    auto info = dynamic_cast<MonsterSearchResultInfo*>(WorldController::getInstance()->m_monsterAttackInfo->objectAtIndex(tag));
    BattleManager::shared()->mailFightReport(info->fid);

}

void MonsterAttackDetailView::updateViewTime(float dt) {
    int dtime = GlobalData::shared()->changeTime(WorldController::getInstance()->monsterAttEndTime) - GlobalData::shared()->getWorldTime();
    int totalTime = WorldController::getInstance()->monsterAttEndTime - WorldController::getInstance()->monsterAttStartTime;
    if(dtime <= 0){
        WorldController::getInstance()->m_selfMarchUuid.erase(m_idx);
        this->closeSelf();
    }else{
        m_timeLabel->setString(_lang("108673") + CCCommonUtils::timeLeftToCountDown(dtime));
        float per = 1.0f - 1.0f * dtime / totalTime;
        per = fminf(1.0, per);
        per = fmaxf(0.0f, per);
        m_cdProgress->setScaleX(560 * per / m_cdProgress->getContentSize().width);
        m_progressLabel->setString(std::string("") + CC_ITOA(per * 100) + "%");
        if(m_timeToNext && m_timeToNext->getParent()){
            int leftTime = dtime % (60 * 3);
            m_timeToNext->setString(CCCommonUtils::timeLeftToCountDown(leftTime));
        }
    }
}

void MonsterAttackDetailView::initView(CCObject* p){
    m_tabView = CCTableView::create(this, m_scrollContainer->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Popup);
    m_scrollContainer->addChild(m_tabView);

    m_tabView->reloadData();
    schedule(schedule_selector(MonsterAttackDetailView::updateViewTime), 1.0f);
    MazeController::getInstance()->resetOpenIdx();
    updateViewTime(0.0f);
    refreshMaze();
    
    this->m_timeLabel->setOpacity(255);
    this->m_progressLabel->setOpacity(0);
    auto sequence1 = CCSequence::create(CCFadeOut::create(1.5)
                                        , CCDelayTime::create(4.5)
                                        , CCFadeIn::create(1.5)
                                        , CCDelayTime::create(1.5)
                                        , NULL
                                        );
    m_timeLabel->runAction(CCRepeatForever::create(sequence1));
    
    auto sequence2 = CCSequence::create(
                                        CCDelayTime::create(1.5)
                                        , CCFadeIn::create(1.5)
                                        , CCDelayTime::create(1.5)
                                        , CCFadeOut::create(1.5)
                                        , CCDelayTime::create(3)
                                        , NULL
                                        );
    m_progressLabel->runAction(CCRepeatForever::create(sequence2));
}

void MonsterAttackDetailView::requestDetail() {
    int sid = GlobalData::shared()->playerInfo.currentServerId;
    if (WorldController::getInstance()->getCityInfos().find(m_idx) != WorldController::getInstance()->getCityInfos().end()) {
        sid = WorldController::getInstance()->getCityInfos()[m_idx].tileServerId;
    }
    auto cmd = new WorldDetailCommand(m_idx,sid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MonsterAttackDetailView::initView), NULL));
    cmd->sendAndRelease();
}

void MonsterAttackDetailView::refreshMaze(){
    m_bgContainer->removeAllChildren();
    CCSpriteBatchNode *batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("maze_1.png")->getTexture());
    float s = 1.0f;
    if(CCDirector::sharedDirector()->getWinSize().height > 1050){
        s = (CCDirector::sharedDirector()->getWinSize().height - 450) / 600;
    }
    CCSprite *bgL = CCLoadSprite::createSprite("yerenyingdi_bg.png");
    bgL->setAnchorPoint(ccp(1, 0.5));
    bgL->setScale(s);
    m_bgContainer->addChild(bgL);
    
    CCSprite *bgL1 = CCLoadSprite::createSprite("yerenyingdi_bg.png");
    bgL1->setAnchorPoint(ccp(0, 0.5));
    bgL1->setFlipX(true);
    bgL1->setScale(s);

    m_bgContainer->addChild(bgL1);

    m_bgContainer->addChild(batch);
    MazeController::getInstance()->generateNewMaze();
    map<int, int>::iterator it;
    for(it = MazeController::getInstance()->m_map.begin(); it != MazeController::getInstance()->m_map.end(); it++){
        int index = it->first;
        int resIndex = it->second;
        if(resIndex != 0){
            CCPoint pt = MazeController::getInstance()->getRealPosByIndex(index);
            std::string pic = CCString::createWithFormat("maze_%d.png", resIndex)->getCString();

            CCSprite *sprite = CCLoadSprite::createSprite(pic.c_str());
            batch->addChild(sprite);
            sprite->setPosition(pt);
        }
    }
    refreshFlag();
}

void MonsterAttackDetailView::refreshFlag(){
    map<int, int>::iterator it;
    m_bgContainer->removeChildByTag(2000);
    m_bgContainer->removeChildByTag(1000);

    CCSpriteBatchNode *batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("maze_1.png")->getTexture());
    batch->setTag(2000);
    m_bgContainer->addChild(batch);
    
    CCNode *labelNod = CCNode::create();
    labelNod->setTag(1000);
    m_bgContainer->addChild(labelNod);
    int currentIndex = MazeController::getInstance()->openedIdx;
    for(it = MazeController::getInstance()->allPoints.begin(); it != MazeController::getInstance()->allPoints.end(); it++){
        int index = it->first;
        std::string picPath = "";
        
        int startIndex = WorldController::getInstance()->m_monsterAttackInfo->count() / num_per_level * num_per_level;
        auto label = CCLabelIF::create(CC_ITOA(index + startIndex));

        if(index < currentIndex){
            picPath = "NewWorldMonsterView_qi_cheng.png";
            label->setColor(ccc3(239, 167, 42));
        }else if(index == currentIndex){
            picPath = "NewWorldMonsterView_qi_hong.png";
            label->setColor(ccc3(219, 197, 76));
            m_timeToNext = CCLabelIF::create();
            labelNod->addChild(m_timeToNext);

        }else{
            picPath = "NewWorldMonsterView_qi_hui.png";
            label->setColor(ccc3(255, 235, 199));
        }

        CCSprite *sprite = CCLoadSprite::createSprite(picPath.c_str());
        CCPoint pt = MazeController::getInstance()->getRealPosByIndex(it->second) + ccp(0, sprite->getContentSize().height / 2);

        batch->addChild(sprite);
        sprite->setPosition(pt);
        
        auto circle = CCLoadSprite::createSprite("NewWorldMonsterView_kuang.png");
        batch->addChild(circle);
        circle->setPosition(pt + ccp(0, 50));
        
        labelNod->addChild(label);
        label->setPosition(circle->getPosition());
        if(index == currentIndex){
            m_timeToNext->setPosition(pt + ccp(0, -50));

        }
    }
    updateViewTime(1.0f);
}

int MonsterAttackDetailView::getIndex(){
    return m_idx;
}

CCSize MonsterAttackDetailView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx > WorldController::getInstance()->m_monsterAttackInfo->count()){
        return CCSizeZero;
    }
    
    std::string str = "";
    if(idx == WorldController::getInstance()->m_monsterAttackInfo->count()){
        str = _lang("108658");
    }else{
        auto info = dynamic_cast<MonsterSearchResultInfo*>(WorldController::getInstance()->m_monsterAttackInfo->objectAtIndex(idx));
        str = info->infoStr;
    }
    auto label = CCLabelIF::create(str.c_str());
    label->setFontSize(22);
    label->setDimensions(CCSize(580, 0));
    int totalH = label->getContentSize().height * label->getOriginScaleY();
    return CCSize(580, totalH);
}
CCSize MonsterAttackDetailView::cellSizeForTable(CCTableView *table)
{
    return CCSize(600, 30);
}
CCTableViewCell* MonsterAttackDetailView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx > WorldController::getInstance()->m_monsterAttackInfo->count()){
        return NULL;
    }
    
    TextCell* cell = (TextCell*)table->dequeueCell();
    std::string str = "";
    if(idx == WorldController::getInstance()->m_monsterAttackInfo->count()){
        str = _lang("108658");
    }else{
        auto info = dynamic_cast<MonsterSearchResultInfo*>(WorldController::getInstance()->m_monsterAttackInfo->objectAtIndex(idx));
        str = info->infoStr;
    }
    if(idx <= WorldController::getInstance()->m_monsterAttackInfo->count()){
        if(cell){
            cell->setData(str, idx == WorldController::getInstance()->m_monsterAttackInfo->count());
        }else{
            cell = TextCell::create(str, idx == WorldController::getInstance()->m_monsterAttackInfo->count());
        }
    }
    return cell;
}
ssize_t MonsterAttackDetailView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(WorldController::getInstance()->m_monsterAttackInfo->count() / 1.0f) + 1;
    return num;
}
void MonsterAttackDetailView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

//-----

TextCell* TextCell::create(std::string str, bool isLast){
    TextCell *ret = new TextCell(str, isLast);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TextCell::setData(std::string str, bool isLast){
    m_str = str;
    m_isLast = isLast;
    refreshView();
}

bool TextCell::init(){
    m_text = CCLabelIF::create();
    m_text->setAnchorPoint(CCPoint(0, 0));
    m_text->setDimensions(CCSize(580, 0));
    m_text->setColor(ccc3(77, 37, 0));
    m_text->setFontSize(22);
    this->addChild(m_text);
    refreshView();
    return true;
}

void TextCell::onEnter(){
    CCTableViewCell::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void TextCell::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCTableViewCell::onExit();
}

void TextCell::refreshView(){
    m_text->setString(m_str);
    dotCount = 0;
    if(m_isLast){
        update(1.0);
    }
}

void TextCell::update(float time){
    if(m_isLast){
        std::string addDot = "";
        int count = dotCount / 8 + 1;
        if(dotCount >= 47){
            dotCount = 0;
        }
        dotCount++;
        
        int index = 0;
        while(index < count){
            addDot += ".";
            index++;
        }

        m_text->setString(m_str + addDot);
    }
}