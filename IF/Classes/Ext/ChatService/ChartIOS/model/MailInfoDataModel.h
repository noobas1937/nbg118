//
//  MailInfoDataModel.h
//  IF
//
//  Created by 张彦涛 on 15/8/17.
//
//

#import <Foundation/Foundation.h>
#import "MailManagerKey.h"
@class MailData;

@interface MailInfoDataModel : NSObject

//DB属性
@property (nonatomic,assign) int tableVer;
@property (nonatomic,assign)    long        _id;
@property (nonatomic,strong) NSString * mail_ID;
@property (nonatomic,strong) NSString * channelID;
@property (nonatomic,strong) NSString * fromUserUid;
@property (nonatomic,strong) NSString * fromName;
@property (nonatomic,strong) NSString * title;
@property (nonatomic,strong) NSString * contents;
@property (nonatomic,strong) NSString * rewardId;//奖励
@property (nonatomic,assign) int itemIdFlag; //title 或者 contents为多语言
@property (nonatomic,assign) int status; // 是否未读，大于1则是
@property (nonatomic,assign) int type;
@property (nonatomic,assign) int mailRewardLevel;//邮件奖励领取条件，比较主城等级
@property (nonatomic,assign) int rewardStatus;//奖励领取状态
@property (nonatomic,assign) int saveFlag;  //锁定邮件
@property (nonatomic,assign) long  creatTime;
@property (nonatomic,assign) int reply;
@property (nonatomic,strong) NSString * translation;
@property (nonatomic,strong) NSString * nameText;
@property (nonatomic,strong) NSString * contentText;
@property (nonatomic,strong) NSString * language;
@property (nonatomic,strong) NSString * parseVersion;
@property (nonatomic,assign) int save;
@property (nonatomic,assign) int mbLevel;
@property (nonatomic,strong) NSString *version;

//过程属性
@property (nonatomic,assign) int tabType;//邮件大类型
@property (nonatomic,strong) NSString * picString;//头像
@property (nonatomic,strong) NSArray *contentsArr;//contents分割数组
@property (nonatomic,assign )BOOL isSelected;//邮件列表中，是否被选择

/**
 *  资源json解析对象
 */
@property (nonatomic,strong) MailData *maildata;
-(void)mailInfoDataSettingTabType;
-(void)mailInfoDataSettingChannelID;

-(NSString *)mailInfoDataForChannelWithSelf;

-(void)upDatingForDB;

-(void)settingToReadForDB;
-(BOOL) isWorldBossKillRewardMail;
-(BOOL)isKNightFightReport;
-(BOOL )isGrandCastleInvestigation;
+(MailInfoDataModel *)gettingLastSystemMailFromDB;
+(NSString *)channelStringWithMailType:(int)vMailType;
@end
