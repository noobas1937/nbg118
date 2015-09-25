//
//  BitSwarmClient.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>

#if TARGET_OS_EMBEDDED || TARGET_IPHONE_SIMULATOR
    #import <CFNetwork/CFNetwork.h>
#else
    #import <CoreServices/CoreServices.h>
#endif

#import "Categories.h"
#import "IoHandler.h"
#import "IController.h"
#import "IBitSwarmEvents.h"
#import "UDPManager.h"
#import "BBEvent.h"


@class SmartFox2XClient;
@class Logger;
@class SystemController;
@class ExtensionController;
@class SendBuffer;
@class BBClient;

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 40000
    @interface BitSwarmClient : NSObject <NSStreamDelegate> {
#else
    @interface BitSwarmClient : NSObject {
#endif


@private
	
	NSInputStream *_inStream;
	NSOutputStream *_outStream;
	
	NSLock *_receiveLock;	
	NSMutableData *_receiveBuffer;
	
	NSLock *_sendLock;	
	NSMutableArray *_sendBuffers;
	
	id <IoHandler> _ioHandler;
	NSMutableDictionary *_controllers;
	NSInteger _compressionThreshold; 
	SmartFox2XClient *_sfs;
	id <IBitSwarmEvents> _delegate;
	BOOL _connected;
	NSString  *_lastIpAddress;
	int _lastTcpPort;
        
	NSInteger _reconnectionDelayMillis;
	NSInteger _reconnectionSeconds;
    NSNumber *_firstReconnAttempt;
    NSInteger _reconnCounter;
        
    NSString  *_connectionMode;
	BOOL _attemptingReconnection;
    BOOL _useBlueBox;
	Logger *_log;
	SystemController *_sysController;
	ExtensionController *_extController;
	BOOL _controllersInited;
        
    UDPManager *_udpManager;
	
	BOOL _noDataSent;
	SendBuffer *_lastSendBuffer;
        
    BBClient *_bbClient;
    NSTimeInterval _pollSpeed;
        
    
}

@property (weak, nonatomic, readonly) SmartFox2XClient *sfs;
@property (nonatomic, readonly) BOOL connected;
@property (nonatomic, strong) id <IoHandler> ioHandler;
@property (nonatomic, readwrite) NSInteger compressionThreshold;
@property (nonatomic, readwrite) NSInteger reconnectionDelayMillis;
@property (weak, nonatomic, readonly) SystemController *sysController;
@property (weak, nonatomic, readonly) ExtensionController *extController;
@property (nonatomic, readonly, strong) UDPManager *udpManager;
@property (nonatomic, readwrite) BOOL isReconnecting;
@property (weak, nonatomic, readonly) NSString *connectionIp;
@property (nonatomic, readonly) NSInteger connectionPort;
@property (nonatomic, readwrite) NSInteger reconnectionSeconds;
@property (weak, nonatomic, readonly) NSString *connectionMode;
@property (nonatomic, readonly) BOOL useBlueBox;
@property (nonatomic, strong) BBClient *httpSocket;
@property (nonatomic, readwrite) NSTimeInterval pollSpeed;


-(id <IController>)getController:(NSInteger)id_;
-(id)initWithSmartFox:(SmartFox2XClient *)sfs;
-(void)initBitSwarm;
-(void)destroy;
-(id <IController>)getControllerById:(NSInteger)id_;
-(void)connect:(NSString *)ip port:(int)port;
-(void)send:(id <IMessage>)message;
-(void)disconnect:(NSString *)reason;
-(void)killConnection;
-(NSString *)connectionMode;
-(void)forceBlueBox:(BOOL)value;
-(NSNumber *)nextUdpPacketId;
-(int)mySelfId;
-(void)writeToSocket:(NSData *)data;
-(void)writeToUDPSocket:(NSData *)data;
-(void)enableBBoxDebug:(BOOL)value;
-(void)stopReconnection;
        
-(void) executeDisconnection:(BitSwarmEvent *)evt;
        
-(void)onBBConnect:(BBEvent *)evt;
-(void)onBBData:(BBEvent *)evt;
-(void)onBBDisconnect:(BBEvent *)evt;
-(void)onBBError:(BBEvent *)evt;

@end
