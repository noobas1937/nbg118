//
//  CSGoogleTranslat.h
//  IF
//
//  Created by 张彦涛 on 15/7/2.
//
//

#import <Foundation/Foundation.h>
typedef enum : NSUInteger {
    ZYTGoogleTranslatHTTPStatus_sucess = 0,
    ZYTGoogleTranslatHTTPStatus_error = 1,
    
    
} ZYTGoogleTranslatHTTPStatus;

@interface CSGoogleTranslat : NSObject
typedef void(^GoogleTranslatBlock)(ZYTGoogleTranslatHTTPStatus status,NSString *resultString,NSString *orgLangTypeString) ;
-(void)translationingWithSL:(NSString *)vSL
                  andWithTL:(NSString *)vTL
                  andWithHL:(NSString *)vHL
                  andWithIE:(NSString *)vIE
                  andWithOE:(NSString *)vOE
                   andWithQ:(NSString *)vQ
             andResultBlock:(GoogleTranslatBlock)vBlock;
/**
 *  Google翻译接口
 *
 *  @param vText  要翻译的文字
 *  @param vSL    原语言国别类型
 *  @param vTL    目标语言国别类型
 *  @param vBlock 返回block
 */
-(void)translationingWithText:(NSString *)vText
                    andWihtSL:(NSString *)vSL
                    andWithTL:(NSString *)vTL
               andResultBlock:(GoogleTranslatBlock)vBlock;
@end
