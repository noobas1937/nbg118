//
//  JsonToMailResourceData.m
//  IF
//
//  Created by 马纵驰 on 15/9/11.
//
//

#import "JsonToMailResourceData.h"
#import "RewardParams.h"
#import "ChatServiceController.h"
#import "DropParams.h"

@interface JsonToMailResourceData ()
@property (nonatomic,strong) NSString *uid;
@end

@implementation JsonToMailResourceData

-(instancetype)init
{
    self = [super init];
    if (self) {
        [JsonToMailResourceData setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"reward" : @"RewardParams",
                     @"drop" : @"DropParams"
                     };
        }];
        
    }
    return self;
}

@end
