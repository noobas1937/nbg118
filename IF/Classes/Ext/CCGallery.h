//
//  CCGallery.h
//  CrazyBirdTD
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef CrazyBirdTD_CCGallery_h
#define CrazyBirdTD_CCGallery_h

#include "cocos2d.h"

USING_NS_CC;

class CCGalleryItem;
class CCGallery;

class CCGalleryDelegate
{
public:
	CCGalleryDelegate() {}
	virtual ~CCGalleryDelegate() {}
	
	virtual void slideBegan(CCGallery *gallery) = 0;
	virtual void slideEnded(CCGallery *gallery, CCGalleryItem *pGItem) = 0;
	virtual void selectionChanged(CCGallery *gallery, CCGalleryItem *pGItem) = 0;
	virtual void selectionDecided(CCGallery *gallery, CCGalleryItem *pGItem) = 0;
};

typedef enum {
	kCCGalleryDirectionHorizontal,
	kCCGalleryDirectionVertical
} CCGalleryDirection;

typedef enum {
	kCCGalleryEventEnded,
	kCCGalleryEventDecided
} CCGalleryEvent;

typedef enum {
    kCCGalleryCycleModeCircular,
    kCCGalleryCycleModeNotCircular
} CCGalleryCycleMode;

typedef enum {
	kDragDirLeft,
	kDragDirRight,
	kDragDirUp,
	kDragDirDown,
	kDragDirCenter
}eDragDirs;

class CCGalleryItem : public CCNode {
private:
	CCGalleryItem()
	{}
	
public:
	virtual ~CCGalleryItem()
	{}
	
	static CCGalleryItem* create(unsigned int uIdx)
	{
		CCGalleryItem *pRet = new CCGalleryItem();
		if (pRet) {
			pRet->setIdx(uIdx);
            pRet->setTag(uIdx);
			pRet->autorelease();
			return pRet;
		}
		
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
	
	unsigned int getIdx() { return m_uIdx; }
    void setIdx(unsigned int uIdx) { m_uIdx = uIdx; }
	
private:
	unsigned int m_uIdx;
};

/*
 用函数setCenter设置中心点所在的位置
 判断是否触摸到区域用 WorldPosition
 点内部元素用 NodePosition
 */
class CCGallery : public Layer {
public:
	CCGallery(void);
	virtual ~CCGallery(void);
	
	static CCGallery* create(Size itemSize, Size viewSize);
	virtual bool init(Size itemSize, Size viewSize);
	
	virtual void onEnter();
	virtual void onExit();
	
	virtual bool onTouchBegan(Touch *pTouch, CCEvent *pEvent);
	virtual void onTouchMoved(Touch *pTouch, CCEvent *pEvent);
	virtual void onTouchEnded(Touch *pTouch, CCEvent *pEvent);
	
	virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
	virtual void addChild(Node * child);
	virtual void addChildFinish();
	
	void setTargetIndexItem(int idx, bool animated = false);
	
	CCGalleryItem* itemAtIndex(int idx);
	
	void setDelegate(CCGalleryDelegate *delegate);
	
	void setDirection(int dir);
    void setCycleMode(int mode);
	void setItemSpace(float space);
	void setFrontScale(float scale);
	void setBackScale(float scale);
	
	void setEnabled(bool isEnabled) { m_bEnabled = isEnabled; };
	bool getEnabled() { return m_bEnabled; };
	
	void registerGalleryScriptHandler(int nFunID);
	void unregisterGalleryScriptHandler();
	int getGalleryScriptHandler();
	
protected:
	void updateItemsPos(void);
	bool updateItemStatus(CCGalleryItem *pGItem);
	void update(float delta);
	
	CCGalleryItem* touchGalleryItems(const Vec2& touchPoint);
	
	Rect getViewRect();
	
protected:
	bool m_bEnabled;
	
	CCGalleryDelegate *m_pDelegate;
	
	int m_nGalleryScriptHandler;
	
	// 动态矫正位置结束
	bool m_bAutoMoveDest;
	
	// 是否拖拽
	bool m_bIsDragging;
	// 栏目数量
	int m_nNumItems;
	// 滚动方向
	int m_nDirection;
    // 循环模式 是否是循环的
    int m_nCycleMode;
	// 栏目间隔
	float m_fInterspace;
	float m_fFrontScale;
	float m_fBackScale;
	Vec2 m_tCenter;
	
	// 上次触摸位置
	Vec2 m_tPreTouchPoint;
	// 当前移动的偏移量
	Vec2 m_tMoveOffset;
	Vec2 m_tMoveDistance;
	float m_fAutoMoveDis;
	float m_fAutoMoveOffset;
	// 拖动方向
	int m_nDragDirection;
	// 拖动两个方向上临界距离
	float m_fLeftTopDis;
	float m_fRightDownDis;
	CCArray *m_pItemsArray;
	
	// 当前选中栏目索引
	int m_nSelectedItemIdx;
	
	// 栏目Size
	Size m_tItemSize;
	// 显示Size
	Size m_tViewSize;
    
};

#endif
