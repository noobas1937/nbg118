//
//  OcupyMailContents.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "OcupyMailContents.h"
#import "MJExtension.h"
@implementation OcupyMailContents
-(instancetype)init
{
    self = [super init];
    if (self) {
        //规定 集合泛型
        [self mappingList];
        //映射K值
//        [self mappingKey];
    }
    return self;
}

-(void) mappingList
{
    [OcupyMailContents setupObjectClassInArray:^NSDictionary *{
        return @{
                 @"arms" : @"ArmysParams"
                 };
    }];
}

//-(void) mappingKey
//{
//    [OcupyMailContents setupReplacedKeyFromPropertyName:^NSDictionary *{
//        return @{
//                 @"user" : @"UserInfoParams"
//                 };
//    }];
//}
@end
