//
//  OcupyMailData.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "OcupyMailData.h"
#import "OcupyMailContents.h"
#import "NSString+Cocos2dHelper.h"
#import "ChatServiceController.h"

@implementation OcupyMailData
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
    
    if (maildata.contents.length <= 0) {
        return ;
    }

    if(self.contents.length > 0)
    {
        self.detail = [OcupyMailContents objectWithKeyValues:maildata.contents];
        if(!self.detail)
            return;
        else
        {
            self.hasParseLocal = true;
            switch ([self.detail.pointType intValue]) {
                case Mail_Throne:{
                    self.nameText = [NSString stringWithMultilingualWithKey:@"105710"]; //占领宫殿
                    self.contentText = [NSString stringWithMultilingualWithKey:@"105712"]; //你成功占领了宫殿
                    break;
                }
                case Mail_Trebuchet:{
                    self.nameText = [NSString stringWithMultilingualWithKey:@"105711"]; //占领投石机
                    self.contentText = [NSString stringWithMultilingualWithKey:@"105713"]; //你成功占领了投石机
                    break;
                }
                default:{
                    NSString *cordX=@"";
                    NSString *cordY=@"";
                    
                    NSString *pt = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingPointByOccupyIdx:[self.detail.pointId intValue]];
                    
                    if([self.detail.serverType intValue] == Mail_Server_Battle_Field)
                        pt = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingPointByMapIndex:[self.detail.pointId intValue] ByType:[self.detail.pointType intValue]];
                    if(pt.length > 0)
                    {
                        NSArray *cord = [pt componentsSeparatedByString:@"_"];
                        
                        if([cord count] == 2)
                        {
                            cordX = cord[0];
                            cordY = cord[1];
                        }
                    }
                    
                    if([self.detail.isTreasureMap boolValue])
                    {
                        self.nameText = [NSString stringWithMultilingualWithKey:@"111504"];  //先贤的馈赠
                        self.contentText = [NSString stringWithMultilingualWithKey:@"111506" andWithPaddingString1:@"" andWithPaddingString2:cordX andWithPaddingString3:cordY]; //成功驻扎{0}（X：{1} Y：{2}）并开始挖掘宝藏
                    }
                    else
                    {
                        self.nameText = [NSString stringWithMultilingualWithKey:@"105537"]; //占领成功
                        
                        self.contentText = [NSString stringWithMultilingualWithKey:@"105538" andWithPaddingString1:@"" andWithPaddingString2:cordX andWithPaddingString3:cordY]; //您成功占领了{0}（X：{1} Y：{2}）
                    }

                    break;
                }
            }
            if(self.contentText.length > 50)
            {
                self.contentText = [NSString formatStrByStr:self.contentText ByLength:50];
            }
            
            self.hasParseCompex = true;
        }
    }

    
    [self parseMode2Json];
    
}
@end
