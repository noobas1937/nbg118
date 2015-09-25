//
//  FavoriteList.cpp
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#include "FavoriteList.h"
#include "WorldMapView.h"
#include "AddFavorite.h"
#include "YesNoDialog.h"
#include "UIComponent.h"

#ifndef MSG_WORLD_FAVO_RESET_POS
#define MSG_WORLD_FAVO_RESET_POS "world_favo_reset_position"
#endif

bool FavoriteListCell::init() {
    auto node = CCBLoadFile("FavoriteCell",this,this);
    auto cellSize = container->m_nodeSize;
    node->setPosition(ccp(cellSize.width/2,cellSize.height));
    this->setContentSize(cellSize);
    if (m_tile.serverId > CROSS_SERVER_START_INDEX) {
        string temp = _lang("138038");
        m_position->setString(_lang_3("108763", temp.c_str(), CC_ITOA((int)m_tile.point.x), CC_ITOA((int)m_tile.point.y)));
    }else{
         m_position->setString(_lang_3("108763", CC_ITOA(m_tile.serverId), CC_ITOA((int)m_tile.point.x), CC_ITOA((int)m_tile.point.y)));
    }
    m_name->setString(m_tile.name.c_str());
    m_deleteLabel->setString(_lang("108523").c_str());
    m_editLabel->setString(_lang("108524").c_str());
    
    string iconStr = "world_favo_3.png";
    switch (m_tile.type) {
        case Favorite:{
        }
            break;
        case Friend:{
            iconStr = "world_friend_icon.png";
        }
            break;
        case Enemy:{
            iconStr = "world_enemy_icon.png";
        }
            break;
            
        default:
            break;
    }
    
    m_icon->setDisplayFrame(CCLoadSprite::loadResource(iconStr.c_str()));
    if(m_tile.isNew){
        auto newSprite = CCLoadSprite::createSprite("new_add.png");
        addChild(newSprite);
        auto size = newSprite->getContentSize();
        newSprite->setPosition(ccp(size.width / 2 - 2, cellSize.height - size.height / 2 + 7));
        
        auto label = CCLabelIF::create(_lang("103767").c_str());
        addChild(label);
        label->setColor(ccc3(255,203,135));
        label->setPosition(newSprite->getPosition() - ccp(8, -8));
        label->setRotation(-45);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            newSprite->setScale(1.8);
            label->setScale(1.8);
        }
    }
    return true;
}

bool FavoriteListCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCSprite*, this->m_icon)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_position", CCLabelIF*, this->m_position)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name", Label*, this->m_name)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBG", CCScale9Sprite*, this->m_editBG)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoTileBG1", CCScale9Sprite*, this->m_gotoTileBG1)
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoTileBG2", CCScale9Sprite*, this->m_gotoTileBG2)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBG", CCScale9Sprite*, this->m_deleteBG)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteLabel", CCLabelIF*, this->m_deleteLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editLabel", CCLabelIF*, this->m_editLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoButtonNode", CCNode*, this->m_gotoButtonNode)
    return false;
}

SEL_CCControlHandler FavoriteListCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

void FavoriteListCell::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FavoriteListCell::updateListCellPosition), MSG_WORLD_FAVO_RESET_POS, NULL);

    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void FavoriteListCell::onExit() {
    m_gotoButtonClicked = true;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WORLD_FAVO_RESET_POS);
    setTouchEnabled(false);
    CCNode::onExit();
}


void FavoriteListCell::updateListCellPosition(CCObject* p)
{
    this->m_gotoButtonNode->setPositionX(0);
}

void FavoriteListCell::onDeleteFavorite()
{
    auto func = [&]() {
        auto cmd = new WorldDelFavoriteCommand(m_index, m_tile.serverId);
        cmd->setCallback(CCCallFuncO::create(container, callfuncO_selector(FavoriteList::onDeleteCallback), NULL));
        cmd->sendAndRelease();
    };
    
    YesNoDialog::show(_lang("108526").c_str(), func);
}

void FavoriteListCell::onEditFavorite()
{
    auto world = WorldMapView::instance();
    if (world) {
        world->addPopupView(AddFavorite::create(m_index,m_tile.name,m_tile.type));
    }
}

void FavoriteListCell::onGotoFavoriteTile()
{
    container->closeThis();
    int index = WorldController::getIndexByPoint(m_tile.point);
    WorldController::getInstance()->openTargetIndex = index;
    WorldMapView::instance()->gotoTilePoint(m_tile.point, m_tile.serverId);
}

bool FavoriteListCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (container->m_table->isTouchMoved())
    {
        return false;
    }
        
    m_touchPoint = pTouch->getLocation();
    
    if (m_movedToLeftFlag && !(isTouchInside(m_editBG, pTouch) || isTouchInside(m_deleteBG, pTouch)))
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_FAVO_RESET_POS);
        m_movedToLeftFlag = false;
    }
    
    if (isTouchInside(m_editBG, pTouch) || isTouchInside(m_deleteBG, pTouch)
        || isTouchInside(m_gotoTileBG1, pTouch))
    {
        return true;
    }
    
    return false;
}

void FavoriteListCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    float dx =pTouch->getLocation().x-m_touchPoint.x;
    float dy =pTouch->getLocation().y-m_touchPoint.y;
    
    if(fabs(dy) < 12 && fabs(dx) < 12)   //防点击当作移动
    {
        m_gotoButtonClicked = true;
        return;
    }
    else
    {
        m_gotoButtonClicked = false;
    }
    
    if (container != NULL && container->m_table != NULL && container->m_table->isTouchMoved())
    {
        return;
    }
    
    if(fabs(dy) > 12 && fabs(dy)>=fabs(dx))
    {//竖向
        return;
    }
    else if(container != NULL && container->m_table != NULL)
    {
        container->m_table->setTouchStop(false);
    }
    
    if(fabs(dy)>20)
    {
        return;
    }
    
    if(dx < 0)
    {
        float currX =this->m_gotoButtonNode->getPositionX();
        
        if(currX <= -278)
        {
            return;
        }
        
        if(dx < -278)
        {
            dx = -278;
        }
        this->m_gotoButtonNode->setPositionX(dx);
        
        m_moveDir = GotoButtonMoveLeft;
        m_movedToLeftFlag = true;
        
    }
    else
    {
        float currX =this->m_gotoButtonNode->getPositionX();
        if(currX>=0)
        {
            return;
        }
        
        dx = -278 + dx;
        
        if(dx > 0)
        {
            dx = 0;
        }
        this->m_gotoButtonNode->setPositionX(dx);
        m_moveDir = GotoButtonMoveRight;
    }
//    return;
}

void FavoriteListCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (container != NULL && container->m_table != NULL)
    {
        container->m_table->setTouchStop(true);
    }
    
    if (m_gotoButtonClicked)
    {
        if (isTouchInside(m_gotoTileBG1, pTouch))
        {
            FavoriteListCell::onGotoFavoriteTile();
        }
        else if (isTouchInside(m_deleteBG, pTouch))
        {
            FavoriteListCell::onDeleteFavorite();
        }
        else if (isTouchInside(m_editBG, pTouch))
        {
            FavoriteListCell::onEditFavorite();
        }
    }
    
    m_gotoButtonClicked = true;
    
    if (m_moveDir == GotoButtonMoveRight)
    {
        this->m_gotoButtonNode->setPositionX(0);
    }
    else if (m_moveDir == GotoButtonMoveLeft)
    {
        this->m_gotoButtonNode->setPositionX(-278);
    }
    m_moveDir = GotoButtonMoveNone;
}



bool FavoriteList::init() {
    PopupBaseView::init();
    setIsHDPanel(true);
    setTitleName(_lang("108521"));
    CCBLoadFile("FavoriteList",this,this);
    m_nodeSize = CCSize(604,190);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_nodeSize = CCSize(604*2.2,190*2.2);
    }
    m_currentTab = All;
    m_button[0]->setHighlighted(true);
    m_data = CCArray::create();
    sortFavoriteList();
    generateData();
    auto height = getBGHeight();
    
    m_subNodeBg->setContentSize(CCSize(640,height - 60));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_subNodeBg->setContentSize(CCSize(1536,height - 60));
    }
    CCCommonUtils::setButtonTitle(m_button[0], _lang("108522").c_str());
    CCCommonUtils::setButtonTitle(m_button[1], _lang("108505").c_str());
    CCCommonUtils::setButtonTitle(m_button[2], _lang("108506").c_str());
    CCCommonUtils::setButtonTitle(m_button[3], _lang("108520").c_str());
    this->m_noFavoTxt->setString(_lang("102378"));
    this->m_noFavoTxt->setVisible(false);
    if(m_data->count()==0)
    {
        this->m_noFavoTxt->setVisible(true);
    }
    

    
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_table = CCTableView::create(this,CCSize(1480,height - 214));
    }
    else
    {
        m_table = CCTableView::create(this,CCSize(620,height - 125));
    }
    
    m_subNode->addChild(m_table);
    m_subNode->setPositionX(m_subNode->getPositionX() + 15);
    
    for (int i = 0; i < 4; i++)
    {
        this->m_button[i]->setZoomOnTouchDown(false);
    }
    
    return true;
}

void FavoriteList::onEnter() {
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FavoriteList::updateList), MSG_WORLD_FAVO_ADD, NULL);
}

void FavoriteList::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WORLD_FAVO_ADD);
    for (auto it = WorldController::getInstance()->m_favoriteTiles.begin(); it != WorldController::getInstance()->m_favoriteTiles.end();it++) {
        it->second.isNew = false;
    }
    UIComponent::getInstance()->setFavoriteIconVisible(false);
    PopupBaseView::onExit();
}

CCSize FavoriteList::cellSizeForTable(  cocos2d::extension::TableView *table) {
    return m_nodeSize;
}

CCTableViewCell* FavoriteList::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx) {
    unsigned int waitIndex = 0;
    
    int i = 0;
    while (i < m_data->count()) {
        double index = dynamic_cast<CCDouble*>(m_data->objectAtIndex(i))->getValue();
        auto it = WorldController::getInstance()->m_favoriteTiles.find(index);
        if(it != WorldController::getInstance()->m_favoriteTiles.end()){
            if (m_currentTab == All || m_currentTab == (FavoriteType)it->second.type) {
                if (waitIndex++ == idx) {
                    auto _tmpNode = FavoriteListCell::create(it->second, this);

                    return _tmpNode;
                }
            }
        }
        i++;
    }
    
    return NULL;
}

ssize_t FavoriteList::numberOfCellsInTableView(  cocos2d::extension::TableView *table) {
    auto &favos = WorldController::getInstance()->m_favoriteTiles;
    if (m_currentTab == All) {
        m_FavoCount = m_FavoVec.size();
        return m_FavoVec.size();
    }
    
    unsigned int ret = 0;
    for (auto & favo:favos) {
        if ((FavoriteType)favo.second.type == m_currentTab) {
            if(GlobalData::shared()->playerInfo.currentServerId<CROSS_SERVER_START_INDEX && favo.second.serverId>CROSS_SERVER_START_INDEX){
                continue;
            }
            ret++;
        }
    }
    m_FavoCount = ret;
    return ret;
}

void FavoriteList::onDeleteCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo: parse error
        return;
    }
    
    updateList(NULL);
}

void FavoriteList::generateData(){
    m_data->removeAllObjects();
    auto tmpArrNew = CCArray::create();
    auto tmpArrCurrentServer = CCArray::create();
    auto tmpArrOther = CCArray::create();
    for (auto i = 0; i < m_FavoVec.size(); i++) {
        if(m_FavoVec[i].second.isNew){
            tmpArrNew->addObject(CCDouble::create(m_FavoVec[i].first));
        }else if(m_FavoVec[i].second.serverId == GlobalData::shared()->playerInfo.currentServerId){
            tmpArrCurrentServer->addObject(CCDouble::create(m_FavoVec[i].first));
        }else{
            tmpArrOther->addObject(CCDouble::create(m_FavoVec[i].first));
        }
    }
    m_data->addObjectsFromArray(tmpArrNew);
    m_data->addObjectsFromArray(tmpArrCurrentServer);
    m_data->addObjectsFromArray(tmpArrOther);
    m_data->reverseObjects();
    tmpArrNew->removeAllObjects();
    tmpArrCurrentServer->removeAllObjects();
    tmpArrOther->removeAllObjects();
}

bool FavoriteList::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*, this->m_subNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNodeBg", CCScale9Sprite*, this->m_subNodeBg)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button1", CCControlButton*, this->m_button[0])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button2", CCControlButton*, this->m_button[1])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button3", CCControlButton*, this->m_button[2])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button4", CCControlButton*, this->m_button[3])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_noFavoTxt",CCLabelIF*,this->m_noFavoTxt);
    return false;
}

SEL_CCControlHandler FavoriteList::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickButtonTab", FavoriteList::onClickButtonTab)
    return NULL;
}

void FavoriteList::onClickButtonTab(CCObject * pSender, Control::EventType pCCControlEvent) {
    auto button = dynamic_cast<CCControlButton*>(pSender);
    if (button) {
        auto tag = button->getTag();
        for (int i=0; i<4; i++) {
            if (m_button[i]->getTag() == tag) {
                m_button[i]->setHighlighted(true);
            } else {
                m_button[i]->setHighlighted(false);
            }
        }
        auto tabToGo = (FavoriteType)tag;
        if (m_currentTab == tabToGo) {
            return;
        }
        m_currentTab = tabToGo;
        updateList(NULL);
    }
}

void FavoriteList::closeThis() {
    PopupViewController::getInstance()->removeAllPopupView();
}

void FavoriteList::updateList(cocos2d::CCObject *obj) {
    m_FavoCount = 0;
    sortFavoriteList();
    generateData();
    m_table->reloadData();
    this->m_noFavoTxt->setVisible(m_FavoCount<=0);
}

void FavoriteList::sortFavoriteList()
{
    m_FavoVec.clear();
    for(auto it = WorldController::getInstance()->m_favoriteTiles.begin(); it != WorldController::getInstance()->m_favoriteTiles.end(); it++)
    {
        if(GlobalData::shared()->playerInfo.currentServerId<CROSS_SERVER_START_INDEX && it->second.serverId>CROSS_SERVER_START_INDEX){
            continue;
        }
        m_FavoVec.push_back(make_pair(it->first, it->second));
    }
    std::sort(m_FavoVec.begin(), m_FavoVec.end(), cmp);
}

bool FavoriteList::cmp(const std::pair<double,FavoriteTile>& item1, const std::pair<double,FavoriteTile>& item2)
{
    return item1.second.createTime > item2.second.createTime;
}
