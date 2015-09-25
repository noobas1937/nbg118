//
//  ZYTChatTableViewCell.m
//  IF
//
//  Created by 张彦涛 on 15/7/9.
//
//

#import "ZYTChatTableViewCell.h"
//#import "ChatLabel.h"
@interface ZYTChatTableViewCell ()
@property (strong,nonatomic) UIImageView * userFaceImageView;
@property (strong,nonatomic) UILabel * userNickNameLabel;
@property (strong,nonatomic) UIImageView * msgBubbleBackImageView;
@property (strong,nonatomic) UIView * msgBubbleView;

//@property (strong,nonatomic) ChatLabel *msgtextLabel;
@end

@implementation ZYTChatTableViewCell

+(instancetype)chatTableVIewCellWithTableView:(UITableView *)vTableView{
    static NSString *identify = @"ZYTChatTableViewCell";
    ZYTChatTableViewCell *cell = [vTableView dequeueReusableCellWithIdentifier:identify];
    if (cell == nil) {
        
        cell = [[ZYTChatTableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify];;
        cell.backgroundColor =[UIColor clearColor];
    }
    return cell;
}
- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.userFaceImageView = [[UIImageView alloc]initWithFrame:CGRectZero];
        self.userNickNameLabel = [[UILabel alloc]initWithFrame:CGRectZero];
        self.userNickNameLabel.font = [UIFont systemFontOfSize:13];
        self.msgBubbleBackImageView = [[UIImageView alloc]initWithFrame:CGRectZero];

    
    }
    return self;
}

-(void)setMsgItem:(NSMsgItem *)msgItem{
    _msgItem = msgItem;
    self.userNickNameLabel.text = @"zhagnsan";
    self.userNickNameLabel.height = 20;
    [self.userNickNameLabel sizeToFit];
    self.userFaceImageView.image =[UIImage imageNamed:msgItem.headPic];
    self.userFaceImageView.frame = CGRectMake(5, 5, 50, 50);
    self.userNickNameLabel.left = self.userFaceImageView.right+5;
    self.userNickNameLabel.top = self.userFaceImageView.top;
//    self.msgtextLabel.width = kMainScreenWidth *0.7- 60;
 
    
    
}


@end
