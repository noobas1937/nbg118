//
//  CSCommandManager.m
//  IF
//
//  Created by 张彦涛 on 15/10/25.
//
//

#import "CSCommandManager.h"
#import "GetNewSystemMailMsg_iOS_Command.h"
#include "MailReadCommand.h"
#include "MailDeleteCommand.h"
#import <string.h>
#import "MailInfoDataModel.h"
#include "MailBatchCommand.h"
#include "GetNewMailMsgCommand.h"
#import "NSMsgItem.h"
#import "ChatChannel.h"
//#import "CSShowTimeTool.h"
#import "NetController.h"
@interface CSCommandManager ()

@property(nonatomic,strong) NSOperationQueue *mailCommandQueue;

@end
@implementation CSCommandManager

+ (CSCommandManager *)sharedCommandManager
{
    static CSCommandManager *sharedCommandManager = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedCommandManager = [[self alloc] init];
    });
    return sharedCommandManager;
}



-(NSOperationQueue *)mailCommandQueue{
    if (_mailCommandQueue == nil) {
        _mailCommandQueue = [[NSOperationQueue alloc] init];
        _mailCommandQueue.maxConcurrentOperationCount = 1;
        
    }
    return _mailCommandQueue;
}

-(MailInfoDataModel *)gettingLastSystemMailInfoDataModel{
    MailInfoDataModel *tempMailInfo =[MailInfoDataModel gettingLastSystemMailFromDB];
    if  (tempMailInfo){
        return tempMailInfo ;
    }else{
        return nil;
    }
}
-(void)systemMailForLockScreenCommandStartWithMailID:(NSString *)vMailID
                               andWithMailCreateTime:(long)vCreateTime {
    NSBlockOperation *op1 = [NSBlockOperation blockOperationWithBlock:^{
   
            [self getSystemMailMSgForiOSWithMailIDSqe:vMailID andWithMailCreatTime:vCreateTime andWithMsgCount:20];
    }];
    [self.mailCommandQueue addOperation:op1];
}
-(void)commandStart{
    
    NSBlockOperation *op1 = [NSBlockOperation blockOperationWithBlock:^{
        MailInfoDataModel *tempModel =  [self  gettingLastSystemMailInfoDataModel];
        if (tempModel){
            [self getSystemMailMSgForiOSWithMailIDSqe:tempModel.mail_ID andWithMailCreatTime:tempModel.creatTime andWithMsgCount:20];
        }else{
            [self getSystemMailMSgForiOSWithMailIDSqe:@"0" andWithMailCreatTime:0 andWithMsgCount:20];
        }
        
       
    }];
    [self.mailCommandQueue addOperation:op1];
}


-(void)getChatMailMsgWithCommandType:(int )vCommandType
     andWithCommandString:(NSString *)vCommandString{
    GetNewMailMsgCommand* getNewCmd = new GetNewMailMsgCommand(vCommandType,[vCommandString UTF8String]);
    getNewCmd->sendAndRelease();
}
-(void)getSystemMailMSgForiOSWithMailIDSqe:(NSString *)vMailIDSqe
                      andWithMailCreatTime:(long)vCreatTime
                           andWithMsgCount:(int )vMsgCount{
    DVLog(@"mailIDSeq :%@",vMailIDSqe);
//    kTimeLog(vMailIDSqe);
   [[NSOperationQueue mainQueue] addOperationWithBlock:^{
       string mailIDSqe = [vMailIDSqe UTF8String];
       string vCreatTimeStr =[[NSString stringWithFormat:@"%ld000",vCreatTime] UTF8String];
       
       GetNewSystemMailMsg_iOS_Command *getnewCmd = new  GetNewSystemMailMsg_iOS_Command(mailIDSqe,vCreatTimeStr, vMsgCount);
       getnewCmd ->sendAndRelease();
   }];
   
}
-(void)settingreadSatusWithMailID:(NSString *)vMailIDString andWithMailTpye:(int)vMailTpye{
    string uid = [vMailIDString UTF8String];
    MailReadStatusCommand* cmd = new MailReadStatusCommand(uid, vMailTpye);
    cmd->sendAndRelease();
}
-(void)settingReadStatus_Chat_WithChatFromUid:(NSString *)vFromUid
                               andWithModFlag:(BOOL)vModFlag{
    string fromUser =[vFromUid UTF8String];
    MailDialogReadCommand* cmd = new MailDialogReadCommand(fromUser, -1, "",vModFlag);
    cmd->sendAndRelease();
}
//系统邮件更具UID批量标记已读
-(void)settingReadStatus_mail_withMailArray:(NSArray *)vMailArray{
    NSString *string =@"";
    for (MailInfoDataModel *model in vMailArray) {
        model.status = 1;
        [model upDatingForDB];
        if (string.length > 0){
            string = [string stringByAppendingString:@","];
        }
        string = [string stringByAppendingString:model.mail_ID];
    }
    
  MailReadStatusBatchCommand* cmd =   new MailReadStatusBatchCommand([string UTF8String]); 
     cmd->sendAndRelease();
}
-(void)settingReadStatus_Banched_WithMailType:(int)vMailType
                               andWithModFlag:(BOOL)vModFlag
                             andWithMailTypes:(NSString *)vMailTypesString{
    string types =[vMailTypesString UTF8String];
//    MailDialogReadCommand* cmd = new MailDialogReadCommand("", vMailType, "",vModFlag,types);
//    cmd->sendAndRelease(); simon
 
//void MailController::notyfyReadDialogMail(int type,bool isMod,std::string types){

}
-(void)deletingMailDataWithMailID:(NSString *)vMailIDString{
    string mailID =[vMailIDString UTF8String];
    MailDeleteCommand *cmd =new MailDeleteCommand(mailID,1);
    cmd->sendAndRelease();
}
-(void)deletingMailDataWithALlMailIDArray:(NSArray *)vMailArray{
    NSString *string =@"";
    for (MailInfoDataModel *model in vMailArray) {
        if (string.length > 0){
            string = [string stringByAppendingString:@","];
        }
        string = [string stringByAppendingString:model.mail_ID];
    }
    MailBatchDelCommand *cmd = new MailBatchDelCommand([string UTF8String], "") ;
     cmd->sendAndRelease();
}

-(void)settingRewardStatusWithMailDataArray:(NSArray *)vMailArray{
    NSString *string =@"";
    for (MailInfoDataModel *model in vMailArray) {
        if (string.length > 0){
            string = [string stringByAppendingString:@","];
        }
        string = [string stringByAppendingString:model.mail_ID];
    }
    DVLog(@"commandString:%@",string);
    MailBatchRewardCommand *cmd = new MailBatchRewardCommand([string UTF8String], "") ;
    cmd->sendAndRelease();

}
-(void)deletingChatChannelWithChannelID:(NSString *)vChatChannelID{
    MailDialogDeleteCommand *cmd = new MailDialogDeleteCommand([vChatChannelID UTF8String],-1 ,"");
    cmd->sendAndRelease();
}
-(void)deletingChannelsWithChannelIDArray:(NSArray *)vMailArray{
     NSString *mailIDString =@"";
//    dispatch_async(dispatch_queue_create(0, 0), ^{
        for (ChatChannel  * tempChannel in vMailArray) {
            //command参数是每个聊天的最后一封邮件的uid拼接
            NSMsgItem *msg = [tempChannel gettingLastMsg];
            if (msg.mailId.length> 0){
                if  (mailIDString.length> 0){
                    mailIDString = [mailIDString stringByAppendingString:@","];
                }
                mailIDString = [mailIDString stringByAppendingString:msg.mailId];
                
            }
        }
        DVLog(@"%@",mailIDString);
//        dispatch_async(dispatch_get_main_queue(), ^{
            MailBatchDelCommand *cmd = new MailBatchDelCommand([mailIDString UTF8String],"0");
            cmd->sendAndRelease();
//        });
//    });
//    __block NSString *mailIDString =@"";
//    NSBlockOperation *op1 = [NSBlockOperation blockOperationWithBlock:^{
//        
//        
//        for (ChatChannel  * tempChannel in vMailArray) {
//            //command参数是每个聊天的最后一封邮件的uid拼接
//            NSMsgItem *msg = [tempChannel gettingLastMsg];
//            if (msg){
//                if  (mailIDString.length> 0){
//                    mailIDString = [mailIDString stringByAppendingString:@","];
//                }
//                mailIDString = [mailIDString stringByAppendingString:msg.mailId];
//                
//            }
//        }
//        DVLog(@"%@",mailIDString);
//    }];
//    [self.mailCommandQueue addOperation:op1];
//       [[NSOperationQueue mainQueue] addOperationWithBlock:^{
//           MailDialogDeleteCommand *cmd = new MailDialogDeleteCommand("",-1 ,[mailIDString UTF8String]);
//           cmd->sendAndRelease();
//          }];
}
#pragma mark -
#pragma mark 判断当前socket是否连接
-(BOOL)isConnected{
    return   NetController::shared()->isConnected();
}

@end
