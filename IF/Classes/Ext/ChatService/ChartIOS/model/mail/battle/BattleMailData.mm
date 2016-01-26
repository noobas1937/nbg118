//
//  BattleMailData.m
//  IF
//
//  Created by 马纵驰 on 15/9/16.
//
//

#import "BattleMailData.h"
#import "MailInfoDataModel.h"
#import "BattleMailContents.h"
#import "ChatServiceController.h"
#import "UserManager.h"
#import "NSString+Cocos2dHelper.h"
#import "ArmyParams.h"
#import "TowerKillParams.h"
#import "MailInfoManager.h"
#import "MailIconNameKey.h"

#define  WIN    0;
#define  DRAW   1;
#define  LOOSE  2;

@implementation BattleMailData

-(instancetype)initWithAddMailData:(MailInfoDataModel*) midm withAddCount:(int) count;
{
    self = [super initWithAddMailData:midm];
    if (self) {
        self.totalNum = count;
        [self parseContentsByMode:midm];
        [BattleMailData setupObjectClassInArray:^NSDictionary *{
            return @{
                     @"knight" : @"BattleMailContents"
                     };
        }];
    }
    return self;
}

-(instancetype)initWithAddMailData:(MailInfoDataModel*) midm
{
    self = [super initWithAddMailData:midm];
    if (self) {
        self.totalNum = 0;
        [self parseContentsByMode:midm];
    }
    return self;
}

-(void) parseContentsByMode:(MailInfoDataModel*) maildata
{
    [self createJson2BattleWithAddMailInfo:maildata];
}

-(void) createJson2BattleWithAddMailInfo:(MailInfoDataModel*) maildata
{
    
    [super parseContents];
    
    self.detail = [BattleMailContents objectWithKeyValues:maildata.contents];
    if(!self.detail)
        return;
    
    self.attualContent = [[Content alloc] init];
    if(self.detail.warPoint.length > 0)
        self.attualContent.warPoint = self.detail.warPoint;
    else
        self.attualContent.warPoint = @"";
    if(self.detail.defUser && self.detail.defUser.npcId.length > 0)
    {
        self.attualContent.npcId = self.detail.defUser.npcId;
        self.attualContent.defName = @"";
    }
    else
    {
        if(self.detail.defUser && self.detail.defUser.name.length > 0)
            self.attualContent.defName = self.detail.defUser.name;
        else
            self.attualContent.defName = @"";
        self.attualContent.npcId = @"";
    }
    if(self.detail.atkUser && self.detail.atkUser.name.length > 0)
        self.attualContent.atkName = self.detail.atkUser.name;
    else
        self.attualContent.atkName = @"";
    
    if(self.detail.winner.length <= 0)
    {
        self.attualContent.win = @"2";
        
    }
    else if([self.detail.winner isEqualToString:[UserManager sharedUserManager].currentUser.uid ])
    {
        self.attualContent.win = @"0";
    }
    else
    {
        self.attualContent.win = @"1";
    }
    
    
    NSString *defUserName = nil;
    
    
    
    if(self.detail.atkUser && self.detail.atkUser.npcId.length > 0)
    {
        defUserName = self.detail.atkUser.npcId;
    }
    else if(self.detail.defUser)
    {
        if(self.detail.defUser.name.length > 0)
            defUserName = self.detail.defUser.name;
    }
    
    [self.uid isSamePrintStrByStr:@"172a4de74a944c72a0b5d831fb5b85bb"];
    if(self.detail.atkUser && self.detail.atkUser.name.length > 0 && [UserManager sharedUserManager].currentUser)
    {
        if([[UserManager sharedUserManager].currentUser.userName isEqualToString:self.detail.atkUser.name])
        {
            //105547 = //{0}攻击{1}  102187 == 我的部队
            
            NSString *str = [NSString stringWithMultilingualWithKey:@"102187"];
            [NSString stringWithMultilingualWithKey:@"105547" andWithPaddingString1:str andWithPaddingString2:defUserName];
        }
        else
        {
            NSString *str = [NSString stringWithMultilingualWithKey:@"102187"];
            self.contentText = [NSString stringWithMultilingualWithKey:@"105547" andWithPaddingString1:self.detail.atkUser.name andWithPaddingString2:str];
        }
    }
    
    NSString *kill = nil;
    NSString *loss = nil;
    
    BOOL isbigLoss = false;
    if(!self.detail.atkGen || [self.detail.atkGen count] <= 0){
        isbigLoss = true;
    }else{
        NSString *killAndLoss = [self calculateKillandLoss:self.detail];
        if(killAndLoss.length > 0)
        {
            NSArray *strArr = [killAndLoss componentsSeparatedByString:@"_"];
            if([strArr count] == 2)
            {
                kill = strArr[0];
                loss = strArr[1];
            }
        }
    }
    
    NSString *playerUid = [UserManager sharedUserManager].currentUser.uid;
    
    BOOL isAtt = false;
    NSString *attUid = @"";
    if(self.detail.atkUser && self.detail.atkUser.uid.length > 0)
        attUid = self.detail.atkUser.uid;
    NSString *attName = @"";
    if(self.detail.atkUser && self.detail.atkUser.name.length > 0)
        attName = self.detail.atkUser.name;
    NSString *defName = @"";
    if(self.detail.defUser && self.detail.defUser.name.length > 0)
        defName = self.detail.defUser.name;
    
    if(self.detail.atkHelper){
        
        for (NSString *tempHelpUid in self.detail.atkHelper) {
            if (tempHelpUid.length > 0 && [tempHelpUid isEqualToString:playerUid]) {
                isAtt = true;
                break;
            }
        }
        
    }
    
    if(attUid.length > 0 && [playerUid isEqualToString:attUid]){
        isAtt = true;
    }
    
    int reportState;
    if(self.detail.winner.length <= 0)
    {
        reportState=DRAW;
    }
    else if([self.detail.winner isEqualToString:attUid])
    {
        if(isAtt){
            reportState= WIN;
        }else{
            reportState=LOOSE;
        }
    }
    else
    {
        if(isAtt){
            reportState= LOOSE;
        }else{
            reportState=WIN;
        }
    }
    
    NSString *str = @"";
    if(isAtt){
        str = [NSString stringWithMultilingualWithKey:@"102187"];//102187=我的部队
        self.contentText = [NSString stringWithMultilingualWithKey:@"105547" andWithPaddingString1:str andWithPaddingString2:defName];//105547={0}攻击{1}
    }else{
        str = [NSString stringWithMultilingualWithKey:@"102187"];//102187=我的部队
        if([self.detail.battleType intValue] != 3){
            self.contentText = [NSString stringWithMultilingualWithKey:@"105547" andWithPaddingString1:attName andWithPaddingString2:str];//105547={0}攻击{1}
        }else{
            str = [NSString stringWithMultilingualWithKey:@"108678"];//108678=我的城堡
            self.contentText = [NSString stringWithMultilingualWithKey:@"105547" andWithPaddingString1:attName andWithPaddingString2:str];//105547={0}攻击{1}
        }
    }
    
    int battleType = [self.detail.battleType intValue];
    int pointType = [self.detail.pointType intValue];
    
    if(reportState == 0){
        if(battleType == 3)
        {
            if(isAtt)
            { //攻城胜利
                self.mailIcon = [[MailInfoManager sharedMailInfoManager] gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_VICTORY];

                switch (pointType)
                {
                    case Mail_Throne:
                    {
                        self.nameText = [NSString stringWithMultilingualWithKey:@"105700"];//105700=攻击宫殿成功
                        break;
                    }
                    case Mail_Trebuchet:
                    {
                        self.nameText = [NSString stringWithMultilingualWithKey:@"105704"];//105704=攻击投石机成功
                        break;
                    }
                    default:
                    {
                        self.nameText = [NSString stringWithMultilingualWithKey:@"105578"];//105578=攻城胜利
                        break;
                    }
                }
                
            }
            else
            { //守城胜利
                self.mailIcon = [[MailInfoManager sharedMailInfoManager] gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_DEFENT_VICTORY];
                switch (pointType)
                {
                    case Mail_Throne:
                    {
                        self.nameText = [NSString stringWithMultilingualWithKey:@"105702"];//105702=宫殿防守成功
                        break;
                    }
                    case Mail_Trebuchet:
                    {
                        self.nameText = [NSString stringWithMultilingualWithKey:@"105706"];//105706=投石机防守成功
                        break;
                    }
                    default:
                    {
                        self.nameText = [NSString stringWithMultilingualWithKey:@"105579"];//105579=守城胜利
                        break;
                    }
                }
            }
            
            
            NSString *str1 = [NSString stringWithMultilingualWithKey:@"108554"];//108554=消灭敌军：
            NSString *str2 = kill;
            NSString *str3 = @"  ";
            NSString *str4 = [NSString stringWithMultilingualWithKey:@"105019"];//105019=部队损失数量
            NSString *str5 = loss;
            self.contentText = [NSString stringWithFormat:@"%@%@%@%@%@",str1,str2,str3,str4,str5];
        }
        else if(battleType==6){//黑骑士
            self.isKnightMail = true;
            self.knight = [[NSMutableArray alloc]init];
            self.detail.uid = self.uid;
            self.detail.type = self.type;
            long time=[self.createTime longLongValue] * 1000;
            self.detail.createTime = [NSString stringWithFormat:@"%ld",time];
            [self.knight addObject:self.detail];
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_KNIGHT];
            self.nameText = [NSString stringWithMultilingualWithKey:@"133053"]; //活动战报
            self.contentText = [NSString stringWithMultilingualWithKey:@"105579"]; //守城胜利
        }
        else if (battleType==7){//联盟领地战斗报告
            if(isAtt)
            {
                self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_VICTORY];
                if(pointType == Mail_Tile_allianceArea)
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115337"]; // 攻击联盟城堡胜利
                else if(pointType == Mail_tile_banner)
                {
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115540"]; //攻击国旗胜利
                }
                self.contentText = [NSString stringWithMultilingualWithKey:@"115341" andWithPaddingString1:kill andWithPaddingString2:loss];//消灭敌军:{0} 部队损失数量：{1}
            }
            else
            {
                self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_DEFENT_VICTORY];
                if(pointType == Mail_Tile_allianceArea)
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115339"]; //防守联盟城堡胜利;
                else if(pointType == Mail_tile_banner)
                {
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115542"]; //防守国旗胜利;
                }
                
                self.contentText = [NSString stringWithMultilingualWithKey:@"115341" andWithPaddingString1:kill andWithPaddingString2:loss];
            }
        }
        else if([self.detail.msReport intValue] == 1){
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_KNIGHT];
            self.nameText = [NSString stringWithMultilingualWithKey:@"133053"]; // 活动战报
            self.contentText = [NSString stringWithMultilingualWithKey:@"133083"]; //活动结束
        }
    }
    else
    {
        if(battleType==3)
        {
            if(isbigLoss)
            {
                if(isAtt)
                { //攻城大败
                    self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_FAILURE];
                    switch (pointType)
                    {
                        case Mail_Throne:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105701"]; //攻击宫殿失败
                            break;
                        }
                        case Mail_Trebuchet:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105705"]; //攻击投石车失败
                            break;
                        }
                        default:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105583"]; //攻城大败
                            break;
                        }
                    }
                }
                else
                { //守城大败
                    self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_DEFENT_FAILURE];
                    switch (pointType)
                    {
                        case Mail_Throne:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105703"]; //宫殿防守失败
                            break;
                        }
                        case Mail_Trebuchet:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105707"]; //投石机防守失败
                            break;
                        }
                        default:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105581"]; //守城大败
                            break;
                        }
                    }
                }
                self.contentText = [NSString stringWithMultilingualWithKey:@"105535"]; //由于您的实力与敌人差距过大，您的部队被全部消灭，战斗结果没有被送回来！
            }
            else
            {
                if(isAtt)
                { //攻城大败
                    self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_FAILURE];
                    switch (pointType)
                    {
                        case Mail_Throne:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105701"]; //攻击宫殿失败
                            break;
                        }
                        case Mail_Trebuchet:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105705"]; //攻击投石车失败
                            break;
                        }
                        default:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105582"]; //攻城失败
                            break;
                        }
                    }
                }
                else
                { //守城大败
                    self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_DEFENT_FAILURE];
                    switch (pointType)
                    {
                        case Mail_Throne:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105703"]; //宫殿防守失败
                            break;
                        }
                        case Mail_Trebuchet:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105707"]; //投石机防守失败
                            break;
                        }
                        default:
                        {
                            self.nameText = [NSString stringWithMultilingualWithKey:@"105580"]; //守城失败
                            break;
                        }
                    }
                }
                self.contentText = [NSString stringWithMultilingualWithKey:@"115341" andWithPaddingString1:kill andWithPaddingString2:loss];
            }
        }
        else if(battleType==6){
            self.isKnightMail = true;
            self.knight = [[NSMutableArray alloc]init];
            self.detail.uid = self.uid;
            self.detail.type = self.type;
            long time=[self.createTime longLongValue] * 1000;
            self.detail.createTime = [NSString stringWithFormat:@"%ld",time];
            [self.knight addObject:self.detail];
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_KNIGHT];
            self.nameText = [NSString stringWithMultilingualWithKey:@"133053"]; //活动战报
            self.contentText = [NSString stringWithMultilingualWithKey:@"105580"]; //守城失败
        }
        else if (battleType==7){
            if(isAtt)
            {
                self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_FAILURE];
                if(pointType == Mail_Tile_allianceArea)
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115338"]; //攻击联盟城堡失败
                else if(pointType == Mail_tile_banner)
                {
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115541"]; //攻击国旗失败
                }
            }
            else
            {
                self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_CITY_DEFENT_FAILURE];
                if(pointType == Mail_Tile_allianceArea)
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115340"]; //防守联盟城堡失败
                else if(pointType == Mail_tile_banner)
                {
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115543"]; //防守国旗失败
                }
            }
            self.contentText = [NSString stringWithMultilingualWithKey:@"115341" andWithPaddingString1:kill andWithPaddingString2:loss]; //杀敌:{0} 部队损失数量：{1}
        }
        else if([self.detail.msReport intValue] == 1){
            self.mailIcon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingMailIconByName:MAILICONNAME_MAIL_ICON_BATTLE_KNIGHT];
            self.nameText = [NSString stringWithMultilingualWithKey:@"133053"]; //黑骑士活动战报
            self.contentText = [NSString stringWithMultilingualWithKey:@"133083"]; //活动结束
        }
    }
    
    if(self.contentText.length > 50)
    {
        self.contentText = [NSString formatStrByStr:self.contentText ByLength:50];
    }
//    self.hasParseCompex = true;
    
    
    [self parseFightMailReplaceDicByArray:self.detail.dfWarEffect];
    [self parseFightMailReplaceDicByArray:self.detail.atkWarEffect];
    
    [self parseMode2Json];
    
}

-(NSString*) calculateKillandLoss:(BattleMailContents*) detail
{
    BOOL isAtt = false;
    NSString *attUid = nil;
    if(self.detail.atkUser && detail.atkUser.uid.length > 0)
        attUid = self.detail.atkUser.uid;
    NSString *playerUid = [UserManager sharedUserManager].currentUser.uid;
    if(playerUid.length > 0 && [playerUid isEqualToString:attUid]){
        isAtt = true;
    }else{
        if(self.detail.atkHelper && [self.detail.atkHelper count] > 0){
            
            for (NSString *helpUid in self.detail.atkHelper ) {
                if (helpUid.length > 0 && [helpUid isEqualToString:playerUid]) {
                    isAtt = TRUE;
                    break;
                }
            }
        }
    }
    
    UserParams *attUserInfo = nil;
    if (isAtt) {
        attUserInfo = self.detail.atkUser;
    }else{
        attUserInfo = self.detail.defUser;
    }
    
    NSString *npc = @"";
    if(attUserInfo && attUserInfo.npcId.length > 0)
        npc = attUserInfo.npcId;
    
    int dead = 0;
    int num = 0;
    int hurt = 0;
    int kill = 0;
    int total = 0;
    
    if(npc.length > 0){
        if(self.detail.defReport){
            int count = [self.detail.defReport count];
            
            for (ArmyParams *army in self.detail.defReport) {
                if (army) {
                    dead += [army.dead intValue];
                    num += [army.num intValue];
                    hurt += [army.hurt intValue];
                    kill += [army.kill intValue];
                }
            }
        }
        total = dead+num+hurt;
        if(total<=0) total = 1;
    }else{
        if(isAtt){
            if(self.detail.atkArmyTotal){
                
                kill += [detail.atkArmyTotal.kill intValue];
                dead += [self.detail.atkArmyTotal.dead intValue];
                hurt += [self.detail.atkArmyTotal.hurt intValue];
                num +=  [self.detail.atkArmyTotal.num intValue];
                
            }
            
            NSMutableArray *genKillArr = self.detail.atkGenKill;
            if(genKillArr)
            {
                for (NSNumber *tempkill in genKillArr ) {
                    kill += [tempkill integerValue];
                }
            }
        }else{
            if(self.detail.defArmyTotal){
                kill += [self.detail.defArmyTotal.kill intValue];
                dead += [self.detail.defArmyTotal.dead intValue];
                hurt += [self.detail.defArmyTotal.hurt intValue];
                num += [self.detail.defArmyTotal.num intValue];
            }
            
            NSMutableArray *genKillArr = self.detail.defGenKill;
            if(genKillArr)
            {
                for (NSNumber *tempkill in genKillArr) {
                    kill += [tempkill integerValue];
                }
            }
            
            if(self.detail.defTowerKill){
                for (TowerKillParams *toweKill in self.detail.defTowerKill) {
                    if (toweKill) {
                        kill += [toweKill.kill intValue];
                    }
                }
            }
            if(self.detail.defFortLost){
                for (ArmyParams *armyParm in self.detail.defFortLost) {
                    if (armyParm) {
                        kill += [armyParm.kill intValue];
                    }
                }
            }
        }
    }

    NSString *killStr = [NSString stringWithFormat:@"%d",kill];
    NSString *deadStr = [NSString stringWithFormat:@"%d",dead];
    
    killStr = [killStr cutBit];
    deadStr = [deadStr cutBit];
    
    NSString *ret = [NSString stringWithFormat:@"%@_%@",killStr,deadStr];
    return ret;
}

-(void) parseFightMailReplaceDicByArray:(NSMutableArray*) arr
{
    for (int i = 0 ; i < [arr count] ; i ++){
        NSDictionary *tempElement =  (NSDictionary*)arr[i];
        NSString * tempElementStr = [self stringTOjson:tempElement];
        
        [arr removeObject:tempElement];
        [arr insertObject:tempElementStr atIndex:i];
    }
}


-(NSString *)stringTOjson:(id)temps   //把字典和数组转换成json字符串
{
    NSData* jsonData =[NSJSONSerialization dataWithJSONObject:temps
                                                      options:NSJSONWritingPrettyPrinted error:nil];
    NSString *strs=[[NSString alloc] initWithData:jsonData
                                         encoding:NSUTF8StringEncoding];
    
    strs = [strs stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    strs = [strs stringByReplacingOccurrencesOfString:@"\"" withString:@""];
    strs = [strs stringByReplacingOccurrencesOfString:@" " withString:@""];
    
    return strs;
}
@end

