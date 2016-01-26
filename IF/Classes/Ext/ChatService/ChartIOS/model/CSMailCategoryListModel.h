//
//  CSMailCategoryListModel.h
//  IF
//
//  Created by 张彦涛 on 15/10/9.
//
//

#import <Foundation/Foundation.h>
#import "ChatChannel.h"
@interface CSMailCategoryListModel : NSObject

@property (nonatomic,strong) ChatChannel *  categetyChannel;
@property (nonatomic,strong) NSString * headImageString;
@property (nonatomic,strong) NSString * titleString;
@property (nonatomic,assign) uint   unReadCount;
@property (nonatomic,assign) uint  allMailCount;
@property (nonatomic,strong) NSMutableArray *mailListArray;

@property (nonatomic,assign)int  showIndexSequence;
@end
