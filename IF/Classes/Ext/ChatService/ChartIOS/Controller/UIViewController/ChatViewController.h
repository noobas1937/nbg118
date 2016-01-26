//
//  ViewController.h
//  气泡
//
//  Created by zzy on 14-5-13.
//  Copyright (c) 2014年 zzy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "KeyBordVIew.h"
#import "ChatCountriesTableViewController.h"
#import "ChatAllianceTableViewController.h"
#import "TopUIView.h"

@interface ChatViewController : UIViewController

@property (nonatomic,strong) ChatCountriesTableViewController *countriesTableViewController;
@property (nonatomic,strong) ChatAllianceTableViewController *allianceTableViewController;

@property (nonatomic,strong) KeyBordVIew *keyBordView;
@property (nonatomic,strong) TopUIView *topUIView;
@property (nonatomic,strong) UIView *jionAllanceView;

-(void) isShowJionAllance;
-(void) hiddenJionAllanceView;
-(void)chatVCEndingEdit;
@end

