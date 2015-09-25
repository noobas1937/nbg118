//
//  ZYTChatSystemCell.m
//  IF
//
//  Created by 张彦涛 on 15/6/15.
//
//

#import "ZYTChatSystemCell.h"
#import "UIViewExt.h"
#import "ChatCellFrame.h"
#import "NSString+Extension.h"
#import "ServiceInterface.h"
@interface ZYTChatSystemCell ()
@property (nonatomic,strong)UILabel * systemLabel;

@end
@implementation ZYTChatSystemCell

+ (instancetype)chatSystemCellWithTableView:(UITableView *)tableView
{
    static NSString *identify = @"ZYTChatSystemCell";
    ZYTChatSystemCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
    if (cell == nil) {
      
        cell = [[ZYTChatSystemCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify];;
        cell.backgroundColor =[UIColor clearColor];
    }
    return cell;
}
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        UILabel *systemLabel =[[UILabel alloc]init];
                systemLabel.text = @"text";
        systemLabel.center = self.center;
        systemLabel.numberOfLines= 0;
        systemLabel.frame  =CGRectZero;
        
        systemLabel.clipsToBounds = YES;
        systemLabel.layer.cornerRadius = 5;
        systemLabel.backgroundColor =[UIColor colorWithRed:146.0/255.0 green:126.0/255.0 blue:102.0/255.0 alpha:1.0];
        systemLabel.textAlignment = NSTextAlignmentCenter;
        systemLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].chatSystemSize];
        systemLabel.textColor = [UIColor whiteColor];
        [systemLabel sizeToFit];
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

 

@end
