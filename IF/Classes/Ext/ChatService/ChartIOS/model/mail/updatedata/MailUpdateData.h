//
//  MailUpdateData.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>

@interface MailUpdateData : NSObject
@property (nonatomic,strong) NSString *uid;
@property (nonatomic,strong) NSString *status;						// 是否未读，大于1则是
@property (nonatomic,strong) NSString *rewardStatus;
@property (nonatomic,strong) NSString *saveFlag;						// 0未保存,1保存,2删除保存过
@property (nonatomic,strong) NSString *mailLastUpdateTime;
@end
