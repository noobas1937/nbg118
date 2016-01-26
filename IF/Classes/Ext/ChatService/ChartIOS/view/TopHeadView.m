//
//  TopHeadView.m
//  IF
//
//  Created by 张彦涛 on 15/6/16.
//
//

#import "TopHeadView.h"
#import "ServiceInterface.h"
@interface TopHeadView ()
{
    UIView *topView;
    UIImageView *topBackImageView;
   UIImageView *_huitiaoImage;
}
@end

@implementation TopHeadView

-(instancetype)init{
    self = [super init];
    if (self) {
//        [self addTop];
    }
    return self;
}


+(instancetype)topHeadViewWithRightButtonShowFlag:(BOOL )vshowFlag andShowRightButtonType:(TopHeadViewRightButtonType)vButtonType{
   TopHeadView *headView =  [[[NSBundle mainBundle ]loadNibNamed:@"TopHeadView" owner:nil options:nil] lastObject];
    [headView settingRightButtonShow:vshowFlag andRightButtonType:vButtonType];
    return headView;
}

-(void)awakeFromNib{
   
    
    _titleNameLabel.font = [UIFont fontWithName:@"Helvetica-Bold" size:[ServiceInterface serviceInterfaceSharedManager].fontSize];
}

-(void)settingRightButtonShow:(BOOL)vShow andRightButtonType:(TopHeadViewRightButtonType)vButtonType{
    if (vShow) {
        if (vButtonType == TopHeadViewRightButtonType_ChatRoomMember) {
            _rightChatRoomMemberButton.hidden = NO;
            _rightEditButton.hidden = YES;
        }else{
            _rightChatRoomMemberButton.hidden = YES;
            _rightEditButton.hidden = NO;
        }
    }else{
        _rightChatRoomMemberButton.hidden = YES;
        _rightEditButton.hidden = YES;
    }
}

- (IBAction)buttonAction:(id)sender {
    
    if ( [self.topHeadViewDelegate respondsToSelector:@selector(clickButton)]) {
        [self.topHeadViewDelegate clickButton ];
    }
}
- (IBAction)rightButtonAction:(id)sender {
    if ( [self.topHeadViewDelegate respondsToSelector:@selector(rightButtonAction:)]) {
        [self.topHeadViewDelegate rightButtonAction:sender ];
    }
}


-(void)backButtonPressed:(UIButton *)sender{
    if ( [self.topHeadViewDelegate respondsToSelector:@selector(clickButton)]) {
         [self.topHeadViewDelegate clickButton ];
    }
   
   
}
 

@end
