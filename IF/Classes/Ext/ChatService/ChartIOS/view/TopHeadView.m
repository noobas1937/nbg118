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
- (IBAction)buttonAction:(id)sender {
    
    if ( [self.topHeadViewDelegate respondsToSelector:@selector(clickButton)]) {
        [self.topHeadViewDelegate clickButton ];
    }
}

+(instancetype)topHeadView{
   return  [[[NSBundle mainBundle ]loadNibNamed:@"TopHeadView" owner:nil options:nil] lastObject];
}

-(void)awakeFromNib{
   
    
    _titleNameLabel.font = [UIFont fontWithName:@"Helvetica-Bold" size:[ServiceInterface serviceInterfaceSharedManager].fontSize];
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
