//
//  ArmyParams.h
//  IF
//
//  Created by 马纵驰 on 15/9/16.
//
//

#import <Foundation/Foundation.h>

@interface ArmyParams : NSObject

/**
 *  java 类型 int
 */
@property (nonatomic,strong) NSString *exp;

@property (nonatomic,strong) NSString *armId;

/**
 *  以下全部 java 类型为 int
 */
@property (nonatomic,strong) NSString *num;
@property (nonatomic,strong) NSString *hurt;
@property (nonatomic,strong) NSString *kill;
@property (nonatomic,strong) NSString *dead;
@property (nonatomic,strong) NSString *star;

@end
