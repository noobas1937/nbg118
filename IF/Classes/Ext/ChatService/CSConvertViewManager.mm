//
//  CSConvertViewManager.m
//  IF
//
//  Created by 张彦涛 on 15/11/6.
//
//

#import "CSConvertViewManager.h"
#import "ServiceInterface.h"
#import "CSCommandManager.h"
#import "MailController.h"
#import "CCJSONConverter.h"
#import "MailData.h"
#import "GlobalData.h"
#import "PopupViewController.h"
#import "ChattingVC.h"
#import "MailInfoManager.h"
#import "ChatServiceCocos2dx.h"
@implementation CSConvertViewManager

-(instancetype)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

+ (CSConvertViewManager *)sharedMailInfoManager
{
    static CSConvertViewManager *sharedCSConvertViewManager = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedCSConvertViewManager = [[self alloc] init];
    });
    return sharedCSConvertViewManager;
}

-(void)refresh_RewardStatusWithMailID:(NSString *)vMailID andWithStatusFlag:(BOOL)vStatusFlag{
    if (YES ==  vStatusFlag) {
        //已领取
        if ([vMailID isEqual:self.currentmailInfoDataModel.mail_ID]) {
            self.currentmailInfoDataModel.rewardStatus = 1;
            [self.currentmailInfoDataModel.maildata changeMailRewardStatus];
            [self.currentmailInfoDataModel.maildata parseMode2Json];
            if  ([self.convertViewManagerDelegate isKindOfClass:[UIViewController class]]){
                if ([self.convertViewManagerDelegate respondsToSelector:@selector(serverRefeshDateSucceedWithManager:)]) {
                    [self.convertViewManagerDelegate serverRefeshDateSucceedWithManager:self];
                }
            }

        }
        
    }
}
-(void)refresh_ReadStatusWithMailID:(NSString *)vMailID andWithStatusFlag:(BOOL)vStatusFlag{
    if (YES ==  vStatusFlag) {
        //已读
        if ([vMailID isEqual:self.currentmailInfoDataModel.mail_ID]) {
            self.currentmailInfoDataModel.status = 1;
            if  ([self.convertViewManagerDelegate isKindOfClass:[UIViewController class]]){
                if ([self.convertViewManagerDelegate respondsToSelector:@selector(serverRefeshDateSucceedWithManager:)]) {
                    [self.convertViewManagerDelegate serverRefeshDateSucceedWithManager:self];
                }
            }

        }

    }
}

-(void)currentMailReadingNofityToServer{
    [[CSCommandManager sharedCommandManager]settingreadSatusWithMailID:self.currentmailInfoDataModel.mail_ID andWithMailTpye:self.currentmailInfoDataModel.type];
}

-(void)mailStarWithMailID:(NSString *)vMailID andWithStarStatus:(int )vStar{
    if ([self.currentmailInfoDataModel.mail_ID isEqualToString:vMailID]){
        self.currentmailInfoDataModel.saveFlag = vStar;
        if (self.convertViewManagerDelegate && [self.convertViewManagerDelegate isKindOfClass:[UIViewController class]] && [self.convertViewManagerDelegate respondsToSelector:@selector(settingStarFormServerCallBackWithManager:)]){
            [self.convertViewManagerDelegate settingStarFormServerCallBackWithManager:self];
        }
        self.currentmailInfoDataModel.maildata.save = [NSString stringWithFormat:@"%d",vStar];
        [self.currentmailInfoDataModel.maildata parseMode2Json];

    }
    
}

-(void)showCocos2DViewWithMailID:(MailInfoDataModel *)vMailData{
    if (self.currentmailInfoDataModel.maildata.jsonStr.length <= 0){
        return ;
    }
    CCDictionary *mailInfoDic = CCJSONConverter::sharedConverter()->dictionaryFrom([self.currentmailInfoDataModel.maildata.jsonStr UTF8String]);
    
    if(mailInfoDic)
    {
        int value = mailInfoDic->valueForKey("reply")->intValue();
        int share = 2;
        int reply = 1;
        int like = 4;
        int donate = 8;
        if((value & share) == share)
        {
            mailInfoDic->setObject(CCString::create("1") , "share");
        }
        else
        {
            mailInfoDic->setObject(CCString::create("0") , "share");
        }
        
        if((value & reply) == reply)
        {
            mailInfoDic->setObject(CCString::create("1") , "reply");
        }
        else
        {
            mailInfoDic->setObject(CCString::create("0") , "reply");
        }
        
        if((value & like) == like)
        {
            mailInfoDic->setObject(CCString::create("1") , "like");
        }
        else
        {
            mailInfoDic->setObject(CCString::create("0") , "like");
        }
        
        if((value & donate) == donate)
        {
            mailInfoDic->setObject(CCString::create("1") , "donate");
        }
        else
        {
            mailInfoDic->setObject(CCString::create("0") , "donate");
        }
        
        CCLOGFUNCF("contents: %s",mailInfoDic->valueForKey("contents")->getCString());
    
    
        MailController::getInstance()->addMailFromAndroidToList(mailInfoDic, true);
        auto search = GlobalData::shared()->mailList.find( [vMailData.mail_ID UTF8String]);
        bool isExistMail=(search != GlobalData::shared()->mailList.end());
        
        MailInfo* mailInfo=NULL;
        if (isExistMail) {
            mailInfo=dynamic_cast<MailInfo*>(search->second);
        }
       PopupBaseView *popUpView =  MailController::getInstance()->gettingShowPopUpViewWithiOS(mailInfo);
//        popUpView->nativeType = IOS1;  Simon
        PopupViewController::getInstance()->addPopupInView(popUpView,true,false,true);
        CCDirector::sharedDirector()->setVisitFlag(true);
    }
//    auto x =popUpView->getPositionX();
//    popUpView->setPositionX(kMainScreenWidth);
//    auto action = MoveBy::create(0.3, ccp(-kMainScreenWidth, 0));
//    popUpView->runAction(action);
    UIWindow *tempWindow=[ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
    
    UIView * currentView =[(UIViewController *)self.convertViewManagerDelegate view];
    [UIView animateWithDuration:0.3 animations:^{
//        popUpView->setPositionX(0);
        tempWindow.right = 0;
        
    } completion:^(BOOL finished) {
         [ServiceInterface serviceInterfaceSharedManager] .chatRootWindow.hidden = YES;
        tempWindow.right = kMainScreenWidth;
    }];
    

    

}

-(void)ocNativeFormCocos2dWithPopUpView:(PopupBaseView *)vPopupView isShowAgain:(BOOL)vShow;{
    UIWindow *tempWindow=[ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
    if (tempWindow.rootViewController){
        tempWindow.right = kMainScreenWidth/2;
        [ServiceInterface serviceInterfaceSharedManager] .chatRootWindow.hidden = NO;
        CCDirector::sharedDirector()->setVisitFlag(false);
        if  ([self.convertViewManagerDelegate isKindOfClass:[UIViewController class]]){
            if (self.convertViewManagerDelegate && [self.convertViewManagerDelegate respondsToSelector:@selector(cocosCallback_Oc_NativeView)]){
                [self.convertViewManagerDelegate cocosCallback_Oc_NativeView];
            }
        }
 
        
        [UIView animateWithDuration:0.3 animations:^{
            
            tempWindow.right = kMainScreenWidth;
          
            
        } completion:^(BOOL finished) {
            
//            if  (vPopupView ){
//                auto x =vPopupView->getPositionX();
//                vPopupView->setPositionX(x);
//                auto action = MoveBy::create(0.2, ccp(kMainScreenWidth, 0));
//                vPopupView->runAction(action);
 //           }
            
        }];

    }
}

-(void)deleteMailAtCocosViewWithMailID:(NSString *)vMailID{
    if (vMailID.length> 0){
        //数据库删除
        [MailInfoManager mailInfoDataFor_deleteWithMailID:vMailID];
        if ([vMailID isEqualToString:self.currentmailInfoDataModel.mail_ID]){
            if  ([self.convertViewManagerDelegate isKindOfClass:[UIViewController class]]){
                if (self.convertViewManagerDelegate && [self.convertViewManagerDelegate respondsToSelector:@selector(deleteMailFromCocosViewWithManager:)]){
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [self.convertViewManagerDelegate deleteMailFromCocosViewWithManager:self];
                    });
                    
                }
            }

        }
    }
}

#pragma mark -
#pragma mark window控制
-(void)chatWindowHidden{
     UIWindow *tempWindow=[ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
    if ([tempWindow.rootViewController isKindOfClass:[UINavigationController class]]){
        UIViewController *vc =[(UINavigationController *)tempWindow.rootViewController visibleViewController];
        [vc.view endEditing:YES ];
    }else if([tempWindow.rootViewController isKindOfClass:[ChattingVC class]]){
        [tempWindow.rootViewController.view endEditing:YES];
    }else if ([tempWindow.rootViewController isKindOfClass:[CSMailChattingVC class]]){
        [tempWindow.rootViewController.view endEditing:YES];

    }
    tempWindow.hidden =YES;
    CCDirector::sharedDirector()->setVisitFlag(true );
}
-(void)chatWindowShow{
    
    UIWindow *tempWindow=[ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
    if (tempWindow.rootViewController != nil){
        tempWindow.hidden =YES;
        CCDirector::sharedDirector()->setVisitFlag(false );
    }
}
-(void)closeMailVC{
    void (^block)() = ^{
        UIWindow *tempWindow=[ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
        if ([tempWindow.rootViewController isKindOfClass:[UINavigationController class]]){
            UIViewController *vc =[(UINavigationController *)tempWindow.rootViewController visibleViewController];
            if  ([vc isKindOfClass:[ChattingVC class]]){
                ChattingVC *chatVC = (ChattingVC *)vc;
                
                [chatVC deallocAny];
 
            }else if([vc isKindOfClass:[CSMailChattingVC class]]){
                CSMailChattingVC *chatVC = (CSMailChattingVC *)vc;
                [chatVC.currentChatChannel channelMarkToReadStatus];
                [chatVC deallocAnyWithMailChattingVC];
            }
            [vc.view endEditing:YES ];
        }else if([tempWindow.rootViewController isKindOfClass:[ChattingVC class]]){
            [tempWindow.rootViewController.view endEditing:YES];
        }else if ([tempWindow.rootViewController isKindOfClass:[CSMailChattingVC class]]){
            [tempWindow.rootViewController.view endEditing:YES];
            
        }
        
        tempWindow.rootViewController = nil;
        tempWindow.hidden = YES;
        //原生端关闭回掉
        ChatServiceCocos2dx::nativeUIClosed();
        CCDirector::sharedDirector()->setVisitFlag(true );

    };
    if ([NSThread isMainThread]) {
        block();
    } else {
        dispatch_async(dispatch_get_main_queue(), block);
        

    }

}
@end
