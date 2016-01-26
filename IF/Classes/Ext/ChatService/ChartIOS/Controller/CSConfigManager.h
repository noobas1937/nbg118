//
//  CSConfigManager.h
//  IF
//
//  Created by 马纵驰 on 15/10/30.
//
//

#import <Foundation/Foundation.h>

@interface CSConfigManager : NSObject
+ (instancetype)csConfigManagerShared;
@property (nonatomic,strong) NSString *gameLang;
/**
 *  获取本地语言
 *
 *  @return 本地语言
 */
-(NSString*)gettingGameLang2IOS;
@end
;