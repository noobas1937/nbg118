//
//  GeneralSkillListPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-5-20.
//
//

#ifndef __IF__GeneralSkillListPopUpView__
#define __IF__GeneralSkillListPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

const int startX = 80;
const int gapX = (640 - startX * 2) / 6;
const int gapY = 3 * gapX;
const int numPerRow = 7;

class GeneralSkillListPopUpView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner

{
public:
    static GeneralSkillListPopUpView *create(GeneralInfo *info, std::string abilityId,std::string skillId="");

private:
    GeneralSkillListPopUpView(GeneralInfo *info, std::string abilityId,std::string skillId) : m_info(info), m_abilityId(abilityId),m_skillId(skillId){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onSkillReset(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAbilityResetClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onTabClick1(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTabClick2(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTabClick3(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void confirmResetAbility();
    void confirmResetSkill();

    void refreshView(bool moveFlag = true);
    void generalSkillChange(CCObject* obj);
    void unLockSkill(CCObject* param);
    void addCell(GeneralSkillInfo *info);
    void addLine();
    float addLineAni(int skillId);
    void saveData();
    void setBtnState();
    void delayPlayAni(float _time);
    void playCountPoint(float _time);
    void playUnLockEnd(float _time);
    void playResetAni(int v1,int v2,int v3);
    CCPoint getPointByIndex(int x, int y);
    int getResetItemNum();
    void resetBtnIcon();
    void resetSuccess(CCObject* param);
    void refreshPoint(CCObject* param);
    
    
    CCSafeObject<GeneralInfo> m_info;
    std::string m_abilityId;
    CCSafeObject<CCNode> m_container;
    
    CCSafeObject<CCControlButton> m_skillResetBtn;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCNode> m_bNode;
    CCSafeObject<CCLabelIF> m_expTxt;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCControlButton> m_tab1;
    CCSafeObject<CCControlButton> m_tab2;
    CCSafeObject<CCControlButton> m_tab3;
    
    CCSafeObject<CCLabelIF> m_tabTxt1;
    CCSafeObject<CCLabelIF> m_tabValue1;
    CCSafeObject<CCLabelIF> m_tabTxt2;
    CCSafeObject<CCLabelIF> m_tabValue2;
    CCSafeObject<CCLabelIF> m_tabTxt3;
    CCSafeObject<CCLabelIF> m_tabValue3;
    CCSafeObject<CCLabelIF> m_btnResetText;
    CCSafeObject<CCLabelIF> m_btnReseValue;
    CCSafeObject<CCNode> m_parNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_aniContainer;
    CCSafeObject<CCNode> m_resetNode;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCRenderTexture> m_renderTexture;
    int m_totalH;
    int m_lastPoint;
    int m_countNum;
    int m_addH;
    bool skillChangeFlag;
    std::string m_skillId;
};

class GeneralSkillCell : public CCAniNode
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static GeneralSkillCell *create(GeneralSkillInfo *info, GeneralInfo *gInfo,CCNode* pNode,bool require=false);
    CCSafeObject<GeneralSkillInfo> m_info;
    CCSafeObject<GeneralInfo> m_ginfo;
    void showUnLockEffect(float delayTime);
private:
    GeneralSkillCell(GeneralSkillInfo *info, GeneralInfo *gInfo,CCNode* pNode,bool require) : m_info(info), m_ginfo(gInfo),m_pNode(pNode),m_isRequire(require){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void animationFunc();
    void clickOnCell();
    void addBoomb(float _time);
    void flyBallAni(CCObject* obj);
    void playAni(float _time);
    void refresh(CCObject* param);
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCNode> m_touchLayer;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCSprite> m_skillItemBg;
    CCSafeObject<CCNode> m_lvNode;
    CCNode* m_pNode;
    bool isOpened;
    bool isPlayingAni;
    bool m_isRequire;
    int m_resetLv;

};

class GeneralSkillUpdatePopUpView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
 
{
public:
    static GeneralSkillUpdatePopUpView *create(GeneralSkillInfo *info, GeneralInfo *gInfo);
private:
    GeneralSkillUpdatePopUpView(GeneralSkillInfo *info, GeneralInfo *gInfo) : m_info(info), m_ginfo(gInfo), m_isLongPressUpBtn(false){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onUpdateClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void cdClick(float _time);
    
    void UpgradeSkill(float f);
    void saveSuccess(CCObject* param);
    void refreshView();
    void setBtnState();
    void setText();
    CCSafeObject<GeneralInfo> m_ginfo;
    CCSafeObject<GeneralSkillInfo> m_info;
    CCSafeObject<CCControlButton> m_updateBtn;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_descriptionText;
    CCSafeObject<CCLabelIF> m_attAddText;
    CCSafeObject<CCLabelIF> m_lvText;
    CCSafeObject<CCLabelIF> m_nextLVText;
    CCSafeObject<CCLabelIF> m_curProTxt;
    CCSafeObject<CCLabelIF> m_addTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_picContainer;
    CCSafeObject<CCNode> m_proNode;
    CCSafeObject<CCSprite> m_skillItemBg;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCScale9Sprite> m_nextPro;
    CCSafeObject<CCNode> m_currentNode;
    CCSafeObject<CCNode> m_nextNode;
    CCSafeObject<CCNode> m_curRomanNode;
    CCSafeObject<CCNode> m_nextRomanNode;
    CCSafeObject<CCNode> m_lockNode;
    int m_initLv;
    bool m_isLongPressUpBtn;
};

class PathLine : public CCObject{
public:
    CREATE_FUNC(PathLine);
    virtual bool init(){return true;};
    CCPoint startPt;
    CCPoint endPt;
    int endY;
    bool isOpen;
};

class LinePath : public CCObject{
public:
    static LinePath *create(vector<CCPoint> &starts, vector<CCPoint> &ends, vector<bool> &relation);
    CCArray *getCurrentPathLine();
    void next();
    bool m_pathWalkOver;
private:
    ~LinePath();
    virtual bool init(vector<CCPoint> &starts, vector<CCPoint> &ends, vector<bool> &relation);
    void generatePath(vector<CCPoint> &starts, vector<CCPoint> &ends, vector<bool> &relation);
    void addLineToMap(PathLine *line);
    CCSafeObject<CCArray> m_currentPathLines;
    map<int, CCArray*> m_allLines;
    CCSafeObject<CCArray> m_startLines;
};

#endif /* defined(__IF__GeneralSkillListPopUpView__) */
