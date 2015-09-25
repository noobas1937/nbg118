//
//  EquipmentController.cpp
//  IF
//
//  Created by fubin on 14-12-2.
//
//

#include "EquipmentController.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "YesNoDialog.h"
#include "QueueController.h"
#include "ToolController.h"
#include "EquipmentCommand.h"
#include "FunBuildController.h"
#include "EquipFinishView.h"
#include "EquipSiteView.h"
#include "EquipmentListView.h"
#include "EquipmentCreateView.h"
#include "EquipNewUseView.h"
#include "FunBuild.h"

static EquipmentController *_instance = NULL;

EquipmentController::EquipmentController()
{
    m_callBackFunc = NULL;
    auto allDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("equipment");
    if(allDictInfo)
    {
        CCDictElement* element;
        CCDICT_FOREACH(allDictInfo, element)
        {
            string strId = element->getStrKey();
            CCDictionary* dictInfo = _dict(element->getObject());
            int id = atoi(strId.c_str());
            EquipmentInfoMap[id] = EquipmentInfo(dictInfo);
            if (EquipmentInfoMap[id].color == 5) {
                SelEquipList.push_back(id);
            }
        }
    }
    
    CCLOG("init equipment xml");
}

EquipmentController::~EquipmentController() {}

EquipmentController* EquipmentController::getInstance() {
    if (!_instance) {
        _instance = new EquipmentController();
    }
    return _instance;
}

void EquipmentController::purgeData() {
    delete _instance;
    _instance = NULL;
}

//后台返回初始化数据
void EquipmentController::retInitData(CCDictionary* dict)
{
    m_MyOnEquip.clear();
    m_MyEquipmentMap.clear();
    EquipEffMap.clear();
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("equip"));
    if( arr ){
        CCDictionary *item = NULL;
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            string uuid = item->valueForKey("uuid")->getCString();
            int itemId = item->valueForKey("itemId")->intValue();
            int on = item->valueForKey("on")->intValue();
            if (on==1) {
                m_MyOnEquip[uuid] = itemId;
            }
            else {
                m_MyEquipmentMap[uuid] = itemId;
            }
//            m_MyEquipmentMap[uuid] = itemId;
            if (EquipmentInfoMap[itemId].site==6) {
                LongEquipmentInfoMap[uuid]=EquipmentInfoMap[itemId];
                EquipmentInfo &equipInfo = LongEquipmentInfoMap[uuid];
                string extraProperty = item->valueForKey("extraProperty")->getCString();
                equipInfo.setLongProperty(extraProperty);
            }
        }
        MakeEffectValue();
    }
}

void EquipmentController::addEquip(CCArray* equipArr)//添加装备
{
    if (equipArr)
    {
        CCDictionary *item = NULL;
        for (int i=0; i<equipArr->count(); i++)
        {
            item = _dict(equipArr->objectAtIndex(i));
            string uuid = item->valueForKey("uuid")->getCString();
            int itemId = item->valueForKey("itemId")->intValue();
            m_MyEquipmentMap[uuid] = itemId;
        }
    }
}

//合成装备
bool EquipmentController::startCreateEquipment(string uuid, string qItemId, vector<int> toolItemIds)
{
    m_curOptMate = toolItemIds;
    m_curOptEquip = uuid;
    EquipmentCreateCommand* cmd = new EquipmentCreateCommand(uuid, qItemId, toolItemIds);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retCreateEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (m_MyEquipmentMap.find(m_curOptEquip) != m_MyEquipmentMap.end()) {
            m_MyEquipmentMap.erase(m_curOptEquip);
        }
        for (int i=0; i<m_curOptMate.size(); i++) {
            auto& tool = ToolController::getInstance()->getToolInfoById(m_curOptMate[i]);
            int count =MAX(tool.getCNT() - 1, 0) ;
            tool.setCNT(count);
        }
        
        if (dict->objectForKey("queue")) {
            auto queue = _dict(dict->objectForKey("queue"));
            string key = QueueController::getInstance()->addQueueInfo(queue);
        }
        if (dict->objectForKey("resource")) {
            auto resInfo = _dict(dict->objectForKey("resource"));
            GlobalData::shared()->resourceInfo.setResourceData(resInfo);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_CREATE_END);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("showEquipTips");
    }
    m_curOptEquip = "";
    m_curOptMate.clear();
}

void EquipmentController::startFinishCrtEquip(string quuid)
{
    HavestEquipCommand * cmd = new HavestEquipCommand(quuid);
    cmd->sendAndRelease();
}

void EquipmentController::retFinishCrtEquip(CCDictionary* dict)
{
    if (dict->objectForKey("uuid") && dict->objectForKey("itemId")) {
        string uuid = dict->valueForKey("uuid")->getCString();
        int itemId = dict->valueForKey("itemId")->intValue();
        m_MyEquipmentMap[uuid] = itemId;
        
        int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_FORGE);
        if (qid != QID_MAX) {
            int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
            QueueController::getInstance()->startFinishQueue(qid, false);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_STATE, CCInteger::create(bid));
        }
        
        auto equipView = EquipFinishView::create(itemId);
        if (PopupViewController::getInstance()->getCurrViewCount()==0) {
            PopupViewController::getInstance()->addPopupView(equipView);
        }
        else {//有其他面板打开时 不显示升级面板
            PopupBaseView* curView = PopupViewController::getInstance()->getCurrentPopupView();
            EquipSiteView* siteView = dynamic_cast<EquipSiteView*>(curView);
            EquipmentListView* listView = dynamic_cast<EquipmentListView*>(curView);
            EquipmentCreateView* createView = dynamic_cast<EquipmentCreateView*>(curView);
            EquipNewUseView* newUseView = dynamic_cast<EquipNewUseView*>(curView);
            if (siteView || listView || createView || newUseView) {
                if (newUseView) {
                    equipView->setCurPopType( newUseView->m_weapType );
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_CLOSE_VIEW);
                }
                PopupViewController::getInstance()->addPopupView(equipView);
            }
            else {
                PopupViewController::getInstance()->pushPop(equipView);
            }
        }
    }
}

//分解装备
bool EquipmentController::startDesEquipment(string uuid)
{
    m_curOptEquip = uuid;
    EquipmentCommand* cmd = new EquipmentCommand(uuid, EQU_DES_COMMAND);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retDesEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        CCLOG("destroy equipment end");
        if (m_MyEquipmentMap.find(m_curOptEquip) != m_MyEquipmentMap.end()) {
            m_MyEquipmentMap.erase(m_curOptEquip);
        }
        
        string mateItems = "";
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("goods"));
        CCDictionary* item = NULL;
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            int itemId = item->valueForKey("itemId")->intValue();
            int num = item->valueForKey("count")->intValue();
            string uuid = item->valueForKey("uuid")->getCString();
            auto &info = ToolController::getInstance()->getToolInfoById(itemId);
            int currentNum = info.getCNT();
            ToolController::getInstance()->addTool(itemId, num, uuid);
            
            mateItems = mateItems+CC_ITOA(itemId)+"|"+CC_ITOA(num - currentNum);
            if (i <= arr->count()-1) {
                mateItems = mateItems+";";
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_BAG_SHOW_DES, CCString::create(mateItems));
    }
    m_curOptEquip = "";
}
//卖龙装备
bool EquipmentController::startSellLongEquipment(string uuid)
{
    m_curOptEquip = uuid;
    EquipmentCommand* cmd = new EquipmentCommand(uuid, EQU_SELL_LONG_COMMAND);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retSellLongEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        CCLOG("destroy equipment end");
        if (m_MyEquipmentMap.find(m_curOptEquip) != m_MyEquipmentMap.end()) {
            m_MyEquipmentMap.erase(m_curOptEquip);
        }
        
        if(dict->valueForKey("crystal")){
            int longJing = dict->valueForKey("crystal")->intValue();
            UIComponent::getInstance()->updateLongJing(longJing);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_BAG_SHOW_DES, NULL);
    }
    m_curOptEquip = "";
}

//穿戴装备
bool EquipmentController::startPutOnEquipment(string uuid, string oldUuid)
{
    int itemId = m_MyEquipmentMap[uuid];
    auto& eInfo = EquipmentInfoMap[itemId];
    if (eInfo.level > GlobalData::shared()->playerInfo.level) {
        CCCommonUtils::flyHint("", "", _lang("119040"));
        return false;
    }
    
    m_curOptEquip = uuid;
    m_oldEquip = oldUuid;
    EquipmentCommand* cmd = new EquipmentCommand(uuid, EQU_PUTON_COMMAND);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retPutOnEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (m_curOptEquip!="" && m_MyEquipmentMap.find(m_curOptEquip)!=m_MyEquipmentMap.end())
        {
            string oldEquip = "";
            string curEquip = m_curOptEquip;
            if (m_oldEquip!="" && m_MyOnEquip.find(m_oldEquip)!=m_MyOnEquip.end()) {//之前穿的装备 换到仓库里
                m_MyEquipmentMap[m_oldEquip] = m_MyOnEquip[m_oldEquip];
                m_MyOnEquip.erase(m_oldEquip);
                oldEquip = m_oldEquip;
            }
            
            m_MyOnEquip[m_curOptEquip] = m_MyEquipmentMap[m_curOptEquip];
            m_MyEquipmentMap.erase(m_curOptEquip);
            MakeEffectValue();
            showPowerChangeEff(oldEquip, curEquip);
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_PUTON_MSG, CCString::create(m_curOptEquip));
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_CLOSE_VIEW);
    m_curOptEquip = "";
    m_oldEquip = "";
}

//卸下装备
bool EquipmentController::startTakeOffEquipment(string uuid)
{
    m_curOptEquip = uuid;
    EquipmentCommand* cmd = new EquipmentCommand(uuid, EQU_TAKEOFF_COMMAND);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retTakeOffEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (m_curOptEquip!="" && m_MyOnEquip.find(m_curOptEquip)!=m_MyOnEquip.end()) {
            string oldEquip = m_curOptEquip;
            string curEquip = "";
            m_MyEquipmentMap[m_curOptEquip] = m_MyOnEquip[m_curOptEquip];
            m_MyOnEquip.erase(m_curOptEquip);
            MakeEffectValue();
            showPowerChangeEff(oldEquip, curEquip);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_TAKEOFF_MSG, CCString::create(m_curOptEquip));
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_CLOSE_VIEW);
    m_curOptEquip = "";
}

bool EquipmentController::startMaterialCreate(int toolId,int type)
{
    MaterialCreateCommand* cmd = new MaterialCreateCommand(CC_ITOA(toolId),type);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retMaterialCreate(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (dict->objectForKey("dec")) {//消耗掉的道具信息
            auto decDic = _dict(dict->objectForKey("dec"));
            ToolController::getInstance()->pushAddTool(decDic);
        }
        if (dict->objectForKey("add")) {//新增的道具信息
            auto addDic = _dict(dict->objectForKey("add"));
            ToolController::getInstance()->pushAddTool(addDic);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MATE_CRT_END);
    }
}

bool compareSelMember(int item1, int item2)
{//先按等级排序， 再按部位排序
    auto& info1 = EquipmentController::getInstance()->EquipmentInfoMap[item1];
    auto& info2 = EquipmentController::getInstance()->EquipmentInfoMap[item2];
    if (info1.level > info2.level) {
        return true;
    }
    else if (info1.level < info2.level) {
        return false;
    }
    else {
        if (info1.site > info2.site) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool compareTakeMember(string item1, string item2)
{//先按是否是胚子排序，再按等级排序， 再按品质排序
    auto& info1 = EquipmentController::getInstance()->EquipmentInfoMap[ EquipmentController::getInstance()->m_MyEquipmentMap[item1] ];
    auto& info2 = EquipmentController::getInstance()->EquipmentInfoMap[ EquipmentController::getInstance()->m_MyEquipmentMap[item2] ];
    
    if(info1.showType != "no" && info2.showType == "no")
    {
        return true;
    }
    else if(info1.showType == "no" && info2.showType != "no")
    {
        return false;
    }
    else if (info1.level > info2.level) {
        return true;
    }
    else if (info1.level < info2.level) {
        return false;
    }
    else {
        if (info1.color > info2.color) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool compareEquipMember(string item1, string item2)
{//先按等级排序 从小到大， 再按品质排序从小到大
    auto& info1 = EquipmentController::getInstance()->EquipmentInfoMap[ EquipmentController::getInstance()->m_MyEquipmentMap[item1] ];
    auto& info2 = EquipmentController::getInstance()->EquipmentInfoMap[ EquipmentController::getInstance()->m_MyEquipmentMap[item2] ];
    if(info1.showType == "no" && info2.showType != "no")
    {
        return true;
    }
    else if(info1.showType != "no" && info2.showType == "no")
    {
        return false;
    }
    else if (info1.level < info2.level) {
        return true;
    }
    else if (info1.level > info2.level) {
        return false;
    }
    else {
        if (info1.color < info2.color) {
            return true;
        }
        else {
            return false;
        }
    }
}

//按部件 和 材料 查找装备
vector<int> EquipmentController::SelEquipmentsByType(int site, int mate)
{
    vector<int> ret;
    if (site!=-1) {
        for (int i=0; i<SelEquipList.size(); i++) {
            if (EquipmentInfoMap[SelEquipList[i]].site == site) {
                ret.push_back(SelEquipList[i]);
            }
        }
    }
    
    if (site==-1 && mate!=-1) {
        for (int i=0; i<SelEquipList.size(); i++) {
            auto& info = EquipmentInfoMap[SelEquipList[i]];
            if (info.mateMap.find(mate) != info.mateMap.end()) {
                ret.push_back(SelEquipList[i]);
            }
        }
    }
    else {
        if (mate!=-1) {
            vector<int> lastRet;
            for (int i=0; i<ret.size(); i++) {
                auto& info = EquipmentInfoMap[ret[i]];
                if (info.mateMap.find(mate) != info.mateMap.end()) {
                    lastRet.push_back(ret[i]);
                }
            }
            sort(lastRet.begin(), lastRet.end(), compareSelMember);
            return lastRet;
        }
    }
    sort(ret.begin(), ret.end(), compareSelMember);
    return ret;
}

//按部件查找 现有 装备
vector<string> EquipmentController::FindEquipmentsByType(int site)
{
    vector<string> ret;
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        int itemId = it->second;
        if (EquipmentInfoMap.find(itemId) != EquipmentInfoMap.end()) {
            if (EquipmentInfoMap[itemId].site == site) {
                ret.push_back(it->first);
            }
        }
    }
    sort(ret.begin(), ret.end(), compareTakeMember);
    return ret;
}

bool EquipmentController::IsExsitRealEquipBySite(int site)
{
    bool ret = false;
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        int itemId = it->second;
        if (EquipmentInfoMap.find(itemId) != EquipmentInfoMap.end()) {
            if (EquipmentInfoMap[itemId].site == site && EquipmentInfoMap[itemId].showType != "no") {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

bool EquipmentController::IsExsitEquipBySite(int site)
{
    bool ret = false;
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        int itemId = it->second;
        if (EquipmentInfoMap.find(itemId) != EquipmentInfoMap.end()) {
            if (EquipmentInfoMap[itemId].site == site) {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

//按部件查找 现有 装备
vector<string> EquipmentController::FindEquipmentsByLv(int needLv1, int needLv2)
{
    vector<string> ret;
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
        if (tmpInfo.level == needLv1 || tmpInfo.level == needLv2) {
            ret.push_back(it->first);
        }
    }
    sort(ret.begin(), ret.end(), compareEquipMember);
    return ret;
}

//按部件查找 已经穿上的装备
string EquipmentController::FindOnEquipBySite(int site)
{
    string ret = "";
    map<string, int>::iterator it = m_MyOnEquip.begin();
    for (; it!=m_MyOnEquip.end(); it++) {
        auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
        if (tmpInfo.site == site) {
            ret = it->first;
            break;
        }
    }
    return ret;
}

//检查当前背包里是否有该类装备 可以是不同品质
bool EquipmentController::IsHaveEquipByLv(int lv, int selfLv)
{
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
        if (tmpInfo.level == lv || tmpInfo.level == selfLv) {
            return true;
        }
    }
    return false;
}

//检查是否有未使用的装备 且有佩戴位置
bool EquipmentController::IsCanSetEquipBySite(int site)
{
    bool ret = false;
    if (site == -1)
    {
        map<int, int> _havePutOnEquips;//已经穿上装备的部位
        map<string, int>::iterator itPutOn = m_MyOnEquip.begin();
        for (; itPutOn!=m_MyOnEquip.end(); itPutOn++) {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[itPutOn->second];
            _havePutOnEquips[tmpInfo.site] = 1;
        }
        
        map<string, int>::iterator it = m_MyEquipmentMap.begin();
        for (; it!=m_MyEquipmentMap.end(); it++) {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if (_havePutOnEquips.find(tmpInfo.site) == _havePutOnEquips.end()) {
                ret = true;
            }
        }
        
    }
    else {//背包里已经有 该部位的 装备了
        ret = true;
        map<string, int>::iterator it = m_MyOnEquip.begin();
        for (; it!=m_MyOnEquip.end(); it++) {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if (tmpInfo.site == site) {
                ret = false;
                break;
            }
        }
    }
    
    return ret;
}

bool EquipmentController::CheckSelEquipMate(int site, int mate)
{
    bool ret = false;
    map<int, int> tmpToolCntMap;
    vector<int> tmpVec = SelEquipmentsByType(site, mate);
    
//    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
//    auto& buildInfo = FunBuildController::getInstance()->getFunbuildById(buildId);
//    int bdPara2 = atoi(buildInfo.para[2].c_str());
    
    for (int i=0; i<tmpVec.size(); i++) {
        auto& info = EquipmentInfoMap[tmpVec[i]];
        
        if (info.costLv>0) {
            if ( !IsHaveEquipByLv(info.costLv, info.level) ) {
                info.canCrt = false;
                continue;
            }
        }
        
//        int tmp_costRes = 0;
//        vector<string> resCostVec;
//        CCCommonUtils::splitString(info.cost, ";", resCostVec);
//        if (resCostVec.size()==2) {
//            tmp_costRes = atoi(resCostVec[1].c_str());
//            int tmpCostRes = tmp_costRes * bdPara2*1.0/100;
//            tmp_costRes -= tmpCostRes;
//            int resType = atoi(resCostVec[0].c_str());
//            int curNum = CCCommonUtils::getCurResourceByType(resType);
//            if (tmp_costRes > curNum) {
//                info.canCrt = false;
//                continue;
//            }
//        }

        
        bool st = true;
        if(info.mateMap.size()>0){
            for (map<int, int>::iterator it=info.mateMap.begin(); it!=info.mateMap.end(); it++) {
                if (tmpToolCntMap.find(it->first) == tmpToolCntMap.end()) {
                    tmpToolCntMap[it->first] = ToolController::getInstance()->getMateCntByType(it->first);
                }
                
                if (tmpToolCntMap[it->first] < it->second) {
                    st = false;
                    break;
                }
            }
        }
        
        info.canCrt = st;
        if (st) {
            ret = st;
        }
    }
    return ret;
}

vector<int> EquipmentController::CheckEquipSiteMake()
{
    vector<int> ret;
    for (int i=0; i<=7; i++) {
        if( CheckSelEquipMate(i, -1) ) {
            ret.push_back(i);
        }
    }
    return ret;
}

void EquipmentController::retInitConf(CCDictionary* dict)
{
    if (dict->objectForKey("k2")) {//基础值
        string defValue = dict->valueForKey("k2")->getCString();
        vector<string> tmpVec;
        CCCommonUtils::splitString(defValue, "|", tmpVec);
        if (tmpVec.size()==6) {
            for (int i=0; i<tmpVec.size(); i++) {
                m_defPro[i] = atof(tmpVec[i].c_str());
            }
        }
    }
    
    if (dict->objectForKey("k9")) {//衰减系数
        string defValue = dict->valueForKey("k9")->getCString();
        vector<string> tmpVec;
        CCCommonUtils::splitString(defValue, "|", tmpVec);
        if (tmpVec.size()==6) {
            for (int i=0; i<tmpVec.size(); i++) {
                m_colorPro[i] = atof(tmpVec[i].c_str());
            }
        }
    }
    
    if (dict->objectForKey("k10")) {//不同等级的装备 转换为相同品质材料的数量
        string defValue = dict->valueForKey("k10")->getCString();
        vector<string> tmpVec;
        CCCommonUtils::splitString(defValue, "|", tmpVec);
        for (int i=0; i<tmpVec.size(); i++)
        {
            vector<string> cellVec;
            CCCommonUtils::splitString(tmpVec[i], ";", cellVec);
            if (cellVec.size()==2) {
                m_lvToCnt[ atoi(cellVec[0].c_str()) ] = atoi(cellVec[1].c_str());
            }
        }
    }
    
    if (true) {//颜色参数
        for (int i=WHITE; i<=GOLDEN; i++) {
            m_colorWeight[i] = i+1;
        }
    }
}

vector<float> EquipmentController::MakeEquipmentPro(string uuid, string qItemId, vector<int> toolUuids, bool isUUid)
{
    map<int,int> m_colorNum;//颜色对应的编号
    map<int, int> tmpColorMap;//统计材料数量
    tmpColorMap[WHITE] = 0;
    tmpColorMap[GREEN] = 0;
    tmpColorMap[BLUE] = 0;
    tmpColorMap[PURPLE] = 0;
    tmpColorMap[ORANGE] = 0;
    tmpColorMap[GOLDEN] = 0;
    if (uuid != "") {
        if (isUUid && m_MyEquipmentMap.find(uuid) != m_MyEquipmentMap.end() ) {
            int eId = m_MyEquipmentMap[uuid];
            auto& eInfo = EquipmentInfoMap[eId];
            int tmpCnt = 1;
            if (m_lvToCnt.find(eInfo.level) != m_lvToCnt.end()) {
                tmpCnt = m_lvToCnt[eInfo.level];
            }
            tmpColorMap[ eInfo.color ] += tmpCnt;
        }
        else if (!isUUid) {
            auto& eInfo = EquipmentInfoMap[atoi(uuid.c_str())];
            int tmpCnt = 1;
            if (m_lvToCnt.find(eInfo.level) != m_lvToCnt.end()) {
                tmpCnt = m_lvToCnt[eInfo.level];
            }
            tmpColorMap[ eInfo.color ] += tmpCnt;
        }
    }
    
    for (int i=0; i<toolUuids.size(); i++) {
        auto& tool = ToolController::getInstance()->getToolInfoById(toolUuids[i]);
        tmpColorMap[ tool.color ] += 1;
    }
    
    int minColor = 99;
    int maxColor = 99;
    float cntPro = 100.0;
    map<int, float> tmpColorPro;//配置默认最小概率
    tmpColorPro[WHITE] = 0.0;
    tmpColorPro[GREEN] = 0.0;
    tmpColorPro[BLUE] = 0.0;
    tmpColorPro[PURPLE] = 0.0;
    tmpColorPro[ORANGE] = 0.0;
    tmpColorPro[GOLDEN] = 0.0;
    //计算最小颜色值 和 最大颜色值 从高品质 到 低品质 初始计算
    int TmpCNum = 1;
    for (int color=GOLDEN; color>=WHITE; color--)
    {
        if (tmpColorMap[color]>0) {
            tmpColorPro[color] = 0;
            if (maxColor == 99) {
                maxColor = color;
                minColor = color;
            }else {
                minColor = color;
            }
            
            m_colorNum[color] = TmpCNum;
            TmpCNum++;
            
        }else {
            if (maxColor == 99 && minColor == 99) {//还未找到最高品质材料时，更高品的装备 显示默认最小概率
                tmpColorPro[color] = m_defPro[color];
                cntPro -= tmpColorPro[color];
                m_colorNum[color] = 0;//编号为0
            }
            else { //已经找到 比当前品质高的材料了，所以当前品质概率为0
                tmpColorPro[color] = 0;
                
                m_colorNum[color] = TmpCNum;
                TmpCNum++;
            }
        }
    }
    
    if (minColor == maxColor) {
        tmpColorPro[maxColor] = cntPro;
    } else {
        //按照权重计算
        float sumWeight = 0;
        map<int, float> curWeightMap;
        for (int c=GOLDEN; c>=WHITE; c--)
        {
            if (m_colorNum[c]>0)
            {
                
                int tmpSum = 0;
                int tmpC = c;
                while (tmpC >= WHITE) {
                    tmpSum += tmpColorMap[tmpC];
                    tmpC--;
                }
                
                int tmpCCNum = m_colorNum[c];
                float tmpCCSum = 0.0;
                while (tmpCCNum >= 1) {
                    tmpCCSum += m_colorPro[tmpCCNum-1];
                    tmpCCNum--;
                }
                
                curWeightMap[c] = tmpColorMap[c]+tmpSum*tmpCCSum;
                sumWeight += curWeightMap[c];
            }
            else
            {
                curWeightMap[c] = 0;
            }
        }
        
        float lastPro = cntPro;
        for (int j=maxColor; j>=minColor; j--) {
            if (j==minColor) {
                tmpColorPro[j] = lastPro;
            }
            else {
                int tmpProTT = cntPro*100.0*curWeightMap[j]/sumWeight;
                tmpColorPro[j] = tmpProTT*1.0/100;
                lastPro -= tmpColorPro[j];
            }
        }
        
    }
    
    vector<float> ret;
    ret.push_back(tmpColorPro[WHITE]);
    ret.push_back(tmpColorPro[GREEN]);
    ret.push_back(tmpColorPro[BLUE]);
    ret.push_back(tmpColorPro[PURPLE]);
    ret.push_back(tmpColorPro[ORANGE]);
    ret.push_back(tmpColorPro[GOLDEN]);
    return ret;
}

void EquipmentController::MakeEffectValue()
{
    EquipEffMap.clear();
    map<string, int>::iterator it = m_MyOnEquip.begin();
    for (; it!=m_MyOnEquip.end() ; it++)
    {
        auto& eInfo = EquipmentInfoMap[it->second];
        for (int i=1; i<=5; i++)
        {
            if (eInfo.values.find(i) != eInfo.values.end() && eInfo.values[i]>0) {
                for (int j=0; j<eInfo.parasMap[i].size(); j++) {
                    int eNum = eInfo.parasMap[i][j];
                    EquipEffMap[eNum] += eInfo.values[i];
                }
            }
        }
    }
}

//播放战斗力加成特效
void EquipmentController::showPowerChangeEff(string oldEquip, string curEquip)
{
    if (oldEquip == "" && curEquip == "") {
        return;
    }
    double oldPower = 0;
    double newPower = 0;
    if (curEquip != "") {
        newPower = EquipmentInfoMap[m_MyOnEquip[curEquip]].power;
    }
    if (oldEquip != "") {
        oldPower = EquipmentInfoMap[m_MyEquipmentMap[oldEquip]].power;
    }
    double exc = newPower - oldPower;
    GlobalData::shared()->playerInfo.equipPower += exc;
    UIComponent::getInstance()->setUserData();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_POWER_EFFECT_SHOW, CCDouble::create(exc));
}
//提示可打造并穿戴装备
bool EquipmentController::isShowEquipTips(int site){
    bool show=false;
    //铁匠铺锻造处于空闲状态
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
    if (tqid != QID_MAX) {
        return show;
    }
    for(int i=0;i<6;i++){
        if (site!=-1) {
            i=site;
        }
        //领主有为空的装备栏位
        bool equipOn=false;
        for (map<string, int>::iterator it = EquipmentController::getInstance()->m_MyOnEquip.begin(); it != EquipmentController::getInstance()->m_MyOnEquip.end(); it++) {
            auto& info = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if (info.site==i) {
                equipOn=true;
                break;
            }
        }
        if (equipOn) {
            if (site!=-1) {
                break;
            }
            continue;
        }
        //装备栏位为空并且从未穿戴过装备（记录栏位穿戴状态：穿戴过、未穿戴过）
        string udString = "user_had_equip_";
        udString+=CC_ITOA(i);
        bool flag0 = CCUserDefault::sharedUserDefault()->getBoolForKey(udString.c_str(),false);
        if (flag0) {
            if (site!=-1) {
                break;
            }
            continue;
        }
        //储物箱中没有装备栏位对应的1级装备
        bool flag1=false;
        for (map<string, int>::iterator it = EquipmentController::getInstance()->m_MyEquipmentMap.begin(); it!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); it++)
        {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if(tmpInfo.showType != "no" && tmpInfo.site==i && tmpInfo.level==1)
            {
                flag1=true;
                break;
            }
        }
        if (flag1) {
            if (site!=-1) {
                break;
            }
            continue;
        }
        //装备栏位对应的1级装备所需锻造材料齐全
        bool flag2=false;
        int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
        if (buildId>0) {
            EquipmentController::getInstance()->CheckSelEquipMate(i, -1);
            vector<int> tmp1 = EquipmentController::getInstance()->SelEquipmentsByType(i, -1);
            for (int j = 0; j < tmp1.size(); ++j) {
                auto& info = EquipmentController::getInstance()->EquipmentInfoMap[tmp1[j]];
                if (info.level==1 && info.showType != "no" && info.canCrt) {
                    flag2=true;
                    break;
                }
            }
        }
        if (!flag2) {
            if (site!=-1) {
                break;
            }
            continue;
        }
        //符合条件
        show=true;
        break;
    }
    return show;
}

string EquipmentController::GetMateNameById(int mateId)
{
    string ret = "";
    if (mateId == 201010 ) {
        ret = "119284";
    }if (mateId == 201020 ) {
        ret = "119285";
    }if (mateId == 201030 ) {
        ret = "119286";
    }if (mateId == 201040 ) {
        ret = "119287";
    }if (mateId == 201050 ) {
        ret = "119288";
    }if (mateId == 201060 ) {
        ret = "119289";
    }if (mateId == 201070 ) {
        ret = "119290";
    }if (mateId == 201080 ) {
        ret = "119291";
    }if (mateId == 201090 ) {
        ret = "119292";
    }if (mateId == 201100 ) {
        ret = "119293";
    }if (mateId == 201110 ) {
        ret = "119294";
    }if (mateId == 201120 ) {
        ret = "119295";
    }
    return _lang(ret);
}
