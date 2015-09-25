//
//  MailInfoDataModel.h
//  IF
//
//  Created by 张彦涛 on 15/8/17.
//
//

#import <Foundation/Foundation.h>
#import "MailManagerKey.h"


@interface MailInfoDataModel : NSObject
@property (nonatomic,strong) NSString * mail_ID;
@property (nonatomic,strong) NSString * channelID;
@property (nonatomic,strong) NSString * fromUserUid;
@property (nonatomic,strong) NSString * fromName;
@property (nonatomic,strong) NSString * rewardId;
@property (nonatomic,strong) NSString * title;
@property (nonatomic,strong) NSString * contents;
@property (nonatomic,assign) long  creatTime;
@property (nonatomic,strong) NSString * language;
@property (nonatomic,strong) NSString * contentText;
@property (nonatomic,assign) int type;
@property (nonatomic,assign) int reply;
@property (nonatomic,assign) int itemIdFlag;
@property (nonatomic,assign) int saveFlag;
@property (nonatomic,assign) int status; // 是否未读，大于1则是
@property (nonatomic,assign) int rewardStatus;//有没有礼物
@property (nonatomic,assign) int tabType;//邮件大类型
 
@property (nonatomic,strong) NSString * picString;//头像
@property (nonatomic,assign) int picVer ;//自定义头像


-(void)mailInfoDataSettingTabType;
-(void)mailInfoDataSettingChannelID;
@end
