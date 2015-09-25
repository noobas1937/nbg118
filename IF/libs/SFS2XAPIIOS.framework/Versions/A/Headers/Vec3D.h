//
//  Vec3D.h
//  SFS2X
//
//  Created by Lapo on 28/11/13.
//  Copyright (c) 2013 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>

/** The Vec3D object represents a position in a 2D or 3D space.

 This class is used to express a position inside a virtual environment with no specific unit of measure (could be pixels, feet, meters, etc).
 
 Positions along the X,Y,Z axes can be expressed as ints or floats. Although Objective-C can handle both ints and floats under the NSNumber class, SmartFoxServer 2X needs to recognize the specific types. For this reason we recommend to initialize the object with the proper literals to declare which type you are using.
 
 For instance (x:@10, y:@10, z:@10) will use integers while (x:@10F, y:@10F, z:@10F) will use the same coordinates but with a floating point type. 
 
 Long and Doubles are not supported for this type of coordinate values.
 
 See <SetUserPositionRequest>, <MMORoom>, <MMOItem>
 */
@interface Vec3D : NSObject

/**
 The X coordinate as Integer
 */
@property (readonly) NSInteger intX;

/**
 The Y coordinate as Integer
 */
@property (readonly) NSInteger intY;

/**
 The Z coordinate as Integer
 */
@property (readonly) NSInteger intZ;

/**
 The X coordinate as Float
 */
@property (readonly) NSNumber* floatX;

/**
 The Y coordinate as Float
 */
@property (readonly) NSNumber* floatY;

/**
 The Z coordinate as Float
 */
@property (readonly) NSNumber* floatZ;

// Constructors
- (id)initWithX:(NSNumber *)x Y:(NSNumber *)y Z:(NSNumber *)z;
- (id)initWithX:(NSNumber *)x Y:(NSNumber *)y;

// Static Constructors

/** 3D Constructor
 
 @param x the x coordinate
 @param y the y coordinate
 @param z the z coordinate
 */
+ (id) vectorWithX:(NSNumber *)x Y:(NSNumber *)y Z:(NSNumber *)z;

/** 2D Constructor
 
 @param x the x coordinate
 @param y the y coordinate
 */
+ (id) vectorWithX:(NSNumber *)x Y:(NSNumber *)y;

/** Checks if the object uses Floating point or Integer values
 @return YES if the floats are used.
 */
- (BOOL) isFloat;


// Private
- (NSArray *) toArray;
+ (Vec3D *) fromArray:(NSArray*) array;

@end
