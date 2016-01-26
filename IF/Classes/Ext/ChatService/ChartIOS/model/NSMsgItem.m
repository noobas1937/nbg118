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
#import "NSString+Cocos2dHelper.h"
#import "TranslateManager.h"
#import "LKDBHelper.h"
#import "CSConfigManager.h"
#import "JSONKit.h"
#import "ChannelManager.h"
#import "MsgMessage.h"

#define MSG_TYPE_MOD  200
#define VERSIONPOST 13

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
        self.sendLocalTime = [sendLocalTime integerValue];
        self.createTime = [sendLocalTime integerValue];
        self.customHeadPicUrl =[[ChatServiceController chatServiceControllerSharedManager].gameHost gettingCustomHeadPic:self.uid int:[UserManager sharedUserManager].currentUser.headPicVer];
        self.changeColor2String = @"";
        self.fontColorStr = @"";
    }
    return self;
}

-(instancetype) init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

-(void) initAttribute
{
    self.localLanString = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingLocalLanString];
    
    if (self.translateMsg.length > 0 && ![self isTranlateDisable] && ![self isOriginalSameAsTargetLang])
        self.hasTranslated = true;
    else
        self.hasTranslated = false;
    
    if (self.originalLang.length <= 0) {
        self.originalLang = @"";
    }
    
    if (self.translateMsg.length <= 0) {
        self.translateMsg = @"";
    }
    
    if (self.changeColor2String.length <= 0) {
        self.changeColor2String = @"";
    }
    
    if (self.fontColorStr.length <= 0) {
        self.fontColorStr = @"";
    }
    
    [self initRandomHelloStr];
    
}

-(BOOL) isOriginalSameAsTargetLang
{
    BOOL isSame=false;
    NSString *gameLang = [[CSConfigManager csConfigManagerShared] gettingGameLang2IOS];
    if(self.originalLang.length > 0 && gameLang.length > 0 && ([gameLang isEqualToString:self.originalLang] || [self isSameZhLangByTargetLang:gameLang]))
        isSame=true;
    return isSame;
}

-(BOOL) isSameZhLangByTargetLang:(NSString*) targetLang
{
    if(self.originalLang.length > 0 && targetLang.length > 0 && (([self isZh_CNWithByLang:self.originalLang] && [self isZh_CNWithByLang:targetLang]) || ([self isZh_TWWithByLang:self.originalLang] && [self isZh_TWWithByLang:targetLang])))
        return true;
    return false;
}

/**
 *  初始化随机打招呼
 */
-(void) initRandomHelloStr
{
    self.helloStrs = [[NSMutableArray alloc] init];
    NSString *hello1 = [NSString stringWithMultilingualWithKey:@"132121"];//132121=打招呼
    NSString *hello2 = [NSString stringWithMultilingualWithKey:@"132122"];//132122=入盟先交保护费！
    
    [self.helloStrs addObject:hello1];
    [self.helloStrs addObject:hello2];
    
    NSString *palyerName = [self gettingUserName];
    NSString *allianceName = [self gettingAsn];
    if (palyerName.length <= 0 ) {
        palyerName = self.name;
    }
    
    if (allianceName.length <= 0) {
        allianceName = self.asn;
    }
    if (palyerName.length > 0 && allianceName.length > 0) {
        NSString *hello3 = [NSString stringWithMultilingualWithKey:@"132123"andWithPaddingString1:allianceName andWithPaddingString2:palyerName];//132123=欢迎{1}加入{0}
        NSString *hello4 = [NSString stringWithMultilingualWithKey:@"132124" andWithPaddingString1:allianceName ];//{0}欢迎你的加入，有什么不懂的欢迎在联盟聊天室问我们
        
        [self.helloStrs addObject:hello3];
        [self.helloStrs addObject:hello4];
    }
    
}

-(BOOL) hasTranslation
{
    //如果有翻译信息显示翻译信息
    if (self.translateMsg.length>0) {
        self.hasTranslated = TRUE;
        return TRUE;
    }else{
        self.hasTranslated = FALSE;
        return FALSE;
    }
}

-(BOOL) isTranlateDisable
{
    
    if(self.originalLang.length > 0 && [TranslateManager sharedTranslateManagerInstance].disableLang.length > 0)
    {
        BOOL isContainsOriginLang=false;
        
        isContainsOriginLang= [self isContainsLangWithAdd:[TranslateManager sharedTranslateManagerInstance].disableLang withAdd:self.originalLang];
        
        if(isContainsOriginLang)
            return true;
    }
    return false;
}

-(BOOL) isContainsLangWithAdd:(NSString*) disableLang withAdd:(NSString*)lang
{
    BOOL ret = false;
    if(disableLang.length > 0 && self.originalLang.length > 0)
    {
        if(CSCONTAINS(disableLang, lang))
            ret = true;
        else
        {
            if(((CSCONTAINS(disableLang, @"zh-CN") || CSCONTAINS(disableLang, @"zh_CN")|| CSCONTAINS(disableLang, @"zh-Hans")) && [self isZh_CNWithByLang:lang])
               || ((CSCONTAINS(disableLang, @"zh-TW") || CSCONTAINS(disableLang, @"zh_TW") || CSCONTAINS(disableLang, @"zh-Hant")) && [self isZh_TWWithByLang:lang]))
                ret = true;
        }
    }
    return ret;
}

-(BOOL) isZh_CNWithByLang:(NSString*) lang
{
    if(lang.length > 0 && ([lang isEqualToString:@"zh-CN"] || [lang isEqualToString:@"zh_CN"] || [lang isEqualToString:@"zh-Hans"]))
        return true;
    return false;
}

-(BOOL) isZh_TWWithByLang:(NSString*)lang
{
    if(lang.length > 0 && ([lang isEqualToString:@"zh-TW"] || [lang isEqualToString:@"zh_TW"] || [lang isEqualToString:@"zh-Hant"]))
        return true;
    return false;
}

-(NSUserInfo*)  gettingUserInfo
{
    NSUserInfo *user = [[UserManager sharedUserManager] gettingUser:self.uid];
    if(user == nil){
        user =[[ UserManager sharedUserManager] gettingUserInforWithUid:self.uid];
        if (user == nil) {
            NSUserInfo *result = [[NSUserInfo alloc]initWithMsgSenderWithMsg:self];
            [[UserManager sharedUserManager] addUser:result];
            [[UserManager sharedUserManager]gettingServicerUser:self.uid];
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
        DVLog(@"%@",userInfoLabel);
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
    
    if (self.post == 7) {
        [self equipShare];
    }
    
    if (self.post == 11) {
        [self parsAllianceTast];
    }
    
    if ([ServiceInterface serviceInterfaceSharedManager].isChangeLanguage) {
        self.translateMsg = @"";
        self.hasTranslated = false;
    }
    
    //    self.showUserInfoLabel = [self gettingNSUserInfoLabel];
    [self settingMsgUserNickString];
    self.showMsg = [self gettingShowMsg];
    self.endMsg = [self gettingEndMsg];
    [self settingCustomHead];
    self.gmodStr = [self gettingGmodStr];
    
}

-(void) equipShare
{
    if(self.msg.length > 0)
    {
        NSArray *equipInfo = [self.msg componentsSeparatedByString:@"|"];
        if(equipInfo.count == 2)
        {
            int colorIndex = ((NSString*)equipInfo[0]).intValue;
            DVLog(@"装备颜色值 = %d",colorIndex);
            self.fontColorStr =[self getColorByIndex:colorIndex];
            DVLog(@"colorStr == %@",self.fontColorStr);
            self.changeColor2String = (NSString*)equipInfo[1];
            DVLog(@"%@",self.changeColor2String);
            //我刚刚在铁匠铺在成功的锻造出了{0}   111660
            self.showMsg = [NSString stringWithMultilingualWithKey:@"111660" andWithPaddingString1:[NSString stringWithMultilingualWithKey:self.changeColor2String]];
        }else{
            DVLog(@"装备分享数据error -----》msg ：%@",self.msg);
            
        }
    }
}

-(NSString*) getColorByIndex:(int) index
{
    NSString *color = @"";
    switch (index)
    {
        case 0:
            color = @"173|175|172";
            break;
        case 1:
            color = @"83|151|33";
            break;
        case 2:
            color = @"71|132|225";
            break;
        case 3:
            color = @"112|72|194";
            break;
        case 4:
            color = @"193|129|72";
            break;
        case 5:
            color = @"205|183|63";
            break;
        default:
            color = @"173|175|172";
            break;
    }
    return color;
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
    }else if([self gettingGmod] == 11){
        return @"vip_certification";
    }else{
        return @"";
    }
}

-(NSString *)gettingEndMsg
{

    NSString *timeStr = [self nsDataSwitchNSString:self.createTime];
    NSString *endMsg = @"";

    if (self.post == 7 || self.post == 11) {
        return [self subStringWithTime:timeStr];
    }else{
        if (self.isSelfMsg )
        {
            return [self subStringWithTime:timeStr];
        }else{
            if ([self isOriginalSameAsTargetLang] || [self isTranlateDisable] || self.translateMsg.length <= 0) {
                return [self subStringWithTime:timeStr];
            }else{
                return [self settingEndTranslateMsg:timeStr endMsg:endMsg isShowTranslateMsg:self.hasTranslated];
            }
        }
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
    if ( time && time> 0) {
        NSDate *data = [NSDate dateWithTimeIntervalSince1970:time];
        NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
        NSString *timeStr = [dateFormatter stringFromDate:data];
        return timeStr;

    }else{
        return @"";
    }
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
    NSMutableString *showMsg = [NSMutableString  string];
    showMsg =  [self settingMsg];
    [self settingSystemIcon:showMsg];
    [self settingReport:showMsg];
    //    DVLog(@"gettingShowMsg: %@",showMsg);
    return [NSString stringWithFormat:@"%@",showMsg];
}

-(void)settingShowMsg:(NSString *)vlocalLanString{
    NSString *showMsg =@"";

    if ((self.post != 7 || self.post != 11) && self.isShowTransLate) {
        showMsg =  [showMsg stringByAppendingString:self.msg];
    }else if(self.post != 7 || self.post != 11){
        showMsg =  [showMsg stringByAppendingString:self.translateMsg];
    }
    
    
    if (self.post != 0 && self.post != 6 && self.post != 100) {
        NSString *sysImage=@"[sysChat]";
        showMsg =   [sysImage stringByAppendingString:showMsg];
    }
    
    if (self.post == 4 || self.post == 5) {
        NSString *battlereportImage=@"[battlereport]";
        showMsg = [showMsg stringByAppendingString:battlereportImage];
    }else if(self.post == 7){
        NSString *equipShareImage=@"[equip_share]";
        showMsg = [showMsg stringByAppendingString:equipShareImage];
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
 
    NSMutableString *tempString =  showMsg;
    //是否添加系统图标 暂时注释 原因是图片渲染不上
    if (self.post != 0 && self.post != 6 && self.post != 100 && self.post != 200) {
        NSString *sysImage=@"[sysChat]";
        [showMsg insertString:sysImage atIndex:0];
    }
}
/**根据翻译信息 设置显示到屏幕上的信息*/
-(NSMutableString *) settingMsg
{
    
    NSMutableString *tempString = [NSMutableString string ];
    
    if (![self isMatchingVersion]) {
        [tempString appendString:[NSString stringWithMultilingualWithKey:@"105110"]];
    }else if (self.post == 7 || self.post == 11) {
        if (self.showMsg.length > 0) {
            [tempString appendString:self.showMsg];
        }else{
            [tempString appendString:@""];
        }
    }else{
        if (self.isSelfMsg) {
            [tempString appendFormat:@"%@",self.msg];
        }else{
            if ([self isTranlateDisable] || [self isOriginalSameAsTargetLang] || self.translateMsg.length <= 0) {
                [tempString appendFormat:@"%@",self.msg];
            }else{
                [tempString appendString:self.translateMsg];
            }
        }
    }
    
    return  tempString;
}

//添加战报  暂时注释 原因是图片渲染不上
-(void) settingReport:(NSMutableString*) showMsg
{
    //普通战报
    if (self.post == 4 || self.post == 5) {
        NSString *battlereportImage=@"[battlereport]";
        [showMsg appendString:battlereportImage];
    }else if(self.post == 7){
        NSString *battlereportImage=@"[equip_share]";
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

-(BOOL)ischeckTheEquip
{
    return self.post == 7;
}

-(UIImage *) gettingCustomHeadPicImg
{
    //    DVLog(@"%@",self.customHeadPicPath);
    UIImage *image =[[UIImage alloc]initWithContentsOfFile:self.customHeadPicPath];
    return image;
    
}

-(void) initUserForReceivedMsg
{
    
    NSUserInfo *user = [self gettingUserInfo];//肯定会返回一个user，或者是通过msg生成 ，或者从数据库拿去
    
    if  (self.lastUpdateTime>user.lastUpdateTime){
        [[UserManager sharedUserManager] gettingServicerUser:self.uid];
    }
    self.name = user.userName;
    self.asn = user.asn;
    self.vip = user.vip;
    self.headPic = user.headPic;
    self.headPicVer = user.headPicVer;
    
    
}

-(void)initMsgUserWithBlock:(MsgUserBlock)vBlock{
    
    NSUserInfo *user = [self gettingUserInfo];
    if (vBlock != nil) {
        vBlock(user);
    }
}
-(BOOL)playerIsVip
{
    return [[self gettingUserInfo].vip isEqualToString:@""] && [self gettingUserInfo].vip.length > 0;
}

#pragma mark -
#pragma mark 检查消息体对应的user是否存在
-(void)msgItemCheckUserInfoWithMsgBeforeMsgSaveToDB{
    NSUserInfo *user = [[UserManager sharedUserManager] gettingUser:self.uid];
    if(user == nil){
        user =[[ UserManager sharedUserManager] gettingUserInforWithUid:self.uid];
        if (user == nil) {
            if (self.name) {
                NSUserInfo *tempUserInfo = [[NSUserInfo alloc]initWithMsgSenderWithMsg:self];
                [tempUserInfo updateToDBWithSelf];
            }
        }else{
            if (user.userName.length == 0 && self.name) {
                user.userName = self.name;
                user.asn =self.asn;
                user.vip =self.vip;
                user.headPic = self.headPic;
                user.headPicVer =self.headPicVer;
                user.lastUpdateTime = 0;
                user.isDummy = NO;
                [user updateToDBWithSelf];
            }
            
        }
    }else{
        if (user.userName.length == 0 && self.name) {
            user.userName = self.name;
            user.asn =self.asn;
            user.vip =self.vip;
            user.headPic = self.headPic;
            user.headPicVer =self.headPicVer;
            user.lastUpdateTime = 0;
            user.isDummy = NO;
            [user updateToDBWithSelf];
            
        }
    }
    
}
#pragma mark -
#pragma mark 消息从数据库取出来后，进行封装
-(void)msgFormatFromDBwithAdd:(NSString *)channelId{
    
    if (self.sequenceId == 40) {
        DVLog(@"%@",self.translateMsg);
    }
    
    [self initAttribute];
    if ([self.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
        self.isSelfMsg = YES;
    }else{
        self.isSelfMsg = NO;
    }
    
    if (self.sequenceId == 40) {
        DVLog(@"%@",self.translateMsg);
    }
    
    [self initUserForReceivedMsg];
    
    self.channelIDString = channelId;
    
    [self packedMsg];
}

#pragma mark -
#pragma mark DB 相关
#ifdef DEBUG
// 将要插入数据库
//+(BOOL)dbWillInsert:(NSObject *)entity
//{
//    LKLog(@"will insert : %@",NSStringFromClass(self));
//    return YES;
//}
//已经插入数据库
+(void)dbDidInserted:(NSObject *)entity result:(BOOL)result
{
    if (result) {
        LKLog(@"did insert Flag: 【%d】\n完成插入的数据:%@",result,[entity printAllPropertys] );
    }else{
        LKLog(@"did insert Flag: 【%d】\n插入失败的数据:%@",result,[entity printAllPropertys] );
    }
    
}

+(void)dbDidUpdated:(NSObject *)entity result:(BOOL)result{
    if (result) {
        LKLog(@"did Update Flag: 【%d】\n更新成功的数据:%@",result,[entity printAllPropertys] );
    }else{
        LKLog(@"did Update Flag: 【%d】\n 更新失败的数据:%@",result,[entity printAllPropertys] );
    }
}

#endif

+(NSString *)getPrimaryKey
{
    return @"_id";
}

+(int)getTableVersion
{
    return kDBVersion;
}

//更新数据模型后，需要调用
//在类 初始化的时候
+(void)initialize
{
    //remove unwant property
    //比如 getTableMapping 返回nil 的时候  会取全部属性  这时候 就可以 用这个方法  移除掉 不要的属性
    //    [self removePropertyWithColumnName:@"error"];
    
    
    //simple set a column as "LKSQL_Mapping_UserCalculate"
    //根据 属性名  来启用自己计算
    //[self setUserCalculateForCN:@"error"];
    
    
    //根据 属性类型  来启用自己计算
    //[self setUserCalculateForPTN:@"NSDictionary"];
    
    //enable own calculations
    //[self setUserCalculateForCN:@"address"];
    
    //enable the column binding property name
    //    [self setTableColumnName:@"MyAge" bindingPropertyName:@"age"];
    //    [self setTableColumnName:@"MyDate" bindingPropertyName:@"date"];
}

 
+(void)columnAttributeWithProperty:(LKDBProperty *)property
{
    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
    if([sqlColumnname isEqualToString:@"TableVer"])
    {
        property.defaultValue = [NSString stringWithFormat:@"%d",[NSMsgItem getTableVersion]];
    }
    //    if([property.propertyName isEqualToString:@"date"])
    //    {
    //        property.isUnique = YES;
    //        property.checkValue = @"MyDate > '2000-01-01 00:00:00'";
    //        property.length = 30;
    //    }
}
-(id)modelGetValue:(LKDBProperty *)property{
    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
    if([sqlColumnname isEqualToString:@"TableVer"])
    {
        return [NSString stringWithFormat:@"%d",[NSMsgItem getTableVersion]];
    }else{
        return  [super modelGetValue:property];
    }
    
}

+(NSDictionary *)getTableMapping{
    return @{@"TableVer"            :@"tableVer",
             @"_id"                  :@"_id"    ,
             @"SequenceID"          :@"sequenceId",
             @"MailID"              :@"mailId",
             @"UserID"              :@"uid",
             @"ChannelType"          :@"channelType",
             @"CreateTime"          :@"createTime",
             @"LocalSendTime"        :@"sendLocalTime",
             @"Type"                :@"post",
             @"Msg"                  :@"msg",
             @"Translation"          :@"translateMsg",
             @"OriginalLanguage"    :@"originalLang",
             @"TranslatedLanguage"  :@"translatedLang",
             @"Status"              :@"sendState",
             @"AttachmentId"        :@"attachmentId",
             @"Media"                :@"media"
             //             @"UserName"             :@"name",
             //             @"Asn"                  :@"asn",
             //             @"Vip"                  :@"vip",
             //             @"HeadPicVer"           :@"headPic",
             //             @"HeadPicVer"           :@"headPicVer"
             };
}
//+(LKTableUpdateType)tableUpdateWithModelTableName:(NSString *)vTableName
//                                 andForOldVersion:(int)oldVersion
//                                       newVersion:(int)newVersion{
//    if (newVersion != oldVersion) {
//
//        //        [self tableUpdateAddColumeWithModelTableName:vTableName andWithPN:@"tempArray"];
//    }
//
//    return LKTableUpdateTypeCustom;
//
//}
//+(void)tableUpdateAddColumeWithModelTableName:(NSString *)vTableName andWithPN:(NSString*)propertyName{
//
//    [super tableUpdateAddColumeWithModelTableName:vTableName andWithPN:propertyName];
//}

-(BOOL) isModMsg
{
    return self.post == MSG_TYPE_MOD;
}

-(void) parsAllianceTast
{
//    134055={0}刚刚发布的联盟任务{1}已经被{2}接取 3|134055|134011|[{name:qwe123qwe},{name:aiyooo72}]

    if(self.msg.length > 0)
    {
        NSArray *datas = [self.msg componentsSeparatedByString:@"|"];
        NSMutableArray *tempParam = [[NSMutableArray alloc]init];
        NSMutableString *jsonStr = @"";
        if ([datas count] >= 4) {
            for (int i = 0; i < [datas count]; i++) {
                if (i >= 3) {
                   jsonStr = [jsonStr stringByAppendingString:(NSString*)datas[i]];
                    continue;
                }
                [tempParam addObject:(NSString*)datas[i]];
            }
            if (jsonStr.length > 0) {
//                jsonStr = [jsonStr stringByReplacingOccurrencesOfString:@"\"" withString:@""];
                [tempParam addObject:jsonStr];
            }
        }else{
            tempParam = datas;
        }
        
        int colorIndex = ((NSString*)datas[0]).intValue;
        DVLog(@"装备颜色值 = %d",colorIndex);
        self.fontColorStr =[self parsAllianceFontColorByIndex:colorIndex];
        DVLog(@"colorStr == %@",self.fontColorStr);
        self.changeColor2String = (NSString*)datas[2];
        DVLog(@"%@",self.changeColor2String);
        //我刚刚在铁匠铺在成功的锻造出了{0}   111660
        
        self.showMsg = [self showMsgByParsAllianceTastWithDic:tempParam];
    }
    
}

-(NSString*) parsAllianceFontColorByIndex:(int) index
{
    NSString *color = @"";
    switch (index)
    {
        case 0:
            //c7beb3 白
            //199|190|179
            color = @"199|190|179";
            break;
        case 1:
            //56e578 绿
            //86|299|120
            color = @"86|299|120";
            break;
        case 2:
            //4599f8 蓝
            //69|153|248
            color = @"69|153|248";
            break;
        case 3:
            //af49ea 紫
            //175|73|234
            color = @"175|73|234";
            break;
        case 4:
            //e8771f  橙
            //232|119|31
            color = @"232|119|31";
            break;
        case 5:
            //edd538  黄
            //237|213|56
            color = @"237|213|56";
            break;
        default:
            color = @"199|190|179";
            break;
    }
    return color;
}

-(NSString*) showMsgByParsAllianceTastWithDic:(NSArray*) datas
{
        
    NSString *jsonStr = (NSString*)datas[3];
    NSLog(@"%@",jsonStr);
    NSData* jsonData = [jsonStr dataUsingEncoding:NSUTF8StringEncoding];
    if (!jsonData) {
        return @"";
    }
    NSArray *arrlist= [jsonData objectFromJSONData];
    
    if (!arrlist) {
        return @"";
    }

    if ([arrlist count] == 1) {
        //        134054		{0}刚刚发布了联盟任务{1}，请大家前往协助
        NSString *releaseName = [arrlist[0] objectForKey:@"name"];
        NSString *allianceTastName = [NSString stringWithMultilingualWithKey:(NSString*)datas[2]];
        if (!releaseName && !allianceTastName) {
            return @"";
        }
        return [NSString stringWithMultilingualWithKey:@"134054" andWithPaddingString1:releaseName andWithPaddingString2:allianceTastName];
    }else{
        //134055={0}刚刚发布的联盟任务{1}已经被{2}接取
        NSString *releaseName = [arrlist[0] objectForKey:@"name"];
        NSString *acceptName = [arrlist[1] objectForKey:@"name"];
        NSString *allianceTastName = [NSString stringWithMultilingualWithKey:(NSString*)datas[2]];
        
        if (!releaseName && !acceptName && !allianceTastName) {
            return @"";
        }
        
        return [NSString stringWithMultilingualWithKey:@"134055" andWithPaddingString1:releaseName andWithPaddingString2:allianceTastName andWithPaddingString3:acceptName];
    }
    return @"";
}

- (id)toArrayOrNSDictionary:(NSString *)jsonString{
    NSData *jsonData = [jsonString dataUsingEncoding:NSASCIIStringEncoding];
    NSError *error = nil;
    id jsonObject = [NSJSONSerialization JSONObjectWithData:jsonData
                                                    options:NSJSONReadingAllowFragments
                                                      error:&error];
    
    if (jsonObject != nil && error == nil){
        return jsonObject;
    }else{
        // 解析错误
        return nil;
    }
    
}

-(BOOL) isAcceptTask
{
    if(self.msg.length > 0)
    {
        NSArray *datas = [self.msg componentsSeparatedByString:@"|"];
        NSMutableArray *tempParam = [[NSMutableArray alloc]init];

        if ([datas count] > 0) {
            for (int i = 0; i < [datas count]; i++) {
                if (i == 1) {
                    if ([(NSString*)datas[1] isEqualToString:@"134054"]) {
                        return true;
                    }else{
                        return false;
                    }
                }
            }
            
        }
    }
    
    return false;
}

-(void) changeRedPackageStatus
{
    NSArray *arr = [self.attachmentId componentsSeparatedByString:@"|"];
    if ([arr count] != 2) {
        DVLog(@"attachmentId 异常");
        return ;
    }
    
    DVLog(@"attachmentId = %@",self.attachmentId);
    
    NSString *redPackageId = arr[0];
    
    NSString *tempStatus = [[ChannelManager sharedChannelManager].redPackageUnHandleMap objectForKey:redPackageId];
    if ([tempStatus isEqualToString:@"1"]) {
        DVLog(@"%@",tempStatus);
    }
    long time = [MsgMessage msgMessageShared].redPackageTime * 3600000;
    
    long limitTime = self.createTime * 1000 + time;
    
    long nowTime = (long)[[NSDate date] timeIntervalSince1970] * 1000;
    
    if (self.isSelfMsg) {
        [self updataAttachmentIdBystatus:@"0" andWithUid:redPackageId];
    }else if(nowTime > limitTime){
        [self updataAttachmentIdBystatus:@"3" andWithUid:redPackageId];
    }else if(tempStatus.length > 0){
        [self updataAttachmentIdBystatus:tempStatus andWithUid:redPackageId];
    }
}

-(void) updataAttachmentIdBystatus:(NSString*) status andWithUid:(NSString*) redPackageId{
    self.attachmentId = [NSString stringWithFormat:@"%@|%@",redPackageId,status];
    [self redPackageStatusSave2DB];
}

-(void) redPackageStatusSave2DB
{
    dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
        if (self.channelType == IOS_CHANNEL_TYPE_ALLIANCE){
            ChatChannel *channel =[[ChannelManager sharedChannelManager] gettingAllianceChannel];
            [channel redPacketMsgSaveToDBWithMsgItem:self];
        }else if(self.channelType == IOS_CHANNEL_TYPE_COUNTRY){
            ChatChannel *channel =[[ChannelManager sharedChannelManager] gettingCountryChannel];
            [channel redPacketMsgSaveToDBWithMsgItem:self];
        }
    });
}



-(BOOL) checkVersionByVersion:(NSString*) version
{
    return [[ChatServiceController chatServiceControllerSharedManager].gameHost checkVersionByVersion:version];
}

-(BOOL) isMatchingVersion{
    return !(self.post > VERSIONPOST && self.post != 200 && self.post != 100);
}

-(int) gettingRedPackageStatus{
    NSArray *arr = [self.attachmentId componentsSeparatedByString:@"|"];
    if ([arr count] == 2) {
        NSString *status = arr[1];
        return [status intValue];
    }else{
        return -1;
    }

}

@end