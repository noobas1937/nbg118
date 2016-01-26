//
//  NSAttributedString+Emotion.m
//  LinkTest
//
//  Created by joywii on 14/12/9.
//  Copyright (c) 2014年 . All rights reserved.
//

#import "NSAttributedString+Emotion.h"

#import "ServiceInterface.h"
@implementation KZTextAttachment


- (CGRect)attachmentBoundsForTextContainer:(NSTextContainer *)textContainer proposedLineFragment:(CGRect)lineFrag glyphPosition:(CGPoint)position characterIndex:(NSUInteger)charIndex
{
    if ([self.imageNameString isEqualToString:@"[equip_share]"]) {
        CGFloat width = [ServiceInterface serviceInterfaceSharedManager].chatLabel_icon_size_width_1;
        CGFloat height = [ServiceInterface serviceInterfaceSharedManager].chatLabel_icon_size_height_1;
        return CGRectMake( 0 , -5, width, height);
    }else{
        CGFloat width = [ServiceInterface serviceInterfaceSharedManager].chatLabel_icon_size_width;
        CGFloat height = [ServiceInterface serviceInterfaceSharedManager].chatLabel_icon_size_height;
        return CGRectMake( 0 , -5, width, height);
    }
}
@end

@implementation NSAttributedString (Emotion)

//----------------------------------------------------------------------实例方法----------------------------------------------------------------------
/*
 * 返回绘制NSAttributedString所需要的区域
 */
- (CGRect)boundsWithSize:(CGSize)size
{
    CGRect contentRect = [self boundingRectWithSize:size options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading context:nil];//NSStringDrawingUsesLineFragmentOrigin
    return contentRect;
}

//----------------------------------------------------------------------静态方法----------------------------------------------------------------------
/*
 * 返回表情数组
 */
+ (NSArray *)emojiStringArray
{
    return [NSArray arrayWithObjects:@"[sysChat]",@"[battlereport]",@"[equip_share]",nil];
}

/*
 * 返回绘制文本需要的区域
 */
+ (CGRect)boundsForString:(NSString *)string size:(CGSize)size attributes:(NSDictionary *)attrs
{
    NSAttributedString *attributedString = [NSAttributedString emotionAttributedStringFrom:string attributes:attrs];
    CGRect contentRect = [attributedString boundingRectWithSize:size options: NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading context:nil];
    return contentRect;
}

/*
 * 返回Emotion替换过的 NSAttributedString
 */
+ (NSAttributedString *)emotionAttributedStringFrom:(NSString *)string attributes:(NSDictionary *)attrs
{
    NSMutableAttributedString *attributedEmotionString = [[NSMutableAttributedString alloc] initWithString:string attributes:attrs];
    
    NSArray *attachmentArray = [NSAttributedString attachmentsForAttributedString:attributedEmotionString];
    for (KZTextAttachment *attachment in attachmentArray)
    {
        NSAttributedString *emotionAttachmentString = [NSAttributedString attributedStringWithAttachment:attachment];
        [attributedEmotionString replaceCharactersInRange:attachment.range withAttributedString:emotionAttachmentString];
    }
    return attributedEmotionString;
}

/*
 * 查找所有表情文本并替换
 */
+ (NSArray *)attachmentsForAttributedString:(NSMutableAttributedString *)attributedString
{
    NSString *markL       = @"[";
    NSString *markR       = @"]";
    NSString *string      = attributedString.string;
    NSMutableArray *array = [NSMutableArray array];
    NSMutableArray *stack = [[NSMutableArray alloc] init];
    
    for (int i = 0; i < string.length; i++)
    {
        NSString *s = [string substringWithRange:NSMakeRange(i, 1)];
        
        if (([s isEqualToString:markL]) || ((stack.count > 0) && [stack[0] isEqualToString:markL]))
        {
            if (([s isEqualToString:markL]) && ((stack.count > 0) && [stack[0] isEqualToString:markL]))
            {
                [stack removeAllObjects];
            }
            
            [stack addObject:s];
            
            if ([s isEqualToString:markR] || (i == string.length - 1))
            {
                NSMutableString *emojiStr = [[NSMutableString alloc] init];
                for (NSString *c in stack)
                {
                    [emojiStr appendString:c];
                }
                //判断当前元素在不在数组中
                if ([[NSAttributedString emojiStringArray] containsObject:emojiStr])
                {
                    NSRange range = NSMakeRange(i + 1 - emojiStr.length, emojiStr.length);
                    
                    [attributedString replaceCharactersInRange:range withString:@" "];
                    KZTextAttachment *attachment = [[KZTextAttachment alloc] initWithData:nil ofType:nil];
                    attachment.range = NSMakeRange(i + 1 - emojiStr.length, 1);
                    attachment.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",emojiStr]];
                    attachment.imageNameString =emojiStr;
                    i -= ([stack count] - 1);
                    [array addObject:attachment];
                }
                [stack removeAllObjects];
            }
        }
    }
    return array;
}
@end
