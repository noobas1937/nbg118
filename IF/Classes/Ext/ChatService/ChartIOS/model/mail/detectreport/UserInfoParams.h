//
//  UserInfoParams.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>

@interface UserInfoParams : NSObject
@property (nonatomic,strong) NSString * level;
@property (nonatomic,strong) NSString * name;
@property (nonatomic,strong) NSString * pointId;
@property (nonatomic,strong) NSString * abbr;
@property (nonatomic,strong) NSString * pic;
@property (nonatomic,strong) NSString * picVer;
@property (nonatomic,strong) NSString * allianceName;
@property (nonatomic,strong) NSString * noDef;
@property (nonatomic,strong) NSString * count;
@property (nonatomic,assign) int  serverType ;

@end
