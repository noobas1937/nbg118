//
//  WbSocketControl.h
//  WBSOCKETDEMO
//
//  Created by 张世良 on 15/12/28.
//  Copyright © 2015年 apple. All rights reserved.
//
#import "ChatChannel.h"
#import "ChatChannel+ChatMessage.h"
#import "ChannelManager.h"
#import "ServiceInterface.h"
#import "NSString+Extension.h"
#import <Foundation/Foundation.h>
#import "SRWebSocket.h"
#import "WbSocketControl.h"
#import "AFNetworking.h"
#import "UserManager.h"
#import "InCommonUsed.h"
#import "CSMessage.h"
#import "CSMessage+Manager.h"
@interface WbSocketControl : NSObject <SRWebSocketDelegate>
{
    SRWebSocket * wbsocket;
    NSMutableURLRequest * urlRequest;
    NSString * addCountryTime;
    NSString * addAllianceTime;
}
@property(nonatomic,assign)BOOL isConnected;
@property(nonatomic,assign)BOOL isAddCountry;
@property(nonatomic,assign)BOOL isAddAlliance;
+(WbSocketControl*)SingleControl;
-(void)webScoketConnect;
-(void)wbSocketOpen;
-(void)wbSocketClose;
-(void)getFirstHistoryMessage;
-(void)wbSocketSendMessageSingle:(NSMutableDictionary*)resultDic;
-(void)wbSocketIntoGroupChatRoom:(NSMutableDictionary*)resultDic;
-(void)wbSocketSendMessageGroup:(NSMutableDictionary*)resultDic;
-(void)wbSocketOutGroupChatRoom:(NSMutableDictionary*)resultDic;
-(void)wbSocketChangRoomName:(NSMutableDictionary*)resultDic;
-(void)wbSocketInitWithHTTPHeaderAppid:(NSString*)appid time:(NSString*)time uid:(NSString*)uid url:(NSString * )url;
@end
