
//
//  InviteTeleportMailData.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "InviteTeleportMailData.h"
#import "InviteTeleportMailContents.h"

@interface InviteTeleportMailData()
@property (nonatomic,strong) InviteTeleportMailContents *detail;
@end

@implementation InviteTeleportMailData

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
    
    self.detail = [InviteTeleportMailContents objectWithKeyValues:maildata.contents];
    
    if(self.detail)
    {
        if (self.nameText.length <= 0) {
            self.nameText = self.detail.inviterName;
        }
//        self.hasParseLocal = true;
        NSDictionary *dic = self.keyValues;
        self.jsonStr = [dic dataTOjsonString];
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
