//
//  WorldBannerRankView.cpp
//  IF
//
//  Created by wangchaoyi on 15/3/24.
//
//

#include "WorldBannerRankView.h"
#include "SoundController.h"
#include "WorldController.h"

const float numPerRow = 1.0;
WorldBannerRankView* WorldBannerRankView::create(int type){
    WorldBannerRankView* ret = new WorldBannerRankView(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool WorldBannerRankView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    auto tmpCCB = CCBLoadFile("WorldBannerRankView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    m_bannerNumMap.clear();
    m_bannerNumVec.clear();
    //m_data = CCArray::create();
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    int BGcount = (newBgHeight-80)/100+1-1;
    for (int i=0; i<BGcount; i++) {
        auto pic = CCLoadSprite::createSprite("technology_09.png");
        m_totalNode->addChild(pic);
        pic->setPositionY(203-(i+1)*100);
    }
    
    m_textLabel1->setString(_lang("110142"));
    m_textLabel2->setString(_lang("110143"));
    m_textLabel3->setString(_lang("110144"));
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    
    
    updateInfo();
    
    return true;
}

void WorldBannerRankView::updateInfo()
{
    //m_data->removeAllObjects();
    
    int num = WorldController::getInstance()->m_serverList.size();
    
    if(WorldController::getInstance()->m_serverList.size() > 0){
        int index = 0;

        while(index < num){
            ServerListInfo &info = WorldController::getInstance()->m_serverList[index];
            std::string banner = "";
            std::string kingName = "";
            banner = info.banner;
            kingName = info.kingName;
            // 保存旗帜信息
            if (kingName != "") {
                std::map<string, int>::iterator it = m_bannerNumMap.find(banner);
                if(it != m_bannerNumMap.end())
                {
                    m_bannerNumMap[banner] = m_bannerNumMap[banner] + 1;
                }
                else
                {
                    m_bannerNumMap[banner] = 1;
                }

            }
            index++;
        }
    }
    
    //对旗帜统计的排序
    sortBanner();
    
    m_tabView->reloadData();
}

void WorldBannerRankView::sortBanner()
{
//    auto cmp = [&](std::pair<string,int>&x, pair<string,int>&y)
//    {
//        return x.second > y.second;
//    };
    
    for(std::map<string, int>::iterator it=m_bannerNumMap.begin(); it!=m_bannerNumMap.end(); it++)
    {
        m_bannerNumVec.push_back(make_pair(it->first, it->second));
    }
    std::sort(m_bannerNumVec.begin(), m_bannerNumVec.end(), cmp);
}

bool WorldBannerRankView::cmp(const std::pair<string, int>& item1, const std::pair<string, int>& item2)
{
    return item1.second > item2.second;
}

CCSize WorldBannerRankView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_bannerNumVec.size()){
        return CCSizeZero;
    }
    
    return CCSize(620, 100);
    
}
CCSize WorldBannerRankView::cellSizeForTable(CCTableView *table)
{
    return CCSize(620, 100);
}
CCTableViewCell* WorldBannerRankView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_bannerNumVec.size()){
        return NULL;
    }
    
    WorldBannerRankCell* cell = (WorldBannerRankCell*)table->dequeueCell();
    if(idx < m_bannerNumVec.size()){
        if(cell){
            cell->setData(m_bannerNumVec[idx].first, m_bannerNumVec[idx].second, idx);
        }else{
            cell = WorldBannerRankCell::create(m_bannerNumVec[idx].first, m_bannerNumVec[idx].second, idx);
        }
    }
    return cell;
}
ssize_t WorldBannerRankView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_bannerNumVec.size() / numPerRow);
    return num;
}
void WorldBannerRankView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void WorldBannerRankView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("110141").c_str());
}

void WorldBannerRankView::onExit()
{
    CCNode::onExit();
}

WorldBannerRankView::~WorldBannerRankView()
{
}

SEL_CCControlHandler WorldBannerRankView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool WorldBannerRankView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textLabel1", CCLabelIF*, this->m_textLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textLabel2", CCLabelIF*, this->m_textLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textLabel3", CCLabelIF*, this->m_textLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    return false;
}

WorldBannerRankCell* WorldBannerRankCell::create(std::string banner, int count, int index)
{
    auto ret = new WorldBannerRankCell(banner, count, index);
    if (ret && ret->init(banner, count, index)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool WorldBannerRankCell::init(std::string banner, int count, int index)
{
    bool ret = true;
    CCBLoadFile("WorldBannerRankCell",this,this);
    
    setData(banner, count, index);
    return ret;
}

void WorldBannerRankCell::setData(std::string banner, int count, int index)
{
    m_index = index;
    m_bannerNode->removeAllChildren();
    
    std::string countryName;
    countryName = banner;
    
    //大陆地区，看到台湾的，都成了大陆的国旗
    if((banner=="TW" || banner=="HK") && CCCommonUtils::checkTaiWanFlag()){
        banner = "CN";
    }
    banner = CCCommonUtils::changeChinaFlag(banner);
    CCSprite* pic;
    string mpic = banner+".png";
    pic = CCLoadSprite::createSprite(mpic.c_str());
    pic->setVisible(CCCommonUtils::isShowFlagForChinese());
    
    CCCommonUtils::setSpriteMaxSize(pic, 60);
    m_bannerNode->addChild(pic);
    m_sprBG1->setVisible(false);
    m_sprBG2->setVisible(false);
    m_sprBG3->setVisible(false);
    //m_sprBG4->setVisible(false);
    m_numText->setVisible(false);
    m_numspr1->setVisible(false);
    m_numspr2->setVisible(false);
    m_numspr3->setVisible(false);
    if(m_index==0){
        m_sprBG1->setVisible(true);
        m_numspr1->setVisible(true);
    }else if(m_index==1){
        m_sprBG2->setVisible(true);
        m_numspr2->setVisible(true);
    }else if(m_index==2){
        m_sprBG3->setVisible(true);
        m_numspr3->setVisible(true);
    }else{
        m_numText->setVisible(true);
        //m_sprBG4->setVisible(true);
        m_numText->setString(CC_ITOA(m_index+1));
    }
    
    //设置信息
    
    m_text1->setString(_lang(countryName).c_str());
    m_text2->setString(CC_CMDITOA(count));
}

void WorldBannerRankCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void WorldBannerRankCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}


SEL_CCControlHandler WorldBannerRankCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
     return NULL;
}


bool WorldBannerRankCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(isTouchInside(this->m_hintBGNode, pTouch)){
        m_startPoint =pTouch->getStartLocation();
        return true;
    }
    return false;
}

void WorldBannerRankCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(fabs(pTouch->getLocation().x-m_startPoint.x)>20||fabs(pTouch->getLocation().y-m_startPoint.y)>20){
        return;
    }
    if (isTouchInside(m_hintBGNode,pTouch)) {
        return;
    }
}



bool WorldBannerRankCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText", CCLabelIF*, m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text1", CCLabelIFTTF*, m_text1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text2", CCLabelIF*, m_text2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numspr1", CCSprite*, m_numspr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numspr2", CCSprite*, m_numspr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numspr3", CCSprite*, m_numspr3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bannerNode", CCNode*, m_bannerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintBGNode", CCNode*, m_hintBGNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG1", CCScale9Sprite*, m_sprBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG2", CCScale9Sprite*, m_sprBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG3", CCScale9Sprite*, m_sprBG3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG4", CCScale9Sprite*, m_sprBG4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bannerSpr", CCSprite*, m_bannerSpr);
    
    return false;
}


