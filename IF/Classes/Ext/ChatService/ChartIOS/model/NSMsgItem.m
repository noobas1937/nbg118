//
//  NSMsgItem.m
//  IF
//
//  Created by mzc on 15/6/25.
//
//

#import "NSMsgItem.h"
#import "UserManager.h"
#import "LanguageManager.h"
#import "LanguageKeys.h"
#import "ChatServiceController.h"
#import "ServiceInterface.h"

#import "LKDBHelper.h"

@interface NSMsgItem ()


@end
@implementation NSMsgItem

-(instancetype) initSend:(NSString *)uidStr
                isNewMsg:(BOOL)isNewMsg
                  isSelf:(BOOL)isSelf
             channelType:(int)channelType
                    post:(int)post
                  msgStr:(NSString *)msgStr
           sendLocalTime:(NSString *)sendLocalTime
{
    self = [super init];
    if (self) {
        self.uid = uidStr;
        self.isNewMsg = isNewMsg;
        self.isSelfMsg = isSelf && (post!=100);
        self.channelType = channelType;
        self.post = post;
        self.msg = msgStr;
        if ([self hasTranslation])
            self.hasTranslated = true;
        else
            self.hasTranslated = false;
        self.sendLocalTime = sendLocalTime;
        self.createTime = [sendLocalTime integerValue];
        self.customHeadPicUrl =[[ChatServiceController chatServiceControllerSharedManager].gameHost gettingCustomHeadPic:self.uid int:[UserManager sharedUserManager].currentUser.headPicVer];
    }
    return self;
}

-(instancetype) init
{
    self = [super init];
    if (self) {
        self.localLanString = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingLocalLanString];
        self.translateMsg = @"";
        self.originalLang = @"";
        self.translatedLang = @"";
    }
    return self;
}

-(BOOL) hasTranslation
{
    //如果有翻译信息显示翻译信息
    if (self.translateMsg.length>0  ||!self.translateMsg) {
        self.hasTranslated = TRUE;
        return TRUE;
    }else{
        self.hasTranslated = FALSE;
        return FALSE;
    }
}

-(NSUserInfo*)  gettingUserInfo
{
    NSUserInfo *user = [[UserManager sharedUserManager] gettingUser:self.uid];
    if(user == nil){
        user =[[ UserManager sharedUserManager] gettingUserInforWithUid:self.uid];
        if (user == nil) {
            NSUserInfo *result = [[NSUserInfo alloc]init:self.uid];
            [[UserManager sharedUserManager] addUser:result];
            return result;
        }else{
            [[UserManager sharedUserManager] addUser:user];
            return user;
        }
    }
    return user;
}

-(NSString*)    gettingUserName
{
    return  [self gettingUserInfo].userName;
}

-(NSString*)    gettingServerId
{
    return [NSString stringWithFormat:@"%d",[self gettingUserInfo].crossFightSrcServerId];
}

-(NSString*)    gettingAsn
{
    if (self.channelType == IOS_CHANNEL_TYPE_ALLIANCE) {
        return @"";
    }else{
        return [self gettingUserInfo].asn;
    }
    
}

-(NSString*)    gettingHeadPic
{
    return [self gettingUserInfo].headPic;
}

-(int)          gettingGmod
{
    return [self gettingUserInfo].mGmod;
}

-(int)          HeadPicVer
{
    return [self gettingUserInfo].headPicVer;
}

-(NSString*) gettingVip
{
    return [[self gettingUserInfo] gettingVipInfo];
}

-(BOOL) isInAlliance
{
    if([[self gettingUserInfo].asn isEqualToString:@""]){
        return FALSE;
    }else{
        return TRUE;
    }
}

-(NSString*) gettingNSUserInfoLabel
{
    
    NSString *userVip = [self gettingVip];
    NSString *userAsn = [self gettingAsn];
    NSString *userName = [self gettingUserName];
    
    if(!userVip || [userVip isEqualToString:@""] || [userVip isEqualToString:@"0"]){
        userVip = @"";
    }else{
        userVip = [userVip stringByAppendingString:@" "];
    }
    
    if(!userAsn || [userAsn isEqualToString:@""]){
        userAsn = @"";
    }else{
        NSMutableString * asnMStr=[[NSMutableString alloc]init];
        asnMStr.string = userAsn;
        [asnMStr insertString:@"(" atIndex:0];
        [asnMStr appendString:@")"];
        userAsn = [NSString stringWithString:asnMStr];
    }
    
    if(!userName){
        userName = @"";
    }
    
    NSString *userInfoLabel = [userVip stringByAppendingString:userAsn];
    userInfoLabel = [userInfoLabel stringByAppendingString:userName];
    
    if ([self gettingUserInfo].crossFightSrcServerId > 0) {
        userInfoLabel = [userInfoLabel stringByAppendingString:@"#"];
        userInfoLabel = [userInfoLabel stringByAppendingString:[NSString stringWithFormat:@"%d",[self gettingUserInfo].crossFightSrcServerId]];
    }
    
    //userInfoLabel = [self addCrossFightMsg:userInfoLabel];
    
    return userInfoLabel;
}

-(void)settingMsgUserNickString{
    NSString *userVip = self.vip;
    NSString *userAsn = self.asn;
    NSString *userName = self.name;
    
    if(!userVip || [userVip isEqualToString:@""] || [userVip isEqualToString:@"0"]){
        userVip = @"";
    }else{
        userVip = [userVip stringByAppendingString:@" "];
    }
    
    if(!userAsn || [userAsn isEqualToString:@"" ]|| self.channelType == 1){
        userAsn = @"";
    }else{
        NSMutableString * asnMStr=[[NSMutableString alloc]init];
        asnMStr.string = userAsn;
        [asnMStr insertString:@"(" atIndex:0];
        [asnMStr appendString:@")"];
        userAsn = [NSString stringWithString:asnMStr];
    }
    
    if(!userName){
        userName = @"";
    }
    
    NSString *userInfoLabel = [userVip stringByAppendingString:userAsn];
    userInfoLabel = [userInfoLabel stringByAppendingString:userName];
    
    if ([self gettingUserInfo].crossFightSrcServerId > 0) {
        userInfoLabel = [userInfoLabel stringByAppendingString:@"#"];
        userInfoLabel = [userInfoLabel stringByAppendingString:[NSString stringWithFormat:@"%d",[self gettingUserInfo].crossFightSrcServerId]];
    }
    
    //userInfoLabel = [self addCrossFightMsg:userInfoLabel];
    
    self.showUserInfoLabel = userInfoLabel;

}

/**判断当前msg是否属于跨服*/
-(BOOL) isCrossFight
{

    if ([self gettingUserInfo].crossFightSrcServerId > 0) {
        return true;
    }else{
        return false;
    }
    
}

/**加入跨服信息*/
-(NSString*) addCrossFightMsg:(NSString*) userInfoLabel
{
    if([self isCrossFight])
    {
        userInfoLabel = [userInfoLabel stringByAppendingString:@"#"];
        userInfoLabel = [userInfoLabel stringByAppendingString:[NSString stringWithFormat:@"%d",[self gettingUserInfo].crossFightSrcServerId]];
        NSLog(@"%@",userInfoLabel);
        return userInfoLabel;
    }
    return @"";
}

-(BOOL) isSystemMessage
{
    if (self.post != 0){
        return TRUE;
    }else{
        return FALSE;
    }
}

-(NSString *)gettingSendTimeFormat
{
    //获取当前系统时间 格式化
    NSDate *data=[NSDate date];
    NSDateFormatter  *dateformatter=[[NSDateFormatter alloc] init];
    [dateformatter setDateFormat:@"YYYY-MM-dd  HH:mm:ss"];
    NSString *sendTime=[dateformatter stringFromDate:data];
    return sendTime;
}

/**包装需要显示的信息
 *
 *  1 用户信息label vip asn name
 *  2 需要显示的内容 内容体 包括 系统图标 文字 战报
 *  3 需要显示信息的结尾 包括 时间 翻译信息
 *
 */
-(void)packedMsg
{
 
    if (!self.isSelfMsg) {
        [self settingCustomHead];
    }
//    self.showUserInfoLabel = [self gettingNSUserInfoLabel];
    [self settingMsgUserNickString];
    self.showMsg = [self gettingShowMsg];
    self.endMsg = [self gettingEndMsg];
    [self settingCustomHead];
    self.gmodStr = [self gettingGmodStr];
    
}


/**设置自定义头像*/
-(void) settingCustomHead
{
    if([[self gettingUserInfo] isCustomHeadImage])
    {
        self.customHeadPicUrl = [[self gettingUserInfo] gettingCustomHeadPicUrl];
        self.customHeadPicPath = [[self gettingUserInfo] gettingCustomHeadPicPath];
    }
    
}

-(void) updateMsg
{
    self.showUserInfoLabel = [self gettingNSUserInfoLabel];
    self.gmodStr = [self gettingGmodStr];
    [self settingCustomHead];
    
    
}

-(NSString *)gettingGmodStr
{
    //添加gmod图标
    if ([self gettingGmod] == 2) {
        return @"mod.png";
    }else if([self gettingGmod] == 3){
        return @"gm.png";
    }else if([self gettingGmod] == 4){
        return @"smod.png";
    }else if([self gettingGmod] == 5){
        return @"tmod.png";
    }else{
        return @"";
    }
}

-(NSString *)gettingEndMsg
{
    NSString *timeStr = [self nsDataSwitchNSString:self.createTime];
    NSString *endMsg = @"";
    //设置聊天结尾信息 条件 1：不是自己的 2：被翻译过 3：原文字符串和翻译字符串不相等
    if (!self.isSelfMsg && [self hasTranslation] && ![self.msg isEqualToString:self.translateMsg]) {
        return [self settingEndTranslateMsg:timeStr endMsg:endMsg isShowTranslateMsg:self.hasTranslated];
    }else{
        return [self subStringWithTime:timeStr];
    }
}
/**
 *  设置结尾翻译信息
 *
 *  @param time   信息发送时间
 *  @param endMsg 如果被翻译过 时间 + 由us翻译而来或原文 例如（00:00 由en翻译而来 || 00:00 原文）
 *  @param flag   是否被翻译过
 *
 *  @return 返回结尾信息字符串
 */
-(NSString*) settingEndTranslateMsg:(NSString*) time endMsg:(NSString*) endMsg isShowTranslateMsg:(BOOL) flag
{
    if (flag) {
        //结尾翻译
        NSString *translatedStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_TRANSLATED_BY :self.originalLang];
        return endMsg = [self endMsgAppending:translatedStr timeStr:time];
        
    }else{
        //结尾原文
        NSString *originallanStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_ORIGINALLAN];
        return endMsg = [self endMsgAppending:originallanStr timeStr:time];
    }
    
}

-(NSString*) endMsgAppending:(NSString*) msg timeStr:(NSString*)timeStr
{
    timeStr = [self subStringWithTime:timeStr];
    return [timeStr stringByAppendingFormat:@"%@%@",@" ", msg];
}

-(NSString*) subStringWithTime:(NSString*)timeStr
{
    return [timeStr substringWithRange:NSMakeRange(11,5)];
}

/**时间戳转字符串*/
-(NSString*) nsDataSwitchNSString:(NSUInteger) time
{
    NSDate *data = [NSDate dateWithTimeIntervalSince1970:time];
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSString *timeStr = [dateFormatter stringFromDate:data];
    return timeStr;
}

/**获取需要显示的内容信息
 *
 *  1 翻译信息或者原文
 *  2 是否加入系统图标
 *  3 是否加入战报图标
 *
 */
-(NSString*) gettingShowMsg
{
    NSMutableString *showMsg = [[NSMutableString alloc]init];
    [self settingMsg:showMsg];
    [self settingSystemIcon:showMsg];
    [self settingReport:showMsg];
//    DVLog(@"gettingShowMsg: %@",showMsg);
    return [NSString stringWithFormat:@"%@",showMsg];
}


-(void)settingShowMsg:(NSString *)vlocalLanString{
    NSString *showMsg =@"";
    if ([self isShowTransLate]) {
        //显示了翻译
       showMsg =  [showMsg stringByAppendingString:self.msg];
    }else{
       showMsg =  [showMsg stringByAppendingString:self.translateMsg];
    }
    if (self.post != 0 && self.post != 6 && self.post != 100) {
        NSString *sysImage=@"[sysChat]";
      showMsg =   [sysImage stringByAppendingString:showMsg];
    }
    
    if (self.post == 4 || self.post == 5) {
        NSString *battlereportImage=@"[battlereport]";
        showMsg = [showMsg stringByAppendingString:battlereportImage];
    }
    self.showMsg = showMsg;
    
    
    NSString *timeStr = [self nsDataSwitchNSString:self.createTime];
    NSString *endMsg = @"";
    
    if (![self.msg isEqualToString:self.translateMsg]) {
        self.endMsg = [self settingEndTranslateMsg:timeStr endMsg:endMsg isShowTranslateMsg:!self.isShowTransLate];
    }else{
        self.endMsg = [self subStringWithTime:timeStr];
    }

}

/**是否加入系统图标*/
-(void) settingSystemIcon:(NSMutableString*) showMsg
{
    //是否添加系统图标 暂时注释 原因是图片渲染不上
    if (self.post != 0 && self.post != 6 && self.post != 100) {
        NSString *sysImage=@"[sysChat]";
        [showMsg insertString:sysImage atIndex:0];
    }
}

/**根据翻译信息 设置显示到屏幕上的信息*/
-(void) settingMsg:(NSMutableString*) showMsg
{
    if ([self hasTranslation]) {
        [showMsg appendString:self.translateMsg];
    }else{
        [showMsg appendString:self.msg];
    }
}

//添加战报  暂时注释 原因是图片渲染不上
-(void) settingReport:(NSMutableString*) showMsg
{
    //普通战报
    if (self.post == 4 || self.post == 5) {
        NSString *battlereportImage=@"[battlereport]";
        [showMsg appendString:battlereportImage];
    }
    
}

-(BOOL) isBattleReport
{
    return self.post == 4;
}

-(BOOL) isDetectReport
{
    return self.post == 5;
}

-(BOOL) isAnnounceInvite
{
    return self.post == 3;
}

-(UIImage *) gettingCustomHeadPicImg
{
//    DVLog(@"%@",self.customHeadPicPath);
    UIImage *image =[[UIImage alloc]initWithContentsOfFile:self.customHeadPicPath];
    return image;
    
}

-(void) initUserForReceivedMsg
{
    
    NSUserInfo *user = [self gettingUserInfo];
    
//    NSLog(@"self.lastUpdateTime == %d,",self.lastUpdateTime);
//    NSLog(@"user.lastUpdateTime == %d,",user.lastUpdateTime);
    
    if (user == nil || self.lastUpdateTime > user.lastUpdateTime)
    {
        if (user == nil){
            //				System.out.println("user == null");
            user = [[NSUserInfo alloc]init:self.uid];
            [[UserManager sharedUserManager] addUser:user];
        }
        
        // 从后台取userInfo
        [[UserManager sharedUserManager] gettingServicerUser:self.uid];
    }else{
        self.name = user.userName;
        self.asn = user.asn;
        self.vip = user.vip;
        self.headPic = user.headPic;
        self.headPicVer = user.headPicVer;
    }
    
}


-(void)initMsgUserWithBlock:(MsgUserBlock)vBlock{
//    NSUserInfo *user = [self gettingUserInfo];
    NSUserInfo *user = [[UserManager sharedUserManager] gettingUser:self.uid];
 
    if (user == nil || self.lastUpdateTime > user.lastUpdateTime)
    {
//        if (user == nil){
//            //				System.out.println("user == null");
//            user = [[NSUserInfo alloc]init:self.uid];
//            [[UserManager sharedUserManager] addUser:user];
//        }
        
        // 从后台取userInfo
        [[UserManager sharedUserManager] gettingServicerUser:self.uid];
    }else{
        if  (user.isDummy == YES){
           NSUserInfo *tempInfo =  [[UserManager sharedUserManager]gettingUserInforWithUid:self.uid];
            if (tempInfo.isDummy == NO) {
                user  = tempInfo;
                if (vBlock != nil) {
                    vBlock(user);
                }
            }
        }else{
            if (vBlock != nil) {
                vBlock(user);
            }
        }

    }
}
-(BOOL)playerIsVip
{
    return [[self gettingUserInfo].vip isEqualToString:@""] && [self gettingUserInfo].vip.length > 0;
}



#pragma mark -
#pragma mark DB 相关

// 将要插入数据库
+(void)dbWillInsert:(NSObject *)entity
{
    LKLog(@"will insert : %@",NSStringFromClass(self));
    
}
//已经插入数据库
+(void)dbDidInserted:(NSObject *)entity result:(BOOL)result
{
    if (result) {
         LKLog(@"did insert Flag: %d\n完成插入的数据:%@",result,[entity printAllPropertys] );
    }else{
         LKLog(@"did insert Flag: %d\n插入失败的数据:%@",result,[entity printAllPropertys] );
    }
   
}

+(void)dbWillDelete:(NSObject *)entity{
    LKLog(@"will Delete : %@",NSStringFromClass(self));
    
}
+(void)dbDidDeleted:(NSObject *)entity result:(BOOL)result{
    
    LKLog(@"did Deleted Flag: %d\n完成删除的数据:%@",result,[entity printAllPropertys] );
}

+(void)dbWillUpdate:(NSObject *)entity{
    LKLog(@"will Update : %@",NSStringFromClass(self));
    
}
+(void)dbDidUpdated:(NSObject *)entity result:(BOOL)result{
    
    LKLog(@"did Updated Flag: %d\n完成更新的数据:%@",result,[entity printAllPropertys] );
}
+(void)dbDidSeleted:(NSObject *)entity{
    LKLog(@"did seleted %@",[entity printAllPropertys]);
}


//+(NSString *)getPrimaryKey
//{
//    return @"sequenceId";
//}
+(int)getTableVersion
{
    return 1;
}
//表名
//+(NSString *)getTableName
//{
//    return @"User";
//}
//更新数据模型后，需要调用

+(NSDictionary *)getTableMapping{
    return @{@"TableVer"             :@"tableVer",
             @"SequenceID"           :@"sequenceId",
             @"MailID"               :@"mailId",
             @"UserID"               :@"uid",
             @"ChannelType"          :@"channelType",
             @"CreateTime"           :@"createTime",
             @"LocalSendTime"        :@"sendLocalTime",
             @"Type"                 :@"post",
             @"Msg"                  :@"msg",
             @"Translation"          :@"translateMsg",
             @"OriginalLanguage"     :@"originalLang",
             @"TranslatedLanguage"   :@"translatedLang",
             @"Status"               :@"sendState",
             @"AttachmentId"         :@"attachmentId",
             @"Media"                :@"media"
//             @"UserName"             :@"name",
//             @"Asn"                  :@"asn",
//             @"Vip"                  :@"vip",
//             @"HeadPicVer"           :@"headPic",
//             @"HeadPicVer"           :@"headPicVer"
             };
}
+(LKTableUpdateType)tableUpdateWithModelTableName:(NSString *)vTableName
                                 andForOldVersion:(int)oldVersion
                                       newVersion:(int)newVersion{
    if (newVersion != oldVersion) {
        
        //        [self tableUpdateAddColumeWithModelTableName:vTableName andWithPN:@"tempArray"];
    }
    
    return LKTableUpdateTypeCustom;
    
}
+(void)tableUpdateAddColumeWithModelTableName:(NSString *)vTableName andWithPN:(NSString*)propertyName{
    
    [super tableUpdateAddColumeWithModelTableName:vTableName andWithPN:propertyName];
}



@end
