//
//  NewEnemyInfoMoreView.cpp
//  IF
//
//  Created by 李锐奇 on 14-9-16.
//
//

#include "NewEnemyInfoMoreView.h"
#include "EnemyInfoController.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "SceneController.h"
#include "OccupyMailPopUpView.h"
#include "WorldController.h"
#include "WatchtowerView.h"
#include "YuanYunReturnCommand.h"
#include "WorldMapView.h"

/*
 1.玩家名称，目的
 2.坐标
 3.抵达时间
 4.部队总数
 5.所有玩家信息
 7.行军部队里面的兵种
 8.兵种和数量
 9.科技
 10.技能和天赋
 */
NewEnemyInfoMoreView *NewEnemyInfoMoreView::create(std::string uid, int buildId, int type){
    NewEnemyInfoMoreView *ret = new NewEnemyInfoMoreView();
    if(ret && ret->init(uid, buildId, type)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void NewEnemyInfoMoreView::updateInfo(){
    
}

void NewEnemyInfoMoreView::onEnter(){
    PopupBaseView::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void NewEnemyInfoMoreView::onExit(){
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    PopupBaseView::onExit();
}

bool NewEnemyInfoMoreView::init(std::string uid, int buildId, int type){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(204, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(204, false);
        });
        setIsHDPanel(true);
        auto bg = CCBLoadFile("EnemyInfoDetailView", this, this);
        this->setContentSize(bg->getContentSize());
        if (!CCCommonUtils::isIosAndroidPad()) {
            int preH = m_buildBG->getContentSize().height;
            changeBGHeight(m_buildBG);
            int dy = m_buildBG->getContentSize().height - preH;
            if (m_btype == MethodUnion || m_btype == MethodYuanSolider)
            {
                //援助士兵和集结士兵添加遣返按钮
                m_goHomeBtn->setVisible(true);
                m_goHomeBtn->setPositionY(m_goHomeBtn->getPositionY() - dy);
                m_ignoreBtn->setPositionY(m_ignoreBtn->getPositionY() - dy);
                m_goHomeBtn->setPositionX(m_goHomeBtn->getPositionX() - 150);
                m_ignoreBtn->setPositionX(m_ignoreBtn->getPositionX() + 150);
                CCCommonUtils::setButtonTitle(m_goHomeBtn, _lang("115157").c_str());
                m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dy - 30));
            }
            else
            {
                m_ignoreBtn->setPositionY(m_ignoreBtn->getPositionY() - dy);
                m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dy + 60));//+ 90
            }
            
            m_ignoreBtn->setEnabled(true);
        }
        else {
            if (m_btype == MethodUnion || m_btype == MethodYuanSolider)
            {
                //援助士兵和集结士兵添加遣返按钮
                m_goHomeBtn->setVisible(true);
                m_goHomeBtn->setPositionX(m_goHomeBtn->getPositionX() - 300);
                m_ignoreBtn->setPositionX(m_ignoreBtn->getPositionX() + 300);
                CCCommonUtils::setButtonTitle(m_goHomeBtn, _lang("115157").c_str());
            }
        }
        m_uid = uid;
        m_buildId = buildId;
        m_btype = type;
        m_h = 0;
        m_headImgNode = HFHeadImgNode::create();
        CCCommonUtils::setButtonTitle(m_ignoreBtn, _lang("139501").c_str());
        m_goHomeBtn->setVisible(false);
        m_ignoreBtn->setEnabled(true);
        std::string ignoreStr = GlobalData::shared()->playerInfo.uid + "WTIgnoreList";
        std::string ignoreList = CCUserDefault::sharedUserDefault()->getStringForKey(ignoreStr.c_str(), "");
        vector<std::string> vector1;
        CCCommonUtils::splitString(ignoreList, "|", vector1);
        int num = vector1.size();
        for (int i = 0; i < num; i++) {
            if (vector1[i] == m_uid) {
                m_ignoreBtn->setEnabled(false);
                break;
            }
        }
        
        m_container = CCNode::create();
        m_scrollView = CCScrollView::create(m_infoList->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_infoList->addChild(m_scrollView);
        m_scrollView->addChild(m_container);
        
        addMarchInfo();
        addResourceInfo();
        addArmyInfo();
        addAbilityInfo();
        addScienceInfo();
        m_progressText->setString(_lang("108611"));
        update(1.0f);
        
        int index = 0;
        auto &arr = m_container->getChildren();
        for (auto child : arr)
        {
            auto node = dynamic_cast<CCNode*>(child);
            node->setPositionY(node->getPositionY() + m_h);
        }
//        while (arr && index < arr->count()) {
//            auto node = dynamic_cast<CCNode*>(arr->objectAtIndex(index++));
//            node->setPositionY(node->getPositionY() + m_h);
//        }
        m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width, m_h));
        m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_h));
        
        ret = true;
    }
    return ret;
}

 bool NewEnemyInfoMoreView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
     EnemyInfo *info = getEnemyInfo();
     if((isContains("2", m_buildId)||info->type == ENEMY_TYPE_MONSTER_ATTACK )&& (isTouchInside(m_coordTextNode, pTouch)||isTouchInside(m_coordTextNode2, pTouch))){
         return true;
     }
     return false;
}

void NewEnemyInfoMoreView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    EnemyInfo *info = getEnemyInfo();
    if(info == NULL){
        return;
    }
    //点击出发坐标
    if((isContains("2", m_buildId) && isTouchInside(m_coordTextNode, pTouch)) || info->type == ENEMY_TYPE_MONSTER_ATTACK){
        CCPoint pt = WorldController::getPointByIndex(info->indexFrom);
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
            PopupViewController::getInstance()->removeAllPopupView();
        }else{
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, info->indexFrom);
        }
    }
    //点击目的地坐标
    if((isContains("2", m_buildId) && isTouchInside(m_coordTextNode2, pTouch)) || info->type == ENEMY_TYPE_MONSTER_ATTACK){
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, info->indexTo);
    }
}

void NewEnemyInfoMoreView::onGoHomeClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    YuanYunReturnCommand* cmd = new YuanYunReturnCommand(m_uid);
    if (m_btype == MethodUnion)
    {
        EnemyInfo* info = getEnemyInfo();
        if (info)
        {
            cmd->putParam("teamId", ccs(info->teamId));
        }
    }
    cmd->sendAndRelease();
    vector<EnemyInfo>::iterator it = EnemyInfoController::getInstance()->m_enemyInfos.begin();
    for (; it != EnemyInfoController::getInstance()->m_enemyInfos.end(); it++)
    {
        if (it->uuid == m_uid)
        {
            EnemyInfoController::getInstance()->m_enemyInfos.erase(it);
            break;
        }
    }
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(WatchtowerView::create(m_buildId), true, true);
}

void NewEnemyInfoMoreView::onClickIgnoreBtn(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    std::string ignoreStr = GlobalData::shared()->playerInfo.uid + "WTIgnoreList";
    std::string ignoreList = CCUserDefault::sharedUserDefault()->getStringForKey(ignoreStr.c_str(), "");
    if (ignoreList == "") {
        ignoreList.append(m_uid);
    }
    else {
        ignoreList.append("|");
        ignoreList.append(m_uid);
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(ignoreStr.c_str(), ignoreList.c_str());
    CCUserDefault::sharedUserDefault()->flush();
    m_ignoreBtn->setEnabled(false);
}

SEL_CCControlHandler NewEnemyInfoMoreView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoHomeClick", NewEnemyInfoMoreView::onGoHomeClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickIgnoreBtn", NewEnemyInfoMoreView::onClickIgnoreBtn);
    return NULL;
}

bool NewEnemyInfoMoreView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", Label*, m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stateText", CCLabelIF*, m_stateText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordText", CCLabelIF*, m_coordText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressText", CCLabelIF*, m_progressText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress", CCScale9Sprite*, m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picContainer", CCNode*, m_picContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordTextNode", CCNode*, m_coordTextNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goHomeBtn", CCControlButton*, m_goHomeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ignoreBtn", CCControlButton*, m_ignoreBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picContainer2", CCNode*, m_picContainer2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText2", Label*, m_nameText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordTextNode2", CCNode*, m_coordTextNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coordText2", CCLabelIF*, m_coordText2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeEnemy", CCNode*, m_nodeEnemy);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodePlayer", CCNode*, m_nodePlayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_labEnemyLv", CCLabelIF*, m_labEnemyLv);

    return false;
}

bool NewEnemyInfoMoreView::isContains(std::string index, int buildId){
    auto m_info = FunBuildController::getInstance()->getFunbuildById(buildId);
    std::string para = m_info.para[1];
    std::vector<std::string> vector;
    CCCommonUtils::splitString(para, ";", vector);
    int i = 0;
    while (i < vector.size()) {
        std::string p = vector[i++];
        if(p == index){
            return true;
        }
    }
    return false;
}

void NewEnemyInfoMoreView::addMarchInfo(){
    EnemyInfo *info = getEnemyInfo();
    if(info == NULL){
        return;
    }
    if(isContains("1", m_buildId) || info->type == ENEMY_TYPE_MONSTER_ATTACK){
        auto pic = CCLoadSprite::createSprite((info->pic + ".png").c_str());
        //对方头像
        this->m_picContainer->addChild(pic);
        int size = 100;
        if (CCCommonUtils::isIosAndroidPad()) {
            size = 200;
        }
        CCCommonUtils::setSpriteMaxSize(pic, size);//100
        if (CCCommonUtils::isUseCustomPic(info->picVer))
        {
            m_headImgNode->initHeadImgUrl2(m_picContainer, CCCommonUtils::getCustomPicUrl(info->uid, info->picVer), 1.0f, size, false);
        }
        //自己图像
        string picName = info->pic+".png";//默认 出征者头像
        bool useCustomPic = true;
        if (info->type == ENEMY_TYPE_BATTLE) {//被攻击
            picName = "march_battle.png";
            useCustomPic = false;
        }
        else if (info->type == ENEMY_TYPE_SCOUT) {//侦查
            picName = "march_scout.png";
            useCustomPic = false;
        }
        else if (info->type == ENEMY_TYPE_TRADE) {//交易
            picName = "march_trade.png";
            useCustomPic = false;
        }
        else if (info->type == ENEMY_TYPE_TEAM) {//集结
            picName = "march_team.png";
            useCustomPic = false;
        }
        else if (info->type == ENEMY_TYPE_MEMBER) {//协防
            picName = "march_member.png";
            useCustomPic = false;
        }
        else if (info->type == ENEMY_TYPE_MONSTER_ATTACK) {//世界活动，怪物攻城
            picName = "tile_pop_icon21.png";
            useCustomPic = false;
        }
        auto pic2 = CCLoadSprite::createSprite(picName.c_str());
        this->m_picContainer2->addChild(pic2);
        CCCommonUtils::setSpriteMaxSize(pic2, size);
        if (useCustomPic && CCCommonUtils::isUseCustomPic(info->picVer))
        {
            m_headImgNode->initHeadImgUrl2(m_picContainer2, CCCommonUtils::getCustomPicUrl(info->uid, info->picVer), 1.0f, size, false);
        }
        
        if (info->type == ENEMY_TYPE_MONSTER_ATTACK) {
            int mType = 0;
            int color = 1;
            string monsterStr = CCCommonUtils::getPropById(info->uid, "monster");
            if(monsterStr=="heiqishi_1"){
                mType = 3;
                color = 0;
            }else if (monsterStr=="heiqishi_2"){
                mType = 3;
                color = 1;
            }else{
                mType = 1;
                color = 3;
            }
            auto preBg = CCLoadSprite::createSprite(CCString::createWithFormat("Activty_icon_BG%d.png",mType)->getCString());
            CCCommonUtils::setSpriteMaxSize(preBg, size,true);
            this->m_picContainer->addChild(preBg,100);
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, size,true);
            this->m_picContainer->addChild(iconBg,-100);
        }
        //对方名字
        std::string nameStr = info->name;
        if(info->abbr != ""){
            nameStr = std::string("(") + info->abbr + ")" + info->name;
        }
        this->m_nameText->setString(nameStr.c_str());
        //对方等级
        m_labEnemyLv->setString(_lang_1("102272", CC_ITOA(info->level)));
        //目的地类型
        std::string typeStr = "123";//info->name;
        switch (info->defType) {
            case 1://玩家城市
                typeStr+="玩家城市";
                break;
            case 2://扎营地
                typeStr+="扎营地";
                break;
            case 3://资源
                typeStr+="资源";
                break;
            default:
//                typeStr+="？";
                break;
        }
        this->m_nameText2->setString(typeStr.c_str());
        //行军类型
        string strTypeName = _lang("108586");
        if (info->type == ENEMY_TYPE_BATTLE) {
            strTypeName = _lang("108607");
        }
        else if (info->type == ENEMY_TYPE_SCOUT) {
            strTypeName = _lang("108605");
        }
        else if (info->type == ENEMY_TYPE_TRADE) {
            strTypeName = _lang("108606");
            m_stateText->setColor(ccc3(62,85,46));
//            m_nameText->setColor(ccc3(62,85,46));
        }
        else if (info->type == ENEMY_TYPE_TEAM) {
            strTypeName = _lang("108608");
        }
        else if (info->type == ENEMY_TYPE_MEMBER) {
            strTypeName = _lang("108582");
            m_stateText->setColor(ccc3(62,85,46));
//            m_nameText->setColor(ccc3(62,85,46));
        }else if(info->type == ENEMY_TYPE_MONSTER_ATTACK){
            strTypeName = _lang("108607");
            string mName = CCCommonUtils::getNameById(info->uid);
            this->m_nameText->setString(mName.c_str());
        }
        else
        {
            m_stateText->setColor(ccc3(62,85,46));
//            m_nameText->setColor(ccc3(62,85,46));
        }
        
        if ((info->defType == Tile_allianceArea || info->defType == tile_tower || info->defType == tile_banner) && info->allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
            strTypeName = _lang("108586");
        }
        
        m_stateText->setString(strTypeName);
        m_stateText->setColor(ccc3(120, 24, 10));
    }
    m_coordTextNode->setVisible(false);
    if(isContains("2", m_buildId)||info->type == ENEMY_TYPE_MONSTER_ATTACK){
        //出发坐标
        this->m_coordText->setString(info->xyFrom);
        m_coordText->setColor(ccc3(52, 101, 143));
        m_coordTextNode->setVisible(true);
        //目的地坐标
        this->m_coordText2->setString(info->xyTo);
        m_coordText2->setColor(ccc3(52, 101, 143));
    }
    //不显示己方信息
    m_nodePlayer->setVisible(false);
    //不显示攻击lab
//    m_stateText->setVisible(false);
}

void NewEnemyInfoMoreView::addResourceInfo(){
    EnemyInfo *info = getEnemyInfo();
    if(info == NULL){
        return;
    }
    if(!info->genDic->objectForKey("tradeRes")){
        return;
    }

    if(info && info->type == ENEMY_TYPE_TRADE){
        auto cell = addTitleCell(_lang("121997"), "");
        int startY = 0;
        if (CCCommonUtils::isIosAndroidPad()) {
            startY = -m_h - 100;
        }
        else
            startY = -m_h - 70;
        
        int h = 0;
        CCArray *arr = dynamic_cast<CCArray*>(info->genDic->objectForKey("tradeRes"));
        CCObject *obj;
        int gapY = 0;
        if (CCCommonUtils::isIosAndroidPad()) {
            gapY = 90;
        }
        else
            gapY = 45;
        
        CCARRAY_FOREACH(arr, obj){
            auto dict = _dict(obj);
            int type = dict->valueForKey("t")->intValue();
            int num = dict->valueForKey("v")->intValue();
            if (num <= 0) {
                continue;
            }
            auto typeSprite = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(type).c_str());
            typeSprite->setAnchorPoint(ccp(0, 1));
            typeSprite->setPosition(ccp(30, startY));
            
            auto numText = CCLabelIF::create(CC_ITOA(num));
            numText->setAnchorPoint(ccp(1, 1));
//            numText->setPosition(580, typeSprite->getPositionY() - (typeSprite->getContentSize().height - numText->getContentSize().height * numText->getOriginScaleY()) / 2);
            if (CCCommonUtils::isIosAndroidPad()) {
                numText->setPosition(1400, startY-10);
            }
            else
                numText->setPosition(580, startY-10);
            
            numText->setColor(ccc3(46, 23, 0));

            m_container->addChild(typeSprite);
            m_container->addChild(numText);
            h += gapY;
            startY -= gapY;
        }
        cell->setBGHeight(h);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_h += cell->getBGHeight() + 10;
        }
        else
            m_h += cell->getBGHeight() + 5;
    }
}

void NewEnemyInfoMoreView::addAbilityInfo(){
    EnemyInfo *info = getEnemyInfo();
    if(info == NULL){
        return;
    }

    if(isContains("10", m_buildId)){
        auto cell = addTitleCell(_lang("105533"), "");
        int startY = 0;
        if (CCCommonUtils::isIosAndroidPad()) {
            startY = -m_h - 100;
        }
        else
            startY = -m_h - 70;
        
        int h = 0;

        int index = 0;
        while (index < info->ablityTypes.size()) {
            auto cell = AbilityCell::create(CC_ITOA(info->ablityTypes[index++]));
            cell->setPosition(30, startY);
            m_container->addChild(cell);
            int gapY = cell->getH() + 5;
            h += gapY;
            startY -= gapY;
        }
        cell->setBGHeight(h);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_h += cell->getBGHeight() + 10;
        }
        else
            m_h += cell->getBGHeight() + 5;
    }
}

void NewEnemyInfoMoreView::addScienceInfo(){
    EnemyInfo *info = getEnemyInfo();
    if(info == NULL){
        return;
    }

    if(isContains("9", m_buildId)){
        auto cell = addTitleCell(_lang("105532"), "");
        int startY = 0;
        if (CCCommonUtils::isIosAndroidPad()) {
            startY = -m_h - 100;
        }
        else
            startY = -m_h - 70;
        int h = 0;

        for(auto it = info->science.begin(); it != info->science.end(); it++){
            auto cell = ScienceInfoCell::create(CC_ITOA(it->first), it->second);
            cell->setPosition(30, startY);
            m_container->addChild(cell);
            int gapY = cell->getH() + 5;
            h += gapY;
            startY -= gapY;
        }
        cell->setBGHeight(h);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_h += cell->getBGHeight() + 10;
        }
        else
            m_h += cell->getBGHeight() + 5;
    }
}

void NewEnemyInfoMoreView::addArmyInfo(){
    EnemyInfo *info = getEnemyInfo();
//    info->genDic->writeToFile("/Users/zhangjun/Desktop/dictf/genDic");
    if(info == NULL){
        return;
    }
    if(info->type != ENEMY_TYPE_BATTLE && info->type != ENEMY_TYPE_TEAM && info->type != ENEMY_TYPE_MEMBER && info->type != ENEMY_TYPE_MONSTER_ATTACK){
        return;
    }
    if(isContains("4", m_buildId) || isContains("5", m_buildId) || isContains("7", m_buildId) || isContains("8", m_buildId) || info->type == ENEMY_TYPE_MONSTER_ATTACK){
        int h = 0;
        int total = 0;
        //generate data
        CCArray *arr = CCArray::create();
        //add leader
        if(info->monsterCount>=1 && info->genDic->objectForKey("team")){//怪物攻城
            CCArray *mTeams = dynamic_cast<CCArray*>(info->genDic->objectForKey("team"));
            if (mTeams!=NULL) {
                for (int i=0;i<mTeams->count(); i++) {
                    auto mDic = _dict(mTeams->objectAtIndex(i));
                    string mId = mDic->valueForKey("monsterId")->getCString();
                    string monsterIcon = CCCommonUtils::getPropById(mId, "monster");
                    string strName = CCCommonUtils::getNameById(mId.c_str());
                    int mTotal = 0;
                    CCArray* arr1 =  dynamic_cast<CCArray*>(mDic->objectForKey("soldiers"));
                    CCDictionary *leader = CCDictionary::create();
                    leader->setObject(CCString::create(strName), "name");
                    leader->setObject(CCString::create(CC_ITOA(info->level)), "level");//发起攻击者等级
                    leader->setObject(CCString::create(""), "asn");
                    leader->setObject(CCString::create(monsterIcon), "monsterIcon");
                    leader->setObject(CCString::create("tile_pop_icon21"), "pic");
                    if (arr1!=NULL) {
                        for(int k=0;k<arr1->count();k++){
                            auto soldierDic = _dict(arr1->objectAtIndex(k));
                            mTotal += soldierDic->valueForKey("count")->intValue();
                        }
                        total += mTotal;
                        leader->setObject(arr1, "soldier");
                    }
                    leader->setObject(CCString::create(CC_ITOA(mTotal)), "total_num");
                    arr->addObject(leader);
                }
            }
        }else{
            CCDictionary *leader = CCDictionary::create();
            leader->setObject(CCString::create(info->name), "name");
            leader->setObject(CCString::create(CC_ITOA(info->level)), "level");
            leader->setObject(CCString::create(info->abbr), "asn");
            leader->setObject(CCString::create(info->pic), "pic");
            leader->setObject(CCString::create(CC_ITOA(info->soldiers)), "total_num");
            CCArray *leadSoldier = CCArray::create();
            for (auto it = info->army.begin(); it != info->army.end(); it++) {
                CCDictionary *s = CCDictionary::create();
                s->setObject(CCString::create(CC_ITOA(it->first)), "id");
                s->setObject(CCString::create(CC_ITOA(it->second)), "count");
                leadSoldier->addObject(s);
                total += it->second;
            }
            leader->setObject(leadSoldier, "soldier");
            arr->addObject(leader);
        }
        //add team
        CCArray *teamSoldier = NULL;
        if(info->genDic->objectForKey("team")){
            teamSoldier = dynamic_cast<CCArray*>(info->genDic->objectForKey("team"));
        }
        if(teamSoldier && info->type != ENEMY_TYPE_MONSTER_ATTACK){
            int index = 0;
            while (index < teamSoldier->count()) {
                auto soldierDict = _dict(teamSoldier->objectAtIndex(index));
                
                auto dict = CCDictionary::create();
                dict->setObject(CCString::create(soldierDict->valueForKey("name")->getCString()), "name");
                dict->setObject(CCString::create(soldierDict->valueForKey("level")->getCString()), "level");
                dict->setObject(CCString::create(soldierDict->valueForKey("abbr")->getCString()), "asn");
                dict->setObject(CCString::create(soldierDict->valueForKey("pic")->getCString()), "pic");
                dict->setObject(CCString::create(soldierDict->valueForKey("totalTroops")->getCString()), "total_num");
                
                CCArray *arr1 = CCArray::create();
                auto sarr = dynamic_cast<CCArray*>(soldierDict->objectForKey("soldiers"));
                if (sarr)
                {
                    for (int i=0; i<sarr->count(); i++)
                    {
                        auto item = _dict(sarr->objectAtIndex(i));
                        CCDictionary *s = CCDictionary::create();
                        std::string armyId = item->valueForKey("armyId")->getCString();
                        int count = item->valueForKey("count")->intValue();
                        total += count;
                        s->setObject(CCString::create(armyId), "id");
                        s->setObject(CCString::create(CC_ITOA(count)), "count");
                        arr1->addObject(s);
                    }
                }

                dict->setObject(arr1, "soldier");
                arr->addObject(dict);
                index++;
            }
        }
        //

        auto cell = addTitleCell(_lang("102149"), CC_ITOA(total));
        int startY = 0;
        if (CCCommonUtils::isIosAndroidPad()) {
            startY = -m_h - 100;
        }
        else
            startY = -m_h - 70;

        if(isContains("7", m_buildId) || isContains("8", m_buildId)){
            cell->hideDecoration();
            if (CCCommonUtils::isIosAndroidPad()) {
                startY = -m_h - 80;
                h -= 30;
            }
            else {
                startY = -m_h - 55;
                h -= 15;
            }
        }
        if(info->type == ENEMY_TYPE_MONSTER_ATTACK || isContains("7", m_buildId) || isContains("8", m_buildId) || (isContains("5", m_buildId) && arr->count() > 1)){
            CCObject *obj;
            CCARRAY_FOREACH(arr, obj) {
                auto cell = ArmyInfoCell::create(_dict(obj), m_buildId, arr->count() != 1,info->type == ENEMY_TYPE_MONSTER_ATTACK);
                cell->setPosition(6, startY);
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(30, startY);
                }
                m_container->addChild(cell);
                int gapY = cell->getH() + 6;
                if (CCCommonUtils::isIosAndroidPad()) {
                    gapY += 10;
                }
                h += gapY;
                startY -= gapY;
            }
            m_h -= 6;
        }
        cell->setBGHeight(h);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_h += cell->getBGHeight() + 10;
        }
        else
            m_h += cell->getBGHeight() + 5;
    }
}

void NewEnemyInfoMoreView::update(float time){
    EnemyInfo *info = getEnemyInfo();
    if(info){
        if(isContains("3", m_buildId)||info->type == ENEMY_TYPE_MONSTER_ATTACK){
            int lastTime = info->arrivalTime - GlobalData::shared()->getWorldTime();
            std::string progressStr="";
            if (info->type == ENEMY_TYPE_BATTLE||info->type == ENEMY_TYPE_TEAM) {
                switch (info->defType) {
                    case 1://玩家城市
                        progressStr = _lang_1("137411", CC_SECTOA(lastTime));
                        break;
                    case 2://扎营地
                        progressStr = _lang_1("137413", CC_SECTOA(lastTime));
                        break;
                    case 3://资源
                        progressStr = _lang_1("137412", CC_SECTOA(lastTime));
                        break;
                    case tile_tower:
                        if (info->allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                            progressStr = _lang_1("115412", CC_SECTOA(lastTime));
                        }
                        break;
                    case Tile_allianceArea:
                        if (info->allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                            progressStr = _lang_1("115360", CC_SECTOA(lastTime));
                        }
                        break;
                    case tile_banner:
                        if (info->allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                            progressStr = _lang_1("115537", CC_SECTOA(lastTime));
                        }
                        break;
                    default:
                        break;
                }
            }
            if (info->type == 7) {//援助
                switch (info->defType) {
                    case 1://玩家城市
                        progressStr = _lang_1("137414", CC_SECTOA(lastTime));
                        break;
                    case 2://扎营地
                        progressStr = _lang_1("137416", CC_SECTOA(lastTime));
                        break;
                    case 3://资源
                        progressStr = _lang_1("137415", CC_SECTOA(lastTime));
                        break;
                    default:
                        break;
                }
            }
            if (info->type == ENEMY_TYPE_SCOUT) {//侦查
                switch (info->defType) {
                    case 1://玩家城市
                        progressStr = _lang_1("137426", CC_SECTOA(lastTime));
                        break;
                    case 2://扎营地
                        progressStr = _lang_1("137428", CC_SECTOA(lastTime));
                        break;
                    case 3://资源
                        progressStr = _lang_1("137427", CC_SECTOA(lastTime));
                        break;
                    default:
                        break;
                }
            }
            
            if (progressStr=="") {
                progressStr = CC_SECTOA(lastTime);
            }
            m_progressText->setString(progressStr);
            
            float fw = m_progressText->getContentSize().width;
            if (fw>700) {//防爆框
                m_progressText->setScaleX(700.0/fw);
            }
            
            float pro = 1 - lastTime * 1.0 / info->marchTime;
            if (pro < 0) {
                m_progress->setScaleX(0);
            } else {
                m_progress->setScaleX(pro);
            }
        }else{
            m_progress->setColor(ccBLACK);
            //***************************************************
            std::string progressStr="";
            if (info->type == ENEMY_TYPE_BATTLE||info->type == ENEMY_TYPE_TEAM) {
                switch (info->defType) {
                    case 1://玩家城市
                        progressStr = _lang("137417");
                        break;
                    case 2://扎营地
                        progressStr = _lang("137419");
                        break;
                    case 3://资源
                        progressStr = _lang("137418");
                        break;
                    case tile_tower:
                        if (info->allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                            progressStr = _lang("115413");
                        }
                        break;
                    case Tile_allianceArea:
                        if (info->allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                            progressStr = _lang("115361");
                        }
                        break;
                    case tile_banner:
                        if (info->allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                            progressStr = _lang("115538");
                        }
                        break;
                    default:
                        break;
                }
            }
            if (info->type == 7) {//援助
                switch (info->defType) {
                    case 1://玩家城市
                        progressStr = _lang("137420");
                        break;
                    case 2://扎营地
                        progressStr = _lang("137422");
                        break;
                    case 3://资源
                        progressStr = _lang("137421");
                        break;
                    default:
                        break;
                }
            }
            if (info->type == ENEMY_TYPE_SCOUT) {//侦查
                progressStr = _lang("137440");
            }
            if(progressStr==""){
                progressStr = _lang("108611");
            }
            m_progressText->setString(progressStr);
            //***************************************************
        }
    }else{
        PopupViewController::getInstance()->goBackPopupView();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WATCH_TOWER);
    }
}

EnemyInfo *NewEnemyInfoMoreView::getEnemyInfo(){
    int index = 0;
    while (index < EnemyInfoController::getInstance()->m_enemyInfos.size()) {
        EnemyInfo &info = EnemyInfoController::getInstance()->m_enemyInfos[index++];
        if(info.uuid == m_uid){
            return &info;
        }
    }
    return NULL;
}

TitleCell *NewEnemyInfoMoreView::addTitleCell(std::string title1, std::string title2){
    TitleCell *cell = TitleCell::create(title1, title2);
    m_container->addChild(cell);
    cell->setPosition(ccp(0, -m_h));
    return cell;
}
//-----

MarchInfoCell *MarchInfoCell::create(CCDictionary *dict){
    MarchInfoCell *ret = new MarchInfoCell();
    if(ret && ret->init(dict)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
    
void MarchInfoCell::onEnter(){
    CCNode::onEnter();
}

void MarchInfoCell::onExit(){
    CCNode::onExit();
}

bool MarchInfoCell::init(CCDictionary *dict){
    std::string picStr = dict->valueForKey("pic")->getCString();
    auto pic = CCLoadSprite::createSprite(picStr.c_str());
    CCCommonUtils::setSpriteMaxSize(pic, 96);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(pic, 200);
    }
    return true;
}

int MarchInfoCell::getH(){
    return m_h;
}

void MarchInfoCell::update(float time){
    
}

//-----

ArmyInfoCell *ArmyInfoCell::create(CCDictionary *dict, int buildId, bool showInfo, bool isMonsterAtt){
    ArmyInfoCell *ret = new ArmyInfoCell();
    if(ret && ret->init(dict, buildId, showInfo,isMonsterAtt)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
    
void ArmyInfoCell::onEnter(){
    CCNode::onEnter();
}

void ArmyInfoCell::onExit(){
    CCNode::onExit();
}

bool ArmyInfoCell::init(CCDictionary *dict, int buildId, bool showInfo, bool isMonsterAtt){
    CCScale9Sprite *decoration = NULL;
    int startY = 0;
    m_h = 0;
    if(NewEnemyInfoMoreView::isContains("7", buildId) || NewEnemyInfoMoreView::isContains("8", buildId) || isMonsterAtt){
        decoration = CCLoadSprite::createScale9Sprite("Mail_frame02.png");
        decoration->setAnchorPoint(ccp(0, 1));
        this->addChild(decoration);
        startY = -15;
        m_h += 10;
        if (CCCommonUtils::isIosAndroidPad()) {
            startY = -30;
            m_h += 20;
        }
    }
//    dict->writeToFile("/Users/zhangjun/Desktop/dictf/ArmyInfoCellDic");
    CCArray *soldier = dynamic_cast<CCArray*>(dict->objectForKey("soldier"));
    
    if(showInfo || isMonsterAtt){
        std::string name = dict->valueForKey("name")->getCString();
        std::string asn = dict->valueForKey("asn")->getCString();
        std::string picPath = dict->valueForKey("pic")->getCString();
        int level = dict->valueForKey("level")->intValue();
    
        auto picBG = CCLoadSprite::createSprite("icon_kuang.png");
        picBG->setAnchorPoint(ccp(0, 1));
        picBG->setPosition(ccp(35, startY));
        this->addChild(picBG);
    
        int mType = 0;
        if (isMonsterAtt) {
            int color =0;
            string monsterStr = dict->valueForKey("monsterIcon")->getCString();
            if(monsterStr=="heiqishi_1"){
                mType = 3;
                color = 0;
                
            }else if (monsterStr=="heiqishi_2"){
                mType = 2;
                color = 1;
            }else{
                mType = 1;
                color = 3;
            }
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
            iconBg->setPosition(ccp(picBG->getPositionX() + picBG->getContentSize().width / 2, picBG->getPositionY() - picBG->getContentSize().height / 2));
            CCCommonUtils::setSpriteMaxSize(iconBg, 100,true);
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(iconBg, 200,true);
            }
            this->addChild(iconBg,-100);
        }
        auto pic = CCLoadSprite::createSprite((picPath + ".png").c_str());
        pic->setPosition(ccp(picBG->getPositionX() + picBG->getContentSize().width / 2, picBG->getPositionY() - picBG->getContentSize().height / 2));
        this->addChild(pic);
        CCCommonUtils::setSpriteMaxSize(pic, picBG->getContentSize().width - 8);
        if (isMonsterAtt) {
            int tempType = mType;
            if (mType==2) {
                tempType = 3;
            }
            auto preBg = CCLoadSprite::createSprite(CCString::createWithFormat("Activty_icon_BG%d.png",tempType)->getCString());
            preBg->setPosition(pic->getPosition());
            CCCommonUtils::setSpriteMaxSize(preBg, 102,true);
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(preBg, 200,true);
            }
            this->addChild(preBg);
        }
        std::string nameStr = (asn == "" ? name : std::string("(") + asn + ")" + name);
        std::string levelStr = _lang_1("107005", CC_ITOA(level));
        nameStr+="  ";//等级
        nameStr+=levelStr;
        auto nameText = CCLabelIF::create(nameStr.c_str());
        nameText->setColor(ccc3(46, 23, 0));
        nameText->setFontSize(22);
        if (CCCommonUtils::isIosAndroidPad()) {
            nameText->setFontSize(45);
        }
        nameText->setAnchorPoint(ccp(0, 0.5));
        this->addChild(nameText);
        if (isMonsterAtt) {
            nameText->setPosition(picBG->getPositionX() + picBG->getContentSize().width + 10, picBG->getPositionY() - picBG->getContentSize().height / 2);
        }else{
            nameText->setPosition(picBG->getPositionX(), picBG->getPositionY() - picBG->getContentSize().height -20);//名字he等级
        }

        
        //等级
//        string levelStr ="Lv ";
//        levelStr+=CC_ITOA(level);
//        auto levelText = CCLabelIF::create(levelStr.c_str());
//        levelText->setAnchorPoint(ccp(0, 0.5));
//        levelText->setColor(ccc3(46, 23, 0));
//        levelText->setFontSize(22);
//        this->addChild(levelText);
//        levelText->setPosition(nameText->getPositionX()+nameText->getContentSize().width/2+20, nameText->getPositionY());
        //兵数
        int total_num = dict->valueForKey("total_num")->intValue();
        auto totalNum = CCLabelIF::create(CC_ITOA(total_num));
        totalNum->setAnchorPoint(ccp(1, 0.5));
        totalNum->setColor(ccc3(46, 23, 0));
        totalNum->setFontSize(22);
        this->addChild(totalNum);
        totalNum->setPosition(560, picBG->getPositionY() - picBG->getContentSize().height / 2);
        if (CCCommonUtils::isIosAndroidPad()) {
            totalNum->setPosition(1400, picBG->getPositionY() - picBG->getContentSize().height / 2);
        }
        m_h += picBG->getContentSize().height;
    }
    
    if(decoration){
        if(showInfo){
            auto line = CCLoadSprite::createSprite("Mail_line_new.png");
            this->addChild(line);
            line->setScale(0.9);
            line->setPosition(ccp(302, -m_h - 15 -30));
            if (CCCommonUtils::isIosAndroidPad()) {
                line->setPosition(ccp(768, -m_h - 15 -30));
            }
        
            m_h += 25 +30;
        }else{
            m_h += 10;
        }
        startY = -m_h;
        int gapY = 110;
        if (CCCommonUtils::isIosAndroidPad())
        {
            gapY = 220;
        }
        
        CCObject *obj = NULL;
        CCARRAY_FOREACH(soldier, obj) {
            auto soldierDict = _dict(obj);
            std::string armyId = soldierDict->valueForKey("id")->getCString();
            if (soldierDict->objectForKey("armyId")) {
                armyId = soldierDict->valueForKey("armyId")->getCString();
            }
            int count = soldierDict->valueForKey("count")->intValue();

//            auto nameLabel = CCLabelIF::create(CCCommonUtils::getNameById(armyId).c_str());
//            nameLabel->setAnchorPoint(ccp(0, 1));
//            nameLabel->setColor(ccc3(46, 23, 0));
//            nameLabel->setFontSize(20);
//            this->addChild(nameLabel);
//            nameLabel->setPosition(45, startY);
            if(!NewEnemyInfoMoreView::isContains("8", buildId) && !isMonsterAtt){
//                auto countLabel = CCLabelIF::create(CC_ITOA(count));
//                countLabel->setColor(ccc3(46, 23, 0));
//                countLabel->setFontSize(20);
//                countLabel->setAnchorPoint(ccp(1, 1));
//                this->addChild(countLabel);
//                countLabel->setPosition(560, nameLabel->getPositionY());
                count = -1;
            }
            auto armyCell = DetectArmyCell::create(armyId, count);
            this->addChild(armyCell);
            armyCell->setPosition(ccp(0, startY-110));
            if (CCCommonUtils::isIosAndroidPad()) {
                armyCell->setPosition(ccp(0, startY-220));
            }
            startY -= gapY;
            m_h += gapY;
        }
        m_h += 5;
        if (CCCommonUtils::isIosAndroidPad()) {
            m_h += 25;
        }
        decoration->setContentSize(CCSize(608, m_h));
        if (CCCommonUtils::isIosAndroidPad()) {
            decoration->setContentSize(CCSize(1476, m_h));
        }
    }
    return true;
}

int ArmyInfoCell::getH(){
    return m_h;
}

//-----

ScienceInfoCell *ScienceInfoCell::create(std::string sid, int level){
    ScienceInfoCell *ret = new ScienceInfoCell();
    if(ret && ret->init(sid, level)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ScienceInfoCell::onEnter(){
    CCNode::onEnter();
}

void ScienceInfoCell::onExit(){
    CCNode::onExit();
}

bool ScienceInfoCell::init(std::string sid, int level){
    CCLoadSprite::doResourceByCommonIndex(5, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(5, false);
    });
    
    auto bg = CCLoadSprite::createSprite("icon_kuang.png");
    bg->setAnchorPoint(ccp(0, 1));
    this->addChild(bg);
    if (CCCommonUtils::isIosAndroidPad()) {
        bg->setScale(2);
    }
    std::string picPath = CCCommonUtils::getIcon(sid);
    auto icon = CCLoadSprite::createSprite(picPath.c_str());
    this->addChild(icon);
    if (CCCommonUtils::isIosAndroidPad()) {
        icon->setScale(2);
    }
    icon->setPosition(ccp(bg->getContentSize().width / 2, -bg->getContentSize().height / 2));
    CCCommonUtils::setSpriteMaxSize(icon, bg->getContentSize().width);
    auto nameText = CCLabelIF::create(CCCommonUtils::getNameById(sid).c_str());
    nameText->setColor(ccc3(46, 23, 0));
    nameText->setFontSize(22);
    if (CCCommonUtils::isIosAndroidPad()) {
        nameText->setFontSize(45);
    }
    this->addChild(nameText);
    nameText->setAnchorPoint(ccp(0, 0.5));
    nameText->setPosition(bg->getContentSize().width + bg->getPositionX() + 10, bg->getPositionY() - bg->getContentSize().height / 2);
    auto levelText = CCLabelIF::create(CC_ITOA(level));
    levelText->setColor(ccc3(46, 23, 0));
    levelText->setFontSize(22);
    if (CCCommonUtils::isIosAndroidPad()) {
        levelText->setFontSize(45);
    }
    this->addChild(levelText);
    levelText->setAnchorPoint(ccp(1, 0.5));
    levelText->setPosition(560, nameText->getPositionY());
    if (CCCommonUtils::isIosAndroidPad()) {
        levelText->setPosition(1400, nameText->getPositionY());
    }
    m_h = bg->getContentSize().height;

    return true;
}

int ScienceInfoCell::getH(){
    return m_h;
}

//----

AbilityCell *AbilityCell::create(std::string abilityId){
    AbilityCell *ret = new AbilityCell();
    if(ret && ret->init(abilityId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
    
void AbilityCell::onEnter(){
    CCNode::onEnter();
}

void AbilityCell::onExit(){
    CCNode::onExit();
}

bool AbilityCell::init(std::string abilityId){
    CCLoadSprite::doResourceByCommonIndex(105, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(105, false);
    });

    auto bg = CCLoadSprite::createSprite("icon_kuang.png");
    bg->setAnchorPoint(ccp(0, 1));
    this->addChild(bg);
    if (CCCommonUtils::isIosAndroidPad()) {
        bg->setScale(2);
    }
    std::string id = abilityId;
    std::string picPath = CCCommonUtils::getIcon(id);
    auto icon = CCLoadSprite::createSprite(picPath.c_str());
    this->addChild(icon);
    if (CCCommonUtils::isIosAndroidPad()) {
        icon->setScale(2);
    }
    icon->setPosition(ccp(bg->getContentSize().width / 2, -bg->getContentSize().height / 2));
    CCCommonUtils::setSpriteMaxSize(icon, bg->getContentSize().width);
    auto nameText = CCLabelIF::create(CCCommonUtils::getNameById(id).c_str());
    nameText->setColor(ccc3(46, 23, 0));
    nameText->setFontSize(22);
    if (CCCommonUtils::isIosAndroidPad()) {
        nameText->setFontSize(45);
    }
    this->addChild(nameText);
    nameText->setAnchorPoint(ccp(0, 0.5));
    nameText->setPosition(bg->getContentSize().width + bg->getPositionX() + 10, bg->getPositionY() - bg->getContentSize().height / 2);
    m_h = bg->getContentSize().height;
    return true;
}

int AbilityCell::getH(){
    return m_h;
}

//--

void TitleCell::setBGHeight(int h){
    if(h == 0){
        h = 50;
        this->m_decoration->setVisible(false);
    }else{
        h = h + 95;
    }
    m_bg->setContentSize(CCSize(m_bg->getContentSize().width, h));
    m_decoration->setContentSize(CCSize(m_decoration->getContentSize().width, h - 64));
}

int TitleCell::getBGHeight(){
    return m_bg->getContentSize().height;
}

bool TitleCell::init(std::string title1, std::string title2){
    CCBLoadFile("EnemyInfoDetailCell", this, this);
    this->m_title1->setString(title1);
    this->m_title2->setString(title2);
    return true;
}

bool TitleCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title1", CCLabelIF*, m_title1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title2", CCLabelIF*, m_title2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_decoration", CCScale9Sprite*, m_decoration);

    return false;
}

void TitleCell::hideBG(){
    this->m_bg->setVisible(false);
}

void TitleCell::hideDecoration(){
    this->m_decoration->setVisible(false);
}