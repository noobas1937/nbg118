

#include "ChooseUserView.h"
#include "GetAllianceMembersCommand.h"
#include "HFHeadImgNode.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"

#define CHOOSE_USER_CELL_WIDTH 640
#define CHOOSE_USER_CELL_ITEM_HEIGHT 75
#define CHOOSE_USER_CELL_TITLE_HEIGHT 55

#define MSG_CHOOSE_USER_TITLE "msg.choose.user.title"
#define MSG_CHOOSE_USER_LIST_CHANGE "msg.choose.user.list.change"

vector<string> ChooseUserView::ChooseUsers;
int ChooseUserView::m_maxCount = -1;
string ChooseUserView::m_maxDialog = "";
void ChooseUserView::removeFromChooseUsers(std::string uid){
    auto iter = std::find(ChooseUsers.begin(), ChooseUsers.end(), uid);
    if(iter != ChooseUsers.end()){
        ChooseUsers.erase(iter);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHOOSE_USER_LIST_CHANGE);
    }
}
bool ChooseUserView::addToChooseUsers(std::string uid){
    if(m_maxCount>0 && ChooseUsers.size()>= m_maxCount){
        YesNoDialog::showYesDialog(_lang(m_maxDialog).c_str());
        return false;
    }
    ChooseUsers.push_back(uid);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHOOSE_USER_LIST_CHANGE);
    return true;
}
bool ChooseUserView::isInChooseList(std::string uid){
    if(std::find(ChooseUsers.begin(), ChooseUsers.end(), uid) != ChooseUsers.end()){
        return true;
    }
    return false;
}
ChooseUserView* ChooseUserView::create(USER_SOURCE source,int limiteNum,string limiteDialog,string &users){
    ChooseUserView* ret = new ChooseUserView();
    if(ret && ret->init(source,limiteNum,limiteDialog,users)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
ChooseUserView::ChooseUserView():m_tabList(NULL){
    m_maxCount=0;
    m_maxDialog = "";
    ChooseUsers.clear();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChooseUserView::updateButtonState), MSG_CHOOSE_USER_LIST_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChooseUserView::updateList), MSG_CHOOSE_USER_TITLE, NULL);
}
ChooseUserView::~ChooseUserView(){
    m_maxCount=0;
    m_maxDialog = "";
    ChooseUsers.clear();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHOOSE_USER_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHOOSE_USER_TITLE);
}
void ChooseUserView::onEnter(){
    PopupBaseView::onEnter();
}
void ChooseUserView::onExit(){
    PopupBaseView::onExit();
}
bool ChooseUserView::init(USER_SOURCE source,int limiteNum,string limiteDialog,string &users){
    if (!PopupBaseView::init()) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(307, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(307, false);
    });
    ChooseUserView::m_maxCount = limiteNum;
    ChooseUserView::m_maxDialog = limiteDialog;
    
    this->mSource = source;
    auto ccbNode = CCBLoadFile("ChooseUserView", this, this);
    this->setContentSize(ccbNode->getContentSize());
    
    int preHeight = this->m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    this->m_downNode->setPositionY(this->m_downNode->getPositionY() - dh);
    CCSize bgSize = m_sprBG->getContentSize();
    this->m_sprBG->setContentSize(CCSize(bgSize.width, bgSize.height + dh));
    bgSize = m_infoList->getContentSize() + CCSize(0,dh);
    this->m_infoList->setContentSize(bgSize);
    
    
    setTitleName(_lang("110099"));
    setButtonTitle(m_btnCancel, _lang("cancel_btn_label").c_str());
    setButtonTitle(m_btnOK, _lang("confirm").c_str());

    if(!users.empty()){
        std::vector<string> vec;
        CCCommonUtils::splitString(users, ",", vec);
        auto iter = vec.begin();
        while (iter!=vec.end()) {
            ChooseUsers.push_back((*iter));
            ++iter;
        }
    }
    
    generateData();
    updateButtonState(NULL);
    return true;
}
bool ChooseUserView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCScale9Sprite*, this->m_sprBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCancel", CCControlButton*, this->m_btnCancel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOK", CCControlButton*, this->m_btnOK);
    return false;
}
SEL_CCControlHandler ChooseUserView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelClick", ChooseUserView::onCancelClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", ChooseUserView::onOKClick);
    return NULL;
}
bool ChooseUserView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return false;
}
void ChooseUserView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void ChooseUserView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}
CCSize ChooseUserView::tableCellSizeForIndex(CCTableView *table, ssize_t idx){
    if(idx > (m_listSize-1)){
        return CCSizeZero;
    }
    float h = CHOOSE_USER_CELL_TITLE_HEIGHT;
    if(mDatas[idx+1].isOpen){
        h += mDatas[idx+1].mUsers.size() * CHOOSE_USER_CELL_ITEM_HEIGHT;
    }
    
    return CCSize(CHOOSE_USER_CELL_WIDTH,h+5);
}
CCSize ChooseUserView::cellSizeForTable(CCTableView *table){
    return CCSize(CHOOSE_USER_CELL_WIDTH,CHOOSE_USER_CELL_ITEM_HEIGHT);
}
CCTableViewCell* ChooseUserView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    if(idx > (m_listSize-1)){
        return NULL;
    }
    ChooseUserNode* cell = (ChooseUserNode*)table->dequeueCell();
    if(cell){
        cell->setData(mDatas[idx+1]);
    }else{
        cell = ChooseUserNode::create(mDatas[idx+1],m_infoList);
    }
    return cell;
}
ssize_t ChooseUserView::numberOfCellsInTableView(CCTableView *table){
    return m_listSize;
}
void ChooseUserView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){
    
}
void ChooseUserView::updateButtonState(cocos2d::CCObject *data){
    if (ChooseUsers.size()>0) {
        m_btnOK->setEnabled(true);
    }else{
        m_btnOK->setEnabled(false);
    }
}
void ChooseUserView::updateList(CCObject *ccObj){
    if(!ccObj)
        return;
    
    float offsetY = 0;
    int rank = 0;
    CCArray *array = dynamic_cast<CCArray*>(ccObj);
    if(array && array->count()>1){
        CCInteger* intVal = dynamic_cast<CCInteger*>(array->objectAtIndex(0));
        if(intVal){
            rank = intVal->getValue();
        }
        CCBool *boolVal = dynamic_cast<CCBool*>(array->objectAtIndex(1));
        bool show = false;
        if(boolVal){
            show = boolVal->getValue();
        }
        auto iter = mDatas.find(rank);
        if(iter!=mDatas.end()){
            (*iter).second.isOpen = show;
        }
        array->release();
    }
    m_tabList->reloadData();
    
    if(m_tabList->getContainer()->getContentSize().height > m_infoList->getContentSize().height){
        float totalH = 0;
        if(rank>0){
            auto iter1 = mDatas.begin();
            while (iter1!=mDatas.end()) {
                if((*iter1).first > rank){
                    totalH += tableCellSizeForIndex(m_tabList, ((*iter1).first-1)).height;
                }
                ++iter1;
            }
        }
        offsetY = m_tabList->getContentOffset().y+totalH;
        CCPoint offset = ccp(0,offsetY);
        m_tabList->setContentOffset(offset);
    }
}
void ChooseUserView::generateData(){
    switch (mSource) {
        case USER_SOURCE_ALLIANCE:{
            if(GlobalData::shared()->playerInfo.isInAlliance()){
                if(mDatas.size()<=0){
                    GetAllianceMembersCommand * command = new GetAllianceMembersCommand();
                    command->putParam("allianceId", CCString::create(GlobalData::shared()->playerInfo.allianceInfo.uid));
                    command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ChooseUserView::onGetAllianceMembers), NULL));
                    command->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(ChooseUserView::removeLoadingAni), NULL));
                    command->sendAndRelease();
                    this->addLoadingAni();
                }else{
                    refreshView();
                }
            }
            break;
        }
        default:
            break;
    }
}
void ChooseUserView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_infoList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon);
    }
}
void ChooseUserView::removeLoadingAni(CCObject* obj){
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
    m_loadingIcon = NULL;
}
void ChooseUserView::onGetAllianceMembers(CCObject* data){
    this->removeLoadingAni(NULL);
    mDatas.clear();
    vector<string> titles;
    NetResult* result = dynamic_cast<NetResult*>(data);
    CCDictionary* params = _dict(result->getData());
    if (params->objectForKey("rankName")) {
        CCDictionary* rankName = _dict(params->objectForKey("rankName"));
        ChooseUserNodeItem item1;
        item1.rank = 5;
        if (rankName->objectForKey("5")) {
            item1.title = rankName->valueForKey("5")->getCString();
        } else {
            item1.title = _lang("115104");
        }
        mDatas[item1.rank] = item1;
        
        ChooseUserNodeItem item2;
        item2.rank = 4;
        if (rankName->objectForKey("4")) {
            item2.title = rankName->valueForKey("4")->getCString();
        } else {
            item2.title =_lang("115103");
        }
        mDatas[item2.rank] = item2;
        
        ChooseUserNodeItem item3;
        item3.rank = 3;
        if (rankName->objectForKey("3")) {
            item3.title = rankName->valueForKey("3")->getCString();
        } else {
            item3.title = _lang("115102");
        }
        mDatas[item3.rank] = item3;
        
        ChooseUserNodeItem item4;
        item4.rank = 2;
        if (rankName->objectForKey("2")) {
            item4.title = rankName->valueForKey("2")->getCString();
        } else {
            item4.title = _lang("115101");
        }
        mDatas[item4.rank] = item4;
        
        ChooseUserNodeItem item5;
        item5.rank = 1;
        if (rankName->objectForKey("1")) {
            item5.title = rankName->valueForKey("1")->getCString();
        } else {
            item5.title = _lang("115100");
        }
        mDatas[item5.rank] = item5;
    }
    
    std::string myAlliance = GlobalData::shared()->playerInfo.allianceInfo.uid;
    CCArray* members = dynamic_cast<CCArray*>(params->objectForKey("list"));
    if(members){
        for (int i=0; i < members->count(); i++) {
            CCDictionary* member = (CCDictionary*)members->objectAtIndex(i);
            int rank = member->valueForKey("rank")->intValue();
            string name = member->valueForKey("name")->getCString();
            string pic = member->valueForKey("pic")->getCString();
            unsigned long power = member->valueForKey("power")->longValue();
            string uid = member->valueForKey("uid")->getCString();
            int kills = member->valueForKey("kills")->intValue();
            bool onLine = member->valueForKey("online")->boolValue();
            int pointId = member->valueForKey("pointId")->intValue();
            int mainCityLv = member->valueForKey("mainCityLv")->intValue();
            float offLineTime =  member->valueForKey("offLineTime")->floatValue();
            int picVer = member->valueForKey("picVer")->intValue();
            if(uid==GlobalData::shared()->playerInfo.uid){
                continue;
            }
            
            if(pic==""||pic=="0"){
                pic = "g044_middle.png";
            }else{
                pic.append("_middle.png");
            }
            AllianceInfoMember infoMember;
            infoMember.setRank(rank);
            infoMember.setName(name);
            infoMember.setPoints(power);
            infoMember.setKills(kills);
            infoMember.setUid(uid);
            infoMember.setAllianceId(myAlliance);
            infoMember.setOffLineTime(offLineTime);
            infoMember.setPointId(pointId);
            infoMember.setHead(member->valueForKey("pic")->getCString());
            infoMember.setPic(pic);
            infoMember.setOnline(onLine);
            infoMember.setMainCityLv(mainCityLv);
            infoMember.setPower(power);
            infoMember.setIsManager(false);
            infoMember.setIsTitle(false);
            infoMember.setPicVer(picVer);
            
            auto iter = mDatas.find(rank);
            if(iter !=mDatas.end()){
                (*iter).second.mUsers.push_back(infoMember);
            }else{
                CCLog("rank_error: [%s,%d]",uid.c_str(),rank);
            }
        }
        
        auto iter = mDatas.begin();
        m_listSize = 0;
        while (iter!=mDatas.end()) {
            std::sort((*iter).second.mUsers.begin(), (*iter).second.mUsers.end(), AllianceInfoMembersView::compareMember2);
            (*iter).second.isOpen=true;
            ++iter;
            ++m_listSize;
        }
    }
    
    refreshView();
}
void ChooseUserView::refreshView(){
    if(!m_tabList){
        m_tabList = CCTableView::create(this, this->m_infoList->getContentSize());
        m_tabList->setDirection(kCCScrollViewDirectionVertical);
        m_tabList->setVerticalFillOrder(kCCTableViewFillBottomUp);
        m_tabList->setTouchPriority(Touch_Default);
        m_tabList->setDelegate(this);
        m_infoList->addChild(m_tabList);
    }
    if (m_tabList) {
        m_tabList->reloadData();
    }
}
void ChooseUserView::onCancelClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}
void ChooseUserView::onOKClick(CCObject * pSender, Control::EventType pCCControlEvent){
    std::string chooseUID = "";
    auto iter = ChooseUsers.begin();
    while (iter!=ChooseUsers.end()) {
        chooseUID = (iter == ChooseUsers.begin())? (*iter) : (chooseUID + ","+(*iter));
        ++iter;
    }
    
    CCArray *array = CCArray::create();
    array->retain();
    array->addObject(CCString::create(chooseUID));
    std::string chooseName = "";
    auto iter1 = mDatas.begin();
    while (iter1!=mDatas.end()) {
        auto iter2 = (*iter1).second.mUsers.begin();
        while (iter2 != (*iter1).second.mUsers.end()) {
            auto iter0 = find(ChooseUsers.begin(), ChooseUsers.end(), (*iter2).getUid());
            if(iter0 != ChooseUsers.end()){
                chooseName = chooseName.empty()?(*iter2).getName() :(chooseName + ";" + (*iter2).getName());
            }
            ++iter2;
        }
        ++iter1;
    }
    array->addObject(CCString::create(chooseName));
    array->addObject(CCInteger::create(ChooseUsers.size()));
    PopupViewController::getInstance()->goBackPopupView();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_CHOOSE,array);
}



///////////////////////////////////////////////////////////////////
//
//   ChooseUserNode
//
///////////////////////////////////////////////////////////////////
ChooseUserNode* ChooseUserNode::create(ChooseUserNodeItem &data,CCNode *touchNode){
    ChooseUserNode* ret = new ChooseUserNode();
    if(ret && ret->init(data,touchNode)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void ChooseUserNode::onEnter(){
    CCTableViewCell::onEnter();
   
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}
void ChooseUserNode::onExit(){
    setTouchEnabled(false);
    CCTableViewCell::onExit();
}
bool ChooseUserNode::init(ChooseUserNodeItem &data,CCNode *touchNode){
    if (!CCTableViewCell::init()) {
        return false;
    }
    mTouchNode = touchNode;
    mTotalNode = CCNode::create();
    addChild(mTotalNode);
    mSprTitle = CCLoadSprite::createScale9Sprite("Mail_biaoti.png");
    mSprTitle->setCapInsets(CCRect(10,10,10,10));
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    mWidth = winSize.width;
    mTotalNode->addChild(mSprTitle);
    mSprTitle->setPreferredSize(CCSize(winSize.width - 50, CHOOSE_USER_CELL_TITLE_HEIGHT-5));
    mlblTitle = CCLabelTTF::create();
    mlblTitle->setHorizontalAlignment(kCCTextAlignmentLeft);
    mlblTitle->setAnchorPoint(ccp(0,0.5));
    mlblTitle->setFontSize(22);
    mlblTitle->setColor(ccc3(208, 186, 160));
    mTotalNode->addChild(mlblTitle);
    mNode = CCNode::create();
    mTotalNode->addChild(mNode);
    setData(data);
    return true;
}
void ChooseUserNode::setData(ChooseUserNodeItem &data){
    mItem = data;
    mNode->removeAllChildren();
    mlblTitle->setString(data.title.c_str());
    
    int offsetY = 0;
    float maxHeight = 0;
    if(data.isOpen==true){
        auto iter = data.mUsers.begin();
        while (iter!=data.mUsers.end()) {
            bool showLine = (iter == data.mUsers.begin())?false:true;
            ChooseUserCell *nodeItem = ChooseUserCell::create((*iter),showLine,mTouchNode);
            mNode->addChild(nodeItem);
            nodeItem->setPosition(ccp(0, offsetY));
            offsetY -= CHOOSE_USER_CELL_ITEM_HEIGHT;
            ++iter;
        }
        maxHeight = abs(offsetY) ;
        mNode->setPosition(ccp(0, maxHeight - CHOOSE_USER_CELL_ITEM_HEIGHT));
        mNode->setVisible(true);
    }else{
        mNode->setPosition(ccp(0, 0));
        mNode->setVisible(false);
    }
    mSprTitle->setPosition(ccp(CHOOSE_USER_CELL_WIDTH*0.5 - 15,maxHeight + (CHOOSE_USER_CELL_TITLE_HEIGHT-5) * 0.5 + 5));
    mlblTitle->setPosition(ccp(20,mSprTitle->getPositionY()));
}
bool ChooseUserNode::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(mTouchNode, pTouch) && isTouchInside(mSprTitle, pTouch)){
        return true;
    }
    return false;
}
void ChooseUserNode::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void ChooseUserNode::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(mTouchNode, pTouch) && isTouchInside(mSprTitle, pTouch)){
        CCArray *array = CCArray::create();
        array->retain();
        array->addObject(CCInteger::create(mItem.rank));
        bool show = mNode->isVisible()?false:true;
        array->addObject(CCBool::create(show));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHOOSE_USER_TITLE,array);
    }
}



///////////////////////////////////////////////////////////////////
//
//   ChooseUserCell
//
///////////////////////////////////////////////////////////////////
ChooseUserCell* ChooseUserCell::create(AllianceInfoMember &ccObj,bool showLine,CCNode *touchNode){
    ChooseUserCell* ret = new ChooseUserCell();
    if(ret && ret->init(ccObj,showLine,touchNode)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void ChooseUserCell::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}
void ChooseUserCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool ChooseUserCell::init(AllianceInfoMember &ccObj,bool showLine,CCNode *touchNode){
    if(!CCNode::init())
        return false;
    mTouchNode = touchNode;
    ccData = ccObj;
//    CCLayerColor *colorLayer = CCLayerColor::create();
//    colorLayer->setColor(ccRED);
//    colorLayer->setOpacity(120);
//    colorLayer->setContentSize(CCSize(CHOOSE_USER_CELL_WIDTH,CHOOSE_USER_CELL_ITEM_HEIGHT));
//    addChild(colorLayer);
//    colorLayer->setPosition(ccp(0, 0));
    _ignoreAnchorPointForPosition = false;
    setAnchorPoint(Vec2(0,0));
    this->setContentSize(CCSize(CHOOSE_USER_CELL_WIDTH,CHOOSE_USER_CELL_ITEM_HEIGHT));
    mNodeHead = CCNode::create();
    addChild(mNodeHead);
    mNodeHead->setPosition(ccp(50, CHOOSE_USER_CELL_ITEM_HEIGHT*0.5));
    CCSprite *bgHead = CCLoadSprite::createSprite("icon_kuang.png");
    mNodeHead->addChild(bgHead);
    mNodeHead->setScale(0.6);
    mlblName = CCLabelTTF::create();
    mlblName->setFontSize(24);
    mlblName->setColor(ccBLACK);
    mlblName->setAnchorPoint(ccp(0, 0.5));
    addChild(mlblName);
    mlblName->setPosition(ccp(100, CHOOSE_USER_CELL_ITEM_HEIGHT*0.5));
    mSprMarkBG = CCLoadSprite::createSprite("mail_check_bg.png");
    addChild(mSprMarkBG);
    mSprMarkBG->setPosition(ccp(CHOOSE_USER_CELL_WIDTH - 100, CHOOSE_USER_CELL_ITEM_HEIGHT*0.5));
    mSprMark = CCLoadSprite::createSprite("mail_check_icon.png");
    addChild(mSprMark);
    mSprMark->setPosition(mSprMarkBG->getPosition());
    mSprMark->setVisible(false);
    mSprLine = CCLoadSprite::createScale9Sprite("mail_line.png");
    mSprLine->setPreferredSize(CCSize(CHOOSE_USER_CELL_WIDTH,1));
    addChild(mSprLine);
    mSprLine->setVisible(showLine);
    mSprLine->setPosition(ccp(CHOOSE_USER_CELL_WIDTH * 0.5, CHOOSE_USER_CELL_ITEM_HEIGHT-1));
   
    if (CCCommonUtils::isUseCustomPic(ccData.getPicVer())){
        mHeadImgNode = HFHeadImgNode::create();
        mHeadImgNode->initHeadImgUrl2(mNodeHead, CCCommonUtils::getCustomPicUrl(ccData.getUid(), ccData.getPicVer()), 1.0f, 90, false);
    }else{
        CCSprite* head =NULL;
        if(ccData.getHead()==""){
            head = CCLoadSprite::createSprite("guidePlayerIcon.png");
            head->setScale(90.0/head->getContentSize().width);
        }else{
            string mpic = ccData.getHead()+".png";
            head = CCLoadSprite::createSprite(mpic.c_str());
        }
        CCCommonUtils::setSpriteMaxSize(head, 90,true);
        mNodeHead->addChild(head);
    }
    mlblName->setString(ccData.getName().c_str());
    if(ChooseUserView::isInChooseList(ccData.getUid())){
        mSprMark->setVisible(true);
    }
    
    return true;
}
bool ChooseUserCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(mTouchNode, pTouch) && isTouchInside(mSprMarkBG, pTouch)){
        return true;
    }
    return false;
}
void ChooseUserCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void ChooseUserCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(mTouchNode, pTouch) && isTouchInside(mSprMarkBG, pTouch)){
        bool isChoose = false;
        if(ChooseUserView::isInChooseList(ccData.getUid())){
            isChoose = true;
            ChooseUserView::removeFromChooseUsers(ccData.getUid());
        }else{
            if(!ChooseUserView::addToChooseUsers(ccData.getUid())){
                return;
            }
            isChoose = false;
        }
        mSprMark->setVisible(!isChoose);
    }
}
