//
//  MailMonsterListView.cpp
//  IF
//
//  Created by lifangkai on 14-11-3.
//
//

#include "MailMonsterListView.h"
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
#include "DynamicResourceController.h"
#include "ChatServiceCocos2dx.h"

MailMonsterListView* MailMonsterListView::create(MailMonsterCellInfo* info ){
    MailMonsterListView* ret = new MailMonsterListView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MailMonsterListView::~MailMonsterListView(){
    //    if(m_mailInfo&&m_mailInfo->collect->count()==0){
    //        map<string, MailInfo*>::iterator it;
    //        it = GlobalData::shared()->mailList.find(m_mailInfo->uid);
    //        if (GlobalData::shared()->mailList.end() != it) {
    //            MailInfo* mail = it->second;
    //            mail->release();
    //            GlobalData::shared()->mailList.erase(it);
    //        }
    //    }
}
bool MailMonsterListView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(206, true);
//    CCLoadSprite::doResourceByCommonIndex(6, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(206, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERLITTLE);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
//        CCLoadSprite::doResourceByCommonIndex(6, false);
        
    });
    auto tmpCCB = CCBLoadFile("MailResourceCCB",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    setTitleName(_lang("105513"));
    m_titleText->setString(_lang("103715").c_str());
    int preHeight = this->m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    if (CCCommonUtils::isIosAndroidPad())
    {
        dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dh));
    m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
    m_downNode->setPositionY(m_downNode->getPositionY() - dh);
    auto battlePic = CCLoadSprite::createSprite("Mail_monster.png");
    this->m_battlePicNode->addChild(battlePic);
    
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    refresh(NULL);
    return true;
}
void MailMonsterListView::refresh(CCObject* obj){
    if(m_mailInfo->save ==0){
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
    setData();
    bool ishasChild = false;
    CCPoint pos;
    if(m_tabView->getContainer()->getChildrenCount()==0){
        ishasChild = true;
    }else{
        pos = m_tabView->getContentOffset();
    }
    m_tabView->reloadData();
    CCPoint minPt = m_tabView->minContainerOffset();
    CCPoint maxPt = m_tabView->maxContainerOffset();
    if (pos.y > maxPt.y) {
        pos.y = maxPt.y;
    }
    if (pos.y < minPt.y) {
        pos.y = minPt.y;
    }
    if(!ishasChild){
        m_tabView->setContentOffset(pos);
    }
}
void MailMonsterListView::readDialogContent(){
    if(m_mailInfo->unread>0){
        MailDialogReadCommand* cmd = new MailDialogReadCommand("",MAIL_ATTACKMONSTER);
        cmd->sendAndRelease();
        GlobalData::shared()->mailCountObj.sysR -=m_mailInfo->unread;
        m_mailInfo->unread = 0;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
}
void MailMonsterListView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("105513"));
    //    setTitleName(m_mailInfo->fromName);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailMonsterListView::refresh), MAIL_MONSTER_LIST_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailMonsterListView::refresh), MAIL_LIST_DELETE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailMonsterListView::refreshAddList), MAIL_LIST_ADD, NULL);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -2, false);
}

void MailMonsterListView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_MONSTER_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_DELETE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_ADD);
    
    setTouchEnabled(false);
    readDialogContent();
    
    PopupBaseView::onExit();
}

bool MailMonsterListView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_tabView->setBounceable(true);
    if(m_isLoadMore&&isTouchInside( m_infoList,pTouch))
        return true;
    else
        return false;
}
void MailMonsterListView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(!m_isLoadMore){
        return;
    }
    m_currMinOffsetY = m_tabView->minContainerOffset().y;
    float offsetY = m_tabView->getContentOffset().y;
    if(offsetY>0&&offsetY>m_currMinOffsetY){
        float y = (pTouch->getLocation().y - pTouch->getStartLocation().y)/5;
        if(y<m_currMinOffsetY){
            y += m_currMinOffsetY;
        }
        m_tabView->setContentOffset(ccp(0,y));
    }
    if(m_isLoadMore && offsetY>-30){
        auto cell = dynamic_cast<MailMonsterCell*>(m_tabView->cellAtIndex(m_data->count()-1));
        if(cell==NULL){
            return;
        }
        CCLabelIF* txt = (CCLabelIF*)cell->getChildByTag(10001);
        txt->setVisible(true);
        if(offsetY>30&&offsetY>m_currMinOffsetY)
            txt->setString(_lang("105318").c_str());
        else
            txt->setString(_lang("115189").c_str());
    }
    return;
    
}
void MailMonsterListView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_isLoadMore&&m_tabView){
        m_currMinOffsetY = m_tabView->minContainerOffset().y;
        m_currOffsetY = m_tabView->getContentOffset().y;
        int dy = m_currMinOffsetY+30;
        if(m_currOffsetY >30&&m_currOffsetY>dy){
            addLoadingAni();
            m_tabView->setBounceable(false);
            this->scheduleOnce(schedule_selector(MailMonsterListView::sendReloadMoreMail),0.0);
        }
    }
}
void MailMonsterListView::sendReloadMoreMail(float _time){
    int realcount = m_mailInfo->monster->count();
    addLoadingAni();
    MailController::getInstance()->reloadMailMore(7,realcount,20);
}
void MailMonsterListView::removeLoadingAni(){
    if(m_loadingIcon){
        m_infoList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
}
void MailMonsterListView::addLoadingAni(){
    if(m_loadingIcon){
        m_infoList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_infoList->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_infoList->addChild(m_loadingIcon,1000000);
}

void MailMonsterListView::refreshAddList(cocos2d::CCObject *obj){
    
    setData();
    removeLoadingAni();
    int add = dynamic_cast<CCInteger*>(obj)->getValue();
    int MinOffsetY = m_tabView->minContainerOffset().y;
    CCPoint pos = m_tabView->getContentOffset();
    int MinOffsetY1 = pos.y;
    if(add==0){
        m_tabView->reloadData();
        m_tabView->setContentOffset(ccp(0,0));
        return;
    }
    m_tabView->reloadData();
    CCPoint minPt = m_tabView->minContainerOffset();
    CCPoint maxPt = m_tabView->maxContainerOffset();
    
    if (MinOffsetY1 > maxPt.y) {
        MinOffsetY1 = maxPt.y;
    }
    
    if (MinOffsetY1 < minPt.y) {
        MinOffsetY1 = minPt.y;
    }
    
    m_newMinOffsetY = m_tabView->minContainerOffset().y;
    if(m_newMinOffsetY<0){
        m_tabView->setContentOffset(ccp(0,m_newMinOffsetY-m_currMinOffsetY));
        float mvdh =m_newMinOffsetY-m_currMinOffsetY + 200;
        if(mvdh>0){
            mvdh = 0;
        }
        m_tabView->setContentOffsetInDuration(ccp(0,mvdh), 0.7);
    }
    
}
SEL_CCControlHandler MailMonsterListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MailMonsterListView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MailMonsterListView::onDeleteClick);
    return NULL;
}

bool MailMonsterListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    return false;
}
void MailMonsterListView::onDeleteClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_mailInfo->save ==1){
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
        return;
    }
    map<string, MailInfo*>::iterator it;
    it = GlobalData::shared()->mailList.find(m_mailInfo->uid);
    if (GlobalData::shared()->mailList.end() == it) {
        return;
    }
    MailMonsterCellInfo* tempInfo = dynamic_cast<MailMonsterCellInfo*>(it->second);
    MailDialogDeleteCommand* command = new MailDialogDeleteCommand("",MAIL_ATTACKMONSTER);
    command->sendAndRelease();
    m_isLoadMore = false;
    GlobalData::shared()->mailCountObj.sysT -=1;
    GlobalData::shared()->mailCountObj.sysR -=tempInfo->unread;
    GlobalData::shared()->mailList.erase(it);
    tempInfo->unread = 0;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    PopupViewController::getInstance()->goBackPopupView();
    CC_SAFE_RELEASE(tempInfo);
    
}
void MailMonsterListView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_mailInfo->monster->count()==0)
        return;
    if(m_mailInfo->save ==1){
        MailDialogSaveCommand *cmd = new MailDialogSaveCommand("", 0,MAIL_ATTACKMONSTER);
        cmd->sendAndRelease();
        m_mailInfo->save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        
        MailDialogSaveCommand *cmd = new MailDialogSaveCommand("", 1,MAIL_ATTACKMONSTER);
        cmd->sendAndRelease();
        m_mailInfo->save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
    
}

void MailMonsterListView::setData(){
    m_data->removeAllObjects();
    if(m_mailInfo->monster->count()<m_mailInfo->totalNum){
        m_isLoadMore = true;
    }else{
        m_isLoadMore = false;
    }
    
    CCObject* obj;
    CCARRAY_FOREACH(m_mailInfo->monster, obj){
        m_data->addObject(obj);
    }
    if(m_isLoadMore){
        MailMonsterInfo* info = MailMonsterInfo::create();
        info->type = 3;
        m_data->addObject(info);
    }
}

void MailMonsterListView::scrollViewDidScroll(CCScrollView* view)
{
    
}

void MailMonsterListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    ChatCell* tmpCell = dynamic_cast<ChatCell*>(cell);
    if (tmpCell) {
        //        tmpCell->cellTouchEnded(m_tabView->m_pTouchedCell);
    }
}
CCSize MailMonsterListView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    int height = 540;
    if (CCCommonUtils::isIosAndroidPad())
    {
        height = 1080;
    }
    if(m_isLoadMore&&idx==m_data->count()-1){
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(1456, 100);
        }
        return CCSize(604,50);
    }
    MailMonsterInfo* info = dynamic_cast<MailMonsterInfo*>(m_data->objectAtIndex(idx));
    if(info->monsterResult==1){
        CCArray *arr = info->normalReward;
        int count = arr->count();
        int rowNum =(count-1)/4+1;
        height = 540+(rowNum-1)*160;
        if (CCCommonUtils::isIosAndroidPad()) {
            height = 1080 + (rowNum - 1) * 320;
        }
    }else if(info->monsterResult==2 || info->monsterResult==4){
        height = 355;
        if (CCCommonUtils::isIosAndroidPad()) {
            height = 710;
        }
    }else{
        CCArray *arr = info->normalReward;
        int count = arr->count();
        int rowNum =(count-1)/4+1;
        height = 450+(rowNum-1)*160;
        if (CCCommonUtils::isIosAndroidPad()) {
            height = 1000 + (rowNum - 1) * 320;
        }
    }
    height+=30;
    if (CCCommonUtils::isIosAndroidPad()) {
        height += 30;
        return CCSize(1456, height);
    }
    return CCSize(604,height);
}

cocos2d::CCSize MailMonsterListView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1456, 1960);
    }
    return CCSize(580, 980);
}

CCTableViewCell* MailMonsterListView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx>=m_data->count()){
        return NULL;
    }
    MailMonsterInfo* dialogInfo = dynamic_cast<MailMonsterInfo*>(m_data->objectAtIndex(idx));
    MailMonsterCell* cell = (MailMonsterCell*)table->dequeueCell();
    if(cell){
        cell->setData(dialogInfo,m_mailInfo,idx);
    }else{
        cell = MailMonsterCell::create(dialogInfo,m_mailInfo,idx);
    }
    return cell;
}

ssize_t MailMonsterListView::numberOfCellsInTableView(CCTableView *table){
    int cellNum = 0;
    cellNum = m_data->count();
    return cellNum;
}


MailMonsterCell* MailMonsterCell::create(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index){
    MailMonsterCell* ret = new MailMonsterCell(dialogInfo,mailInfo,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailMonsterCell::init()
{
    bool ret = true;
//    m_ccbNode = CCBLoadFile("MailResourceCell",this,this);
//    this->setContentSize(CCSizeMake(640, 130));
    m_cellNode = CCNode::create();
    this->addChild(m_cellNode);
    m_tipLoadingText = CCLabelIF::create();
    m_tipLoadingText->setFontSize(22);
    m_tipLoadingText->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    m_tipLoadingText->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_tipLoadingText->setColor({117,62,15});
    m_tipLoadingText->setString(_lang("115189").c_str());
    this->addChild(m_tipLoadingText,10001,10001);
    m_tipLoadingText->setPosition(290, 20);
    m_tipLoadingText->setVisible(false);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_tipLoadingText->setFontSize(44);
        m_tipLoadingText->setPosition(728, 40);
        m_cellNode->setPositionX(8);
    }
    setData(m_dialogInfo,m_mailInfo, m_idx);
    return ret;
}

void MailMonsterCell::setData(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index)
{
    m_dialogInfo=dialogInfo;
    m_mailInfo = mailInfo;
    m_idx = index;
    this->m_cellNode->removeAllChildren();
    m_tipLoadingText->setVisible(false);
    if(m_dialogInfo->type==3){
        m_tipLoadingText->setVisible(true);
        return;
    }
    if(m_dialogInfo->monsterResult==1){
        auto cell = MailMonsterFirstKillCell::create(dialogInfo, mailInfo, index);
        m_cellNode->addChild(cell);
    }else if(m_dialogInfo->monsterResult==2){
        auto cell = MailMonsterFailCell::create(dialogInfo, mailInfo, index,false);
        m_cellNode->addChild(cell);
    }else if(m_dialogInfo->monsterResult==4){
        auto cell = MailMonsterFailCell::create(dialogInfo, mailInfo, index,true);
        m_cellNode->addChild(cell);
    }else{
        auto cell = MailMonsterVictoryCell::create(dialogInfo, mailInfo, index);
        m_cellNode->addChild(cell);
    }
}

void MailMonsterCell::onEnter(){
    CCNode::onEnter();
}

void MailMonsterCell::onExit(){
    CCNode::onExit();
}

SEL_CCControlHandler MailMonsterCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteBtnClick", MailResourceCell::onDeleteBtnClick);
    return NULL;
}

bool MailMonsterCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    return false;
}

//.first kill
MailMonsterFirstKillCell* MailMonsterFirstKillCell::create(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index){
    MailMonsterFirstKillCell* ret = new MailMonsterFirstKillCell(dialogInfo,mailInfo,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailMonsterFirstKillCell::init()
{
    bool ret = true;
    m_ccbNode = CCBLoadFile("MailMonster1Cell",this,this);
    this->setContentSize(CCSizeMake(640, 130));
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setContentSize(m_ccbNode->getContentSize());
    }
    setData(m_dialogInfo,m_mailInfo, m_idx);
    return ret;
}

void MailMonsterFirstKillCell::setData(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index)
{
    m_dialogInfo=dialogInfo;
    m_mailInfo = mailInfo;
    m_idx = index;
    m_titleText->setString(_lang("103758"));
    m_timeText->setString(CCCommonUtils::timeStampToDate(m_dialogInfo->createTime).c_str());
    std::string name = CCCommonUtils::getNameById(m_dialogInfo->monsterId);
    string level = CCCommonUtils::getPropById(m_dialogInfo->monsterId,"level");
    int level1=atoi(level.c_str())+1;
    m_firstkillTipText->setString(_lang_2("103759", level.c_str(),CC_ITOA(level1)));
    name+=" Lv.";
    name+=level;
    m_nameText->setString(name);
    CCPoint pos = WorldController::getPointByIndex(m_dialogInfo->pointId);
    std::string strPos = _lang_2("105521", CC_ITOA(pos.x), CC_ITOA(pos.y));
    m_posTxt->setString(strPos.c_str());

    m_posBG->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_posBG->getContentSize().height));
    m_underlineSpr->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_underlineSpr->getContentSize().height));
    m_rewardTitleText->setString(_lang("114131"));
    
    m_headNode->removeAllChildren();
    string path = CCCommonUtils::getPropById(m_dialogInfo->monsterId,"monster");
    path+=".png";
    auto spr = CCLoadSprite::createSprite(path.c_str(),true,CCLoadSpriteType_MONSTERLAYERLITTLE);
    m_headNode->addChild(spr);
    float scale = 96/spr->getContentSize().width;
    if(scale>1){
        scale = 1.0;
    }
    spr->setScale(scale);
    if(!m_dialogInfo->normalReward){
        return;
    }
    CCArray *arr = m_dialogInfo->normalReward;
    int count = arr->count();
    int rowNum =(count-1)/4+1;
    int i = 0;
    int picWh = 0.9*114;
    int wGap = 140-picWh;
    if (CCCommonUtils::isIosAndroidPad()) {
        wGap = 160 - picWh;
    }
    int startX = 0;
    int startY = -9.7;
    if(count%2==0||(count>4)){
        if(count>2){
            startX =-(wGap/2+picWh/2)*3;
        }else{
            startX =-(wGap/2+picWh/2);
        }
    }else{
        if(count>1){
            startX =-(wGap/2+picWh/2)*2;
        }else{
            startX =0;
        }
    }
    
    while(i < arr->count()){
        CCDictionary *dict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
        int type = 0;
        int value = 0;
        int num = 0;
        type = dict->valueForKey("type")->intValue();
        if(type != R_GOODS){
            value = dict->valueForKey("value")->intValue();
        }else{
            auto good = _dict(dict->objectForKey("value"));
            value = atoi(good->valueForKey("itemId")->getCString());
            num = good->valueForKey("count")->intValue();
            if(good->objectForKey("rewardAdd")){
                num = good->valueForKey("rewardAdd")->intValue();
            }
        }
        int row = i/4;
        int hang = (i%4)*(picWh+wGap);
        auto icon1 = CCLoadSprite::createSprite("icon_kuang.png");
        m_rewardNode->addChild(icon1);
        icon1->setAnchorPoint(ccp(0.5, 0.5));
        CCCommonUtils::setSpriteMaxSize(icon1, picWh);
        icon1->setPosition(CCPoint(startX+hang, startY-row*160));
        
        if(type == R_GOODS){
            CCNode *node = CCNode::create();
            m_rewardNode->addChild(node);
            node->setPosition(CCPoint(startX+hang, startY-row*160));
            CCCommonUtils::createGoodsIcon(value, node, CCSize(80, 80));
//            auto& toolInfo = ToolController::getInstance()->getToolInfoById(value);
//            int color = toolInfo.color;
//            if (color>=0) {
//                auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
//                iconBg->setAnchorPoint(ccp(0.5, 0.5));
//                CCCommonUtils::setSpriteMaxSize(iconBg, 80);
//                m_rewardNode->addChild(iconBg);
//                iconBg->setPosition(CCPoint(startX+hang, startY-row*160));
//            }
        }else{
            auto icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, value).c_str());
            m_rewardNode->addChild(icon);
            icon->setAnchorPoint(ccp(0.5, 0.5));
            CCCommonUtils::setSpriteMaxSize(icon, 96);
            icon->setPosition(CCPoint(startX+hang, startY-row*160));
        }
        std::string namestr = RewardController::getInstance()->getNameByType(type, value);
        auto label = CCLabelIF::create();
        label->setFontSize(21);
        label->setColor(ccc3(133, 83, 4));
        label->setString(namestr.c_str());
        label->setAnchorPoint(ccp(0.5, 0.5));
        label->setPosition(startX+hang, -73.6-row*160);
        m_rewardNode->addChild(label);
        
        std::string numstr = "+";
        if(type == R_GOODS){
            numstr.append(CC_CMDITOA(num));
        }else{
            numstr.append(CC_CMDITOA(value));
        }
        
        auto label1 = CCLabelIF::create();
        label1->setFontSize(21);
        label1->setColor(ccc3(0, 134, 0));
        label1->setString(numstr.c_str());
        label1->setAnchorPoint(ccp(0.5, 0.5));
        label1->setPosition(startX+hang, -97.6-row*160);
        m_rewardNode->addChild(label1);
        //        startX+=(picWh+wGap);
        if (CCCommonUtils::isIosAndroidPad()) {
            label->setFontSize(42);
            label->setScale(0.5);
            label1->setFontSize(42);
            label1->setScale(0.5);
        }
        i++;
    }
    if(rowNum>1){
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_rewardBG->setContentSize(CCSize(m_rewardBG->getContentSize().width, m_rewardBG->getContentSize().height+(rowNum-1)*320));
            this->m_totalNode->setPositionY(this->m_totalNode->getPositionY()+(rowNum-1)*320);
            this->m_firstkillTipText->setPositionY(m_firstkillTipText->getPositionY()-(rowNum-1)*320);
        } else {
            m_rewardBG->setContentSize(CCSize(m_rewardBG->getContentSize().width, m_rewardBG->getContentSize().height+(rowNum-1)*160));
            this->m_totalNode->setPositionY(this->m_totalNode->getPositionY()+(rowNum-1)*160);
            this->m_firstkillTipText->setPositionY(m_firstkillTipText->getPositionY()-(rowNum-1)*160);
        }
    }else{
        
    }
}

void MailMonsterFirstKillCell::onEnter(){
    CCNode::onEnter();
   
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void MailMonsterFirstKillCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}





SEL_CCControlHandler MailMonsterFirstKillCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){

    return NULL;
}

bool MailMonsterFirstKillCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr", CCScale9Sprite*, this->m_underlineSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG", CCNode*, this->m_posBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posTxt", CCLabelIF*, this->m_posTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_firstkillTipText", CCLabelIF*, this->m_firstkillTipText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBG", CCScale9Sprite*, this->m_rewardBG);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTitleText", CCLabelIF*, this->m_rewardTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTitleText", CCLabelIF*, this->m_powerTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerText", CCLabelIF*, this->m_powerText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedTitleText", CCLabelIF*, this->m_woundedTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedText", CCLabelIF*, this->m_woundedText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTitleText", CCLabelIF*, this->m_numTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nunText", CCLabelIF*, this->m_nunText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killTitleText", CCLabelIF*, this->m_killTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killText", CCLabelIF*, this->m_killText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armInfoNode", CCNode*, this->m_armInfoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr", CCScale9Sprite*, this->m_underlineSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    
    return false;
}


bool MailMonsterFirstKillCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchPoint = pTouch->getLocation();
    if(m_dialogInfo->type==MAIL_ATTACKMONSTER&&isTouchInside(m_posBG, pTouch)){
        return true;
    }
    
    return false;
}

void MailMonsterFirstKillCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 10) {
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#endif
    int pos = m_dialogInfo->pointId;
    WorldController::getInstance()->openTargetIndex = pos;
    CCPoint pt = WorldController::getPointByIndex(pos);
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    PopupViewController::getInstance()->removeAllPopupView();
}

void MailMonsterFirstKillCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
//.fail kill
MailMonsterFailCell* MailMonsterFailCell::create(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index, bool miss){
    MailMonsterFailCell* ret = new MailMonsterFailCell(dialogInfo,mailInfo,index);
    if(ret && ret->init(miss)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailMonsterFailCell::init(bool miss)
{
    bool ret = true;
    m_ccbNode = CCBLoadFile("MailMonsterCell",this,this);
    this->setContentSize(CCSizeMake(640, 130));
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(m_ccbNode->getContentSize());
    }
    if(miss){
        m_nodeMiss->setVisible(true);
        m_nodeFail->setVisible(false);
    }else{
        m_nodeFail->setVisible(true);
        m_nodeMiss->setVisible(false);
        
    }
    setData(m_dialogInfo,m_mailInfo, m_idx,miss);
    return ret;
}

void MailMonsterFailCell::setData(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index,bool miss)
{
    m_dialogInfo=dialogInfo;
    m_mailInfo = mailInfo;
    m_idx = index;
    m_timeText->setString(CCCommonUtils::timeStampToDate(m_dialogInfo->createTime).c_str());
    std::string name = CCCommonUtils::getNameById(m_dialogInfo->monsterId);
    string level = CCCommonUtils::getPropById(m_dialogInfo->monsterId,"level");
    m_tipText->setString(_lang_2("103757", level.c_str(),name.c_str()));
    name+=" Lv.";
    name+=level;
    m_nameText->setString(name);
    CCPoint pos = WorldController::getPointByIndex(m_dialogInfo->pointId);
    std::string strPos = _lang_2("105521", CC_ITOA(pos.x), CC_ITOA(pos.y));
    m_posTxt->setString(strPos.c_str());

    m_posBG->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_posBG->getContentSize().height));
    m_underlineSpr->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_underlineSpr->getContentSize().height));
 

    m_headNode->removeAllChildren();
    string path = CCCommonUtils::getPropById(m_dialogInfo->monsterId,"monster");
    path+=".png";
    auto spr = CCLoadSprite::createSprite(path.c_str(),true,CCLoadSpriteType_MONSTERLAYERLITTLE);
    m_headNode->addChild(spr);
    float scale = 96/spr->getContentSize().width;
    if(scale>1){
        scale = 1.0;
    }
    spr->setScale(scale);
    
    if(miss==false){
        m_titleText->setString(_lang("105118"));
        m_woundedTitleText->setString(_lang_1("108600", ""));
        m_woundedText->setString(CC_CMDITOA(m_dialogInfo->total));
        
        std::string powerStr = _lang("102163");
        
        m_powerTitleText->setString(powerStr);
        string powerNumStr =CC_CMDITOA(m_dialogInfo->selfPowerLost);
        powerNumStr ="-"+powerNumStr;
        m_powerText->setString(powerNumStr);
        
        m_killTitleText->setString(_lang("105546").c_str());
        m_killText->setString(CC_CMDITOA(m_dialogInfo->survived));
        m_numTitleText->setString(_lang("105545").c_str());
        m_numText->setString(CC_CMDITOA(m_dialogInfo->hurt));
    }else{
        m_titleText->setString(_lang("103786"));
        m_missText->setString(_lang_1("103787",name.c_str()));
    }
    
}

void MailMonsterFailCell::onEnter(){
    CCNode::onEnter();
   
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void MailMonsterFailCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}





SEL_CCControlHandler MailMonsterFailCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    
    return NULL;
}

bool MailMonsterFailCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr", CCScale9Sprite*, this->m_underlineSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG", CCNode*, this->m_posBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posTxt", CCLabelIF*, this->m_posTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipText", CCLabelIF*, this->m_tipText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTitleText", CCLabelIF*, this->m_powerTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerText", CCLabelIF*, this->m_powerText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedTitleText", CCLabelIF*, this->m_woundedTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedText", CCLabelIF*, this->m_woundedText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTitleText", CCLabelIF*, this->m_numTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText", CCLabelIF*, this->m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killTitleText", CCLabelIF*, this->m_killTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killText", CCLabelIF*, this->m_killText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armInfoNode", CCNode*, this->m_armInfoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_missText", CCLabelIF*, this->m_missText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeMiss", CCNode*, this->m_nodeMiss);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeFail", CCNode*, this->m_nodeFail);
    
    return false;
}


bool MailMonsterFailCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchPoint = pTouch->getLocation();
    if(m_dialogInfo->type==MAIL_ATTACKMONSTER&&isTouchInside(m_posBG, pTouch)){
        return true;
    }
    
    return false;
}

void MailMonsterFailCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 10) {
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#endif
    int pos = m_dialogInfo->pointId;
    WorldController::getInstance()->openTargetIndex = pos;
    CCPoint pt = WorldController::getPointByIndex(pos);
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    PopupViewController::getInstance()->removeAllPopupView();
}

void MailMonsterFailCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

//.victory kill
MailMonsterVictoryCell* MailMonsterVictoryCell::create(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index){
    MailMonsterVictoryCell* ret = new MailMonsterVictoryCell(dialogInfo,mailInfo,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailMonsterVictoryCell::init()
{
    bool ret = true;
    m_ccbNode = CCBLoadFile("MailMonster2Cell",this,this);
    this->setContentSize(CCSizeMake(640, 130));
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setContentSize(m_ccbNode->getContentSize());
    }
    setData(m_dialogInfo,m_mailInfo, m_idx);
    return ret;
}

void MailMonsterVictoryCell::setData(MailMonsterInfo* dialogInfo,MailMonsterCellInfo* mailInfo,int index)
{
    m_dialogInfo=dialogInfo;
    m_mailInfo = mailInfo;
    m_idx = index;
    
    m_titleText->setString(_lang("105117"));
    m_timeText->setString(CCCommonUtils::timeStampToDate(m_dialogInfo->createTime).c_str());
    std::string name = CCCommonUtils::getNameById(m_dialogInfo->monsterId);
    string level = CCCommonUtils::getPropById(m_dialogInfo->monsterId,"level");
    name+=" Lv.";
    name+=level;
    m_nameText->setString(name);
    CCPoint pos = WorldController::getPointByIndex(m_dialogInfo->pointId);
    std::string strPos = _lang_2("105521", CC_ITOA(pos.x), CC_ITOA(pos.y));
    m_posTxt->setString(strPos.c_str());

    m_posBG->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_posBG->getContentSize().height));
    m_underlineSpr->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_underlineSpr->getContentSize().height));
    m_headNode->removeAllChildren();
    string path = CCCommonUtils::getPropById(m_dialogInfo->monsterId,"monster");
    path+=".png";
    auto spr = CCLoadSprite::createSprite(path.c_str(),true,CCLoadSpriteType_MONSTERLAYERLITTLE);
    m_headNode->addChild(spr);
    float scale = 96/spr->getContentSize().width;
    if(scale>1){
        scale = 1.0;
    }
    spr->setScale(scale);

    m_rewardTitleText->setString(_lang("114131"));
    m_woundedTitleText->setString(_lang_1("108600", ""));
    m_woundedText->setString(CC_CMDITOA(m_dialogInfo->total));
    
    std::string powerStr = _lang("102163");

    m_powerTitleText->setString(powerStr);
    string powerNumStr =CC_CMDITOA(m_dialogInfo->selfPowerLost);
    powerNumStr ="-"+powerNumStr;
    m_powerText->setString(powerNumStr);
    
    m_killTitleText->setString(_lang("105546").c_str());
    m_killText->setString(CC_CMDITOA(m_dialogInfo->survived));
    m_numTitleText->setString(_lang("105545").c_str());
    m_numText->setString(CC_CMDITOA(m_dialogInfo->hurt));
//    m_numTitleText->setString(_lang("105546").c_str());
//    m_numText->setString(CC_CMDITOA(m_dialogInfo->survived));
    
    if(!m_dialogInfo->normalReward){
        return;
    }
    CCArray *arr = m_dialogInfo->normalReward;
    int count = arr->count();
    int rowNum =(count-1)/4+1;
    int i = 0;
    int picWh = 0.9*114;
    int wGap = 140-picWh;
    if (CCCommonUtils::isIosAndroidPad())
    {
        wGap = 160 - picWh;
    }
    int startX = 0;
    int startY = -9.7;
    if (CCCommonUtils::isIosAndroidPad())
    {
        startY = 10;
    }
    if(count%2==0||(count>4)){
        if(count>2){
            startX =-(wGap/2+picWh/2)*3;
        }else{
            startX =-(wGap/2+picWh/2);
        }
    }else{
        if(count>1){
            startX =-(wGap/2+picWh/2)*2;
        }else{
            startX =0;
        }
    }
    
    while(i < arr->count()){
        CCDictionary *dict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
        int type = 0;
        int value = 0;
        int num = 0;
        type = dict->valueForKey("type")->intValue();
        if(type != R_GOODS){
            value = dict->valueForKey("value")->intValue();
        }else{
            auto good = _dict(dict->objectForKey("value"));
            value = atoi(good->valueForKey("itemId")->getCString());
            num = good->valueForKey("count")->intValue();
            if(good->objectForKey("rewardAdd")){
                num = good->valueForKey("rewardAdd")->intValue();
            }
        }
        int row = i/4;
        int hang = (i%4)*(picWh+wGap);
        auto icon1 = CCLoadSprite::createSprite("icon_kuang.png");
        m_rewardNode->addChild(icon1);
        icon1->setAnchorPoint(ccp(0.5, 0.5));
        CCCommonUtils::setSpriteMaxSize(icon1, picWh);
        icon1->setPosition(CCPoint(startX+hang, startY-row*160));
        
        if(type == R_GOODS){
            CCNode *node = CCNode::create();
            m_rewardNode->addChild(node);
            node->setPosition(CCPoint(startX+hang, startY-row*160));
            CCCommonUtils::createGoodsIcon(value, node, CCSize(80,80));
//            auto& toolInfo = ToolController::getInstance()->getToolInfoById(value);
//            int color = toolInfo.color;
//            if (color>=0) {
//                auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
//                iconBg->setAnchorPoint(ccp(0.5, 0.5));
//                CCCommonUtils::setSpriteMaxSize(iconBg, 80);
//                m_rewardNode->addChild(iconBg);
//                iconBg->setPosition(CCPoint(startX+hang, startY-row*160));
//            }
        }else{
            int tempValue =value;
            if(R_EXP==type){
                tempValue=-1;
            }
            auto icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, tempValue).c_str());
            m_rewardNode->addChild(icon);
            icon->setAnchorPoint(ccp(0.5, 0.5));
            CCCommonUtils::setSpriteMaxSize(icon, 96);
            icon->setPosition(CCPoint(startX+hang, startY-row*160));
        }
        std::string namestr = RewardController::getInstance()->getNameByType(type, value);
        auto label = CCLabelIF::create();
        label->setFontSize(21);
        label->setColor(ccc3(133, 83, 4));
        label->setString(namestr.c_str());
        label->setAnchorPoint(ccp(0.5, 0.5));
        label->setPosition(startX+hang, -73.6-row*160);
        if (CCCommonUtils::isIosAndroidPad()) {
            label->setFontSize(42);
            label->setScale(0.5);
        }
        m_rewardNode->addChild(label);
        
        std::string numstr = "+";
        if(type == R_GOODS){
            numstr.append(CC_CMDITOA(num));
        }else{
            numstr.append(CC_CMDITOA(value));
        }
        
        auto label1 = CCLabelIF::create();
        label1->setFontSize(21);
        label1->setColor(ccc3(0, 134, 0));
        label1->setString(numstr.c_str());
        label1->setAnchorPoint(ccp(0.5, 0.5));
        label1->setPosition(startX+hang, -97.6-row*160);
        if (CCCommonUtils::isIosAndroidPad())
        {
            label1->setFontSize(42);
            label1->setScale(0.5);
            label->setPositionY(-53.6 - row * 160);
            label1->setPositionY(-77.6 - row * 160);
        }
        m_rewardNode->addChild(label1);
//        startX+=(picWh+wGap);
        i++;
    }
    if(rowNum>1){
        if (CCCommonUtils::isIosAndroidPad()) {
            m_rewardBG->setContentSize(CCSize(m_rewardBG->getContentSize().width, m_rewardBG->getContentSize().height+(rowNum-1)*320));
            this->m_totalNode->setPositionY(this->m_totalNode->getPositionY()+(rowNum-1)*320);
            this->m_armInfoNode->setPositionY(m_armInfoNode->getPositionY()-(rowNum-1)*320);
        } else {
            m_rewardBG->setContentSize(CCSize(m_rewardBG->getContentSize().width, m_rewardBG->getContentSize().height+(rowNum-1)*160));
            this->m_totalNode->setPositionY(this->m_totalNode->getPositionY()+(rowNum-1)*160);
            this->m_armInfoNode->setPositionY(m_armInfoNode->getPositionY()-(rowNum-1)*160);
        }
    }else{
    
    }
}

void MailMonsterVictoryCell::onEnter(){
    CCNode::onEnter();

    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void MailMonsterVictoryCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}





SEL_CCControlHandler MailMonsterVictoryCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    
    return NULL;
}

bool MailMonsterVictoryCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armInfoNode", CCNode*, this->m_armInfoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr", CCScale9Sprite*, this->m_underlineSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBG", CCScale9Sprite*, this->m_rewardBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG", CCNode*, this->m_posBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posTxt", CCLabelIF*, this->m_posTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipText", CCLabelIF*, this->m_tipText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTitleText", CCLabelIF*, this->m_rewardTitleText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTitleText", CCLabelIF*, this->m_powerTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerText", CCLabelIF*, this->m_powerText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedTitleText", CCLabelIF*, this->m_woundedTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedText", CCLabelIF*, this->m_woundedText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTitleText", CCLabelIF*, this->m_numTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText", CCLabelIF*, this->m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killTitleText", CCLabelIF*, this->m_killTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killText", CCLabelIF*, this->m_killText);

    
    return false;
}


bool MailMonsterVictoryCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchPoint = pTouch->getLocation();
    if(m_dialogInfo->type==MAIL_ATTACKMONSTER&&isTouchInside(m_posBG, pTouch)){
        return true;
    }
    
    return false;
}

void MailMonsterVictoryCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 10) {
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#endif
    int pos = m_dialogInfo->pointId;
    WorldController::getInstance()->openTargetIndex = pos;
    CCPoint pt = WorldController::getPointByIndex(pos);
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    PopupViewController::getInstance()->removeAllPopupView();
}

void MailMonsterVictoryCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}
