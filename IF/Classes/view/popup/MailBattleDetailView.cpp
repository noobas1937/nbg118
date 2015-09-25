//
//  MailBattleDetailView.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-9.
//
//

#include "MailBattleDetailView.h"
#include "DetectMailPopUpView.h"

MailBattleDetailView* MailBattleDetailView::create(MailInfo *info){
    MailBattleDetailView* ret = new MailBattleDetailView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool MailBattleDetailView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_data = CCArray::create();
    auto bg = CCBLoadFile("MailBattleDetailView",this,this);
    CCSize size = bg->getContentSize();
    setContentSize(size);
    CCLoadSprite::doResourceByCommonIndex(105, true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(105, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    this->m_upTitle->setString(_lang("114114"));
    m_tabView = CCTableView::create(this, m_infoContainer->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    //m_tabView->setTouchPriority(2);
    m_infoContainer->addChild(m_tabView);
    m_tabView->setPositionX(-6);
    m_btnClose->setVisible(false);
    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
        // detect touch inside panel
        if (!isTouchInside(m_infoContainer, pTouch)) {
            this->btnClose(NULL,Control::EventType::TOUCH_DOWN);
        }
    });
    generateData();
    m_tabView->reloadData();
    return true;
}
void MailBattleDetailView::generateData(){
    m_data->removeAllObjects();
    CCArray* dictarr = NULL;
    auto armyDic0 = CCDictionary::create();
    armyDic0->setObject(CCString::create("-1"), "type");
    m_data->addObject(armyDic0);
    if(m_info->attReport){
        CCDictionary *armyDicTitle = CCDictionary::create();
         armyDicTitle->setObject(CCString::create("0"), "type");
        m_data->addObject(armyDicTitle);
        
        auto armyDic = CCDictionary::create();
        string name = m_info->attUser->valueForKey("name")->getCString();
        CCString* namec = CCString::create(name);
        armyDic->setObject(namec, "palyerName");
        armyDic->setObject(m_info->attReport, "armList");
        armyDic->setObject(CCString::create("1"), "type");
        
        string tmp1 = armyDic->valueForKey("stype")->getCString();
        string tmp2 = armyDic->valueForKey("palyerName")->getCString();
        auto obj = armyDic->objectForKey("armList");
        CCLOG("test");
        if(m_info->atkGenKill&&GlobalData::shared()->playerInfo.gmFlag==1){
            armyDic->setObject(m_info->atkGenKill, "mainGenKill");
        }
        m_data->addObject(armyDic);
        
    }
    dictarr = m_info->atkHelpReport;
    if(dictarr!=NULL){
        CCObject* obj;
        CCARRAY_FOREACH(dictarr, obj)
        {
            CCDictionary* item = _dict(obj);
//            string key = pElement->getStrKey();
            string name = item->valueForKey("name")->getCString();
            CCArray* dictobj = (CCArray*)item->objectForKey("armyInfo");
            CCDictionary *armyDic = CCDictionary::create();
            armyDic->setObject(dictobj, "armList");
            armyDic->setObject(CCString::create(name), "palyerName");
            armyDic->setObject(CCString::create("1"), "type");
            
            if(item->objectForKey("genKill")&&GlobalData::shared()->playerInfo.gmFlag==1){
                armyDic->setObject((CCArray*)item->objectForKey("genKill"), "mainGenKill");
            }
            m_data->addObject(armyDic);
        }
    }

    if(m_info->defReport){
        CCDictionary *armyDicTitle = CCDictionary::create();
        armyDicTitle->setObject(CCString::create("2"), "type");
        m_data->addObject(armyDicTitle);
        
        CCDictionary *armyDic = CCDictionary::create();
        string name = m_info->defUser->valueForKey("name")->getCString();
        CCString* namec = CCString::create(name);
        armyDic->setObject(namec, "palyerName");
        armyDic->setObject(m_info->defReport, "armList");
        armyDic->setObject(CCString::create("3"), "type");
        if(m_info->defTowerKill){
            armyDic->setObject(m_info->defTowerKill, "defTowerKill");
        }
        if(m_info->defFortLost){
            armyDic->setObject(m_info->defFortLost, "defFortLost");
        }
        CCLOG("test");
        if(m_info->defGenKill&&GlobalData::shared()->playerInfo.gmFlag==1){
            armyDic->setObject(m_info->defGenKill, "mainGenKill");
        }
        m_data->addObject(armyDic);
    }
    
    dictarr = m_info->defHelpReport;
    if(dictarr!=NULL){
        CCObject* obj;
        CCARRAY_FOREACH(dictarr, obj){
            CCDictionary* item = _dict(obj);

            string name = item->valueForKey("name")->getCString();
            CCArray* dictobj = (CCArray*)item->objectForKey("armyInfo");
            CCDictionary *armyDic = CCDictionary::create();
            armyDic->setObject(dictobj, "armList");
            armyDic->setObject(CCString::create(name), "palyerName");
            armyDic->setObject(CCString::create("3"), "type");
            
            if(item->objectForKey("genKill")&&GlobalData::shared()->playerInfo.gmFlag==1){
                armyDic->setObject((CCArray*)item->objectForKey("genKill"), "mainGenKill");
            }
            m_data->addObject(armyDic);
        }
    }
    if(m_info->isShowDefEndTxt){//战斗提前结束
        CCDictionary *armyDicEnd = CCDictionary::create();
        armyDicEnd->setObject(CCString::create("4"), "type");
        m_data->addObject(armyDicEnd);
    }
    
}

CCSize MailBattleDetailView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (idx<m_data->count()) {
        CCDictionary* info = (CCDictionary*)m_data->objectAtIndex(idx);
        string type = info->valueForKey("type")->getCString();
        if(type == "-1"){
            return CCSizeMake(610, 120);
        }else
        if (type == "0" || type == "2") {
            return CCSizeMake(610, 100);
        }
        if (type == "4"){
            return CCSizeMake(610, 130);
        }
        else {
            auto armList = dynamic_cast<CCArray*>(info->objectForKey("armList"));
            auto defFortLost = dynamic_cast<CCArray*>(info->objectForKey("defFortLost"));
            auto defTowerKill = dynamic_cast<CCArray*>(info->objectForKey("defTowerKill"));
            auto mainGeneral = dynamic_cast<CCArray*>(info->objectForKey("mainGenKill"));
            int count = 0;
            if (armList && armList->count()>0) {
                count+=armList->count();
            }
            if (defFortLost) {
                count+=defFortLost->count();
            }
            if(defTowerKill){
                count+=defTowerKill->count();
            }
            if(mainGeneral&&GlobalData::shared()->playerInfo.gmFlag==1){
                count+= 1;
            }

            if (count>0) {
                return CCSizeMake(610, (count*120)+80);
            }
            else {
                return CCSizeMake(610, 200);
            }
        }
    }
    return CCSizeMake(610, 0);
}
CCSize MailBattleDetailView::cellSizeForTable(CCTableView *table)
{
    return CCSize(610, 500);
}


CCTableViewCell* MailBattleDetailView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    //CCTableViewCell AchievementNewPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    MailPlayerCell* cell = (MailPlayerCell*)table->dequeueCell();
    CCDictionary* info = (CCDictionary*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = MailPlayerCell::create(info);
           // m_cells->addObject(cell);
        }
    }
    return cell;
    return NULL;
}
ssize_t MailBattleDetailView::numberOfCellsInTableView(CCTableView *table){

    int count = m_data->count();
    return count;
}
void MailBattleDetailView::btnClose(CCObject *pSender, CCControlEvent event){
    this->closeSelf();
}

void MailBattleDetailView::onEnter(){
    CCNode::onEnter();
}

void MailBattleDetailView::onExit(){
    CCNode::onExit();
}

SEL_CCControlHandler MailBattleDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "btnClose", MailBattleDetailView::btnClose);
    return NULL;
}

bool MailBattleDetailView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoContainer", CCNode*, this->m_infoContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnClose", CCControlButton*, m_btnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_upTitle", CCLabelIF*, m_upTitle);
    return false;
}


MailPlayerCell* MailPlayerCell::create(CCDictionary *dict,bool attack,bool isTitle)
{
    auto ret = new MailPlayerCell(dict,attack,isTitle);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MailPlayerCell::onEnter() {
    CCNode::onEnter();
}

void MailPlayerCell::onExit() {
    CCNode::onExit();
}
bool MailPlayerCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_hurtTxt", CCLabelIF*, this->m_hurtTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_killTxt", CCLabelIF*, this->m_killTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_playerNameTxt", CCLabelIF*, this->m_playerNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_lostTxt", CCLabelIF*, this->m_lostTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTitleText", CCLabelIF*, this->m_tipTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cellNode", CCNode*, this->m_cellNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cellTitleNode", CCNode*, this->m_cellTitleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleNode", CCNode*, this->m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kuangBG", CCScale9Sprite*, this->m_kuangBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dftFlag", CCScale9Sprite*, this->m_dftFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_atkFlag", CCScale9Sprite*, this->m_atkFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipNode", CCNode*, this->m_tipNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_endTxt", CCLabelIF*, this->m_endTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_endNode", CCNode*, this->m_endNode);

    return false;
}

SEL_CCControlHandler MailPlayerCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){

    return NULL;
}
float MailPlayerCell::getdh(){
//    if(m_dict==NULL){
//        return 200;
//    }
//    string type = m_dict->valueForKey("type")->getCString();
//    if(type=="0"||type=="2"){
//        return 37;
//    }
    return m_sumH;
}
void MailPlayerCell::setData(CCDictionary *info, bool attack,bool isTitle){
    m_listNode->removeAllChildren();
    this->m_kuangBG->setContentSize(CCSize(this->m_kuangBG->getContentSize().width, 200));
    this->m_bg->setContentSize(CCSize(this->m_bg->getContentSize().width, 200));
    this->m_titleNode->setPositionY(0);
    
    m_dict = info;
    m_sumH = 0;
    string type = m_dict->valueForKey("type")->getCString();
    m_cellTitleNode->setVisible(false);
    m_cellNode->setVisible(false);
    this->m_tipNode->setVisible(false);
    this->m_dftFlag->setVisible(false);
    this->m_atkFlag->setVisible(false);
    m_endNode->setVisible(false);
    m_titleText->setColor({0,26,89});
    if(type=="-1"){
         this->m_tipNode->setVisible(true);
        //  this->m_nameTxt->setString(_lang("105557"));
        this->m_numTxt->setString(_lang("114113"));
        this->m_lostTxt->setString(_lang("105544"));
        this->m_killTxt->setString(_lang("105543"));
        this->m_hurtTxt->setString(_lang("105545"));
        this->m_tipTitleText->setString(_lang("114123"));
        return;
    }else if(type=="0"){
        m_cellTitleNode->setVisible(true);
        m_titleText->setString(_lang("105559"));
        m_titleText->setColor({169,21,0});
        return;
    }
    else if(type=="1"){
        this->m_atkFlag->setVisible(true);
        m_cellNode->setVisible(true);
      //  this->m_nameTxt->setString(_lang("105557"));
//        this->m_numTxt->setString(_lang("114113"));
//        this->m_lostTxt->setString(_lang("105544"));
//        this->m_killTxt->setString(_lang("105545"));
//        this->m_hurtTxt->setString(_lang("105543"));
    }
    else if(type=="2"){
        m_cellTitleNode->setVisible(true);
        m_titleText->setString(_lang("105560"));
        return;
    }
    else if(type=="3"){
        this->m_dftFlag->setVisible(true);
        m_cellNode->setVisible(true);
       // this->m_nameTxt->setString(_lang("105557"));
//        this->m_numTxt->setString(_lang("114113"));
//        this->m_lostTxt->setString(_lang("105544"));
//        this->m_killTxt->setString(_lang("105545"));
//        this->m_hurtTxt->setString(_lang("105543"));
    }
    else if(type=="4"){
        m_endNode->setVisible(true);
        m_endTxt->setString(_lang("105151"));//_lang("105151")
        return;
    }
    string playname = m_dict->valueForKey("palyerName")->getCString();
    m_playerNameTxt->setString(playname);


    int curY = 0;
    int curX = 0;
    int itemH = 120;
    //3.箭塔
    CCArray *defTowerKillArr = dynamic_cast<CCArray*>(m_dict->objectForKey("defTowerKill"));
    CCObject *defTowerKillObject;
    CCARRAY_FOREACH(defTowerKillArr, defTowerKillObject){
        CCDictionary *armyDic = _dict(defTowerKillObject);
        int level = armyDic->valueForKey("level")->intValue();
        std::string armName = _lang("102016")+"Lv"+CC_CMDITOA(level);
        int kill = armyDic->valueForKey("kill")->intValue();
        DetailBattleCellCCB* cell = DetailBattleCellCCB::create(armName,"~","~",CC_CMDITOA(kill),"~","battle_tower.png");
        m_listNode->addChild(cell);
        cell->setPosition(ccp(curX, curY));
        curY += itemH;
    }
    //2.陷阱
    CCArray *defFortLostArr = dynamic_cast<CCArray*>(m_dict->objectForKey("defFortLost"));
    CCObject *defFortLostObject;
    CCARRAY_FOREACH(defFortLostArr, defFortLostObject){
        CCDictionary *armyDic = _dict(defFortLostObject);
        std::string arm = CCCommonUtils::getPropById(armyDic->valueForKey("armId")->getCString(), "arm");
        std::string armyId = armyDic->valueForKey("armId")->getCString();
        std::string armName = CCCommonUtils::getNameById(armyId);
        std::string icon = "ico" + armyId+ ".png";// CCCommonUtils::getIcon(armyId);
        int total =armyDic->valueForKey("num")->intValue();
        int dead =armyDic->valueForKey("dead")->intValue();
        int kill = armyDic->valueForKey("kill")->intValue();
        
        DetailBattleCellCCB* cell = DetailBattleCellCCB::create(armName,CC_CMDITOA(total),CC_CMDITOA(dead),CC_CMDITOA(kill),"~",icon);
        m_listNode->addChild(cell);
        cell->setPosition(ccp(curX, curY));
        curY += itemH;
    }

    //1. army
    CCArray *armyArr = dynamic_cast<CCArray*>(m_dict->objectForKey("armList"));

    CCArray* m_tmpArray = CCArray::create();
    CCObject *armysortObject;
    CCARRAY_FOREACH(armyArr, armysortObject){
        std::string armyId = _dict(armysortObject)->valueForKey("armId")->getCString();
        int index = 0;
        bool addFlag = false;
        while (index < m_tmpArray->count()) {
            std::string armyId1 = _dict(m_tmpArray->objectAtIndex(index))->valueForKey("armId")->getCString();
            int level1 = atoi(armyId1.c_str()) % 100;
            int level = atoi(armyId.c_str()) % 100;
            if(level1 < level){
                m_tmpArray->insertObject(armysortObject, index);
                addFlag = true;
                break;
            }
            index++;
        }
        if(!addFlag){
            m_tmpArray->addObject(armysortObject);
        }
    }
    CCObject *armyObject;
    CCARRAY_FOREACH_REVERSE(m_tmpArray, armyObject){
        CCDictionary *armyDic = _dict(armyObject);
        std::string arm = CCCommonUtils::getPropById(armyDic->valueForKey("armId")->getCString(), "arm");
        std::string armyId = armyDic->valueForKey("armId")->getCString();
        std::string armName = CCCommonUtils::getNameById(armyId);
        //int total = armyDic->valueForKey("dead")->intValue() + armyDic->valueForKey("num")->intValue() + armyDic->valueForKey("hurt")->intValue();
        int total =armyDic->valueForKey("num")->intValue();
        int dead =armyDic->valueForKey("dead")->intValue();
        int kill = armyDic->valueForKey("kill")->intValue();
        int hurt = armyDic->valueForKey("hurt")->intValue();
        
        std::string icon = GlobalData::shared()->armyList[armyId].getHeadIcon();

        string path = armyId.substr(armyId.size()-2);
        int num  = atoi(path.c_str())+1;
        path = "Roman_";
        path.append(CC_ITOA(num));
        path.append(".png");
        DetailBattleCellCCB* cell = DetailBattleCellCCB::create(armName,CC_CMDITOA(total),CC_CMDITOA(dead),CC_CMDITOA(kill),CC_CMDITOA(hurt),icon,path);
        m_listNode->addChild(cell);
        cell->setPosition(ccp(curX, curY));
        curY += itemH;
    }
    //2.陷阱
    CCArray *atkGenKilltArr = dynamic_cast<CCArray*>(m_dict->objectForKey("mainGenKill"));
    CCObject *atkGenKilltobj;
    if(atkGenKilltArr){
        int total = 0;
        CCARRAY_FOREACH(atkGenKilltArr, atkGenKilltobj){
           total+=dynamic_cast<CCString*>(atkGenKilltobj)->intValue();
        }
        DetailBattleCellCCB* cell = DetailBattleCellCCB::create(_lang("3000004"),"~","~",CC_CMDITOA(total),"~","");
        m_listNode->addChild(cell);
        cell->setPosition(ccp(curX, curY));
        curY += itemH;
    }
    curY -= itemH;
    m_sumH = curY;
    this->m_kuangBG->setContentSize(CCSize(this->m_kuangBG->getContentSize().width, m_sumH+200));
    this->m_bg->setContentSize(CCSize(this->m_bg->getContentSize().width, m_sumH+200));
    this->m_titleNode->setPositionY(this->m_titleNode->getPositionY()+curY);
    CCLOG("%d",m_sumH+130);

}
bool MailPlayerCell::init()
{
    auto bg = CCBLoadFile("MailBattleResultView", this, this);
    this->setContentSize(bg->getContentSize());
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_playerNameTxt->setScale(0.5);
        m_playerNameTxt->setFontSize(m_playerNameTxt->getFontSize() * 2);
        m_titleText->setScale(0.5);
        m_titleText->setFontSize(m_titleText->getFontSize() * 2);
        m_numTxt->setScale(0.5);
        m_numTxt->setFontSize(m_numTxt->getFontSize() * 2);
        m_killTxt->setScale(0.5);
        m_killTxt->setFontSize(m_killTxt->getFontSize() * 2);
        m_hurtTxt->setScale(0.5);
        m_hurtTxt->setFontSize(m_hurtTxt->getFontSize() * 2);
        m_lostTxt->setScale(0.5);
        m_lostTxt->setFontSize(m_lostTxt->getFontSize() * 2);
        m_tipTitleText->setScale(0.5);
        m_tipTitleText->setFontSize(m_tipTitleText->getFontSize() * 2);
        m_endTxt->setScale(0.5);
        m_endTxt->setFontSize(m_endTxt->getFontSize() * 2);
    }
    setData(m_dict);
    return true;
}

GeneralItemCell* GeneralItemCell::create(GeneralInfo *info,int kill)
{
    auto ret = new GeneralItemCell(info,kill);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void GeneralItemCell::onEnter() {
    CCNode::onEnter();
}

void GeneralItemCell::onExit() {
    CCNode::onExit();
}

bool GeneralItemCell::init()
{
    //setContentSize(CCSizeMake(640, 852));
    int index = GeneralManager::getInstance()->getBustPlistIndex(m_info->generalId);
    CCLoadSprite::doLoadResourceAsync(GENERAL_PATH, CCCallFuncO::create(this, callfuncO_selector(GeneralItemCell::handleAsyRes), NULL), index);
    setCleanFunction([index](){
        CCLoadSprite::doResourceByGeneralIndex(index, false);
    });
    CCLabelIF* lable = CCLabelIF::create();
    lable->setAnchorPoint(ccp(0.5, 0.5));
    lable->setPosition(ccp(0,0));
    lable->setColor(ccRED);
    lable->setFontSize(22);
    lable->setString(m_info->getName().c_str());
    this->addChild(lable);
    
    std::string killStr = "kill:";
    killStr.append(CC_ITOA(m_kill));
    lable = CCLabelIF::create();
    lable->setAnchorPoint(ccp(0.5, 0.5));
    lable->setPosition(ccp(0,-220));
    lable->setColor(ccRED);
    lable->setFontSize(22);
    lable->setString(killStr.c_str());
    this->addChild(lable);
    return true;
}

void GeneralItemCell::handleAsyRes(CCObject* p){
    std::string  path = CCCommonUtils::getGeneralBustPicById(m_info->generalId);
    auto pic = CCLoadSprite::createSprite(path.c_str());
    pic->setAnchorPoint(ccp(0.5, 1));
    CCCommonUtils::setSpriteMaxSize(pic, 200);
    this->addChild(pic,-1);
}

//----
DetailBattleCellCCB *DetailBattleCellCCB::create(std::string name,std::string num,std::string lost,std::string kill,std::string hurt,string icon,string icon1){
    DetailBattleCellCCB *ret = new DetailBattleCellCCB(name,num,lost,kill,hurt,icon,icon1);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DetailBattleCellCCB::init(){
    std::string ccbStr = false?"DetailTitleCell":"DetailArmyCell";
    auto node = CCBLoadFile(ccbStr.c_str(), this, this);
    this->setContentSize(node->getContentSize());
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_numTxt->setScale(0.5);
        m_numTxt->setFontSize(m_numTxt->getFontSize() * 2);
        m_killTxt->setScale(0.5);
        m_killTxt->setFontSize(m_killTxt->getFontSize() * 2);
        m_hurtTxt->setScale(0.5);
        m_hurtTxt->setFontSize(m_hurtTxt->getFontSize() * 2);
        m_lostTxt->setScale(0.5);
        m_lostTxt->setFontSize(m_lostTxt->getFontSize() * 2);
        m_nameTxt->setScale(0.5);
        m_nameTxt->setFontSize(m_nameTxt * 2.0);
    }
    m_nameTxt->setString(m_nameStr);
    m_numTxt->setString(m_numStr);
    m_lostTxt->setString(m_lostStr);
    m_killTxt->setString(m_killStr);
    m_hurtTxt->setString(m_hurtStr);
    if(m_icon1!=""){
        auto spr = CCLoadSprite::createSprite(m_icon1.c_str());
        m_levelSprNode->addChild(spr);
        m_levelSprNode->setVisible(true);
        m_nameTxt->setString("");
    }else{
        m_levelSprNode->setVisible(false);
    }
    if(m_icon!=""){
        auto spr = CCLoadSprite::createSprite(m_icon.c_str());
        m_picHead->addChild(spr);
        m_picHead->setVisible(true);
       CCCommonUtils::setSpriteMaxSize(spr, 90);
    }
//    m_nameTxt->setFntFile("Arial_Bold_Regular.fnt");
//    m_numTxt->setFntFile("Arial_Bold_Regular.fnt");
//    m_lostTxt->setFntFile("Arial_Bold_Regular.fnt");
//    m_killTxt->setFntFile("Arial_Bold_Regular.fnt");
//    m_hurtTxt->setFntFile("Arial_Bold_Regular.fnt");
    return true;
}

bool DetailBattleCellCCB::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lostTxt", CCLabelIF*, this->m_lostTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killTxt", CCLabelIF*, this->m_killTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hurtTxt", CCLabelIF*, this->m_hurtTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelSprNode", CCNode*, this->m_levelSprNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picHead", CCNode*, this->m_picHead);
    return false;
}