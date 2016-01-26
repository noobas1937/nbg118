//
//  TopHeadView.h
//  IF
//
//  Created by 张彦涛 on 15/6/16.
//
//

#import <UIKit/UIKit.h>


typedef enum : NSUInteger {
    TopHeadViewRightButtonType_ChatRoomMember = 0,
    TopHeadViewRightButtonType_ViewEdit = 1,//下拉刷新数据
 
}TopHeadViewRightButtonType;

@protocol TopHeadViewDelegate <NSObject>
@optional
- (void)clickButton ;
-(void)rightButtonAction:(UIButton *)sender;
@end

@interface TopHeadView : UIView
@property (weak, nonatomic) IBOutlet UILabel *titleNameLabel;

@property (weak, nonatomic) IBOutlet UIButton *rightEditButton;

@property (weak,nonatomic)IBOutlet UIButton *rightChatRoomMemberButton;


//@property (nonatomic,strong) UILabel *titleName;
@property (nonatomic,assign)id<TopHeadViewDelegate>topHeadViewDelegate;

+(instancetype)topHeadViewWithRightButtonShowFlag:(BOOL )vshowFlag andShowRightButtonType:(TopHeadViewRightButtonType)vButtonType;
@end
