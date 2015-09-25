//
//  ResourceTileInfoPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-8-21.
//
//

#include "ResourceTileInfoPopUpView.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "UseItemStatusView.h"
#include "ShowStatusItemCommand.h"

ResourceTileInfoPopUpView *ResourceTileInfoPopUpView::create(WorldCityInfo &info){
    ResourceTileInfoPopUpView *ret = new ResourceTileInfoPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


void ResourceTileInfoPopUpView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize ResourceTileInfoPopUpView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(250, 150);
}

CCTableViewCell* ResourceTileInfoPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    ResourceTroopInfo* cell = (ResourceTroopInfo*)table->dequeueGrid();
    CCDictionary* info = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = ResourceTroopInfo::create(info);
        }
    }
    return cell;
}

ssize_t ResourceTileInfoPopUpView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return ceil(m_data->count() * 1.0f / 2.0f);
}

unsigned int ResourceTileInfoPopUpView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 2;
}

bool ResourceTileInfoPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        WorldMapView::instance()->m_directionIcon->setVisible(false);
        WorldMapView::instance()->m_directionLabel->setVisible(false);
        auto bg = CCBLoadFile("TipsView", this, this);
        m_toolUseBtn->setSwallowsTouches(false);
        this->setContentSize(bg->getContentSize());
        m_data = CCArray::create();
        refreshView();
        ret = true;
    }
    return ret;
}

void ResourceTileInfoPopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void ResourceTileInfoPopUpView::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    WorldMapView::instance()->updateDirection();
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool ResourceTileInfoPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_toolUseBtn, pTouch))
    {
        return true;
    }
    if(isTouchInside(m_buildBG, pTouch)){
        return false;
    }

    return true;
}

void ResourceTileInfoPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_toolUseBtn, pTouch))
    {
        onToolUseBtnClick(nullptr,Control::EventType::TOUCH_DOWN);
    }
    if(!isTouchInside(m_buildBG, pTouch)){
        this->closeSelf();
    }
}

void ResourceTileInfoPopUpView::onToolUseBtnClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent)
{
    closeSelf();
    PopupViewController::getInstance()->addPopupInView(UseItemStatusView::create(8,_lang("101406"),_lang("101448")));
}

SEL_CCControlHandler ResourceTileInfoPopUpView::onResolveCCBCCControlSelector(CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onToolUseBtnClick", ResourceTileInfoPopUpView::onToolUseBtnClick);
    return NULL;
}

bool ResourceTileInfoPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameAndLevelText", CCLabelIF*, this->m_nameAndLevelText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_capLabel", CCLabelIF*, this->m_capLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_capText", CCLabelIF*, this->m_capText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedText", CCLabelIF*, this->m_speedText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedAddText", CCLabelIF*, this->m_speedAddText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alreadyGetText", CCLabelIF*, this->m_alreadyGetText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_introText", CCLabelIF*, this->m_introText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_troopTitleText", CCLabelIF*, this->m_troopTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedTimeText", CCLabelIF*, this->m_speedTimeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_line", CCSprite*, this->m_line);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolUseBtn", CCControlButton*, this->m_toolUseBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_smalIntoTxt", CCLabelIF*, this->m_smalIntoTxt);
    return false;
}

void ResourceTileInfoPopUpView::generateData(){
    
}

void ResourceTileInfoPopUpView::update(float delta){
    auto time = WorldController::getInstance()->getTime() - m_info.resource.digStartTime;
    int currentNum = m_info.resource.digSpeed * time / 1000;
    
    if(m_info.resource.changeCollectSpdTime != 0){
        time = WorldController::getInstance()->getTime() - m_info.resource.changeCollectSpdTime;
        currentNum = m_info.resource.digStartNum + (m_info.resource.digSpeed * time)/1000;
    }

    currentNum = MAX(0.0f, currentNum);
    currentNum = MIN(m_info.resource.troopMax, currentNum);
    this->m_alreadyGetText->setString(_lang_1("108868", CC_CMDITOA(currentNum).c_str()));
    
    bool flag = CCCommonUtils::hasSpeedEffect(8);
    if(m_info.resource.type != Gold)
    {
        if(flag){
            int time = CCCommonUtils::getSpeedEffectTime(8);
            m_speedTimeText->setString(_lang("102361") + CCCommonUtils::timeLeftToCountDown(time));
        }else{
            m_speedTimeText->setString(_lang("102361") + "00:00:00");
        }
    }
    if(m_toolUseBtn->isVisible())
    {
//        CCLOG("lh__00__: %f",m_speedTimeText->getContentSize().width);
        
        float sizeWidth = m_speedTimeText->getContentSize().width;
        std::string language = LocalController::shared()->getLanguageFileName();
        if (!m_speedTimeText->canBeSupportedWithBmpFont(language))
        {
            sizeWidth = 300;
        }
        m_toolUseBtn->setPositionX(m_speedTimeText->getPositionX()+sizeWidth*m_speedTimeText->getOriginScaleX()+60);
        
        if(language == "ru")
        {
            m_toolUseBtn->setPositionY(m_alreadyGetText->getPositionY());
        }
    }
}

void ResourceTileInfoPopUpView::refreshView(){
    this->m_troopTitleText->setString(_lang("108638"));
    auto pt = WorldController::getInstance()->getPointByIndex(m_info.cityIndex);
    this->m_icon->removeAllChildren();
    auto icon = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(m_info.resource.type).c_str());
    CCCommonUtils::setSpriteMaxSize(icon, 30);
    m_icon->addChild(icon);
    this->m_nameAndLevelText->setString(_lang("108634")
                                        + CC_ITOA(int(m_info.resource.lv))
                                        + " "
                                        + CCCommonUtils::getResourceProductByType(m_info.resource.type)
                                        + "("
                                        + CC_ITOA(int(pt.x))
                                        + ","
                                        + CC_ITOA(int(pt.y))
                                        + ")"
    );
    this->m_capLabel->setString(_lang_1("108870", ""));
    this->m_capText->setString(CC_CMDITOA(double(m_info.resource.sum)).c_str());
    int speed = round(m_info.resource.digSpeed * 3600);

    int addSpeed = round(speed - speed / (1.0 + CCCommonUtils::getEffectValueByNum(65) / 100.0));
    //采集速度显示分为基础和加成两部分
    if (m_info.resource.type != Gold) {
        this->m_speedText->setString(_lang_1("108869", CC_CMDITOA(speed-addSpeed).c_str()));
    }
    else
        this->m_speedText->setString(_lang_1("108869", CC_CMDITOA(speed).c_str()));
    
    if(addSpeed != 0 && m_info.resource.type != Gold){
        m_speedAddText->setString(std::string("+ ") + CC_CMDITOA(addSpeed) + "/h");
        m_speedAddText->setColor(ccGREEN);
        float sizeWidth = m_speedText->getContentSize().width;
        std::string language = LocalController::shared()->getLanguageFileName();
        if (!m_speedText->canBeSupportedWithBmpFont(language))
        {
            sizeWidth = 300;
        }
        m_speedAddText->setPositionX(m_speedText->getPositionX() + sizeWidth * m_speedText->getOriginScaleX() + 10);
    }

    m_smalIntoTxt->setString(_lang("135010"));// 临时
    string content = m_info.resource.type != Gold ? "108629" : "108631";
    content = "";
    this->m_introText->setString(_lang(content));
    int upH = 260;
    int bgH = m_buildBG->getContentSize().height;
    int introH = m_introText->getContentSize().height * m_introText->getOriginScaleY() + 15;
    m_line->setVisible(false);

    if(atoi(m_info.citySubType.c_str()) == Gold)
    {
        m_toolUseBtn->setVisible(false);
        m_speedTimeText->setString(_lang("135011"));
    }
    else
    {
        m_toolUseBtn->setVisible(true);
        m_toolUseBtn->setTitleForState(_lang("102359"), CCControlStateNormal);
//        ShowStatusItemCommand* cmd = new ShowStatusItemCommand();
//        cmd->sendAndRelease();
        
        vector<int> tmpGTypes;
        tmpGTypes.push_back(0);
        tmpGTypes.push_back(1);
        tmpGTypes.push_back(2);
        tmpGTypes.push_back(3);
        tmpGTypes.push_back(5);
        tmpGTypes.push_back(6);
        tmpGTypes.push_back(7);
        tmpGTypes.push_back(13);
        
        ToolController::getInstance()->m_typeItems.clear();
        if(ToolController::getInstance()->m_typeItems.size()<=0){
            for(int i=0;i<tmpGTypes.size();i++){
                int type = tmpGTypes[i]+1;
                ToolController::getInstance()->m_typeItems[type] = CCCommonUtils::getTypeArray(type);
            }
        }
    }
    if(m_info.playerName != GlobalData::shared()->playerInfo.name){
        this->m_troopTitleText->setVisible(false);
        this->m_buildBG->setContentSize(CCSize(this->m_buildBG->getContentSize().width, upH + introH));
    }else{
        this->m_buildBG->setContentSize(CCSize(this->m_buildBG->getContentSize().width, 640));
        this->m_line->setPositionY(this->m_introText->getPositionY() - introH);
        this->m_troopTitleText->setPositionY(m_line->getPositionY() - 20);
        this->m_listContainer->setContentSize(CCSize(this->m_listContainer->getContentSize().width, 600 - upH - introH - 80));
        this->m_listContainer->setPositionY(-300 - m_buildBG->getPositionY());
        
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_listContainer->addChild(m_tabView);

        getTroopData();
    }
    update(0.0f);
    int dy = m_buildBG->getContentSize().height - bgH;
    m_totalNode->setPositionY(m_totalNode->getPositionY() + dy / 2);
}

void ResourceTileInfoPopUpView::troopDataBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    m_data->removeAllObjects();
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (info && info->objectForKey("uuid")) {
        string uuid = info->valueForKey("uuid")->getCString();
        if(info->objectForKey("soldiers")){
            auto arr = dynamic_cast<CCArray*>(info->objectForKey("soldiers"));
            int index = 0;
            while (index < arr->count()) {
                auto dict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(index));
                int num = dict->valueForKey("count")->intValue();
                std::string armyId = dict->valueForKey("armyId")->getCString();
                CCDictionary *data = CCDictionary::create();
                data->setObject(CCString::create(CC_ITOA(num)), "count");
                data->setObject(CCString::create(armyId), "armyId");
                m_data->addObject(data);
                index++;
            }
        }
    }
    m_tabView->reloadData();
}

void ResourceTileInfoPopUpView::getTroopData(){
    marchId = "";
    map<string, MarchInfo>::iterator it;
    for(it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++){
        if(it->second.endPointIndex == m_info.cityIndex){
            marchId = it->first;
        }
    }
    
    auto cmd = new WorldMarchDetailUpdateCommand(marchId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ResourceTileInfoPopUpView::troopDataBack), NULL));
    cmd->sendAndRelease();
}

//---

ResourceTroopInfo *ResourceTroopInfo::create(CCDictionary *dict){
    ResourceTroopInfo *ret = new ResourceTroopInfo();
    if(ret && ret->init(dict)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ResourceTroopInfo::setData(CCDictionary *dict){
    m_dict = dict;
    refreshView();
}

bool ResourceTroopInfo::init(CCDictionary *dict){
    auto node = CCBLoadFile("TroopArmyCell", this, this);
    this->setContentSize(node->getContentSize());
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });

    m_dict = dict;
    refreshView();
    return true;
}

bool ResourceTroopInfo::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelNode", CCNode*, this->m_levelNode);
    return false;
}

void ResourceTroopInfo::refreshView(){
    std::string itemId = m_dict->valueForKey("armyId")->getCString();
    int num = m_dict->valueForKey("count")->intValue();
    m_nameTxt->setString(CCCommonUtils::getNameById(itemId));
    m_nameTxt->setDimensions(CCSize(140, 0));
    m_numTxt->setString(CC_CMDITOA(num));

    m_iconNode->removeAllChildren();
    string picName = GlobalData::shared()->armyList[itemId].getHeadIcon();
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    m_iconNode->addChild(pic);
    pic->setPositionY(-10);
    CCCommonUtils::setSpriteMaxSize(pic, 110);
    this->m_levelNode->removeAllChildren();
    string num1 = itemId.substr(itemId.size()-2);
    auto pic1= CCCommonUtils::getRomanSprite(atoi(num1.c_str())+1);
    m_levelNode->addChild(pic1);
}

void ResourceTroopInfo::onEnter(){
    
}

void ResourceTroopInfo::onExit(){
    
}
