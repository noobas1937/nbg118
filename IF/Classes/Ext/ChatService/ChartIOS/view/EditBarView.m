//
//  EditBarView.m
//  IF
//
//  Created by 张彦涛 on 15/9/16.
//
//

#import "EditBarView.h"
#import "UIImage+resized.h"
#import "NSString+Cocos2dHelper.h"
@interface EditBarView ()<UITextViewDelegate>
{
    
    
    CGRect oldCountry;
    CGRect oldAlliance;
   
    
    CGFloat fontH;
    int HeadH;
    
}

@property (nonatomic,strong)UIImageView * textViewBackImageFrame;
@property (nonatomic,strong)UIImageView * textViewBackImage;
@property (nonatomic,strong)UIImageView *backGroundView;
@property (nonatomic,strong)UIImageView *topbackRegulaImageView;
@property (nonatomic,strong)UIView *headView;
@property (nonatomic,strong)UIView *bodyView;

@property (nonatomic,strong)UIImageView *headbackImageView;
@property (nonatomic,strong)UIImageView *voliceImageView;
@end
@implementation EditBarView
-(instancetype)initWithFrame:(CGRect)frame{
    self  = [super initWithFrame:frame];
    
    if (self){
        self.backgroundColor = [UIColor clearColor];
        _bodyHeght = frame.size.height;
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            
            fontH=43.5;
            HeadH=35;
        }else{
            
            fontH=35.5;
            HeadH=25;
        }
      
        
        [self _initStructView];
        [self _initSubViews];
        [self _initHeadSubView];
        
        
    }
    return self;

}
-(void)passchatVCShowTableType:(BOOL)isType
{
    self.isCountry=isType;
}
-(void)setSpeakerButtonShowing:(BOOL)speakerButtonShowing{
    
    
    _speakerButtonShowing = speakerButtonShowing;
    if (_speakerButtonShowing) {
        //显示喇叭按钮
        self.oldAllianceText=self.inputTextField.text;
       self.speakerChangeButton.hidden = !_speakerButtonShowing;
       self.inputTextField.frame =oldCountry;
       self.textPlace.frame=CGRectMake( self.speakerChangeButton.width, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 -self.speakerChangeButton.width, fontH);
       self.textViewBackImage.frame=oldCountry;
       self.inputTextField.text=self.oldCountryText;
        self.textPlace.text=self.countryPlaceStr;
        if (self.inputTextField.text.length!=0) {
             self.textPlace.text=@"";
        }
        
        
        
        if (self.speakerChangeButton.selected) {
 
            [self showSpeakAllStyle];
            self.headView.hidden = NO;
            self.textViewBackImageFrame.hidden=NO;
             self.topbackRegulaImageView.hidden=YES;
        }else{
 
            [self showNormalStyle];
             self.headView.hidden = YES;
            self.textViewBackImageFrame.hidden=YES;
             self.topbackRegulaImageView.hidden=NO;
        }

    }else{
        //隐藏喇叭按钮
        
        self.oldCountryText=self.inputTextField.text;
        self.speakerChangeButton.hidden = !_speakerButtonShowing;
        self.inputTextField.frame = oldAlliance;
        self.textPlace.frame=CGRectMake( 5, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 , fontH);
        self.textViewBackImage.frame=oldAlliance;
        self.inputTextField.text=self.oldAllianceText;
        self.textPlace.text=self.alliancePlaceStr;
        if (self.inputTextField.text.length!=0) {
            self.textPlace.text=@"";
        }
        [self showNormalStyle];
        self.topbackRegulaImageView.hidden=NO;
        self.headView.hidden = YES;
        self.textViewBackImageFrame.hidden=YES;
        

    }
}

-(void)globalBroadcastModeWithOpened:(BOOL)vOpened{
    self.speakerChangeButton.selected = vOpened;
    if (vOpened) {
 
        [self showSpeakAllStyle];
//        self.height = _bodyHeght + _headHeight;
//        self.headView.hidden = NO;
//        self.bodyView.top = self.height - _bodyHeght;
        
    }else{
 
        [self showNormalStyle];
//        self.height = _bodyHeght;
//        self.headView.hidden = YES;
//        self.bodyView.top = self.height - _bodyHeght;
        
    }
    if ([self.editBarViewDelegate respondsToSelector:@selector(editBarViewSpeakAllButtonAction:)]) {
        [self.editBarViewDelegate editBarViewSpeakAllButtonAction:self];
    }
}
-(void)_initStructView{
    self.headView =[[UIView alloc]initWithFrame:CGRectMake(0, -HeadH, self.frame.size.width, HeadH)];
    self.headView.backgroundColor =[UIColor orangeColor];
    [self addSubview:self.headView];
    self.bodyView =[[UIView alloc]initWithFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height)];
    self.bodyView.backgroundColor =[UIColor blueColor];
    
    [self addSubview:self.bodyView];
}
-(void)_initSubViews{

    self.backGroundView = [[UIImageView alloc]initWithFrame:CGRectMake(0, 0, self.bodyView.width, self.bodyView.height+100)];
    self.backGroundView.image = [UIImage resizableImage:@"chuzheng_frame02"];
    [self.bodyView addSubview:self.backGroundView];
    
    self.topbackRegulaImageView=[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, self.bodyView.width, self.bodyView.height*0.15)];
    self.topbackRegulaImageView.image =[UIImage resizableImage:@"line_grey02"];
    [self.bodyView addSubview:self.topbackRegulaImageView];
    
    self.senderButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.senderButton setBackgroundImage:[UIImage imageNamed:@"btn_yellow"] forState:UIControlStateNormal];
    NSString *buttonTitleStr=[NSString stringWithMultilingualWithKey:@"105302"];//105302=发送
    [self.senderButton setTitle:buttonTitleStr forState:UIControlStateNormal];
    self.senderButton.frame = CGRectMake(self.bodyView.width * 0.7, self.bodyView.height * 0.18, self.bodyView.width * 0.29, self.bodyView.height * 0.78);
    self.senderButton.titleLabel.font =[UIFont systemFontOfSize:14.0];
    [self.senderButton addTarget:self action:@selector(senderButtonAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.bodyView addSubview:self.senderButton];
    
    self.speakerChangeButton = [UIButton buttonWithType:UIButtonTypeCustom];
    self.speakerChangeButton.frame = CGRectMake(0, self.bodyView.height * 0.15, self.bodyView.height * 0.8, self.bodyView.height * 0.8);
    [self.speakerChangeButton setImage:[UIImage imageNamed:@"horn_unchecked"] forState:UIControlStateNormal];
    [self.speakerChangeButton setImage:[UIImage imageNamed:@"horn_checked"] forState:UIControlStateSelected];
    [self.speakerChangeButton addTarget:self action:@selector(speakChangeButtonAction:) forControlEvents:UIControlEventTouchUpInside];
    
    [self.bodyView addSubview:self.speakerChangeButton];
    
    
    self.textViewBackImage=[[UIImageView alloc]initWithFrame:CGRectMake( self.speakerChangeButton.width, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 -self.speakerChangeButton.width, fontH)];
    self.textViewBackImage.image=[UIImage resizableImage:@"text_field_bg2"];
    [self.bodyView addSubview:self.textViewBackImage];
    
    
    self.textViewBackImageFrame=[[UIImageView alloc]initWithFrame:CGRectMake( self.speakerChangeButton.width-2, self.bodyView.height  * 0.25-5 , self.bodyView.width * 0.7 -self.speakerChangeButton.width, fontH+8)];
    self.textViewBackImageFrame.image=[UIImage resizableImage:@"input"];
    [self.bodyView addSubview:self.textViewBackImageFrame];
    self.textViewBackImageFrame.hidden=YES;
    
    
    //35.5 - 16号字体需要的高度  34 - 15
    self.inputTextField =[[UITextView alloc]initWithFrame:CGRectMake( self.speakerChangeButton.width, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 -self.speakerChangeButton.width, fontH)];
    self.textPlace=[[UILabel alloc]initWithFrame:CGRectMake( self.speakerChangeButton.width, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 -self.speakerChangeButton.width, fontH)];
    self.textPlace.backgroundColor=[UIColor clearColor];
    self.textPlace.textColor=[UIColor grayColor];
    self.textPlace.text=self.countryPlaceStr;
    [self.bodyView addSubview:self.textPlace];

    
    int font;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        
        font=23;
        
    }else{
        
        font=16;
    }
    self.textPlace.font=[UIFont systemFontOfSize:font];
    self.textPlace.text=@"123";
    
    self.inputTextField.font=[UIFont systemFontOfSize:font];
    self.inputTextField.delegate=self;
    self.inputTextField.returnKeyType = UIReturnKeyDefault;
    self.inputTextField.backgroundColor=[UIColor clearColor];
    [self.bodyView addSubview:self.inputTextField];
    
    
    oldCountry=CGRectMake( self.speakerChangeButton.width, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 -self.speakerChangeButton.width, fontH);
    oldAlliance=CGRectMake( 5 , self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 , fontH);
    self.oldAllianceText=@"";
    self.oldCountryText=@"";
    //[self.inputTextField addTarget:self action:@selector(textFieldDidChange:) forControlEvents:UIControlEventEditingChanged];
    
    //输入框左侧加入内边距
    
//    CGRect frame =_inputTextField.frame;
//    frame.size.width = 10.0f;
//    UIView *leftview = [[UIView alloc] initWithFrame:frame];
    //self.inputTextField.leftViewMode = UITextFieldViewModeAlways;
    //self.inputTextField.leftView = leftview;
    
    self.senderButton.enabled = NO;
}

- (void)textViewDidBeginEditing:(UITextView *)textView{
    if ([self.editBarViewDelegate respondsToSelector:@selector(editBarViewTextViewEditBegin:)]) {
        [self.editBarViewDelegate editBarViewTextViewEditBegin:self];
    }

}

-(void)textViewDidChange:(UITextView *)textView
{
    //font 15 一行+18   font 16 一行+19
    //54 73 92
    
    if (textView.text.length==0) {
        if (self.isCountry) {
            self.textPlace.text=self.countryPlaceStr;
            
        }else{
            self.textPlace.text=self.alliancePlaceStr;
            
        }
    }else{
        self.textPlace.text=@"";
    }
    
//    NSLog(@"%@",textView.text);
    if ([self.editBarViewDelegate respondsToSelector:@selector(editBarViewWithInputTextFieldChanged:)]) {
        [self.editBarViewDelegate editBarViewWithInputTextFieldChanged:self];
    }
    
    CGFloat lineH;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        
        lineH=27.5;
    }else{
        
        lineH=19;
    }
    CGSize size=[textView sizeThatFits:CGSizeMake(textView.frame.size.width, 0)];
    
    if (size.height>fontH+lineH) {
        size.height=fontH+lineH;
    }
    if (size.height<=fontH+lineH) {
       
        CGRect frame=textView.frame;
        if (textView.frame.size.height!=size.height) {
            
            if (self.isCountry) {
                if (self.editBarViewDelegate && [self.editBarViewDelegate respondsToSelector:@selector(passChangTextViewHeight:)]) {
                    [self.editBarViewDelegate passChangTextViewHeight:size.height-textView.frame.size.height];
                    //                self.backGroundView.frame=CGRectMake(0, 0, self.bodyView.frame.size.width, self.frame.size.height);
                    self.textViewBackImage.frame=CGRectMake( self.speakerChangeButton.width, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 -self.speakerChangeButton.width, size.height);
                    self.textViewBackImageFrame.frame=CGRectMake( self.speakerChangeButton.width-2, self.bodyView.height  * 0.25-5 , self.bodyView.width * 0.7 -self.speakerChangeButton.width, size.height+8);
                    self.inputTextField.frame=CGRectMake( self.speakerChangeButton.width, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 -self.speakerChangeButton.width, size.height);
                    frame.size.height=size.height;
                    textView.frame=frame;
                    oldCountry=frame;

                }
                
                
            }else{
                 if (self.editBarViewDelegate && [self.editBarViewDelegate respondsToSelector:@selector(passChangTextViewHeight:)]) {
                     [self.editBarViewDelegate passChangTextViewHeight:size.height-textView.frame.size.height];
                     //                self.backGroundView.frame=CGRectMake(0, 0, self.bodyView.frame.size.width, self.frame.size.height);
                     self.textViewBackImage.frame=CGRectMake( 5, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 , size.height);
                     self.inputTextField.frame=CGRectMake( 5, self.bodyView.height  * 0.25 , self.bodyView.width * 0.7-5 , size.height);
                     frame.size.height=size.height;
                     textView.frame=frame;
                     oldAlliance=frame;
                 }
               
                

                
            }
           
        }
    }
    

}
-(void)_initHeadSubView{
    self.headbackImageView = [[UIImageView alloc]initWithFrame:CGRectMake(0, 0, self.width, HeadH)];
    self.headbackImageView.image =[UIImage resizableImage:@"horn_text_bg"];
    self.voliceImageView =[[UIImageView alloc]initWithFrame:CGRectMake(5, 0, HeadH, HeadH)];
    self.voliceImageView.image =[UIImage resizableImage:@"horn_icon"];
    UILabel *titleLabel =[[UILabel alloc]initWithFrame:CGRectMake(10+HeadH, 3, 100, HeadH-5)];
    titleLabel.text = [NSString stringWithMultilingualWithKey:@"105331"];//105331=公告
    titleLabel.font = [UIFont systemFontOfSize:12.0f];
    titleLabel.textColor = [UIColor yellowColor];
    [self.headView addSubview:self.headbackImageView];
    [self.headView addSubview:self.voliceImageView];
    [self.headView addSubview:titleLabel];
    self.headView.hidden=YES;
    
}

-(void)showNormalStyle{
   // self.inputTextField.background =[UIImage resizableImage:@"text_field_bg2"];
    
    self.textViewBackImageFrame.hidden=YES;
    self.backGroundView.image =[UIImage resizableImage:@"chuzheng_frame02"];
    [self.senderButton setBackgroundImage:[UIImage imageNamed:@"btn_yellow"] forState:UIControlStateNormal];
    //self.topbackRegulaImageView.hidden = NO;
    
}
-(void)showSpeakAllStyle{
    
    self.textViewBackImageFrame.hidden=NO;
    //self.inputTextField.background =[UIImage resizableImage:@"input"];
    self.backGroundView.image =[UIImage resizableImage:@"bottom_bg"];
    [self.senderButton setBackgroundImage:[UIImage imageNamed:@"buttonBack_green"] forState:UIControlStateNormal];
    //self.topbackRegulaImageView.hidden = YES;

}

- (void)senderButtonAction:(id)sender {
    if ([self.editBarViewDelegate respondsToSelector:@selector(editBarViewBtnPressedAction:)]) {
        [self.editBarViewDelegate editBarViewBtnPressedAction:self];
    }
}
-(void)speakChangeButtonAction:(UIButton *)sender{
    
    sender.selected = !sender.selected;
    
    if (sender.selected) {
        self.headView.hidden=NO;
        self.topbackRegulaImageView.hidden=YES;
    }else{
        self.headView.hidden=YES;
        self.topbackRegulaImageView.hidden=NO;
    }
    [self globalBroadcastModeWithOpened:sender.selected];
 
}
//-(void)textFieldDidChange:(UITextField *)sender{
//    NSLog(@"%@",sender.text);
//    if ([self.editBarViewDelegate respondsToSelector:@selector(editBarViewWithInputTextFieldChanged:)]) {
//        [self.editBarViewDelegate editBarViewWithInputTextFieldChanged:self];
//    }
//}



@end
