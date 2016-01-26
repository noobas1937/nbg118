//
//  FortParams.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "FortParams.h"
#import "MJExtension.h"

@implementation FortParams
-(instancetype)init
{
    self = [super init];
    if (self) {
        [FortParams setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"forts" : @"ArmsParams"
                     };
        }];
    }
    return self;
}
@end
