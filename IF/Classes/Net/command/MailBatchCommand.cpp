
#include "MailBatchCommand.h"
#include "ToolController.h"
#include "GCMRewardController.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include "PopupViewController.h"

bool MailBatchDelCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DELETE_BATCH_COMMAND) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return true;
    }
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(!MailController::getInstance()->getIsNewMailListEnable())
        MailController::getInstance()->endRemoveAllOpMails(_dict(params->objectForKey("RetObj")));
    else
    {
        ChatServiceCocos2dx::setMutiMailStatusByConfigType(m_uids,3);
    }
#else
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
        ChatServiceCocos2dx::deleteingbanchMailsWithMailID(m_uids);
    }else{
        MailController::getInstance()->endRemoveAllOpMails(_dict(params->objectForKey("RetObj")));
    }
    
#endif
    return true;
}

bool MailBatchRewardCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_REWARD_BATCH_COMMAND) != 0)
        return false;
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        
        return true;
    }
    
    if (params->valueForKey("success")->intValue() == 1 ) {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
            ChatServiceCocos2dx::setMailRewardStatusForBatchToReadWithMailIDs(m_uids,1);
        }
#endif
    }
    if(params->objectForKey("honor")){
        int honor = params->valueForKey("honor")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.accPoint = honor;
    }
    if(params->objectForKey("alliancepoint")){
        int alliancepoint = params->valueForKey("alliancepoint")->intValue();
        GlobalData::shared()->playerInfo.allianceInfo.point = alliancepoint;
    }
    string rewardStr = "{\"flyToolReward\":[";
    if(params->objectForKey("goods")){
        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("goods"));
        
        CCLOGFUNC("111");
        
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        
        if(MailController::getInstance()->getIsNewMailListEnable())
        {
            //            MailController::getInstance()->m_mutiFlyToolRewardArray->removeAllObjects();
            //            MailController::getInstance()->m_mutiFlyToolRewardArray->addObjectsFromArray(arr);
            //            if (ChatServiceCocos2dx::isChatShowing) {
            //                ChatServiceCocos2dx::exitChatActivityFrom2dx(false);
            //            }
            
            CCObject* obj;
            
            string rewardToolBody = "";
            CCARRAY_FOREACH(arr, obj) {
                auto tdic = _dict(obj);
                if(tdic)
                {
                    string itemId ="";
                    if (tdic->objectForKey("itemId")) {
                        itemId = tdic->valueForKey("itemId")->getCString();
                        if (itemId!="") {
                            auto &item = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
                            int num = 0;
                            if (tdic->objectForKey("rewardAdd")) {
                                num = tdic->valueForKey("rewardAdd")->intValue();
                            } else if (tdic->objectForKey("count")){
                                num = tdic->valueForKey("count")->intValue() - item.getCNT();
                            }
                            if(rewardToolBody!="")
                                rewardToolBody.append(",");
                            rewardToolBody.append("{\"itemPic\":").append("\"").append(CCCommonUtils::getIcon(itemId.c_str())).append("\"").append(",\"itemNum\":").append(CC_ITOA(num)).append("}");
                        }
                    }
                }
            }
            rewardStr.append(rewardToolBody);
        }
        else
        {
            GCMRewardController::getInstance()->flyToolReward(arr);
        }
        
#else
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
            //            ChatServiceCocos2dx::setMailRewardStatusForBatchToReadWithMailIDs(m_uids,0);
            CCObject* obj;
            
            string rewardToolBody = "";
            CCARRAY_FOREACH(arr, obj) {
                auto tdic = _dict(obj);
                if(tdic)
                {
                    string itemId ="";
                    if (tdic->objectForKey("itemId")) {
                        itemId = tdic->valueForKey("itemId")->getCString();
                        if (itemId!="") {
                            auto &item = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
                            int num = 0;
                            if (tdic->objectForKey("rewardAdd")) {
                                num = tdic->valueForKey("rewardAdd")->intValue();
                            } else if (tdic->objectForKey("count")){
                                num = tdic->valueForKey("count")->intValue() - item.getCNT();
                            }
                            if(rewardToolBody!="")
                                rewardToolBody.append(",");
                            rewardToolBody.append("{\"itemPic\":").append("\"").append(CCCommonUtils::getIcon(itemId.c_str())).append("\"").append(",\"itemNum\":").append(CC_ITOA(num)).append("}");
                        }
                    }
                }
            }
            rewardStr.append(rewardToolBody);
            
        }else{
            GCMRewardController::getInstance()->flyToolReward(arr);
            
        }
        
#endif
    }
    CCLOGFUNCF("1 rewardStr:%s",rewardStr.c_str());
    rewardStr.append("],\"flyReward\":[");
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(MailController::getInstance()->getIsNewMailListEnable())
    {
        //        MailController::getInstance()->m_mutiFlyRewardArray->removeAllObjects();
        //        MailController::getInstance()->m_mutiFlyRewardArray->addObject(params);
        //        if (ChatServiceCocos2dx::isChatShowing) {
        //            ChatServiceCocos2dx::exitChatActivityFrom2dx(false);
        //        }
        ChatServiceCocos2dx::setMutiMailRewardStatus(m_uids);
        
        if (params) {
            CCLOGFUNCF("rewardStr:%s",rewardStr.c_str());
            CCArray* arr = GCMRewardController::getInstance()->retReward(params);
            string rewardBody = "";
            int i = 0;
            CCLOGFUNCF(" arr->count():%d", arr->count());
            while(i < arr->count()){
                string typeStr = "";
                CCString* type = dynamic_cast<CCString*>(arr->objectAtIndex(i));
                if(type)
                {
                    typeStr = type->getCString();
                    i++;
                }
                int num = 0;
                CCInteger* numInteger = dynamic_cast<CCInteger*>(arr->objectAtIndex(i));
                if(numInteger)
                {
                    num = numInteger->getValue();
                    i++;
                }
                i++;
                
                if(typeStr == "general"){
                    num = 0;
                }
                else if (typeStr == "equip") {
                    num = 1;
                }
                CCLOGFUNCF("typeStr:%s   num:%d",typeStr.c_str(),num);
                if(typeStr!="")
                {
                    if(rewardBody!="")
                        rewardBody.append(",");
                    rewardBody.append("{\"itemPic\":").append("\"").append(GCMRewardController::getInstance()->getPicByType(typeStr, num)).append("\"").append(",\"itemNum\":").append(CC_ITOA(num)).append("}");
                }
                
            }
            rewardStr.append(rewardBody);
        }
        
    }
    else
    {
        CCArray* arr = GCMRewardController::getInstance()->retReward(params);
        GCMRewardController::getInstance()->flyReward(arr);
        MailController::getInstance()->endRewardAllOpMails();
    }
#else
    
    
    if (ChatServiceCocos2dx::Mail_OC_Native_Enable) {
        
        //         ChatServiceCocos2dx::setMailRewardStatusForBatchToReadWithMailIDs(m_uids,0);
        if (params) {
            CCLOGFUNCF("rewardStr:%s",rewardStr.c_str());
            CCArray* arr = GCMRewardController::getInstance()->retReward(params);
            string rewardBody = "";
            int i = 0;
            CCLOGFUNCF(" arr->count():%d", arr->count());
            while(i < arr->count()){
                string typeStr = "";
                CCString* type = dynamic_cast<CCString*>(arr->objectAtIndex(i));
                if(type)
                {
                    typeStr = type->getCString();
                    i++;
                }
                
                
                int num = 0;
                CCInteger* numInteger = dynamic_cast<CCInteger*>(arr->objectAtIndex(i));
                if(numInteger)
                {
                    num = numInteger->getValue();
                    i++;
                }
                i++;
                
                if(typeStr == "general"){
                    num = 0;
                }
                else if (typeStr == "equip") {
                    num = 1;
                }
                CCLOGFUNCF("typeStr:%s   num:%d",typeStr.c_str(),num);
                if(typeStr!="")
                {
                    if(rewardBody!="")
                        rewardBody.append(",");
                    rewardBody.append("{\"itemPic\":").append("\"").append(GCMRewardController::getInstance()->getPicByType(typeStr, num)).append("\"").append(",\"itemNum\":").append(CC_ITOA(num)).append("}");
                }
                
            }
            rewardStr.append(rewardBody);
        }
        
    }else{
        CCArray* arr = GCMRewardController::getInstance()->retReward(params);
        GCMRewardController::getInstance()->flyReward(arr);
        MailController::getInstance()->endRewardAllOpMails();
        
        
    }
    
#endif
    
    rewardStr.append("]}");
    
    CCLOGFUNCF("rewardStr:%s",rewardStr.c_str());
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(rewardStr!="" && rewardStr!="{\"flyToolReward\":[],\"flyReward\":[]}")
    {
        ChatServiceCocos2dx::postMutiRewardItem(rewardStr);
    }
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if(rewardStr!="" && rewardStr!="{\"flyToolReward\":[],\"flyReward\":[]}")
    {
        ChatServiceCocos2dx::settingMailRewadToReadFor_batchWithRewardString(m_uids,rewardStr,0);
    }else{
        ChatServiceCocos2dx::settingMailRewadToReadFor_batchWithRewardString(m_uids,"",1);
    }
    
#endif
    
    return true;
}
bool MailBatchSaveCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_SAVE_BATCH_COMMAND) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return true;
    }
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(!MailController::getInstance()->getIsNewMailListEnable())
        MailController::getInstance()->endRemoveAllOpMails(_dict(params->objectForKey("RetObj")));/////////////
    else
    {
        if(m_flag==1)
            ChatServiceCocos2dx::setMutiMailStatusByConfigType(m_uids,2);
        else if(m_flag==0)
            ChatServiceCocos2dx::setMutiMailStatusByConfigType(m_uids,2,true);
    }
#else
    MailController::getInstance()->endRemoveAllOpMails(_dict(params->objectForKey("RetObj")));///////////
#endif
    return true;
}
bool MailReadStatusBatchCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_READ_BATCH_COMMAND) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return true;
    }
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    if(MailController::getInstance()->getIsNewMailListEnable())
    {
        ChatServiceCocos2dx::setMutiMailStatusByConfigType(m_uids,1);
    }
#endif
    return true;
}