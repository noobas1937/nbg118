//
//  WbSocketControl.m
//  WBSOCKETDEMO
//
//  Created by 张世良 on 15/12/28.
//  Copyright © 2015年 apple. All rights reserved.
//

#import "WbSocketControl.h"
static WbSocketControl *singleton = nil;
@implementation WbSocketControl
+(WbSocketControl*)SingleControl
{
    @synchronized(self) {
        if (singleton==nil) {
            singleton=[[WbSocketControl alloc]init];
        }
    }
    return singleton;
}

-(void)webScoketConnect
{
    self.isConnected=NO;
    self.isAddAlliance=NO;
    self.isAddCountry=NO;
    addCountryTime=@"";
    addAllianceTime=@"";
    
    NSString * t=[self getCurrentTime];
    NSString * s=[[[[t substringToIndex:3] md5] stringByAppendingString:[[t substringFromIndex:t.length-3] md5]] md5];
    NSMutableDictionary * json=[[NSMutableDictionary alloc]init];
    [json setObject:t forKey:@"t"];
    [json setObject:s forKey:@"s"];
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    manager.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"text/html", nil];
    manager.requestSerializer = [AFHTTPRequestSerializer serializer];
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    [manager POST:kWebSocketLinkUrl parameters:json success:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        NSDictionary * dic = [NSJSONSerialization JSONObjectWithData:responseObject options:NSJSONReadingAllowFragments error:nil];
        if ([[dic objectForKey:@"message"] isEqualToString:@"success"]) {
            NSString * appid=@"100001";
            NSString * uid=[UserManager sharedUserManager].currentUser.uid;
            NSString * url=[NSString stringWithFormat:@"%@://%@:%@",
                            [[dic objectForKey:@"data"] objectForKey:@"protocol"],
                            [[dic objectForKey:@"data"] objectForKey:@"ip"],
                            [[dic objectForKey:@"data"] objectForKey:@"port"]
                            ];
            [self wbSocketInitWithHTTPHeaderAppid:appid time:t uid:uid url:url];
            [self wbSocketOpen];
            
        }else{
            
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        
        
    }];

}

-(void)wbSocketInitWithHTTPHeaderAppid:(NSString*)appid time:(NSString*)time uid:(NSString*)uid url:(NSString * )url{
    
    
    if (urlRequest==nil) {
        urlRequest=[[NSMutableURLRequest alloc]initWithURL:[NSURL URLWithString:url]];
        urlRequest.timeoutInterval=10;
    }
    NSString * result=[[[[[appid stringByAppendingString:uid] md5] stringByAppendingString:time] md5] lowercaseString];
    NSString * sign=result;
    [urlRequest setValue:appid forHTTPHeaderField:@"appid"];
    [urlRequest setValue:time forHTTPHeaderField:@"time"];
    [urlRequest setValue:uid forHTTPHeaderField:@"uid"];
    [urlRequest setValue:sign forHTTPHeaderField:@"sign"];
    if (wbsocket==nil) {
        wbsocket=[[SRWebSocket alloc]initWithURLRequest:urlRequest protocols:nil allowsUntrustedSSLCertificates:YES];
        wbsocket.delegate = self;
    }
}

-(void)wbSocketOpen{
    
    [wbsocket open];
}
-(void)wbSocketClose{
    
    [wbsocket close];
}
-(void)wbSocketChangRoomName:(NSMutableDictionary *)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)wbSocketSendMessageSingle:(NSMutableDictionary*)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)wbSocketSendMessageGroup:(NSMutableDictionary*)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)wbSocketIntoGroupChatRoom:(NSMutableDictionary*)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)wbSocketOutGroupChatRoom:(NSMutableDictionary*)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}

-(void)timerFired
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [wbsocket send:@"heartbeat"];
    });
}
- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean;
{
    NSLog(@"wbsoket连接断开 %ld %@ %d",code,reason,wasClean);
    [self webScoketConnect];
}
- (void)webSocketDidOpen:(SRWebSocket *)webSocket;
{
    NSLog(@"wbsoket连接成功");
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:60.0 target:self selector:@selector(timerFired) userInfo:nil repeats:YES];
    [timer fire];
}
- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error;
{
    NSLog(@"wbsoket连接错误 %@", error);
    [self webScoketConnect];
}
- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessage:(id)message;
{
    
    NSLog(@"wbsoket接收消息 \"%@\"", message);
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    //心跳
    if ([(NSString*)message isEqualToString:@"heartbeat"]) {
        return;
    }
        
    NSDictionary * dic=[self parseJSONStringToNSDictionary:(NSString*)message];
    //判断连接是否成功
    if (self.isConnected==NO)
    {
        
        if ([[dic objectForKey:@"cmd"] isEqualToString:@"login.success"])
        {
            self.isConnected=YES;
            //进入国家
                if (self.isAddCountry==NO) {
                    [self addCountryGroup];
                }
            
            //进入联盟
                if ([UserManager sharedUserManager].currentUser.allianceId.length> 0) {
            
                    if (self.isAddAlliance==NO) {
                        [self addAllianceGroup];
                    }
            
                }else{
                    self.isAddAlliance=YES;
                }
        }
        
    }

        
        
    //进入国家    成功会返回 sendtime  失败的话不会返回
    if (self.isAddCountry==NO) {
        NSString * sendTimeString =[NSString stringWithFormat:@"%@",[dic objectForKey:@"sendTime"]];
        if ([sendTimeString isEqualToString:addCountryTime]) {
            self.isAddCountry=YES;
            [self getFirstHistoryMessage];
        }
    }
        
    //进入联盟     成功会返回 sendtime  失败的话不会返回
    if ([UserManager sharedUserManager].currentUser.allianceId.length>0) {
        if (self.isAddAlliance==NO)
        {
             NSString * sendTimeString =[NSString stringWithFormat:@"%@",[dic objectForKey:@"sendTime"]];
            if ([sendTimeString isEqualToString:addAllianceTime]) {
                self.isAddAlliance=YES;
                [self getFirstHistoryMessage];
            }
        }
    }

        
        
        
    //push的消息
    if ([[dic objectForKey:@"cmd"] isEqualToString:@"push.chat.room"]) {
        NSArray * dataArr = [CSMessage TransformationToArrModel:dic];
        ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingCountryChannel];
        if (dataArr.count>0) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [cc chatNewMsgPushed:dataArr];//同步UI
            });
            for (int i=0; i<dataArr.count; i++) {
                 [cc savingMessageToDBWithMsg:(CSMessage*)[dataArr objectAtIndex:i]];//进数据库
            }
        }
    }
    
    
    
    //第一次拉取历史消息
    if ([[dic objectForKey:@"cmd"] isEqualToString:@"history.roomsv2"]) {
        ChatChannel *ccCountry = [[ChannelManager sharedChannelManager] gettingCountryChannel];
        ChatChannel *ccAlliance = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
        NSDictionary * dataDic = [CSMessage TransformationHistoryToArrModel:dic];
        NSArray * CountryArr=[dataDic objectForKey:@"CountryArr"];
        NSArray * AllianceArr=[dataDic objectForKey:@"AllianceArr"];
        for (int i=0; i<CountryArr.count; i++) {
            [ccCountry savingMessageToDBWithMsg:(CSMessage*)[CountryArr objectAtIndex:i]];//循环进数据库
        }
        for (int i=0; i<AllianceArr.count; i++) {
            [ccAlliance savingMessageToDBWithMsg:(CSMessage*)[AllianceArr objectAtIndex:i]];//循环进数据库
        }
        DVLog(@"ccCountry:  %@",ccCountry);
        DVLog(@"ccCountry:  %@",ccAlliance);
       
    }
        
     });
}





-(void)getFirstHistoryMessage
{
    return;
    //国家和联盟都进入后  拉取历史消息
    if (self.isAddAlliance==YES&&self.isAddCountry==YES) {
        NSMutableDictionary * resultDic=[[NSMutableDictionary alloc]init];
        NSMutableDictionary * params=[[NSMutableDictionary alloc]init];
        [resultDic setObject:@"history.roomsv2" forKey:@"cmd"];
        [resultDic setObject:[self getCurrentTime] forKey:@"sendTime"];
        NSMutableDictionary * roomsDic=[[NSMutableDictionary alloc]init];
        [roomsDic setObject:@"0" forKey:@"test_country_1"];
        if ([UserManager sharedUserManager].currentUser.allianceId.length>0) {
            [roomsDic setObject:@"0" forKey:[NSString stringWithFormat:@"test_alliance_%ld_%@",[UserManager sharedUserManager].currentUser.serverId,[UserManager sharedUserManager].currentUser.allianceId]];
        }
        [params setObject:roomsDic forKey:@"rooms"];
        [params setObject:@"" forKey:@"lang"];
        [resultDic setObject:params forKey:@"params"];
        [wbsocket send:[self dictionaryToJson:resultDic]];
    }
}
-(void)addAllianceGroup
{
    NSMutableDictionary * resultDic=[[NSMutableDictionary alloc]init];
    NSMutableDictionary * params=[[NSMutableDictionary alloc]init];
    [resultDic setObject:@"room.join" forKey:@"cmd"];
    long long time=[[self getCurrentTime] longLongValue]+1;//时间+1区分返回的是国家还是联盟
    [resultDic setObject:[NSString stringWithFormat:@"%lld",time]forKey:@"sendTime"];
    [params setObject:[NSString stringWithFormat:@"test_alliance_%ld_%@",[UserManager sharedUserManager].currentUser.serverId,[UserManager sharedUserManager].currentUser.allianceId] forKey:@"roomId"];//id 需要对应一下
    [params setObject:@"alliance" forKey:@"group"];
    [resultDic setObject:params forKey:@"params"];
    addAllianceTime=[resultDic objectForKey:@"sendTime"];
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)addCountryGroup
{
    NSMutableDictionary * resultDic=[[NSMutableDictionary alloc]init];
    NSMutableDictionary * params=[[NSMutableDictionary alloc]init];
    [resultDic setObject:@"room.join" forKey:@"cmd"];
    [resultDic setObject:[self getCurrentTime] forKey:@"sendTime"];
    [params setObject:@"test_country_1" forKey:@"roomId"];
    [params setObject:@"country" forKey:@"group"];
    [resultDic setObject:params forKey:@"params"];
    addCountryTime=[resultDic objectForKey:@"sendTime"];
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
- (NSString*)dictionaryToJson:(NSDictionary *)dic
{
    NSError *parseError = nil;
    NSData  *jsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:&parseError];
    return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
}
-(NSDictionary *)parseJSONStringToNSDictionary:(NSString *)JSONString {
    NSData *JSONData = [JSONString dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *responseJSON = [NSJSONSerialization JSONObjectWithData:JSONData options:NSJSONReadingMutableLeaves error:nil];
    return responseJSON;
}
-(NSString*)getCurrentTime
{
    NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:0];
    //NSTimeInterval a=[dat timeIntervalSince1970] * 1000;
    NSTimeInterval a=[dat timeIntervalSince1970];
    long long int date = (long long int)a;
    NSString* time=[NSString stringWithFormat:@"%lld",date];
    return [NSString stringWithFormat:@"%@000",time];
}
@end
