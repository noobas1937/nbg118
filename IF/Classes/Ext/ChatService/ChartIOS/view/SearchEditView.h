//
//  SearchEditView.h
//  IF
//
//  Created by 张彦涛 on 15/7/6.
//
//

#import <UIKit/UIKit.h>

@protocol SearchEditViewDelegate <NSObject>
@optional
-(void)searchEditViewBtnPressedAction:(UIButton *)sender;

@end

@interface SearchEditView : UIView

+(instancetype)searchEditView;
@property (weak, nonatomic) IBOutlet UITextField *searchEditTextField;
@property (assign,nonatomic) id<SearchEditViewDelegate>seachEditViewDelegate;

@end
