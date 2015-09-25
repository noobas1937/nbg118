//
//  MMOItem.h
//  SFS2X
//
//  Created by Lapo on 28/11/13.
//  Copyright (c) 2013 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IMMOItem.h"

/** An MMOItem represents an active non-player entity inside an MMORoom.
 
 MMOItems can be used to represent bonuses, triggers, bullets, etc, or any other non-player entity that will be handled using the MMORoom's rules of visibility.
 
 This means that whenever one or more MMOItems fall within the Area of Interest (AoI) of a user, their presence will be notified to that user by means of the <[ISFSEvents onProximityListUpdate]> event.
 
 MMOItems are identified by a unique ID and can have one or more MMOItem Variables associated to store custom data.
 
 NOTE: MMOItems can be created in a server side Extension only; client side creation is not allowed.
 
 @see MMORoom
 @see MMOItemVariable
 */
@interface MMOItem : NSObject<IMMOItem>

-(id) initWithId:(NSInteger)itemId;

@end
