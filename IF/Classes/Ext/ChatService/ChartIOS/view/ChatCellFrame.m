//
//  ChartCellFrame.m
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//
#define kIconMarginX 5
#define kIconMarginY 0

#import "ChatCellFrame.h"
#import "ChatViewController.h"
#import <Foundation/Foundation.h>
#import "ServiceInterface.h"
#import "MailViewController.h"
#import "UserManager.h"
#import "ChannelManager.h"
#import "NSString+Extension.h"

#import "CSGoogleTranslat.h"

#import "NSAttributedString+Emotion.h"

@interface ChatCellFrame()
@property (nonatomic,assign) CGSize contentSize;
@property (nonatomic,assign) CGFloat contentX;
@end

@implementation ChatCellFrame
//设置布局 RectMake
-(instancetype)init:(NSMsgItem *)chatMessage
{
    if ( self = [super init] )
    {
        self.isShowUiTopView_BOOL = FALSE;
        [self initWithData:chatMessage];
//        [self setChatMessage:chatMessage];
    }
    return self;
    
}

/**设置翻译信息*/
-(void) settingtrTranslation
{
    
    if (self.chatMessage.isSelfMsg ==YES){
        return;
    }
    if (self.chatMessage.translateMsg.length > 0 && self.chatMessage.translatedLang.length > 0) {
        return;
    }
    __block ChatCellFrame *weakSelf = self;
    
    CSGoogleTranslat *  translat =[[CSGoogleTranslat alloc]init];
    _chatMessage.translateMsg =  _chatMessage.msg;
    [translat translationingWithSL:@"auto" andWithTL:_chatMessage.localLanString andWithHL:@"en" andWithIE:@"UTF-8" andWithOE:@"UTF-8" andWithQ:_chatMessage.msg andResultBlock:^(ZYTGoogleTranslatHTTPStatus status,NSString *resultString,NSString *orgLangTypeString) {
        
        if (status == ZYTGoogleTranslatHTTPStatus_sucess) {
//            DVLog(@"翻译后的语言:%@",resultString);
//            DVLog(@"原语言出处 :%@",orgLangTypeString);
            if (resultString.length>0) {
                weakSelf.chatMessage.translateMsg = resultString;
                weakSelf.chatMessage.originalLang = orgLangTypeString;
                weakSelf.chatMessage.hasTranslated = YES;
                [weakSelf.chatMessage packedMsg];
                [weakSelf setChatMessage:weakSelf.chatMessage];
                
            }
        }else{
//            [weakSelf settingtrTranslation];
        }
        
    }];
    
}

/**
 *  检查当前msg对象是否有翻译
 */
-(void)chatCellFrameCheckTranslate{
    if (self.chatMessage.isSelfMsg ==NO){
        if (!(self.chatMessage.translateMsg.length > 0 && self.chatMessage.translatedLang.length > 0)) {
            self.chatMessage.translateMsg = @"";
            self.chatMessage.translatedLang = @"";
            [self settingtrTranslation];
        }
    }
}

-(void)setChatMessage:(NSMsgItem *)chatMessage{
    _chatMessage = chatMessage;

    [self settingCellFrame2Content];
}

-(void)initWithData:(NSMsgItem *)chatMessage{
    
    _chatMessage=chatMessage;
 
    
    [self settingCellFrame2Content];
}

-(void)chatCellFrameUpdateUserInfo:(NSMsgItem *)vMsgItem{
    _chatMessage = vMsgItem;
    [self settingCellUserInfoFrame];
}

/**设置cell内部的所有元素frame*/
-(void) settingCellFrame2Content
{
    
    [self settingCellHeadFrame];
    
    [self settingCellChatContentFrame];
    
    [self settingCellEndContentFrame];
    
    [self settingCellUserInfoFrame];
    
    [self settingCellChatViewFrame];
    
    [self settingCellTimeFrame];
    
    [self settingCellactivityRect];
    
    [self settingCellHAndSettingCellDownRect];
    
    [self isShowUiTopView];
}

/**
 *  内部调用 翻译或原文的时候需要设置显示时间的frame
 */
-(void) isShowUiTopView{
    if (self.isShowUiTopView_BOOL) {
        CGSize winSize=[UIScreen mainScreen].bounds.size;
        self.uiDownViewRect=CGRectMake(0,self.uiTopViewRect.size.height,winSize.width,self.cellHeight);
    }
}

-(void) isShowUiTopView:(NSMutableArray *)cellFrameArray
{
    
    int i = [cellFrameArray indexOfObject:self];
    CGSize winSize=[UIScreen mainScreen].bounds.size;
    if(i == 0){
        self.uiDownViewRect=CGRectMake(0,self.uiTopViewRect.size.height,winSize.width,self.cellHeight);
        self.isShowUiTopView_BOOL = TRUE;
    }else{
        ChatCellFrame *previous = [cellFrameArray objectAtIndex:(i - 1)];
        //当前cell的时间
        NSString *selfTime = [[self.chatMessage nsDataSwitchNSString:self.chatMessage.createTime] substringToIndex:10];
        //上一个cell的时间
        NSString *previousTime = [[previous.chatMessage nsDataSwitchNSString:previous.chatMessage.createTime] substringToIndex:10];
        if([selfTime isEqualToString:previousTime]){
            
            self.isShowUiTopView_BOOL = FALSE;
        }else{
            self.uiDownViewRect=CGRectMake(0,self.uiTopViewRect.size.height,winSize.width,self.cellHeight);
            self.isShowUiTopView_BOOL = TRUE;
        }
    }
}

CG_INLINE CGRect

CGRectMake1(CGFloat x, CGFloat y, CGFloat width, CGFloat height)

{
    
    ServiceInterface *serviceInterface = [ServiceInterface serviceInterfaceSharedManager];
    
    CGRect rect;
    
    rect.origin.x = x * serviceInterface.autoSizeScaleX; rect.origin.y = y * serviceInterface.autoSizeScaleY;
    
    rect.size.width = width * serviceInterface.autoSizeScaleWidth; rect.size.height = height * serviceInterface.autoSizeScaleHeight;
    
    return rect;
    
}

/**头像frame计算*/
-(void) settingCellHeadFrame
{
    
    CGFloat x = [self gettingX];
    
    CGSize winSize= [self gettingWinSize];
    CGFloat iconX=winSize.width * x;
    CGFloat iconY=kIconMarginY;
    CGFloat iconWidth=winSize.width * 0.14 * [ServiceInterface serviceInterfaceSharedManager].autoSizeScaleWidth;
    CGFloat iconHeight= iconWidth;
    
    
    if(self.chatMessage.isSelfMsg){
        iconX=winSize.width-winSize.width * x-iconWidth;
    }
    
    self.iconRect=CGRectMake(iconX, iconY, iconWidth, iconHeight);
    self.borderRect=CGRectMake(0, 0, self.iconRect.size.width, self.iconRect.size.height);
    self.headPicRect=CGRectMake(3, 3, self.iconRect.size.width - 8, self.iconRect.size.height - 7);
    self.gmodRect=CGRectMake(self.iconRect.size.width * 0.56, self.iconRect.size.height * 0.67, self.iconRect.size.width * 0.35, self.iconRect.size.height * 0.25);
    
}


/**文本frame计算*/
-(void) settingCellChatContentFrame
{
    
    CGSize winSize = [self gettingWinSize];
    
    //文本矩形
    if (_chatMessage.post == 100) {
        
        CGFloat fontFloat =[ServiceInterface serviceInterfaceSharedManager].chatSystemSize;
        CGSize tempSize =[_chatMessage.showMsg sizeWithFont:[UIFont systemFontOfSize:fontFloat] maxSize:CGSizeMake(kMainScreenWidth * 0.6, MAXFLOAT)];
        self.chatSystemSize =tempSize;
    }
    
    CGFloat fs = [ServiceInterface serviceInterfaceSharedManager].chatFontSize;
    
//    CGSize labelSize = [_chatMessage.showMsg textSizeWithFont:[UIFont systemFontOfSize:fs] constrainedToSize:CGSizeMake(kMainScreenWidth * 0.6, MAXFLOAT)];
    UIFont *font = [UIFont systemFontOfSize:fs];
    NSDictionary *attributes = @{NSFontAttributeName: font};
    NSAttributedString *attributedString = [NSAttributedString emotionAttributedStringFrom:_chatMessage.showMsg attributes:attributes];
    
    CGRect attributeRect = [attributedString boundsWithSize:CGSizeMake(kMainScreenWidth *0.6,MAXFLOAT)];
//    DVLog(@"文本是:%@",attributedString);
//    DVLog(@"文本的frame  %@",    NSStringFromCGRect(attributeRect));
    _contentSize = attributeRect.size;
    self.chatLabelSize = attributeRect.size;
}
/**聊天结尾的frame*/
-(void) settingCellEndContentFrame
{
    //聊天结尾矩形
    CGFloat endStrfs = [ServiceInterface serviceInterfaceSharedManager].endtimeFontSize;
    CGSize endStrSize = [_chatMessage.endMsg sizeWithFont:[UIFont systemFontOfSize:endStrfs] maxSize:CGSizeMake(kMainScreenWidth *0.6, MAXFLOAT)];
//    CGSize endStrSize = [self gettingTextFrame:endStrfs withAdd:_chatMessage.endMsg];
    endStrSize.width = endStrSize.width + kMainScreenWidth * 0.02;
    
    self.endStrLabelSize = endStrSize;
    
    if (_contentSize.width < endStrSize.width) {
        _contentSize.width = endStrSize.width;
//        self.chatLabelSize = CGSizeMake(_contentSize.width, _contentSize.height);
    }
    
}

-(void) settingCellUserInfoFrame
{
    //VIP  计算长度
    
    CGFloat userInfoFontSize = [ServiceInterface serviceInterfaceSharedManager].userInfofontSize;
    CGSize userInfoSize = [_chatMessage.showUserInfoLabel sizeWithFont:[UIFont systemFontOfSize:userInfoFontSize] maxSize:CGSizeMake(kMainScreenWidth * 0.6, MAXFLOAT)];
//    CGSize userInfoSize = [self gettingTextFrame:userInfoFontSize withAdd:_chatMessage.showUserInfoLabel];
    
    //vip_asn_name_Rect; vip + asn + name = 70
    
    if (_chatMessage.isSelfMsg) {
        CGFloat vip_asn_name_X;
        if ([_chatMessage gettingUserInfo].isDummy == TRUE) {
            vip_asn_name_X = kMainScreenWidth * 0.4;
        }else{
            vip_asn_name_X = kMainScreenWidth - self.iconRect.size.width - userInfoSize.width - kMainScreenWidth * 0.05;
        }
        
        self.vip_asn_name_Rect = CGRectMake(vip_asn_name_X, 0,userInfoSize.width ,userInfoSize.height);
    }else{
        self.vip_asn_name_Rect = CGRectMake(self.iconRect.size.width + kMainScreenWidth * 0.04, 0,userInfoSize.width, userInfoSize.height);
    }
}

/**设置cell中文本区域大小*/
-(void) settingCellChatViewFrame
{
    
    if (self.chatMessage.post == 12) {
        _contentX=CGRectGetMaxX(self.iconRect)+kIconMarginX;
        _contentSize =  CGSizeMake(kMainScreenWidth *0.6,kMainScreenHeight * 0.1);
        //chatView
        if(_chatMessage.isSelfMsg){
            CGFloat cx = [ServiceInterface serviceInterfaceSharedManager].contentX;
            _contentX=kMainScreenWidth-_contentSize.width-self.iconRect.size.width - kMainScreenWidth * cx;
        }else{
            _contentX = _contentX - 5;
        }
        
        self.redEnvelopeRect=CGRectMake(_contentX, kIconMarginY + self.vip_asn_name_Rect.size.height, _contentSize.width + 25, _contentSize.height + 10);
    }else{
        _contentX=CGRectGetMaxX(self.iconRect)+kIconMarginX;
        //chatView
        if(_chatMessage.isSelfMsg){
            CGFloat cx = [ServiceInterface serviceInterfaceSharedManager].contentX;
            _contentX=kMainScreenWidth-_contentSize.width-self.iconRect.size.width - kMainScreenWidth * cx;
        }else{
            _contentX = _contentX - 5;
        }
        
        self.chatViewRect=CGRectMake(_contentX, kIconMarginY + self.vip_asn_name_Rect.size.height, _contentSize.width + 25, _contentSize.height + 10);
    }

}

/**设置显示cell中时间的frame*/
-(void) settingCellTimeFrame
{
    NSString *time = [[_chatMessage nsDataSwitchNSString:_chatMessage.createTime ] substringToIndex:10];
    CGFloat timefontSize = [ServiceInterface serviceInterfaceSharedManager].timefontSize;
    CGSize topTimeStrSize =[time sizeWithFont:[UIFont systemFontOfSize:timefontSize] maxSize:CGSizeMake(kMainScreenWidth * 0.6, MAXFLOAT)];
//    CGSize topTimeStrSize = [self gettingTextFrame:timefontSize withAdd:time];
    
    self.timeLabelRect=CGRectMake(self.uiTopViewRect.size.width/2 - kMainScreenWidth * 0.23, self.uiTopViewRect.origin.y, topTimeStrSize.width,  topTimeStrSize.height);
    
    
    self.uiTopViewRect=CGRectMake(0, 0, kMainScreenWidth, [self gettingWinSize].height * 0.06);
    
}

/**设置风火轮frame*/
-(void) settingCellactivityRect
{
    if (_chatMessage.isSelfMsg) {
        self.activityIndicatorRect = CGRectMake( _contentX - kMainScreenWidth * 0.075, self.chatViewRect.size.height / 2 + (kMainScreenHeight *(11/kMainScreenHeight)), kMainScreenWidth * 0.04 ,kMainScreenWidth * 0.04);
        
        self.resetSendBtnRect = CGRectMake( _contentX - kMainScreenWidth * 0.075, self.chatViewRect.size.height / 2 + (kMainScreenHeight *(6/kMainScreenHeight)), kMainScreenWidth * 0.06 ,kMainScreenWidth * 0.06);
    }
    
}
/**设置cell 高度 和 cell 下半部view的frame*/
-(void) settingCellHAndSettingCellDownRect
{
    CGFloat cellH = 0;
    if (self.chatMessage.post == 12) {
        cellH = self.vip_asn_name_Rect.size.height + self.redEnvelopeRect.size.height;
    }else{
        cellH =self.chatLabelSize.height + self.vip_asn_name_Rect.size.height + self.endStrLabelSize.height;
    }
    self.cellHeight = MAX(self.iconRect.size.height, cellH) + [self gettingWinSize].height * 0.04;
    
    self.uiDownViewRect=CGRectMake(0,0,kMainScreenWidth,self.cellHeight);
}

/**获取不通分辨率下x坐标*/
-(CGFloat) gettingX
{
    return [ServiceInterface serviceInterfaceSharedManager].x;
}

-(CGSize) gettingWinSize
{
    return [UIScreen mainScreen].bounds.size;
}

-(CGSize) gettingTextFrame:(CGFloat) f withAdd:(NSString*)text
{
    NSDictionary *userInfoAttributes = @{NSFontAttributeName:[UIFont systemFontOfSize:f]};
    
    return [text boundingRectWithSize:CGSizeMake(kMainScreenWidth * 0.6, MAXFLOAT) options: NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading attributes:userInfoAttributes context:nil].size;
}


@end


