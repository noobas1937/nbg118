//
//  GenParams.h
//  IF
//
//  Created by 马纵驰 on 15/9/16.
//
//

#import <Foundation/Foundation.h>

@interface GenParams : NSObject
@property (nonatomic,strong) NSString *level;
@property (nonatomic,strong) NSString *defence;
@property (nonatomic,strong) NSString *status;
@property (nonatomic,strong) NSString *att;
@property (nonatomic,strong) NSString *uuid;
@property (nonatomic,strong) NSString *generalId;
/**
 *  泛型为 SkillParams
 */
@property (nonatomic,strong) NSMutableArray *skill;
/**
 *  泛型为 NSString
 */
@property (nonatomic,strong) NSMutableArray *ability;


@end
