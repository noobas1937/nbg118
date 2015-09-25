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

/**
 ChatMailTableTableViewController
 */
typedef enum : NSUInteger {
    ChatMailTableVCType_ShowAddMemButton = 0,
    ChatMailTableVCType_UnShow = 1,
    
    
} ChatMailTableVCType;


#define kSearchUserWithNameString @"searchUserWithNameString"
#define kChatMsgPushFromServer @"chatMsgPushFromServer"
#define kChatMailMsgPushFromServer @"chatMailMsgPushFromServer"
#endif
