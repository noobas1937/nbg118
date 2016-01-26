//
//  JsonToMailBattleData.m
//  IF
//
//  Created by 马纵驰 on 15/9/16.
//
//

#import "BattleMailContents.h"
#import "MJExtension.h"
@implementation BattleMailContents

-(instancetype)init
{
    self = [super init];
    if (self) {
        [BattleMailContents setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"atkHelpReport" : @"HelpReportParams",
                     @"defHelpReport" : @"HelpReportParams",
                     
                     @"atkHelper" : @"NSString",
                     @"defHelper" : @"NSString",
                     
                     @"defGen" : @"GenParams",
                     @"atkGen" : @"GenParams",
                     
                     @"atkReport" : @"ArmyParams",
                     @"defReport" : @"ArmyParams",
                     
                     @"atkWarEffect" : @"NSString",
                     @"dfWarEffect" : @"NSString",
                     
                     @"defFortLost" : @"ArmyParams",
                     
                     @"reward" : @"RewardParams",
                     
                     @"defTowerKill" : @"TowerKillParams"
                     
//                     @"atkGenKill" : @"NSString",
//                     @"defGenKill" : @"NSString"
                     };
        }];
    }
    return self;
}

@end
