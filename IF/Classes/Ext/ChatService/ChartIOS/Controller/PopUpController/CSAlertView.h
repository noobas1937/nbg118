//
//  CSAlertView.h
//  IF
//
//  Created by 张彦涛 on 15/6/18.
//
//

#import <UIKit/UIKit.h>
@class CSAlertView;
typedef enum : NSUInteger {
    ZYTAlertViewTitleType_creat= 0,
    ZYTAlertViewTitleType_add = 1,
    ZYTAlertViewTitleType_sub = 2,
    ZYTAlertViewTitleType_reName=3,
    ZYTAlertViewTitleType_quit=4,
    ZYTAlertViewTitleType_shield= 5
    
} ZYTAlertViewTitleType;


typedef enum {
    BLOCK,                  //屏蔽玩家
    CONTENTOFTHEREPORT,     //举报内容
    BANSENDMSG,             
    NOGETREWARD,
    CHANGENAME
    
} ViewType;

@protocol ZYTAlertViewDelegate <NSObject>

@optional
- (void)clickEnterButtonWithZYTAlertView:(CSAlertView *)vAlertView;
-(void)cancalBtnAction:(CSAlertView *)vAlertView;
-(void) popUpChangeName;
@end

@interface CSAlertView : UIView

//@property (nonatomic, assign) TipType1 tipType;
@property (nonatomic, strong) IBOutlet UITextView *tipText;
@property (nonatomic,strong)id<ZYTAlertViewDelegate>alertViewDelegate;
@property (nonatomic,strong)NSString *nameString;
@property (nonatomic,assign)ZYTAlertViewTitleType titleType;
@property (nonatomic,assign) ViewType viewType;
-(void) showViewByType;
+(instancetype)alertViewWithTitleString:(NSString *)vTitleStr;

@end
