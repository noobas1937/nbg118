//
//  HeadEditView.h
//  IF
//
//  Created by 张彦涛 on 15/6/17.
//
//

#import <UIKit/UIKit.h>


@interface HeadEditView : UIView

@property (weak, nonatomic) IBOutlet UITextField *editView;

@property (strong,nonatomic) NSString * titleString ;
+(instancetype)headEditView;



@end
