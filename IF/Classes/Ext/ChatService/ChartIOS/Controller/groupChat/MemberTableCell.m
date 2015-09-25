//
//  MemberTableCell.m
//  CreatMoreChat
//
//  Created by 张彦涛 on 15/6/10.
//  Copyright (c) 2015年 elex-tech. All rights reserved.
//

#import "MemberTableCell.h"
#import "NSUserInfo.h"
#import "ServiceInterface.h"
@interface MemberTableCell ()
@property (weak, nonatomic) IBOutlet UIImageView *faceImageVIew;
@property (weak, nonatomic) IBOutlet UILabel *nickLabel;
@property (weak, nonatomic) IBOutlet UIButton *chooseButton;

@end

@implementation MemberTableCell


+ (instancetype)cellWithTableView:(UITableView *)tableView
{
    static NSString *identify = @"MemberTableCell";
    MemberTableCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
    if (cell == nil) {
        // 从xib中加载cell
        cell = [[[NSBundle mainBundle] loadNibNamed:@"MemberTableCell" owner:nil options:nil] lastObject];
        cell.backgroundColor =[UIColor clearColor];
    }
    return cell;
}
-(void)setUserInfo:(NSUserInfo *)userInfo{
    _userInfo = userInfo;
    [_faceImageVIew setImage:[UIImage imageNamed:_userInfo.headPic]];
    _nickLabel.text = userInfo.userName;
    if (_userInfo.chooseState == ChooseState_normal) {
        _chooseButton.selected = NO;
        _chooseButton.enabled = YES;
    }else if (_userInfo.chooseState == ChooseState_pressed){
        _chooseButton.selected = YES;
        _chooseButton.enabled = YES;
    }else{
        _chooseButton.selected = NO;
        _chooseButton.enabled = NO;
    }
}


- (void)awakeFromNib {
    _nickLabel.font =[UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].sectionFontSize -1];
    _nickLabel.textColor =[UIColor colorWithRed:47/255.0 green:22/255.0 blue:0 alpha:1];
}

- (IBAction)selectedButtonAction:(id)sender {
    if ([_memberCellDelegate respondsToSelector:@selector(clickButtonActionWithCell:andSelectedButton:)]) {
        [_memberCellDelegate clickButtonActionWithCell:self andSelectedButton:sender];
    }
}


@end
