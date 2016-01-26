//
//  CSConvertViewManager.h
//  IF
//
//  Created by 张彦涛 on 15/11/6.
//
//

#import <Foundation/Foundation.h>
#import "MailInfoDataModel.h"
#import "PopupBaseView.h"
@class CSConvertViewManager;
@protocol CSConvertViewManagerDelegate <NSObject>
@optional
-(void)serverRefeshDateSucceedWithManager:(CSConvertViewManager *)vManager;
-(void)settingStarFormServerCallBackWithManager:(CSConvertViewManager *)vManager;
-(void)deleteMailFromCocosViewWithManager:(CSConvertViewManager *)vManager;
-(void)cocosCallback_Oc_NativeView;
@end
@interface CSConvertViewManager : NSObject


@property (nonatomic,strong) NSIndexPath * currentIndexPath;
@property (nonatomic,strong) MailInfoDataModel * currentmailInfoDataModel;
@property (nonatomic,assign)id<CSConvertViewManagerDelegate>convertViewManagerDelegate;

+ (CSConvertViewManager *)sharedMailInfoManager;
-(void)refresh_RewardStatusWithMailID:(NSString *)vMailID andWithStatusFlag:(BOOL)vStatusFlag;
-(void)refresh_ReadStatusWithMailID:(NSString *)vMailID andWithStatusFlag:(BOOL)vStatusFlag;


-(void)currentMailReadingNofityToServer;
-(void)showCocos2DViewWithMailID:(MailInfoDataModel *)vMailData;
/**邮件标星*/
-(void)mailStarWithMailID:(NSString *)vMailID andWithStarStatus:(int )vStar;

-(void)ocNativeFormCocos2dWithPopUpView:(PopupBaseView *)vPopupView isShowAgain:(BOOL)vShow;
/** C++页面删除邮件回调 */
-(void)deleteMailAtCocosViewWithMailID:(NSString *)vMailID;
-(void)closeMailVC;
-(void)chatWindowShow;
-(void)chatWindowHidden;
@end
