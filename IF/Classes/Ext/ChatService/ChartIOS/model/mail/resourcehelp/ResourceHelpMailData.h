//
//  ResourceHelpMailData.h
//  IF
//
//  Created by 马纵驰 on 15/9/26.
//
//

#import <Foundation/Foundation.h>
#import "MailData.h"

@interface ResourceHelpMailData : MailData

@property (nonatomic,assign) int unread;
@property (nonatomic,assign) int totalNum;
/**
 *  泛型 ResourceHelpMailContents
 */
@property (nonatomic,strong) NSMutableArray  *collect;

@end
