//
//  IMMOItem.h
//  SFS2X
//
//  Created by Lapo on 28/11/13.
//  Copyright (c) 2013 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Vec3D.h"
#import "IMMOItemVariable.h"


/** The IMMOItem</em> protocol defines all the methods and properties that an object representing a SmartFoxServer MMOItem entity exposes.
    
 In the SmartFoxServer 2X client API the protocol is implemented by the <MMOItem> class. Read the class description for additional informations.
 
 See <MMOItem>, <MMORoom>, <Vec3D>

*/
@protocol IMMOItem <NSObject>

/** The MMOItem unique id */
@property (readonly, nonatomic) NSInteger id;

/** 
 The MMOItem entry point, it represents the coordinates in which the item appeared in the player's AoI (Area Of Interest
 
 @see Vec3D
*/
@property (nonatomic, strong) Vec3D* aoiEntryPoint;

+(id<IMMOItem>) fromSFSArray:(id<ISFSArray>)array;

/** Get an MMOItemVariable via its name
 @param varName the variable name
 @return the MMOItemVariable, nil if no variable with such name exist
 @see IMMOItemVariable
*/
-(id <IMMOItemVariable>)getVariable:(NSString *)varName;

/** Checks whether an MMOItemVariable exists with the provided name 
 @param varName the variable name
 @return YES if the variable exists
 @see IMMOItemVariable
*/
-(BOOL)containsVariable:(NSString *)varName;

-(void)setVariable:(id <IMMOItemVariable>)var;

/** Get all MMOItemVariables associated with this MMOItem
 @return all MMOItem variables
 @see IMMOItemVariable
*/
-(NSArray*) getVariables;

-(void) setVariables:(NSArray*) variables;
@end
