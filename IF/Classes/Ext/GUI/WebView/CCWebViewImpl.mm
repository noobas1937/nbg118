//
//  CCWebViewImpl.mm
//  aoe
//
//  Created by 谢文杰 on 13-3-1.
//
//

#import "CCWebViewImpl.h"
#import "CCEAGLView-ios.h"

USING_NS_CC;

static CGPoint convertDesignCoordToScreenCoord(const CCPoint& designCoord, bool bInRetinaMode)
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) glview->getEAGLView();
    
    float viewH = (float)[eaglview getHeight];
    
    CCPoint visiblePos = ccp(designCoord.x * glview->getScaleX(), designCoord.y * glview->getScaleY());
    CCPoint screenGLPos = ccpAdd(visiblePos, glview->getViewPortRect().origin);
    
    CGPoint screenPos = CGPointMake(screenGLPos.x, viewH - screenGLPos.y);
    
    if (bInRetinaMode)
    {
        screenPos.x = screenPos.x / 2.0f;
        screenPos.y = screenPos.y / 2.0f;
    }
   
    return screenPos;
}

CCWebViewImpl::CCWebViewImpl(const cocos2d::CCPoint& pos,const cocos2d::CCSize& size)
{
    m_impl = [CCWebViewImplIOS alloc];
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) glview->getEAGLView();

    CGRect rect = CGRectMake(0, 0, size.width * glview->getScaleX(), size.height * glview->getScaleY());
    if ([eaglview contentScaleFactor] == 2.0f)
    {
        rect.size.width /= 2.0f;
        rect.size.height /= 2.0f;
    }
    initWithFrame(rect);
    
    CGRect frame = [m_impl.mWebView frame];
    frame.origin = convertDesignCoordToScreenCoord(pos, [eaglview contentScaleFactor] == 2.0f);
    [m_impl.mWebView setFrame:frame];
}

CCWebViewImpl::~CCWebViewImpl()
{
    [m_impl release];
}

bool CCWebViewImpl::initWithFrame(const CGRect &frameRect)
{
    return [m_impl initWithFrame:frameRect];
}

void CCWebViewImpl::setVisible(bool bVisible)
{
    [m_impl.mWebView setHidden:bVisible==false];
}

void CCWebViewImpl::loadUrl(const char *urlString)
{
    NSString *urlBase = [NSString stringWithUTF8String:urlString];
    [m_impl.mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlBase]]];
    [m_impl.mWebView setUserInteractionEnabled:NO]; //don't let the user scroll while things are
}

void CCWebViewImpl::loadFile(const char *filePath)
{
    NSString * urlStr = [NSString stringWithUTF8String:CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath).c_str()];
//    [m_impl.mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL fileURLWithPath:urlStr isDirectory:NO]]];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"file://%@",[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]]]];
    [m_impl.mWebView loadRequest:request];
    [request release];
}

@implementation CCWebViewImplIOS

@synthesize mWebView = mWebView_;

- (id) initWithFrame:(CGRect)frameRect {
    self = [super init];
    
    if (self) {
        // init code here.
        // create webView
        self.mWebView = [[[UIWebView alloc] initWithFrame:frameRect] autorelease];
		if (!mWebView_)
			return nil;
        mWebView_.delegate = self;
        mWebView_.opaque = NO;
        mWebView_.backgroundColor = [UIColor clearColor];
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        CCEAGLView *eaglview = (CCEAGLView *) glview->getEAGLView();
        [eaglview addSubview:mWebView_];
    }
    return self;
}

- (void)dealloc{
    [mWebView_ removeFromSuperview];
	self.mWebView = NULL;
    [super dealloc];
}

- (void)webViewDidStartLoad:(UIWebView *)thisWebView {
}

- (void)webViewDidFinishLoad:(UIWebView *)thisWebView{
    [mWebView_ setUserInteractionEnabled:YES];
//    mLayerWebView->webViewDidFinishLoad();
}

- (void)webView:(UIWebView *)thisWebView didFailLoadWithError:(NSError *)error {
    if ([error code] != -999 && error != NULL) { //error -999 happens when the user clicks on something before it's done loading.
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Network Error" message:@"Unable to load the page. Please keep network connection." delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        [alert show];
        [alert release];
    }
}

@end
