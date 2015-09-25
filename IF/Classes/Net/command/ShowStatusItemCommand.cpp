//
//  ShowStatusItemCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-21.
//
//

#include "ShowStatusItemCommand.h"
#include "NetController.h"
#include "ToolController.h"


ShowStatusItemCommand::ShowStatusItemCommand()
: CommandBase(SHOW_STATUS_ITEM)
{
}

bool ShowStatusItemCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SHOW_STATUS_ITEM) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        ToolController::getInstance()->m_statusItems.clear();
        CCArray* arr =  (CCArray*)params->objectForKey("statusItems");
        int num = arr->count();
        for (int i=0; i<num; i++) {
            CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(i);
            dic->retain();
            int type = dic->valueForKey("type2")->intValue();
            ToolController::getInstance()->m_statusItems[type] = dic;  
        }
        callSuccess(NetResult::create());
    }
    return true;
}
bool CancelStatusItemCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(CANCEL_STATUS_ITEM) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        auto iter = GlobalData::shared()->statusMap.find(m_statusId);
        if(iter!=GlobalData::shared()->statusMap.end()){
            GlobalData::shared()->statusMap.erase(iter);
        }
        auto iter1 = ToolController::getInstance()->m_statusItems.find(100);
        if(iter1!=ToolController::getInstance()->m_statusItems.end()){
            ToolController::getInstance()->m_statusItems.erase(iter1);
        }
        ToolController::getInstance()->resetMaincityCustom(0);
        callSuccess(NetResult::create());
    }
    return true;
}