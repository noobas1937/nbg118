//
//  GetFriendListCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-10-20.
//
//

#include "GetFriendListCommand.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"


bool GetFriendListCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_GET_FRIEND_LIST) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        onGetFriendInfoSuccess(params);
    }
    return true;
}

void GetFriendListCommand::onGetFriendInfoSuccess(CCObject* pObj)
{
    CCLOGFUNC("");
    CCDictionary* params = dynamic_cast<CCDictionary*>(pObj);
    if (params==NULL || ChatController::getInstance()->m_chatUserInfoDic==NULL) {
        return;
    }
    CCArray* members = dynamic_cast<CCArray*>(params->objectForKey("contactList"));
    string uidStr="";
    string lastUpdateTimeStr="";
    if(members)
    {
        vector<std::string> *uids = new vector<std::string>();
        CCLOGFUNCF("members->count():%d",members->count());
        for (int i=0; i < members->count(); i++) {
            CCDictionary* member = (CCDictionary*)members->objectAtIndex(i);

            string uid=member->valueForKey("uid")->getCString();
            if(uid!="")
            {
                if (ChatController::getInstance()->m_chatUserInfoDic->objectForKey(uid)==NULL) {
                    uids->push_back(uid);
                }
                string time=member->valueForKey("lastUpdateTime")->getCString();
                if (time=="") {
                    time="0";
                }
                
                if(uidStr!="")
                {
                    uidStr.append("_").append(uid);
                    lastUpdateTimeStr.append("_").append(time);
                }
                else
                {
                    uidStr=uid;
                    lastUpdateTimeStr=time;
                }
            }

        }

        CCLOGFUNCF("uidStr:%s",uidStr.c_str());
    }
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if (uidStr!="") {
        ChatServiceCocos2dx::notifyUserUids(uidStr,lastUpdateTimeStr,GET_MUTIUSER_TYPE_FRIEND);
    }
#endif
}
