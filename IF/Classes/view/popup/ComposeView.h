//
//  ComposeView.h
//  IF
//
//  Created by 李朝辉 on 15-7-28.
//
//

#ifndef __IF__ComposeView__
#define __IF__ComposeView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ToolInfo.h"

class ComposeView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner  
{
public:
    static ComposeView *create(int itemId = 0);
    ComposeView(int itemId)
    :itemIndex(0),bcompose(true){
        itemIndex = itemId;
    };
    ~ComposeView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onFuseClick(CCObject *pSender, CCControlEvent event);
    void onSendClick(CCObject *pSender, CCControlEvent event);
    void getData(CCObject *pSender);
    void successCall(CCObject *param);
    void UpdateNumShow(float _time);
    void ParticalShow(float _time);
    void DataClear();
    int GetIdVector(vector<string>& Vect1,vector<string>& Vect2,string str1,string str2);
    void GetNumIconVector(vector<string>& pVector1,vector<string>& PvectCNT,vector<string>& PvectSprite);
    void CopyPicAndStrToBase(int num, int bg_width, vector<string>& pVector2,vector<string>& PvectCNT, vector<string>& PvectSprite);
    void AnimationCom(float _time);
    void AnimationPart(float _time);
    
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_materialneededText;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_tipTxt1;
    CCSafeObject<CCLabelIF> m_tipTxt2;
    CCSafeObject<CCLabelIF> m_target;
    CCSafeObject<CCLabelIF> m_targetdes;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCNode> m_toCompose;
    CCSafeObject<CCNode> m_materialposNode;
    int itemIndex;
    bool bcompose;

    vector<std::string>  strNum;
    vector<CCPoint> strPos;
    vector<CCPoint> materialPos;
    string strTarNum;
    vector< string> InfoMat;
    vector<string> InfoNeemNum;
    CCPoint tarpoint;
    CCLabelTTF *labeltar;
    CCLabelTTF *labelmat;
    vector<CCLabelTTF *> labelArray;
    
};


#endif /* defined(__IF__ComposeView__) */
