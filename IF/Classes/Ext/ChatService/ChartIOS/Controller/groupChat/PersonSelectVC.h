//
//  PersonSelectVC.h
//  IF
//
//  Created by 张彦涛 on 15/6/15.
//
//

#import <UIKit/UIKit.h>
@class ChatChannel;
typedef enum : NSUInteger {
    PersonSelectVCType_New = 0,
    PersonSelectVCType_ChangeMember = 1,
} PersonSelectVCType;



@interface PersonSelectVC : UIViewController

 
@property (nonatomic , strong) NSMutableArray * memberArr;
@property (nonatomic , strong) NSString * roomMemberName;
@property (nonatomic , strong) NSString * roomMemberUid;
/** 已经加入聊天室的联盟人员  */
@property (nonatomic,strong) NSArray *memberInGroupArr;
/** 查询页的显示数组  */
@property (strong,nonatomic) NSMutableArray * searchMemberArray;

@property (nonatomic,strong)ChatChannel *chatChannel;
@property (nonatomic,assign)PersonSelectVCType personSelectVCType;
-(void) commitCreateRoom;
-(instancetype)initWithType:(PersonSelectVCType )vType;
/**  设置当前VC的标题*/
-(void)settingVCTitleNameWithString:(NSString *)vNameString ;
@end
