//
//  ScienceParams.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "ScienceParams.h"
#import "MJExtension.h"
@implementation ScienceParams

-(instancetype)init
{
    self = [super init];
    if (self) {
        [ScienceParams setupReplacedKeyFromPropertyName:^NSDictionary *{
            return @{
                     @"ID" : @"id"
                     };
        }];
    }
    return self;
}

@end
