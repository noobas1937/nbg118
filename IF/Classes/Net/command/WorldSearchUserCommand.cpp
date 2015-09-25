//
//  WorldSearchUserCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-22.
//
//

#include "WorldSearchUserCommand.h"
#include "VipUtil.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"

bool WorldSearchUserCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(WORLD_SEARCH_USER) != 0)
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
        callSuccess(NetResult::create(Error_OK, params));
        if(MailController::getInstance()->getIsSearchUser())
            parseSearchedResult(params);
    }
    return true;
}

void WorldSearchUserCommand::parseSearchedResult(CCDictionary* param)
{
    CCLOGFUNC("parseSearchedResult");
    if(param==NULL)
    {
        CCLOGFUNC("param==NULL");
        return;
    }
    CCArray* members = NULL;
    if ((param && param->objectForKey("list"))) {
        CCLOGFUNC("parseSearchedResult 1 ");
        members = dynamic_cast<CCArray*>(param->objectForKey("list"));
    }
    CCLOGFUNC("parseSearchedResult 3");
    CCArray* memberArr=CCArray::create();
    if(members!=NULL){
        CCLOGFUNCF("members count %d",members->count());
        for (int i=0; i < members->count(); i++) {
            CCDictionary* member = dynamic_cast<CCDictionary*>(members->objectAtIndex(i));
            if(member!=NULL)
            {
                CCDictionary* userInfoDic=CCDictionary::create();
                
                string uid ="";
                if(member->objectForKey("uid")){
                    uid=member->valueForKey("uid")->getCString();
                    CCLOGFUNCF("uid %s",uid.c_str());
                }
                userInfoDic->setObject(CCString::create(uid), "uid");
                
                string allianceId="";
                if(member->objectForKey("allianceId")){
                    allianceId=member->valueForKey("allianceId")->getCString();
                    CCLOGFUNCF("allianceId %s",allianceId.c_str());
                }
                userInfoDic->setObject(CCString::create(allianceId), "allianceId");
                
                int rank=-1;
                if(member->objectForKey("rank")){
                    rank=member->valueForKey("rank")->intValue();
                    CCLOGFUNCF("rank %d",rank);
                }
                userInfoDic->setObject(CCInteger::create(rank), "allianceRank");
                
                string name="";
                if(member->objectForKey("name")){
                    name=member->valueForKey("name")->getCString();
                    CCLOGFUNCF("name %s",name.c_str());
                }
                userInfoDic->setObject(CCString::create(name), "userName");
                
                string pic="g004";
                if(member->objectForKey("pic")){
                    pic=member->valueForKey("pic")->getCString();
                    CCLOGFUNCF("pic %s",pic.c_str());
                }
                userInfoDic->setObject(CCString::create(pic), "headPic");
                
                int picVer=0;
                if(member->objectForKey("picVer")){
                    picVer=member->valueForKey("picVer")->intValue();
                    CCLOGFUNCF("picVer %d",picVer);
                }
                 userInfoDic->setObject(CCInteger::create(picVer), "headPicVer");
                
                int gmFlag=0;
                if(member->objectForKey("gmFlag")){
                    gmFlag=member->valueForKey("gmFlag")->intValue();
                    CCLOGFUNCF("gmFlag %d",gmFlag);
                }
                userInfoDic->setObject(CCInteger::create(gmFlag), "mGmod");
                
                int lastUpdateTime=0;
                if(member->objectForKey("lastUpdateTime")){
                    lastUpdateTime=member->valueForKey("lastUpdateTime")->intValue();
                    CCLOGFUNCF("lastUpdateTime %d",lastUpdateTime);
                }
                userInfoDic->setObject(CCInteger::create(lastUpdateTime), "lastUpdateTime");
                
                int vipLevel=0;
                if(member->objectForKey("vipLevel")){
                    vipLevel=VipUtil::getVipLevel(member->valueForKey("vipLevel")->longValue());
                    CCLOGFUNCF("vipLevel %d",vipLevel);
                }
                userInfoDic->setObject(CCInteger::create(vipLevel), "vipLevel");
                
                long vipEndTime=0;
                if(member->objectForKey("vipEndTime")){
                    vipEndTime=member->valueForKey("vipEndTime")->longValue();
                    CCLOGFUNCF("vipEndTime %ld",vipEndTime);
                }
                userInfoDic->setObject(CCInteger::create(vipEndTime), "vipEndTime");
                
                string asn="";
                if(member->objectForKey("abbr")){
                    asn=member->valueForKey("abbr")->getCString();
                    CCLOGFUNCF("asn %s",asn.c_str());
                }
                userInfoDic->setObject(CCString::create(asn), "asn");
                
                int serverId=-1;
                if(member->objectForKey("serverId")){
                    serverId=member->valueForKey("serverId")->intValue();
                     CCLOGFUNCF("serverId %d",serverId);
                }
                userInfoDic->setObject(CCInteger::create(serverId), "serverId");
                
                int crossFightSrcServerId=-1;
                if(member->objectForKey("crossFightSrcServerId")){
                    crossFightSrcServerId=member->valueForKey("crossFightSrcServerId")->intValue();
                    CCLOGFUNCF("crossFightSrcServerId %d",crossFightSrcServerId);
                }
                userInfoDic->setObject(CCInteger::create(crossFightSrcServerId), "crossFightSrcServerId");
                
                memberArr->addObject(userInfoDic);
            }
        }
    }
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    CCLOGFUNCF("memberArr->count():%d",memberArr->count());
    ChatController::getInstance()->m_userInfoDic->setObject(memberArr, ChatServiceCocos2dx::m_curUserInfoIndex);
    ChatServiceCocos2dx::notifySearchedUserInfo(ChatServiceCocos2dx::m_curUserInfoIndex);
    ChatServiceCocos2dx::m_curUserInfoIndex++;
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    ChatServiceCocos2dx::selectPlayer(memberArr);
#endif
    MailController::getInstance()->setIsSearchUser(false);
}