//
//  ScienceListView.h
//  IF
//
//  Created by fubin on 14-3-19.
//
//

#ifndef IF_ScienceListView_h
#define IF_ScienceListView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "CCClipNode.h"

class ScienceListView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ScienceListView* create(int buildId);
    ScienceListView():m_buildId(0),m_dataCnt(0),m_title(""),m_clickType(-1),m_isInit(false){};
    virtual ~ScienceListView(){};
    void updateInfo();
    CCNode *getGuideNode(std::string sid);
    void playClose(int type=-1, bool force=false);
    virtual void update(float time);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int buildId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    
    void updateScienceType(int pos, string name, string pic);
    
    void onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn5Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClearCDClick(CCObject * pSender, Control::EventType pCCControlEvent);

    void AnimationCallback();
    void AnimationClose();
    void playBtnSmoke(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void closeView(CCObject* params);
    void playCircleGlow();
    void stopCircleGlow(CCObject* obj);
    void addBGPic();
    
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_name1Label;
    CCSafeObject<CCLabelIF> m_name2Label;
    CCSafeObject<CCLabelIF> m_name3Label;
    CCSafeObject<CCLabelIF> m_name4Label;
    CCSafeObject<CCLabelIF> m_name5Label;
    
    CCSafeObject<CCScale9Sprite> m_name1BG;
    CCSafeObject<CCScale9Sprite> m_name2BG;
    CCSafeObject<CCScale9Sprite> m_name3BG;
    CCSafeObject<CCScale9Sprite> m_name4BG;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_icon1Node;
    CCSafeObject<CCNode> m_icon2Node;
    CCSafeObject<CCNode> m_icon3Node;
    CCSafeObject<CCNode> m_icon4Node;
    CCSafeObject<CCNode> m_icon5Node;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_btn2;
    CCSafeObject<CCControlButton> m_btn3;
    CCSafeObject<CCControlButton> m_btn4;
    CCSafeObject<CCControlButton> m_btn5;
    
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCNode> m_node3;
    CCSafeObject<CCNode> m_node4;
    CCSafeObject<CCNode> m_node5;
    
    CCSafeObject<CCNode> m_crNode1;
    CCSafeObject<CCNode> m_crNode2;
    CCSafeObject<CCNode> m_crNode3;
    CCSafeObject<CCNode> m_crNode4;
    
    CCSafeObject<CCNode> m_sceinceNode;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCLabelIF> m_descriptionText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCControlButton> m_btn;
    CCSafeObject<CCScale9Sprite> m_progrossBar;
    CCSafeObject<CCScale9Sprite> m_progressBG;
    CCSafeObject<CCClipNode>  m_barClipNode;
    
    CCSafeObject<CCLayer> m_smokeLayer;
    vector<CCParticleBatchNode*> m_parVec;
    
    int m_buildId;
    int m_dataCnt;
    string m_title;
    map<int, int> m_posTypes;
    int m_clickType;
    bool m_isInit;
};

#endif
