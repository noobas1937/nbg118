//
//  ZYTMailChattingVC.m
//  IF
//
//  Created by 张彦涛 on 15/8/31.
//
//

#import "ZYTMailChattingVC.h"
#import "TopHeadView.h"
#import "ChatChannel.h"
#import "ChatCellIOS.h"
#import "ZYTChatSystemCell.h"
#import "InputView.h"
@interface ZYTMailChattingVC ()<TopHeadViewDelegate,UITableViewDataSource,UITableViewDelegate,InputViewDelegate>
@property (strong, nonatomic)  UIView *headView;
@property (strong, nonatomic)  UIView *footView;
@property (strong,nonatomic) TopHeadView *topHeadView;
@property (strong,nonatomic) InputView  *inputView;
@property (nonatomic,strong) UITableView * mailChatTableView;

@property (nonatomic,strong) NSMutableArray * dataSourceArray;
@end

@implementation ZYTMailChattingVC


- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:self.headView];
    [self.view addSubview:self.footView];
    [self.footView addSubview:self.inputView];
    [self.headView addSubview:self.topHeadView];
    [self.view addSubview:self.mailChatTableView];
    [self setTitleWithString:self.chatChannel.nameString];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillChangeFrame:) name:UIKeyboardWillChangeFrameNotification object:nil];
}

-(void)dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillChangeFrameNotification object:nil];
}

#pragma mark -
#pragma mark getter

-(UIView *)headView{
    if (_headView == nil) {
        UIView *headView =[[UIView alloc]init];
        headView.backgroundColor = [UIColor grayColor];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            headView.frame = CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight * 0.07);
        } else{
            headView.frame = CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight * 0.07);
        }
        _headView = headView;
    }
    return _headView;
}
-(UIView *)footView{
    if (_footView == nil) {
        UIView *footView =[[UIView alloc]init];
        footView.backgroundColor = [UIColor grayColor];
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            footView.frame = CGRectMake(0, kMainScreenHeight- kMainScreenHeight * 0.1 , kMainScreenWidth, kMainScreenHeight * 0.1);
        } else{
            footView.frame = CGRectMake(0, kMainScreenHeight-kMainScreenHeight * 0.1  , kMainScreenWidth, kMainScreenHeight * 0.1);
        }
        _footView = footView;
    }
    return _footView;
}

-(TopHeadView *)topHeadView{
    if (_topHeadView == nil) {
        TopHeadView *topView = [TopHeadView topHeadView];
        topView.frame = self.headView.frame;
        topView.rightButton.hidden = NO;
        topView.topHeadViewDelegate = self;
        _topHeadView = topView;
    }
    return  _topHeadView;
}
-(InputView *)inputView{
    if (_inputView == nil) {
        InputView *inputView = [InputView inputViewWithFrame:CGRectMake(0, 0, self.footView.width, self.footView.height)];
        inputView.inputViewDelegate  = self;
        _inputView = inputView;
    }
    return _inputView;
}
-(UITableView *)mailChatTableView{
    if (_mailChatTableView == nil) {
        UITableView *tableView = [[UITableView alloc]initWithFrame:CGRectMake( 0, self.headView.bottom, kMainScreenWidth, kMainScreenHeight - self.headView.height-self.footView.height) style:UITableViewStylePlain];
        tableView.delegate = self;
        tableView.dataSource = self;
        tableView.backgroundView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"chat_bg_default.jpg"]];
        tableView.separatorStyle=UITableViewCellSeparatorStyleNone;
        
        UITapGestureRecognizer *tap =[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(keyBoardClosed)];
        [tableView addGestureRecognizer:tap];
        _mailChatTableView = tableView;
    }
    return _mailChatTableView;
}
-(NSMutableArray *)dataSourceArray{
    if (_dataSourceArray == nil) {
        NSMutableArray *array = [NSMutableArray array ];
        _dataSourceArray = array;
    }
    return  _dataSourceArray;
}
#pragma mark -
#pragma mark set
-(void)setTitleWithString:(NSString *)titleString{
    self.topHeadView.titleNameLabel.text = titleString;
}
-(void)setChatChannel:(ChatChannel *)chatChannel{
    _chatChannel = chatChannel;
    self.dataSourceArray = [chatChannel.msgList mutableCopy];
}
-(void)keyBoardClosed{
    [self.inputView.inputTextField resignFirstResponder];
}
#pragma mark -
#pragma mark tableView 
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.dataSourceArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *identify = @"mailIdentify";
    ChatCellFrame * cellFrame =[self.dataSourceArray objectAtIndex:indexPath.row];
    
    DVLog(@"%@",[cellFrame.chatMessage printAllPropertys]);
    if (cellFrame.chatMessage.post == 100) {
        
        ZYTChatSystemCell *cell =[ZYTChatSystemCell chatSystemCellWithTableView:tableView];
        [cell setChatCellFrame:cellFrame];
        
        cell.backgroundColor=[UIColor clearColor];
        
        return cell;
        
    }else{
        
        ChatCellIOS *cell = [tableView dequeueReusableCellWithIdentifier:identify];
        cell.selectionStyle = UITableViewCellSeparatorStyleNone;
        if (cell == nil) {
            cell = [[ChatCellIOS alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify];
        }
        cell.backgroundColor=[UIColor clearColor];
        
        [cell setCellFrame:cellFrame];
        return cell;
   
    }

}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    ChatCellFrame *cellFrame = self.dataSourceArray[indexPath.row];

    CGFloat totalHeight = 0;
    
    if (cellFrame.isShowUiTopView_BOOL) {
        totalHeight = cellFrame.cellHeight + cellFrame.uiTopViewRect.size.height;
    }else{
        totalHeight = cellFrame.cellHeight;
    }
    
    return totalHeight;
}


#pragma mark -
#pragma mark topHeadView  delegate
-(void)clickButton{
    [self.navigationController popViewControllerAnimated:YES];
}

-(void)rightButtonAction:(UIButton *)sender{
    
}
#pragma mark -
#pragma mark InputView Delegate
-(void)inputViewBtnPressedAction:(UIButton *)sender{
    self.inputView.inputTextField.text;
}

#pragma mark - UIKeyboardNotification

- (void)keyboardWillChangeFrame:(NSNotification *)notification
{
    NSDictionary *userInfo = notification.userInfo;
    CGRect endFrame = [userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    CGRect beginFrame = [userInfo[UIKeyboardFrameBeginUserInfoKey] CGRectValue];
    CGFloat duration = [userInfo[UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    UIViewAnimationCurve curve = [userInfo[UIKeyboardAnimationCurveUserInfoKey] integerValue];
    
    void(^animations)() = ^{
        [self willShowKeyboardFromFrame:beginFrame toFrame:endFrame];
    };
    
    void(^completion)(BOOL) = ^(BOOL finished){
    };
    
    [UIView animateWithDuration:duration delay:0.0f options:(curve << 16 | UIViewAnimationOptionBeginFromCurrentState) animations:animations completion:completion];
}

- (void)willShowKeyboardFromFrame:(CGRect)beginFrame toFrame:(CGRect)toFrame
{
 
     self.footView.bottom = toFrame.origin.y;
    self.mailChatTableView.height = self.footView.top-self.headView.height;
    [self.view bringSubviewToFront:self.footView];
   
 
}
@end
