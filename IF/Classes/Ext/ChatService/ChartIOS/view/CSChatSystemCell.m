//
//  CSChatSystemCell.m
//  IF
//
//  Created by 张彦涛 on 15/6/15.
//
//

#import "CSChatSystemCell.h"
#import "UIViewExt.h"
#import "ChatCellFrame.h"
#import "NSString+Extension.h"
#import "ServiceInterface.h"
#import "CSMessageModel.h"
#import "Cellconfig.h"

@implementation InsetsSubLabel

@synthesize insets = _insets;

- (id)initWithFrame:(CGRect)frame andInsets:(UIEdgeInsets)insets {
    self = [super initWithFrame:frame];
    if(self) {
        self.insets = insets;
    }
    return self;
}

- (id)initWithInsets:(UIEdgeInsets)insets {
    self = [super init];
    if(self) {
        self.insets = insets;
    }
    return self;
}

- (void)drawTextInRect:(CGRect)rect {
    return [super drawTextInRect:UIEdgeInsetsInsetRect(rect, self.insets)];
}

@end

@interface CSChatSystemCell ()
@property (nonatomic,strong)InsetsSubLabel * systemLabel;

@end
@implementation CSChatSystemCell

+ (instancetype)chatSystemCellWithTableView:(UITableView *)tableView
{
    static NSString *identify = @"CSChatSystemCell";
    CSChatSystemCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
    if (cell == nil) {
      
        cell = [[CSChatSystemCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify];;
        cell.backgroundColor =[UIColor clearColor];
    }
    return cell;
}
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        InsetsSubLabel *systemLabel =[[InsetsSubLabel alloc]initWithInsets:UIEdgeInsetsMake(3, 5, 3, 5)];
                systemLabel.text = @"text";
        systemLabel.center = self.center;
        systemLabel.numberOfLines= 0;
        systemLabel.frame  =CGRectZero;
        
        systemLabel.clipsToBounds = YES;
        systemLabel.layer.cornerRadius = 5;
        systemLabel.backgroundColor =cellConfig.timeLabelbackColor;
        systemLabel.lineBreakMode = NSLineBreakByWordWrapping;
        systemLabel.textAlignment = NSTextAlignmentCenter;
        systemLabel.font = cellConfig.chatRoomSystemTextSize;
        systemLabel.textColor = cellConfig.timeLabelTextColor;
//        [systemLabel sizeToFit];
        [self.contentView addSubview:systemLabel ];
        self.systemLabel = systemLabel;
        
        self.selectionStyle = UITableViewCellSelectionStyleNone;
    }

    return self;
}

-(void)setChatCellFrame:(ChatCellFrame *)chatCellFrame{
    _chatCellFrame = chatCellFrame;
    self.systemLabel.text = chatCellFrame.chatMessage.showMsg;
//    CGSize stringSize=[self.systemLabel.text  sizeWithFont:self.systemLabel.font maxSize:CGSizeMake(kMainScreenWidth-20, MAXFLOAT)];
    self.systemLabel.frame =CGRectMake((kMainScreenWidth-chatCellFrame.chatSystemSize.width)/2,5, chatCellFrame.chatSystemSize.width, chatCellFrame.chatSystemSize.height);
    
    
//    self.systemLabel.frame =CGRectMake((self.width-stringSize.width)/2, self.height/2-stringSize.height/2, stringSize.width, stringSize.height);
//    
//    self.systemLabel.frame =[]
}
-(void)setMsgModel:(CSMessageModel *)msgModel{
    _msgModel = msgModel;
    self.systemLabel.text = msgModel.showContentsString;
    self.systemLabel.top = 10;
    self.systemLabel.width = msgModel.showContentsSize.width + 10;
     self.systemLabel.height = msgModel.showContentsSize.height + 6;
    self.systemLabel.left = (kMainScreenWidth - self.systemLabel.width)/2;
//    self.systemLabel.frame =CGRectMake((kMainScreenWidth-msgModel.showContentsSize.width)/2,cellConfig.avatarMargin, msgModel.showContentsSize.width+10,msgModel.showContentsSize.height+6);
}
 

@end
