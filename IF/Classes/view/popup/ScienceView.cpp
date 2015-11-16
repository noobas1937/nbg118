//
//  ScienceView.cpp
//  IF
//
//  Created by 邹 程 on 13-11-1.
//
//

#include "ScienceView.h"
#include "ScienceCommand.h"
#include "YesNoDialog.h"
#include "PopupViewController.h"
#include "ScienceListView.h"
#include "UseCDToolView.h"
#include "QueueController.h"
#include "ScienceController.h"

#include "GeneralSkillListPopUpView.h"
#include "FunBuildController.h"
#include "ParticleController.h"
#include "ConfirmDialogView.h"
#include "ScienceCommand.h"
#include "CCMathUtils.h"
#include "SoundController.h"

#define SCIENCE_TYPE_DOMESTIC 1
#define SCIENCE_TYPE_WARFARE  2
#define BATCH_TAG 457

#define MSG_ADD_SCIENCE_POPUP "_science_add"
#define MSG_REMOVE_SCIENCE_POPUP "_science_remove"

bool isReturn = true;
ScienceView* ScienceView::create(int buildId, int scienceType){
    ScienceView* ret = new ScienceView();
    if(ret && ret->init(buildId, scienceType)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ScienceView::~ScienceView()
{
//    m_scrollView->removeAllChildren();
//    m_lineBatch = NULL;
//    m_commonBatch = NULL;
//    m_itemBatch = NULL;
//    m_iconBatch = NULL;
//    CCLoadSprite::doResourceByCommonIndex(4, false);
//    CCLoadSprite::doResourceByCommonIndex(5, false);
}

bool ScienceView::init(int buildId, int scienceType)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    isReturn = true;
    m_buildId = buildId;
    m_scienceType = scienceType;
    
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        CCLoadSprite::doResourceByCommonIndex(5, false);
        CCLoadSprite::doResourceByCommonIndex(6, false);//fusheng
    });
    
//    auto tbg = CCLoadSprite::loadResource("technology_09.png");//fusheng 背景图片
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//    int curHight = -500;
//    while (curHight<maxHight) {
//        auto bg = CCLoadSprite::createSprite("technology_09.png");
//        if (CCCommonUtils::isIosAndroidPad()) {
//            bg->setScale(2.4);
//        }
//        bg->setAnchorPoint(ccp(0, 1));
//        bg->setPosition(ccp(0, curHight));
//        curHight += bg->getContentSize().height;
//        tBatchNode->addChild(bg);
//    }
//    this->addChild(tBatchNode);
    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(5, true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    auto tmpCCB = CCBLoadFile("ScienceTreeView",this,this);//ScienceView
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    changeBGMaxHeight(m_buildBG);
    
    setTitleName(CCCommonUtils::getNameById(CC_ITOA(m_scienceType)));
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int addHeight = getExtendHeight();
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_huaWenBottom->setPositionY(m_huaWenBottom->getPositionY()-addHeight);//fusheng
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - addHeight);
        this->m_bgContainer->setPositionY(this->m_bgContainer->getPositionY() - addHeight);

    }
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_scrollView);
    m_desLabel->setString(_lang("121990"));
    
    
    float m_kejiBG33Top  = m_kejiBG33->convertToWorldSpaceAR(Vec2::ZERO).y;
    float m_kejiBG33Bottom = m_kjBG33BottomNode->convertToWorldSpaceAR(Vec2::ZERO).y;
    m_kejiBG33->setContentSize(Size(m_kejiBG33->getContentSize().width,m_kejiBG33Top-m_kejiBG33Bottom));
    reInitView();
//    addBGPic();//fusheng 这里会崩溃 "UI_UseSkill_picture_blackwhite.png"  图片丢失
    
//    for (int i=1; i<=4; i++) {
//        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//        m_fireNode1->addChild(particle);
//        
//        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
//        m_fireNode2->addChild(particle1);
//    }

    return true;
}

void ScienceView::reInitView(CCObject* obj)
{

    
    auto spLine = CCLoadSprite::loadResource("kji_line.png");//science_line.png
//    m_lineBatch = CCSpriteBatchNode::createWithTexture(spLine->getTexture());
    m_lineBatch = Node::create();//fusheng 修改为Node
    m_scrollView->addChild(m_lineBatch);
    
    auto sp = CCLoadSprite::loadResource("technology_08.png");//science_line.png
    m_itemBatch = CCSpriteBatchNode::createWithTexture(sp->getTexture());
    m_scrollView->addChild(m_itemBatch);
    
    auto cellBG = CCLoadSprite::loadResource("technology_08.png");
    m_commonBatch = CCSpriteBatchNode::createWithTexture(cellBG->getTexture());
    m_scrollView->addChild(m_commonBatch);
    
    auto iconBC = CCLoadSprite::loadResource("science_def_a_1.png");
    m_iconBatch = CCSpriteBatchNode::createWithTexture(iconBC->getTexture());
    m_scrollView->addChild(m_iconBatch);
    
    if (GlobalData::shared()->scienceInfo.IsHasData) {
        updateInfo(NULL);
    }
    else {
        ScienceDataInitCommand * cmd = new ScienceDataInitCommand();
        cmd->sendAndRelease();
    }
}

void ScienceView::updateInfo(CCObject* obj)
{
    //<ItemSpec id="229990" name="107100" icon="ico229990" science="220000;220100;220200;220300;220400;220500;220600;220700;220800;220900;221000;221100"/>
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_scienceType));
    string scienceIds = dict->valueForKey("science")->getCString();
    std::vector<string> sciList;
    CCCommonUtils::splitString(scienceIds, ";", sciList);
    
    idList.clear();//fusheng 先清一下
   
    int maxPosY = 0;
    for (int i=0; i<sciList.size(); i++) {
        int scienceId = atoi(sciList[i].c_str());
        idList.push_back(scienceId);
        int curY = GlobalData::shared()->scienceInfo.infoMap[scienceId].posY;
        if (curY > maxPosY) {
            maxPosY = curY;
        }
    }
    
    int _itemH = 160;
    int _itemW = 80;
    int curY = 10;
    int curX = 0;
    if (CCCommonUtils::isIosAndroidPad()) {
        _itemH = 340;
        _itemW = 160;
        curY = 0;
        curX = 125;
    }
    
    int maxHeight = _itemH * maxPosY;
    if (CCCommonUtils::isIosAndroidPad()) {
        maxHeight += 0;
    }
    else
        maxHeight += 50;
    
    
    this->m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,maxHeight));
    this->m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - maxHeight));
    
    for (int i=0; i<idList.size(); i++) {
        
        
        auto& info = GlobalData::shared()->scienceInfo.infoMap[idList[i]];
        
        auto cellTemp = cells[info.posX*10 + info.posY];
        if(cellTemp)
        {
            cellTemp->refreash(nullptr);
        }
        else
        {
            string relation = info.relation;
            int posx = curX + (info.posX-1)*_itemW;
            int posy = curY + (maxPosY - info.posY)*_itemH;
            auto cell = ScienceCell::create(idList[i], m_commonBatch, m_iconBatch, posx, posy);
            cell->setPosition(ccp(posx,posy));
            cell->setTouchNode(m_infoList);
            m_scrollView->addChild(cell);
            
            cells[info.posX*10 + info.posY] = cell;//fusheng 把cell存起来  x*10+y
        }
        
       
    }
    newAddLine(&idList, maxPosY);
}

const int st_w = 80;
const int st_h = 160;
const int st_H = 130;
const int st_w_HD = 160;
const int st_h_HD = 340;
const int st_H_HD = 280;

void ScienceView::newAddLine(vector<int>* idList, int maxHeight)
{
    
    m_lineBatch->removeAllChildrenWithCleanup(true);//fusheng 删除原本的sprite
    
    auto addLine = [this](const CCPoint &startPt, std::vector<CCPoint> &endPt, std::vector<bool> openFlag, CCNode *parent, int maxH,int index){
        if (CCCommonUtils::isIosAndroidPad()) {
            int curX = 140;
            int curY = -120;
            auto startRealPt = ccp((startPt.x) * st_w_HD + curX, (maxH-startPt.y) * st_h_HD+st_H_HD + curY);
            int index = 0;
            while (index < endPt.size()) {
                auto &pt = endPt[index];
                auto endRealPt = ccp((pt.x) * st_w_HD + curX, (maxH - pt.y) * st_h_HD + st_H_HD + curY);
                auto len = ccpDistance(endRealPt, startRealPt);
                auto lineBG = CCLoadSprite::createSprite("kji_line.png");
               
                parent->addChild(lineBG);
                lineBG->setAnchorPoint(ccp(0.5, 0));
                lineBG->setPosition(startRealPt);
                float angle = -CCMathUtils::getAngle(startRealPt, endRealPt) + 90;
                lineBG->setRotation(angle);
                float s = len / lineBG->getContentSize().height;
                lineBG->setScaleY(s);
                
              
                
                index++;
            }
        }
        else {
            auto startRealPt = ccp((startPt.x) * st_w, (maxH-startPt.y) * st_h+st_H);
            int index = 0;
            while (index < endPt.size()) {
                auto &pt = endPt[index];
                auto endRealPt = ccp((pt.x) * st_w, (maxH - pt.y) * st_h + st_H);
                auto len = ccpDistance(endRealPt, startRealPt);
                auto lineBG = CCLoadSprite::createSprite("kji_line.png");
                parent->addChild(lineBG);
                lineBG->setAnchorPoint(ccp(0.5, 0));
                lineBG->setPosition(startRealPt);
                float angle = -CCMathUtils::getAngle(startRealPt, endRealPt) + 90;
                lineBG->setRotation(angle);
                float s = len / lineBG->getContentSize().height;
                lineBG->setScaleY(s);
                
                
                auto cell = this->cells[pt.x*10+pt.y];
                
                if (cell&&cell->m_isOpen) {
                    auto lineBG2 = CCLoadSprite::createSprite("kji_line2.png");
                    parent->addChild(lineBG2);
                    lineBG2->setAnchorPoint(ccp(0.5, 0));
                    lineBG2->setPosition(startRealPt);
                    lineBG2->setRotation(angle);
                    lineBG2->setScaleY(s);
                }
                
                

                
                index++;
            }
        }
    };
    
    for (int i=0; i<idList->size(); i++) {
        auto& info = GlobalData::shared()->scienceInfo.infoMap[(*idList)[i]];
        
        bool isOpen = false;
        if(info.scienceLv>=1){
            isOpen=true;
        }
        
        string relation = info.relation;
        map<int, vector<int>> relationMap;
        vector<int> relationVecY;
        vector<string> relationPts;
        vector<CCPoint> endVec;
        vector<bool> relVec;
        CCCommonUtils::splitString(relation, "|", relationPts);
        for (int j=0; j<relationPts.size(); j++) {
            vector<string> tmpPt;
            CCCommonUtils::splitString(relationPts[j], ";", tmpPt);//fusheng  耍我啊  y在前 x在后
            
            int ptX = atoi(tmpPt[1].c_str());
            int ptY = atoi(tmpPt[0].c_str());
            endVec.push_back(ccp(ptX, ptY));
            relVec.push_back(isOpen);
        }
        addLine(ccp(info.posX, info.posY), endVec, relVec, m_lineBatch, maxHeight,i);
    }
}

void ScienceView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScienceView::updateInfo), MSG_SCIENCE_DATA_REFRESH, NULL);
    UIComponent::getInstance()->showCloseBtn(true);
    
    setTitleName(CCCommonUtils::getNameById(CC_ITOA(m_scienceType)));
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScienceView::updateInfo), MSG_SCIENCE_RESEARCH_FINISH, NULL);
    
    if(m_openNum>0) {
        PopupViewController::getInstance()->removePopupView(this);
        PopupViewController::getInstance()->addPopupInView(ScienceView::create(m_buildId, m_scienceType));
    }
    m_openNum++;
}

void ScienceView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_DATA_REFRESH);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_CELL_STATE);
    
    if (m_openNum<=1) {
        m_scrollView->removeAllChildren();
        m_lineBatch = NULL;
        m_commonBatch = NULL;
        m_itemBatch = NULL;
        m_iconBatch = NULL;
    }
    
    UIComponent::getInstance()->showCloseBtn(false);
    
    CCLoadSprite::doResourceByCommonIndex(4, false);
    CCLoadSprite::doResourceByCommonIndex(5, false);
    
    CCNode::onExit();
}

void ScienceView::addBGPic(){
    int totalH = 0;
    auto size = CCDirector::sharedDirector()->getWinSize();
    while (totalH < size.height) {
//        auto sprite = CCLoadSprite::createSprite("kji_bg.png");
        auto sprite = CCCommonUtils::addFilterSprite("UI_UseSkill_picture_blackwhite.png", CCCommonUtils::covertHSBToRGB(55, 0.77, 1.0), 0, 1.04, 4.4);

        if (CCCommonUtils::isIosAndroidPad()) {
            sprite->setScale(2.4);
        }
        this->m_bgContainer->addChild(sprite);
        sprite->setAnchorPoint(ccp(0, 0));
        sprite->setPosition(ccp(0, totalH));
        totalH += sprite->getContentSize().height * sprite->getScaleY();
    }
}


SEL_CCControlHandler ScienceView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool ScienceView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, this->m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_huaWenBottom", CCSprite*, this->m_huaWenBottom);
    

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kejiBG33", CCScale9Sprite*, this->m_kejiBG33);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kjBG33BottomNode", CCNode*, this->m_kjBG33BottomNode);
    return false;
}

CCNode *ScienceView::getGuideNode(int sid){
    auto& arr = m_scrollView->getContainer()->getChildren();
    for(auto child : arr)
    {
        ScienceCell *cell = dynamic_cast<ScienceCell*>(child);
        if(cell && cell->m_scienceId == sid){
            return cell;
        }
    }
    
//    int i = 0;
//    while(i < arr->count()){
//        ScienceCell *cell = dynamic_cast<ScienceCell*>(arr->objectAtIndex(i++));
//        if(cell && cell->m_scienceId == sid){
//            return cell;
//        }
//    }
    return NULL;
}

void ScienceView::setAttentionTo(int sid){
    if(sid != 0){
        auto& arr = m_scrollView->getContainer()->getChildren();
        for(auto child : arr)
        {
            ScienceCell *cell = dynamic_cast<ScienceCell*>(child);
            if(cell && cell->m_scienceId == sid){
                cell->setAttention();
                int totalH = m_scrollView->getContentSize().height;
                m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - totalH + (totalH - cell->getPositionY()) - CCDirector::sharedDirector()->getWinSize().height / 2));
                
                break;
            }
        }
//        int i = 0;
//        while(i < arr->count()){
//            ScienceCell *cell = dynamic_cast<ScienceCell*>(arr->objectAtIndex(i++));
//            if(cell && cell->m_scienceId == sid){
//                cell->setAttention();
//                int totalH = m_scrollView->getContentSize().height;
//                m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - totalH + (totalH - cell->getPositionY()) - CCDirector::sharedDirector()->getWinSize().height / 2));
//                
//                break;
//            }
//        }
    }
}

void ScienceView::doWhenClose()
{
    if (isReturn) {
        PopupViewController::getInstance()->addPopupInView(ScienceListView::create(m_buildId));
    }
}

///////
ScienceCell* ScienceCell::create(int scienceId, CCSpriteBatchNode* batch, CCSpriteBatchNode* iconBatch, int posx, int posy) {
    auto ret = new ScienceCell();
    if (ret && ret->init(scienceId, batch, iconBatch, posx, posy)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ScienceCell::~ScienceCell()
{
}

bool ScienceCell::init(int scienceId, CCSpriteBatchNode* batch, CCSpriteBatchNode* iconBatch, int posx, int posy)
{
    m_scienceId = scienceId;
    m_batch = batch;
    m_iconBatch = iconBatch;
    m_posx = posx;
    m_posy = posy;
    
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
    });
    
    m_ccbNode = CCBLoadFile("ScienceCellView",this,this);//ScienceCellView
    
    this->setContentSize(m_ccbNode->getContentSize());
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setAnchorPoint(ccp(0.5, 0.5));
    }
    m_nameLabel->setString(CCCommonUtils::getNameById(CC_ITOA(scienceId)).c_str());
//    m_bg->removeFromParent();
//    m_bg->setPosition(ccpAdd(m_bg->getPosition(), ccp(posx, posy)));
//    m_batch->addChild(m_bg);
//    m_stateLabel->setString("");
    
    m_buildLv = 0;
    int bId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SCIENE);
    if (bId>0) {
        auto& bInfo = FunBuildController::getInstance()->getFunbuildById(bId);
        m_buildLv = bInfo.level;
    }
    
    refreash(NULL);
    onEnterFrame(0);
    return true;
}

void ScienceCell::setAttention(){
    auto sprite = CCLoadSprite::createScale9Sprite("rect.png");
    if (CCCommonUtils::isIosAndroidPad()) {
        sprite->setScale(2);
    }
    this->addChild(sprite);
    sprite->setScale(this->getContentSize().width / sprite->getContentSize().width);
    sprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    sprite->setOpacity(0);
    CCSequence *seque = CCSequence::create(CCFadeIn::create(0.3), CCFadeOut::create(0.3) ,NULL);
    CCSequence *seque1 = CCSequence::create(CCRepeat::create(seque, 5), CCRemoveSelf::create(), NULL);
    sprite->runAction(seque1);
}

void ScienceCell::refreash(CCObject* obj)
{
    bool tmpSt = false;
    if(obj) {
        int sid = dynamic_cast<CCInteger*>(obj)->getValue();
        if (sid == m_scienceId) {
            m_particleNode->removeAllChildren();
            string tmpStart = "ScienceOut_";
            int count = 9;
            for (int i=1; i<count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                m_particleNode->addChild(particle);
                tmpSt = true;
            }
        }
    }
    
    m_newIcon->setVisible(false);
    
    auto& info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    string tmpStr = CC_ITOA(info.scienceLv);
    tmpStr = tmpStr + "/" + CC_ITOA(info.scienceMaxLv);
    m_lvLabel->setString(tmpStr);
    
    if (info.scienceLv == info.scienceMaxLv) {
//        m_lvLabel->setColor(ccc3(0, 190, 0));
//        m_nameLabel->setColor(ccc3(0, 190, 0));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Complete");
    }else if (tmpSt) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("ResearchFadeOut_Yellow");
    }
    
    if(m_isInit && m_isOpen) {
        return;
    }
    
    string picName = CCCommonUtils::getIcon(CC_ITOA(m_scienceId));
    if (info.scienceLv<=0 && GlobalData::shared()->scienceInfo.infoMap[m_scienceId].getCondition()>0) {
        if (!m_isInit) {
            m_isInit = true;
            m_picNode->removeAllChildren();
            auto grayPic = CCLoadSprite::createSprite(picName.c_str());
            CCCommonUtils::setSpriteGray(grayPic, true);
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(grayPic, 240, true);
            }
            else
                CCCommonUtils::setSpriteMaxSize(grayPic, 120, true);
            CCCommonUtils::setSpriteGray(m_bg, true);
            m_picNode->addChild(grayPic);
            m_nameLabel->setColor(ccGRAY);
            m_lvLabel->setColor(ccGRAY);
            m_type = -1;
        }
        m_isOpen = false;
    }
    else {
        if (!m_isInit || !m_isOpen) {
            m_picNode->removeAllChildren();
            auto pic = CCLoadSprite::createSprite(picName.c_str());
            if (pic) {
                if (CCCommonUtils::isIosAndroidPad()) {
                    CCCommonUtils::setSpriteMaxSize(pic, 240, true);
                }
                else
                    CCCommonUtils::setSpriteMaxSize(pic, 120, true);
                m_picNode->addChild(pic);
            }
            m_isInit = true;
            m_isOpen = true;
            CCCommonUtils::setSpriteGray(m_bg, false);//fusheng 刷新时原本 灰度图 没有变回去
            auto& m_info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
            resItems.clear();
            CCCommonUtils::splitString(m_info.resCondition, "|", resItems);
            buildItems.clear();
            CCCommonUtils::splitString(m_info.buildCondition, "|", buildItems);
            
            if (m_info.scienceLv==0)
            {
                string newKey = CCUserDefault::sharedUserDefault()->getStringForKey(CC_ITOA(m_scienceId),"");
                if (newKey == "1") {
                    m_newIcon->setVisible(true);
                }
//                string _sid = CCUserDefault::sharedUserDefault()->getStringForKey(CC_ITOA(m_scienceId),"");
//                if (_sid == "") {
//                    this->getAnimationManager()->runAnimationsForSequenceNamed("Lock");
//                    string tmpStart = "ScienceGlow_";
//                    int count = 4;
//                    for (int i=3; i<count; i++) {
//                        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//                        m_particleNode->addChild(particle);
//                    }
//                    m_type = 2;
//                }
//                else {
                    m_type = 0;
//                }
            }
            else {
                m_type = 0;
            }
        }
    }
}

void ScienceCell::refreashState(CCObject* obj)
{
}

void ScienceCell::setTouchNode(CCNode* node)
{
    m_touchNode = node;
}

bool ScienceCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(is_playing) {
        return false;
    }
    if (m_touchNode && isTouchInside(m_touchNode, pTouch) && isTouchInside(m_mainNode, pTouch)) {
        auto tmpPt = pTouch->getLocation();
        m_startY = tmpPt.y;
        return true;
    }
    return false;
}

void ScienceCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    auto tmpPt = pTouch->getLocation();
    if(fabs(tmpPt.y - m_startY) >= 20){
        return;
    }

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("SC2_%d", m_scienceId));
    
//    if(m_type == 2 && m_particleNode->getChildrenCount()>0 && m_open==0) {
//        m_open = 1;
//        CCUserDefault::sharedUserDefault()->setStringForKey(CC_ITOA(m_scienceId), CC_ITOA(m_scienceId));
//        CCUserDefault::sharedUserDefault()->flush();
//        
//        is_playing = true;
//        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(ScienceCell::animationFunc));
//        this->getAnimationManager()->runAnimationsForSequenceNamed("LockOut");
//        scheduleOnce(schedule_selector(ScienceCell::addBoomb), 2.0f);
//        m_particleNode->removeAllChildren();
//    }
//    else {
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupView(SciencePopupView::create(m_scienceId));
    if (m_newIcon->isVisible()) {
        CCUserDefault::sharedUserDefault()->setStringForKey(CC_ITOA(m_scienceId),"");
        CCUserDefault::sharedUserDefault()->flush();
        m_newIcon->setVisible(false);
    }
    
//    }
}

void ScienceCell::animationFunc()
{
    is_playing = false;
    string name = this->getAnimationManager()->getLastCompletedSequenceName();
    if (name == "LockOut") {
        m_particleNode->removeAllChildren();
        PopupViewController::getInstance()->addPopupView(SciencePopupView::create(m_scienceId));
    }
}

void ScienceCell::addBoomb()
{
    string tmpStart = "ScienceOut_";
    int count = 9;
    for (int i=1; i<count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        m_particleNode->addChild(particle);
    }
}

void ScienceCell::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ScienceCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, true);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScienceCell::refreash), MSG_SCIENCE_RESEARCH_FINISH, NULL);
    


}

void ScienceCell::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ScienceCell::onEnterFrame), this);
    setTouchEnabled(false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_RESEARCH_FINISH);

    
    if(m_picNode) {
        m_picNode->removeAllChildren();
    }
    m_batch->removeAllChildren();
    m_batch = NULL;
    m_iconBatch->removeAllChildren();
    m_iconBatch = NULL;
    CCNode::onExit();
}

SEL_CCControlHandler ScienceCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool ScienceCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSelNode", CCNode*, this->m_unSelNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selNode", CCNode*, this->m_selNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvNode", CCNode*, this->m_lvNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stateLabel", CCLabelIF*, this->m_stateLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCSprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockSpr", CCSprite*, this->m_lockSpr);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvBG", CCScale9Sprite*, this->m_lvBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newIcon", CCSprite*, this->m_newIcon);
    
    return false;
}

void ScienceCell::onEnterFrame(float dt)
{
    auto& info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    if (info.scienceLv == info.scienceMaxLv) {
        return;
    }
    
    if(m_isOpen) {
        int lastTime = info.finishTime - GlobalData::shared()->getWorldTime();
        if (lastTime>0) {
            if(m_type != 1) {
                m_type=1;
                m_particleNode->removeAllChildren();
//                string tmpStart = "ScienceGlow_";
//                int count = 4;
//                for (int i=1; i<count; i++) {
//                    auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//                    m_particleNode->addChild(particle);
//                }
                this->getAnimationManager()->runAnimationsForSequenceNamed("ResearchFadeIn_Yellow");
            }
        }
        else {
            bool m_isOk = true;
            for (int i=0; i<resItems.size(); i++) {
                vector<string> resCell;
                CCCommonUtils::splitString(resItems[i], ";", resCell);
                int resType = atoi(resCell[0].c_str());
                int resNum = atoi(resCell[1].c_str());
                m_isOk = CCCommonUtils::isEnoughResourceByType(resType, resNum);
                if (!m_isOk) {
                    break;
                }
            }
            
            bool _tmpLock = true;
            for (int i=0; i<buildItems.size(); i++) {
                int buildId = atoi(buildItems[i].c_str());
                int lv = buildId%1000;
                buildId = buildId-lv;
                if (lv<=m_buildLv) {// !FunBuildController::getInstance()->isExistBuildByTypeLv(buildId, lv)
                    _tmpLock = false;
                    break;
                }
            }
            m_lockSpr->setVisible(_tmpLock);
            
            if (m_isOk) {
                m_nameLabel->setColor(ccc3(183, 194, 254));
                m_lvLabel->setColor(ccc3(0, 0, 0));
            }
            else {
//                m_nameLabel->setColor(ccc3(164, 28, 28));
//                m_lvLabel->setColor(ccc3(164, 28, 28));
                m_nameLabel->setColor(ccc3(183, 194, 254));
                m_lvLabel->setColor(ccc3(0, 0, 0));
            }
            
            if(m_type != 0 && m_type != 2) {
                m_type=0;
            }
        }
    }
    else {
        if ( GlobalData::shared()->scienceInfo.infoMap[m_scienceId].CanResearch() ) {
            refreash(NULL);
        }
    }
}



SciencePopupView* SciencePopupView::create(int scienceId) {
    auto ret = new SciencePopupView();
    if (ret && ret->init(scienceId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SciencePopupView::init(int scienceId) {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        setModelLayerTouchPriority(Touch_Popup_Item);
        m_scienceId = scienceId;
        
        CCBLoadFile("ScienceUpView",this,this);//SciencePopupView
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        CCLoadSprite::doResourceByCommonIndex(5, true);

        
        
        m_scrollView = CCScrollView::create(m_infoList->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_infoList->addChild(m_scrollView);
        
        updateInfo(NULL);
        ret = true;
    }
    m_instantBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    
    m_upBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    
    m_confitionLabel->setString(_lang("102138"));
 
    return ret;
}

void SciencePopupView::updateInfo(CCObject* obj)
{
    int type = 0;
    m_nameLabel->setString(CCCommonUtils::getNameById(CC_ITOA(m_scienceId)).c_str());
    string des = CCCommonUtils::getPropById(CC_ITOA(m_scienceId), "description");
    m_desLabel->setString(_lang(des).c_str());
    
    string picName = CCCommonUtils::getIcon(CC_ITOA(m_scienceId));
    
    auto& info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    string tmpStr = CC_ITOA(info.scienceLv);
    tmpStr = tmpStr + "/" + CC_ITOA(info.scienceMaxLv);
    m_barLabel->setString(tmpStr.c_str());
    float pro = 0;
    if (info.scienceMaxLv>0) {
        pro = info.scienceLv*1.0/info.scienceMaxLv;
    }
    m_bar->setScaleX(pro);
    if (info.scienceLv >= info.scienceMaxLv) {
        type = 2;
    }
    
    _isOpen = true;
    m_picNode->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    if (pic) {
        if (info.getCondition() > 0 ) {
            CCCommonUtils::setSpriteGray(pic, true);
            _isOpen = false;
            type = 3;
        }
        CCCommonUtils::setSpriteMaxSize(pic, 120, true);
        m_picNode->addChild(pic);
    }
    
    if (info.finishTime > GlobalData::shared()->getWorldTime()) {
        CCCommonUtils::setButtonTitle(m_upBtn, _lang("104903").c_str());
        m_btnMsgLabel->setString(_lang("102102").c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            float fw = m_btnMsgLabel->getContentSize().width;
            float s = m_btnMsgLabel->getScaleX();
            if (fw>360.0) {//防爆框
                m_btnMsgLabel->setScaleX(360.0/fw*s);
            }
        }
        else
            m_btnMsgLabel->setMaxScaleXByWidth(180.0);
        m_infoNode->setVisible(false);
        CCCommonUtils::setButtonTitle(m_instantBtn, _lang("108532").c_str());
//        CCCommonUtils::setButtonSprite(m_instantBtn, "mail_red_btn.png");//fusheng 竟然换图片
        m_isUping = true;
        type = 1;
    }
    else {
        CCCommonUtils::setButtonTitle(m_upBtn, _lang("121994").c_str());
        m_btnMsgLabel->setString(_lang("102102").c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            float fw = m_btnMsgLabel->getContentSize().width;
            float s = m_btnMsgLabel->getScaleX();
            if (fw>360.0) {//防爆框
                m_btnMsgLabel->setScaleX(360.0/fw*s);
            }
        }
        else
            m_btnMsgLabel->setMaxScaleXByWidth(180.0);
        CCCommonUtils::setButtonTitle(m_instantBtn, "");
//        CCCommonUtils::setButtonSprite(m_instantBtn, "btn_green3.png");//fusheng 竟然换图片
        m_infoNode->setVisible(true);
        m_isUping = false;
    }
    
    vector<string> curPara;
    CCCommonUtils::splitString(info.para2, "|", curPara);
    vector<string> nextPara;
    CCCommonUtils::splitString(info.nextPara2, "|", nextPara);
    string curStr = "";
    if (curPara.size()>0) {
        curStr = curPara[0];
    }
    string nextStr = "";
    if (nextPara.size()>0) {
        nextStr = nextPara[0];
    }
    if (info.show == 1) {
        curStr += "%";
        nextStr += "%";
    }
    m_curLabel->setString(_lang_1("121995",""));
    m_nextLabel->setString(_lang_1("121996",""));
    m_curNumLabel->setString(curStr);
    m_nextNumLabel->setString(nextStr);
    
    onUpdateRes(type);
}

void SciencePopupView::onUpdateRes(int type)
{
    bool isCanUp = true;
    auto& m_info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    
    int _tmpTime = m_info.time_need/(1+CCCommonUtils::getEffectValueByNum(69)*1.0/100);
    _tmpGold = CCCommonUtils::getGoldByTime(_tmpTime);
    string timeInfo = CC_SECTOA(_tmpTime);
    m_timeLabel->setString(timeInfo.c_str());
    
    _resGold = 0;
    m_lfood = 0;
    m_lwood = 0;
    m_liron = 0;
    m_lstone = 0;
    
    int _itemH = 46;
    int curY = 25;
    int curX = 30;
    if (CCCommonUtils::isIosAndroidPad()) {
        _itemH = 92;
        curY = 50;
        curX = 40;
    }
    m_scrollView->getContainer()->removeAllChildren();
    m_inBtnGoldNum->setString("9999");
    if (type==0 || type==3) {
        vector<string> resItems;
        CCCommonUtils::splitString(m_info.resCondition, "|", resItems);
        for (int i=0; i<resItems.size(); i++) {
            vector<string> resCell;
            CCCommonUtils::splitString(resItems[i], ";", resCell);
            int resType = atoi(resCell[0].c_str());
            int resNum = atoi(resCell[1].c_str());
            
            if(resNum>0) {
                auto item1 = UpgradeCell::create(0, resType, resNum, true);
                item1->setPosition(ccp(curX, curY));
                item1->setTouchNode(m_infoList,m_scienceId, 1);
                m_scrollView->addChild(item1);
                curY += _itemH;
                
                if (!item1->m_isOk) {
//                    isCanUp = false;
                }
                
                int tmpSub = resNum - CCCommonUtils::getCurResourceByType(resType);
                if (tmpSub>0) {
                    int subGold = CCCommonUtils::getResGlodByType(resType, tmpSub);
                    _tmpGold += subGold;
                    _resGold += subGold;
                    if (resType == Food) {
                        m_lfood = tmpSub;
                    }
                    else if (resType == Wood) {
                        m_lwood = tmpSub;
                    }
                    else if (resType == Iron) {
                        m_liron = tmpSub;
                    }
                    else if (resType == Stone) {
                        m_lstone = tmpSub;
                    }
                }
            }
        }
        m_inBtnGoldNum->setString(CC_ITOA(_tmpGold));
        
        if (type==3 || m_info.scienceLv<=0) {
            vector<string> scienceConItem;
            CCCommonUtils::splitString(m_info.scienceCondition, ";", scienceConItem);
            for (int i=0; i<scienceConItem.size(); i++) {
                int curCond = atoi(scienceConItem[i].c_str());
                int scienceConditionLv = curCond%100;
                int scienceConditionId = curCond - scienceConditionLv;
                
                auto item1 = UpgradeCell::create(5, scienceConditionId, scienceConditionLv);
                item1->setPosition(ccp(curX, curY));
                item1->setTouchNode(m_infoList,m_scienceId, 1);
                m_scrollView->addChild(item1);
                curY += _itemH;
                
                if (!item1->m_isOk) {
                    isCanUp = false;
                }
            }
        }
        
        vector<string> buildItems;
        CCCommonUtils::splitString(m_info.buildCondition, "|", buildItems);
        for (int i=0; i<buildItems.size(); i++) {
            int buildId = atoi(buildItems[i].c_str());
            auto item1 = UpgradeCell::create(2, buildId, 0, true);
            item1->setPosition(ccp(curX, curY));
            item1->setTouchNode(m_infoList,m_scienceId, 1);
            m_scrollView->addChild(item1);
            curY += _itemH;
            
            if (!item1->m_isOk && !m_isUping) {
                isCanUp = false;
//                m_instantBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"), CCControlStateDisabled);
                m_instantBtn->setEnabled(false);
            }
        }
        
        int qid = QueueController::getInstance()->canMakeItemByType(TYPE_SCIENCE, _tmpTime);
        if(QID_MAX == qid)
        {
            int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
            auto itemQueue = UpgradeCell::create(3, Food, qid,true);
            itemQueue->setPosition(ccp(curX, curY));
            itemQueue->setTouchNode(m_infoList,m_scienceId, 1);
            m_scrollView->addChild(itemQueue);
            curY += _itemH;
        }

    }
    else {
//        if (!_isOpen) {
//            vector<string> scienceConItem;
//            CCCommonUtils::splitString(m_info.scienceCondition, ";", scienceConItem);
//            auto &scienceMap = GlobalData::shared()->scienceInfo.infoMap;
//            vector<string> vecInfo;
//            for (int i=0; i<scienceConItem.size(); i++) {
//                int curCond = atoi(scienceConItem[i].c_str());
//                int scienceConditionLv = curCond%100;
//                int scienceConditionId = curCond - scienceConditionLv;
//                if (scienceMap.find(scienceConditionId) == scienceMap.end() || scienceMap[scienceConditionId].scienceLv < scienceConditionLv ) {
//                    string name = CCCommonUtils::getNameById(CC_ITOA(scienceConditionId));
//                    string tmp_Info = _lang_2("121989", name.c_str(), CC_ITOA(scienceConditionLv));// 121989
//                    vecInfo.push_back(tmp_Info);
//                }
//            }
//            
//            curY = (m_infoList->getContentSize().height-_itemH*vecInfo.size())/2;
//            for (int j=0; j<vecInfo.size(); j++) {
//                auto label = CCLabelIF::create(vecInfo[j].c_str(), 24, CCSizeMake(460, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//                label->setColor(ccRED);
//                int labelW = label->getContentSize().width*label->getOriginScaleX();
//                label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
//                m_scrollView->addChild(label);
//                curY += _itemH;
//            }
//            
//            curY += _itemH;
//            curY -= 20;
//            curY = m_infoList->getContentSize().height;
//        }
    }
    m_upBtn->setEnabled(isCanUp);
    
    curY -= 23;
    if (CCCommonUtils::isIosAndroidPad()) {
        curY -= 46;
    }
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
    if(curY <= m_infoList->getContentSize().height) {
        m_scrollView->setTouchEnabled(false);
    }
    
    m_timeBarNode->setVisible(false);
    _lastTime = -1;
    if(m_isUping) {
        m_upBtn->setEnabled(true);
        m_instantBtn->setEnabled(true);
        _lastTime = m_info.finishTime - GlobalData::shared()->getWorldTime();
        _sumTime = m_info.time_need;
    }
    else {
        if (!_isOpen) {
            m_upBtn->setEnabled(false);
//            m_instantBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"), CCControlStateDisabled);
            m_instantBtn->setEnabled(false);
        }
    }
    onChangeBG(type);
    onEnterFrame(0);
}

void SciencePopupView::onChangeBG(int type)
{
    if (type==1 || type==2) {
        m_bigBG->setVisible(false);
        m_smallBG->setVisible(true);
        m_listBG->setVisible(false);
        m_timeNode->setPosition(Vec2::ZERO);
        
        if (type==1) {//正在升级
            m_upBtn->setPositionX(0);
            m_infoNode->setPosition(m_upBtn->getPosition());
            CCCommonUtils::setButtonTitle(m_upBtn, "");
            m_btnMsgLabel->setString(_lang("104903"));
            m_inBtnGoldNum->setString("99");
            m_maxMsgLabel->setString("");
            m_infoNode->setVisible(true);
            
            m_instantBtn->setVisible(false);
            m_instantBtn->setEnabled(false);
        }
        else {//达到最大级
            m_btnNode->setVisible(false);
            m_maxMsgLabel->setString(_lang("121987"));
            m_nextNode->setVisible(false);
        }
        m_mainNode->setPositionY(m_mainNode->getPositionY()-200);
    }
    else {
        m_bigBG->setVisible(true);
        m_smallBG->setVisible(false);
        m_listBG->setVisible(true);
        
        m_timeNode->setPosition(Vec2(110,-40));
        m_maxMsgLabel->setString("");
    }
}

void SciencePopupView::onEnter() {
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SciencePopupView::closeView), MSG_SCIENCE_CLOSE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SciencePopupView::updateInfo), MSG_SCIENCE_RESEARCH_FINISH, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SciencePopupView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    PopupBaseView::onEnter();
}

void SciencePopupView::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_CLOSE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_RESEARCH_FINISH);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SciencePopupView::onEnterFrame), this);
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool SciencePopupView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
//    if (m_bigBG->isVisible() && isTouchInside(m_bigNode, pTouch)) {
//        return false;
//    }
//    else if (m_smallBG->isVisible() && isTouchInside(m_smallNode, pTouch)) {
//        return false;
//    }
    return true;
}

void SciencePopupView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_bigBG->isVisible() && isTouchInside(m_bigNode, pTouch)) {
        return ;
    }
    else if (m_smallBG->isVisible() && isTouchInside(m_smallNode, pTouch)) {
        return ;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler SciencePopupView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", SciencePopupView::onClickCloseBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstantClick", SciencePopupView::onInstantClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onResearchBtnClick", SciencePopupView::onResearchBtnClick);
    return NULL;
}

bool SciencePopupView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, m_instantBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeBar", CCScale9Sprite*, m_timeBar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_confitionLabel", CCLabelIF*, m_confitionLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", CCNode*, m_timeNode);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barLabel", CCLabelIF*, m_barLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curLabel", CCLabelIF*, m_curLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLabel", CCLabelIF*, m_nextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curNumLabel", CCLabelIF*, m_curNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextNumLabel", CCLabelIF*, m_nextNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockSpr", CCSprite*, m_lockSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, m_inBtnGoldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeBarNode", CCNode*, m_timeBarNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bigNode", CCNode*, m_bigNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_smallNode", CCNode*, m_smallNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_maxMsgLabel", CCLabelIF*, m_maxMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bigBG", CCNode*, m_bigBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_smallBG", CCNode*, m_smallBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBG", CCScale9Sprite*, m_listBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curNode", CCNode*, m_curNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextNode", CCNode*, m_nextNode);
    return false;
}

void SciencePopupView::onEnterFrame(float dt)
{
    if(_lastTime>=0) {
        m_timeBarNode->setVisible(true);
        m_timeLabel->setString(CC_SECTOA(_lastTime));
        _lastTime--;
        float pro = _lastTime*1.0/_sumTime;
        m_timeBar->setScaleX(1-pro);
        _tmpGold = CCCommonUtils::getGoldByTime(_lastTime);
        m_inBtnGoldNum->setString(CC_ITOA(_tmpGold));
        if(_lastTime<0) {
            PopupViewController::getInstance()->removePopupView(this);
        }
    }
}

void SciencePopupView::onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void SciencePopupView::closeView(CCObject* obj)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void SciencePopupView::onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_isUping) {
        YesNoDialog::show( _lang("101320").c_str() , CCCallFunc::create(this, callfunc_selector(SciencePopupView::onYesCancel)));
    }
    else {
        YesNoDialog::showButtonAndGold(_lang("102370").c_str(), CCCallFunc::create(this, callfunc_selector(SciencePopupView::onOKInstantResearch)),m_btnMsgLabel->getString().c_str(), _tmpGold);
    }
}

void SciencePopupView::onOKInstantResearch()
{
    isReturn = false;
    if (_tmpGold > GlobalData::shared()->playerInfo.gold) {
        YesNoDialog::gotoPayTips();
        return;
    }
    ScienceController::getInstance()->startDirResearchScience(m_scienceId);
    PopupViewController::getInstance()->removePopupView(this);
}

void SciencePopupView::onYesCancel()
{
    string key = QueueController::getInstance()->getKey(m_scienceId, TYPE_SCIENCE);
    int qid = QueueController::getInstance()->getQueueQidByKey(key);
    if (qid>0) {
        GlobalData::shared()->scienceInfo.infoMap[m_scienceId].finishTime = 0;
        QueueController::getInstance()->startCancelQueue(qid);
        isReturn = false;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_CELL_STATE);
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void SciencePopupView::onResearchBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    
    if (m_isUping) {
        YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(SciencePopupView::onOKCostCD)), _lastTime, _lang("104903").c_str(), false);
        
//        string key = QueueController::getInstance()->getKey(m_scienceId, TYPE_SCIENCE);
//        int qid = QueueController::getInstance()->getQueueQidByKey(key);
//        if (qid>0) {
//            _tmpGold = CCCommonUtils::getGoldByTime(_lastTime);
//            QueueController::getInstance()->startCCDQueue(qid, "", false, _tmpGold);
//            
//        }
//        isReturn = false;
//        PopupViewController::getInstance()->removePopupView(this);
    }
    else {
        auto& info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
        int qid = QueueController::getInstance()->canMakeItemByType(TYPE_SCIENCE, info.time_need);
        if(QID_MAX == qid)
        {
            CCCommonUtils::flyText(_lang("102152"));
            return;
        }
        
        if (_resGold>0) {
            PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(m_lfood,m_lwood,m_liron,m_lstone,0,_resGold
                                                                                       ,this,callfunc_selector(SciencePopupView::lastResearch)
                                                                                       , _lang("102314"), _lang("121994")),false);
        }
        else {
            lastResearch();
        }
    }
}

void SciencePopupView::onOKCostCD()
{
    if (m_isUping) {
        string key = QueueController::getInstance()->getKey(m_scienceId, TYPE_SCIENCE);
        int qid = QueueController::getInstance()->getQueueQidByKey(key);
        if (qid>0) {
            _tmpGold = CCCommonUtils::getGoldByTime(_lastTime);
            QueueController::getInstance()->startCCDQueue(qid, "", false, _tmpGold,"",4);
            
        }
        isReturn = false;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

void SciencePopupView::lastResearch()
{
    isReturn = false;
    PopupViewController::getInstance()->removePopupView(this);
    
    ScienceController::getInstance()->startResearchScience(m_scienceId, _resGold);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("SC3_%d_research", m_scienceId));
}

CCNode *SciencePopupView::getGuideNode(int sid){
    if(this->m_scienceId == sid && m_upBtn->isEnabled()){
        return this->m_upBtn;
    }
    return NULL;
}
