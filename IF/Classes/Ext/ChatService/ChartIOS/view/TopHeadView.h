//
//  TopHeadView.h
//  IF
//
//  Created by 张彦涛 on 15/6/16.
//
//

#import <UIKit/UIKit.h>

@protocol TopHeadViewDelegate <NSObject>
@optional
- (void)clickButton ;
-(void)rightButtonAction:(UIButton *)sender;
@end

@interface TopHeadView : UIView
@property (weak, nonatomic) IBOutlet UILabel *titleNameLabel;

@property (weak, nonatomic) IBOutlet UIButton *rightButton;

//@property (nonatomic,strong) UILabel *titleName;
@property (nonatomic,assign)id<TopHeadViewDelegate>topHeadViewDelegate;

+(instancetype)topHeadView;
@end
