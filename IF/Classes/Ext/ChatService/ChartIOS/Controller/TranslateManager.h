//
//  TranslateManager.h
//  IF
//
//  Created by 马纵驰 on 15/10/9.
//
//

#import <Foundation/Foundation.h>

@interface TranslateManager : NSObject
+ (TranslateManager *)sharedTranslateManagerInstance;
// 禁用翻译包含语言
@property (nonatomic,strong) NSString *disableLang;
@end
