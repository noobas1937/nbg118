//
//  ArcPopupBaseView.h
//  IF
//
//  Created by fubin on 14-5-14.
//
//

#ifndef IF_ArcPopupBaseView_h
#define IF_ArcPopupBaseView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"

#define TYPE_POS_DEF 0
#define TYPE_POS_UP  1
#define TYPE_POS_MID  2
#define TYPE_POS_TO_UP  3
#define TYPE_POS_CLICK  4
#define TYPE_POS_MID_UP  5

class ArcPopupBaseView : public PopupBaseView
{
public:
    bool init(int type=TYPE_POS_DEF);
private:
    void showArcModel(int type);
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
};

#endif
