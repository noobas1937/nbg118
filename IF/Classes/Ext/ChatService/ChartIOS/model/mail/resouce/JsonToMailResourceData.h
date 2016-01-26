//
//  JsonToMailResourceData.h
//  IF
//
//  Created by 马纵驰 on 15/9/11.
//
//

#import <Foundation/Foundation.h>
#import "MailInfoDataModel.h"
#import "MJExtension.h"

@interface JsonToMailResourceData : NSObject
@property (nonatomic,strong) NSString *createTime;
@property (nonatomic,strong) NSArray *reward;
@property (nonatomic,strong) NSArray *drop;
@property (nonatomic,strong) NSString *level;
@property (nonatomic,strong) NSString *pointId;
@end
