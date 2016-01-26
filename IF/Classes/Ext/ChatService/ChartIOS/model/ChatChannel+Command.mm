//
//  ChatChannel+Command.m
//  IF
//
//  Created by 张彦涛 on 15/12/24.
//
//

#import "ChatChannel+Command.h"
#import <string.h>
#import "GetMsgBySeqIdCommand.h"
@implementation ChatChannel (Command)

//int minSeq = [cc gettingChatChannelMinSeqId];
//int maxSeq = [cc gettingChatChannelMaxSeqId];
//DVLog(@"channel：%@  最小seq%d 最大seq%d",cc.channelID,minSeq,maxSeq);
//if ((maxSeq <= cc.serverMaxSeqId && minSeq >= cc.serverMinSeqId) || minSeq != 0) {
//    GetMsgBySeqIdCommand* cmd = new GetMsgBySeqIdCommand(minSeq,maxSeq,cc.channelType,[cc.channelID UTF8String]);
//    cmd->sendAndRelease();
//    return TRUE;
//}else{
//    return FALSE;

//}

-(BOOL)gettingServerOldMsgForCountryWithMinSeq:(long)vMinSeq andWithMaxSeq:(long)vMaxSeq {
    if  ((vMaxSeq > vMinSeq) && (vMaxSeq > 0) && (vMinSeq > 0)){
        int  count  = vMaxSeq - vMinSeq;
        NSString *channelID = self.channelID;
        int channeltype = self.channelType;
        GetMsgBySeqIdCommand* cmd = new GetMsgBySeqIdCommand(vMinSeq,vMaxSeq,channeltype,[channelID UTF8String]);
        cmd->sendAndRelease();
        return true;
    }else{
        return false;
    }
}

@end
