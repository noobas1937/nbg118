//
//  CSFriendInfo.m
//  IF
//
//  Created by 马纵驰 on 15/10/20.
//
//

#import "CSFriendInfo.h"

@implementation CSFriendInfo
-(instancetype)initByPic:(NSString *)pic withPicVer:(int)picVer withServerId:(int)serverId withRelation:(int)relation withVipEndTime:(double)vipEndTime withAllianceId:(NSString *)allianceId withVipLevel:(int)vipLevel withUid:(NSString *)uid withCrossFightSrcServerId:(int)crossFightSrcServerId withOffLineTime:(double)offLineTime withGmFlag:(int)gmFlag withName:(NSString *)name withOnline:(int)online withRank:(int)rank withPower:(double)power withAbbr:(NSString *)abbr withLang:(NSString *)lang withMainBuildingLevel:(int)mainBuildingLevel withFriendDescription:(NSString *)friendDescription
{
    self = [super init];
    if (self) {
        
        if (pic.length > 0) {
            self.pic = pic;
        }else{
            self.pic = @"";
        }
        
        if (allianceId.length > 0) {
            self.allianceId = allianceId;
        }else{
            self.allianceId = @"";
        }
        
        if (uid.length > 0) {
            self.uid = uid;
        }else{
            self.uid = @"";
        }
        
        if (name.length > 0) {
            self.name = name;
        }else{
            self.name = @"";
        }
        
        if (abbr.length > 0) {
            self.abbr = abbr;
        }else{
            self.abbr = @"";
        }
        
        if (lang.length > 0) {
            self.lang = lang;
        }else{
            self.lang = @"";
        }
        
        if (friendDescription.length > 0) {
            self.friendDescription = friendDescription;
        }else{
            self.friendDescription = @"";
        }
        
        if (picVer >= 0) {
            self.picVer = picVer;
        }else{
            self.picVer = 0;
        }
        
        if (serverId >= 0) {
            self.serverId = serverId;
        }else{
            self.serverId = 0;
        }
        
        if (vipEndTime >= 0.0) {
            self.vipEndTime = vipEndTime;
        }else{
            self.vipEndTime = 0.0;
        }
        
        if (vipLevel >= 0) {
            self.vipLevel = vipLevel;
        }else{
            self.vipLevel = 0;
        }
        
        if (crossFightSrcServerId >= 0) {
            self.crossFightSrcServerId = crossFightSrcServerId;
        }else{
            self.crossFightSrcServerId = 0;
        }
        
        if (offLineTime >= 0.0) {
            self.offLineTime = offLineTime;
        }else{
            self.offLineTime = 0.0;
        }
        
        if (gmFlag >= 0) {
            self.gmFlag = gmFlag;
        }else{
            self.gmFlag = 0;
        }
        
        if (online >= 0) {
            self.online = online;
        }else{
            self.online = 0;
        }
        
        if (rank >= 0) {
            self.rank = rank;
        }else{
            self.rank = 0;
        }
        
        if (power >= 0.0) {
            self.power = power;
        }else{
            self.power = 0.0;
        }
        
        if (mainBuildingLevel >= 0) {
            self.mainBuildingLevel = mainBuildingLevel;
        }else{
            self.mainBuildingLevel = 0;
        }
        
    }
    return self;
}

@end
