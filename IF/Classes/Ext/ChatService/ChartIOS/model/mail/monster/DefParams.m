//
//  DefParams.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "DefParams.h"
#import "MJExtension.h"

@implementation DefParams
-(instancetype)init
{
    self = [super init];
    if (self) {
        [DefParams setupReplacedKeyFromPropertyName:^NSDictionary *{
            return @{
                     @"ID" : @"id"
                     };
        }];
    }
    return self;
}
@end
