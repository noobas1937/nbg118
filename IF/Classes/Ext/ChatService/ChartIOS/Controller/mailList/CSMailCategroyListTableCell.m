//
//  CSMailCategroyListTableCell.m
//  IF
//
//  Created by 张彦涛 on 15/10/9.
//
//

#import "CSMailCategroyListTableCell.h"
#import "UIImage+resized.h"
#import "NSString+Cocos2dHelper.h"
#import "ServiceInterface.h"
@interface CSMailCategroyListTableCell ()
@property (weak, nonatomic) IBOutlet UIImageView *cellbackImageView;

@property (weak, nonatomic) IBOutlet UIImageView *headImageView;
@property (weak, nonatomic) IBOutlet UILabel *nameLabel;
@property (weak, nonatomic) IBOutlet UILabel *unreadCountLabel;
@property (retain, nonatomic) IBOutlet UIImageView *underLineImageView;


@property (retain, nonatomic) IBOutlet NSLayoutConstraint *headImageView_top;

@property (retain, nonatomic) IBOutlet NSLayoutConstraint *headImageView_left;

@property (retain, nonatomic) IBOutlet NSLayoutConstraint *headImageView_bottom;

@property (retain, nonatomic) IBOutlet NSLayoutConstraint *mailListArrow_right;

@property (retain, nonatomic) IBOutlet NSLayoutConstraint *unreadCountLabel_right;



@end


@implementation CSMailCategroyListTableCell

+ (instancetype)mailCategroyListTableCellWithTableView:(UITableView *)tableView
{
    
    static NSString *identify = @"CSMailCategroyListTableCell";
    CSMailCategroyListTableCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
    
    if (cell == nil) {
        
        
        // 从xib中加载cell
        cell = [[[NSBundle mainBundle] loadNibNamed:@"CSMailCategroyListTableCell" owner:nil options:nil] lastObject];
        cell.backgroundColor =[UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleDefault;
        UIImageView *backImageView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"mail_list_selected_bg"]];
        backImageView.frame = CGRectMake(0, 0, cell.width, cell.height);
        cell.selectedBackgroundView = backImageView;
        
        
        [cell initView];
        
    }else{
        for ( UIView *subView in cell.contentView.subviews) {
            [subView removeFromSuperview];
        }
    }
    return cell;
}
- (void)awakeFromNib {
    // Initialization code
    
    _cellbackImageView.image =[UIImage resizedImage:@"mail_list_bg"];
    _headImageView_top.constant = self.height * 0.159;
    _headImageView_left.constant = self.width * 0.025;
    _headImageView_bottom.constant= self.height * 0.09;
    _mailListArrow_right.constant = self.width * 0.034;
    _unreadCountLabel_right.constant = self.width * 0.034;
    
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//        _nameLabel.font = [UIFont systemFontOfSize:23];
//        _unreadCountLabel.font =[UIFont systemFontOfSize:23];
//    }else{
//          _nameLabel.font = [UIFont systemFontOfSize:13];
//        _unreadCountLabel.font =[UIFont systemFontOfSize:13];
//    }
    
    _nameLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_nameLabelSize];
    _unreadCountLabel.font = [UIFont systemFontOfSize:[ServiceInterface serviceInterfaceSharedManager].mail_native_contentsLabelSize];
    
   
}


-(void)setMailCategoryListModel:(CSMailCategoryListModel *)mailCategoryListModel{
    _mailCategoryListModel = mailCategoryListModel;
    
    _headImageView.image = [UIImage imageNamed:_mailCategoryListModel.headImageString];
    _nameLabel.text = _mailCategoryListModel.titleString;
    if (_mailCategoryListModel.unReadCount > 0) {
        _unreadCountLabel.text = [NSString stringWithFormat:@"%d",_mailCategoryListModel.unReadCount];
        _unreadCountLabel.hidden = NO;
    }else{
        _unreadCountLabel.hidden = YES;
    }
    if (_mailCategoryListModel.showIndexSequence == 6) {
        _underLineImageView.hidden = YES;
    }else{
         _underLineImageView.hidden = NO;
    }    
}

-(void)initView
{
    
//    self.showMainView.backgroundColor=[UIColor colorWithRed:(float)222/255.f green:(float)150/255.f blue:(float)62/255.f  alpha:1];
    UIWindow *window=[[[UIApplication sharedApplication] delegate] window];
    self.readButton=[UIButton buttonWithType:UIButtonTypeCustom];
    self.readButton.frame=CGRectMake(window.frame.size.width, 0, 80, self.showMainView.frame.size.height);
    self.readButton.backgroundColor=[UIColor grayColor] ;
    [self.readButton setTintColor:[UIColor blackColor]];
    NSString *buttonTitleString =[NSString stringWithMultilingualWithKey:@"132120"];//132120=标记已读
    self.readButton.titleLabel.font =[UIFont systemFontOfSize:12.0];
    self.readButton.titleLabel.lineBreakMode= NSLineBreakByWordWrapping;
    [self.readButton setTitle:buttonTitleString forState:UIControlStateNormal];
    [self.readButton addTarget:self action:@selector(click:) forControlEvents:UIControlEventTouchUpInside];
    self.readButton.tag=1;
    [self.contentView insertSubview:self.readButton atIndex:0];
    
//    self.myContentView=[[UIView alloc]initWithFrame:CGRectMake(0, 0, self.contentView.frame.size.width  , 50)];
//    self.myContentView.backgroundColor=[UIColor lightGrayColor];
    //[self addSubview:self.myContentView];
    
    UISwipeGestureRecognizer *swipeLeft=[[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeCell:)];
    swipeLeft.direction=UISwipeGestureRecognizerDirectionLeft;
    [self addGestureRecognizer:swipeLeft];
    
    UISwipeGestureRecognizer *swipeRight=[[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeCell:)];
    swipeRight.direction=UISwipeGestureRecognizerDirectionRight;
    [self addGestureRecognizer:swipeRight];
}
-(void)openCell
{
    if (self.delegate && [self.delegate respondsToSelector:@selector(RootCellopen:)]) {
        
        if ([self.delegate RootCellopen:self]) {
            [UIView animateWithDuration:0.15 animations:^{
                self.showMainView.frame=CGRectMake(-85, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                self.readButton.frame=CGRectMake(self.showMainView.frame.size.width-85, 0, 80, self.showMainView.frame.size.height);
            } completion:^(BOOL finished) {
                [UIView animateWithDuration:0.15 animations:^{
                    self.showMainView.frame=CGRectMake(-75, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                    self.readButton.frame=CGRectMake(self.showMainView.frame.size.width-75, 0, 80, self.showMainView.frame.size.height);
                } completion:^(BOOL finished) {
                    [UIView animateWithDuration:0.15 animations:^{
                        self.showMainView.frame=CGRectMake(-80, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                        self.readButton.frame=CGRectMake(self.showMainView.frame.size.width-80, 0, 80, self.showMainView.frame.size.height);
                    } completion:^(BOOL finished) {
                        
                    }];
                }];
            }];
        };
    }
}
-(void)closeCell
{
    if (self.delegate && [self.delegate respondsToSelector:@selector(RootCellclose:)]) {
        
        if ([self.delegate RootCellclose:self]) {
            [UIView animateWithDuration:0.2 animations:^{
                
                self.showMainView.frame=CGRectMake(0, 0, self.showMainView.frame.size.width,  self.showMainView.frame.size.height);
                self.readButton.frame=CGRectMake(self.showMainView.frame.size.width, 0, 80, self.showMainView.frame.size.height);
                
            } completion:^(BOOL finished) {
                
            }];
            
        }else{
            [UIView animateWithDuration:0.1 animations:^{
                self.showMainView.frame=CGRectMake(10, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                self.readButton.frame=CGRectMake(self.showMainView.frame.size.width+10, 0, 80, self.showMainView.frame.size.height);
            } completion:^(BOOL finished) {
                [UIView animateWithDuration:0.2 animations:^{
                    self.showMainView.frame=CGRectMake(-10, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                     self.readButton.frame=CGRectMake(self.showMainView.frame.size.width-10, 0, 80, self.showMainView.frame.size.height);
                } completion:^(BOOL finished) {
                    [UIView animateWithDuration:0.1 animations:^{
                        self.showMainView.frame=CGRectMake(0, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
                         self.readButton.frame=CGRectMake(self.showMainView.frame.size.width, 0, 80, self.showMainView.frame.size.height);
                    } completion:^(BOOL finished) {
                        
                    }];
                }];
            }];
        }
    }
    
}

-(void)closeUpCell:(CSMailCategroyListTableCell*)cell
{
    
    [UIView animateWithDuration:0.2 animations:^{
        cell.showMainView.frame=CGRectMake(0, 0, self.showMainView.frame.size.width, self.showMainView.frame.size.height);
         self.readButton.frame=CGRectMake(self.showMainView.frame.size.width, 0, 80, self.showMainView.frame.size.height);
    } completion:^(BOOL finished) {
        
    }];
}
-(void)swipeCell:(UISwipeGestureRecognizer *)sender
{
    if (sender.direction==UISwipeGestureRecognizerDirectionLeft) {
        
        [self openCell];
        
    }else if (sender.direction==UISwipeGestureRecognizerDirectionRight){
        
        [self closeCell];
    }
}
-(void)click:(UIButton*)button
{
    if (self.delegate && [self.delegate respondsToSelector:@selector(cellbuttonClick:andWithMailCategroyListCell:)]) {
        
        [self.delegate cellbuttonClick:button andWithMailCategroyListCell:self];
        ;
    }
}

@end
