
//
//  WorldExploreMailData.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "WorldExploreMailData.h"
#import "ExploreMailContents.h"
#import "NSString+Cocos2dHelper.h"
#import "ExporeRewardParams.h"

@interface WorldExploreMailData()
@property (nonatomic,strong) ExploreMailContents *detail;
@end

@implementation WorldExploreMailData
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
        
        self.detail = [ExploreMailContents objectWithKeyValues:maildata.contents];
        
        if(!self.detail)
            return ;
        
        self.hasParseLocal = true;
        self.nameText = [NSString stringWithMultilingualWithKey:@"108675"]; //城堡遗迹探险报告

        BOOL isExp = false;
        BOOL isloss = false;
        NSString *exptem = @"+";
        if(self.detail.reward && [self.detail.reward count] > 0)
        {
            
            for (ExporeRewardParams *reward in self.detail.reward) {
                if(reward)
                {
                    int value = [reward.value intValue];
                    int type = [reward.type intValue];
                    if(type == 6)
                    {
                        isExp = true;
                        exptem = [NSString stringWithFormat:@"%@%@",exptem,[NSString gettingRoundFormatNumber:value]];
                    }
                    
                }
            }
        }
        
        int numLoss = 0;
        if([self.detail.trap count] > 0)
        {
            for (NSString *key in self.detail.trap) {
                if (key.length > 0) {
                    isloss = true;
                    NSNumber *object=[self.detail.trap objectForKey:key];
                    if (object > 0 ) {
                        numLoss += [object longValue];
                    }
                }
            }
        }
        
        if(isExp&&isloss){
            NSString *paramStr1 = @"EXP";
            NSString *paramStr2 = exptem;
            NSString *paramStr3 = @"  ";
            NSString *paramStr4 = [NSString stringWithMultilingualWithKey:@"105019"]; //部队损失数量
            NSString *paramStr5 = @" ";
            NSString *paramStr6 = [NSString gettingRoundFormatNumber:numLoss];
            self.contentText = [NSString stringWithFormat:@"%@%@%@%@%@%@",paramStr1,paramStr2,paramStr3,paramStr4,paramStr5,paramStr6];
        }else if(isExp){
            self.contentText = [NSString stringWithFormat:@"EXP%@",exptem];
        }else if(isloss){
            NSString *paramStr1 = [NSString stringWithMultilingualWithKey:@"105019"]; //部队损失数量
            NSString *paramStr2 = @" ";
            NSString *paramStr3 = [NSString gettingRoundFormatNumber:numLoss];
            self.contentText = [NSString stringWithFormat:@"%@%@%@",paramStr1,paramStr2,paramStr3];
        }
        
        if(self.contentText.length>50)
        {
            self.contentText = [NSString formatStrByStr:self.contentText ByLength:50];
        }
        self.hasParseCompex = true;
      
    }
    
    [self parseMode2Json];
}
@end
