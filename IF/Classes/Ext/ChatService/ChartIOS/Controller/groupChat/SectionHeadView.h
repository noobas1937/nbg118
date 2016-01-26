//
//  SectionHeadView.h
//  CreatMoreChat
//
//  Created by 张彦涛 on 15/6/10.
//  Copyright (c) 2015年 elex-tech. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "UserGroup.h"
@class UserGroup;
@protocol SectionHeadViewDelegate <NSObject>

@optional
- (void)clickHeadView;

@end
@interface SectionHeadView : UITableViewHeaderFooterView

@property (nonatomic , strong)  UserGroup* userGroup;
@property (nonatomic , assign) id<SectionHeadViewDelegate>sectionHeadViewDelegate;

+(instancetype)sectionHeadViewWithTableView:(UITableView *)tableView ;
@end
