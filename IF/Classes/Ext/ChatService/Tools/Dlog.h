//
//  Dlog.h
//  
//
//  Created by Yi.Xia on 12-6-21.
//  Copyright (c) 2012年 . All rights reserved.
//  调试信息打印（Debug时打印信息，生成Release版本时屏蔽打印信息）

//Dlog displays when debug



#ifdef DEBUG
    #ifndef DLog
        #define DLog(fmt, ...) NSLog((@"***Dlog***:   " fmt), ##__VA_ARGS__);
        //#define DVLog(fmt, ...) NSLog((@"***DVLog***:   \n%s [Line %d]-输出:\n" fmt), __func__,__LINE__,##__VA_ARGS__)
        # define DVLog(fmt, ...) NSLog((@"\n *********************  DVLog  *********************\n[文件路径: ] %s\n" "[函数名:   ]  %s\n" "[行号:    ]  %d \n------------------------  输出  ------------------:\n" fmt), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
        #define DTLog(exception) NSLog(@"***DTLog***    :SW - try-reason:%@,",exception.reason);
    #endif
#else
    #ifndef DLog
        #define DLog(...)
        #define DVLog(fmt, ...)
        #define DTLog(exception)
    #endif
#endif

#define ALog(fmt, ...) NSLog((@"Alog:%s-" fmt), __PRETTY_FUNCTION__, ##__VA_ARGS__)

#ifdef DEBUG
#   define ULog(fmt, ...)  { UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Debug"  message:[NSString stringWithFormat:fmt, ##__VA_ARGS__]  delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil]; [alert show]; }
#   define UVLog(fmt, ...)  { UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithFormat:@"%s\n [Line %d] ", __PRETTY_FUNCTION__, __LINE__] message:[NSString stringWithFormat:fmt, ##__VA_ARGS__]  delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil]; [alert show]; }
#else
#   define ULog(...)
#   define UVLog(...)
#endif




