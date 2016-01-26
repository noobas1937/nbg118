//
//  MailData.m
//  IF
//
//  Created by 马纵驰 on 15/9/16.
//
//

#import "MailData.h"
#import "MailInfoDataModel.h"
#import "CSConfigManager.h"
#import "ChatServiceController.h"
#import "MailIconNameKey.h"
#import "NSString+Cocos2dHelper.h"
#import "NSString+Extension.h"
#import "UserManager.h"

@implementation MailData

-(instancetype)initWithAddMailData:(MailInfoDataModel*)maildata
{
    self = [super init];
    if (self) {
        if (maildata.mail_ID.length > 0 && maildata.creatTime && maildata) {
            [self initProperty];
            self.type = [NSString stringWithFormat:@"%d",maildata.type];
            
            if ([maildata isKNightFightReport]) {
                self.createTime = [NSString stringWithFormat:@"%ld",maildata.creatTime];
            }else{
                self.createTime = [NSString stringWithFormat:@"%ld",maildata.creatTime];
                long time=[self.createTime longLongValue] * 1000;
                self.createTime = [NSString stringWithFormat:@"%ld",time];
            }
            self.status = [NSString stringWithFormat:@"%d",maildata.status];
            self.reply = [NSString stringWithFormat:@"%d",maildata.reply];
            self.rewardStatus = [NSString stringWithFormat:@"%d",maildata.rewardStatus];
            self.itemIdFlag = [NSString stringWithFormat:@"%d",maildata.itemIdFlag];
            self.save = [NSString stringWithFormat:@"%d",maildata.save];
            self.mbLevel = [NSString stringWithFormat:@"%d",maildata.mbLevel];
            self.uid = maildata.mail_ID;
            self.title = maildata.title;
            self.contents = maildata.contents;
            self.fromName = maildata.fromName;
            self.fromUid = maildata.fromUserUid;
            self.rewardId = maildata.rewardId;
            self.version = maildata.version;
            self.channelId = maildata.channelID;
            self.hasParseLocal = FALSE;
            self.save = [NSString stringWithFormat:@"%d",maildata.saveFlag];
            if (maildata.picString.length) {
                self.pic = maildata.picString;
            }else{
                self.pic = @"";
            }
            if (maildata.itemIdFlag >= 0) {
                self.itemIdFlag = maildata.itemIdFlag;
            }else{
                self.itemIdFlag = -1;
            }
            
            self.tabType = maildata.tabType;
        }
    }
    return self;
}

-(void) initProperty
{
    self.jsonStr = @"";
    self.tabType = -1;
    self.isAtk = false;
    self.hasParseBy2dx = false;
    self.hasParseCompex = false;
    self.hasParseLocal = false;
    self.language = @"";
    self.parseVersion = -1;
    
    self.nameText = @"";
    self.contentText = @"";
    self.mailIcon = @"";
    self.timeText = @"";
    self.usePersonalPic = false;
    self.channelId = @"";
}

-(void) parseContents
{
    if([self.type intValue] == Mail_Sysupdate)
        self.version = self.fromUid;
    [self parseMailCellIcon];
    [self parseMailName];
    [self parseContentText];
//    if (![self needParseContent]) {
//        [self parseContents];
//        self.hasParseLocal = true;
//    }
    
//    self.timeText = TimeManager.getReadableTime(createTime);
    self.language = [[CSConfigManager csConfigManagerShared] gettingGameLang2IOS];
//    [self parseMode2Json];
}

-(void)parseMode2Json
{
    NSDictionary *dic = self.keyValues;
    
    self.jsonStr = [dic dataTOjsonString];
}


-(void) parseMailCellIcon
{
    switch ([self.type intValue])
    {
        case Mail_System :
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_CHANNEL_ICON_SYSTEM];
            break;
        case Mail_Battle_Report:
        {
            if (self.isAtk)
            {
                self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_VICTORY];
            }
            else
            {
                self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_DEFENT_VICTORY];
            }
            break;
        }
        case Mail_Resource:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_CHANNEL_ICON_RESOURCE];
            break;
        case Mail_Detect:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_DETECT];
            break;
        case Mail_Detect_Report:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_DETECT];
            break;
        case Mail_Encamp:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CAMP];
            break;
        case Mail_Fresher:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_SYSTEM];
            break;
        case Mail_Wounded:
            self.mailIcon = self.pic;
            self.usePersonalPic = true;
            break;
        case Mail_Digong:
            self.mailIcon = self.pic;
            self.usePersonalPic = true;
            break;
        case All_Service:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_SYSTEM];
            break;
        case World_New_Explore:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_EXPLORE];
            break;
        case Mail_Sysnotice:
            if(self.title.length > 0 && [self.title isEqualToString:@"114020"]) //114020 成为VIP
                self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_SYSTEM_VIP];
            else
                self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_SYSTEM];
            break;
        case Mail_Sysupdate:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_ANNOUNCEMENT];
            break;
        case Mail_Allianceinvaite:
            self.mailIcon = self.pic;
            self.usePersonalPic = true;
            break;
        case Mail_Attackmonster:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_CHANNEL_ICON_MONSTER];
            break;
        case World_Monster_Special:
            self.mailIcon = self.pic;
            self.usePersonalPic = true;
            break;
        case Mail_Alliance_All:
            self.mailIcon = self.pic;
            self.usePersonalPic = true;
            break;
        case Mail_Invite_teleport:
            self.mailIcon = self.pic;
            self.usePersonalPic = true;
            break;
        case Mail_Alliance_Kickout:
            self.mailIcon = self.pic;
            self.usePersonalPic = true;
            break;
        case Mail_World_boss:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_REPORT];
            break;
        case Chat_Room:
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_CHAT_ROOM];
            break;
        default:
            break;
    }
    
    if([self isWorldBossKillRewardMail])
        self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_REPORT];
    if (self.mailIcon.length <= 0) {
        self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_SYSTEM];
    }
}

-(void) parseMailName
{
    self.nameText = self.fromName;
    if([self.type intValue] == Mail_Resource_Help){
        self.nameText = [NSString stringWithMultilingualWithKey:@"114121"]; // 资源帮助报告
    }
    else if([self.type intValue] == Mail_Mod_Send || [self.type intValue] == Mail_Mod_Personal){
        self.nameText = [NSString stringWithFormat:@"%@[MOD]%@",self.nameText,self.fromName];
    }
    else if([self.type intValue] == Mail_Allianceinvaite){
        self.nameText = [NSString stringWithMultilingualWithKey:@"103738"]; //邀请函
    }
    else if([self.type intValue] == Mail_Alliance_Kickout){
        self.nameText = [NSString stringWithMultilingualWithKey:@"103783"]; //退盟通知
    }
    else if([self.type intValue] == Mail_Gift){
        
        if([[UserManager sharedUserManager] gettingUser:self.fromUid] && [[UserManager sharedUserManager] gettingUser:self.fromUid].asn.length > 0){
            NSString *asn = [[UserManager sharedUserManager] gettingUser:self.fromUid].asn;
            self.nameText = [NSString stringWithFormat:@"(%@)%@",asn,self.fromName];
        }else{
            self.nameText = self.fromName;
        }
    }
    else if([self.type intValue] == Mail_World_boss || [self isWorldBossKillRewardMail])
    {
        self.nameText = [NSString stringWithMultilingualWithKey:@"137451"]; // 挑战世界BOOS战报
    }
    else if([self.type intValue] == Mail_Resource)
    {
        self.nameText = [NSString stringWithMultilingualWithKey:@"105516"]; //资源采集报告
    }
    else if([self.type intValue] == Mail_Fresher || [self.type intValue] == Mail_System){
        if(self.title.length > 0 && [self.title isEqualToString:@"115429"]) //联盟指令
        {
            if(self.fromName.length > 0)
                self.nameText = self.fromName;
            else
                self.nameText = [NSString stringWithMultilingualWithKey:@"3000002"]; // 阿加莎
        }
        else
            self.nameText = [NSString stringWithMultilingualWithKey:@"3000002"]; // 阿加莎
    }
    else if([self.type intValue] == Mail_Refuse_All_Apply){
        self.nameText = [NSString stringWithMultilingualWithKey:@"115464"]; //拒接通知
    }
    else if([self.type intValue] == Mail_Attackmonster)
    {
        self.nameText = [NSString stringWithMultilingualWithKey:@"103715"]; //怪物报告
    }
    else if([self.type intValue] == Mail_Detect)
    {
        self.nameText = [NSString stringWithMultilingualWithKey:@"105522"]; //侦查战报
        if(self.itemIdFlag==0 && self.title.length > 0)
        {
            if([self.title isEqualToString:@"1"])
            {
                self.nameText = [NSString stringWithMultilingualWithKey:@"105523"]; //你得城市被侦查
            }else if([self.title isEqualToString:@"12"]){
                NSString *catapultStr = [NSString stringWithMultilingualWithKey:@"110081"]; // 投石车
                self.nameText = [NSString stringWithMultilingualWithKey:@"140181" andWithPaddingString1:catapultStr];
            }else if([self.title isEqualToString:@"10"]){
                NSString *catapultStr = [NSString stringWithMultilingualWithKey:@"110172"]; // 车
                self.nameText = [NSString stringWithMultilingualWithKey:@"140181" andWithPaddingString1:catapultStr];
            }else
            {
                self.nameText = [NSString stringWithMultilingualWithKey:@"105567"]; //你得资源点被侦查
            }
        }
    }
    
    
    if(self.nameText.length <= 0)
    {
        if([self.channelId isEqualToString:Mail_ChannelID_Fight]){
            self.nameText = [NSString stringWithMultilingualWithKey:@"105519"]; //战斗报告
        }else if([self.channelId isEqualToString:Mail_ChannelID_Mod]){
            self.nameText = [NSString stringWithMultilingualWithKey:@"132000"]; //联系MOD
        }else if([self.channelId isEqualToString:Mail_ChannelID_Studio]){
            if ([self.type intValue] == Mail_Sysupdate) {
                self.nameText = [NSString stringWithMultilingualWithKey:@"105569"]; // 公告
            }else{
                self.nameText = [NSString stringWithMultilingualWithKey:@"103731"]; //列王的纷争游戏工作室
            }
        }
        else if([self.channelId isEqualToString:Mail_ChannelID_System] || [self.type intValue] == Mail_Allianceapply || [self.type intValue] == Mail_Donate){
            self.nameText = [NSString stringWithMultilingualWithKey:@"115181"]; //系统
        }
//        else if([self.channelId isEqualToString:Mail_ChannelID_Notice]){
//            self.nameText = [NSString stringWithMultilingualWithKey:@"105569"]; // 公告
//        }
    }
    //1 == 阿加莎 2 == 阿加莎
    if([self.nameText isEqualToString:@"3000001"] || [self.nameText isEqualToString:@"3000002"])
        self.nameText = [NSString stringWithMultilingualWithKey:@"3000002"];
    if ([self.nameText isEqualToString:@"system"] && [self.type intValue] == All_Service) {
        self.nameText = [NSString stringWithMultilingualWithKey:@"103731"]; //列王的纷争游戏工作室
    }
}

-(BOOL) isWorldBossKillRewardMail
{
    if(self.itemIdFlag == 1 && self.title.length > 0 && [self.title isEqualToString:@"137460"]) //大魔王消灭奖励
        return true;
    return false;
}

-(void) parseContentText
{
    //		System.out.println("parseTitleText type:"+type+"  title:"+title);
    if(self.title.length > 0)
        self.contentText = [self.title stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
    if([self.title isEqualToString:@"105734"])
        self.contentText= [NSString stringWithMultilingualWithKey:@"105734"]; //您的城堡已经被迫迁走
    else if([self.title isEqualToString:@"138067"])
        self.contentText= [NSString stringWithMultilingualWithKey:@"138067"]; //远古战场关闭
    else if([self.title isEqualToString:@"114010"])
        self.contentText = [NSString stringWithMultilingualWithKey:@"114010"]; //新手迁城
    else if([self.title isEqualToString:@"115295"])
        self.contentText = [NSString stringWithMultilingualWithKey:@"115295"]; //迁城邀请
    else if([self.title isEqualToString:@"114012"])
        self.contentText = [NSString stringWithMultilingualWithKey:@"114012"]; //入盟申请
    else if([self.title isEqualToString:@"105718"])
        self.contentText = [NSString stringWithMultilingualWithKey:@"105718"]; //击杀敌军活动
    else if([self.title isEqualToString:@"133017"])
        self.contentText = [NSString stringWithMultilingualWithKey:@"133017"]; //联盟积分奖励
    else if(self.title.length > 0 && [self.title isPureInt])
    {
        self.contentText=[NSString stringWithMultilingualWithKey:self.title];
    }
    
    if(self.itemIdFlag == 1)
    {
        NSArray *contentArr = [self.contents componentsSeparatedByString:@"|"]; //从字符A中分隔成2个元素的数组
        int num = [contentArr count];
        
        //			System.out.println("contents:"+contents+" str size:"+num);
        switch (num)
        {
            case 5:
                if ([contentArr[0] isEqualToString:@"115336"]) //{0}被{1}{2}摧毁了！\n周围的领地已经失效，相关联盟建筑已经被收回\n您的联盟可以重新建造{0}
                {// 领地使用默认名字
                    NSString *name = [NSString stringWithMultilingualWithKey:@"115312" andWithPaddingString1:contentArr[4]];//115312联盟堡垒{0}
                    self.contentText = [NSString stringWithMultilingualWithKey:contentArr[3] andWithPaddingString1:name];
                }
                break;
            case 6:
            {
                NSString *name = @"";
                if ([contentArr[1] isEqualToString:@""] || [contentArr[2] isEqualToString:@""])
                {
                    //115312联盟堡垒{0}
                    name = [NSString stringWithMultilingualWithKey:@"115312" andWithPaddingString1:contentArr[5]];
                }
                else
                {
                    name = contentArr[1];
                }
                self.contentText = [NSString stringWithMultilingualWithKey:contentArr[4] andWithPaddingString1:name];
            }
                break;
            default:
                break;
        }
    }
    
    
    if([self.type intValue] == Mail_Donate)
    {
        NSArray *str = [self.contents componentsSeparatedByString:@"|"];

        if([str count] > 4)
        {
            NSString *userName = str[1];
            if(![str[2] isEqualToString:@""])
            {
                userName = [NSString stringWithFormat:@"(%@)%@",str[2],userName];
            }
            //来自{0}的礼物
            self.contentText = [NSString stringWithMultilingualWithKey:@"101007" andWithPaddingString1:userName];
        }
    }
    else if([self.type intValue] == Mail_Digong || [self.type intValue] == World_New_Explore)
    {
        self.contentText = [NSString stringWithMultilingualWithKey:@"108675"];////城堡遗迹探险报告
    }
    else if([self.type intValue] == Mail_Wounded)
    {
        self.contentText = [NSString stringWithMultilingualWithKey:@"105550"];
    }
    else if([self.type intValue] == Mail_Detect)
    {
        if(self.itemIdFlag==1)
        {
            if(![self.contents isEqualToString:@""] && self.contents.length > 6)
            {
                NSString *content = [self.contents substringFromIndex:6];//截取下标6之后的字符串
                if([self.contents isEqualToString:@"105554"])//您被{0}侦查，但由于您使用了反侦察道具，所以对方侦查失败！
                {
                    self.contentText= [NSString stringWithMultilingualWithKey:@"105523"]; //你的城市被侦查
                }
            }
            
        }
        else
        {
            if([self.title isEqualToString:@"1"])
            {
                self.contentText = [NSString stringWithMultilingualWithKey:@"105523"]; //你的城市被侦查
            }
            else
            {
                self.contentText = [NSString stringWithMultilingualWithKey:@"105567"]; //你的资源点被侦查
            }
            NSArray *content = [self.contents componentsSeparatedByString:@"|"];
            int num = [content count];

            switch (num) {
                case 1:
                    self.contentText = [NSString stringWithMultilingualWithKey:@"105524" andWithPaddingString1:self.contents]; //被{0}侦查
                    break;
                case 3:{
                    NSString *name = content[0];
                    NSString *type = content[1];
                    if([type isEqualToString:@"1"]){
                        self.contentText = [NSString stringWithMultilingualWithKey:@"137429" andWithPaddingString1:name]; //{0}侦查了您的城堡
                    }else if ([type isEqualToString:@"2"]){
                        self.contentText = [NSString stringWithMultilingualWithKey:@"137431" andWithPaddingString1:name]; //{0}侦查了您的部队
                    }else if ([type isEqualToString:@"3"]){
                        self.contentText = [NSString stringWithMultilingualWithKey:@"137430" andWithPaddingString1:name]; //{0}侦查了您的资源田
                    }else{
                        self.contentText = [NSString stringWithMultilingualWithKey:@"105524" andWithPaddingString1:name]; //被{0}侦查
                    }
                    break;
                }
                default:
                    break;
            }
            
        }
    }
    else if([self.type intValue] == Mail_Gift){
        self.contentText = self.contents;
    }
    if(self.contentText.length > 50)
    {
        self.contentText = [NSString formatStrByStr:self.contentText ByLength:50];
    }

}

-(BOOL) needParseContent
{
    NSString *localLanguage = [[CSConfigManager csConfigManagerShared] gettingGameLang2IOS];
    if(self.nameText.length > 0 && self.contentText.length > 0 && ![self.nameText isPureInt]
       && ![self.contentText isPureInt] && self.language.length > 0 &&
       [self.language isEqualToString:localLanguage])
    {
        return false;
    }
    return true;
}

-(void)formatContentText
{
    self.contentText = [self.contentText substringWithRange:NSMakeRange(0, 50)];
    self.contentText = [self.contentText stringByAppendingString:@"..."];
}

-(void) settingMailDealStatus{

}

-(void) changeMailRewardStatus
{
    if (self.rewardStatus.length > 0 && [self.rewardStatus isEqualToString:@"0"]) {
        self.rewardStatus = @"1";
    }
}
@end
