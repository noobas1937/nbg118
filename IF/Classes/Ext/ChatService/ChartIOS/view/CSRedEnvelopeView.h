//
//  CSRedEnvelopeView.h
//  IF
//
//  Created by 马纵驰 on 15/11/19.
//
//

#import <UIKit/UIKit.h>
#import "NSMsgItem.h"
@protocol CSRedEnvelopeViewDelegate <NSObject>
@optional
-(void) createRedPackageViewByMsg:(NSMsgItem*) mi;
/**
 *  老版本国家联盟使用弹出红包代理 
 */
-(void) onClickRedPackageMsg;
@end

@interface CSRedEnvelopeView : UIView
-(instancetype)initWithFrame:(CGRect)frame addWithIsSelf:(NSMsgItem*) msgItem;
@property (nonatomic,strong) UIImageView *backgroundImage;
@property (nonatomic,strong) UIImage *upImage;
@property (nonatomic,strong) UIImage *downImage;
@property (nonatomic,strong) NSMsgItem *msgItem;
@property(nonatomic,assign)id<CSRedEnvelopeViewDelegate> redEnvelopeViewDelegate;
@end
