//
//  WorldBossMailData.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "MailData.h"
#import "WorldBossMailContents.h"
@interface WorldBossMailData : MailData
@property (nonatomic,strong) WorldBossMailContents *detail;
@end
