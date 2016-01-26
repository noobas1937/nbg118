//
//  AnimateView.m
//  cell_test
//
//  Created by Zsl on 15/11/24.
//  Copyright © 2015年 Zsl. All rights reserved.
//

#import "AnimateView.h"

@implementation AnimateView
-(void)initView:(id)data
{
//    {
//        "flyReward" : [
//        
//        ],
//        "flyToolReward" : [
//                           {
//                               "itemPic" : "item403.png",
//                               "itemNum" : 2
//                           }
//                           ]
//    }
    NSDictionary * dic=(NSDictionary*)data;
    NSLog(@"====%@",dic);
    arr=[dic objectForKey:@"flyReward"];
    arr2=[dic objectForKey:@"flyToolReward"];
    dataArr=[[NSMutableArray alloc]initWithArray:arr];
    [dataArr addObjectsFromArray:arr2];
    num=dataArr.count;  //6行每行最多5个最多30个
}
-(void)StartAnimate
{
    int width = 0;
    int interval = 0;
    
    width =(self.frame.size.width-20*6)/5;
    interval=(self.frame.size.width-width*5)/6;
    
    for (int i=0; i<num; i++) {
        
            NSDictionary * dic=[dataArr objectAtIndex:i];
            UIView * iconImage=[[UIImageView alloc]init];
            iconImage.frame=CGRectMake(self.frame.size.width/2, self.frame.size.height/2, 0, 0);
            iconImage.backgroundColor=[UIColor clearColor];
            iconImage.tag=i;
            iconImage.clipsToBounds=YES;
            [self addSubview:iconImage];
       
            UIImageView * image=[[UIImageView alloc]init];
            image.frame=CGRectMake(0, 0, width, width);
            NSString *imageNameString = [NSString stringWithFormat:@"%@",[dic objectForKey:@"itemPic"]] ;
            imageNameString = [imageNameString lowercaseString];
            UIImage *currentImage =[UIImage imageNamed:imageNameString];
            if (currentImage) {
                image.image=currentImage;
            }else
            {
                image.image= [UIImage imageNamed:@"no_iconflag"];
            }
            image.backgroundColor=[UIColor clearColor];
            [iconImage addSubview:image];
            
            UILabel * lb=[[UILabel alloc]init];
            lb.text=[NSString stringWithFormat:@"+%d",[[dic objectForKey:@"itemNum"] intValue]];
            lb.textColor=[UIColor grayColor];
            lb.textAlignment=NSTextAlignmentCenter;
            lb.font=[UIFont fontWithName:@"HelveticaNeue-Bold" size:14.0];
            lb.frame=CGRectMake(0, width, width, 20);
            lb.backgroundColor=[UIColor clearColor];
            [iconImage addSubview:lb];
            
        
    }
    
    if (num<5) {
        width =(self.frame.size.width-20*6)/5;
        interval=(self.frame.size.width-width*num)/(num+1);
        for (UIView * view in [self subviews]) {
            [UIView animateWithDuration:0.8 animations:^{
                view.frame=CGRectMake(interval+view.tag*(width+interval),70,width,width+20);
                
            } completion:^(BOOL finished) {
                [self upAnimate];
                
                
            }];
            
        }

    }else{//大于5
        //01234 56      01234 56789 101112
        for (UIView * view in [self subviews]) {
            if (num%5!=0) {
                int line=num/5+1;
                if ((view.tag/5+1)==line) {
                    //最后一行
                    width =(self.frame.size.width-20*6)/5;
                    interval=(self.frame.size.width-width*(num%5))/((num%5)+1);
                    
                    [UIView animateWithDuration:0.8 animations:^{
                        view.frame=CGRectMake(interval+view.tag%5*(width+interval),70+(width+40)*(view.tag/5),width,width+20);
                        
                    } completion:^(BOOL finished) {
                        [self upAnimate];
                    }];

                }else{
                    //非最后一行
                    width =(self.frame.size.width-20*6)/5;
                    interval=(self.frame.size.width-width*5)/6;
                    [UIView animateWithDuration:0.8 animations:^{
                        view.frame=CGRectMake(interval+view.tag%5*(width+interval),70+(width+40)*(view.tag/5),width,width+20);
                        
                    } completion:^(BOOL finished) {
                        [self upAnimate];
                        
                    }];
                }
                
            }else{
                
                
                width =(self.frame.size.width-20*6)/5;
                interval=(self.frame.size.width-width*5)/6;
                [UIView animateWithDuration:0.8 animations:^{
                    view.frame=CGRectMake(interval+view.tag%5*(width+interval),70+(width+40)*(view.tag/5),width,width+20);
                    
                } completion:^(BOOL finished) {
                    
                        [self upAnimate];
                }];
 
            }
            
        }

        
    }
    
    [self performSelector:@selector(finishRemove) withObject:nil afterDelay:2];
}
-(void)upAnimate{
    
    for (UIImageView * view in [self subviews]) {
        [UIView animateWithDuration:0.8 animations:^{
            view.frame=CGRectMake(self.frame.size.width/2, self.frame.size.height, 0, 0);
        } completion:^(BOOL finished) {
            
        }];
    }
}
-(void)finishRemove
{
    self.finishAnimated();
}

@end
