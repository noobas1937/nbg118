//
//  ChatInfo.cpp
//  IF
//
//  Created by fubin on 13-9-26.
//
//

#include "ChatInfo.h"

ChatInfo::ChatInfo(CCDictionary* dict)
{
    if(dict->valueForKey("sendLocalTime"))
    {
        sendLocalTime=dict->valueForKey("sendLocalTime")->getCString();
        CCLOG("ChatInfo sendLocalTime %s",sendLocalTime.c_str());
    }
    else
        sendLocalTime="";
    
    uid = dict->valueForKey("senderUid")->getCString();
    type = dict->valueForKey("type")->intValue();
    name = dict->valueForKey("senderName")->getCString();
    msg = dict->valueForKey("msg")->getCString();
    time = dict->valueForKey("time")->doubleValue()/1000;
    senderPic = dict->valueForKey("senderPic")->getCString();
    asn = dict->valueForKey("asn")->getCString();
    vip = dict->valueForKey("vip")->intValue();
    translationMsg = dict->valueForKey("translationMsg")->getCString();
    originalLang = dict->valueForKey("originalLang")->getCString();
    post = dict->valueForKey("post")->intValue();
    isNew = true;
    gmFlag = dict->valueForKey("gmFlag")->intValue();
    banTime = dict->valueForKey("banTime")->doubleValue()/1000;
    banTime = GlobalData::shared()->changeTime(banTime);
    banGmName = dict->valueForKey("banGmName")->getCString();
    officer = dict->valueForKey("officer")->getCString();
    allianceId = dict->valueForKey("allianceId")->getCString();
    //comment用
    if(dict->objectForKey("message")){
        msg = dict->valueForKey("message")->getCString();
    }
    if(dict->objectForKey("abbr")){
        asn = dict->valueForKey("abbr")->getCString();
    }
    if(dict->objectForKey("icon")){
        senderPic = dict->valueForKey("icon")->getCString();
    }
    if(dict->objectForKey("translate")){
        translationMsg = dict->valueForKey("translate")->getCString();
    }
    if(dict->objectForKey("name")){
        name = dict->valueForKey("name")->getCString();
    }
    if(dict->objectForKey("lang")){
        originalLang = dict->valueForKey("lang")->getCString();
    }
    if(dict->objectForKey("senderAllianceId")){
        senderAllianceId = dict->valueForKey("senderAllianceId")->getCString();
    }
    if(dict->objectForKey("type")){
        stype = dict->valueForKey("type")->intValue();
    }
    if (dict->objectForKey("senderPicVer")){
        senderPicVer = dict->valueForKey("senderPicVer")->intValue();
    }
    if(dict->objectForKey("seqId"))
    {
        sequenceId=dict->valueForKey("seqId")->intValue();
    }
    if(dict->objectForKey("lastUpdateTime"))
    {
        lastUpdateTime=dict->valueForKey("lastUpdateTime")->intValue();
    }
    
    //战报分享
    reportAtt = dict->valueForKey("reportAtt")->getCString();
    reportDef = dict->valueForKey("reportDef")->getCString();
    reportLang = dict->valueForKey("reportLang")->getCString();
    reportUid = dict->valueForKey("reportUid")->getCString();
    if (dict->objectForKey("reportAttAbbr")) {
        reportAttAbbr = dict->valueForKey("reportAttAbbr")->getCString();
    }
    if (dict->objectForKey("reportDefAbbr")) {
        reportDefAbbr = dict->valueForKey("reportDefAbbr")->getCString();
    }
    if(post==CHAT_TYPE_BATTLE_SHARE){
        if (reportLang=="115277" || reportLang=="115278" || reportLang=="115283") {
            if (reportDefAbbr != "") {
                msg = _lang_1(reportLang, (std::string("(" + reportDefAbbr + ")" + reportDef)).c_str());
            } else {
                msg = _lang_1(reportLang,reportDef.c_str());
            }
        }else{
            if (reportAttAbbr != "") {
                msg = _lang_1(reportLang, (std::string("(" + reportAttAbbr + ")" + reportAtt)).c_str());
            } else {
                msg = _lang_1(reportLang,reportAtt.c_str());
            }
        }
    }
    //侦查分享
    if (post == CHAT_TYPE_DETECT_SHARE) {
        if (dict->objectForKey("reportUid")) {
            detectRepUid = dict->valueForKey("reportUid")->getCString();
        }
    }
    //装备分享
    if (post == CHAT_TYPE_EQUIP_SHARE) {
        CCLOGFUNCF("equip msg:%s",msg.c_str());
        if (dict->objectForKey("equipId")) {
            equipId  = dict->valueForKey("equipId")->intValue();
            CCLOGFUNCF("equipId:%d",equipId);
        }
    }
    
    //客服公告
    if(uid.compare(CHAT_NOTICE_SYS_DIALOG) == 0){
        name = _lang(uid);
    }
    //大小喇叭使用道具 用于界面展示
    itemId = dict->valueForKey("itemId")->intValue();
    
    showOriginal = false;
	isTimeShowed = false;
}