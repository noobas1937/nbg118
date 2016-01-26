//
//  TipPopUpController.h
//  IF
//
//  Created by mzc on 15/4/17.
//
//

#import <UIKit/UIKit.h>
#import "NSMsgItem.h"
#import "ChatCellIOS.h"
@class CSMessage;
@class TipPopUpController;

typedef enum{
    BLOCKTYPE,
    RESENDTYPE,
    RADIOTYPE,
    NOHORN,
    NOTICEPRICE,
    CREATEROOM,
    INVITATION
    
}TipType;

@protocol TipPopUpControllerDelegate<NSObject>
/**
 *  喇叭道具发送喇叭
 *
 *  @param mi 所需参数 
 */
-(void) onClickButtonSendRadio:(NSMsgItem*) mi;
-(void) onClickButtonSendRadioWithMessage:(CSMessage *)vMsg andWithTiPView:(TipPopUpController *)vTipPopView;

/**
 *  金币发送喇叭
 *
 *  @param mi 所需参数
 */
-(void) onClickButtonSendRadio2:(NSMsgItem*) mi;
-(void) onClickButtonSendRadio2WithMessage:(CSMessage *)vMsg andWithTiPView:(TipPopUpController *)vTipPopView;
/**
 *  重发喇叭
 *
 *  @param mi 所需参数
 */
-(void) resendRadioByMsg:(ChatCellIOS*) vCell;
@end

@interface TipPopUpController : UIViewController
@property (nonatomic, strong) ChatCellIOS *cell;
@property (nonatomic, strong) NSMsgItem *chatMessage;

@property (nonatomic,strong)CSMessage *message;
@property (nonatomic, strong) IBOutlet UITextView *tipText;
@property (nonatomic, assign) TipType tipType;
@property (nonatomic, strong) NSString *roomMemberName;
@property (nonatomic,assign) id<TipPopUpControllerDelegate>tipPopUpControllerDelegate;
 
@end
