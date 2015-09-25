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
#import "PersonSelectVC.h"

typedef enum{
    BLOCKTYPE,
    RESENDTYPE,
    RADIOTYPE,
    NOHORN,
    NOTICEPRICE,
    CREATEROOM,
    INVITATION
    
}TipType;

@interface TipPopUpController : UIViewController
@property (nonatomic, strong) ChatCellIOS *cell;
@property (nonatomic, strong) NSMsgItem *chatMessage;
@property (nonatomic, strong) IBOutlet UITextView *tipText;
@property (nonatomic, assign) TipType tipType;
@property (nonatomic, strong) NSString *roomMemberName;
//@property (nonatomic, strong) PersonSelectVC *personSelectVC;
@end
