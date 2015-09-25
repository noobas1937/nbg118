//
//  AlliacnePushHelpUpdate.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-17.
//
//

#include "AlliacnePushHelpUpdate.h"
#include "FunBuildController.h"

void AlliacnePushHelpUpdate::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    std::string queueId = params->valueForKey("queueId")->getCString();
    std::string senderId = params->valueForKey("senderId")->getCString();
    std::string helpName = params->valueForKey("helpName")->getCString();
    std::string itemId = params->valueForKey("itemId")->getCString();
    int level = params->valueForKey("level")->intValue();
    int type = params->valueForKey("type")->intValue();
    int currentCount = params->valueForKey("nowCount")->intValue();
    int maxCount = params->valueForKey("maxCount")->intValue();
    double updateTime = params->valueForKey("updateTime")->doubleValue();
    std::string puid = GlobalData::shared()->playerInfo.uid;
    if(senderId==puid){
        if(helpName!=""){
            std::string buildingName = CCCommonUtils::getNameById(itemId);
            if (type == TYPE_BUILDING) {
                CCCommonUtils::flyHint("", "", _lang_3("115094", helpName.c_str(), CC_ITOA(level), buildingName.c_str()));
            }
            else if (type == TYPE_SCIENCE) {
                CCCommonUtils::flyHint("", "", _lang_2("115092", helpName.c_str(), buildingName.c_str()));
            }
            else if (type == TYPE_FORGE) {
                CCCommonUtils::flyHint("", "", _lang_2("115092", helpName.c_str(), buildingName.c_str()));
            }
            else if (type == TYPE_HOSPITAL) {
                CCCommonUtils::flyHint("", "", _lang_1("115093", helpName.c_str()));
            }
        }
        if(true){
            int finishTime = params->valueForKey("updateTime")->doubleValue()/1000;
            finishTime = GlobalData::shared()->changeTime(finishTime);
            
            map<int, QueueInfo>::iterator it;
            for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
                if(it->second.uuid==queueId){
                    it->second.finishTime = finishTime;
                    std::string key = it->second.key;
                    if (it->second.type == TYPE_BUILDING) {
                        auto& info = FunBuildController::getInstance()->getFunbuildById(atoi(key.c_str()));
                        info.updateTime = finishTime;
                    }
                    else if (it->second.type == TYPE_SCIENCE) {
                        int scienceId = atoi(it->second.key.c_str()) / 100;
                        if (GlobalData::shared()->scienceInfo.infoMap.find(scienceId) != GlobalData::shared()->scienceInfo.infoMap.end()) {
                            GlobalData::shared()->scienceInfo.infoMap[scienceId].finishTime = finishTime;
                        }
                    }
                    
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHECK_TIME,CCInteger::create(atoi(key.c_str())));
                    break;
                }
            }
        }
    }else{
        if(currentCount>=maxCount){
            GlobalData::shared()->playerInfo.allianceInfo.helpcount = GlobalData::shared()->playerInfo.allianceInfo.helpcount - 1;
            if(GlobalData::shared()->playerInfo.allianceInfo.helpcount<=0){
                GlobalData::shared()->playerInfo.allianceInfo.helpcount = 0;
            }
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_ONE_NUM,params);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_NUM_CHANGE);
    
}

void AlliacneMarkLinePush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if ( GlobalData::shared()->playerInfo.isInAlliance() )
    {
        int rallyPoint = params->valueForKey("rallyPoint")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.rallyPoint = rallyPoint;
        int leaderPoint = params->valueForKey("leaderPoint")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.leaderPoint = leaderPoint;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MARK_LINE_AL);
}