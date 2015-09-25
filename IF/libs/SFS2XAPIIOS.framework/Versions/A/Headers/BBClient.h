//
//  BBClient.h
//  SFS2X
//
//  Created by Wayne Helman on 2012-08-14.
//  Copyright (c) 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BBEvent.h"
#import "Logger.h"
#import "BitSwarmClient.h"


NSString const * BB_DEFAULT_HOST;
int const BB_DEFAULT_PORT;
NSString * const BB_SERVLET;
NSString * const BB_NULL;

NSString * const CMD_CONNECT;
NSString * const CMD_POLL;
NSString * const CMD_DATA;
NSString * const CMD_DISCONNECT;
NSString * const ERR_INVALID_SESSION;

NSString * const SFS_HTTP;
NSString * const SEP;

NSTimeInterval const MIN_POLL_SPEED;
NSTimeInterval const MAX_POLL_SPEED;
NSTimeInterval const DEFAULT_POLL_SPEED;


@interface BBClient : NSObject
{
    
@private
    BOOL _isConnected;
    NSString * __weak _host;
    int _port;
    NSString * _bbUrl;
    BOOL _debug;
    NSString * _sessId;
    NSTimeInterval _pollSpeed;
    Logger * _log;
    BitSwarmClient *_delegate;
    CFMutableDictionaryRef _requests;
    
}

@property (readwrite) BOOL isDebug;
@property (weak, readonly) NSString * host;
@property (readonly) int port;
@property (readwrite, strong) NSString * sessionId;
@property (readwrite) NSTimeInterval pollSpeed;

-(id)initWithBitSwarm:(BitSwarmClient *) bsClient debug:(BOOL)debug;

-(BOOL)isConnected;
-(void)connect:(NSString * )host usingPort:(int) port;
-(void)send:(NSData *)data;
-(void)disconnect;
-(void)close;
-(void)pollSpeed:(NSTimeInterval)value;

-(void)handleData:(NSURLConnection *) connection;

- (NSString *)encodeBase64WithString:(NSString *)strData;
- (NSString *)encodeBase64WithData:(NSData *)data;
- (NSData *)decodeBase64WithString:(NSString *)strBase64;


@end
