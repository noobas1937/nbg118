//
//  ChannelView.h
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import <Foundation/Foundation.h>
#import "ChatChannel.h"

@interface ChannelView : NSObject
@property (nonatomic,assign) BOOL     isLoadingStart;           //是否开始加载 下拉做判断 一旦开始下拉刷新，就不允许在次下拉
//public ListView					messagesListView;       //相当于tabelview
//public MessagesAdapter			messagesAdapter;        //数据存储的结构
@property (nonatomic,assign)    BOOL                    isFirstVisit;
@property (nonatomic,assign)    int                     tab;
@property (nonatomic,assign)    int                     channelType;
@property (nonatomic,strong)    ChatChannel             *chatChannel;

@end
