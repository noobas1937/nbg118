 //
//  DetectReportMailData.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "DetectReportMailData.h"
#import "DetectReportMailContents.h"
#import "NSString+Cocos2dHelper.h"
#import "MailManagerKey.h"
#import <math.h>
#import "ReinAboutDetailParams.h"

@implementation DetectReportMailData
-(instancetype)initWithAddMailData:(MailInfoDataModel*) maildata;
{
    self = [super initWithAddMailData:maildata];
    if (self) {
        if (maildata.mail_ID.length > 0 && maildata.creatTime && maildata) {
            
            [self parseContents:maildata];
        }
    }
    return self;
}

-(void) parseContents:(MailInfoDataModel*) maildata
{
    [super parseContents];
    
    if(self.contents.length > 0)
    {
        if([self.contents isEqualToString:@"114005"]) //对方使用了反侦察道具，无法获得侦查信息
        {
            self.nameText = [NSString stringWithMultilingualWithKey:@"114102"]; //侦查失败
            self.contentText = [NSString stringWithMultilingualWithKey:@"114005"];
            self.detail = [[DetectReportMailContents alloc] init];
            self.detail.contents = self.contents;
        }
        else
        {
            self.detail = [DetectReportMailContents objectWithKeyValues:maildata.contents];
            if(!self.detail)
                return;
            
            
            switch ([self.detail.pointType intValue]) {
                case Mail_Throne:{ //王座
                    self.nameText = [NSString stringWithMultilingualWithKey:@"105708"]; //侦查宫殿成功
                    break;
                }
                case Mail_Trebuchet:{//投石机
                    self.nameText = [NSString stringWithMultilingualWithKey:@"105709"]; //侦查投石机成功
                    break;
                }
                case Mail_Tile_allianceArea:{
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115356"]; //侦查联盟堡垒成功
                    break;
                }
                case Mail_tile_banner:{
                    self.nameText = [NSString stringWithMultilingualWithKey:@"115539"]; //侦查国旗成功
                    break;
                }
                default:{
                    self.nameText = [NSString stringWithMultilingualWithKey:@"105527"]; //侦查成功
                    break;
                }
            }
            
            NSString *nameStr=@"";
            self.contentText = [NSString stringWithMultilingualWithKey:@"105527"];
            UserInfoParams *user=self.detail.user;
            if(user)
            {
                if([user.noDef intValue] == 1)
                {
                    
                    NSString *asn=user.abbr;
                    if(asn.length > 0)
                        [NSString stringWithFormat:@"%@(%@)",nameStr,asn];
                    NSString *name=user.name;
                    if(name.length > 0)
                    {
                        nameStr = [nameStr stringByAppendingString:name];
                    }
                    else
                    {
                        if ([self.detail.pointType intValue] == Mail_Tile_allianceArea) {
                            if([user.count intValue] > 0)
                                nameStr = [nameStr stringByAppendingString:[NSString stringWithMultilingualWithKey:@"115312" andWithPaddingString1:user.count]];//联盟堡垒{0}
                            else
                                nameStr = [nameStr stringByAppendingString:[NSString stringWithMultilingualWithKey:@"115312" andWithPaddingString1:@"1"]];//联盟堡垒{0}
                        }
                        else if ([self.detail.pointType intValue] == Mail_tile_banner) {
                            nameStr = [nameStr stringByAppendingString:[NSString stringWithMultilingualWithKey:@"115534"]];//国旗
                        }
                    }
                    self.contentText = [NSString stringWithMultilingualWithKey:@"114101" andWithPaddingString1:nameStr]; ////已侦查到{0}的信息
                }
                else
                {
                    nameStr=user.name;
                    NSString *asn=user.abbr;
                    if(asn.length > 0)
                        nameStr = [NSString stringWithFormat:@"(%@)%@",asn,nameStr];
                    self.contentText = [NSString stringWithMultilingualWithKey:@"114101" andWithPaddingString1:nameStr]; //已侦查到{0}的信息
                }
            }
            
            int defenceNum = 0;
            if(self.detail.defence)
            {
                defenceNum = [self.detail.defence.total intValue];
            }else{
                defenceNum = -1;
            }
            
            NSString *defenceInfo = @"";
            if (defenceNum >= 0) {
                defenceInfo = [NSString gettingRoundFormatNumber:defenceNum];
            }else{
                defenceInfo = @"NA";
            }
            
            int resourceNum = 0;
            if(self.detail.resource)
            {
                resourceNum = (int)([self.detail.resource.food intValue] +
                                    [self.detail.resource.food_not_collected intValue] +
                                    [self.detail.resource.wood intValue] +
                                    [self.detail.resource.wood_not_collected intValue] +
                                    [self.detail.resource.iron intValue] +
                                    [self.detail.resource.iron_not_collected intValue] +
                                    [self.detail.resource.stone intValue] +
                                    [self.detail.resource.stone_not_collected intValue]);
            }
            NSString *resourInfo = [NSString gettingRoundFormatNumber:resourceNum];
            
            int helperNum = 0;
            
            if(self.detail.rein_about_detail && [self.detail.rein_about_detail count] > 0){
                if(!self.detail.rein_about){
                    NSMutableArray *rein_about_detailList = self.detail.rein_about_detail;
                    for(int i=0;i<[rein_about_detailList count];i++)
                    {
                        NSMutableArray *reinAboutDetailParamsList = (NSMutableArray*)[rein_about_detailList objectAtIndex:i];
                        if(reinAboutDetailParamsList && [reinAboutDetailParamsList count] > 0)
                        {
                            for(int j=0;j<[reinAboutDetailParamsList count];j++)
                            {
                                ReinAboutDetailParams *params = [reinAboutDetailParamsList objectAtIndex:j];
                                if(params)
                                {
                                    if ([params isKindOfClass:[NSDictionary class]]) {
                                        int paramsCount = [(NSDictionary*)params objectForKey:@"count"];
                                    }else if([params isKindOfClass:[ReinAboutDetailParams class]]){
                                        helperNum += [params.count intValue];
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(self.detail.rein_about)
                {
                    helperNum = [self.detail.rein_about.total intValue];
                }else{
                    helperNum = -1;
                }
            }
            
            NSString *helperInfo = @"";
            if (helperNum >= 0) {
                helperInfo = [NSString gettingRoundFormatNumber:helperNum];
            }else{
                helperInfo = @"NA";
            }
            
            
            self.contentText = [NSString stringWithMultilingualWithKey:@"132111" andWithPaddingString1:defenceInfo andWithPaddingString2:resourInfo andWithPaddingString3:helperInfo]; //守军：{0} 资源：{1} 援军：{2}
            
            if(self.contentText.length > 50)
            {
                self.contentText = [NSString formatStrByStr:self.contentText ByLength:50];
            }
            
            if(nameStr.length > 0)
                self.contentText = [NSString stringWithFormat:@"%@\n%@",nameStr,self.contentText];
        }
    }
    
    if(!self.detail)
        return;
    
    NSDictionary *dic = self.keyValues;
    
    self.jsonStr = [dic dataTOjsonString];
}

@end
