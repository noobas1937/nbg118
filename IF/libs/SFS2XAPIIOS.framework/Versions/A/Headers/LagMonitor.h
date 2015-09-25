//
//  LagMonitor.h
//  SFS2X
//
//  Created by Lapo on 24/02/14.
//  Copyright (c) 2014 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

@interface LagMonitor : BaseRequest

@property (nonatomic) NSInteger interval;
@property (nonatomic) NSInteger queueSize;

-(void) start;
-(void) stop;
-(void) destroy;
-(BOOL) isRunning;
-(NSInteger) onPingPong;
-(int) lastPingTime;
-(int) averagePingTime;

+(id) initWithInterval:(NSInteger)interval qSize:(NSInteger)size smartfox:(SmartFox2XClient*)sfs;

@end
