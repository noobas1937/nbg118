//
//  CheckBox.h
//  IF
//
//  Created by 李锐奇 on 13-10-10.
//
//

#ifndef IF_CheckBox_h
#define IF_CheckBox_h

#include "cocos2d.h"
#include "CommonInclude.h"

USING_NS_CC;
using namespace std;

#define CHECK_BOX_SELECT "checkBoxSelect"
#define CHECK_BOX_UNSELECT "checkBoxUnselect"

class CheckBox : public CCNode{
public:
    static CheckBox* create(bool state = false,string bgStr="Check box_01.png");
    CheckBox(){};
    bool isSelected();
    void setSelect(bool b);
    void setCanSelect(bool b);
    void setBtnTouchPriority(int p);
private:
    CheckBox(bool state,string bgStr) : m_select(state), m_bgStr(bgStr),m_canSelect(true){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    void onBtnClick(CCObject* pSender, CCControlEvent evt);
    
    bool m_select;
    bool m_canSelect;
    string m_bgStr;
    CCSafeObject<CCControlButton> m_Btn;
    CCSafeObject<CCSprite> m_selectIcon;
};

#endif
