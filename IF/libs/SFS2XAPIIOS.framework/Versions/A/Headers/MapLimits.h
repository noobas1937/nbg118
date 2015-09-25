//
//  MapLimis.h
//  SFS2X
//
//  Created by Lapo on 29/11/13.
//  Copyright (c) 2013 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Vec3D.h"

/** The MapLimits class is used to set the limits of the virtual environment represented by an <MMORoom> when creating it. The limits represent the minimum and maximum coordinate values (2D or 3D) that the <MMORoom> should expect.
 
 @see MMORoom
 @see MMORoomSettings
 @see Vec3D
 */
@interface MapLimits : NSObject

/** The lower MMORoom coordinates 
 @see Vec3D
 @see MMORoom
 */
@property (readonly, nonatomic, strong) Vec3D* lowerLimit;

/** The higher MMORoom coordinates
 @see Vec3D
 @see MMORoom
 */
@property (readonly, nonatomic, strong) Vec3D* higherLimit;

/**
 @param lower the lower coordinate limit
 @param higher the higher coordinate limit
 @see MMORoom
 @see MMORoomSettings
 */
+(id) limitWithLowerValue:(Vec3D*)lower higherValue:(Vec3D*)higher;

@end
