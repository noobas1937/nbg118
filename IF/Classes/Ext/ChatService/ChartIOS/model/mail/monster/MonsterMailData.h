//
//  CSMonsterMailData.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "MailData.h"

@interface MonsterMailData : MailData
@property (nonatomic,strong) NSString *unread;
@property (nonatomic,strong) NSString *totalNum;
//MonsterMailContents
@property (nonatomic,strong) NSMutableArray *monster;
-(instancetype)initWithAddMailData:(MailInfoDataModel*) maildata withAddCount:(int) count;
@end
