//
//  ChatCell.h
//  IF
//
//  Created by fubin on 13-10-30.
//
//

#ifndef IF_ChatCell_h
#define IF_ChatCell_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "../../Ext/CCDevice.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
class ChatCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static ChatCell* create(int type, int idx, CCNode* touchNode,string allianceId="");
    ChatCell():m_type(0),m_idx(0),m_uid(""),m_allianceId(string()){};
    ~ChatCell(){};
    void setData(int type, int idx,string allianceId="");
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init(int type, int idx, CCNode* touchNode,string allianceId="");
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void initContents(string info, int lastWidth, int maxNum, int maxWidth);
    void onShieldClick(cocos2d::CCObject *pTarget, CCControlEvent touchEvent);
    void onYesClick();
    void setTranslation(CCObject* msg);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTranslateClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void openChatFun(CCObject* p);
    void joinSuccess(CCObject* p);
    void readMailSuccess(CCObject* p);
    void readDetectRepMailSuccess(CCObject* p);
    bool checkSameLangBySystem(string oLang);
    bool showCustomPic(int senderPicVer);
    
    CCSafeObject<CCNode> m_userNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIFTTF> m_nameLabel;
    CCSafeObject<CCSprite> m_mod;
    CCSafeObject<CCSprite> m_gm;
    CCSafeObject<CCNode> m_msgNode;
    CCSafeObject<CCLabelIFTTF> m_msgLabel;
    CCSafeObject<CCLabelIFTTF> m_timeLabel;
    CCSafeObject<CCNode> m_timeNode;
    CCSafeObject<CCNode> m_touchNode1;
    CCSafeObject<CCNode> m_touchNode2;
//    CCSafeObject<CCNode> m_touchNode3;
    CCSafeObject<CCSprite> m_left;
    CCSafeObject<CCSprite> m_right;
    CCSafeObject<CCScale9Sprite> m_chatBG;
    CCSafeObject<CCScale9Sprite> m_sysBG;
    CCSafeObject<CCLabelIFBMFont> m_vipLabel;
//    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_translateNode;
    CCSafeObject<CCLabelIF> m_translateLabel;
    CCSafeObject<CCControlButton> m_translateBtn;
    CCSafeObject<CCNode> m_langNode;
    CCSafeObject<CCControlButton> m_joinBtn;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    CCSafeObject<CCScale9Sprite> m_noticeBG;
    CCSafeObject<CCSprite> m_noticeleft;
    CCSafeObject<CCSprite> m_noticeright;
    
    int m_type;
    int m_idx;
    string m_uid;
    CCSize m_bgSize;
    int m_timeNodeY;
    CCPoint m_nameLabelPt;
    CCPoint m_vipLabelPt;
    CCPoint m_translatePt;
    int m_touchNode3Y;
    int m_msgLabelY;
    CCSize m_msgDimensions;
    CCPoint m_touchPoint;
	bool m_isOriginal;
	bool m_isCheckingInAllance;
	int m_prevPopupCount;
    string m_allianceId;
    
    CCSize getTextDimension(const char *text);
};

#endif
