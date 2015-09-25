//
//  MailDalogInfo.cpp
//  IF
//
//  Created by lifangkai on 14-9-19.
//
//

#include "MailDialogInfo.h"
#include "MailInfo.h"
#include "MailController.h"

MailDialogInfo* MailDialogInfo::create(){
    MailDialogInfo* pret = new MailDialogInfo();
    if(pret){
        pret->init();
        pret->autorelease();
    }else{
        CC_SAFE_DELETE(pret);
    }
    return pret;
}
//add one dialog
void MailDialogInfo::parse(CCDictionary *dic){
    
    type = dic->valueForKey("type")->intValue();
    if(type==CHAT_ROOM)
        uid = dic->valueForKey("fromUid")->getCString();
    else
        uid = dic->valueForKey("uid")->getCString();
    fromUid = dic->valueForKey("fromUid")->getCString();
    toUid = dic->valueForKey("toUid")->getCString();
    contents = dic->valueForKey("contents")->getCString();
    if(type==MAIL_Alliance_ALL){
        contents=_lang("105589")+contents;
    }
    createTime =  dic->valueForKey("createTime")->doubleValue()/1000;
    pic = dic->valueForKey("pic")->getCString();
    picVer=dic->valueForKey("picVer")->intValue();
    if(dic->objectForKey("translationMsg")){
        translationMsg = dic->valueForKey("translationMsg")->getCString();
    }
    if(dic->objectForKey("originalLang")){
        originalLang = dic->valueForKey("originalLang")->getCString();
    }
    
    if(dic->objectForKey("sendLocalTime")){
        sendLocalTime = dic->valueForKey("sendLocalTime")->getCString();
    }
    else
    {
        sendLocalTime="";
    }
    if(dic->objectForKey("title")){
        title = dic->valueForKey("title")->getCString();
    }
    if(dic->objectForKey("seqId")){
        sequenceId = dic->valueForKey("seqId")->intValue();
    }

    if(dic->objectForKey("lastUpdateTime")){
        lastUpdateTime = dic->valueForKey("lastUpdateTime")->intValue();
    }
    
    if(dic->objectForKey("tipFlag")){
        tipFlag = dynamic_cast<CCInteger*>(dic->objectForKey("tipFlag"))->getValue();
    }
    
    if(dic->objectForKey("fromName"))
    {
        senderName = dic->valueForKey("fromName")->getCString();
    }
    
    if(dic->objectForKey("vip"))
    {
        vip = dic->valueForKey("vip")->intValue();
    }
    
    if(dic->objectForKey("vipEndTime"))
    {
        vipEndTime = dic->valueForKey("vipEndTime")->longValue();
    }
    
    if(dic->objectForKey("asn"))
    {
        asn = dic->valueForKey("asn")->getCString();
    }
    
    if(dic->objectForKey("allianceId"))
    {
        allianceId = dic->valueForKey("allianceId")->getCString();
    }
    if(dic->objectForKey("status"))
    {
        status = dic->valueForKey("status")->intValue();
    }
    
    
}
//add one message
void MailDialogInfo::addOneDialog(CCDictionary* dic){

}
MailDialogInfo::~MailDialogInfo(){

}