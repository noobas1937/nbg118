//
//  TroopInformationView.cpp
//  IF
//
//  Created by 邹 程 on 14-4-18.
//
//

#include "TroopInformationView.h"
#include "WorldCommand.h"
#include "ToolController.h"
#include "UseCDToolView.h"
#include "PopupViewController.h"
#include "WorldMapView.h"
#include "AllianceWarView.h"
#include "SoundController.h"
#include "FunBuildController.h"

static CCSize _col_size = CCSize(530,40);
static ccColor3B _text_color = ccColor3B{56,8,0};

bool TroopInformation::init(CCDictionary* dict) {
    if (dict && dict->count()) {
        m_info.setInfo(dict);
    }

    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    CCScrollView* m_scrollView = CCScrollView::create(CCSize(580,360));
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setPosition(ccp(-285, -400));
    this->addChild(m_scrollView,100);
    auto node = CCNode::create();
    m_scrollView->addChild(node);
    int totalH = 0;
    int numH = 0;
    int cellH = 135;
    for (auto it = m_info.soldiers.begin(); it != m_info.soldiers.end(); it++) {
        int totalTroop = 0;
        for (int i = 0; i < it->second.size(); i++) {
            auto info = it->second[i];
            if(i == 0){
                auto bg = CCLoadSprite::createScale9Sprite("Mail_biaoti.png");
                bg->setAnchorPoint(ccp(0, 1));
                bg->setContentSize(CCSize(568.0,44));
                bg->setPosition(ccp(0, totalH));
                node->addChild(bg);
                
                std::string nameStr = info.playerName;
                if (info.monsterId != "") {
                    nameStr = CCCommonUtils::getNameById(info.monsterId);
                }
                auto name = CCLabelIFTTF::create(nameStr.c_str());
                node->addChild(name);
                name->setColor({178,162,139});
                name->setPosition(ccp(10, totalH));
                name->setAnchorPoint(ccp(0, 1));
                numH = totalH;
                totalH -= name->getContentSize().height * name->getOriginScaleY() + 15;
            }
            int col = i%2;
            if(col == 0){
                totalH -= cellH;
            }

            TroopArmyCell* cell = TroopArmyCell::create(CCCommonUtils::getNameById(info.armyId), CCCommonUtils::getResourceStr(info.count), info.armyId);
            cell->setPosition(ccp(col == 0 ? 0 : 290, totalH));
            node->addChild(cell);
            
            totalTroop += info.count;
        }
        
        string str = _lang("108638");
        str.append(": ");
        str.append(CC_ITOA(totalTroop));
        auto numTxt = CCLabelIFTTF::create(str.c_str());
        node->addChild(numTxt);
        numTxt->setColor({178,162,139});
        numTxt->setPosition(ccp(545, numH));
        numTxt->setAnchorPoint(ccp(1, 1));
    }
    totalH = -totalH;
    int index = 0;
    int count = 0;
    if(node->getChildrenCount()>0){
        count = node->getChildrenCount();
    }
    while (index < count) {
        auto child = dynamic_cast<CCNode*>(node->getChildren().at(index));
        if (child) {
            child->setPositionY(child->getPositionY() + totalH);
        }
        index++;
    }
    m_scrollView->setContentSize(CCSize(580, totalH));
    m_scrollView->setContentOffset(ccp(0, 360 - totalH));
    return true;
}

bool TroopInformationView::init() {
    BaseTileInfo::init(true);
    setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
        // detect touch inside panel
        if (! isTouchInside(m_tileBg, pTouch) )
        {
            this->closeThis();
        }
    });
    
    sendRequest();
    
    return true;
}

void TroopInformationView::sendRequest() {
    if(m_marchId == ""){
        return;
    }
    bool isHeiqishi = false;
    auto &marchInfos = WorldController::getInstance()->m_marchInfo;
    auto it = marchInfos.find(m_marchId);
    if (it != marchInfos.end()) {
        auto &info = it->second;
        if (info.marchType == MethodHeiqishi) {
            isHeiqishi = true;
        }
    }
    auto cmd = new WorldMarchDetailUpdateCommand(m_marchId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TroopInformationView::onRequestCallback), NULL));
    if (isHeiqishi) {
        cmd->putParam("msFlag", CCInteger::create(1));
    }
    cmd->sendAndRelease();
}

void TroopInformationView::onRequestCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (info && info->objectForKey("uuid")) {
        string uuid = info->valueForKey("uuid")->getCString();
        if (!uuid.empty()) {
            auto infoNode = TroopInformation::create(info);
            auto bgSize = m_tileBg->getContentSize();
            auto pos = ccp(bgSize.width / 2, bgSize.height - 40);
            infoNode->setPosition(pos);
            m_subNode->addChild(infoNode);
            
            int totalTroops = 0;
            if(info->objectForKey("totalTroops")){
                totalTroops = info->valueForKey("totalTroops")->intValue();
            }
            if(info->objectForKey("members")){
                CCArray* arr = dynamic_cast<CCArray*>(info->objectForKey("members"));
                if (arr!=NULL) {
                    int num = arr->count();
                    for (int i=0; i<num; i++) {
                        auto tDic = _dict(arr->objectAtIndex(i));
                        if (tDic && tDic->objectForKey("totalTroops")) {
                            totalTroops += tDic->valueForKey("totalTroops")->intValue();
                        }
                    }
                }
            }
            std::string titleStr = _lang("108585");//_lang("108638");
//            titleStr.append(" ");
//            if (totalTroops!=0) {
//                titleStr.append(CCCommonUtils::getResourceStr(totalTroops).c_str());
//            }
            m_title->setString(titleStr);
        }
    }
}


static float _panel_height = 220.0;


bool TroopInfoPanel::init() {
    NewBaseTileInfo::init(false, true);
    this->setModelLayerTouchPriority(3);
    auto &marchInfos = WorldController::getInstance()->m_marchInfo;
    auto it = marchInfos.find(m_marchId);
    if (it == marchInfos.end()) {
        return false;
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
    auto &info = it->second;
    
    double remainTime = info.endStamp - WorldController::getInstance()->getTime();
    if (remainTime <= 0) {
        return false;
    }

    std::string nameStr = info.playerName;
    if(info.asn != ""){
        nameStr = CCString::createWithFormat("(%s)%s", info.asn.c_str(), info.playerName.c_str())->getCString();
    }
    this->m_nameAndAlliance->setString(nameStr);
    m_nameAndAlliance->setPositionX(0);
    m_infoBG->setScaleY(1.05);
    this->m_nameAndAlliance->setPositionY(m_nameAndAlliance->getPositionY() - 2);
    m_tilePoint->setPositionY(m_tilePoint->getPositionY() - 14);
    m_tilePoint->setPositionX(-5);
    m_faveIcon->setVisible(false);
    m_addNode->setVisible(false);
    // organise state str
    CCLabelIF* stateLabel = nullptr;
    switch (info.stateType) {
        case StateMarch:{
            stateLabel = CCLabelIF::create(_lang(MarchStateName.at("depart")).c_str());
        }
            break;
        case StateReturn:{
            stateLabel = CCLabelIF::create(_lang(MarchStateName.at("return")).c_str());
        }
            break;
        default:
            break;
    }
    
    CCSize size = this->getContentSize();
    this->setPosition(ccp(0, size.height/8));
    if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
    {
        this->setPosition(ccp(0, size.height/60));
    }
    
    bool isSelf = info.ownerType == PlayerSelf;
    if(info.marchType == MethodRally && !isSelf){
        for (auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++) {
            if(it->second.teamUid == info.uuid && it->second.uuid != info.uuid){
                isSelf = (it->second.ownerType == PlayerSelf);
                break;
            }
        }
    }
    if (isSelf && info.marchType != MethodHeiqishi) {
        if(info.stateType == StateReturn){
            if (info.marchType == MethodDeal || info.marchType == MethodWarehouse) {
                setButtonCount(1);
                setButtonName(1, _lang("108558"));
                setButtonCallback(1, cccontrol_selector(TroopInfoPanel::clickSpeedUp));
                setButtonState(1, ButtonSpeedUp);
                getButton(1)->setTouchPriority(Touch_Default);
            }
            else {
                setButtonCount(2);
                setButtonName(3, _lang("108558"));
                setButtonCallback(3, cccontrol_selector(TroopInfoPanel::clickSpeedUp));
                setButtonState(3, ButtonSpeedUp);
                getButton(3)->setTouchPriority(Touch_Default);
                setButtonName(2, _lang("108557"));
                setButtonCallback(2, cccontrol_selector(TroopInfoPanel::onMarchInfoClick));
                setButtonState(2, ButtonViewTroop);
                getButton(2)->setTouchPriority(Touch_Default);
            }
        }else{
            setButtonCount(3);
            setButtonName(3, _lang("108558"));
            setButtonCallback(3, cccontrol_selector(TroopInfoPanel::clickSpeedUp));
            setButtonState(3, ButtonSpeedUp);
            
            if (info.marchType == MethodDeal || info.marchType == MethodScout || info.marchType == MethodWarehouse){
                setButtonName(2, _lang("108556"));
                setButtonCallback(2, cccontrol_selector(TroopInfoPanel::onCallBackClick));
                setButtonState(2, ButtonRecall);
            }
            else
            {
                setButtonName(2, _lang("108557"));
                setButtonCallback(2, cccontrol_selector(TroopInfoPanel::onMarchInfoClick));
                setButtonState(2, ButtonViewTroop);
                
                if(info.marchType == MethodRally){//添加组队行军召回
                    setButtonName(1, _lang("115191"));//解散
                    setButtonCallback(1, cccontrol_selector(TroopInfoPanel::onCallBackClick));
                    setButtonState(1, ButtonRecall);
                }else if(info.marchType != MethodUnion){// && info.marchType != MethodRally
                    setButtonName(1, _lang("108556"));
                    setButtonCallback(1, cccontrol_selector(TroopInfoPanel::onCallBackClick));
                    setButtonState(1, ButtonRecall);
                }
            }
            
            getButton(1)->setTouchPriority(Touch_Default);
            getButton(2)->setTouchPriority(Touch_Default);
            getButton(3)->setTouchPriority(Touch_Default);
        }
    }
    else {
        if(GlobalData::shared()->playerInfo.isInAlliance() && info.marchType == MethodHeiqishi && info.marchToIndexLeague == GlobalData::shared()->playerInfo.allianceInfo.uid){
            setButtonCount(1);
            setButtonName(1, _lang("108557"));
            setButtonCallback(1, cccontrol_selector(TroopInfoPanel::onMarchInfoClick));
            setButtonState(1, ButtonAllianceAct);
        }
        else{
            setButtonCount(0);
        }
    }

    schedule(schedule_selector(TroopInfoPanel::timeCount), 0.2f);
    
    return true;
}

bool TroopInfoPanel::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(getButton(1), pTouch) || isTouchInside(getButton(2), pTouch)
        || isTouchInside(getButton(3), pTouch)){
        return false;
    }
    return true;
}

void TroopInfoPanel::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void TroopInfoPanel::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    closeThis();
}

void TroopInfoPanel::onEnter(){
    NewBaseTileInfo::onEnter();
}

void TroopInfoPanel::onExit(){
    NewBaseTileInfo::onExit();
}

void TroopInfoPanel::clickSpeedUp(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent) {
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupInView(UseCDToolView::create(m_marchId,CCD_MARCH_TYPE));
    this->closeThis();
}
    
void TroopInfoPanel::onMarchInfoClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    auto world = WorldMapView::instance();
    if (world) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        world->addPopupView(TroopInformationView::create(m_marchId));
    }
    this->closeThis();
}

void TroopInfoPanel::onCallBackClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    //getButton(2)->setEnabled(false);
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    auto &marchInfos = WorldController::getInstance()->m_marchInfo;
    auto it = marchInfos.find(m_marchId);
    if (it == marchInfos.end()) {
        return;
    }
    
    auto &info = it->second;

    if (info.stateType == StateOccupy) {
        auto dict = CCDictionary::create();
        dict->setObject(CCString::create(info.uuid), "marchId");
        WorldMapView::instance()->afterMarchCancel(dict);
    }else if (info.marchType == MethodRally){
        auto dict = CCDictionary::create();
        dict->setObject(CCString::create(info.uuid), "marchId");
        PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_MARCH_CANCEL_RALLY,dict));
    }else {
        auto dict = CCDictionary::create();
        dict->setObject(CCString::create(info.uuid), "marchId");
        PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_MARCH_CANCEL,dict));
    }
    this->closeThis();
}

void TroopInfoPanel::timeCount(float dt) {
    auto &marchInfos = WorldController::getInstance()->m_marchInfo;
    auto it = marchInfos.find(m_marchId);
    if (it == marchInfos.end()) {
        closeThis();
        return;
    }
    
    auto &info = it->second;
    
    double remainTime = info.endStamp - WorldController::getInstance()->getTime();
    if (remainTime <= 0) {
        closeThis();
        return;
    }
    CCPoint endPt = WorldController::getPointByIndex(info.endPointIndex);

    string stateStr = "";
    switch (info.stateType) {
        case StateMarch:{
            if (info.marchType == MethodDeal){
                stateStr = _lang("102280") + "\n"; //"Trade\n"
            }
            else if (info.marchType == MethodScout)
            {
                stateStr = _lang("108539") + "\n"; //"Scout\n"
            }
            else {
                stateStr = _lang("108562") + "\n";//"Attack\n";
            }
        }
            break;
        case StateReturn:{
            stateStr = _lang("108572") + "\n";//"return\n";
        }
            break;
        default:
            break;
    }
    std::string str = stateStr + std::string("To X:") + CC_ITOA(endPt.x) + ", Y:" + CC_ITOA(endPt.y);
    if(info.ownerType != PlayerSelf){
        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_WATCH_TOWER);
        int lv3 = GlobalData::shared()->imperialInfo[bid].level;
        str = stateStr + CCCommonUtils::timeLeftToCountDown(remainTime / 1000);
    }
    m_tilePoint->setString(str);
    
}

void TroopInfoPanel::closeThis() {
    NewBaseTileInfo::closeThis();
    auto world = WorldMapView::instance();
    if (world) {
        world->inFollowView = false;
        world->followMarchId = "";
    }
}

TroopArmyCell *TroopArmyCell::create(std::string name,std::string num,std::string armyId){
    TroopArmyCell *ret = new TroopArmyCell(name,num,armyId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TroopArmyCell::init(){
    auto node = CCBLoadFile("TroopArmyCell", this, this);
    this->setContentSize(node->getContentSize());
    m_nameTxt->setString(m_nameStr);
    m_numTxt->setString(m_numStr);
    int index = atof(m_armyId.c_str())/100 - 1070 + 200;
    std::string icon = "ico" + m_armyId+ "_small.png";//CCCommonUtils::getIcon(m_armyId);
    CCLoadSprite::doResourceByCommonIndex(index, true);
    auto pic = CCLoadSprite::createSprite(icon.c_str());
    m_iconNode->addChild(pic);
    CCCommonUtils::setSpriteMaxSize(pic, 90);
    CCLoadSprite::doResourceByCommonIndex(index, false);
    m_levelNode->removeAllChildren();
    string num1 = m_armyId.substr(m_armyId.size()-2);
    auto pic1 = CCCommonUtils::getRomanSprite(atoi(num1.c_str())+1);
    m_levelNode->addChild(pic1);
    return true;
}

bool TroopArmyCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelNode", CCNode*, this->m_levelNode);
    
    return false;
}
