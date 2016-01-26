//
//  CSMessage+Manager.m
//  IF
//
//  Created by 张彦涛 on 15/12/28.
//
//
 
#import "CSMessage+Manager.h"
#import "UserManager.h"
@implementation CSMessage (Manager)
+(CSMessage *)messageWithSelfSendString:(NSString *)vSendString andWithChannelType:(CSChannelType )vChannelType andWithMessageBodyType:(CSMessageBodyType)vMsgBodyType{
    CSMessage *message =[[CSMessage alloc]init];
    message.msg = vSendString;
    message.createTime = [[NSDate date]timeIntervalSince1970];
    message.sendLocalTime = message.createTime;
    message.post = (int )vMsgBodyType;
    message.channelType = vChannelType;
    message.uid = [UserManager sharedUserManager].currentUser.uid;
    message.sendState = CSMessageSendState_Delivering;
    return message;
}
+(NSMutableDictionary*)TransformationHistoryToArrModel:(NSDictionary *)dataDic
{
    
    NSMutableArray * dataArrAlliance=[[NSMutableArray alloc]init];
    NSMutableArray * dataArrCountry=[[NSMutableArray alloc]init];
    NSArray * resultArr=(NSArray*)[[dataDic objectForKey:@"result"] objectForKey:@"rooms"];
    for (int i=0; i<resultArr.count; i++) {
        if ([[[resultArr objectAtIndex:i] objectForKey:@"roomId"] isEqualToString:[NSString stringWithFormat:@"test_alliance_%ld_%@",[UserManager sharedUserManager].currentUser.serverId,[UserManager sharedUserManager].currentUser.allianceId]]) {
            NSArray * arr=[[resultArr objectAtIndex:i]objectForKey:@"msgs"];
            for (int i=0; i<arr.count; i++) {
                NSDictionary * dic=[arr objectAtIndex:i];
                
                CSMessage * message=[[CSMessage alloc]init];
                message.sendState=2;
                message.sequenceId=[[dic objectForKey:@"seqId"] intValue];
                message.uid=[dic objectForKey:@"sender"];
                NSString *transString = [NSString stringWithString:[[dic objectForKey:@"msg"] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
                message.msg=transString;
                
                NSString * obsendLocalTime=[NSString stringWithFormat:@"%ld",[[dic objectForKey:@"sendTime"] integerValue]];
                if (obsendLocalTime.length>=10) {
                message.sendLocalTime=[[obsendLocalTime substringToIndex:10] intValue];
                }
                
                NSString * obTime=[NSString stringWithFormat:@"%ld",[[dic objectForKey:@"serverTime"] integerValue]];
                if (obTime.length>=10) {
                message.createTime=[[obTime substringToIndex:10] intValue];
                }
                
                message.post=[[dic objectForKey:@"type"] intValue];
                message.originalLang=[dic objectForKey:@"originalLang"];
                message.translatedLang=[dic objectForKey:@"translationMsg"];
                [dataArrAlliance addObject:message];
            }

        }else if ([[[resultArr objectAtIndex:i] objectForKey:@"roomId"] isEqualToString:@"test_country_1"]){
            NSArray * arr=[[resultArr objectAtIndex:i]objectForKey:@"msgs"];
            for (int i=0; i<arr.count; i++) {
                NSDictionary * dic=[arr objectAtIndex:i];
               
                
                CSMessage * message=[[CSMessage alloc]init];
                message.sendState=2;
                message.sequenceId=[[dic objectForKey:@"seqId"] intValue];
                message.uid=[dic objectForKey:@"sender"];
                NSString *transString = [NSString stringWithString:[[dic objectForKey:@"msg"] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
                message.msg=transString;
                
                NSString * obsendLocalTime=[NSString stringWithFormat:@"%ld",[[dic objectForKey:@"sendTime"] integerValue]];
                if (obsendLocalTime.length>=10) {
                    message.sendLocalTime=[[obsendLocalTime substringToIndex:10] intValue];
                }
                
                NSString * obTime=[NSString stringWithFormat:@"%ld",[[dic objectForKey:@"serverTime"] integerValue]];
                if (obTime.length>=10) {
                    message.createTime=[[obTime substringToIndex:10] intValue];
                }

                message.post=[[dic objectForKey:@"type"] intValue];
                message.originalLang=[dic objectForKey:@"originalLang"];
                message.translatedLang=[dic objectForKey:@"translationMsg"];
                [dataArrCountry addObject:message];
            }
        }
    }

    NSMutableDictionary * dic=[[NSMutableDictionary alloc]init];
    [dic setObject:dataArrCountry forKey:@"CountryArr"];
    [dic setObject:dataArrAlliance forKey:@"AllianceArr"];
    
    return dic;
}
+(NSArray*)TransformationToArrModel:(NSDictionary *)dataDic
{
//    {"cmd":"push.chat.room",
//"serverTime":1451472174811,
//      "data":{"appId":"100001",
//            "seqId":137,
//          "sender":"1380131787000001",
//          "roomId":"test_country_1",
//          "msg":"xbxbxbx",
//          "sendTime":1451471961,
//          "serverTime":1451472174771,
//          "senderInfo":{"uid":"1380131787000001","lang":"zh_CN"},
//          "type":0,
//          "group":"country",
//          "extra":null},
//    "server":"s2",
//  "clientid":"14"}

//    "{"cmd":"push.chat.room",
//    "serverTime":1451873895952,
//    "data":{"appId":"100001",
//        "seqId":170,
//        "sender":"138939645000001",
//        "roomId":"test_country_1",
//        "msg":"u",
//        "sendTime":"1451873893",
//        "serverTime":"1451873675734",
//        "senderInfo":null,
//        "type":0,
//        "group":"country",
//        "extra":{"userLang":"zh_CN"},
//        "originalLang":"hr",
//        "translationMsg":"in the"},
//    "server":"s2",
//    "clientid":"75"}"

    NSDictionary * dic=[dataDic objectForKey:@"data"];
    CSMessage * message=[[CSMessage alloc]init];
    message.sequenceId=[[dic objectForKey:@"seqId"] intValue];
    message.sendState=2;
    message.uid=[dic objectForKey:@"sender"];
    message.msg=[dic objectForKey:@"msg"];
    message.sendLocalTime=[[dic objectForKey:@"sendTime"] intValue];
    if (((NSString*)[dic objectForKey:@"serverTime"]).length>=10) {
      message.createTime=[[[dic objectForKey:@"serverTime"]substringToIndex:10] intValue];
    }
    message.post=[[dic objectForKey:@"type"] intValue];
    message.originalLang=[dic objectForKey:@"originalLang"];
    message.translatedLang=[dic objectForKey:@"translationMsg"];
    NSArray * arr=[NSArray arrayWithObjects:message, nil];
    return arr;
}

-(int)gettingRedPackageStatusWithMessage{
    NSArray *arr = [self.attachmentId componentsSeparatedByString:@"|"];
    if ([arr count] == 2) {
        NSString *status = arr[1];
        return [status intValue];
    }else{
        return -1;
    }
    
}
@end
