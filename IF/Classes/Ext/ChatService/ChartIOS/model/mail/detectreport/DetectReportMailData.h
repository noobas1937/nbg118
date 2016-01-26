//
//  DetectReportMailData.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "MailData.h"
#import "DetectReportMailContents.h"

@interface DetectReportMailData : MailData
@property (nonatomic,strong) DetectReportMailContents *detail;
@end
