//
//  UserGroup.h
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import <Foundation/Foundation.h>
@class NSUserInfo;
@interface UserGroup : NSObject
@property (nonatomic,strong)NSMutableArray *memberArray;//成员组
@property (nonatomic,assign,getter=isOpen)BOOL open;//折叠状态
@property (nonatomic,assign)NSInteger grade;//等级
@property (nonatomic,strong)NSString *groupNameString;


+(instancetype)userGroup;
@end
