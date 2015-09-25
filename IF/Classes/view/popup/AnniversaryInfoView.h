//
//  AnniversaryInfoView.h
//
//  Created by zhangjun on 2015-07-16
//
//
		
#ifndef __LastEmpire__AnniversaryInfoView__
#define __LastEmpire__AnniversaryInfoView__
		
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "PopupBaseView.h"
#include "HFHttpRequest.h"
#include "HFHttpClient.h"
		
class AnniversaryInfoView :public PopupBaseView,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    AnniversaryInfoView(){CCLOGFUNC();};
    virtual ~AnniversaryInfoView();
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    static AnniversaryInfoView* create();
		
    bool onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    
    void setScrollData(CCObject *obj);
    void insertIntoArr(double time,int type);
    std::string getStringByDialog(int index);
    void fbShare(CCObject* pSender, CCControlEvent event);
    void AnimationCallback();
    
    void getImgUrl(cocos2d::CCObject *pObj);
    void onHttpRequestCompleted(HFHttpClient *pSender, HFHttpResponse *pResponse);
    CCDictionary* parseResponseData(vector<char>* response);
    std::string UrlEncode(const std::string& str);
protected:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCScrollView* m_scroll;
    
    CCSafeObject<CCNode> m_nodeBG;
    CCSafeObject<CCNode> m_scrollNode;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_headTxt;
    CCSafeObject<CCSprite> m_sprSubBg;
    
    CCArray *dialogArr;
    CCArray *timeArr;
    string itemPic;
    string playerName;
    string otherName;
    
    string shareString[4];
    string shareLang;
    
private:
//    CCArray *m_dataArr;
};
		
#endif /* defined(__LastEmpire__AnniversaryInfoView__) */
		
