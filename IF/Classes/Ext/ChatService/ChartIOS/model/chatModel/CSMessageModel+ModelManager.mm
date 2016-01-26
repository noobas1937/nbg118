//
//  CSMessageModel+ModelManager.m
//  IF
//
//  Created by 张彦涛 on 15/12/15.
//
//

#import "CSMessageModel+ModelManager.h"
#import "NSDateFormatter+Category.h"
#import "CSMessage.h"
#import "NSString+Cocos2dHelper.h"
#import "UserManager.h"
#import "NSUserInfo.h"
#import "ServiceInterface.h"
#import "NSString+Cocos2dHelper.h"
#import "CSMessageModel+CustemColor.h"
#import "NSUserInfo+UserManager.h"


@implementation CSMessageModel (ModelManager)

-(void)messageModelShowingFormat{
 
    self.timeString = [self stringWithDateFormat:[NSDateFormatter custemDateFarmetterWithYearToMinutes] AndWithTimeDate:self.message.createTime];
    
    [self settingContestsString];
    [self settingTransliteString];
    [self settingUserInfo];
    [self calculateWithTextBubbleView];
    
}


-(void)settingUserInfo{
    NSUserInfo *user= [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:self.message.uid];
    if (user) {
        self.currentUser = user;
        self.nickname = user.userName;
        
        NSString *nickNameString = @"";
        
        if (self.message.channelType == IOS_CHANNEL_TYPE_ALLIANCE) {
            //联盟消息
        }else{
            if (user.asn.length > 0) {
                nickNameString = [nickNameString stringByAppendingFormat:@"(%@)",user.asn];
            }
        }
        if (user.userName.length> 0) {
            nickNameString = [nickNameString stringByAppendingString:user.userName];
        }
        if (nickNameString.length> 0) {
            self.nickname = nickNameString ;
        }
        NSString *vipString =[user vipStringWithSelf];
        if (user.isVipShow && vipString.length> 0) {
            nickNameString= vipString;
            nickNameString = [nickNameString stringByAppendingString:@" "];
            nickNameString = [nickNameString stringByAppendingString:self.nickname];
            self.nickNameAttribut =[[NSMutableAttributedString alloc]initWithString:nickNameString];
            [self.nickNameAttribut addAttribute:NSForegroundColorAttributeName value:[UIColor yellowColor] range:NSMakeRange(0,vipString.length)];
        }

        
        if (user.headPicVer> 0) {
            NSString *headImageUrlString =[NSString gettingCustomHeadPic:user.uid intValue:user.headPicVer];
            self.avatarCustomURLPath = headImageUrlString;
             self.avatarDefaultImageName = user.headPic;
        }else{
            self.avatarDefaultImageName = user.headPic;
        }
    }
    
}



-(void)settingContestsString{
    
  
    if (self.messageBodyType == CSMessageBodyType_Text_EquipShare) {
        self.isTranslatLabelShow = NO;
        self.showContentsString = @"[sysChat]";
        NSArray *equipInfo = [self.message.msg componentsSeparatedByString:@"|"];
        if  (equipInfo.count >= 2   ){
            self.changeTextColor =[self colorWithColorDefine:[equipInfo[0]intValue]];
 
            self.changeTextString = equipInfo[1];
            if (self.changeTextString.length> 0) {
                self.changeTextString =   [NSString stringWithMultilingualWithKey:self.changeTextString];
            }
            //111660=我刚刚在铁匠铺成功的锻造出了{0}
             self.showContentsString = [self.showContentsString stringByAppendingString:[NSString stringWithMultilingualWithKey:@"111660" andWithPaddingString1:[NSString stringWithMultilingualWithKey:equipInfo[1]]]];
            self.showContentsString = [self.showContentsString stringByAppendingString:@"[equip_share]"];
        }
        
       
        
    }else if(self.messageBodyType == CSMessageBodyType_Text_AllianceTask){
        self.showContentsString = @"[sysChat]";
       
        NSString *jsonString = nil;
        NSString *dialog_describe= nil;
        NSString *dialog_task= nil;
        
        NSString *firstName= nil;
        NSString *secondName = nil;
        if (self.message.msg.length> 0){
            NSArray *dataArray =[self.message.msg componentsSeparatedByString:@"|"];
            if (dataArray.count >= 4) {
                self.changeTextColor = [self colorWithColorDefine:[dataArray[0]intValue]];
                dialog_describe =dataArray[1];
                dialog_task = dataArray[2];
                jsonString = dataArray[3];
                self.changeTextString = [NSString stringWithMultilingualWithKey:dialog_task];
                id object  =[jsonString JSONValue];
//                DVLog(@"%@",object);
                if ([object isKindOfClass:[NSArray class]]) {
                    for (int xx = 0;xx<[(NSArray *)object count];xx++) {
                        NSDictionary *dic = [object objectAtIndex:xx];
                        if (xx ==0) {
                            firstName =[dic objectForKey:@"name"];
                        }else{
                            secondName = [dic objectForKey:@"name"];
                        }
                    }
                }
                if (secondName && firstName) {
                    //134055={0}刚刚发布的联盟任务{1}已经被{2}接取
                    NSString *showString =[NSString stringWithMultilingualWithKey:dialog_describe andWithPaddingString1:firstName andWithPaddingString2:self.changeTextString andWithPaddingString3:secondName];
                    self.showContentsString = [self.showContentsString stringByAppendingString:showString];
                    
                }else if (firstName){
                     //134054={0}刚刚发布了联盟任务{1}，请大家前往协助
                     NSString *showString =[NSString stringWithMultilingualWithKey:dialog_describe andWithPaddingString1:firstName andWithPaddingString2:self.changeTextString];
                     self.showContentsString = [self.showContentsString stringByAppendingString:showString];
                }else{
                    
                }
                
            }
            
            
        }
    }else if (self.messageBodyType == CSMessageBodyType_Text_FightReport || self.messageBodyType == CSMessageBodyType_Text_InvestigateReport){
        
        self.showContentsString = @"[sysChat]";
        self.showContentsString = [self.showContentsString stringByAppendingString:self.message.msg];
        self.showContentsString = [self.showContentsString stringByAppendingString:@"[battlereport]"];
    }else if(self.messageBodyType == CSMessageBodyType_Text_AllianceAdded   ||
             self.messageBodyType == CSMessageBodyType_Text_AllianceCreated  ||
             self.messageBodyType == CSMessageBodyType_Text_AllianceInvite  ||
             self.messageBodyType == CSMessageBodyType_Text_SayHello        ||
             self.messageBodyType == CSMessageBodyType_Text_AllianceConciles ||
             self.messageBodyType == CSMessageBodyType_Text_TurntableShare   ){
  
        self.showContentsString = @"[sysChat]";
        self.showContentsString = [self.showContentsString stringByAppendingString:self.message.msg];
    }
    else if (self.messageBodyType ==CSMessageBodyType_Text_ChatRoomSystemMsg ){
        self.showContentsString = self.message.msg;
    }
    else if (self.messageBodyType ==CSMessageBodyType_RedPackge ){
        self.showContentsString = self.message.msg;
    }
    else{
        if (self.messageBodyType == CSMessageBodyType_Text_Normal ||
            self.messageBodyType == CSMessageBodyType_Text_Loudspeaker ||
            self.messageBodyType == CSMessageBodyType_Text_ModMsg) {
            if (self.isTranslatLabelShow) {
               self.showContentsString = self.message.translateMsg;
            }else{
                self.showContentsString = self.message.msg;
            }
            
        }    
    }

}

-(void)settingTransliteString{
    if (self.isTranslatLabelShow) {
        NSString *dialogKey = @"lang_";
        dialogKey = [dialogKey stringByAppendingFormat:@"%@",[self checkOriginalLangString:self.message.originalLang ] ];
        NSString *dialog= [NSString stringWithMultilingualWithKey:dialogKey];
        NSString *transStringFrom = nil;
        if (dialog.length> 0){
         transStringFrom   =[NSString stringWithMultilingualWithKey:@"105321" andWithPaddingString1:dialog];// @"105321= 由{0}翻译

        }else{
           transStringFrom =[NSString stringWithMultilingualWithKey:@"105321" andWithPaddingString1:self.message.originalLang];// @"105321= 由{0}翻译

        }
        self.translatExplainString = transStringFrom;

    }else{
         NSString *originallanStr = [NSString stringWithMultilingualWithKey:@"105322"];//105322=原文
         self.translatExplainString =originallanStr;
    }
   
}

-(NSString *)checkOriginalLangString:(NSString *)vOrignalString {
    if ([vOrignalString isEqualToString:@"ara"]){
        return @"ar";
    }else if ([vOrignalString isEqualToString:@"bs-Latn"]){
        return @"bs";
    }else if ([vOrignalString isEqualToString:@"cht"] || [vOrignalString isEqualToString:@"zh-CHT"] || [vOrignalString isEqualToString:@"zh_TW"]|| [vOrignalString isEqualToString:@"zh-Hant"]){
        return @"zh-TW";
    }else if ([vOrignalString isEqualToString:@"zh-CHS"]|| [vOrignalString isEqualToString:@"zh"] || [vOrignalString isEqualToString:@"zh_CN"]|| [vOrignalString isEqualToString:@"zh-Hans"] ){
        return @"zh-CN";
    }else if ([vOrignalString isEqualToString:@"dan"]){
        return @"da";
    }else if ([vOrignalString isEqualToString:@"bul"]){
        return @"bg";
    }else if ([vOrignalString isEqualToString:@"est"]){
        return @"et";
    }else if ([vOrignalString isEqualToString:@"fin"]){
        return @"fi";
    }else if ([vOrignalString isEqualToString:@"fra"]){
        return @"fr";
    }else if ([vOrignalString isEqualToString:@"jp"]){
        return @"ja";
    }else if ([vOrignalString isEqualToString:@"kor"]){
        return @"ko";
    }else if ([vOrignalString isEqualToString:@"slo"]){
        return @"sl";
    }else if ([vOrignalString isEqualToString:@"sr-Cyrl"] || [vOrignalString isEqualToString:@"sr-Latn"]){
        return @"sr";
    }else if ([vOrignalString isEqualToString:@"rom"]){
        return @"ro";
    }else{
        return vOrignalString;
    }
    
}

/**时间戳转字符串*/
-(NSString*) stringWithDateFormat:(NSDateFormatter *)vDateFormat AndWithTimeDate:(long)vTimeDate;
{
    if ( vTimeDate && vTimeDate> 0) {
        NSDate *data = [NSDate dateWithTimeIntervalSince1970:vTimeDate];
        NSString *timeStr = [vDateFormat stringFromDate:data];
        return timeStr;
        
    }else{
        return @"时间不详";
    }
}


@end
