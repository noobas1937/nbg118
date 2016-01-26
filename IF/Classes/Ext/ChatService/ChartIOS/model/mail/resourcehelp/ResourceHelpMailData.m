//
//  ResourceHelpMailData.m
//  IF
//
//  Created by 马纵驰 on 15/9/26.
//
//

#import "ResourceHelpMailData.h"
#import "ResourceHelpMailContents.h"
#import "ChatServiceController.h"
#import "RewardParams.h"
#import "NSString+Cocos2dHelper.h"

@implementation ResourceHelpMailData

-(instancetype)initWithAddMailData:(MailInfoDataModel*) maildata;
{
    self = [super initWithAddMailData:maildata];
    if (self) {
        if (maildata.mail_ID.length > 0 && maildata.creatTime && maildata) {
            
            self.collect = [[NSMutableArray alloc]init];
            self.unread = 0;
            self.totalNum = 0;
            
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
    ResourceHelpMailContents *detail = [ResourceHelpMailContents objectWithKeyValues:maildata.contents];
    
    detail.uid = self.uid;
    self.totalNum = 1;
    if([self.status intValue] == 0)
        self.unread = 1;
    else
        self.unread = 0;
    detail.fromName = self.fromName;
    long time = [self.createTime longLongValue] ;
    detail.createTime = [NSString stringWithFormat:@"%ld",time];
    [self.collect addObject:detail];
    if(self.collect)
        self.hasParseLocal = true;
    
    if(!detail)
        return;
    NSString *tempContent = @"";
    if(detail.reward && [detail.reward count] > 0)
    {
        RewardParams *helpReward =  [detail.reward objectAtIndex:0];
        if(helpReward)
        {
            int type = [helpReward.t intValue];
            int value = [helpReward.v intValue];
            NSString *icon = [[ChatServiceController  chatServiceControllerSharedManager].gameHost gettingPicByType:type withVal:value];
            NSString *valueStr = [NSString stringWithFormat:@"%d",value];
            tempContent = [NSString stringWithFormat:@"[%@]+%@",icon,valueStr];
        }
        
        if([detail.reward count] > 1)
        {
            RewardParams *helpReward1 = [detail.reward objectAtIndex:1];
            if(helpReward1)
            {
                int type = [helpReward1.t intValue];
                int value = [helpReward1.v intValue];
                NSString *icon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingPicByType:type withVal:value];
                if(value>0){
                    NSString *valueStr = [NSString stringWithFormat:@"%d",value];
                    tempContent = [NSString stringWithFormat:@"%@[%@]+%@ \n",tempContent,icon,valueStr];
                }
            }
        }
        
        if([detail.reward count] > 2)
        {
            RewardParams *helpReward2 =  [detail.reward objectAtIndex:2];
            if(helpReward2)
            {
                int type = [helpReward2.t intValue];
                int value = [helpReward2.v intValue];
                NSString *icon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingPicByType:type withVal:value];
                if(value>0){
                    NSString *valueStr = [NSString stringWithFormat:@"%d",value];
                    tempContent = [NSString stringWithFormat:@"%@[%@]+%@",tempContent,icon,valueStr];
                }
            }
        }
        
        if([detail.reward count] > 3)
        {
            RewardParams *helpReward3 =  [detail.reward objectAtIndex:3];
            if(helpReward3)
            {
                int type = [helpReward3.t intValue];
                int value = [helpReward3.v intValue];
                NSString *icon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingPicByType:type withVal:value];
                if(value>0){
                    NSString *valueStr = [NSString stringWithFormat:@"%d",value];
                    tempContent = [NSString stringWithFormat:@"%@[%@]+%@",tempContent,icon,valueStr];
                }
            }
        }
    }
    
    self.contentText = tempContent;
    if(detail)
    {
        self.hasParseLocal = true;
        [self parseMode2Json];
    }
}

@end
