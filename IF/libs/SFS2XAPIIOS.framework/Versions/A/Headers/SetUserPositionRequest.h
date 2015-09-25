//
//  SetUserPositionRequest.h
//  SFS2X
//
//  Created by Lapo on 29/11/13.
//  Copyright (c) 2013 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"
#import "Vec3D.h"
#import "Room.h"

extern NSString* const SetUserPositionRequest_KEY_ROOM;
extern NSString* const SetUserPositionRequest_KEY_VEC3D;
extern NSString* const SetUserPositionRequest_KEY_PLUS_USER_LIST;
extern NSString* const SetUserPositionRequest_KEY_MINUS_USER_LIST;
extern NSString* const SetUserPositionRequest_KEY_PLUS_ITEM_LIST;
extern NSString* const SetUserPositionRequest_KEY_MINUS_ITEM_LIST;


/** Updates the User position inside an MMORoom.
 
 MMORooms represent virtual environments and can host any number of users. Based on their position, the system allows users within a certain range from each other (Area of Interest, or AoI) to interact.
 This request allows the current user to update his position inside the MMORoom, which in turn will trigger a [ISFSEvents onProximityListUpdate:] event for all users that fall within his AoI.
 
 @see MMORoom
 @see [ISFSEvents onProximityListUpdate:]

 */
@interface SetUserPositionRequest : BaseRequest

/**
 @param pos     the position in the MMORoom
 @param theRoom MMORoom, can be nil in which case the last joined Room will be used
 
 @see MMORoom
 @see [ISFSEvents onProximityListUpdate:]
 */
+(id)requestWithPosition:(Vec3D*)pos mmoRoom:(id<Room>)theRoom;

@end
