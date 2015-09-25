//
//  ZYTSystemMailListVC.m
//  IF
//
//  Created by 张彦涛 on 15/8/31.
//
//

#import "ZYTSystemMailListVC.h"

@interface ZYTSystemMailListVC ()
@property (strong, nonatomic)  UIView *headView;
@property (strong, nonatomic)  UIView *footView;


@end

@implementation ZYTSystemMailListVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
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
            footView.frame = CGRectMake(0, kMainScreenHeight  , kMainScreenWidth, kMainScreenHeight * 0.07);
        } else{
            footView.frame = CGRectMake(0, kMainScreenHeight  , kMainScreenWidth, kMainScreenHeight * 0.07);
        }
        _footView = footView;
    }
    return _footView;
}



@end
