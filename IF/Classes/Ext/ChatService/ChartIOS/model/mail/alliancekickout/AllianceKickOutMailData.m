//
//  AllianceKickOutMailData.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "AllianceKickOutMailData.h"
#import "AllianceKickOutMailContents.h"


@interface AllianceKickOutMailData()
@property (nonatomic,strong) AllianceKickOutMailContents *detail;
@end

@implementation AllianceKickOutMailData
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
    
    self.detail = [AllianceKickOutMailContents objectWithKeyValues:maildata.contents];
    
    if(self.detail)
    {
//        self.hasParseLocal = true;
        [self parseMode2Json];
    }
}
@end
