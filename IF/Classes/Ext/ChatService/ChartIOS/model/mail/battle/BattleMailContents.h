//
//  BattleMailContents.h
//  IF
//
//  Created by 马纵驰 on 15/9/16.
//
//

#import <Foundation/Foundation.h>
#import "ArmyTotalParams.h"
#import "UserParams.h"
#import "AllianceParams.h"
#import "HelpReportParams.h"
#import "CkfWarInfoParams.h"
#import "MailInfoDataModel.h"

@interface BattleMailContents : NSObject
@property (nonatomic,assign) BOOL isResourceShieldState;
@property (nonatomic,strong) NSString *atkPowerLost;
@property (nonatomic,strong) NSString *defPowerLost;
@property (nonatomic,strong) NSString *winner;
@property (nonatomic,strong) NSString *battleType;
@property (nonatomic,strong) NSString *reportUid;
@property (nonatomic,strong) NSString *pointType;
@property (nonatomic,strong) NSString *warPoint;

@property (nonatomic,strong) ArmyTotalParams *atkArmyTotal;
@property (nonatomic,strong) ArmyTotalParams *defArmyTotal;
@property (nonatomic,strong) UserParams *defUser;
@property (nonatomic,strong) UserParams *atkUser;
@property (nonatomic,strong) AllianceParams *atkAlliance;
@property (nonatomic,strong) AllianceParams *defAlliance;
/**
 *  泛型为 HelpReportParams
 */
@property (nonatomic,strong) NSMutableArray *atkHelpReport;
@property (nonatomic,strong) NSMutableArray *defHelpReport;
/**
 *  泛型为 NSString
 */
@property (nonatomic,strong) NSMutableArray *atkHelper;
@property (nonatomic,strong) NSMutableArray *defHelper;
/**
 *  泛型为 GenParams
 */
@property (nonatomic,strong) NSMutableArray *defGen;
@property (nonatomic,strong) NSMutableArray *atkGen;
/**
 *  泛型为 ArmyParams
 */
@property (nonatomic,strong) NSMutableArray *atkReport;
@property (nonatomic,strong) NSMutableArray *defReport;

/**
 *  泛型为 字典
 */
@property (nonatomic,strong) NSMutableArray *atkWarEffect;
@property (nonatomic,strong) NSMutableArray *dfWarEffect;
/**
 *  泛型为 ArmyParams
 */
@property (nonatomic,strong) NSMutableArray *defFortLost;

/**
 *  泛型为 RewardParams
 */
@property (nonatomic,strong) NSMutableArray *reward;

/**
 *  泛型为 TowerKillParams
 */
@property (nonatomic,strong) NSMutableArray *defTowerKill;

/**
 *  泛型为 NSInteger
 */
@property (nonatomic,strong) NSMutableArray *atkGenKill;
@property (nonatomic,strong) NSMutableArray *defGenKill;

/**
 *  以下对应java类型 int
 */
@property (nonatomic,strong) NSString *userKill;
@property (nonatomic,strong) NSString *failTimes;
@property (nonatomic,strong) NSString *winPercent;
@property (nonatomic,strong) NSString *monsterLevel;
@property (nonatomic,strong) NSString *userScore;
@property (nonatomic,strong) NSString *allKill;
@property (nonatomic,strong) NSString *msReport;
@property (nonatomic,strong) NSString *killRound;
@property (nonatomic,strong) NSString *defBeKilledCount;

@property (nonatomic,assign) BOOL defProtectActivate;

/**
 *  对应java类型 int
 */
@property (nonatomic,strong) NSString *level;
@property (nonatomic,strong) NSString *ckf;

@property (nonatomic,strong) CkfWarInfoParams *ckfWarInfo;

@property (nonatomic,assign) int serverType;
@property (nonatomic,strong) NSString *uid;
@property (nonatomic,strong) NSString *type;
@property (nonatomic,strong) NSString *createTime;

@end
