//
//  ResourceMailData.h
//  IF
//
//  Created by 马纵驰 on 15/9/11.
//
//

#import <Foundation/Foundation.h>
#import "MailInfoDataModel.h"
#import "MailData.h"

@interface ResourceMailData : MailData
//泛型 ResourceMailContents
@property (nonatomic,strong) NSMutableArray *collect;

@property (nonatomic,assign) int unread;
@property (nonatomic,assign) int totalNum;
-(instancetype)initWithAddMailData:(MailInfoDataModel*) maildata;
-(instancetype)initWithAddMailData:(MailInfoDataModel*) maildata withAddCount:(int) count;
@end
