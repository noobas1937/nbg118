//
//  CSGoogleTranslat.m
//  IF
//
//  Created by 张彦涛 on 15/7/2.
//
//

#import "CSGoogleTranslat.h"
#import "AFNetworking.h"
//转化成UTF - 8
#define UnicodeStr(URL)  [URL stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]

@interface CSGoogleTranslat ()
{
    NSString *transString;//翻译后的文字
    NSString *transLangType;//翻译的语言类型
}

@end
@implementation CSGoogleTranslat

-(void)translationingWithText:(NSString *)vText
                    andWihtSL:(NSString *)vSL
                    andWithTL:(NSString *)vTL
               andResultBlock:(GoogleTranslatBlock)vBlock{
    NSString *urlString =@"http://translate.google.com/translate_a/t?client=te";
    urlString = [urlString stringByAppendingFormat:@"&text=%@&hl=zh-CN&sl=%@&tl=%@&multires=1&otf=1&ssel=0&tsel=0&uptl=ru&sc=1",UnicodeStr(vText),vSL ,vTL];
    
    NSLog(@"urlString :%@",urlString);
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    
    manager.requestSerializer = [AFHTTPRequestSerializer serializer];
    
    manager.requestSerializer = [AFJSONRequestSerializer serializer];
    manager.responseSerializer = [AFJSONResponseSerializer serializer];
    [manager.requestSerializer setValue:@"text/javascript; charset=UTF-8" forHTTPHeaderField:@"Content-Type"];
    [manager.requestSerializer setValue:@"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36" forHTTPHeaderField:@"User-Agent"];
    
  
    [manager GET:urlString parameters:nil success:^(AFHTTPRequestOperation *operation, id responseObject) {
        if ([responseObject isKindOfClass:[NSDictionary class]]) {
            NSString *src =[responseObject objectForKey:@"src"];
            NSArray *sentencesArray =[responseObject objectForKey:@"sentences"];
            //            NSDictionary *sentenceDic =[sentencesArray objectAtIndex:0];
            
            NSString *yuanString = @"";
            NSString *mubiaoString = @"";
            
            for(NSDictionary *sentenceDic in sentencesArray){
                NSString *y =[sentenceDic objectForKey:@"orig"];
                NSString *m =[sentenceDic objectForKey:@"trans"];
                
                yuanString = [yuanString stringByAppendingString:y];
                mubiaoString = [mubiaoString stringByAppendingString:m];
                
            }
            
            vBlock (ZYTGoogleTranslatHTTPStatus_sucess ,mubiaoString,src);
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        vBlock (ZYTGoogleTranslatHTTPStatus_error,error,nil);
    }];
    
}



-(void)translationingWithSL:(NSString *)vSL
                  andWithTL:(NSString *)vTL
                  andWithHL:(NSString *)vHL
                  andWithIE:(NSString *)vIE
                  andWithOE:(NSString *)vOE
                   andWithQ:(NSString *)vQ
             andResultBlock:(GoogleTranslatBlock)vBlock{
    NSString *urlString =@"https://translate.google.com/translate_a/single?client=t";
    urlString = [urlString stringByAppendingString:[NSString stringWithFormat:@"&sl=%@&tl=%@&hl=%@&dt=t&ie=%@&oe=%@&q=%@",
                                                    vSL,
                                                    vTL,
                                                    vHL,
                                                    vIE,
                                                    vOE,
                                                    vQ]];
    
    
//    NSLog(@"urlString :%@",urlString);
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    
    manager.requestSerializer = [AFHTTPRequestSerializer serializer];
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    manager.requestSerializer.timeoutInterval = 60;
    [manager GET:UnicodeStr(urlString) parameters:nil success:^(AFHTTPRequestOperation *operation, id responseObject) {
        //        NSLog(@"%@",operation);
        NSString *string =[[NSString alloc]initWithData:responseObject encoding:4];
//        NSLog(@"%@",string);
       [self fenjieString:string];
        if (vBlock != nil) {
            vBlock(ZYTGoogleTranslatHTTPStatus_sucess,transString,transLangType);
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
//        NSLog(@"error :%@",error);
        vBlock (ZYTGoogleTranslatHTTPStatus_error,error,nil);
    }];
    
    
    
}

-(void)fenjieString:(NSString *)vString {
//    DVLog(@"要翻译的文字 : %@",vString);
    NSArray *array =[vString componentsSeparatedByString:@",,,,"];
    if (array.count == 0) {
        return;
    }
    NSString *tempString =[array objectAtIndex:0];
//    NSLog(@"%@",tempString);
    NSArray *array1 =[tempString componentsSeparatedByString:@",,"];
    NSString *string1 =[array1 objectAtIndex:0];
    NSString *transLateLangTypeStr = [array1 objectAtIndex:1];
    transLateLangTypeStr =[transLateLangTypeStr stringByReplacingOccurrencesOfString: @"\"" withString:@""];
//    DVLog(@"原语言类型%@",transLateLangTypeStr);
    transLangType = transLateLangTypeStr;
//    NSLog(@"%@",string1);
    string1 =[string1 stringByReplacingOccurrencesOfString: @"[[[\"" withString:@""];
    string1 = [string1 stringByReplacingOccurrencesOfString:@"\"]]" withString:@""];
//    NSLog(@"%@",string1);
    NSArray *stringArray =[string1 componentsSeparatedByString:@"\",\""];
    NSString *lastString =[stringArray objectAtIndex:0];
//    DVLog(@"翻译后的语言%@",lastString);
    transString = lastString;

}

@end
