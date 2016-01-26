//
//  RefuseAllReplyMailData.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "RefuseAllReplyMailData.h"
#import "RefuseAllReplyMailContents.h"

@interface RefuseAllReplyMailData()
@property (nonatomic,strong) RefuseAllReplyMailContents *detail;

@end

@implementation RefuseAllReplyMailData
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
    
    self.detail = [RefuseAllReplyMailContents objectWithKeyValues:maildata.contents];
    
    if(self.detail)
    {
//        self.hasParseLocal = true;
        [self parseMode2Json];
    }
}
@end
