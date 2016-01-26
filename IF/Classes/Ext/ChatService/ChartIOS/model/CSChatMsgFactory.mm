//
//  CSChatMsgFactory.m
//  IF
//
//  Created by 张彦涛 on 15/10/13.
//
//

#import "CSChatMsgFactory.h"
#import "ChatMailInfo.h"
#import "CSMessage.h"
#import "ServiceInterface.h"
#import "UserManager.h"
@implementation CSChatMsgFactory
+(NSMsgItem *)chatMsgWithChatMailInfo:(ChatMailInfo *)vChatMailInfo{
    
    NSString * ns_uid = [NSString stringWithUTF8String:vChatMailInfo->uid.c_str()];
    
    NSString * ns_vip = [NSString stringWithUTF8String:vChatMailInfo->vip.c_str()];
    if (vChatMailInfo->channelMsgType == IOS_CHANNEL_TYPE_CHATROOM) {
        ns_vip = [@"VIP" stringByAppendingString:ns_vip];
    }
    
    NSString * ns_name = [NSString stringWithUTF8String:vChatMailInfo->name.c_str()];
    NSString * ns_asn = [NSString stringWithUTF8String:vChatMailInfo->asn.c_str()];
    
    NSString * ns_msg = [NSString stringWithUTF8String:vChatMailInfo->msg.c_str()];
    NSString * ns_translateMsg = [NSString stringWithUTF8String:vChatMailInfo->translateMsg.c_str()];
    ns_msg= [ns_msg stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
    ns_translateMsg = [ns_translateMsg stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
    NSString * ns_headPic = [NSString stringWithUTF8String:vChatMailInfo->headPic.c_str()];
    NSString * ns_sendLocalTime = [NSString stringWithUTF8String:vChatMailInfo->sendLocalTime.c_str()];
    NSString * ns_originalLang = [NSString stringWithUTF8String:vChatMailInfo->originalLang.c_str()];
    NSString * ns_mailID = [NSString stringWithUTF8String:vChatMailInfo->id.c_str()];
    
    int sendStatus = 2;
    NSInteger ns_createTime = vChatMailInfo->createTime;
    bool isNewMsg = vChatMailInfo->isNewMsg;
    bool isSelfMsg = vChatMailInfo->isSelfMsg;
    int ns_channelType = vChatMailInfo->channelMsgType;
    int gmod = vChatMailInfo->gmod;
    int post = vChatMailInfo->post;
    int headPicVer = vChatMailInfo->headPicVer;
    int sequenceId = vChatMailInfo->sequenceId;
//    DLog(@"sequenceId = %d",sequenceId);
    NSInteger lastUpdateTime = vChatMailInfo->lastUpdateTime;
    long createTime = vChatMailInfo->createTime;
    
    NSString *ns_attachmentId = nil;
    if(post == 3){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->allianceId.c_str()];
    }else if(post == 4){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->reportUid.c_str()] ;
    }else if(post == 5){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->detectReportUid.c_str()];
    }else if(post == 7){
        if (vChatMailInfo->equipId > 0) {
            CCLog("%s",vChatMailInfo->msg.c_str());
            ns_attachmentId = [NSString stringWithFormat:@"%d",vChatMailInfo->equipId];
//            DLog(@"ns_attachmentId : %@",ns_attachmentId);
        }else{
            ns_attachmentId = @"0";
        }
        
    }else if(post == 9){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->teamUid.c_str()];
    }else if(post == 10){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->lotteryInfo.c_str()];
    }else if(post == 12){
        NSString *ns_redPacketsId = [NSString stringWithUTF8String:vChatMailInfo->redPackets.c_str()];
        NSString *ns_server = [NSString stringWithFormat:@"%d",vChatMailInfo->server];
        
        ns_attachmentId = [NSString stringWithFormat:@"%@_%@",ns_redPacketsId,ns_server];
        
        if (isSelfMsg){
            ns_attachmentId = [NSString stringWithFormat:@"%@|0",ns_attachmentId];
        }else{
            ns_attachmentId = [NSString stringWithFormat:@"%@|1",ns_attachmentId];
        }
    }
    
    
    /**创建信息模型*/
    NSMsgItem *cm = [[NSMsgItem alloc]init];
    
    cm.uid = ns_uid;
    cm.vip = ns_vip;
    cm.createTime = ns_createTime;
    cm.name = ns_name;
    cm.asn = ns_asn;
    cm.msg = ns_msg;
    cm.translateMsg = ns_translateMsg;
    cm.headPic = ns_headPic;
    cm.sendState = sendStatus;
    if (vChatMailInfo->channelMsgType == IOS_CHANNEL_TYPE_USER && ns_headPic.length== 0){
        cm.headPic = @"g015";
    }
    cm.originalLang = ns_originalLang;
    cm.attachmentId = ns_attachmentId;
    cm.isNewMsg = isNewMsg;
    cm.isSelfMsg = isSelfMsg;
    cm.channelType = ns_channelType;
    cm.gmod = gmod;
    cm.post = post;
    cm.headPicVer = headPicVer;
    cm.sequenceId = sequenceId;
    cm.mailId = ns_mailID;
    cm.lastUpdateTime = lastUpdateTime;
    cm.createTime = createTime;
    cm.sendLocalTime = [ns_sendLocalTime longLongValue];

    
    return cm ;
}

+(CSMessage *)chatMessageWithChatMailInfo:(ChatMailInfo *)vChatMailInfo{
    
    NSString * ns_uid = [NSString stringWithUTF8String:vChatMailInfo->uid.c_str()];
    
    NSString * ns_vip = [NSString stringWithUTF8String:vChatMailInfo->vip.c_str()];
    if (vChatMailInfo->channelMsgType == IOS_CHANNEL_TYPE_CHATROOM) {
        ns_vip = [@"VIP" stringByAppendingString:ns_vip];
    }
    
    NSString * ns_name = [NSString stringWithUTF8String:vChatMailInfo->name.c_str()];
    NSString * ns_asn = [NSString stringWithUTF8String:vChatMailInfo->asn.c_str()];
    
    NSString * ns_msg = [NSString stringWithUTF8String:vChatMailInfo->msg.c_str()];
    NSString * ns_translateMsg = [NSString stringWithUTF8String:vChatMailInfo->translateMsg.c_str()];
    ns_msg= [ns_msg stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
    ns_translateMsg = [ns_translateMsg stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
    NSString * ns_headPic = [NSString stringWithUTF8String:vChatMailInfo->headPic.c_str()];
    NSString * ns_sendLocalTime = [NSString stringWithUTF8String:vChatMailInfo->sendLocalTime.c_str()];
    NSString * ns_originalLang = [NSString stringWithUTF8String:vChatMailInfo->originalLang.c_str()];
    NSString * ns_mailID = [NSString stringWithUTF8String:vChatMailInfo->id.c_str()];
    
    int sendStatus = 2;
    NSInteger ns_createTime = vChatMailInfo->createTime;
    bool isNewMsg = vChatMailInfo->isNewMsg;
    bool isSelfMsg = vChatMailInfo->isSelfMsg;
    int ns_channelType = vChatMailInfo->channelMsgType;
    int gmod = vChatMailInfo->gmod;
    int post = vChatMailInfo->post;
    int headPicVer = vChatMailInfo->headPicVer;
    int sequenceId = vChatMailInfo->sequenceId;
    //    DLog(@"sequenceId = %d",sequenceId);
    NSInteger lastUpdateTime = vChatMailInfo->lastUpdateTime;
    long createTime = vChatMailInfo->createTime;
    
    NSString *ns_attachmentId = nil;
    if(post == 3){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->allianceId.c_str()];
    }else if(post == 4){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->reportUid.c_str()] ;
    }else if(post == 5){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->detectReportUid.c_str()];
    }else if(post == 7){
        if (vChatMailInfo->equipId > 0) {
            CCLog("%s",vChatMailInfo->msg.c_str());
            ns_attachmentId = [NSString stringWithFormat:@"%d",vChatMailInfo->equipId];
            //            DLog(@"ns_attachmentId : %@",ns_attachmentId);
        }else{
            ns_attachmentId = @"0";
        }
        
    }else if(post == 9){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->teamUid.c_str()];
    }else if(post == 10){
        ns_attachmentId = [NSString stringWithUTF8String:vChatMailInfo->lotteryInfo.c_str()];
    }else if(post == 12){
        NSString *ns_redPacketsId = [NSString stringWithUTF8String:vChatMailInfo->redPackets.c_str()];
        NSString *ns_server = [NSString stringWithFormat:@"%d",vChatMailInfo->server];
        
        ns_attachmentId = [NSString stringWithFormat:@"%@_%@",ns_redPacketsId,ns_server];
        
        if (isSelfMsg){
            ns_attachmentId = [NSString stringWithFormat:@"%@|0",ns_attachmentId];
        }else{
            ns_attachmentId = [NSString stringWithFormat:@"%@|1",ns_attachmentId];
        }
    }
    
    
    /**创建信息模型*/
    CSMessage *cm = [[CSMessage alloc]init];
    
    cm.uid = ns_uid;
//    cm.vip = ns_vip;
    cm.createTime = ns_createTime;
//    cm.name = ns_name;
//    cm.asn = ns_asn;
    cm.msg = ns_msg;
    cm.translateMsg = ns_translateMsg;
//    cm.headPic = ns_headPic;
    cm.sendState = sendStatus;
    if (vChatMailInfo->channelMsgType == IOS_CHANNEL_TYPE_USER && ns_headPic.length== 0){
//        cm.headPic = @"g015";
    }
    cm.originalLang = ns_originalLang;
    cm.attachmentId = ns_attachmentId;
//    cm.isNewMsg = isNewMsg;
//    cm.isSelfMsg = isSelfMsg;
    cm.channelType = ns_channelType;
//    cm.gmod = gmod;
    cm.post = post;
//    cm.headPicVer = headPicVer;
    cm.sequenceId = sequenceId;
    cm.mailId = ns_mailID;
//    cm.lastUpdateTime = lastUpdateTime;
    cm.createTime = createTime;
    cm.sendLocalTime = [ns_sendLocalTime longLongValue];
    cm.translatedLang = [NSString stringWithUTF8String: LocalController::shared()->getLanguageFileName().c_str()] ;
    
    if  (cm.channelType == IOS_CHANNEL_TYPE_COUNTRY || cm.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
        cm.readState =1;
    }else if(cm.channelType  == IOS_CHANNEL_TYPE_CHATROOM ){
        if ([cm.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
            cm.readState = 1;
        }else{
            cm.readState = 0;
        }
        
    }else{
//        cm.readState = vChatMailInfo->readStatus; Simon
    }
    
    
    return cm ;
}
@end
