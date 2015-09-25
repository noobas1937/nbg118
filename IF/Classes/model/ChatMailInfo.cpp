//
//  ChatMailInfo.cpp
//  IF
//
//  Created by xupengzhan on 15-4-2.
//
//

#include "ChatMailInfo.h"
#include "HFUrlImageCache.h"
#include "VipUtil.h"
#include "EquipmentController.h"

ChatMailInfo::ChatMailInfo():
id("")
,isNewMsg(true),
channelMsgType(-1),
msg(""),
translateMsg(""),
time(""),
fromName(""),
channelId(""),
name(""),
vip(""),
headPic(""),
isSelfMsg(false),
uid(""),
asn(""),
post(-1),
gmod(0),
allianceId(""),
reportUid(""),
detectReportUid(""),
originalLang(""),
hornItemId(-1),
headPicVer(0),
customHeadPic(""),
isCustomHeadPicExist(false),
customHeadPicUrl(""),
sendLocalTime(""),
sequenceId(0),
lastUpdateTime(0),
equipId(0)
{}

ChatMailInfo* ChatMailInfo::create(ChatInfo chatInfo, bool isNewMsg)
{
    ChatMailInfo* ret=new ChatMailInfo(chatInfo,isNewMsg);
    if(ret && ret->init())
    {
        ret->autorelease();
    }
    else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ChatMailInfo* ChatMailInfo::create(MailInfo* mailInfo,MailDialogInfo* mailDialogInfo,bool isNew,int msgType/*=CHANNEL_TYPE_USER*/)
{
    ChatMailInfo* ret=new ChatMailInfo(mailInfo,mailDialogInfo,isNew,msgType);
    if(ret && ret->init())
    {
        ret->autorelease();
    }
    else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ChatMailInfo::~ChatMailInfo()
{
}

ChatMailInfo::ChatMailInfo(ChatInfo info,bool isNew)
{
    CCLOG("ChatMailInfo(ChatInfo info,bool isNew)");
    headPic="";
    //头像
    if(info.senderPic=="" || info.senderPic=="0"){
        headPic="g044";
    }else{
        headPic = info.senderPic;
    }
    headPicVer = info.senderPicVer;
    
    CCLOG("headPicVer :%d",headPicVer);
    
    vip="";
    //vip
    if (info.vip > 0) {
        vip=_lang_1("103001", CC_ITOA(info.vip));
    }
    
    //时间
   time=CCCommonUtils::timeStampToDHM(info.time);
    createTime=info.time;
    isSelfMsg=false;
    //是自己的信息还是别人的信息标识
    if (info.uid == GlobalData::shared()->playerInfo.uid) {
        isSelfMsg=true;
    }
    CCLOGFUNCF("info.type:%d",info.type);
    isNewMsg=isNew;
    if(info.type==0)
        channelMsgType=CHANNEL_TYPE_COUNTRY;
    else if (info.type==2)
        channelMsgType=CHANNEL_TYPE_ALLIANCE;
    msg=info.msg;
    translateMsg=info.translationMsg;
    name=info.name;
    uid=info.uid;
    asn=info.asn;
    post=info.post;
    gmod=info.gmFlag;
    allianceId=info.allianceId;
    reportUid=info.reportUid;
    equipId = info.equipId;
    detectReportUid=info.detectRepUid;
    originalLang=info.originalLang;
    hornItemId=info.itemId;
    sendLocalTime=info.sendLocalTime;
    sequenceId=info.sequenceId;
    lastUpdateTime=info.lastUpdateTime;
    if(post == CHAT_TYPE_EQUIP_SHARE && equipId>0)
    {
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[equipId];
//        string nameStr = CCCommonUtils::getNameById(equipId);
//        string colorStr = CCCommonUtils::getPropById(equipId, "color");
        string nameStr = _lang(eInfo.name) ;
        string colorStr = CC_ITOA(eInfo.color);
        CCLOGFUNCF("nameStr:%s",nameStr.c_str());
        if (nameStr!="" && colorStr!="" ) {
            msg =colorStr.append("|").append(nameStr);
        }
        CCLOGFUNCF("msg:%s",msg.c_str());
    }
    
    if(uid!="" &&  CCCommonUtils::isUseCustomPic(headPicVer))
    {
        customHeadPicUrl=CCCommonUtils::getCustomPicUrl(uid, headPicVer);
        if(customHeadPicUrl!="")
        {
            CCLOG("customHeadPicUrl %s",customHeadPicUrl.c_str());
            customHeadPic=HFUrlImageCache::shared()->getCachePath(customHeadPicUrl);
            CCLOG("customHeadPic %s",customHeadPic.c_str());
            isCustomHeadPicExist=CCFileUtils::sharedFileUtils()->isFileExist(customHeadPic);
        }
    }
}

ChatMailInfo::ChatMailInfo(MailInfo* mailInfo,MailDialogInfo* mailDialogInfo,bool isNew,int msgType/*=CHANNEL_TYPE_USER*/)
{
    //将时间转换为day hour min
    time = CCCommonUtils::timeStampToDHM(mailDialogInfo->createTime);
    createTime=mailDialogInfo->createTime;

    isSelfMsg=false;
    
    bool isSelf=false;
    if (mailDialogInfo->fromUid!="") {
        isSelf=mailDialogInfo->fromUid==GlobalData::shared()->playerInfo.uid;
    }
    
    id=mailDialogInfo->uid;
    vip="";
    asn="";
    if(mailDialogInfo->type==MAIL_SELF_SEND || mailDialogInfo->type==MAIL_MOD_PERSONAL || isSelf)
    {
        isSelfMsg=true;
        headPic = GlobalData::shared()->playerInfo.pic;
        headPicVer = GlobalData::shared()->playerInfo.picVer;
        name = GlobalData::shared()->playerInfo.name;
        uid=GlobalData::shared()->playerInfo.uid;
        gmod=GlobalData::shared()->playerInfo.gmFlag;
        allianceId=GlobalData::shared()->playerInfo.allianceInfo.uid;
        asn=GlobalData::shared()->playerInfo.allianceInfo.shortName;
        if (GlobalData::shared()->playerInfo.vipEndTime > GlobalData::shared()->getWorldTime())
            vip=CC_ITOA(VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints));
    }
    else
    {
        uid=mailInfo->fromUid;
        fromName = mailInfo->fromName;
        
        if(mailDialogInfo->type==MAIL_Alliance_ALL){
            channelId = mailInfo->fromUid;
        }
        
        if(mailDialogInfo->type!=CHAT_ROOM)
        {
            
            headPic = mailInfo->pic;
            headPicVer = mailInfo->picVer;
            name= mailInfo->nowName;
            allianceId=mailInfo->allianceId;
            
        }
        else
        {
            if(mailDialogInfo->type==CHAT_ROOM)
            {
                if(!mailInfo->crGroupId.empty()){
                    channelId = mailInfo->crGroupId;
                }
            }
            uid = mailDialogInfo->fromUid;
            id="";
            headPic = mailDialogInfo->pic;
            headPicVer = mailDialogInfo->picVer;
            name= mailDialogInfo->senderName;
            allianceId=mailDialogInfo->allianceId;
            asn=mailDialogInfo->asn;
            if (mailDialogInfo->vipEndTime > GlobalData::shared()->getWorldTime())
                vip=CC_ITOA(mailDialogInfo->vip);
        }
        gmod=0;
        
    }
    
    if(uid!="" &&  CCCommonUtils::isUseCustomPic(headPicVer))
    {
        customHeadPicUrl=CCCommonUtils::getCustomPicUrl(uid, headPicVer);
        
        if(customHeadPicUrl!="")
        {
            CCLOG("customHeadPicUrl %s",customHeadPicUrl.c_str());
            customHeadPic=HFUrlImageCache::shared()->getCachePath(customHeadPicUrl);
            CCLOG("customHeadPic %s",customHeadPic.c_str());
            isCustomHeadPicExist=CCFileUtils::sharedFileUtils()->isFileExist(customHeadPic);
        }
    }
    
    detectReportUid="";
    if(mailInfo->detectReport)
    {
        CCString* detectUidStr=dynamic_cast<CCString*>(mailInfo->detectReport->objectForKey("id"));
        if(detectUidStr)
            detectReportUid=detectUidStr->getCString();
    }
    
    
    isNewMsg=isNew;
    msg=mailDialogInfo->contents;
    channelMsgType=msgType;
    translateMsg=mailDialogInfo->translationMsg;
    
    if(mailDialogInfo->type == MAIL_MOD_SEND || mailDialogInfo->type==MAIL_MOD_PERSONAL)
    {
        post = 200;
    }
    else
    {
        if(mailDialogInfo->tipFlag==1)
            post=100;
        else
            post=0;
    }
    
    reportUid=mailInfo->reportUid;
    originalLang=mailDialogInfo->originalLang;
    sendLocalTime=mailDialogInfo->sendLocalTime;
    sequenceId=mailDialogInfo->sequenceId;
    lastUpdateTime=mailDialogInfo->lastUpdateTime;
}
