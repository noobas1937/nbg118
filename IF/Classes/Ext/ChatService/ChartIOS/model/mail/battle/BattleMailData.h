//
//  BattleMailData.h
//  IF
//
//  Created by 马纵驰 on 15/9/16.
//
//

#import <Foundation/Foundation.h>
#import "BattleMailContents.h"
#import "Content.h"
#import "MailData.h"
@interface BattleMailData : MailData
@property (nonatomic,strong) BattleMailContents *detail;
@property (nonatomic,strong) Content *attualContent;
//泛型BattleMailContents
@property (nonatomic,strong) NSMutableArray *knight;
@property (nonatomic,assign) BOOL isKnightMail;
@property (nonatomic,assign) int unread;
@property (nonatomic,assign) int totalNum;
-(instancetype)initWithAddMailData:(MailInfoDataModel*) midm;
-(instancetype)initWithAddMailData:(MailInfoDataModel*) maildata withAddCount:(int) count;
@end
