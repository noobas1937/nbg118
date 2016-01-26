//
//  CSShowTimeTool.h
//  IF
//
//  Created by 张彦涛 on 15/10/16.
//
//

#import <Foundation/Foundation.h>

@interface CSShowTimeTool : NSObject
/**
 *  写log进文件
 *  自动带上时间戳
 *  @param vString 要写入的文本
 */
+(void)saveLogToFile:(NSString *)vString;
@end
