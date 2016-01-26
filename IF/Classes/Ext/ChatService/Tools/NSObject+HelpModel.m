//
//  NSObject+HelpModel.m
//  CreatMoreChat
//
//  Created by 张彦涛 on 15/6/10.
//  Copyright (c) 2015年 elex-tech. All rights reserved.
//

#import "NSObject+HelpModel.h"

@implementation NSObject (HelpModel)

-(NSString*)printAllPropertys
{
#ifdef DEBUG
    NSMutableString* sb = [NSMutableString stringWithFormat:@"<%@> \n", [self class]];
    unsigned int outCount, i;
    objc_property_t *properties = class_copyPropertyList([self class], &outCount);

    for (i = 0; i < outCount; i++) {
        objc_property_t property = properties[i];
        NSString *propertyName = [NSString stringWithCString:property_getName(property) encoding:NSUTF8StringEncoding];
       
        NSString *propertyType = [NSString stringWithCString: property_getAttributes(property) encoding:NSUTF8StringEncoding];
        propertyType = [propertyType lowercaseString];
//        DLog(@"%@ :%@",propertyName ,propertyType);
        if ([propertyType hasPrefix:@"ti"] || [propertyType hasPrefix:@"tb"] || //int bool
            [propertyType hasPrefix:@"tf"] ||  //float
            [propertyType hasPrefix:@"td"] ||  //double
            [propertyType hasPrefix:@"tc"] || //chat
            [propertyType hasPrefix:@"tl"] || [propertyType hasPrefix:@"tq"] ||  //long
            [propertyType hasPrefix:@"ts"]) //
 
        {
            
              [sb appendFormat:@" %@ : %@ \n",propertyName,[self valueForKey:propertyName]];
        
        }
        else if([propertyType hasPrefix:@"t@\"ns"])
        {
            [sb appendFormat:@" %@ : %@ \n",propertyName,[self valueForKey:propertyName]];

        }else{
            id value = [self valueForUndefinedKey:propertyName];
 
              [sb appendFormat:@" %@ : %@ \n",propertyName,value ];
        }
    
        
    }
    free(properties);
 
    return sb;
#else
    return @"";
#endif

}
@end
