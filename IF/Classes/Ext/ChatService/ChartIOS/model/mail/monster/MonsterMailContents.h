//
//  MonsterMailContents.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>
#import "DefParams.h"
#import "AttParams.h"
@interface MonsterMailContents : NSObject
@property (nonatomic,strong) NSString *uid;
@property (nonatomic,strong) NSString * type;
@property (nonatomic,strong) NSString * createTime;
@property (nonatomic,strong) DefParams *def;
@property (nonatomic,strong) AttParams *att;
//泛型 RateRewardParams
@property (nonatomic,strong) NSMutableArray *rateReward;
@property (nonatomic,strong) NSString *reportUid;
@property (nonatomic,strong) NSString * xy;
@property (nonatomic,strong) NSString * firstKill;
@property (nonatomic,strong) NSString * stat;
@end
