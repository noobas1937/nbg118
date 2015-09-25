#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include "cocos2d.h"
USING_NS_CC;

#define PI 3.14159265
#define CCRANDOM_IN(left,right) left+(right-left)*CCRANDOM_0_1()
class CCMathUtils
{
public:
    static int randCounter;
	static int getRandomInt(int min, int max);
    static int getRandomIntWithKey(int min, int max,int key);
	static float getRandom(float min=0, float max=1.0);
    static float getRandomWithKey(float min, float max,int key);
	static int getRandomPlusOrMinus();
	static float getAngle(CCPoint from, CCPoint to);
    static int getCurrentTime();//取得的是毫秒
};

class CircleDoubleRect
{
public:
	CircleDoubleRect(float originX,float originY,float fR)
	{
		//out circle
		mOutQuad.x = originX - fR;
		mOutQuad.y = originY - fR;
		mOutQuad.r = originX + fR;
		mOutQuad.b = originY + fR;
        
		//in circle
		float f2 = 0.707*fR;
		mInQuad.x = originX - f2;
		mInQuad.y = originY - f2;
		mInQuad.r = originX + f2;
		mInQuad.b = originY + f2;
        
		fR2  = fR*fR;
	}
    
	inline bool canReach(CCPoint& point)
	{
		bool bAttack = false;
        
		if (inRect(point,mOutQuad))
		{
			if (inRect(point,mInQuad))
			{
				bAttack = true;
			}else
			{
				if(point.getDistanceSq(point) <= fR2)
					bAttack = true;
			}
		}
        
		return bAttack;
	}
    
    inline bool inOutQuad(CCPoint& point){
        return inRect(point, mOutQuad);
    }
    inline bool inInQuad(CCPoint& point){
        return inRect(point,mInQuad);
    }
private:
	struct JArea
	{
		float x;
		float y;
		float r;
		float b;
	};
    
	JArea mOutQuad;
	JArea mInQuad;
	float fR2;
    
	inline bool inRect(CCPoint& point,JArea& target)
	{
		bool bRet = false;
        
		if (point.x >= target.x && point.x <= target.r
			&& point.y >= target.y && point.y <= target.b)
		{
			bRet = true;
		}
        
		return bRet;
	}
};



#endif //__MATH_UTILS_H__
