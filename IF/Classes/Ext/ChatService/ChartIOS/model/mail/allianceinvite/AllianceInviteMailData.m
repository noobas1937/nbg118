//
//  AllianceInviteMailData.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "AllianceInviteMailData.h"
#import "AllianceInviteMailContents.h"

@interface AllianceInviteMailData()
@property (nonatomic,strong) AllianceInviteMailContents *detail;
@end

@implementation AllianceInviteMailData

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
    
    if (self.hasParseLocal) {
        return ;
    }
    
    [super parseContents];
    
    if (maildata.contents.length <= 0) {
        return ;
    }
    
    self.detail = [AllianceInviteMailContents objectWithKeyValues:maildata.contents];
    
    if(self.detail)
    {
        [self parseMode2Json];
        self.hasParseLocal = TRUE;
    }
}

-(void)settingMailDealStatus
{
    if (self.detail)
    {
        self.detail.deal = @"1";
        if (self.contents.length > 0 && [self.contents containsString:@"deal"])
        {
            NSDictionary *dic = self.detail.keyValues;
            NSString *jsonStr = [dic dataTOjsonString];
            self.contents = jsonStr;
            self.jsonStr = jsonStr;
        }
    }
}

@end
