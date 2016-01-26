//
//  OcupyMailData.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "MailData.h"
#import "OcupyMailContents.h"
@interface OcupyMailData : MailData
@property (nonatomic,strong) OcupyMailContents *detail;
@end
