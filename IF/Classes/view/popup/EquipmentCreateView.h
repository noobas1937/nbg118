//
//  EquipmentCreateView.h
//  IF
//
//  Created by fubin on 14-12-18.
//
//

#ifndef __IF__EquipmentCreateView__
#define __IF__EquipmentCreateView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "CCClipNode.h"

#define CHANGE_MATE_SEL "change.mate.sel"

class EquipColorCell;

class EquipmentCreateView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
//, public CCTouchDelegate
{
public:
    static EquipmentCreateView* create(int itemId);
    void playClose();
    CCNode* getGuideNode(string _key);
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int itemId);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickEquipCrtBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickGetMore(CCObject * pSender, Control::EventType pCCControlEvent);
    void onHelpBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onCostGoldBack();
    void AnimationFadeIn();
    void AnimationClose();
    void closeView(CCObject* params);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void showParticle();
    void removeParticle();
    
    void addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void showPopParticle(float dt);
    void removePopParticle(float dt);
    void playStartCrtParticle();
    void playCrtEndParticle();
    void flyEnd();
    
    void onCostGoldOtherQueue();
    
    void playCrtSound(float dt);
    void playPutOnSound(float dt);
    void canPlayAdd(float dt);
    
    void resetRectSpr(float dt);
    
    CCSafeObject<CCControlButton> m_helpBtn;
    
    CCSafeObject<CCControlButton> m_equipCrtBtn;
    CCSafeObject<CCControlButton> m_getMoreBtn;
    CCSafeObject<CCNode> m_middleNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_btmMidNode;
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_particleNode;
    
    CCSafeObject<CCNode> m_guide1Node;
    CCSafeObject<CCNode> m_guide2Node;
    
    CCSafeObject<CCNode> m_midBgNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_newEIconNode;
    CCSafeObject<CCLabelIF> m_equipLabel;
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCLabelIF> m_msg3Label;
    CCSafeObject<CCLabelIF> m_msg4Label;
    
    CCSafeObject<CCLabelIF> m_mateLabel1;
    CCSafeObject<CCLabelIF> m_mateLabel2;
    CCSafeObject<CCLabelIF> m_mateLabel3;
    CCSafeObject<CCLabelIF> m_mateLabel4;
    
    CCSafeObject<CCLabelIF> m_costLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    
    CCSafeObject<CCNode> m_colorBarNode;
    CCSafeObject<CCScale9Sprite> m_fireBG;
    CCSafeObject<CCSprite> m_rectSpr;
    
    CCSafeObject<CCNode> m_mateNode;
    CCSafeObject<CCLabelIF> m_mateLabel;
    CCSafeObject<CCScale9Sprite> m_resEquipBG;
    CCSafeObject<CCNode> m_resEIconNode;
    CCSafeObject<CCScale9Sprite> m_mate1BG;
    CCSafeObject<CCScale9Sprite> m_mate2BG;
    CCSafeObject<CCScale9Sprite> m_mate3BG;
    CCSafeObject<CCScale9Sprite> m_mate4BG;
    CCSafeObject<CCNode> m_mateNode1;
    CCSafeObject<CCNode> m_mateNode2;
    CCSafeObject<CCNode> m_mateNode3;
    CCSafeObject<CCNode> m_mateNode4;
    
    CCSafeObject<CCSprite> m_noEquip;
    CCSafeObject<CCNode> m_equipAddSpr;
    CCSafeObject<CCSprite> m_mateSpr1;
    CCSafeObject<CCSprite> m_mateSpr2;
    CCSafeObject<CCSprite> m_mateSpr3;
    CCSafeObject<CCSprite> m_mateSpr4;
    
    CCSafeObject<CCNode> m_selNode;
    CCSafeObject<CCScale9Sprite> m_itemBg;
    CCSafeObject<CCSprite> m_arrowSpr;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_flyNode;
    CCSafeObject<CCLabelIF> m_noMateLabel;
    CCSafeObject<CCLabelIF> m_selTitleLabel;
    CCSafeObject<CCLabelIF> m_selBottomLabel;
    
    CCSafeObject<CCNode> m_costNode;
    CCSafeObject<CCLabelIF> m_resNumLabel;
    CCSafeObject<CCNode> m_resIconNode;
    CCSafeObject<CCNode> m_barNode;
    CCSafeObject<CCLabelIF> m_barLabel;
    CCSafeObject<CCScale9Sprite> m_timeBar;
    
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCLabelIF> m_errorLabel;
    
    CCSafeObject<CCNode> m_popNode;
    CCSafeObject<CCNode> m_par1Node;
    CCSafeObject<CCNode> m_par2Node;
    CCSafeObject<CCNode> m_par3Node;
    CCSafeObject<CCNode> m_par4Node;
    CCSafeObject<CCNode> m_par5Node;
    CCSafeObject<CCNode> m_par6Node;
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    
    CCSafeObject<CCLayer> m_smokeLayer;
    vector<CCParticleBatchNode*> m_parVec;
    
    CCSafeObject<CCLayer> m_popLayer;
    vector<CCParticleBatchNode*> m_parPopVec;
    
    void onGenerateMateData();
    void onShowMates(int type=0);
    
    void onRefreshEquipInfo(int color,bool needRefresh=true);
    void onMakeCrtPro(bool needRefresh=true);
    
    void onOKCreateEquip();
    void onGotoGetRes();
    
    void changeData(int idx);
    void refreshData(float dt);
    void onEnterFrame(float dt);
    void afterRefreshData(CCObject* p);
    void updateCrtRet(CCObject* p);
    void generateData();
    void refreshMateData();
    void refreshEquipData();
    
    void onTipAddEquip();
    
    void showErrorInfo();
    void onYesGoToBuild();
    
    vector<string> m_data;
    
    //材料选择 临时数据
    map<int, int> m_tmpMates;
    map<int, int> m_selMates;
    
    //装备选择 临时数据
    vector<string> m_myEquips;
    map<int, EquipColorCell*> m_colorCellMap;
    string m_tmpEquips;
    
    int m_itemId;
    int m_Type;
    int m_idx;
    
    int m_qid;
    int m_sumTime;
    int m_curTime;
    int _tmpGold;
    bool _isUUid;
    int m_costRes;
    int m_costResType;
    bool m_isEnough;
    int _openNum;
    
    string m_curOptItem;
    
    string _tmpBGPath;
    string _tmpIconPath;
    CCPoint startPt;
    int m_curCreatingItemId;
    vector<float> m_proVec;
    bool m_canPlayAdd;
};

//...cell class
class EquipmentSelCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static EquipmentSelCell* create(string itemId, int type, int num);
    EquipmentSelCell():m_itemId(""),m_type(0){};
    void setData(string itemId, int type, int num);
    void cellTouchEnded(CCTouch* pTouch);
private:
    bool init(string itemId, int type, int num);
    virtual void onEnter();
    virtual void onExit();
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void hideDesNode(CCObject* param);
    
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_picNode;
    string m_itemId;
    int m_type;
    int m_num;
};

//...EquipColorCell class
class EquipColorCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCNode
{
public:
    static EquipColorCell* create(int color);
    EquipColorCell():m_color(0),m_num(0){};
    void setData(int color, float num);
    bool isTouchIn(CCTouch* pTouch);
private:
    bool init(int color);
    virtual void onEnter();
    virtual void onExit();
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_barNode;
    CCSafeObject<CCClipNode> m_clipNode;
    CCSafeObject<CCNode> m_touchNode;
    
    int m_color;
    double m_num;
    CCSize m_barSize;
};

#endif /* defined(__IF__EquipmentCreateView__) */
