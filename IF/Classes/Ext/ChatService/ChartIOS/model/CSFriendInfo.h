//
//  CSFriendInfo.h
//  IF
//
//  Created by 马纵驰 on 15/10/20.
//
//

#import <Foundation/Foundation.h>

@interface CSFriendInfo : NSObject

@property (nonatomic,strong) NSString *pic;
@property (nonatomic,assign) int picVer;
@property (nonatomic,assign) int serverId;
@property (nonatomic,assign) int relation;
@property (nonatomic,assign) double vipEndTime;
@property (nonatomic,strong) NSString *allianceId;
@property (nonatomic,assign) int vipLevel;
@property (nonatomic,strong) NSString *uid;
@property (nonatomic,assign) int crossFightSrcServerId;
@property (nonatomic,assign) double offLineTime;
@property (nonatomic,assign) int gmFlag;
@property (nonatomic,strong) NSString *name;
@property (nonatomic,assign) int online;
@property (nonatomic,assign) int rank;
@property (nonatomic,assign) double power;
@property (nonatomic,strong) NSString *abbr;
@property (nonatomic,strong) NSString *lang;
@property (nonatomic,assign) int mainBuildingLevel;
@property (nonatomic,strong) NSString *friendDescription;
/**
 *  通过c++模型数据转换IOS模型数据
 *
 *  @param ccFriendInfo c++模型数据
 *
 *  @return IOS模型数据
 */
- (instancetype)initByPic:(NSString*)pic withPicVer:(int)picVer withServerId:(int)serverId withRelation:(int)relation withVipEndTime:(double)vipEndTime withAllianceId:(NSString*)allianceId withVipLevel:(int)vipLevel withUid:(NSString*)uid withCrossFightSrcServerId:(int)crossFightSrcServerId withOffLineTime:(double)offLineTime withGmFlag:(int)gmFlag withName:(NSString*)name withOnline:(int)online withRank:(int)rank withPower:(double)power withAbbr:(NSString*)abbr withLang:(NSString*)lang withMainBuildingLevel:(int)mainBuildingLevel withFriendDescription:(NSString*)friendDescription;
@end
