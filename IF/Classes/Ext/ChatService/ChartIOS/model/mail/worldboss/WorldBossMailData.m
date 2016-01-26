//
//  WorldBossMailData.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "WorldBossMailData.h"
#import "WorldBossMailContents.h"
#import "AttListParams.h"
#import "NSString+Cocos2dHelper.h"


@implementation WorldBossMailData

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

        self.detail = [WorldBossMailContents objectWithKeyValues:maildata.contents];
        if(!self.detail)
            return;
        
        if(self.detail.attList && [self.detail.attList count] > 0)
        {
            
            //得到队长的名字
            NSString *leaderName=@"";
            for (NSDictionary *att in self.detail.attList) {
                if(att)
                {
                    NSString *name=[att objectForKey:@"name"];
                    if([[att objectForKey:@"leader"] intValue] == 1 && name.length > 0)
                    {
                        leaderName=name;
                        break;
                    }
                }
            }

            self.contentText = [NSString stringWithMultilingualWithKey:@"137450" andWithPaddingString1:leaderName];//{0}集结的队伍挑战了守护者
            
            if(self.contentText.length > 50)
            {
                self.contentText = [NSString formatStrByStr:self.contentText ByLength:50];
            }
        }
    }
    
    
    
    if(!self.detail)
        return ;
    
    [self parseMode2Json];
}

@end
