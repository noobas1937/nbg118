//
//  OcupyMailContents.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>
#import "OcupyUserInfoParams.h"

@interface OcupyMailContents : NSObject
@property (nonatomic,strong)  NSString *pointId;
//泛型 ArmysParams
@property (nonatomic,strong)  NSMutableArray *arms;
//泛型 OcupyUserInfoParams 映射K值 UserInfoParams
@property (nonatomic,strong)  OcupyUserInfoParams *user;
@property (nonatomic,strong)  NSString *pointType;
@property (nonatomic,strong)  NSString *isTreasureMap;
@property (nonatomic,strong)  NSString *ckf;
@property (nonatomic,strong)  NSString *serverType;
@end
