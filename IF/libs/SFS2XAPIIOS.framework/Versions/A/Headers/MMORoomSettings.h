//
//  MMORoomSettings.h
//  SFS2X
//
//  Created by Lapo on 29/11/13.
//  Copyright (c) 2013 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "RoomSettings.h"
#import "Vec3D.h"
#import "MapLimits.h"

/** The MMORoomSettings class is a container for the settings required to create an MMORoom using the <CreateRoomRequest> request.
 
 @see MMORoom
 @see CreateRoomRequest
 @see CreateRoomSettings
 */
@interface MMORoomSettings : RoomSettings

/** Defines the Area of Interest (AoI) for the MMORoom.
 
 This value represents the area/range around the user that will be affected by server events and other users events. It is represented by a <Vec3D> object providing 2D or 3D coordinates.
 
 NOTE: Setting this value is mandatory.
 
 Example #1: a Vec3D(50,50) describes a range of 50 units (e.g. pixels) in all four directions (top, bottom, left, right) with respect to the user position in a 2D coordinates system.
 
 Example #2: a Vec3D(120,120,60) describes a range of 120 units in all four directions (top, bottom, left, right) and 60 units along the two Z-axis directions (backward, forward) with respect to the user position in a 3D coordinates system.

 @see Vec3D
 */
@property (strong, nonatomic) Vec3D* defaultAOI;

/** Defines the limits of the virtual environment represented by the MMORoom.
 
 When specified, this property must contain two non-null <Vec3D> objects representing the minimum and maximum limits of the 2D/3D coordinates systems. Any positional value that falls outside the provided limit will be refused by the server.
 
 This setting is optional but its usage is highly recommended.
 
 @see Vec3D
 */
@property (strong, nonatomic) MapLimits* mapLimits;

/** Defines the time limit before a user without a physical position set inside the <MMORoom> is kicked from the Room.
 
 As soon as the <MMORoom> is joined, the user still doesn't have a physical position set in the coordinates system, therefore it is considered in a "limbo" state. At this point the user is expected to set his position (via the <SetUserPositionRequest> request) within the amount of seconds expressed by this value.
 
 Default value is 50 seconds.
 */
@property (assign, nonatomic) NSInteger userMaxLimboSeconds;

/** Configures the speed at which the <[ISFSEvents onProximityListUpdate:]> event is sent by the server.
 
 In an <MMORoom>, the regular users list is replaced by a proximity list, which keeps an updated view of the users currently within the Area of Interest (AoI) of the current user. The speed at which these updates are fired by the server is regulated by this parameter, which sets the minimum time between two subsequent updates.
 
 NOTE: values below the default might be unnecessary for most applications unless they are in realtime.
 
 Default value is 250 milliseconds.
 
 @see [ISFSEvents onProximityListUpdate:]
 */
@property (assign, nonatomic) NSInteger proximityListUpdateMillis;

/** Sets if the users entry points in the current user's Area of Interest should be transmitted in the <[ISFSEvents onProximityListUpdate:]> event.
 
 If this setting is set to YES, when a user enters the AoI of another user, the server will also send the coordinates at which the former "appeared" within the AoI. This option should be turned off in case these coordinates are not needed, in order to save bandwidth.
 
 @default value: YES
 
 @see [User aoiEntryPoint]
 @see [MMOItem aoiEntryPoint]
 @see [ISFSEvents onProximityListUpdate]
 */
@property (assign, nonatomic) BOOL sendAOIEntryPoint;

/**
 @param name the name of the MMORoom
 */
-(id)initWithName:(NSString *)name;

/**
 @param name the name of the MMORoom
 */
+(id)settingsWithName:(NSString *)name;

@end
