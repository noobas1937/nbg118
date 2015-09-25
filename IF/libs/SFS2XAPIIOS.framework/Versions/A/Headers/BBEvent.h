//
//  BBEvent.h
//  SFS2X
//
//  Created by Wayne Helman on 2012-08-14.
//  Copyright (c) 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseEvent.h"

EXTERN NSString * const BBEvent_CONNECT;
EXTERN NSString * const BBEvent_DISCONNECT;
EXTERN NSString * const BBEvent_DATA;
EXTERN NSString * const BBEvent_IO_ERROR;
EXTERN NSString * const BBEvent_SECURITY_ERROR;

@interface BBEvent : BaseEvent {
	
}

-(id)initWithType:(NSString *)type params:(NSDictionary *)params;
-(id)clone;

@end
