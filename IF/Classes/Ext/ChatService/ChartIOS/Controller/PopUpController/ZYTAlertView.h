//
//  ZYTAlertView.h
//  IF
//
//  Created by 张彦涛 on 15/6/18.
//
//

#import <UIKit/UIKit.h>
@class ZYTAlertView;
typedef enum : NSUInteger {
    ZYTAlertViewTitleType_creat= 0,
    ZYTAlertViewTitleType_add = 1,
    ZYTAlertViewTitleType_sub = 2,
    ZYTAlertViewTitleType_reName=3,
    ZYTAlertViewTitleType_quit=4,
    
    
    ZYTAlertViewTitleType_shield= 5
    
} ZYTAlertViewTitleType;

@protocol ZYTAlertViewDelegate <NSObject>

@optional
- (void)clickEnterButtonWithZYTAlertView:(ZYTAlertView *)vAlertView;
-(void)cancalBtnAction:(ZYTAlertView *)vAlertView;
@end

@interface ZYTAlertView : UIView

//@property (nonatomic, assign) TipType1 tipType;
@property (nonatomic, strong) IBOutlet UITextView *tipText;
@property (nonatomic,strong)id<ZYTAlertViewDelegate>alertViewDelegate;
@property (nonatomic,strong)NSString *nameString;
@property (nonatomic,assign)ZYTAlertViewTitleType titleType;

+(instancetype)alertViewWithTitleString:(NSString *)vTitleStr;

@end
