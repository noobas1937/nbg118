//
//  DetectReportMailContents.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "DetectReportMailContents.h"
#import "MJExtension.h"
@implementation DetectReportMailContents
-(instancetype)init
{
    self = [super init];
    if (self) {
        [DetectReportMailContents setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"rein_detail" : @"ReinDetailParams",
                     @"tower" : @"TowerParams",
                     @"rein_about_detail" : @"NSMutableArray",
                     @"science" : @"ScienceParams",
                     @"ability" : @"NSString",
                     @"atkWarEffect" : @"NSDictionary",
                     @"dfWarEffect" : @"NSDictionary"
                     };
        }];
    }
    return self;
}
@end
