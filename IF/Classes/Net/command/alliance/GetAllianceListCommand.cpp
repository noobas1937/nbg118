//
//  GetAllianceListCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-3-25.
//
//

#include "GetAllianceListCommand.h"
#include "NetController.h"
#include "AllianceInfo.h"
#include "AllianceManager.h"

GetAllianceListCommand::GetAllianceListCommand(std::string key,int page,int type)
: CommandBase(GET_ALLIANCE_LIST),m_key(key),m_page(page),m_type(type)
{
    putParam("key", CCString::create(m_key.c_str()));
    putParam("page", CCInteger::create(page));
    putParam("type", CCInteger::create(m_type));
    if(m_type==0){
        string langId = "";
        
        CCDictionary* dic_all = LocalController::shared()->shared()->DBXMLManager()->getGroupByKey("language");
        int idx = 9500;
        int num = dic_all->count();
        string curLang = LocalController::shared()->getLanguageFileName();
        if(curLang.find("zh_CN")<curLang.length()||curLang.find("zh-Hans")<curLang.length() || curLang.find("zh_TW")<curLang.length() || curLang.find("zh-Hant")<curLang.length()){
            curLang = "zh";
        }
        for (int i=0; i<num; ++i) {
            CCDictionary* dic_one = _dict(dic_all->objectForKey(CC_ITOA(idx+i)));
            string mark = dic_one->valueForKey("mark")->getCString();
            string lang_ma = dic_one->valueForKey("lang_ma")->getCString();
            string lang_id = dic_one->valueForKey("lang_id")->getCString();
            if(mark!="" && lang_ma!="" && curLang.find(lang_ma)<curLang.length()){
                langId = lang_id;
                break;
            }
        }
        if(langId==""){
            langId = "115601";
        }
        putParam("language", CCString::create(langId.c_str()));
    }
}

bool GetAllianceListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_ALLIANCE_LIST) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        if(m_key!=""){
            map<std::string, AllianceInfo*>::iterator it;
            for(it = AllianceManager::getInstance()->allianceList.begin(); it != AllianceManager::getInstance()->allianceList.end(); it++){
                it->second->release();
            }
            AllianceManager::getInstance()->allianceList.clear();
        }
        //        CCArray* arr =  (CCArray*)params->objectForKey("list");
        //        int total = params->valueForKey("total")->intValue();
        //        AllianceManager::getInstance()->total = total;
        //        AllianceManager::getInstance()->maxAlliancePage = total/20+(total%20==0?0:1);
        //        int num = arr->count();
        //        for (int i=0; i<num; i++) {
        //            CCDictionary* dicAlliance = (CCDictionary*)arr->objectAtIndex(i);
        //            std::string uid = dicAlliance->valueForKey("uid")->getCString();
        //            map<std::string, AllianceInfo*>::iterator tempIt = AllianceManager::getInstance()->allianceList.find(uid);
        //            if(tempIt!=AllianceManager::getInstance()->allianceList.end()){
        //                tempIt->second->updateAllianceInfo(dicAlliance);
        //            }else{
        //                AllianceInfo* alliance = new AllianceInfo();
        //                alliance->updateAllianceInfo(dicAlliance);
        //                AllianceManager::getInstance()->allianceList[alliance->uid] = alliance;
        //            }
        //        }
        //        callSuccess(NetResult::create());
        
        callSuccess(NetResult::create(Error_OK, (CCObject*)params));
    }
    return true;
}

bool GetAllianceUrlCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_ALLIANCE_URL) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    else {
        AllianceManager::getInstance()->endGetAllianceUrl(params);
    }
    return true;
}