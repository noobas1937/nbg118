//
//  LotteryController.cpp
//  IF
//
//  Created by chenfubi on 15/1/23.
//
//

#include "LotteryController.h"
#include "ToolController.h"
#include "GCMRewardController.h"
#include "LotteryActView.h"
#include "PopupViewController.h"
#include "EquipmentController.h"
static LotteryController* s_sharedLotteryController = NULL;

LotteryController* LotteryController::shared(void)
{
	if (!s_sharedLotteryController)
	{
		s_sharedLotteryController = new LotteryController();
	}
	return s_sharedLotteryController;
}

void LotteryController::purge()
{
	CC_SAFE_RELEASE_NULL(s_sharedLotteryController);
}

LotteryController::LotteryController()
:m_nLotteryReward(nullptr)
,m_sLotteryReward(nullptr)
,m_nLotteryCost(0)
,m_sLotteryCost(0)
,m_rate(0.01f)
,m_rate2(20.0f)
,m_openFlag(0)
,m_openFlag2(0)
,m_openFlag_ko(0)
,m_updateTime(0)
,m_bSendCMD(false)
{
    
}

LotteryController::~LotteryController()
{
    
}
void LotteryController::addCountInfo(cocos2d::CCNode *pNode, std::string itemId){
    int intID = atoi(itemId.c_str());
    if(intID>1000){
        if(ToolController::getInstance()->m_toolInfos.find(intID)!=ToolController::getInstance()->m_toolInfos.end()){
            auto &toolInfo = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
            if(toolInfo.type==3 && !toolInfo.getPara().empty()){
                CCScale9Sprite* numBG = CCLoadSprite::createScale9Sprite("BG_quatnity.png");
                numBG->setColor(CCCommonUtils::getItemColor(toolInfo.color));
                numBG->setOpacity(200);
                CCLabelBMFont* numIF = CCLabelBMFont::create(CC_ITOA_K(atol(toolInfo.getPara().c_str())), "pve_fnt_boss.fnt");
                CCSize numSize = numIF->getContentSize();
                
                const float constScale = 0.3;
                CCSize defSize = pNode->getContentSize();
                float scale = defSize.height*constScale/numSize.height;
                if((numSize.width * scale) > defSize.width){
                    scale = defSize.width/numSize.width;
                }
                numIF->setScale(scale);
                numSize.height *= scale;
                numBG->setPreferredSize(CCSize(defSize.width, defSize.height*constScale));
                pNode->addChild(numBG);
                pNode->addChild(numIF);
                numBG->setPosition(ccp(defSize.width*0.5,defSize.height*(0.5+constScale * 0.5)));
                numIF->setPosition(numBG->getPosition());
            }
        }
    }
}
void LotteryController::parseLotteryData(CCDictionary *dict)
{
    if (m_nLotteryReward == nullptr)
    {
        m_nLotteryReward = CCArray::create();
    }
    if (m_sLotteryReward == nullptr)
    {
        m_sLotteryReward = CCArray::create();
    }
    m_nLotteryReward->removeAllObjects();
    m_sLotteryReward->removeAllObjects();
    lotteryInfo.hasResetReward = true;
    lotteryInfo.reward2Map.clear();
    lotteryInfo.rewardInfo.clear();
    lotteryInfo.rewardSort.clear();
    if (dict == NULL || dict->count() == 0)
    {
        CCLOG("lotteryDataError");
        return;
    }
    
    lotteryInfo.type = dict->valueForKey("type")->intValue()==0?1:dict->valueForKey("type")->intValue();
    lotteryInfo.superMode = dict->valueForKey("supermode")->intValue();
    lotteryInfo.superMode = 0;//todocfb 转盘超级模式关闭
    lotteryInfo.lotteryCnt = dict->valueForKey("times1")->intValue();
    lotteryInfo.lotteryCnt2 = dict->valueForKey("times2")->intValue();
    m_updateTime = dict->valueForKey("updatetime")->doubleValue();
    string normal = dict->valueForKey("normal")->getCString();
    string super = dict->valueForKey("super")->getCString();
    CCLOG("normal:%s,super:%s,updateTime:%s",normal.c_str(),super.c_str(),dict->valueForKey("updatetime")->getCString());
    string rewardStr = dict->valueForKey("rewards")->getCString();
    string lottery = dict->valueForKey("lotteryInfo")->getCString();
    vector<string> rewardVec;
    vector<string> stdVec;
    rewardVec.clear();
    CCCommonUtils::splitString(normal, "|", rewardVec);
    for (int i = 0; i < rewardVec.size(); i++)
    {
        m_nLotteryReward->addObject(ccs(rewardVec.at(i)));
    }
    if (m_nLotteryReward->count() >= 9)
    {
        m_nLotteryReward->insertObject(ccs("100:1"), 2);
        m_nLotteryReward->insertObject(ccs("100:1"), 5);
        m_nLotteryReward->insertObject(ccs("100:1"), 9);
    }
    
    rewardVec.clear();
    CCCommonUtils::splitString(super, "|", rewardVec);
    for (int i = 0; i < rewardVec.size(); i++)
    {
        m_sLotteryReward->addObject(ccs(rewardVec.at(i)));
    }
    if (m_sLotteryReward->count() >= 9)
    {
        m_sLotteryReward->insertObject(ccs("100:1"), 0);
        m_sLotteryReward->insertObject(ccs("100:1"), 4);
        m_sLotteryReward->insertObject(ccs("100:1"), 7);
    }
    
    rewardVec.clear();
    
    string localInfo = CCUserDefault::sharedUserDefault()->getStringForKey(LOTTERY_REWARD_INFO);
    if(localInfo.empty()){
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0,"");
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD,"");
        CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERYACT2_PREVIEW_REWARD, "");
        CCCommonUtils::splitString(lottery, "|", rewardVec);
        CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERY_REWARD_INFO,lottery);
        CCUserDefault::sharedUserDefault()->flush();
    }else if(!lottery.empty()){
        vector<string> tmpInfo;
        CCCommonUtils::splitString(lottery, "|", tmpInfo);
        vector<string> tmpInfo1;
        CCCommonUtils::splitString(localInfo, "|",tmpInfo1);
        map<int,string> tmpMap;
        auto iter1 = tmpInfo1.begin();
        int index = 0;
        while (iter1!=tmpInfo1.end()) {
            tmpMap[index] = (*iter1);
            rewardVec.push_back("");
            ++iter1;
            ++index;
        }
        auto iter = tmpInfo.begin();
        while (iter!=tmpInfo.end()) {
            bool find = false;
            auto iter2 = tmpMap.begin();
            while (iter2!=tmpMap.end()) {
                if((*iter2).second == (*iter)){
                    rewardVec[(*iter2).first]= (*iter);
                    tmpMap.erase(iter2);
                    find=true;
                    break;
                }
                ++iter2;
            }
            if(find == false){
                rewardVec.clear();
                CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0,"");
                CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD,"");
                CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERYACT2_PREVIEW_REWARD, "");
                CCCommonUtils::splitString(lottery, "|", rewardVec);
                CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERY_REWARD_INFO,lottery);
                CCUserDefault::sharedUserDefault()->flush();
                break;
            }
            ++iter;
        }
    }
    
    for (int i = 0; i < rewardVec.size(); i++)
    {
        stdVec.clear();
        CCCommonUtils::splitString(rewardVec.at(i), ":", stdVec);
        vector<string> reward;
        reward.push_back(stdVec.at(0));
        reward.push_back(stdVec.at(1));
        lotteryInfo.rewardInfo[i] = reward;
    }
    
    rewardVec.clear();
    vector<string> tmpStepPosV;
    string tmpStepPosS = CCUserDefault::sharedUserDefault()->getStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0);
    if(!tmpStepPosS.empty()){
        CCCommonUtils::splitString(tmpStepPosS, ",", tmpStepPosV);
    }
    CCCommonUtils::splitString(rewardStr, "|", rewardVec);
    for (int i = 0; i < rewardVec.size(); i++)
    {
        stdVec.clear();
        CCCommonUtils::splitString(rewardVec.at(i), ":", stdVec);
        vector<string> reward;
        reward.push_back(stdVec.at(0));
        reward.push_back(stdVec.at(1));
        reward.push_back(CC_ITOA(i));
        if(std::find(tmpStepPosV.begin(), tmpStepPosV.end(), stdVec.at(2))!=tmpStepPosV.end()){
            reward.push_back("1");
        }
        lotteryInfo.reward2Map[atoi(stdVec.at(2).c_str())] = reward;
    }
    for (int i = 0; i < rewardVec.size(); i++)
    {
        stdVec.clear();
        CCCommonUtils::splitString(rewardVec.at(i), ":", stdVec);
        lotteryInfo.rewardSort.push_back(stdVec.at(0));
    }
    lotteryInfo.hasResetReward = (rewardStr.length()>0);
    
}

void LotteryController::parseLotteryData2(cocos2d::CCDictionary *dict)
{
    CCDictionary* lotteryCost = _dict(dict->objectForKey("lottery_cost"));
    if (lotteryCost)
    {
        m_nLotteryCost = lotteryCost->valueForKey("k1")->intValue();
        m_sLotteryCost = lotteryCost->valueForKey("k2")->intValue();
//        m_openFlag = lotteryCost->valueForKey("k9")->intValue();
    }
    
    CCDictionary* lotteryCost2 = _dict(dict->objectForKey("lottery_cost2"));
    if (lotteryCost2)
    {
        CCDictElement* pelem = NULL;
        int i = 0;
        string key = "";
        CCDICT_FOREACH(lotteryCost2, pelem)
        {
            key = CCString::createWithFormat("k%d",(i+1))->getCString();
            m_lotteryCost2[i] = lotteryCost2->valueForKey(key)->intValue();
            i++;
            if (i >= 9)
            {
                break;
            }
        }
        
        m_rate = lotteryCost2->valueForKey("k10")->floatValue();
        m_rate2 = lotteryCost2->valueForKey("k11")->floatValue();
    }
}

void LotteryController::setReward2Map(int position, vector<string> reward)
{
    lotteryInfo.reward2Map[position] = reward;
    if (lotteryInfo.reward2Map.size() >= 9)
    {
        lotteryInfo.type = 1;
        lotteryInfo.superMode = 0;
        lotteryInfo.rewardSort.clear();
    }
}

void LotteryController::getReward2Map(int position, vector<string> &reward)
{
    map<int, vector<string>>::iterator it = lotteryInfo.reward2Map.begin();
    bool hasData = false;
    for (; it!=lotteryInfo.reward2Map.end(); it++)
    {
        if (it->first == position)
        {
            reward.push_back(it->second.at(0));
            if (it->second.at(1) == "" || it->second.at(1) == "0")
            {
                reward.push_back("1");
            }
            else
            {
                reward.push_back(it->second.at(1));
            }

            hasData = true;
            break;
        }
    }
    if (!hasData)
    {
        reward.push_back("");
        reward.push_back("1");
    }
}

void LotteryController::setRewardInfo(int position, vector<string> reward)
{
    lotteryInfo.rewardInfo[position] = reward;
}

void LotteryController::getRewardInfo(int position, vector<string> &reward)
{
    map<int, vector<string>>::iterator it = lotteryInfo.rewardInfo.begin();
    bool hasData = false;
    for (; it!=lotteryInfo.rewardInfo.end(); it++)
    {
        if (it->first == position)
        {
            reward.push_back(it->second.at(0));
            if (it->second.at(1) == "" || it->second.at(1) == "0")
            {
                reward.push_back("1");
            }
            else
            {
                reward.push_back(it->second.at(1));
            }
            
            hasData = true;
            break;
        }
    }
    if (!hasData)
    {
        reward.push_back("");
        reward.push_back("1");
    }
}

int LotteryController::getStepByPos(int position){
    auto iter = lotteryInfo.reward2Map.find(position);
    if(iter!=lotteryInfo.reward2Map.end()){
        std::string tmpStr = iter->second.at(2);
        return atoi(tmpStr.c_str())+1;
    }
    return 0;
}
bool LotteryController::getStep(string itemId,std::string value, vector<int> &pos){
    map<int, vector<string>>::iterator it = lotteryInfo.reward2Map.begin();
    for (; it!=lotteryInfo.reward2Map.end(); it++)
    {
        if (it->second.at(0) == itemId && it->second.at(1) == value)
        {
            if(it->second.size()>3){
                continue;
            }else{
                std::string tmpStr = it->second.at(2);
                it->second.push_back("1");
                pos.push_back(it->first);
                pos.push_back(atoi(tmpStr.c_str())+1);
                return true;
            }
        }
    }
    return false;
}
int LotteryController::getLotteryCost1()
{
    int cost = m_nLotteryCost;
    if (lotteryInfo.superMode == 1)
    {
//        cost = m_sLotteryCost;
    }
    else
    {
        if (isFirstTime())
        {
            cost = 0;
        }
    }
    return cost;
}

int LotteryController::getLotteryCost2()
{
    int size = lotteryInfo.reward2Map.size();
    if (m_lotteryCost2.size() <= size)
    {
        size = m_lotteryCost2.size() - 1;
        
    }
    return m_lotteryCost2.at(size);
}

CCArray* LotteryController::getLotteryReward()
{
    CCArray* arr = m_nLotteryReward;
    if (lotteryInfo.superMode == 1)
    {
//        arr = m_sLotteryReward;
    }
    return arr;
}

bool LotteryController::isFirstTime()
{
    double time = m_updateTime/1000;
    double tomorrowZero = GlobalData::shared()->tomorrow_time;
    double todayZero = tomorrowZero - 86400;
    if(time >= todayZero &&  time<tomorrowZero){
        return false;
    }
    return true;
    
//    bool _isInSameDay = false;
//    time_t infoTime = GlobalData::shared()->getTimeStamp();
//    string lastTime = CCCommonUtils::timeStampToUTCDate(infoTime).substr(0,10);
//    double time = m_updateTime/1000;
//    string nowTime = CCCommonUtils::timeStampToUTCDate(time).substr(0,10);
//    _isInSameDay = lastTime == nowTime;
//    
//    if (m_updateTime == 0)
//    {
//        _isInSameDay = false;
//    }
//    return !_isInSameDay;
}

bool LotteryController::isMultiReward(string rewardId)
{
    int index = lotteryInfo.reward2Map.size()-1;
    index = index<0?0:index;
    if (index >= lotteryInfo.rewardSort.size())
    {
        return false;
    }
    string id = lotteryInfo.rewardSort.at(index);
    
    return rewardId == id;
}

CCLabelIFBMFont* LotteryController::getLabelBMfont(int itemId)
{
    string numStr = "";
    switch (itemId)
    {
        case 200:
            numStr = "x2";
            break;
        case 201:
            numStr = "x3";
            break;
        case 202:
            numStr = "x5";
            break;
        case 203:
            numStr = "x10";
            break;
        case 204:
            numStr = "x15";
            break;
        case 205:
            numStr = "x20";
            break;
        default:
            break;
    }
    CCLabelIFBMFont* spr = CCLabelIFBMFont::create(numStr.c_str(), "pve_fnt_boss.fnt");
    return spr;
}

string LotteryController::getLotteryIcon(int itemId)
{
    std::string fix = ".png";
    switch (itemId) {
        case Silver:
            return "ui_steel" + fix;
        case Wood:
            return "ui_wood" + fix;
        case Iron:
            return "ui_iron" + fix;
        case Food:
            return "ui_food" + fix;
        case Stone:
            return "ui_stone" + fix;
        case Gold:
            return "ui_gold" + fix;
        case Chip:
            return "Lottery_bargaining-chip1" + fix;
        case Diamond:
            return "Lottery_bargaining-chip2" + fix;
        case R_CHIP:
            return "Lottery_bargaining-chip1" + fix;
        case R_DIAMOND:
            return "Lottery_bargaining-chip2" + fix;
        case 100:
            return "Lottery_baoxiang.png";
        default:
            return CCCommonUtils::getIcon(CC_ITOA(itemId));
    }
    return std::string("");
}

void LotteryController::getItemDes(int position, vector<string> &vecStr, bool desAniInfo/*=false*/)
{
    vector<string> reward;
    if (desAniInfo)
    {
        getRewardInfo(position, reward);
    }
    else
    {
        getReward2Map(position, reward);
    }
    string itemId = reward.at(0);
    string cnt = reward.at(1);
    int itemIdInt = atoi(itemId.c_str());
    string name = "";
    string info = "";
    switch (itemIdInt)
    {
        case Silver:
            name = cnt.append(" ").append(_lang(""));
            break;
        case Wood:
            name = cnt.append(" ").append(_lang("102209"));
            break;
        case Iron:
            name = cnt.append(" ").append(_lang("102210"));
            break;
        case Food:
            name = cnt.append(" ").append(_lang("102211"));
            break;
        case Stone:
            break;
        case Gold:
            name = cnt.append(" ").append(_lang("107518"));
            break;
        case Chip:
        case R_CHIP:
            name = _lang("111101");
            info = _lang("111117");
            break;
        case Diamond:
        case R_DIAMOND:
            name = _lang("111102");
            info = _lang("111118");
            break;
        case 200:
            name = "X2";
            info = _lang("111115");
            break;
        case 201:
            name = "X3";
            info = _lang("111115");
            break;
        case 202:
            name = "X5";
            info = _lang("111115");
            break;
        case 203:
            name = "X10";
            info = _lang("111115");
            break;
        case 204:
            name = "X15";
            info = _lang("111115");
            break;
        case 205:
            name = "X20";
            info = _lang("111115");
            break;
        default:
            if(CCCommonUtils::isEquip(itemIdInt)){
                auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[itemIdInt];
                name= _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
                info = _lang(eInfo.des);
            }else{
                ToolInfo& tool = ToolController::getInstance()->getToolInfoById(itemIdInt);
                name = tool.getName();
                info = _lang(tool.des);
            }
            break;
    }
    vecStr.push_back(name);
    vecStr.push_back(info);
}

string LotteryController::getLotteryName(int itemId, int itemCnt)
{
    string itemCntStr = CC_CMDITOA(itemCnt); //CC_ITOA(itemCnt);
    string name = "";
    switch (itemId)
    {
        case Silver:
            name = itemCntStr + " " + _lang("107512");
            break;
        case Wood:
            name = itemCntStr + " " + _lang("102209");
            break;
        case Iron:
            name = itemCntStr + " " + _lang("102210");
            break;
        case Food:
            name = itemCntStr + " " + _lang("102211");
            break;
        case Stone:
            name = itemCntStr + " " + _lang("107512");
            break;
        case Gold:
            name = itemCntStr + " " + _lang("107518");
            break;
        case Chip:
        case R_CHIP:
            name = itemCntStr + " " + _lang("111101");
            break;
        case Diamond:
        case R_DIAMOND:
            name = itemCntStr + " " + _lang("111102");
            break;
        case 200:
            name = "X2";
            break;
        case 201:
            name = "X3";
            break;
        case 202:
            name = "X5";
            break;
        case 203:
            name = "X10";
            break;
        case 204:
            name = "X15";
            break;
        case 205:
            name = "X20";
            break;
        default:
            if(CCCommonUtils::isEquip(itemId)){
                auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[itemId];
                name= _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level) + " X " + itemCntStr;
            }else{
                ToolInfo& tool = ToolController::getInstance()->getToolInfoById(itemId);
                name = tool.getName() + " X " + itemCntStr;
            }
            break;
    }
    CCLOG("LotteryName:%s,%s",itemCntStr.c_str(), name.c_str());
    return name;
}

void LotteryController::lotteryActCmdBack(cocos2d::CCObject *pObj)
{
    lotteryInfo.type = 1;
    CCDictionary* rewardDic = _dict(pObj);
    if (rewardDic->objectForKey("updatetime"))
    {
        bool send = isFirstTime();
        double updateTime = rewardDic->valueForKey("updatetime")->doubleValue();
        this->setUpdateTime(updateTime);
        if (send)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("DAILYQEUST_REWARD");
        }
    }
    const CCString *pStr = rewardDic->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        return ;
    }
    
    CCDictionary* rewards = _dict(rewardDic->objectForKey("rewards"));
    
    if (!rewardDic || rewardDic->count()==0)
    {
        return;
    }
    int rewardId = rewards->valueForKey("rewardId")->intValue();
    
    if (rewardId == 0)
    {
        CCArray* arr = CCArray::create();
        arr->addObject(rewards);
        GCMRewardController::getInstance()->retReward2(arr, false);
        
        LotteryActView * lotteryActView = dynamic_cast<LotteryActView*>(PopupViewController::getInstance()->getCurrentPopupView());
        if(lotteryActView==NULL){
            string m_itemId = rewards->valueForKey("type")->getCString();
            int m_itemCnt = rewards->valueForKey("value")->intValue();

            int id = atoi(m_itemId.c_str());
            CCCommonUtils::flyHintWithDefault(getIcon(m_itemId), "", _lang_1("104913", LotteryController::shared()->getLotteryName(id, m_itemCnt).c_str()));
        }
    }
    else if (rewardId == 100)
    {
        lotteryInfo.rewardSort.clear();
        string lottery = rewardDic->valueForKey("lottery")->getCString();
        if (lottery.length() > 0)
        {
            vector<string> lotVec;
            CCCommonUtils::splitString(lottery, "|", lotVec);
            
            vector<string> strVec;
            for (int i = 0; i < lotVec.size(); i++)
            {
                strVec.clear();
                CCCommonUtils::splitString(lotVec.at(i), ":", strVec);
                LotteryController::shared()->setRewardInfo(i, strVec);
            }
        }
        lotteryInfo.type = 2;
        lotteryInfo.hasResetReward = false;
        
        CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERY_REWARD_INFO, lottery);
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD, "");
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0, "");
        CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERYACT2_PREVIEW_REWARD, "");
        CCUserDefault::sharedUserDefault()->flush();
    }
    else
    {
        if (rewards->objectForKey("items"))
        {
            CCArray* arr = dynamic_cast<CCArray*>(rewards->objectForKey("items"));
            GCMRewardController::getInstance()->retReward2(arr, false);
            LotteryActView * lotteryActView = dynamic_cast<LotteryActView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(lotteryActView==NULL){
                string m_itemId ="";
                int m_itemCnt = 0;
                CCObject* obj = NULL;
                CCARRAY_FOREACH(arr, obj){
                    auto dict = dynamic_cast<CCDictionary*>(obj);
                    int type = dict->valueForKey("type")->intValue();
                    if (type == R_GOODS)
                    {
                        m_itemId = _dict(dict->objectForKey("value"))->valueForKey("itemId")->getCString();
                        m_itemCnt = _dict(dict->objectForKey("value"))->valueForKey("rewardAdd")->intValue();
                    }
                    else
                    {
                        m_itemId = CC_ITOA(type);
                        m_itemCnt = dict->valueForKey("value")->intValue();
                    }
                }
                int id = atoi(m_itemId.c_str());
                CCCommonUtils::flyHintWithDefault(getIcon(m_itemId), "", _lang_1("104913", LotteryController::shared()->getLotteryName(id, m_itemCnt).c_str()));
            }
        }
    }

}

string LotteryController::getIcon(string itemId)
{
    std::string fix = ".png";
    switch (atoi(itemId.c_str()))
    {
        case Silver:
        return "ui_silver" + fix;
        case Wood:
        return "ui_wood" + fix;
        case Iron:
        return "ui_stone" + fix;
        case Food:
        return "ui_food" + fix;
        case Stone:
        return "ui_silver" + fix;
        case Gold:
        return "Lottery_gold" + fix;
        case R_CHIP:
        return "item409" + fix;
        case R_DIAMOND:
        return "item410" + fix;
        case 100:
        return "Lottery_baoxiang.png";
        default:
        return CCCommonUtils::getIcon(itemId);
    }
    return std::string("");
}

void LotteryController::lotteryAct2CmdBack(cocos2d::CCObject *pObj)
{
    CCDictionary* dic = _dict(pObj);
    const CCString *pStr = dic->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        return;
    }
    CCDictionary* rewardDic = _dict(dic->objectForKey("rewards"));
    int position = dic->valueForKey("position")->intValue();
    
    int rewardId = rewardDic->valueForKey("rewardId")->intValue();
    if (rewardId == 0)
    {
        return;
    }
    int multiple = rewardDic->valueForKey("multiple")->intValue();
    multiple = multiple<=0?1:multiple;
    string itemId = "";
    int itemCnt = 1;
    if (rewardId >= 200 && rewardId <= 205)
    {
        itemId = rewardDic->valueForKey("rewardId")->getCString();
        itemCnt = 1;
    }
    else
    {
        int type = rewardDic->valueForKey("type")->intValue();
        if(type == R_EQUIP){
            CCArray* array = dynamic_cast<CCArray*>(rewardDic->objectForKey("value"));
            if(array && array->count()>0){
                CCDictionary *ccDict = _dict(array->objectAtIndex(0));
                itemId = ccDict->valueForKey("itemId")->getCString();
                itemCnt = array->count();
                EquipmentController::getInstance()->addEquip(array);
            }
        }else{
            if (type != R_GOODS)
            {
                itemId = CC_ITOA(type);
                itemCnt = rewardDic->valueForKey("value")->intValue();
            }
            else
            {
                CCDictionary* value = _dict(rewardDic->objectForKey("value"));
                itemId = value->valueForKey("itemId")->getCString();
                itemCnt = value->valueForKey("rewardAdd")->intValue();
            }
            CCArray* arr = CCArray::create();
            arr->addObject(rewardDic);
            GCMRewardController::getInstance()->retReward2(arr, false);
        }
    }
    string cntStr = CC_ITOA(itemCnt/multiple);
    vector<string> strVec;
    strVec.push_back(itemId);
    strVec.push_back(cntStr);
    int nIndex = LotteryController::shared()->lotteryInfo.reward2Map.size();
    strVec.push_back(CC_ITOA(nIndex));
    LotteryController::shared()->lotteryInfo.rewardSort.push_back(itemId);
    LotteryController::shared()->setReward2Map(position, strVec);
    
}