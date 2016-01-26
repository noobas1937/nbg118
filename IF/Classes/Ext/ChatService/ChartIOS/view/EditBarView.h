//
//  EditBarView.h
//  IF
//
//  Created by 张彦涛 on 15/9/16.
//
//

#import <UIKit/UIKit.h>
@class EditBarView;
@protocol EditBarViewDelegate <NSObject>
@optional
-(void)editBarViewBtnPressedAction:(EditBarView *)sender;
-(void)editBarViewWithInputTextFieldChanged:(EditBarView *)sender;
-(void)editBarViewSpeakAllButtonAction:(EditBarView *)vSender;
-(void)passChangTextViewHeight:(CGFloat)changHeight;
-(void)editBarViewTextViewEditBegin:(EditBarView *)sender;

@end


@interface EditBarView : UIView
@property (nonatomic,assign)BOOL isCountry;
@property (nonatomic,assign,getter=isSpeakerButtonShowing) BOOL speakerButtonShowing;
@property (nonatomic,strong) UITextView *inputTextField;
@property (nonatomic,strong) UIButton  * senderButton;
@property (nonatomic,strong) UIButton  *  speakerChangeButton;
@property (nonatomic,strong) UILabel * textPlace;
@property (nonatomic,strong) NSString * countryPlaceStr;
@property (nonatomic,strong) NSString * alliancePlaceStr;
@property (nonatomic,strong) NSString * oldCountryText;
@property (nonatomic,strong) NSString * oldAllianceText;
@property (nonatomic,assign) id<EditBarViewDelegate>editBarViewDelegate;
@property (nonatomic,assign )int  headHeight;
@property (nonatomic,assign)int bodyHeght;
-(void)passchatVCShowTableType:(BOOL)isType;
-(void)textViewDidChange:(UITextView *)textView;

@end
