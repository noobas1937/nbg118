//
//  MMORoom.h
//  SFS2X
//
//  Created by Lapo on 29/11/13.
//  Copyright (c) 2013 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "SFSRoom.h"
#import "Vec3D.h"
#import "IMMOItem.h"

/** The MMORoomobject represents a specialized type of Room entity on the client.
 
 The MMORoom is ideal for huge virtual worlds and MMO games because it works with proximity lists instead of "regular" users lists.
 
 This allows thousands of users to interact with each other based on their Area of Interest (AoI). The AoI represents a range around the user that is affected by server and user events, outside which no other events are received.
 
 The size of the AoI is set at Room creation time and it is the same for all users who joined it.
 Supposing that the MMORoom hosts a 3D virtual world, setting an AoI of (x=100, y=100, z=40) for the Room tells the server to transmit updates and broadcast events to and from those users that fall within the AoI range around the current user; this means the area within +/- 100 units on the X axis, +/- 100 units on the Y axis and +/- 40 units on the Z axis.
 
 As the user moves around in the virtual environment, he can update his position in the corresponding MMORoom and thus continuously receive events about other users (and items - see below) entering and leaving his AoI.
 
 The player will be able to update his position via the <SetUserPositionRequest> request and receive updates on his current proximity list by means of the <[ISFSEvents onProximityListUpdate]> event.
 
 Finally, MMORooms can also host any number of "MMOItems" which represent dynamic non-player objects that users can interact with. They are handled by the MMORoom using the same rules of visibility described before.
 
 @see CreateRoomRequest
 @see MMORoomSettings
 @see SetUserPositionRequest
 @see MMOItem
 @see [ISFSEvents onProximityListUpdate]

 */
@interface MMORoom : SFSRoom

/**
 The default Area of Interest (AoI) of this MMORoom.
 @see Vec3D
 */
@property (strong, nonatomic) Vec3D* defaultAOI;

/**
 The lower coordinates value for the MMORoom.
 @see Vec3D
 */
@property (strong, nonatomic) Vec3D* lowerMapLimit;

/**
 The highest coordinates value for the MMORoom.
 @see Vec3D
 */
@property (strong, nonatomic) Vec3D* higherMapLimit;

/** Gets an MMOItem by its unique id
 @param itemId the id
 @return the MMOItem or nil if no item was found with the provided id
 @see MMOItem
 */
- (id<IMMOItem>) getMMOItem:(NSInteger)itemId;

/** Get all MMOItems managed by this MMORoom
 @return all MMOItems
 @see MMOItem
 */
- (NSArray*) getMMOItems;


- (void) addMMOItem:(id<IMMOItem>)item;
- (void) removeMMOItem:(NSInteger) itemId;

@end
