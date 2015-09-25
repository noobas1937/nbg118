//
//  MMOItemVariable.h
//  SFS2X
//
//  Created by Lapo on 28/11/13.
//  Copyright (c) 2013 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IMMOItemVariable.h"
#import "SFSUserVariable.h"


/** The MMOItemVariable object represents a SmartFoxServer MMOItem Variable entity on the client.
 
 It is a custom value attached to an <MMOItem> object that gets automatically synchronized between client and server on every change, provided that the MMOItem is inside the Area of Interest (AoI) of the current user in the MMORoom.
 
 Changes in the MMOItemVariables are dispatched via the <[ISFSEvents onMMOItemVariablesUpdate]> event
 
 NOTE: MMOItem Variables behave exactly like User Variables and support the same data types, but they can be created, updated and deleted on the server side only.</p>
 
 @see SFSUserVariable
 @see MMORoom
 @see MMOItem
 @see [ISFSEvents onMMOItemVariablesUpdate]

 */
@interface MMOItemVariable : SFSUserVariable<IMMOItemVariable>

+ (id <IMMOItemVariable>) fromSFSArray:(id <ISFSArray>)sfsa;

- (id <IMMOItemVariable>)initWithName:(NSString *)name value:(id)value type:(NSInteger)type;
+ (id <IMMOItemVariable>)variableWithName:(NSString *)name value:(id)value;
+ (id <IMMOItemVariable>)variableWithName:(NSString *)name value:(id)value type:(NSInteger)type;


@end
