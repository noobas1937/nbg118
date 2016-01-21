
#include "CommonInclude.h"
#include "PropSpeedupView.h"
#include "PopupViewController.h"
#include "ToolController.h"
#include "FunBuildController.h"
#include "QueueController.h"
#include "CCCommonUtils.h"
#include "SceneController.h"
#include "DragonBuildingController.h"
#include "ImperialScene.h"
#include "YesNoDialog.h"

#define PROP_SPEEDUP_CELL_W 150
PropSpeedupView::~PropSpeedupView(void){
}

PropSpeedupView::PropSpeedupView():m_dragPos(CCPointZero),m_isDrag(false),m_waitInterface(NULL),m_clickIndex(-1),m_totalTime(0),m_editOpen(false),m_chooseCount(0),m_maxCount(0),m_isSendingCMD(false),m_sprSelect(NULL),m_lastClickIndex(-1),m_addTime(0){
}

PropSpeedupView* PropSpeedupView::show(int type,int bid, int qid){
    PropSpeedupView* dialog = new PropSpeedupView();
    dialog->init(type,bid,qid);
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

bool PropSpeedupView::init(int type,int bid, int qid){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    bool bRet=false;
//    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByWorldIndex(2, true);
    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByWorldIndex(2, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    if (CCBLoadFile("PropSpeedupView.ccbi", this, this)) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("stop");
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        m_proptype = type,
        m_qID = qid;
        m_buildingID = bid;
        CCSize nodeSize = m_nodeProp1->getContentSize();
        m_scrollView = CCScrollView::create(nodeSize);
        m_scrollView->setDirection(kCCScrollViewDirectionHorizontal);
        m_scrollView->setTouchEnabled(false);
        //m_scrollView->setDelegate(this);
        m_nodeProp1->addChild(m_scrollView);
        m_lblInfo->setString(_lang("102358").c_str());
        switch (m_proptype) {
            case ItemSpdMenu_Soldier:{ //造兵
                m_lblTitle->setString(_lang("102356").c_str());
                break;
            }
            case ItemSpdMenu_Heal:{//治疗
                m_lblTitle->setString(_lang("102354").c_str());
                break;
            }
            case ItemSpdMenu_Trap:{//造陷阱
                m_lblTitle->setString(_lang("102357").c_str());
                break;
            }
            case ItemSpdMenu_Science:{//科技
                m_lblTitle->setString(_lang("102355").c_str());
                break;
            }
            case ItemSpdMenu_City:{//城建
                if (FunBuildController::getInstance()->curBuildsInfo->find(m_buildingID) != FunBuildController::getInstance()->curBuildsInfo->end()) {
                    auto m_info = &((*FunBuildController::getInstance()->curBuildsInfo)[m_buildingID]);
                    bool isUpgrading = (m_info->state == FUN_BUILD_UPING)?true:false ;
                    if(isUpgrading){
                        m_lblTitle->setString(_lang("102352").c_str());
                    }else{
                        m_lblTitle->setString(_lang("102352").c_str());
                    }

                } else if (DragonBuildingController::getInstance()->curDragonBuildsInfo->find(m_buildingID) != DragonBuildingController::getInstance()->curDragonBuildsInfo->end()) {
                    auto m_info = &((*DragonBuildingController::getInstance()->curDragonBuildsInfo)[m_buildingID]);
                    bool isUpgrading = (m_info->state == FUN_BUILD_UPING)?true:false ;
                    if(isUpgrading){
                        m_lblTitle->setString(_lang("102352").c_str());
                    }else{
                        m_lblTitle->setString(_lang("102352").c_str());
                    }
                }
                break;
            }
            case ItemSpdMenu_DuanZao:{ //锻造
                m_lblTitle->setString(_lang("102373").c_str());
                break;
            }
            default:
                break;
        }
        //auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qID];
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qID];
        m_buildTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
        
        initDataList();
        updateProgressBar(0);
        initSliderBar();
        m_clickIndex=0;
        refreshEditBox();
        m_sprSelect=CCLoadSprite::createScale9Sprite("world_icon_select.png");
        m_sprSelect->setInsetBottom(10);
        m_sprSelect->setInsetTop(10);
        m_sprSelect->setInsetLeft(10);
        m_sprSelect->setInsetRight(10);
        m_sprSelect->setPreferredSize(CCSize(PROP_SPEEDUP_CELL_W, PROP_SPEEDUP_CELL_W));
        m_scrollView->addChild(m_sprSelect);
        m_sprSelect->setZOrder(-1);
        setSelectSpritePosition();
        this->schedule(schedule_selector(PropSpeedupView::updateProgressBar), 1);
        m_lastClickIndex = m_clickIndex;
        m_useBtn->setTouchPriority(0);
        bRet=true;
    }
    return bRet;
}
void PropSpeedupView::setSelectSpritePosition(){
    if(m_sprSelect){
        m_sprSelect->setPosition(ccp(PROP_SPEEDUP_CELL_W * (m_clickIndex), PROP_SPEEDUP_CELL_W * 0.6));
    }
}
void PropSpeedupView::refreshView(CCObject* ccObj){
    if(m_clickIndex > -1 && m_clickIndex<m_Tools.size()){
        
        m_isSendingCMD=false;
        m_sprSelect->removeFromParent();
        m_sprSelect=NULL;
        CCPoint offset = m_scrollView->getContentOffset();
        CCSize nodeSize = m_nodeProp1->getContentSize();
        CCScrollView *scroll = CCScrollView::create(nodeSize);
        scroll->setDirection(kCCScrollViewDirectionHorizontal);
        scroll->setTouchEnabled(false);
        m_nodeProp1->addChild(scroll);
        std::vector<int>::iterator iter = m_Tools.begin();
        int index =0;
        while (iter!=m_Tools.end()) {
            auto &info = ToolController::getInstance()->getToolInfoById((*iter));
            if(info.getCNT()>0){
                PropSpeedupCell* cell = PropSpeedupCell::create(info.itemId);
                scroll->addChild(cell);
                cell->setPosition(ccp(PROP_SPEEDUP_CELL_W * (index + .5), PROP_SPEEDUP_CELL_W * 1.1));
                ++index;
                ++iter;
            }else{
                m_Tools.erase(iter);
            }
        }
        if(m_clickIndex >= m_Tools.size()){
            m_clickIndex = m_Tools.size() - 1;
        }
        scroll->setContentOffset(offset);
        m_scrollView->removeFromParent();
        m_scrollView = scroll;
        m_sprSelect=CCLoadSprite::createScale9Sprite("world_icon_select.png");
        m_sprSelect->setInsetBottom(10);
        m_sprSelect->setInsetTop(10);
        m_sprSelect->setInsetLeft(10);
        m_sprSelect->setInsetRight(10);
        m_sprSelect->setPreferredSize(CCSize(PROP_SPEEDUP_CELL_W, PROP_SPEEDUP_CELL_W));
        m_scrollView->addChild(m_sprSelect);
        m_sprSelect->setZOrder(-1);
        setSelectSpritePosition();
        autoBoundsScroll();
        refreshEditBox();
    }
    
    if(m_Tools.size()==0){
        closeDialog();
    }else{
//        m_clickIndex = -1;
        m_isSendingCMD=false;
        if(m_waitInterface){
            m_waitInterface->removeFromParent();
            m_waitInterface=NULL;
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,  MSG_QUEUE_TIME_UPDATE);
    }
}
void PropSpeedupView::initDataList(){
    m_Tools.clear();
    std::vector<int> tmpSPD;
    std::map<int,ToolInfo>::iterator iter = ToolController::getInstance()->m_toolInfos.begin();
    while (iter!=ToolController::getInstance()->m_toolInfos.end()) {
        auto &info = (*iter).second;
        if(info.type == ITEM_TYPE_SPD && (info.type2 == m_proptype || info.type2 == ItemSpdMenu_ALL) && info.getCNT()>0){
            tmpSPD.push_back(info.itemId);
        }
        ++iter;
    }
    std::sort(tmpSPD.begin(),tmpSPD.end(),sortByOrder);
    std::vector<int>::iterator iter1 = tmpSPD.begin();
    int index = 0;
    while (iter1!=tmpSPD.end()) {
        PropSpeedupCell* cell = PropSpeedupCell::create((*iter1));
        m_scrollView->addChild(cell);
        cell->setPosition(ccp(PROP_SPEEDUP_CELL_W * (index + .5), PROP_SPEEDUP_CELL_W * 1.1));
        m_Tools.push_back((*iter1));
        ++index;
        ++iter1;
    }
    
    m_scrollOffset = m_scrollView->getContentOffset();
}
bool PropSpeedupView::sortByOrder(int itemId1, int itemId2){
    auto& info1 = ToolController::getInstance()->getToolInfoById(itemId1);
    auto& info2 = ToolController::getInstance()->getToolInfoById(itemId2);
    return (info1.orderNum<info2.orderNum);
}


//void PropSpeedupView::scrollViewDidScroll(CCScrollView* view){
//    m_clickIndex = -1;
//}
//void PropSpeedupView::scrollViewDidZoom(CCScrollView* view){
//    
//}
void PropSpeedupView::updateProgressBar(float pUpdate){
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qID];
    float sumTime = qInfo.finishTime - qInfo.startTime;
    float curTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
    
    m_progressTime->setScaleX(1-curTime*1.0/sumTime);
    m_progressTxt->setString(CC_SECTOA(curTime));
}
void PropSpeedupView::initSliderBar(){
//    auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
//    m_sliderBg->setInsetBottom(5);
//    m_sliderBg->setInsetLeft(5);
//    m_sliderBg->setInsetRight(5);
//    m_sliderBg->setInsetTop(5);
//    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
//    m_sliderBg->setPosition(ccp(304/2, 25));
//    m_sliderBg->setContentSize(CCSize(304,18));
//    
//    auto bgSp = CCLoadSprite::createSprite("huadongtiao4.png");
//    bgSp->setVisible(false);
//    auto proSp = CCLoadSprite::createSprite("huadongtiao4.png");
//    auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
//    
//    m_trainSlider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
//    m_trainSlider->setMinimumValue(0.0f);
//    m_trainSlider->setMaximumValue(1.0f);
//    m_trainSlider->setProgressScaleX(300/proSp->getContentSize().width);
//    m_trainSlider->setTag(1);
//    m_trainSlider->setLimitMoveValue(25);
//    m_trainSlider->setTouchPriority(0);
//    m_trainSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(PropSpeedupView::sliderCallBack), CCControlEventValueChanged);
    
    m_trainSlider = NBSlider::create("nb_bar_bg.png", "nb_bar_pro.png", "nb_cursor_icon.png",NBSlider::TextureResType::PLIST);
    m_trainSlider->setCapInsets(Rect(8, 1, 30, 13));
    m_trainSlider->setContentSize(Size(300,15));
    m_trainSlider->addEventListener(CC_CALLBACK_2(PropSpeedupView::sliderCallBack, this));

    m_sliderContainer->addChild(m_trainSlider, 1);
    
    auto editSize = m_editBoxNode->getContentSize();
    auto editpic =CCLoadSprite::createScale9Sprite("frame_3.png");
    editpic->setContentSize(editSize);
    editpic->setInsetBottom(1);
    editpic->setInsetTop(1);
    editpic->setInsetRight(1);
    editpic->setInsetLeft(1);
    m_editBox = CCEditBox::create(editSize,editpic );
    m_editBox->setInputMode(kEditBoxInputModeNumeric);
    m_editBox->setText("0");
    m_editBox->setDelegate(this);
    //m_editBox->setTouchPriority(0);
    m_editBox->setMaxLength(12);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_editBoxNode->addChild(m_editBox);
//    m_lblBtn->setString(_lang("102137"));
    setButtonTitle(m_useBtn, _lang("102137").c_str());
    m_addBtn->setTouchPriority(0);
    m_subBtn->setTouchPriority(0);
}
void PropSpeedupView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    if (getParent() && (getParent()->getChildByTag(BUILDINGVIEW_TAG) || getParent()->getChildByTag(YESNODLG_TAG))){
    }else{
        setTag(YESNODLG_TAG);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PropSpeedupView::checkClose), MSG_QUEUE_REMOVE, NULL);
}
void PropSpeedupView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUEUE_REMOVE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUEUE_TIME_UPDATE);

    CCNode::onExit();
}
void PropSpeedupView::checkClose(CCObject* ccObj){
    CCInteger* pid = dynamic_cast<CCInteger*>(ccObj);
    if(pid && pid->getValue() == m_qID){
        closeDialog();
    }
}
void PropSpeedupView::closeDialog(){
    if(m_waitInterface){
        m_waitInterface->removeFromParent();
        m_waitInterface=NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,  MSG_QUEUE_TIME_UPDATE);
    this->unschedule(schedule_selector(PropSpeedupView::updateProgressBar));
    PopupViewController::getInstance()->removePopupView(this);
}
bool PropSpeedupView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_sprBG, pTouch)) {
        if(m_waitInterface)
            return false;
        
        m_dragPos = pTouch->getLocation();
        if(isTouchInside(m_nodeProp, pTouch)){
            CCPoint pos = m_scrollView->getContainer()->convertToNodeSpace(m_dragPos);
            m_clickIndex = floor(pos.x / PROP_SPEEDUP_CELL_W);
            m_scrollOffset = m_scrollView->getContentOffset();
        }else{
            //m_clickIndex = -1;
//            if (isTouchInside(m_btnNode, pTouch)){
//                m_btnNode->setScale(0.9);
//            }
        }
        return true;
    }
    m_dragPos = CCPointZero;
    closeDialog();
    return false;
}
void PropSpeedupView::autoBoundsScroll(){
    CCPoint offset = m_scrollView->getContentOffset();
    if(offset.x>0){
        offset.x = 0;
    }else{
        int maxW = (int)m_Tools.size() * PROP_SPEEDUP_CELL_W;
        int nodeW = m_nodeProp1->getContentSize().width;
        if(maxW <nodeW){
            offset.x = 0;
        }else{
            int aaa = nodeW - maxW;
            if(offset.x < aaa){
                offset.x = aaa;
            }
        }
    }
    m_scrollOffset = offset;
    m_scrollView->setContentOffsetInDuration(m_scrollOffset, 0.2);
}
void PropSpeedupView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
//    if (isTouchInside(m_btnNode, pTouch)){
//        useTool();
//    }else{
        if(m_isDrag == false && m_clickIndex > -1 && m_clickIndex < m_Tools.size()){
            if(m_lastClickIndex != m_clickIndex){
                setSelectSpritePosition();
                playAnimation();
                m_lastClickIndex=m_clickIndex;
            }
        }else if(m_isDrag){
//            m_clickIndex = -1;
            autoBoundsScroll();
            m_isDrag=false;
        }
//    }
//    m_btnNode->setScale(1);
}
void PropSpeedupView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_editNode, pTouch)){
        
    }else{
        CCPoint movePoint = pTouch->getLocation();
        float distance = movePoint.x - m_dragPos.x;
        if(fabs(distance)>50){
            m_isDrag = true;
//            m_clickIndex=-1;
            CCPoint adjustPoint = ccp(m_scrollOffset.x + distance, 0);
            m_scrollView->setContentOffset(adjustPoint, false);
        }else{
            if(isTouchInside(m_nodeProp, pTouch)){
                CCPoint pos = m_scrollView->getContainer()->convertToNodeSpace(pTouch->getLocation());
                m_clickIndex = floor(pos.x / PROP_SPEEDUP_CELL_W);
            }
        }
    }
}
SEL_MenuHandler PropSpeedupView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler PropSpeedupView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", PropSpeedupView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", PropSpeedupView::onSubClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUseClick", PropSpeedupView::onUseClick);
    return NULL;
}
bool PropSpeedupView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCNode*, this->m_sprBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeTime", CCNode*, this->m_nodeTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTime", CCScale9Sprite*, this->m_progressTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt", CCLabelIF*, this->m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeProp", CCNode*, this->m_nodeProp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeProp1", CCNode*, this->m_nodeProp1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblInfo", CCLabelIF*, this->m_lblInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle", CCLabelIF*, this->m_lblTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, this->m_editNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBtn", CCSprite*, this->m_sprBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblBtn", CCLabelIF*, this->m_lblBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBoxNode", CCNode*, this->m_editBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderContainer", CCNode*, this->m_sliderContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, this->m_addBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBtn", CCControlButton*, this->m_subBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTimeTip", CCLabelIF*, this->m_lblTimeTip);
    return false;
}
void PropSpeedupView::keypressedBtnLast(CCObject * pSender, Control::EventType pCCControlEvent) {
}
void PropSpeedupView::keypressedBtnNext(CCObject * pSender, Control::EventType pCCControlEvent) {
}
void PropSpeedupView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    std::string temp  =text;
    size_t pos = temp.find_first_not_of("1234567890");
    while (pos!=std::string::npos) {
        temp.replace(pos, 1, "");
        pos = temp.find_first_not_of("1234567890");
    }
    int a = atoi(temp.c_str());
    std::string aaaa = CC_CMDITOA(a);
    editBox->setText(aaaa.c_str());
}
void PropSpeedupView::sliderCallBack(Ref *pSender, NBSlider::EventType type){
    m_chooseCount = m_trainSlider->getValue() * m_maxCount;
    if(m_chooseCount>m_maxCount){
        m_chooseCount = m_maxCount;
    }
    if(m_chooseCount == 0){
        m_chooseCount = 1;
    }
    m_editBox->setText(CC_CMDITOA(m_chooseCount).c_str());
    m_editBox->setScale(0);
    m_editBox->setScale(1);
    if(m_chooseCount>0){
        CCCommonUtils::setSpriteGray(m_sprBtn, false);
    }else{
        CCCommonUtils::setSpriteGray(m_sprBtn, true);
    }
    if(m_maxCount<=0){
        m_trainSlider->setEnabled(false);
    }else{
        m_trainSlider->setEnabled(true);
    }
    string tip = _lang("102457");
    string time = CC_SECTOA(m_addTime * m_chooseCount);
    tip.append(time);
    m_lblTimeTip->setString(tip.c_str());
    
}
CCNode* PropSpeedupView::getGuideNode()
{
    return m_useBtn;
}
void PropSpeedupView::editBoxReturn(CCEditBox *editBox){
    string numStr = m_editBox->getText();
    if(!numStr.empty()){
        size_t pos = numStr.find(",");
        while(pos!=string::npos){
            numStr.replace(pos, 1, "");
            pos=numStr.find(",");
        }
    }
    int num = atoi(numStr.c_str());
    if(num>m_maxCount){
        num = m_maxCount;
        
    }else if(num<0){
        num=0;
    }
    m_chooseCount = num;
    if(m_maxCount<=0){
        m_trainSlider->setValue(m_trainSlider->getMinimumValue());
    }else{
        m_trainSlider->setValue(1.0f * num / m_maxCount);
    }
    if(m_maxCount<=0){
        m_trainSlider->setEnabled(false);
    }else{
        m_trainSlider->setEnabled(true);
    }
}
void PropSpeedupView::onUseClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    useTool();
}
void PropSpeedupView::onAddClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    float value  = m_trainSlider->getValue() * m_maxCount+1;
    if(m_maxCount<=0){
        m_trainSlider->setValue(m_trainSlider->getMinimumValue());
    }else{
        m_trainSlider->setValue(1.0f * value / m_maxCount);
    }
}
void PropSpeedupView::onSubClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    float value = m_trainSlider->getValue() * m_maxCount-1;
    if(m_maxCount<=0){
        m_trainSlider->setValue(m_trainSlider->getMinimumValue());
    }else{
        m_trainSlider->setValue(1.0f * value / m_maxCount);
    }
}
void PropSpeedupView::playAnimation(){
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(PropSpeedupView::animationCallback));
    this->getAnimationManager()->runAnimationsForSequenceNamed("change");
    this->runAction(CCSequence::create(CCDelayTime::create(0.1),CCCallFunc::create(this, callfunc_selector(PropSpeedupView::refreshEditBox)),NULL));
}
void PropSpeedupView::refreshEditBox(){
    if(m_clickIndex > -1 && m_clickIndex < m_Tools.size()){
        int propID = m_Tools[m_clickIndex];
        auto &info = ToolController::getInstance()->getToolInfoById(propID);
        m_maxCount = info.getCNT();
        std::vector<std::string> paras;
        CCCommonUtils::splitString(info.getPara(), ";", paras);
        if(paras.size()>2){
            m_addTime = atoi(paras.at(2).c_str());
        }else{
            m_addTime = 0;
        }
        
        m_maxCount = MIN(m_maxCount, ceil(m_buildTime/m_addTime));
        float minVal = 1.0/m_maxCount;
        m_trainSlider->setMinimumValue(minVal);
        m_trainSlider->setMaximumValue(1);
        m_trainSlider->setValue(1);
    }
}
void PropSpeedupView::animationCallback(){
    this->getAnimationManager()->runAnimationsForSequenceNamed("stop");
}
void PropSpeedupView::useTool(){
    if(m_chooseCount<=0)
        return;
    if(m_clickIndex == -1 || m_clickIndex >= m_Tools.size() || m_addTime == 0)
        return;
    
    bool showTip =false;
//    float sumTime = 0;
    float curTime = 0;
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qID];
//    sumTime = qInfo.finishTime - qInfo.startTime;
    curTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
    int addT = m_addTime*m_chooseCount;
    if(curTime < addT){
        showTip=true;
    }else{
        showTip=false;
    }
    if(showTip==true){
#pragma mark temp
        YesNoDialog::show(_lang("101036").c_str(), CCCallFunc::create(this, callfunc_selector(PropSpeedupView::sureToUseTool)));
    }else{
        sureToUseTool();
    }
}
void PropSpeedupView::sureToUseTool(){
    if(m_isSendingCMD==true)
        return;
    m_isSendingCMD=true;
    
    
    int propID = m_Tools[m_clickIndex];
    auto &info = ToolController::getInstance()->getToolInfoById(propID);
    float sumTime = 0;
    float curTime = 0;
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qID];
    sumTime = qInfo.finishTime - qInfo.startTime;
    curTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
    int addT = m_addTime * m_chooseCount;
    if(addT<=0){
        m_isSendingCMD=false;
        return;
    }
    if((addT-curTime)>=m_addTime){
        m_chooseCount = ceil(curTime/m_addTime);
        addT = m_addTime * m_chooseCount;
    }
    if(curTime <= addT){
        closeDialog();
        m_buildTime=0;
    }else{
        auto &ccArr = m_scrollView->getContainer()->getChildren();
        for(auto child:ccArr)
        {
            PropSpeedupCell *cell = dynamic_cast<PropSpeedupCell*>(child);
            if(cell && cell->getItemID() == propID){
                m_waitInterface=GameController::getInstance()->showWaitInterface(m_sprBG);
                CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PropSpeedupView::refreshView), MSG_QUEUE_TIME_UPDATE, NULL);
                break;
            }
        }
        m_buildTime = curTime - addT;
    }
    int state = 0;
    int type = 0;
    if (FunBuildController::getInstance()->curBuildsInfo->find(m_buildingID) != FunBuildController::getInstance()->curBuildsInfo->end()) {
        type = 1;
        state = FunBuildController::getInstance()->curBuildsInfo->at(m_buildingID).state;
    } else if (DragonBuildingController::getInstance()->curDragonBuildsInfo->find(m_buildingID) != DragonBuildingController::getInstance()->curDragonBuildsInfo->end()) {
        type = 2;
        state = DragonBuildingController::getInstance()->curDragonBuildsInfo->at(m_buildingID).state;
    }
    if (state == FUN_BUILD_NORMAL) {
        QueueController::getInstance()->startCCDQueue(m_qID, CC_ITOA(propID), false,0,"",1,"",m_chooseCount);
        int toolCount = info.getCNT() - m_chooseCount;
        info.setCNT(toolCount);
        string addTip = info.getName();
//        if(m_chooseCount>1){
            addTip.append(string(" x") + CC_ITOA(m_chooseCount));
//        }
        CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(info.itemId)), "", _lang_1("104909",addTip.c_str()));
        auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
        auto flyHit = layer->getChildByTag(19999);
        if(flyHit){
            flyHit->setPositionY(flyHit->getPositionY() + 100);
        }
    } else {
        if (type == 1) {
            if (FunBuildController::getInstance()->costCD(m_buildingID, CC_ITOA(info.itemId),0,m_chooseCount)) {
                string addTip = info.getName();
//                if(m_chooseCount>1){
                    addTip.append(string(" x") + CC_ITOA(m_chooseCount));
//                }
                CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(info.itemId)), "", _lang_1("104909",addTip.c_str()));
                auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
                auto flyHit = layer->getChildByTag(19999);
                if(flyHit){
                    flyHit->setPositionY(flyHit->getPositionY() + 100);
                }
            }
        }
        else if (type == 2) {
            if (DragonBuildingController::getInstance()->costCD(m_buildingID, CC_ITOA(info.itemId))) {
                string addTip = info.getName();
//                if(m_chooseCount>1){
                    addTip.append(string(" x") + CC_ITOA(m_chooseCount));
//                }
                CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(info.itemId)), "", _lang_1("104909",addTip.c_str()));
                auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
                auto flyHit = layer->getChildByTag(19999);
                if(flyHit){
                    flyHit->setPositionY(flyHit->getPositionY() + 100);
                }
            }
        }
    }
    
    
//    m_clickIndex = -1;
}

/////////////////////////////////////////////
//
//              PropSpeedupCell
//
/////////////////////////////////////////////

PropSpeedupCell* PropSpeedupCell::create(int itemId)
{
    auto ret = new PropSpeedupCell();
    if (ret && ret->init(itemId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

PropSpeedupCell::PropSpeedupCell(){
}

bool PropSpeedupCell::init(int itemId)
{
    CCBLoadFile("PropSpeedupCell",this,this);
    mitemID = itemId;
    CCCommonUtils::createGoodsIcon(mitemID, m_nodeIcon, CCSize(95, 95),m_lblCount,m_lblName);
    
//    auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
//    string name = _lang(toolInfo.name);
//    auto picBG = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//    //CCCommonUtils::setSpriteMaxSize(picBG, 110, true);
//    m_nodeIcon->addChild(picBG);
//    string picStr =  CCCommonUtils::getIcon(CC_ITOA(itemId));
//    auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_GOODS);
//    //CCCommonUtils::setSpriteMaxSize(icon, 110, true);
//    m_nodeIcon->addChild(icon);
//    m_lblName->setString(name);
//    m_lblCount->setString(CC_CMDITOA(toolInfo.getCNT()));
    bool ret = true;
    return ret;
}

void PropSpeedupCell::onEnter() {
    CCNode::onEnter();
}

void PropSpeedupCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler PropSpeedupCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool PropSpeedupCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblName", CCLabelIF*, m_lblName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblCount", CCLabelIF*, m_lblCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeIcon", CCNode*, m_nodeIcon);
    return false;
}