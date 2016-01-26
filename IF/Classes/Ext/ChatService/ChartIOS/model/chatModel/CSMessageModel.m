//
//  CSMessageModel.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CSMessageModel.h"
#import "CSMessage.h"
#import "NSString+Extension.h"
#import "YYKit.h"
#import "Cellconfig.h"
#import "UserManager.h"
#import "CSMessageModel+ModelManager.h"
#import "TranslateManager.h"
#import "ServiceInterface.h"
#import "NSString+Cocos2dHelper.h"
@interface CSMessageModel()
{
    YYLabel* label;
}
@end
@implementation CSMessageModel
- (instancetype)initWithMessage:(CSMessage *)message{
    self = [super init];
    if (self) {
        _cellHeight = -1;
        _message = message;
        
        if ([message.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
            _isSelfSender= YES;
        }else{
            _isSelfSender = NO;
        }
        
     
//        _message.msg = @"【YYY火爆收活人联盟公会入会公告】  不求土豪，但求活人！ 不求高战，但求兄弟！  😍你若不离不弃😍我便生🚲死相依😍 本公会都是😊来自天南地北💣五湖四海⭐的朋友😁希望大家能积极为个人而战👊 为联盟而战👊 为王国而战👊 为国家而战👊  既然大家能相聚在这个公会44：33即是缘分😍作为本公会的一份子⭐希望大家都积极的上线： 聊天扯淡✌ 做任务✌ 攻城略地✌ 抢钱✌ 抢人✌ 抢地盘✌😙 抢！抢！抢！！！！ 杀！杀！杀！！！！ 🚀另外警告一点！非我族类，其心必异！首先亁死歪果仁，其次才是亁死敢弄我们的人🔫";
 
        [self setterMessageKeys];
    }
   
    return self;
}

-(void)setterMessageKeys{
    
//    if (self.sequenceId %2 == 0) {
//        _isTimeLabelShow = YES;
//    }else{
//        _isTimeLabelShow = NO;
//    }
    if  (CSChannelType_User != (CSChannelType)self.message.channelType){
        _isNickNameShow = YES;
    }else{
        _isNickNameShow = NO;
    }
    if (!_isSelfSender) {
        if (self.messageBodyType == CSMessageBodyType_Text_Normal | self.messageBodyType == CSMessageBodyType_Text_Loudspeaker) {
            if (self.message.translateMsg.length > 0) {
                
                if ([self isTranlateDisable]) {
                    _isTranslatLabelShow = NO;
//                    _isTranslated = NO;
                }else{
                    
                    NSString *currentLang = [NSString currentLanguageType];
                    if ([currentLang isEqualToString:self.message.translatedLang]) {
                          _isTranslatLabelShow = YES;
                    }else{
                          _isTranslatLabelShow = NO;
                    }
                    
                  
//                    _isTranslated = YES;
                }
                
//                _isTranslated = YES;
            }else{
                _isTranslatLabelShow = NO;
//                _isTranslated = NO;
            }
        }
    }
 
 
}

-(BOOL) isTranlateDisable
{
    DLog(@"self.message.originalLang   :%@",self.message.originalLang);
    DLog(@" self.message.translatedLang  :%@",self.message.translatedLang);
    DLog(@"disableLang  :%@",[TranslateManager sharedTranslateManagerInstance].disableLang);
    if(self.message.originalLang.length > 0 && [TranslateManager sharedTranslateManagerInstance].disableLang.length > 0)
    {
        BOOL isContainsOriginLang=false;
        
        isContainsOriginLang= [self isContainsLangWithAdd:[TranslateManager sharedTranslateManagerInstance].disableLang withAdd:self.message.originalLang];
        
        if(isContainsOriginLang)
            return true;
    }
    return false;
}

-(BOOL) isContainsLangWithAdd:(NSString*) disableLang withAdd:(NSString*)lang
{
    BOOL ret = false;
    if(disableLang.length > 0 && self.message.originalLang.length > 0)
    {
        if(CSCONTAINS(disableLang, lang))
            ret = true;
        else
        {
            if(((CSCONTAINS(disableLang, @"zh-CN") || CSCONTAINS(disableLang, @"zh_CN")|| CSCONTAINS(disableLang, @"zh-Hans")) && [self isZh_CNWithByLang:lang])
               || ((CSCONTAINS(disableLang, @"zh-TW") || CSCONTAINS(disableLang, @"zh_TW") || CSCONTAINS(disableLang, @"zh-Hant")) && [self isZh_TWWithByLang:lang]))
                ret = true;
        }
    }
    return ret;
}

-(BOOL) isZh_CNWithByLang:(NSString*) lang
{
    if(lang.length > 0 && ([lang isEqualToString:@"zh-CN"] || [lang isEqualToString:@"zh_CN"] || [lang isEqualToString:@"zh-Hans"]))
        return true;
    return false;
}

-(BOOL) isZh_TWWithByLang:(NSString*)lang
{
    if(lang.length > 0 && ([lang isEqualToString:@"zh-TW"] || [lang isEqualToString:@"zh_TW"] || [lang isEqualToString:@"zh-Hant"]))
        return true;
    return false;
}




-(YYTextLayout*)returnText:(NSMutableAttributedString*)text
{
    text.font=cellConfig.contentsTextFont;

    
    
    //匹配坐标
    YYTextBorder *highlightBorder = [YYTextBorder new];
    highlightBorder.insets = UIEdgeInsetsMake(-2, 0, -2, 0);
    highlightBorder.cornerRadius = 3;
    highlightBorder.fillColor = [UIColor orangeColor];
    
    
    NSRegularExpression *regexCoordinate=
    [NSRegularExpression regularExpressionWithPattern:@"(1200|[1][0-1][0-9]{2}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])([:：])(1200|[1][0-1][0-9]{2}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])" options:kNilOptions error:NULL];
    NSArray *atResultsCoordinate = [regexCoordinate matchesInString:text.string options:0 range:text.rangeOfAll];
    for (NSTextCheckingResult *at in atResultsCoordinate) {
        if (at.range.location == NSNotFound && at.range.length <= 1) continue;
        if ([text attribute:YYTextHighlightAttributeName atIndex:at.range.location] == nil) {
            [text setColor:[UIColor blueColor] range:at.range];
            
            // 高亮状态
            YYTextHighlight *highlight = [YYTextHighlight new];
            [highlight setBackgroundBorder:highlightBorder];
            // 数据信息，用于稍后用户点击
            highlight.userInfo = @{kCSTouch_Point : [text.string substringWithRange:NSMakeRange(at.range.location, at.range.length )]};
            [text setTextHighlight:highlight range:at.range];
        }
    }
/*
    //匹配超链接
    NSRegularExpression *regexUrl=
    [NSRegularExpression regularExpressionWithPattern:@"((http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?)|(www.[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?)" options:kNilOptions error:NULL];
    NSArray *atResultsUrl = [regexUrl matchesInString:text.string options:0 range:text.rangeOfAll];
    for (NSTextCheckingResult *at in atResultsUrl) {
        if (at.range.location == NSNotFound && at.range.length <= 1) continue;
        if ([text attribute:YYTextHighlightAttributeName atIndex:at.range.location] == nil) {
            [text setColor:[UIColor blueColor] range:at.range];
            
            // 高亮状态
            YYTextHighlight *highlight = [YYTextHighlight new];
            [highlight setBackgroundBorder:highlightBorder];
            // 数据信息，用于稍后用户点击
            highlight.userInfo = @{kCSTouch_Link : [text.string substringWithRange:NSMakeRange(at.range.location, at.range.length )]};
            [text setTextHighlight:highlight range:at.range];
        }
    }

*/
    if (self.messageBodyType == CSMessageBodyType_Text_EquipShare  || self.messageBodyType == CSMessageBodyType_Text_AllianceTask) {
 
        NSRegularExpression *regexUrl=
        [NSRegularExpression regularExpressionWithPattern:self.changeTextString options:kNilOptions error:NULL];
        NSArray *atResultsUrl = [regexUrl matchesInString:text.string options:0 range:text.rangeOfAll];
        for (NSTextCheckingResult *at in atResultsUrl) {
            if (at.range.location == NSNotFound && at.range.length <= 1) continue;
            if ([text attribute:YYTextHighlightAttributeName atIndex:at.range.location] == nil) {
                [text setColor:self.changeTextColor range:at.range];
                
                // 高亮状态
                YYTextHighlight *highlight = [YYTextHighlight new];
                [highlight setBackgroundBorder:highlightBorder];
                // 数据信息，用于稍后用户点击
                if (self.messageBodyType ==CSMessageBodyType_Text_EquipShare) {
                    //装备分享
                     highlight.userInfo = @{kCSTouch_EquipName : [text.string substringWithRange:NSMakeRange(at.range.location, at.range.length )]};
                }else{
                    //联盟任务
                     highlight.userInfo = @{kCSTouch_AllianceTask : [text.string substringWithRange:NSMakeRange(at.range.location, at.range.length )]};
                }
                
                [text setTextHighlight:highlight range:at.range];
            }
        }

    }
    
    //匹配表情
    NSRegularExpression *regex=
    [NSRegularExpression regularExpressionWithPattern:@"\\[[^ \\[\\]]+?\\]" options:kNilOptions error:NULL];
    NSArray *emoticonResults = [regex matchesInString:text.string options:0 range:text.rangeOfAll];
    NSUInteger emoClipLength = 0;
    for (NSTextCheckingResult *emo in emoticonResults) {
        if (emo.range.location == NSNotFound && emo.range.length <= 1) continue;
        NSRange range = emo.range;
        range.location -= emoClipLength;
        if ([text attribute:YYTextHighlightAttributeName atIndex:range.location]) continue;
        if ([text attribute:YYTextAttachmentAttributeName atIndex:range.location]) continue;
        NSString *emoString = [text.string substringWithRange:range];
        NSAttributedString *emoText;
        UIImage *image;
        int h=0;
        if ([emoString isEqualToString:@"[sysChat]"]) {
            image = [UIImage imageNamed:@"[sysChat]"];
            if (!image) continue;
           //长方形图标 设置 h=12大概
            h= 14;
            emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:cellConfig.contentsTextSize  changN:h];


        }else if ([emoString isEqualToString:@"[equip_share]"]) {
            image = [UIImage imageNamed:@"[equip_share]"];
            if (!image) continue;
            
            emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:cellConfig.contentsTextSize  changN:h];
        }
        else if ([emoString isEqualToString:@"[battlereport]"]) {
            h= 14;
            image = [UIImage imageNamed:@"[battlereport]"];
            if (!image) continue;
            
           emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:cellConfig.contentsTextSize  changN:h];
        }
        if (image==nil)continue;
        
//        }else if ([emoString isEqualToString:@"xxx.gif"]){
//            YYImage *image = [YYImage imageNamed:@"xxx.gif"];
//            // image.preloadAllAnimatedImageFrames = YES;
//            if (!image) continue;
//            emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:cellConfig.contentsTextSize  changN:h];
//
//        }
        
        [text replaceCharactersInRange:range withAttributedString:emoText];
        emoClipLength += range.length - 1;
        
    }
    
    WBTextLinePositionModifier *modifier = [WBTextLinePositionModifier new];
    modifier.font = cellConfig.contentsTextFont;
    modifier.paddingTop = 0;
    modifier.paddingBottom = 0;
    
    YYTextContainer *container = [YYTextContainer new];
    //设置最大行数
    //container.maximumNumberOfRows=2;
    container.size = CGSizeMake(cellConfig.bubbleMaxWidth, HUGE);
    container.linePositionModifier = modifier;
    
    YYTextLayout * _textLayout = [YYTextLayout layoutWithContainer:container text:text];
    
    CGFloat _textHeight = [modifier heightForLineCount:_textLayout.rowCount];
    CGSize needSize;
    if (_textLayout.rowCount==1){
        // _textLayout.rowCount行数  行数为1 小于最大宽度  获取文本宽度 _textLayout. textBoundingSize.width
        needSize.width=_textLayout.textBoundingRect.size.width+1;
    }else{
        needSize.width=cellConfig.bubbleMaxWidth;
        
    }
    
    needSize.height = _textHeight;
    
    _showContentsSize=needSize;
    
   
    return _textLayout;

}
-(void)calculateWithTextBubbleView{
    
    if (self.messageBodyType == CSMessageBodyType_RedPackge) {
        CGFloat tempWidth = cellConfig.redPacketBubbleWith;
        CGFloat tempHeight = cellConfig.redPacketBubbleHeight;
        _textBubbleSize =  CGSizeMake(tempWidth, tempHeight);
        _textBubbleBackImageSize = CGSizeMake(tempWidth +cellConfig.redPacketLeftBubbleMargin.left + cellConfig.redPacketLeftBubbleMargin.right,tempHeight + cellConfig.redPacketLeftBubbleMargin.top+cellConfig.redPacketLeftBubbleMargin.bottom);

    }else if (self.messageBodyType == CSMessageBodyType_Text_ChatRoomSystemMsg){
        
        CGSize tempSize =[self.showContentsString sizeWithFont:cellConfig.chatRoomSystemTextSize maxSize:CGSizeMake(kMainScreenWidth * 0.6, MAXFLOAT)];
        _showContentsSize = tempSize;

    }else{
        NSMutableAttributedString *text = [NSMutableAttributedString new];
        [text appendAttributedString:[[NSMutableAttributedString alloc] initWithString:_showContentsString attributes:nil]];
        text.font=cellConfig.contentsTextFont;
        _newshowAttContentsString=[self returnText:text];
        
        
        _showTranslatExplainSize =[_translatExplainString sizeWithFont:cellConfig.translatExplainFont maxSize:CGSizeMake(cellConfig.bubbleMaxWidth, MAXFLOAT)];
        CGFloat tempWidth = 0;
        CGFloat tempHeight = 0;
        
        if (_isTranslatLabelShow == YES) {
            tempHeight = _showContentsSize.height + _showTranslatExplainSize.height ;
            if (_showContentsSize.width > _showTranslatExplainSize.width) {
                tempWidth = _showContentsSize.width;
            }else{
                tempWidth = _showTranslatExplainSize.width ;
            }
            
        }else{
            tempWidth = _showContentsSize.width;
            tempHeight = _showContentsSize.height  ;
        }
        _textBubbleSize =  CGSizeMake(tempWidth, tempHeight);
        _textBubbleBackImageSize = CGSizeMake(tempWidth +cellConfig.leftBubbleMargin.left + cellConfig.leftBubbleMargin.right,tempHeight + cellConfig.leftBubbleMargin.top+cellConfig.leftBubbleMargin.bottom);

    }
 
  }

#pragma mark -
#pragma mark getter
-(NSString *)messageId{
    return _message.mailId;
}
-(long)sequenceId{
    return _message.sequenceId;
}
-(CSMessageSendState)messageSendStatus{
 
    return  [self gettingMsgBodySendStateWithMsgSend:_message.sendState];
}
-(CSMessageBodyType)messageBodyType{
    return [self gettingMsgBodyTypeWithMsgPost:_message.post];
}

-(CGFloat)cellHeight{
    
     if (self.messageBodyType == CSMessageBodyType_Text_ChatRoomSystemMsg){
         //文字上下各内缩5像素
         return  _showContentsSize.height + cellConfig.avatarMargin +10 ;
    }else{
        CGFloat height = 0;
        if (_isNickNameShow == YES) {
            height = height + cellConfig.avatarMargin;//头像上边距
            height = height + cellConfig.messageNameHeight;
            height = height + _textBubbleBackImageSize.height;
//            height = height + cellConfig.avatarMargin;//气泡下边距
            
        }else{
            height = height + cellConfig.avatarMargin;
            height = height + _textBubbleBackImageSize.height;
//            height = height + cellConfig.avatarMargin;
        }
        
        
        
        if (_isTimeLabelShow == YES) {
            height = height + cellConfig.timeLabelHeight;
            if (height < cellConfig.avatarMargin+ cellConfig.avatarSize + cellConfig.timeLabelHeight) {
                return cellConfig.avatarMargin+ cellConfig.avatarSize + cellConfig.timeLabelHeight;
            }else{
                return height;
            }
        }else{
            if (height <cellConfig.avatarMargin+ cellConfig.avatarSize ) {
                return cellConfig.avatarMargin+ cellConfig.avatarSize ;
            }else{
                return height;
            }
        }

    }
    
 
   
}

-(CSMessageSendState)gettingMsgBodySendStateWithMsgSend:(int)vSendState{
    if (vSendState == 0) {
        return CSMessageSendState_Delivering;
    }else if (vSendState == 1){
        return CSMessageSendState_Failure;
    }else if (vSendState == 2){
        return CSMessageSendState_Delivered;
    }else{
        return CSMessageSendState_Pending;
    }
}

-(CSMessageBodyType )gettingMsgBodyTypeWithMsgPost:(int )vPost{
    if (vPost == 100) {
        return CSMessageBodyType_Text_ChatRoomSystemMsg;
    }else if (vPost == 200){
        return CSMessageBodyType_Text_ModMsg;
    }
    else if (vPost > 12){
        return CSMessageBodyType_NotCanParse;
    }
    else if ( vPost == 0){
        return CSMessageBodyType_Text_Normal;
    }
    else if  (vPost == 1){
        return CSMessageBodyType_Text_AllianceCreated;
    }
    else if  (vPost == 2){
        return CSMessageBodyType_Text_AllianceAdded;
    } else if  (vPost == 3){
        return CSMessageBodyType_Text_AllianceInvite;
    }else if (vPost == 4){
        return CSMessageBodyType_Text_FightReport;
    }else if (vPost == 5){
        return CSMessageBodyType_Text_InvestigateReport;
    }else if ( vPost == 6){
        return CSMessageBodyType_Text_Loudspeaker;
    }else if (vPost == 7){
        return CSMessageBodyType_Text_EquipShare;
    }else if (vPost == 8){
        return CSMessageBodyType_Text_SayHello;
    }else if (vPost == 9){
        return CSMessageBodyType_Text_AllianceConciles;
    }else if (vPost == 10){
        return CSMessageBodyType_Text_TurntableShare;
    }else if (vPost == 11){
        return CSMessageBodyType_Text_AllianceTask;
    }else if (vPost == 12){
        return CSMessageBodyType_RedPackge;
    }else {
        return CSMessageBodyType_Text_Normal;
    }
}


#pragma mark -
#pragma mark action
-(void)updateMessageModelWithMsg{
    [self settingContestsString];
    [self settingTransliteString];
    [self calculateWithTextBubbleView];
}
-(void)updateMessageModelWithUser{
    [self settingUserInfo];
}

@end

@implementation WBTextLinePositionModifier

- (instancetype)init {
    self = [super init];
    
    if (kiOS9Later) {
        _lineHeightMultiple = 1.24;   // for PingFang SC
    } else {
        _lineHeightMultiple = 1.2125; // for Heiti SC
    }
    
    return self;
}

- (void)modifyLines:(NSArray *)lines fromText:(NSAttributedString *)text inContainer:(YYTextContainer *)container {
    //CGFloat ascent = _font.ascender;
    CGFloat ascent = _font.pointSize * 1;//0.86
    
    CGFloat lineHeight = _font.pointSize * _lineHeightMultiple;
    for (YYTextLine *line in lines) {
        CGPoint position = line.position;
        position.y = _paddingTop + ascent + line.row  * lineHeight;
        line.position = position;
    }
}

- (id)copyWithZone:(NSZone *)zone {
    WBTextLinePositionModifier *one = [self.class new];
    one->_font = _font;
    one->_paddingTop = _paddingTop;
    one->_paddingBottom = _paddingBottom;
    one->_lineHeightMultiple = _lineHeightMultiple;
    return one;
}

- (CGFloat)heightForLineCount:(NSUInteger)lineCount {
    if (lineCount == 0) return 0;
    //    CGFloat ascent = _font.ascender;
    //    CGFloat descent = -_font.descender;
    CGFloat ascent = _font.pointSize * 0.86;
    CGFloat descent = _font.pointSize * 0.21;
    CGFloat lineHeight = _font.pointSize * _lineHeightMultiple;
    return _paddingTop + _paddingBottom + ascent + descent + (lineCount - 1) * lineHeight+4;
}

@end

