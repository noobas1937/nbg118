//
//  TitanInfo.cpp
//  IF
//
//  Created by xxrdsg on 15-2-5.
//
//

#include "TitanInfo.h"
#include "QueueInfo.h"
#include "GlobalData.h"


void splitString(const std::string& strSrc, const std::string& strFind, std::vector<std::string>& arSplit) {
    std::string tmpStr(strSrc.data(),strSrc.length());
    
    if(tmpStr.length() > strFind.length())
    {
        while (!tmpStr.empty() && tmpStr.find(strFind.c_str()) == 0) {
            tmpStr = tmpStr.substr(strFind.length(), tmpStr.length()-strFind.length());
        }
        while (!tmpStr.empty() && tmpStr.rfind(strFind.c_str()) == (tmpStr.length()-strFind.length())) {
            tmpStr = tmpStr.substr(0, tmpStr.length()-strFind.length());
        }
    }
    
    char* src = const_cast<char*>(tmpStr.c_str());
    while (src != NULL && !tmpStr.empty()) {
        arSplit.push_back(std::string(strtok_r(src, strFind.c_str(), &src)));
    }
}

TitanInfo::TitanInfo(CCDictionary* dict)
{
    
    resetTitanInfo(dict);
}

int TitanInfo::resetTitanInfo(CCDictionary* dict)//0没有改变 1数值改变(页面需要刷新的数值) 2状态改变
{
    
    int dataStatus = TITANSTATUSNONE;
    if (dict->objectForKey("feednum")) {
        feedNum = dict->valueForKey("feednum")->intValue();
    }
    
    if (feedcdfix == 0) {//fusheng 等于0的时候尝试取本地数据
        
        string puid = GlobalData::shared()->playerInfo.uid;
        
        
        feedcdfix = CCUserDefault::sharedUserDefault()->getIntegerForKey((puid+"feedcdfix").c_str(), 0);
    }
    
    if (dict->objectForKey("level")) {
        int newLevel = dict->valueForKey("level")->intValue();
        if (newLevel != level) {
            dataStatus |= TITANVALUECHANGE;
            
            if (level != 0) {//fusheng 级别不是0 表示不是第一次初始化 泰坦升级
                dataStatus |= TITANUPGRADE;
            }
            level = newLevel;
            tid = 60000 + level;
            auto temp = dynamic_cast<CCDictionary*>(LocalController::shared()->DBXMLManager()->getGroupByKey("titan")->objectForKey(CCString::createWithFormat("%d", tid)->getCString()));
//            if(temp)//fusheng 这个应该被干掉
//            {
//                feedFoodNum = temp->valueForKey("needfood")->intValue();
//                nextExp = temp->valueForKey("needexp")->intValue();
//                maxManual= temp->valueForKey("maxmanual")->intValue();
//                std::vector<std::string> tmpVec;
//                splitString(temp->valueForKey("feedcdtime")->getCString(), ",", tmpVec);
//                maxFeedIntervalTime = atoi(tmpVec[tmpVec.size()-1].c_str())*60;//fusheng 最后一次喂食时间间隔（秒数）
//                feedMaxNum = tmpVec.size()+1;
//                recoverymanual = temp->valueForKey("recoverymanual")->intValue();
//                costmanual = temp->valueForKey("costmanual")->intValue();
//                recoverInterval = temp->valueForKey("recoverInterval")->intValue();
//                recoverPerInterval = (recoverInterval*1000)/(float)recoverymanual;
////                lastCalTime(0),
////                recoverymanual(0),
////                costmanual(0),
////                recoverInterval(0)
//            }
            if(newLevel != 1)
            {
                
                string oldID =CCString::createWithFormat("1074%02d",newLevel-1)->getCString();
                
                if( GlobalData::shared()->armyList.find(oldID)!= GlobalData::shared()->armyList.end())
                {
                    GlobalData::shared()->armyList[CCString::createWithFormat("1074%02d",newLevel-1)->getCString()].free = 0; //fusheng 刷新泰坦士兵数据
                }
                
                string newID =CCString::createWithFormat("1074%02d",newLevel)->getCString();
                
                if( GlobalData::shared()->armyList.find(oldID) == GlobalData::shared()->armyList.end())
                {
                    
                   
                    ArmyInfo ai ;
                    ai.free = 1;
                    ai.finishTime = 0;
                    ai.food = 0;
                    GlobalData::shared()->armyList.insert(make_pair(newID, ai));
                    
                }
                else
                {
                    GlobalData::shared()->armyList[CCString::createWithFormat("1074%02d",newLevel)->getCString()].free = 1;
                    GlobalData::shared()->armyList[CCString::createWithFormat("1074%02d",newLevel)->getCString()].finishTime = 0;

                }
                
            }

        }
        
        
    }
    if (dict->objectForKey("maxmanual")) {
        int newData =  dict->valueForKey("maxmanual")->intValue();
        if (newData != currentManual) {
            dataStatus |= TITANVALUECHANGE;
            currentManual = newData;
            
        }
        
    }
  
    if (dict->objectForKey("turemaxmanual")) {
        int newData =  dict->valueForKey("turemaxmanual")->intValue();
        
 
        maxManual = newData;
        
    }
    
    if (dict->objectForKey("needfood")) {
        int newData =  dict->valueForKey("needfood")->intValue();

        feedFoodNum = newData;

    }
    
    if (dict->objectForKey("needexp")) {
        int newData =  dict->valueForKey("needexp")->intValue();
        
        nextExp = newData;
        
    }
   
    if (dict->objectForKey("recoverymanual")) {
        int newData =  dict->valueForKey("recoverymanual")->intValue();
        
        recoverymanual = newData;
        
    }
    
    if (dict->objectForKey("recoverInterval")) {
        int newData =  dict->valueForKey("recoverInterval")->intValue();
        
        recoverInterval = newData;
        
         recoverPerInterval = (recoverInterval*1000)/(float)recoverymanual;
        
    }
    
    
    if (dict->objectForKey("costmanual")) {
        int newData =  dict->valueForKey("costmanual")->intValue();
        
        costmanual = newData;
        
    }
    
    
    if (dict->objectForKey("feedcdtime")) {
        string newData =  dict->valueForKey("feedcdtime")->getCString();
        

        
        std::vector<std::string> tmpVec;
        splitString(newData, ",", tmpVec);

        maxFeedIntervalTime = atoi(tmpVec[tmpVec.size()-1].c_str())*60;//fusheng 最后一次喂食时间间隔（秒数）
         feedMaxNum = tmpVec.size()+1;
        
    }
    
    
    
    if (dict->objectForKey("exp")) {
        int newData =  dict->valueForKey("exp")->intValue();
        if (newData != exp) {
            dataStatus |= TITANVALUECHANGE;
            exp = newData;
        }
        
    }
    if (dict->objectForKey("titanid")) {
        
        std::string newData =  dict->valueForKey("titanid")->getCString();
        if (newData != titanId) {
            dataStatus |= TITANVALUECHANGE;
            titanId = newData;
        }
        
    }
    if (dict->objectForKey("status")) {
        int newData =  dict->valueForKey("status")->intValue();
        if (newData != status) {
            dataStatus |= TITANSTATUECHANGE;
            status = newData;
        }
        
    }
    
    if (dict->objectForKey("cdqueue")) {
        auto temp222 = dynamic_cast<CCDictionary*>(dict->objectForKey("cdqueue"));
        
        if (temp222->objectForKey("updateTime")) {
            long newData =  temp222->valueForKey("updateTime")->longValue();
            if (newData/1000 != feedcd) {
                dataStatus |= TITANVALUECHANGE;
                feedcd = newData/1000;
            }
            
        }
        
        
        if (temp222->objectForKey("uuid")) {
            auto newData =  temp222->valueForKey("uuid")->getCString();
//            if (newData != feedcd) {
//                dataStatus |= 1;
//                feedcd = newData;
//            }
            uuid = newData;
            
        }
        
        
        
        
        QueueInfo info = QueueInfo(temp222);

        GlobalData::shared()->allQueuesInfo[info.qid] = info;
        
        
        
    }
    
    
    if (dict->objectForKey("feedcdfix")) {//fusheng 逼我存本地
        
        long WorldTime =  GlobalData::shared()->getWorldTime();
        
        long newData =  dict->valueForKey("feedcdfix")->longValue();
        
        if (newData-GlobalData::shared()->getWorldTime()<=maxFeedIntervalTime *1.2 && newData-GlobalData::shared()->getWorldTime()>0) {//fusheng 临时
            
            
            if (newData != feedcdfix) { //fusheng
                dataStatus |= TITANVALUECHANGE;
                feedcdfix = newData;
                
                string puid = GlobalData::shared()->playerInfo.uid;
                
                CCUserDefault::sharedUserDefault()->setIntegerForKey((puid+"feedcdfix").c_str(), feedcdfix);
                
                CCUserDefault::sharedUserDefault()->flush();
            }
            
            
        }
        
        
        if (newData-GlobalData::shared()->getWorldTime() <= 0) {//可以喂食
//            feedcdfix = GlobalData::shared()->getWorldTime();
            feedcdfix = 0;
            //            CCUserDefault::sharedUserDefault()->setIntegerForKey("feedcdfix", feedcdfix);
            
            string puid = GlobalData::shared()->playerInfo.uid;
            
            CCUserDefault::sharedUserDefault()->setIntegerForKey((puid+"feedcdfix").c_str(), feedcdfix);
            
            CCUserDefault::sharedUserDefault()->flush();          
            
        }
        
        
        
    }


    
    if (dict->objectForKey("remainGold")) {
        
        long newData =  dict->valueForKey("remainGold")->longValue();
        
        GlobalData::shared()->playerInfo.gold = newData;
        
        CCLOG("fusheng gold %ld",GlobalData::shared()->playerInfo.gold);
    }
    
    
    
    if(feedNum>=feedMaxNum)//fusheng 免费次数已用光
    {
        feedcd = 0;//fusheng 不用CD了
        feedcdfix = 0;
        string puid = GlobalData::shared()->playerInfo.uid;
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey((puid+"feedcdfix").c_str(), feedcdfix);
    }
    
    return dataStatus;
}

void TitanInfo::resetLastCalTime(long time)
{
    if(time == -1)
    {
        lastCalTime = (long)GlobalData::shared()->getWorldTime();
    }
    else
    {
        lastCalTime = time/1000;
    }
    
}

TitanInfo::TitanInfo():
tid(0),
feedNum(0),
level(0),
currentManual(-1),
exp(0),
titanId(""),
status(0),
feedcd(0),
nextExp(0),
feedMaxNum(0),
feedFoodNum(0),
uuid(""),
lastCalTime(-1),
recoverymanual(0),
costmanual(0),
recoverInterval(0),
recoverPerInterval(-1),
feedcdfix(0),
maxFeedIntervalTime(60*60)
{
    

}


