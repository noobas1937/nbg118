//
//  ResourceMailData.m
//  IF
//
//  Created by 马纵驰 on 15/9/11.
//
//

#import "ResourceMailData.h"
#import "ResourceMailContents.h"
#import "RewardParams.h"
#import "ChatServiceController.h"
#import "NSString+Cocos2dHelper.h"
#import "DropParams.h"
#import "NSString+Cocos2dHelper.h"


@interface ResourceMailData()

@end

@implementation ResourceMailData


-(instancetype)initWithAddMailData:(MailInfoDataModel*) maildata withAddCount:(int) count
{
    self = [super initWithAddMailData:maildata];
    if (self) {
        
        [self mappingArray];
        
        if (maildata.mail_ID.length > 0 && maildata.creatTime && maildata) {
            
            self.collect = [[NSMutableArray alloc]init];
            self.unread = 0;
            self.totalNum = count;
            
            [self parseContents:maildata];
        }
    }
    return self;
}

-(instancetype)initWithAddMailData:(MailInfoDataModel*) maildata
{
    self = [super initWithAddMailData:maildata];
    if (self) {
        
        [self mappingArray];
        
        if (maildata.mail_ID.length > 0 && maildata.creatTime && maildata) {
            
            self.collect = [[NSMutableArray alloc]init];
            self.unread = 0;
            self.totalNum = 0;
            
            [self parseContents:maildata];
        }
    }
    return self;
}

-(void) parseContents:(MailInfoDataModel*) maildata;
{
    
    [super parseContents];
    
    if (maildata.contents.length <= 0) {
        return ;
    }
    
    ResourceMailContents *detail = [ResourceMailContents objectWithKeyValues:maildata.contents];
    
    detail.uid = self.uid;
    detail.createTime = self.createTime;
    [self.collect addObject:detail];
    if(!self.collect)
        self.hasParseLocal = true;
    
    if(self.hasParseCompex)
        return;
    if(detail)
    {
        self.contentText = [NSString stringWithMultilingualWithKey:@"108896"]; //很可惜，您慢了一步！该资源点已消失！
    }
    else
    {
        BOOL resourceResult = false;
        if(!detail.reward && [detail.reward count] <= 0)
        {
            resourceResult = false;
        }
        else
        {
            resourceResult = true;
        }
        
        if(resourceResult)
        {
            RewardParams *reward = [detail.reward objectAtIndex:0];
            if(reward)
            {
                int type = [reward.t intValue];
                int value = [reward.v intValue];
                NSString *icon = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingPicByType:type withVal:value];
                
                self.contentText = [NSString stringWithFormat:@"[%@]+%@",icon,value];
            }
            else
            {
                self.contentText = [NSString stringWithMultilingualWithKey:@"108896"]; //很可惜，您慢了一步，该资源点已消失
            }
        }
        else
            self.contentText = [NSString stringWithMultilingualWithKey:@"108896"]; //很可惜，您慢了一步，该资源点已消失
    }
    
    self.hasParseCompex = true;
    
    [self parseMode2Json];
}

/**
 *  映射 json 和 array
 */
-(void) mappingArray
{
    [ResourceMailData setupObjectClassInArray:^NSDictionary *{
        return @{
                 @"collect" : @"ResourceMailContents"
                 };
    }];
}


@end
