//
//  RateRewardParams.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>
#import "RateRewardValueParams.h"

@interface RateRewardParams : NSObject
@property (nonatomic,strong) RateRewardValueParams *value;
@property (nonatomic,strong) NSString *type;
@end
