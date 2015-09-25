//
//  MailPush.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-28.
//
//

#include "MailPush.h"
#include "MailController.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "GCMRewardController.h"
#include "MailPopUpView.h"
#include "ToolController.h"
#include "ChatServiceCocos2dx.h"
//接收新邮件
void MailPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* result = dynamic_cast<cocos2d::CCDictionary*>(dict->objectForKey("params"));
    if (result->objectForKey("reward")) {
        cocos2d::CCDictionary* resultReward = dynamic_cast<cocos2d::CCDictionary*>(result->objectForKey("reward"));
        GCMRewardController::getInstance()->retReward(resultReward);
    }
    int type = result->valueForKey("type")->intValue();
    string fromUid = result->valueForKey("fromUid")->getCString();
    string mailId = result->valueForKey("uid")->getCString();

    if(type==MAIL_USER||type==MAIL_SELF_SEND||type==MAIL_Alliance_ALL||type==MAIL_MOD_SEND||type==MAIL_MOD_PERSONAL){
        bool isChat = false;
        map<string, MailInfo*>::iterator it;
        for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
            if(type == MAIL_MOD_PERSONAL || type == MAIL_MOD_SEND){
                if((it->second->type == MAIL_MOD_PERSONAL || it->second->type == MAIL_MOD_SEND) && it->second->fromUid==fromUid){
                    MailController::getInstance()->addOneDialogToMailEnd(result,it->second->uid);
                    isChat = true;
                    break;
                }
            }else{
                if(it->second->tabType==0 && it->second->fromUid==fromUid){
                    if(it->second->type != MAIL_MOD_PERSONAL && it->second->type != MAIL_MOD_SEND){
                        MailController::getInstance()->addOneDialogToMailEnd(result,it->second->uid);
                        isChat = true;
                        break;
                    }
                }
            }
        }
        if(!isChat){
            MailController::getInstance()->pushMailDialogFirst(result);
        }
    }else if(type==MAIL_RESOURCE){
        //添加装备那边的掉落物品
        CCArray* arr = dynamic_cast<CCArray*>(result->objectForKey("collect"));
        if(arr){
            int index = 0;
            while (index<arr->count()) {
                CCDictionary* tempDic = _dict(arr->objectAtIndex(index));
                if(tempDic && tempDic->objectForKey("drop")){
                    CCArray *drop = dynamic_cast<CCArray*>(tempDic->objectForKey("drop"));
                    int index0 = 0;
                    while (index0<drop->count()) {
                        auto dict = dynamic_cast<CCDictionary*>(drop->objectAtIndex(index0));
                        int type = dict->valueForKey("type")->intValue();
                        if(type == R_GOODS){
                            CCDictionary *goodsDict = _dict(dict->objectForKey("value"));
                            int itemId = goodsDict->valueForKey("itemId")->intValue();
                            int num = goodsDict->valueForKey("count")->intValue();
                            string uuid = goodsDict->valueForKey("uuid")->getCString();
                            auto &info = ToolController::getInstance()->getToolInfoById(itemId);
                            ToolController::getInstance()->addTool(itemId, num, uuid);
                            break;
                        }
                        ++index0;
                    }
                }
                ++index;
            }
        }
        
        //
        bool isChat = false;
        map<string, MailInfo*>::iterator it;
        for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
            if(it->second->type==MAIL_RESOURCE){
                MailController::getInstance()->addOneResourceToMail(result,it->second->uid);
                isChat = true;
                break;
            }
        }
        if(!isChat){
            MailController::getInstance()->pushMailResourceFirst(result);
        }
    
    }else if(type==MAIL_RESOURCE_HELP){
        bool isChat = false;
        map<string, MailInfo*>::iterator it;
        for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
            if(it->second->type==MAIL_RESOURCE_HELP){
                MailController::getInstance()->addOneResourceHelpToMail(result,it->second->uid);
                isChat = true;
                break;
            }
        }
        if(!isChat){
            MailController::getInstance()->pushMailResourceHelpFirst(result);
        }
        
    }else if(type==MAIL_ATTACKMONSTER){
        bool isChat = false;
        map<string, MailInfo*>::iterator it;
        for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
            if(it->second->type==MAIL_ATTACKMONSTER){
                MailController::getInstance()->addOneMonsterToMail(result,it->second->uid);
                isChat = true;
                break;
            }
        }
        if(!isChat){
            MailController::getInstance()->pushMailMonsterFirst(result);
        }
        
    }else{
        MailController::getInstance()->addMail(result);
    }
    
    if(MailController::getInstance()->getIsNewMailListEnable() && type!=MAIL_USER && type!=MAIL_SELF_SEND && type!=MAIL_Alliance_ALL&&  type!=MAIL_MOD_SEND && type!=MAIL_MOD_PERSONAL)
    {
        CCArray* mailDataArr=CCArray::create();
        int minCreateTime[6]={-1,-1,-1,-1,-1,-1};
       
        
        if(result->objectForKey("contentsLocal"))
        {
            CCLOGFUNCF("contentsLocal %s",result->valueForKey("contentsLocal")->getCString());
            result->setObject(CCString::create(result->valueForKey("contentsLocal")->getCString()), "contents");
        }
        
        if(result->objectForKey("contentsArr"))
        {
            CCArray* contentsArr = dynamic_cast<CCArray*>(result->objectForKey("contentsArr"));
            if(contentsArr)
            {
                std::string actualContent = "";
                CCLOGFUNCF("contentsArr size %d",contentsArr->count());
                for (int i=0; i<contentsArr->count(); i++) {
                    CCString* content = dynamic_cast<CCString*>(contentsArr->objectAtIndex(i));
                    if (content) {
                        CCLOGFUNCF("content I :%s",content->getCString());
                        actualContent.append(content->getCString());
                    }
                }
                CCLOGFUNCF("actualContent :%s",actualContent.c_str());
                result->setObject(CCString::create(actualContent), "contents");
            }
        }
        
        
        MailInfo* mailInfo=MailInfo::create();
        mailInfo->parse(result);
        
        //                        CCLOGFUNCF("CREATE TIME: %d  tabType：%d",mailInfo->createTime,mailInfo->tabType);
        if(minCreateTime[mailInfo->tabType]==-1)
            minCreateTime[mailInfo->tabType]=mailInfo->createTime;
        else if(mailInfo->createTime<minCreateTime[mailInfo->tabType])
            minCreateTime[mailInfo->tabType]=mailInfo->createTime;
        
        CCLOGFUNCF("CHANNEL_TYPE_OFFICIAL mail: type:%d  contents:%s",mailInfo->type,result->valueForKey("contents")->getCString());
        mailDataArr->addObject(result);
        
        if (mailDataArr->count()>0) {
            int index=MailController::getInstance()->getCurNotifyMailDataIndex();
            MailController::getInstance()->m_mailDataDic->setObject(mailDataArr, index);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            ChatServiceCocos2dx::notifyMailDataIndex(index, false);
            MailController::getInstance()->setCurNotifyMailDataIndex(index+1);
#endif
        }
    }
    
    if (type == MAIL_BATTLE_REPORT)
    {
        auto oneMail = GlobalData::shared()->mailList.find(mailId);
        if(oneMail!=GlobalData::shared()->mailList.end())
        {
            oneMail->second->isReadContent = true;
            if (result->objectForKey("fightContents")) {
                auto dict = _dict(result->objectForKey("fightContents"));
                MailController::getInstance()->refreshMailContent(mailId, dict);
                MailController::getInstance()->showMailTips(mailId);
            }
        }
    }
    if (type==MAIL_ATTACKMONSTER) {
        MailController::getInstance()->showMailTips(CC_ITOA(MAIL_ATTACKMONSTER));
    }
    
    return;
}