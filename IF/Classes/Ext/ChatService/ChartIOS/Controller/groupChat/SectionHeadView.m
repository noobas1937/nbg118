//
//  SectionHeadView.m
//  CreatMoreChat
//
//  Created by 张彦涛 on 15/6/10.
//  Copyright (c) 2015年 elex-tech. All rights reserved.
//

#import "SectionHeadView.h"
#import "UserGroup.h"
#import "UIImage+resized.h"
#import "ServiceInterface.h"

@interface SectionHeadView ()
@property (nonatomic , strong) UILabel  * nameLabel;
@end
@implementation SectionHeadView


+(instancetype)sectionHeadViewWithTableView:(UITableView *)tableView{
    static NSString *headIdentifier = @"header";
    
    SectionHeadView *headView = [tableView dequeueReusableCellWithIdentifier:headIdentifier];
    if (headView == nil) {
        headView = [[SectionHeadView alloc] initWithReuseIdentifier:headIdentifier andTableView:tableView];
        headView.backgroundColor =[UIColor clearColor];
    }
    
    return headView;
}

- (id)initWithReuseIdentifier:(NSString *)reuseIdentifier andTableView:(UITableView *)tableView
{
    if (self = [super initWithReuseIdentifier:reuseIdentifier]) {
       
        UIView *headView =[[UIView alloc]initWithFrame:CGRectMake(0, 0, tableView.width, 30)];
         headView.backgroundColor =[UIColor clearColor];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            headView.height = 50;
        }
//        DVLog(@"headView.frame %@",NSStringFromCGRect(headView.frame));
        UIImageView *backImage   =[[UIImageView alloc]initWithFrame:headView.frame];
        backImage.image = [UIImage resizableImage:@"groupBack"];
        backImage.backgroundColor =[UIColor clearColor];
        UILabel *nameLabel =[[UILabel alloc]init];
        nameLabel.font =[UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].sectionFontSize];
        [nameLabel sizeToFit];
        nameLabel.backgroundColor = [UIColor clearColor];
        nameLabel.numberOfLines = 1;
        nameLabel.text = @"分组";
        CGSize textSize =[nameLabel.text sizeWithAttributes:@{NSFontAttributeName:nameLabel.font}];
        nameLabel.center = backImage.center;
        nameLabel.left = 10;
        nameLabel.frame = CGRectMake(10, backImage.height/2-textSize.height/2, 200, textSize.height);
        self.nameLabel = nameLabel;
        nameLabel.textColor = [UIColor colorWithRed:212/255.0 green:187/255.0 blue:163/255.0 alpha:0.9];
        [headView addSubview:backImage];
        [headView insertSubview:nameLabel aboveSubview:backImage];
        
        UITapGestureRecognizer *tap =[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(headBtnClick)];
       
        
        [headView addGestureRecognizer:tap];
        [self addSubview:headView];
    }
    return self;
}

-(void)setUserGroup:(UserGroup *)userGroup{
    _userGroup = userGroup;
    self.nameLabel.text = userGroup.groupNameString;
}
 
- (void)headBtnClick
{
    _userGroup.open = !_userGroup.isOpen;
    if ([_sectionHeadViewDelegate respondsToSelector:@selector(clickHeadView)]) {
        [_sectionHeadViewDelegate clickHeadView];
    }
}
@end
