//
//  BBSIOSViewController.m
//  IF
//
//  Created by 马纵驰 on 15/7/23.
//
//

#import "BBSIOSViewController.h"
#import "TopUIView.h"
#import "ServiceInterface.h"
#import "LanguageManager.h"
#import "LanguageKeys.h"
#import "NSString+Extension.h"
#import "UserManager.h"
#import "ServiceInterface.h"

@interface BBSIOSViewController ()<UIWebViewDelegate>
@property (nonatomic,strong) TopUIView *topUIView;
@property (nonatomic,strong) UIView *navigationView;
@property (nonatomic,strong) UIButton *leftBtn;
@property (nonatomic,strong) UIButton *rightBtn;
@property (nonatomic,strong) UIButton *overloadingBtn;
@property (nonatomic,strong) UIButton *translationBtn;
@property (nonatomic,assign) BOOL isLoadingTranslationPage;
@property (nonatomic,assign) BOOL webPageTranslated;
@property (nonatomic,assign) BOOL needToRecoverLastPosition;
@property (nonatomic,assign) BOOL isFirstLogin;
@property (nonatomic,strong) NSString *currentWebUrl;
@property (nonatomic,strong) NSString *loginUrl;

@property (nonatomic,assign) CGRect bbsViewFrame;

-(void) addTopUIView;
-(void) addUIWebView;
-(void) addNavigationView;
@end

@implementation BBSIOSViewController

-(instancetype)init
{
    self = [super init];
    if (self) {
        self.isLoadingTranslationPage = FALSE;
        self.webPageTranslated = FALSE;
        self.needToRecoverLastPosition = FALSE;
        self.isFirstLogin = TRUE;
        
        NSString *uid = [UserManager sharedUserManager].currentUser.uid;
//        NSString *uid = @"819031593000343";
        NSString *name = [UserManager sharedUserManager].currentUser.userName;
//        NSString *name = @"跑起";
        NSString *gmailAccount = @"";
        NSString *accessToken = [uid stringByAppendingString:@"accesstoken"];
        accessToken = [accessToken md5];
        NSString *lang = @"zh-CN";
        
        if([UserManager sharedUserManager].currentUser.userName.length>0  && [UserManager sharedUserManager].currentUser.uid.length > 0){
            //gmailAccount可以为空
            
            self.loginUrl = @"http://f.elex.com/game_login.php?username=";
            self.loginUrl = [self.loginUrl stringByAppendingString:name];
            
            self.loginUrl = [self.loginUrl stringByAppendingString:@"&userid="];
            self.loginUrl = [self.loginUrl stringByAppendingString:uid];
            
            self.loginUrl = [self.loginUrl stringByAppendingString:@"&useremail="];
            self.loginUrl = [self.loginUrl stringByAppendingString:gmailAccount];
            
            self.loginUrl = [self.loginUrl stringByAppendingString:@"&accesstoken="];
            self.loginUrl = [self.loginUrl stringByAppendingString:accessToken];
            
            self.loginUrl = [self.loginUrl stringByAppendingString:@"&lang="];
            self.loginUrl = [self.loginUrl stringByAppendingString:lang];
            
            self.loginUrl = [self.loginUrl stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        }else{
            self.loginUrl=@"http://f.elex.com/game_login.php?lang=zh-CN";
        }
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self addTopUIView];
    [self addUIWebView];
    [self addNavigationView];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(CGSize) gettingWinSize
{
    return [ UIScreen mainScreen ].bounds.size;
}

-(void)addTopUIView
{
    self.topUIView=[[TopUIView alloc] init:IOS_CHANNEL_TYPE_BBS];
    
    CGSize winSize = [self gettingWinSize];
    
    self.topUIView.frame = CGRectMake(0,0, winSize.width, winSize.height * 0.07);
    
    NSString *title = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TITLE_FORUM];
    
    self.topUIView.titlePlayerName.text = title;
    [self.view addSubview:self.topUIView];
    
}

-(void)addUIWebView
{
    CGSize winSize = [self gettingWinSize];
    CGFloat x = 0;
    CGFloat y = self.view.frame.size.height * 0.07;
    CGFloat width = self.view.frame.size.width;
    CGFloat height = winSize.height * 0.86 ;
    
    //add allianceTableViewController
    self.bbsView = [[UIWebView alloc]init];
    self.bbsView.delegate = self;
    
    self.bbsView.frame = CGRectMake(x, y, width, height);
    self.bbsViewFrame = CGRectMake(x, y, width, height);
    [(UIScrollView *)[[self.bbsView subviews] objectAtIndex:0] setBounces:NO];
    [self.view addSubview:self.bbsView];
    self.bbsView.backgroundColor = [UIColor blackColor];
    [self loadFirstPage];
}

-(void) loadFirstPage
{
    [self loadUrl:self.loginUrl];
//    BOOL hasLogined = true;
//    if(self.isFirstLogin){
//        self.isFirstLogin = false;
//        hasLogined = false;
//    }else{
//        //        	hasLogined = getCookieValue("game_user") != null && getCookieValue("game_user").equals("gameuser");
//    }
//    //	    System.out.println("cookiestring: " + CookieManager.getInstance().getCookie(domain));
//    //	    System.out.println("getCookieValue(\"game_user\"): " + getCookieValue("game_user"));
//    //	    System.out.println("hasLogined:" + hasLogined + " | instanceState:" + (instanceState!=null) + " | targetUrl:" + targetUrl);
//    
//    if(self.targetUrl.length > 0){
//        self.targetUrl = @"";
//        self.needToRecoverLastPosition = true;
//    }
//    
//    if(!hasLogined){
//        if(self.targetUrl.length > 0){
//            [self loadUrl:self.targetUrl];
//        }else{
//            [self loadUrl:self.loginUrl];
//        }
//        self.needToRecoverLastPosition = false;
//    }else{
//        if(self.targetUrl.length > 0){
//            [self loadUrl:self.targetUrl];
//            self.needToRecoverLastPosition = false;
//        }else{
//            if(self.targetUrl.length <= 0){
//                [self loadUrl:self.loginUrl];
//            }
//        }
//    }
//    self.targetUrl = @"";
}

-(void) loadUrl:(NSString*) url
{
    url = [url stringByReplacingOccurrencesOfString:@"|"withString:@"%7C"];
//    url = @"http://f.elex.com/game_login.php?lang=zh-CN";
    
    DVLog(@"url = %@",url);
    
    NSURLRequest *request =[NSURLRequest requestWithURL:[NSURL URLWithString:url]];
    
    [self.bbsView loadRequest:request];
}

-(void)addNavigationView
{
    self.navigationView =[[UIView alloc]init];
    CGSize winSize = [self gettingWinSize];
    self.navigationView.frame = CGRectMake(0, winSize.height * 0.93, winSize.width, winSize.height * 0.07);
    
    
    self.navigationView.backgroundColor = [UIColor blackColor];
    [self.view addSubview:self.navigationView];
    
    [self settingNavigationBtn];
    
}

-(void) settingNavigationBtn
{
    [self settingLeftBtn];
    [self settingRightBtn];
    [self refreshButtonState];
    [self settingOverLoadingBtn];
    [self settingTranslation];
}

-(void) settingLeftBtn
{
    self.leftBtn=[[UIButton alloc]init];
    self.leftBtn.frame = CGRectMake(kMainScreenWidth * 0.05, self.navigationView.frame.size.height * 0.1 , self.navigationView.frame.size.height * 0.8, self.navigationView.frame.size.height * 0.8);
    [self.leftBtn addTarget:self action:@selector(leftEvent:) forControlEvents:UIControlEventTouchUpInside];
    [self.leftBtn setBackgroundImage:[UIImage imageNamed:@"forum_back"] forState:UIControlStateNormal];
    
    [self.navigationView addSubview:self.leftBtn];

}

-(void) settingRightBtn
{
    self.rightBtn=[[UIButton alloc]init];
    self.rightBtn.frame = CGRectMake(kMainScreenWidth * 0.05 + self.leftBtn.frame.size.width, self.navigationView.frame.size.height * 0.1 , self.navigationView.frame.size.height * 0.8, self.navigationView.frame.size.height * 0.8);
    [self.rightBtn setBackgroundImage:[UIImage imageNamed:@"forum_forward"] forState:UIControlStateNormal];
    [self.rightBtn addTarget:self action:@selector(rightEvent:) forControlEvents:UIControlEventTouchUpInside];
    [self.navigationView addSubview:self.rightBtn];
    
}

-(void) settingOverLoadingBtn
{
    self.overloadingBtn=[[UIButton alloc]init];
    self.overloadingBtn.frame = CGRectMake(kMainScreenWidth * 0.05 + self.leftBtn.frame.size.width + self.rightBtn.frame.size.width, self.navigationView.frame.size.height * 0.1 , self.navigationView.frame.size.height * 0.8, self.navigationView.frame.size.height * 0.8);
    [self.overloadingBtn setBackgroundImage:[UIImage imageNamed:@"forum_refresh"] forState:UIControlStateNormal];
    [self.overloadingBtn addTarget:self action:@selector(overLoadingEvent:) forControlEvents:UIControlEventTouchUpInside];
    [self.navigationView addSubview:self.overloadingBtn];
    
    
}

-(void) settingTranslation
{
    self.translationBtn=[[UIButton alloc]init];
    self.translationBtn.frame = CGRectMake(kMainScreenWidth * 0.60, self.navigationView.frame.size.height * 0.1 , kMainScreenWidth * 0.35, self.navigationView.frame.size.height * 0.8);
    [self.translationBtn addTarget:self action:@selector(translationEvent:) forControlEvents:UIControlEventTouchUpInside];

    [self.navigationView addSubview:self.translationBtn];
    
}

-(void) leftEvent:(UIButton *)sender
{
    [self.bbsView goBack];
}

-(void) rightEvent:(UIButton *)sender
{
    [self.bbsView goForward];
}

-(void) overLoadingEvent:(UIButton *)sender
{
    NSString *url = self.bbsView.request.URL.absoluteString;
    [self loadUrl:url];
}

-(void) translationEvent:(UIButton *)sender
{
    if(!self.isLoadingTranslationPage)
    {
        if(!self.webPageTranslated)
        {
            self.currentWebUrl = self.bbsView.request.URL.absoluteString;
            self.isLoadingTranslationPage=true;
            NSString *localLang = [[NSLocale preferredLanguages] objectAtIndex:0];
            
            NSString *newUrl=@"http://translate.google.com/translate?u=";
            newUrl = [newUrl stringByAppendingString:self.bbsView.request.URL.absoluteString];
            newUrl = [newUrl stringByAppendingString:@"&langpair=auto|"];
            newUrl = [newUrl stringByAppendingString:localLang];
            newUrl = [newUrl stringByAppendingString:@"&complete=1&hl=auto&newwindow=1&ie=UTF-8&oe=UTF-8&prev=/language_tools"];
            [self loadUrl:newUrl];
        }
        else
        {
            self.isLoadingTranslationPage=true;
            [self loadUrl:self.currentWebUrl];
        }
    }
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error{
    if (error != nil) {
        DVLog(@"%@",error);
    }
}
-(void)webViewDidFinishLoad:(UIWebView *)webView
{
    if(self.isLoadingTranslationPage)
    {
        self.isLoadingTranslationPage=false;
    }
    
    [self refreshButtonState];
    
    if(self.needToRecoverLastPosition){
        self.needToRecoverLastPosition = false;
    }
    
//    [self calcCurrentWebUrl];
    
    if(CSCONTAINS(self.bbsView.request.URL.absoluteString, @"translate.google"))
    {
        self.webPageTranslated=true;
        NSString *originalItem_Str = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_ORIGINALLAN];
        
        [self.translationBtn setTitle:originalItem_Str forState:UIControlStateNormal];
        [self.translationBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    }
    else
    {
        self.webPageTranslated=false;
        NSString *translateItem_Str = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].MENU_TRANSLATE];
        
        [self.translationBtn setTitle:translateItem_Str forState:UIControlStateNormal];
        [self.translationBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    }
}

-(void) refreshButtonState{
    [self settingBtnAlpha:self.leftBtn withAdd:self.bbsView.canGoBack ? TRUE : false];
    [self settingBtnAlpha:self.rightBtn withAdd:self.bbsView.canGoForward ? TRUE : false];
    [self.leftBtn setEnabled:self.bbsView.canGoBack ? TRUE : FALSE];
    [self.rightBtn setEnabled:self.bbsView.canGoForward ? TRUE : FALSE];
}

-(void) settingBtnAlpha:(UIButton*)btn withAdd:(BOOL) flg
{
    CGFloat alpha = flg ? 1.0f : 0.4f;
    [btn setAlpha:alpha];
}

-(void) calcCurrentWebUrl
{
    //获取源语言网页URL
    NSString *url = self.bbsView.request.URL.absoluteString;
    
    if(CSCONTAINS(url, @"=http:") && CSCONTAINS(url, @"=http://translate.google"))
    {
        
        NSInteger langpairIndex = [url rangeOfString:@"&langpair="].location;
        NSInteger httpIndex = [url rangeOfString:@"=http:"].location;
        NSInteger sandboxIndex = [url rangeOfString:@"&sandbox="].location;
        NSInteger depthIndex= [url rangeOfString:@"&depth="].location;
        NSInteger usgIndex= [url rangeOfString:@"&usg="].location;
        NSInteger netErrorIndex = [url rangeOfString:@"%3Fs%3D"].location;
        NSInteger minIndex=url.length;
        
        if(depthIndex>httpIndex)
        {
            minIndex=minIndex<depthIndex?minIndex:depthIndex;
        }
        
        if(usgIndex>httpIndex)
        {
            minIndex=minIndex<usgIndex?minIndex:usgIndex;
        }
        
        if(sandboxIndex>httpIndex)
        {
            minIndex=minIndex<sandboxIndex?minIndex:sandboxIndex;
        }
        
        if(langpairIndex>httpIndex)
        {
            minIndex=minIndex<langpairIndex?minIndex:langpairIndex;
        }
        
        if(netErrorIndex>httpIndex)
        {
            minIndex=minIndex<netErrorIndex?minIndex:netErrorIndex;
        }
        
        if(minIndex>httpIndex){
            self.currentWebUrl = [url substringWithRange:NSMakeRange(httpIndex+1, minIndex)];
        }else{
            self.currentWebUrl= [url substringToIndex:httpIndex+1];
        }
    }
    else if(CSCONTAINS(url, @"http://") && CSCONTAINS(url, @"?s=")){
        NSInteger index = [url rangeOfString:@"http://"].location;
        NSInteger index2 = [url rangeOfString:@"?s="].location;
        self.currentWebUrl = [url substringWithRange:NSMakeRange(index, index2)];
    }
}

- (void)viewWillAppear:(BOOL)animated {
        [super viewWillAppear:animated];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
           
}
   
- (void)viewWillDisappear:(BOOL)animated {
        [super viewWillDisappear:animated];
        [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillShowNotification object:nil];
        [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillHideNotification object:nil];
}
   
- (void)keyboardWillShow:(NSNotification *)notification {
        NSDictionary *userInfo = [notification userInfo];
        NSValue* value = [userInfo objectForKey:UIKeyboardFrameEndUserInfoKey];
        CGRect keyboardRect = [value CGRectValue]; // 这里得到了键盘的frame
        // 你的操作，如键盘出现，控制视图上移等
    self.navigationView.hidden = YES;
     
}
   
- (void)keyboardWillHide:(NSNotification *)notification {
        // 获取info同上面的方法
        // 你的操作，如键盘移除，控制视图还原等
    self.navigationView.hidden = NO;
}


@end
