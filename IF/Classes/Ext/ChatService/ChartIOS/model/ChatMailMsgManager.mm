//
//  ChatMailMsgManager.m
//  IF
//
//  Created by 张彦涛 on 15/8/28.
//
//

#import "ChatMailMsgManager.h"
#import "ServiceInterface.h"
#import <string.h>

@implementation ChatMailMsgManager
+(NSMsgItem *)chatMsgWithChatMailInfo:(ChatMailInfo *)vMailInfo{
    NSMsgItem *msgItem = [[NSMsgItem alloc]init];
    msgItem.uid =  [NSString stringWithUTF8String:vMailInfo->uid.c_str()];
    msgItem.vip = [NSString stringWithUTF8String:vMailInfo->vip.c_str()];
    if (vMailInfo->channelMsgType == IOS_CHANNEL_TYPE_CHATROOM) {
        msgItem.vip  = [@"VIP" stringByAppendingString:msgItem.vip ];
    }
    
    msgItem.name = [NSString stringWithUTF8String:vMailInfo->name.c_str()];
    msgItem.asn = [NSString stringWithUTF8String:vMailInfo->asn.c_str()];
    
    msgItem.msg = [NSString stringWithUTF8String:vMailInfo->msg.c_str()];
   msgItem.translateMsg= [NSString stringWithUTF8String:vMailInfo->translateMsg.c_str()];
//    ns_msg= [ns_msg stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
//    ns_translateMsg = [ns_translateMsg stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
    msgItem.headPic = [NSString stringWithUTF8String:vMailInfo->headPic.c_str()];
    NSString * _sendLocalTime = [NSString stringWithUTF8String:vMailInfo->sendLocalTime.c_str()];
    msgItem.sendLocalTime = [_sendLocalTime intValue];;
    msgItem.originalLang = [NSString stringWithUTF8String:vMailInfo->originalLang.c_str()];
    msgItem.mailId = [NSString stringWithUTF8String:vMailInfo->id.c_str()];
    
    
    msgItem.createTime = vMailInfo->createTime;
    msgItem.isNewMsg = vMailInfo->isNewMsg;
    msgItem.isSelfMsg = vMailInfo->isSelfMsg;
    msgItem.channelType = vMailInfo->channelMsgType;
    msgItem.gmod = vMailInfo->gmod;
    msgItem.post = vMailInfo->post;
    msgItem.headPicVer = vMailInfo->headPicVer;
    msgItem.sequenceId =  vMailInfo->sequenceId;
    NSLog(@"sequenceId = %d",msgItem.sequenceId);
    msgItem.lastUpdateTime = vMailInfo->lastUpdateTime;
    
    
    [msgItem packedMsg];

    
    return msgItem;
}
@end
