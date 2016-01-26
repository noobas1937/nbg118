//
//  MailData.h
//  IF
//
//  Created by 马纵驰 on 15/9/16.
//
//

#import <Foundation/Foundation.h>
#import "MailInfoDataModel.h"
#import "NSString+JSONCategories.h"
#import "NSDictionary+DataToJsonString.h"
#import "MJExtension.h"

@interface MailData : NSObject
/**
 *  java 数据结构父类属性
 */
@property (nonatomic,strong) NSString *uid;
@property (nonatomic,strong) NSString *fromUid;
@property (nonatomic,strong) NSString *title;
@property (nonatomic,strong) NSString *fromName;
@property (nonatomic,strong) NSString *createTime;
@property (nonatomic,strong) NSString *rewardStatus;
@property (nonatomic,strong) NSString *type;
@property (nonatomic,assign) int itemIdFlag;
@property (nonatomic,strong) NSString *pic;
@property (nonatomic,strong) NSString *status;
@property (nonatomic,strong) NSString *reply;
@property (nonatomic,strong) NSString *save;
@property (nonatomic,strong) NSString *mbLevel;
@property (nonatomic,strong) NSString *contents;
@property (nonatomic,strong) NSString *rewardId;
@property (nonatomic,strong) NSString *version;
@property (nonatomic,assign) int saveFlag;  //锁定邮件

/**
 *  运行时属性
 */
@property (nonatomic,assign) int tabType ;
@property (nonatomic,assign) BOOL isAtk ;
@property (nonatomic,assign) BOOL hasParseCompex ;
@property (nonatomic,assign) BOOL hasParseBy2dx ;
@property (nonatomic,assign) BOOL hasParseLocal ;
@property (nonatomic,strong) NSString *language ;
@property (nonatomic,assign) int parseVersion ;


/**
 *  显示属性
 */
@property (nonatomic,strong) NSString * nameText;
@property (nonatomic,strong) NSString * contentText;
@property (nonatomic,strong) NSString * mailIcon;
@property (nonatomic,strong) NSString * timeText;
@property (nonatomic,assign) BOOL usePersonalPic;
@property (nonatomic,strong) NSString * channelId;

/**
 *  解析完成后 mode 转换 json_nsstring
 */
@property (nonatomic,strong) NSString *jsonStr;
-(instancetype)initWithAddMailData:(MailInfoDataModel *)maildata;

/**
 *  解析 Json 2 Mode
 */
-(void) parseContents;

/**
 *  解析 Mode 2 Json
 */
-(void) parseMode2Json;

-(void) settingMailDealStatus;

-(void) changeMailRewardStatus;
@end
