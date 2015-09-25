//
//  AllianceApplylistCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#include "AllianceApplylistCommand.h"
#include "NetController.h"
#include "AllianceManager.h"
#include "GlobalData.h"

AllianceApplylistCommand::AllianceApplylistCommand(int page)
: CommandBase(AL_APPLY_LIST),m_page(page)
{
    putParam("page", CCInteger::create(m_page));
}

bool AllianceApplylistCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(AL_APPLY_LIST) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::create());
    }else{
        map<std::string, PlayerInfo*>::iterator it;
        AllianceManager::getInstance()->applyUserList.clear();
        CCArray* arr =  (CCArray*)params->objectForKey("list");
        int num = arr->count();
        for (int i=0; i<num; i++) {
            CCDictionary* dicPlayer = (CCDictionary*)arr->objectAtIndex(i);
            PlayerInfo* player = new PlayerInfo();
            player->updateInfo(dicPlayer);
            AllianceManager::getInstance()->applyUserList[player->uid] = player;
            if(player->uid==GlobalData::shared()->playerInfo.uid){
                if(dicPlayer->objectForKey("relinquishEndTime")){
                    GlobalData::shared()->playerInfo.relinquishEndTime = dicPlayer->valueForKey("relinquishEndTime")->doubleValue();
                }
                if(dicPlayer->objectForKey("officer")){
                    GlobalData::shared()->playerInfo.officer = dicPlayer->valueForKey("officer")->getCString();
                }
            }
        }
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}