//
//  CSMessageModel+CustemColor.m
//  IF
//
//  Created by 张彦涛 on 15/12/20.
//
//

#import "CSMessageModel+CustemColor.h"
 @implementation CSMessageModel (CustemColor)
-(UIColor *)colorWithColorDefine:(int)vColorDefine{
    UIColor *color = nil;
    switch (vColorDefine)
    {
        case 0:
            
            color = [self colorCustemWith256Red:199 green:190 blue:179  ];
//            color = @"173|175|172";
            break;
        case 1:
            color = [self colorCustemWith256Red:86 green:299 blue:120];
//            color = @"83|151|33";
            break;
        case 2:
            color = [self colorCustemWith256Red:69 green:153 blue:248];
//            color = @"71|132|225";
            break;
        case 3:
            color =[self colorCustemWith256Red:175 green:73 blue:234];
//            color = @"112|72|194";
            break;
        case 4:
            color =[self colorCustemWith256Red:232 green:119 blue:31];
//            color = @"193|129|72";
            break;
        case 5:
            color =[self colorCustemWith256Red:237 green:213 blue:56];
//            color = @"205|183|63";
            break;
        default:
            color =[UIColor blackColor];
//            color = @"173|175|172";
            break;
    }
    return color;

}
- (UIColor *) colorCustemWith256Red:(NSUInteger)red green:(NSUInteger)green blue:(NSUInteger)blue {
    return [UIColor colorWithRed:red / 255.0
                           green:green / 255.0
                            blue:blue / 255.0
                           alpha:1];
}

@end
