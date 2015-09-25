//
//  KoreaPermitPopUPView.cpp
//  IF
//
//  Created by 李锐奇 on 15/3/9.
//
//

#include "KoreaPermitPopUPView.h"


KoreaPermitPopUPView *KoreaPermitPopUPView::create(std::string title, std::string title1, const std::vector<std::string> v1, std::string title2, const std::vector<std::string> v2){
    KoreaPermitPopUPView *ret = new KoreaPermitPopUPView(v1, v2);
    if(ret && ret->init(title, title1, title2)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool KoreaPermitPopUPView::init(std::string title, std::string title1, std::string title2){
    bool ret = false;
    if(PopupBaseView::init()){
        GlobalData::shared()->isBind = true;
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(305, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(305, false);
        });
        m_arr = CCArray::create();
        m_permit1 = false;
        m_permit2 = false;
        auto ccb = CCBLoadFile("permit", this, this);
        this->setContentSize(ccb->getContentSize());
        auto addScrollView = [](std::vector<std::string> v, CCNode *container, CCArray *arr){
            auto cell = KoreaTextTab::create(v, container);
            arr->addObject(cell);
        };
        addScrollView(m_v1, m_viewContainer1, m_arr);
        addScrollView(m_v2, m_viewContainer2, m_arr);
        
        m_title->setString(title);
        m_title1->setString("게임서비스이용약관");
        m_title2->setString("개인정보취급방침");
        CCCommonUtils::setButtonTitle(m_btn1, "게임 이용 약관에 동의합니다");
        CCCommonUtils::setButtonTitle(m_btn2, "개인 정보 수집 및 이용에 동의합니다");
        ret = true;
    }
    return ret;
}

void KoreaPermitPopUPView::onExit(){
    GlobalData::shared()->isBind = false;
    PopupBaseView::onExit();
}

void KoreaPermitPopUPView::onEnter(){
    PopupBaseView::onEnter();
}

SEL_CCControlHandler KoreaPermitPopUPView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn1Click", KoreaPermitPopUPView::onBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn2Click", KoreaPermitPopUPView::onBtn2Click);

    return NULL;
}

bool KoreaPermitPopUPView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title1", CCLabelIF*, m_title1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title2", CCLabelIF*, m_title2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewContainer1", CCNode*, m_viewContainer1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewContainer2", CCNode*, m_viewContainer2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, m_btn2);

    return false;
}

void KoreaPermitPopUPView::onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent){
    m_permit1 = true;
    m_btn1->setEnabled(false);
    doPermit();
}

void KoreaPermitPopUPView::onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent){
    m_permit2 = true;
    m_btn2->setEnabled(false);
    doPermit();
}

bool KoreaPermitPopUPView::checkPermit(){
    if(m_permit1 && m_permit2){
        return true;
    }
    return false;
}

void KoreaPermitPopUPView::doPermit(){
    if(!checkPermit()){
        return;
    }
    CCUserDefault::sharedUserDefault()->setStringForKey("service_terms_check", "ok");
    CCUserDefault::sharedUserDefault()->flush();
    this->closeSelf();
}

//---

KoreaTextTab *KoreaTextTab::create(const std::vector<std::string> v, CCNode *node){
    KoreaTextTab *ret = new KoreaTextTab(v);
    if(ret && ret->init(node)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

CCSize KoreaTextTab::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    int size = m_map.size();
    if(idx > size){
        return CCSizeZero;
    }
    static int count = 0;
    if(m_map.find(idx) != m_map.end()){
        count++;
        CCLOG("total:%d", count);
        return CCSize(500, m_map[idx]);
    }
    std::string str = m_v[idx];
    auto label = CCLabelIF::create(str.c_str());
    label->setFontSize(22);
    label->setDimensions(CCSize(500, 0));
    int totalH = label->getContentSize().height * label->getOriginScaleY();
    m_map[idx] = totalH;
    return CCSize(500, totalH);
}
CCSize KoreaTextTab::cellSizeForTable(CCTableView *table)
{
    return CCSize(600, 30);
}

CCTableViewCell* KoreaTextTab::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    int size = m_map.size();
    if(idx > size){
        return NULL;
    }
    
    std::string str = m_v[idx];
    
    KoreaTextCell* cell = (KoreaTextCell*)table->dequeueCell();

    if(cell){
        cell->setData(str);
    }else{
        cell = KoreaTextCell::create(str);
    }
    return cell;
}

ssize_t KoreaTextTab::numberOfCellsInTableView(CCTableView *table){
    int size = m_map.size();
    return size;
}

void KoreaTextTab::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){
    
}
    
bool KoreaTextTab::init(CCNode *node){
    m_scrollContainer = node;
    m_tabView = CCTableView::create(this, m_scrollContainer->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_scrollContainer->addChild(m_tabView);
    update(1.0f);
    m_tabView->reloadData();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    return true;
}

void KoreaTextTab::update(float time){
    if(m_tabView->getContentOffset().y > -50 && m_map.size() != m_v.size()){
        int count = m_map.size();
        int addPerUpdate = 6;
        int i = 0;
        while (i < addPerUpdate && m_map.size() != m_v.size()) {
            int index = count + i;
            if(m_map.find(index) != m_map.end()){
                continue;
            }else{
                std::string str = m_v[index];
                auto label = CCLabelIF::create(str.c_str());
                label->setFontSize(22);
                label->setDimensions(CCSize(500, 0));
                int totalH = label->getContentSize().height * label->getOriginScaleY();
                m_map[index] = totalH;
            }
            i++;
        }
        CCPoint curr = m_tabView->getContentOffset();
        CCPoint min = m_tabView->minContainerOffset();
        m_tabView->reloadData();
        curr.y += m_tabView->minContainerOffset().y - min.y;
        m_tabView->setContentOffset(curr);
    }
}

KoreaTextTab::~KoreaTextTab(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

//-----

KoreaTextCell* KoreaTextCell::create(std::string str){
    KoreaTextCell *ret = new KoreaTextCell(str);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void KoreaTextCell::setData(std::string str){
    m_str = str;
    refreshView();
}

bool KoreaTextCell::init(){
    m_text = CCLabelIF::create();
    m_text->setAnchorPoint(CCPoint(0, 0));
    m_text->setDimensions(CCSize(500, 0));
    m_text->setColor(ccWHITE);
    m_text->setFontSize(22);
    this->addChild(m_text);
    refreshView();
    return true;
}

void KoreaTextCell::onEnter(){
    CCTableViewCell::onEnter();
}

void KoreaTextCell::onExit(){
    CCTableViewCell::onExit();
}

void KoreaTextCell::refreshView(){
    m_text->setString(m_str);
}