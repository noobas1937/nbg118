//
//  AnimateView.h
//  cell_test
//
//  Created by Zsl on 15/11/24.
//  Copyright © 2015年 Zsl. All rights reserved.
//

#import <UIKit/UIKit.h>
typedef void (^finishAnimate)(void);
@interface AnimateView : UIView
{
    int num;
    NSArray *arr;
    NSArray * arr2;
    NSMutableArray * dataArr;
}
-(void)initView:(id)data;
-(void)StartAnimate;
@property(nonatomic,copy) finishAnimate finishAnimated;
@end
