//
//  ServerLsitCell.h
//  IF
//
//  Created by wangzhenlei on 13-10-18.
//
//

#ifndef __IF__ServerLsitCell__
#define __IF__ServerLsitCell__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class ServerListCell :
public CCTableViewCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
	~ServerListCell();
    ServerListCell();
    static ServerListCell* create(int idx,ServerInfo serverInfo);
    
    void setInfo(int idx,ServerInfo serverInfo);
protected:
    
    bool init(int idx,ServerInfo serverInfo);
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onBtnClick(CCObject *pSender, CCControlEvent event);

    CCSafeObject<CCLabelIF> m_ipText;
    CCSafeObject<CCLabelIF> m_levelText;
    CCSafeObject<CCControlButton> m_redBtn;
    CCSafeObject<CCControlButton> m_greenBtn;

    int m_index;
    ServerInfo m_serverInfo;
    
};

#endif /* defined(__IF__WeaponCell__) */

