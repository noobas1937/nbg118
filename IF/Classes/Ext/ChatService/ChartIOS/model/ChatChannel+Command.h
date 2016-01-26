//
//  ChatChannel+Command.h
//  IF
//
//  Created by 张彦涛 on 15/12/24.
//
//

#import "ChatChannel.h"

@interface ChatChannel (Command)

-(BOOL)gettingServerOldMsgForCountryWithMinSeq:(long)vMinSeq andWithMaxSeq:(long)vMaxSeq ;
@end
