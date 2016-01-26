//
//  InCommonUsed.h
//  IF
//
//  Created by 张彦涛 on 15/6/19.
//
//

#ifndef IF_InCommonUsed_h
#define IF_InCommonUsed_h

#define kMainScreenHeight ([UIScreen mainScreen].bounds.size.height)
#define kMainScreenWidth ([UIScreen mainScreen].bounds.size.width)
 
//打印时间戳到日志文件
#define kTimeLog(contents) [CSShowTimeTool saveLogToFile:[NSString stringWithFormat:@"%@  \n  【函数名:】 %s 【行号:】%d 【文件路径:】%s  " ,contents,__FUNCTION__ ,__LINE__,__FILE__]];

#define DBLog_Key 1 //DBlog是否开启
/**
 ChatMailTableTableViewController
 */
typedef enum : NSUInteger {
    ChatMailTableVCType_ShowAddMemButton = 0,
    ChatMailTableVCType_UnShow = 1,
    
    
} ChatMailTableVCType;
#define kWebSocketLinkUrl @"http://184.173.82.76:8080/server/link"

#define kSearchUserWithNameString @"searchUserWithNameString"
#define kChatMsgPushFromServer @"chatMsgPushFromServer"
#define kChatMailMsgPushFromServer @"chatMailMsgPushFromServer"

#define kChatChannelUnReadCountChanged @"chatChannelUnReadCountChanged"
#define kMailChannelUnReadCountChanged @"mailChannelUnReadCountChanged"

#define kMailRewardBanchForClaim @"mailRewardBanchForClaim"

#define kMailChatCreatByOCNative @"mailChatCreatByOCNative"

#define kRefreshRedPackage @"RefreshRedPackage"
#define KCLOSEKEYBOARD @"CloseKeyBoard"
#define kDBVersion 2
#define kMsgPulledCount 20

#endif
