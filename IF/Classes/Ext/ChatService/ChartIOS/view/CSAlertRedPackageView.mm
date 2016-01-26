//
//  CSAlertRedPackageView.m
//  IF
//
//  Created by 马纵驰 on 15/11/25.
//
//

#import "CSAlertRedPackageView.h"
#import "UIImageView+EMWebCache.h"
#import "InCommonUsed.h"
#import "NSString+Cocos2dHelper.h"
#import "ServiceInterface.h"
#include "math.h"

#import "CSMessage.h"
#import "CSMessageModel.h"
#import "NSUserInfo.h"
#import "ChatServiceCocos2dx.h"
#import "UIImageView+YYWebImage.h"
#import "NSString+Cocos2dHelper.h"
#import "ChannelManager.h"
#import "ChatChannel.h"
#import "ChatChannel+ChatMessage.h"
#import "CSMessage+Manager.h"
#import "ChatServiceController.h"
@interface CSAlertRedPackageView()
@property (nonatomic,strong) NSMsgItem *msgItem;

@property (nonatomic,strong)CSMessageModel *messageModel;
@property (nonatomic,strong) UIImageView *backgroundImage;
@property (nonatomic,strong) UIImageView *headViewBackground;
@property (nonatomic,strong) UIImageView *headView;
@property (nonatomic,strong) UILabel *playerName;
@property (nonatomic,strong) UILabel *sendRedPackageLabel;
@property (nonatomic,strong) UILabel *tryLuckLabel;
@property (nonatomic,strong) UILabel *quickRobberyLabel;
@property (nonatomic,strong) UILabel *robberyGoneLabel;
@property (nonatomic,strong) UIButton *openRedPackageButton;
@property (nonatomic,strong) UIButton *deleteSelf;
@end

@implementation CSAlertRedPackageView

-(instancetype)initWithFrame:(CGRect)frame AndWith:(NSMsgItem*) msgItem{
    self  = [super initWithFrame:frame];
    if (self){
        [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(refreshRedPackage:) name:kRefreshRedPackage object:nil];
        self.msgItem = msgItem;
        [self addDeleteSelf];
        [self addBackgroundImage];
        [self addHeadView];
        [self addPlayerNameLabel];
        [self addSendRedPackageLabel];
        [self addTryLuckLabel];
        [self addRobberyGoneLabel];
        [self addOpenRedPackageButton];
        int status = [self.msgItem gettingRedPackageStatus];
        if (status == 1) {
            [self haveRedPackageView];
        }else{
            [self notHaveRedPackageView];
        }
    }
    return self;
}

-(instancetype)initWithFrame:(CGRect)frame andWithMessageModel:(CSMessageModel *)vMessageModel{
    self  = [super initWithFrame:frame];
    if (self){
        [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(refreshRedPackage:) name:kRefreshRedPackage object:nil];
        self.messageModel = vMessageModel;
        [self addDeleteSelf];
        [self addBackgroundImage];
        [self addHeadView];
        [self addPlayerNameLabel];
        [self addSendRedPackageLabel];
        [self addTryLuckLabel];
        [self addRobberyGoneLabel];
        [self addOpenRedPackageButton];
        int status = [self.messageModel.message gettingRedPackageStatusWithMessage];
        if (status == 1) {
            [self haveRedPackageView];
        }else{
            [self notHaveRedPackageView];
        }
    }
    return self;

}

-(void)addHeadViewBackground{
    float x = self.width * 0.5;
    float y = 0;
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad){
        y = self.height / 4.2;
    }else{
        y = self.height / 3.7;
    }
    
    float width = self.frame.size.width * 0.15;
    float hight = width;
    self.headViewBackground = [[UIImageView alloc] init];
    self.headViewBackground.image = [UIImage imageNamed:@"icon_kuang.png"];
    self.headViewBackground.center = CGPointMake(x,y);
    self.headViewBackground.bounds = CGRectMake(0.0f, 0.0f, width, hight);
    [self addSubview:self.headViewBackground];
}

-(void) addHeadView{
    [self addHeadViewBackground];
    float x = 3;
    float y = 3;
    float width = self.headViewBackground.width - 8;
    float hight = self.headViewBackground.height - 7;
    self.headView = [[UIImageView alloc] init];
    
    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New) {
        NSString * urlString =@"";
        if (self.messageModel.currentUser.headPicVer > 0){
            urlString =[NSString gettingCustomHeadPic:self.messageModel.currentUser.uid intValue:self.messageModel.currentUser.headPicVer];
        }
        [self.headView setImageWithURL:[NSURL URLWithString:urlString] placeholder:[UIImage imageNamed:self.messageModel.currentUser.headPic]];
    }else{
         [self.headView sd_setImageWithURL:[NSURL URLWithString:self.msgItem.customHeadPicUrl] placeholderImage:[UIImage imageNamed:self.msgItem.headPic]];
    }
    
   
    self.headView.frame = CGRectMake(x, y, width, hight);
    [self.headViewBackground addSubview:self.headView];
}

-(void) addPlayerNameLabel{
    float x = 0;
    float y = self.headViewBackground.bottom;
    float width = self.frame.size.width;
    float hight = self.frame.size.height * 0.02;
    self.playerName = [[UILabel alloc] initWithFrame:CGRectMake(x, y, width, hight)];
    
    if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        self.playerName.text = self.messageModel.currentUser.userName;
    }else{
        self.playerName.text = self.msgItem.name;
    }
    
    self.playerName.textAlignment = NSTextAlignmentCenter;
    float fontSize = [ServiceInterface serviceInterfaceSharedManager].redEnvelopeFont;
    self.playerName.font = [UIFont systemFontOfSize:fontSize];
    self.playerName.textColor = [UIColor whiteColor];
    [self addSubview:self.playerName];
}

-(void) addSendRedPackageLabel{
    float x = 0;
    float y = self.playerName.bottom + self.height * 0.01;
    float width = self.width;
    float hight = self.height * 0.02;
    self.sendRedPackageLabel = [[UILabel alloc] initWithFrame:CGRectMake(x, y, width, hight)];
    // 104983 == 红包 104998 == 发了一个{0}
    NSString *redPackageStr = [NSString stringWithMultilingualWithKey:@"104983"];
    NSString *sendRedPackageStr = [NSString stringWithMultilingualWithKey:@"104998" andWithPaddingString1:redPackageStr];
    self.sendRedPackageLabel.text = sendRedPackageStr;
    self.sendRedPackageLabel.textAlignment = NSTextAlignmentCenter;
    float fontSize = [ServiceInterface serviceInterfaceSharedManager].redEnvelopeFont;
    self.sendRedPackageLabel.font = [UIFont systemFontOfSize:fontSize];
    self.sendRedPackageLabel.textColor = [UIColor whiteColor];
    [self addSubview:self.sendRedPackageLabel];
    [self bringSubviewToFront:self.sendRedPackageLabel];
}

-(void) addTryLuckLabel{
    float x = 0;
    float y = self.height * 0.7;
    float width = self.width;
    float hight = self.height * 0.1;
    self.tryLuckLabel = [[UILabel alloc] initWithFrame:CGRectMake(x, y, width, hight)];
    //看看大家手气
    NSString *tryLuckStr = [NSString stringWithMultilingualWithKey:@"104975"];
    
    NSMutableAttributedString *content = [[NSMutableAttributedString alloc]initWithString:[NSString stringWithFormat:tryLuckStr]];
    NSRange contentRange = {0,[content length]};
    [content addAttribute:NSUnderlineStyleAttributeName value:[NSNumber numberWithInteger:NSUnderlineStyleSingle] range:contentRange];
    self.tryLuckLabel.attributedText = content;
    self.tryLuckLabel.textAlignment = NSTextAlignmentCenter;
    float fontSize = [ServiceInterface serviceInterfaceSharedManager].redEnvelopeFont;
    self.tryLuckLabel.font = [UIFont systemFontOfSize:fontSize];
    self.tryLuckLabel.textColor = [UIColor yellowColor];
    self.tryLuckLabel.userInteractionEnabled = YES;
    UITapGestureRecognizer *labelTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(lookRedPackage:)];
    [self.tryLuckLabel addGestureRecognizer:labelTapGestureRecognizer];
    [self addSubview:self.tryLuckLabel];
}

-(void) addQuickRobberyLabel{
    float x = 0;
    float y = self.sendRedPackageLabel.bottom;
    float width = self.frame.size.width;
    float hight = self.frame.size.height * 0.1;
    self.quickRobberyLabel = [[UILabel alloc] initWithFrame:CGRectMake(x, y, width, hight)];
    //大家快来抢
    self.quickRobberyLabel.text = [NSString stringWithMultilingualWithKey:@"104999"];
    self.quickRobberyLabel.textAlignment = NSTextAlignmentCenter;
    self.quickRobberyLabel.font = [UIFont systemFontOfSize:14.0];
    self.quickRobberyLabel.textColor = [UIColor whiteColor];
    [self addSubview:self.quickRobberyLabel];
}

-(void) addRobberyGoneLabel{
    float x = self.width / 2;
    float y = self.height / 1.7;
    float width = self.width * 0.6;
    float hight = self.frame.size.height * 0.1;
    self.robberyGoneLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, width, hight)];
    
    self.robberyGoneLabel.center = CGPointMake(x,y);
    self.robberyGoneLabel.bounds = CGRectMake(0, 0, width, hight);
    
    if ([self.msgItem gettingRedPackageStatus] == 2) {
        //好可惜，您來晚了，紅包已經被搶光了
        self.robberyGoneLabel.text = [NSString stringWithMultilingualWithKey:@"104978"];
    }else if([self.msgItem gettingRedPackageStatus] == 3){
        //红包
        NSString *redPackageStr = [NSString stringWithMultilingualWithKey:@"104983"];
        //160256 == 该{0}已过期
        self.robberyGoneLabel.text = [NSString stringWithMultilingualWithKey:@"160256" andWithPaddingString1:redPackageStr];
    }else if([self.msgItem gettingRedPackageStatus] == 0){
        self.robberyGoneLabel.text = [NSString stringWithMultilingualWithKey:@"104978"];
    }
    self.robberyGoneLabel.textAlignment = NSTextAlignmentCenter;
    self.robberyGoneLabel.font = [UIFont systemFontOfSize:14.0];
    self.robberyGoneLabel.textColor = [UIColor whiteColor];
    self.robberyGoneLabel.numberOfLines = 3;
    [self addSubview:self.robberyGoneLabel];
}

-(void) addOpenRedPackageButton{
    
    float x = self.width / 2;
    float y = self.height / 2;
    float width = self.width * 0.25;
    float hight = width * 0.8;
    
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad){
//        width = self.backgroundImage.frame.size.width * 0.15;
//        hight = width * 0.6;
//    }else{
//        width = self.backgroundImage.frame.size.width * 0.2;
//        hight = width * 0.8;
//    }
    
    self.openRedPackageButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    self.openRedPackageButton.center = CGPointMake(x,y);
    self.openRedPackageButton.bounds = CGRectMake(0.0f, 0.0f, width, hight);
    [self.openRedPackageButton setBackgroundImage:[UIImage imageNamed:@"redPackage4"] forState:UIControlStateNormal];
    [self.openRedPackageButton setBackgroundImage:[UIImage imageNamed:@"redPackage4"] forState:UIControlStateHighlighted];
    [self.openRedPackageButton addTarget:self action:@selector(openRedPackageButton:) forControlEvents:UIControlEventTouchUpInside];
    self.openRedPackageButton.backgroundColor = [UIColor clearColor];
    [self addSubview:self.openRedPackageButton];
    
    if ([self.msgItem gettingRedPackageStatus] == 1) {
        self.openRedPackageButton.hidden = NO;
    }else{
        self.openRedPackageButton.hidden = YES;
    }
    
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"transform"];
    animation.delegate = self;
    animation.toValue = [NSValue valueWithCATransform3D:CATransform3DMakeRotation(radians(30.0), 0, 0, 1.0)];
    //执行时间
    animation.duration = 0.1;
    animation.cumulative = NO;//累积的
    //执行次数
    animation.repeatCount = HUGE_VALF;
    animation.autoreverses=YES;//是否自动重复
    [self.openRedPackageButton.layer addAnimation:animation forKey:@"animation"];
    
    
}

-(void)openRedPackageButton:(id)sender {
    
    if  (self.alertRedPackageViewDelegate &&[self.alertRedPackageViewDelegate respondsToSelector:@selector(redPackageOpenedWithAttachmentID:andWithPackageView:)]){
         DVLog(@"redPackageOpenedWithAttachmentID:andWithPackageView:");
        if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
            [self.alertRedPackageViewDelegate redPackageOpenedWithAttachmentID:self.messageModel.message.attachmentId andWithPackageView:self];
        }else{
            [self.alertRedPackageViewDelegate redPackageOpenedWithAttachmentID:self.msgItem.attachmentId andWithPackageView:self];
        }

    }
    
//    if (self.alertRedPackageViewDelegate &&[self.alertRedPackageViewDelegate respondsToSelector:@selector(openRedPackageByMsg:)]){
//        [self.alertRedPackageViewDelegate openRedPackageByMsg:self.msgItem];
//    }
 
}

-(void) lookRedPackage:(UITapGestureRecognizer *)recognizer{
    
    
//    if ([self.alertRedPackageViewDelegate respondsToSelector:@selector(lookRedPackageByMsg:)]) {
//        DVLog(@"alertRedPackageViewDelegate::robberyGoneLabelTouch");
//        [self.alertRedPackageViewDelegate lookRedPackageByMsg:self.msgItem];
//    }
    
    if (self.alertRedPackageViewDelegate && [self.alertRedPackageViewDelegate respondsToSelector:@selector(redPackageLookedWithAttachMentID:andWithMsgChannelType:)]) {
        if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
 
            [self.alertRedPackageViewDelegate redPackageLookedWithAttachMentID:self.messageModel.message.attachmentId andWithMsgChannelType:(int )self.messageModel.message.channelType];
        }else{
            [self.alertRedPackageViewDelegate redPackageLookedWithAttachMentID:self.msgItem.attachmentId andWithMsgChannelType:(int )self.msgItem.channelType];
        }
        
    }
}

-(void)refreshRedPackage:(NSNotification*)vNotify{
    DVLog(@"当前对象 ：%@",self);
    DVLog(@"接收通知");
    NSDictionary *userInfo =vNotify.userInfo;
    NSString *attachmentId = [userInfo objectForKey:@"attachmentId"];
    NSString *status = [userInfo objectForKey:@"status"];
    
    NSArray *arr =nil;
    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        arr = [self.messageModel.message.attachmentId componentsSeparatedByString:@"|"];
        self.messageModel.message.attachmentId = [NSString stringWithFormat:@"%@|%@",arr[0],status];
        if (self.messageModel.message.channelType == CSChannelType_Country){
            ChatChannel *coutryChannel = [[ChannelManager sharedChannelManager]gettingCountryChannel];
            [coutryChannel redPacketMsgUpdateStautsWithMessage:self.messageModel.message];
        }else{
            ChatChannel *allianceChannel = [[ChannelManager sharedChannelManager]gettingAllianceChannel];
            [allianceChannel redPacketMsgUpdateStautsWithMessage:self.messageModel.message];
        }
        
    }else{
        arr = [self.msgItem.attachmentId componentsSeparatedByString:@"|"];
        self.msgItem.attachmentId = [NSString stringWithFormat:@"%@|%@",arr[0],status];
        [self.msgItem redPackageStatusSave2DB];
    }
    
   
    
    
    if ([status isEqualToString:@"0"]) {
        self.robberyGoneLabel.text = [NSString stringWithMultilingualWithKey:@"104978"];
        [self notHaveRedPackageView];
    }else if([status isEqualToString:@"1"]){
        if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
            if  (self.alertRedPackageViewDelegate && [self.alertRedPackageViewDelegate respondsToSelector:@selector(redPackagePopupCocosWithMessageModel:)]){
                [self.alertRedPackageViewDelegate redPackagePopupCocosWithMessageModel:self.messageModel];
            }
        }else{
            if ([self.alertRedPackageViewDelegate respondsToSelector:@selector(popUpCocosRedPackageByMsg:)]) {
                DVLog(@"alertRedPackageViewDelegate::robberyGoneLabelTouch");

                [self.alertRedPackageViewDelegate popUpCocosRedPackageByMsg:self.msgItem];
            }

        }
   }else if([status isEqualToString:@"2"]){
        self.robberyGoneLabel.text = [NSString stringWithMultilingualWithKey:@"104978"];
        [self notHaveRedPackageView];
    }else if([status isEqualToString:@"3"]){
        NSString *redPackageStr = [NSString stringWithMultilingualWithKey:@"104983"];
        //160256 == 该{0}已过期
        self.robberyGoneLabel.text =[NSString stringWithMultilingualWithKey:@"160256" andWithPaddingString1:redPackageStr];
        [self notHaveRedPackageView];
    }
}

-(void) haveRedPackageView{
    self.openRedPackageButton.hidden = NO;
    self.sendRedPackageLabel.hidden = NO;
    self.robberyGoneLabel.hidden = YES;
    
}

-(void) notHaveRedPackageView{
    self.openRedPackageButton.hidden = YES;
    self.sendRedPackageLabel.hidden = YES;
    self.robberyGoneLabel.hidden = NO;
}

-(void)addBackgroundImage{
    //1
    float x = 0;
    float y = 0;
    float width = self.width ;
    float hight = self.height ;
    self.backgroundImage = [[UIImageView alloc] initWithFrame:CGRectMake(0,0,width,hight)];
    self.backgroundImage.contentMode = UIViewContentModeScaleAspectFit;
    self.backgroundImage.image = [UIImage imageNamed:@"cs_red_package_bg"];
    self.backgroundImage.backgroundColor =[UIColor clearColor];
    [self addSubview:self.backgroundImage];
    
}

-(void)addDeleteSelf{
    float x = 0.0;
    float y = 0.0;
    float width= kMainScreenWidth;
    float hight = kMainScreenHeight;
    self.deleteSelf = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    
    self.deleteSelf = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    self.deleteSelf.frame = CGRectMake(x, y, width, hight);
    self.deleteSelf.backgroundColor = [UIColor clearColor];
    [self.deleteSelf addTarget:self action:@selector(removeView:) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:self.deleteSelf];
}

-(void)removeView:(id)sender {
    [UIView animateWithDuration:0.5
                     animations:^{
                         self.transform = CGAffineTransformMakeScale(0.01, 0.01);
                         
                     }completion:^(BOOL finish){
                         [self removeFromSuperview];
                     }];
    
}

double radians(float degrees) {
    return ( degrees * M_PI    ) / 180;
}

@end
