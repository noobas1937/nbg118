//
//  MailInfoManager.h
//  IF
//
//  Created by 张彦涛 on 15/8/17.
//
//

#import <Foundation/Foundation.h>
#import "MailInfo.h"
#import "MailInfoDataModel.h"
@interface MailInfoManager : NSObject
+(MailInfoDataModel *)maildataWithMailInfo:(MailInfo *)vMailInfo;
@end
