//
//  CSAlertRedPackageView.h
//  IF
//
//  Created by 马纵驰 on 15/11/25.
//
//

#import <UIKit/UIKit.h>
#import "NSMsgItem.h"
@class CSMessageModel;
@class CSAlertRedPackageView;
@protocol CSAlertRedPackageViewDelegate <NSObject>
@optional
-(void) openRedPackageByMsg:(NSMsgItem*) msgItem;
-(void) lookRedPackageByMsg:(NSMsgItem*) msgItem;
-(void) popUpCocosRedPackageByMsg:(NSMsgItem*) msgItem;

-(void)redPackageLookedWithAttachMentID:(NSString *)vAttachMentID andWithMsgChannelType:(int)vType;
-(void)redPackageOpenedWithAttachmentID:(NSString *)vAttachMentIDString andWithPackageView:(CSAlertRedPackageView *)vPackageView;
-(void)redPackagePopupCocosWithMessageModel:(CSMessageModel *)vMessageModel;
@end

@interface CSAlertRedPackageView : UIView
-(instancetype)initWithFrame:(CGRect)frame AndWith:(NSMsgItem*) msgItem;
-(instancetype)initWithFrame:(CGRect)frame andWithMessageModel:(CSMessageModel *)vMessageModel;
@property(nonatomic,assign)id<CSAlertRedPackageViewDelegate> alertRedPackageViewDelegate;
@end
