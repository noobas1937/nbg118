//
//  NewEnemyInfoMoreView.h
//  IF
//
//  Created by 李锐奇 on 14-9-16.
//
//

#ifndef __IF__NewEnemyInfoMoreView__
#define __IF__NewEnemyInfoMoreView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "EnemyInfo.h"
#include "HFHeadImgNode.h"

class MarchInfoCell;
class TitleCell;
class ScienceCell;
class AbilityCell;
class ArmyInfoCell;

class NewEnemyInfoMoreView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
{
public:
    static NewEnemyInfoMoreView *create(std::string uid, int buildId, int type);
    NewEnemyInfoMoreView() : m_uid(""){};
    virtual ~NewEnemyInfoMoreView(){};
    void updateInfo();
    static bool isContains(std::string index, int buildId);
    virtual void update(float time);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(std::string uid, int buildId, int type);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onGoHomeClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickIgnoreBtn(CCObject * pSender, CCControlEvent pCCControlEvent);

    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_picContainer;
    CCSafeObject<CCNode> m_coordTextNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<Label> m_nameText;
    CCSafeObject<CCLabelIF> m_stateText;
    CCSafeObject<CCLabelIF> m_coordText;
    CCSafeObject<CCLabelIF> m_progressText;
    
    CCSafeObject<CCNode> m_picContainer2;
    CCSafeObject<Label> m_nameText2;
    CCSafeObject<CCNode> m_coordTextNode2;
    CCSafeObject<CCLabelIF> m_coordText2;
    
    CCSafeObject<CCNode> m_nodeEnemy;
    CCSafeObject<CCNode> m_nodePlayer;
    CCSafeObject<CCLabelIF> m_labEnemyLv;

    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCControlButton> m_goHomeBtn;
    CCSafeObject<CCControlButton> m_ignoreBtn;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    void addMarchInfo();
    void addResourceInfo();
    void addAbilityInfo();
    void addScienceInfo();
    void addArmyInfo();
    
    TitleCell *addTitleCell(std::string title1, std::string title2);
    EnemyInfo *getEnemyInfo();
    std::string m_uid;
    CCSafeObject<MarchInfoCell> m_marchCell;
    int m_h;
    int m_buildId;
    int m_btype;
};

//--

class MarchInfoCell : public CCNode{
public:
    static MarchInfoCell *create(CCDictionary *dict);
    void update(float time);
    int getH();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(CCDictionary *dict);
    int m_h;
};

//--

class ArmyInfoCell : public CCNode{
public:
    static ArmyInfoCell *create(CCDictionary *dict, int buildId, bool showInfo = true, bool isMonsterAtt = false);
    int getH();

private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(CCDictionary *dict, int buildId, bool showInfo, bool isMonsterAtt);
    int m_h;
};

//--

class ScienceInfoCell : public CCNode{
public:
    static ScienceInfoCell *create(std::string sid, int level);
    int getH();

private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(std::string sid, int level);
    int m_h;
};

//--

class AbilityCell : public CCNode{
public:
    static AbilityCell *create(std::string abilityId);
    int getH();

private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(std::string abilityId);
    int m_h;
};

//--

class TitleCell : public CCNode
,public CCBMemberVariableAssigner

{
public:
    static TitleCell *create(std::string title1, std::string title2 = ""){
        TitleCell *ret = new TitleCell();
        if(ret && ret->init(title1, title2)){
            ret->autorelease();
        }else{
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    void setBGHeight(int h);
    int getBGHeight();
    void hideBG();
    void hideDecoration();
private:
    virtual bool init(std::string title1, std::string title2);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

    CCSafeObject<CCLabelIF> m_title1;
    CCSafeObject<CCLabelIF> m_title2;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_decoration;
};
#endif /* defined(__IF__NewEnemyInfoMoreView__) */
