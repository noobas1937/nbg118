#ifndef __TiledFourCell_H_
#define __TiledFourCell_H_

#include "cocos2d.h"
USING_NS_CC;

class TiledFourCell : public CCNode {
public:
    
	CREATE_FUNC(TiledFourCell);
    
	virtual bool init();
    
	void setiLeftTop(int _iLeftTop);
    
	int getiLeftTop();
    
	void setiLeftBottom(int _iLeftBottom);
    
	int getiLeftBottom();
    
	void setiRightTop(int _iRightTop);
    
	int getiRightTop();
    
	void setiRightBottom(int _iRightBottom);
    
	int getiRightBottom();
    
	/* 获取顶点值的总和 */
    
	int getiTotalNum();
    
private:
    
	int iLeftTop; // 左上角
    
	int iLeftBottom;// 左下角
    
	int iRightTop; // 右上角
    
	int iRightBottom; // 右下角
    
};


#endif