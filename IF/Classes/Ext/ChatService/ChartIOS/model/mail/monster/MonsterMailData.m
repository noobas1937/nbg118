//
//  MonsterMailData.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "MonsterMailData.h"
#import "MonsterMailContents.h"
#import "ChatServiceController.h"
#import "NSString+Cocos2dHelper.h"

@implementation MonsterMailData

-(instancetype)initWithAddMailData:(MailInfoDataModel*) maildata withAddCount:(int) count
{
    self = [super initWithAddMailData:maildata];
    if (self) {
        self.monster = [[NSMutableArray alloc] init];
        [self mappingArray];
        
        if (maildata.mail_ID.length > 0 && maildata.creatTime && maildata) {
            [self parseContents:maildata withAddCount:count];
        }
    }
    return self;
}

-(void) parseContents:(MailInfoDataModel*) maildata withAddCount:(int) count
{
    [super parseContents];
    if (maildata.contents.length <= 0) {
        return ;
    }
    
    if([self.status intValue] == 0)
        self.unread = @"1";
    else
        self.unread = @"0";
    self.totalNum = [NSString stringWithFormat:@"%d",count];
    MonsterMailContents *detail = [MonsterMailContents objectWithKeyValues:maildata.contents];
    if(!detail)
        return;
    detail.uid = self.uid;
    detail.createTime = self.createTime;
    detail.type = [NSString stringWithFormat:@"%d",maildata.type];
    [self.monster addObject:detail];
//    if(self.monster)
//        self.hasParseLocal = true;
    
    if(!detail)
        return;
    
    DefParams *def = detail.def;
    if(!def)
        return;
    NSString *name = @"";
    NSString *level = @"";
    if(def.ID.length > 0)
    {
        name = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingNameById:def.ID];
        
        level = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingPropById:def.ID ByLeve:@"level"];
    }
    [name stringByAppendingString:@" Lv."];
    [name stringByAppendingString:level];
    
    self.contentText = name;
    [self.contentText stringByAppendingString:@"  "];
    
    int monsterResult=-1;
    if(detail.stat.length > 0){
        monsterResult = 4; //怪物消失
    }else{
        if([detail.firstKill intValue] == 1){
            monsterResult = 1;//首次杀
        }else if([def.mchp intValue]>0){//失败
            monsterResult = 2;//
        }else{
            monsterResult = 3;//
        }
    }
    
    if(monsterResult==1){
        /**
         *  首杀奖励 103758
         */
        [self.contentText stringByAppendingString:[NSString stringWithMultilingualWithKey:@"103758"]];
    }else if(monsterResult==2){
        /**
         *  战斗失败 105118
         */
        [self.contentText stringByAppendingString:[NSString stringWithMultilingualWithKey:@"105118"]];
    }else if(monsterResult == 4){
        /**
         *  战斗无效 103786
         */
        [self.contentText stringByAppendingString:[NSString stringWithMultilingualWithKey:@"103786"]];
    }else{
        /**
         *  战斗胜利 105117
         */
        [self.contentText stringByAppendingString:[NSString stringWithMultilingualWithKey:@"105117"]];
    }
    if(self.contentText.length > 50)
    {
        self.contentText = [NSString formatStrByStr:self.contentText ByLength:50];
    }
    
    [self parseMode2Json];
}

/**
 *  映射 json 和 array
 */
-(void) mappingArray
{
    [MonsterMailData setupObjectClassInArray:^NSDictionary *{
        return @{
                 @"monster" : @"MonsterMailContents"
                 };
    }];
}


-(void)parseMode
{
    NSDictionary *dic = self.keyValues;
    
    self.jsonStr = [dic dataTOjsonString];
}

@end
