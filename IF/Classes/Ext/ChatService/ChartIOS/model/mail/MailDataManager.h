//
//  MailDataManager.h
//  IF
//
//  Created by 马纵驰 on 15/11/10.
//
//

#import <Foundation/Foundation.h>
#import "MailData.h"
@interface MailDataManager : NSObject
+(MailData *)mailDataWithMailInfoData:(MailInfoDataModel *)vMailInfoDataModel;

+(MailData *)maildataWIthMailID:(NSString *)vMailID;
@end
