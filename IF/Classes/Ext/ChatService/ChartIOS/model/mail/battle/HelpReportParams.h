//
//  HelpReportParams.h
//  IF
//
//  Created by 马纵驰 on 15/9/16.
//
//

#import <Foundation/Foundation.h>

@interface HelpReportParams : NSObject
/**
 *  泛型为 NSInteger
 */
@property (nonatomic,strong) NSMutableArray *genKill;
/**
 *  泛型为 GenParams
 */
@property (nonatomic,strong) NSMutableArray *genInfo;

/**
 *  泛型为 ArmyParams
 */
@property (nonatomic,strong) NSMutableArray *armyInfo;

@property (nonatomic,strong) NSString *name;

@end
