//
//  UpdateParam.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "UpdateParam.h"
#import "MJExtension.h"

@implementation UpdateParam

-(instancetype)init
{
    self = [super init];
    if (self) {
        [self mappingList];
        [self mappingKey];
    }
    return self;
}

-(void) mappingList
{
    [UpdateParam setupObjectClassInArray:^NSDictionary *{
        return @{
                 @"Delete" : @"NSString"
                 };
    }];
}

-(void) mappingKey
{
    [UpdateParam setupReplacedKeyFromPropertyName:^NSDictionary *{
        return @{
                 @"Delete" : @"delete"
                 };
    }];
}
@end
