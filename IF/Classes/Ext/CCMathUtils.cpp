#include "CCMathUtils.h"

using namespace cocos2d;

/**
 *	@return [min, max]
 */
#define MAX_RAND_FLOAT_NUM 200
static const float RAND_FLOATS[MAX_RAND_FLOAT_NUM]={0.264345,0.850785,0.138896,0.427164,0.353874,0.562798,0.949151,0.385279,0.378017,0.329568,0.056227,0.013966,0.734634,0.993767,0.240267,0.172952,0.805049,0.456191,0.200662,0.533711,0.089419,0.873059,0.507713,0.131163,0.461295,0.981763,0.486024,0.602261,0.192959,0.067015,0.328974,0.059340,0.324886,0.366013,0.586157,0.533835,0.159643,0.117241,0.471935,0.805410,0.518168,0.846771,0.680860,0.218302,0.998935,0.097157,0.915009,0.558663,0.447275,0.346118,0.204397,0.299276,0.923570,0.433760,0.196457,0.847236,0.490645,0.277872,0.193452,0.349451,0.218645,0.770475,0.375947,0.548651,0.181150,0.580970,0.360157,0.162535,0.727001,0.699461,0.840485,0.036497,0.411984,0.223205,0.411636,0.369292,0.697625,0.989634,0.782500,0.480765,0.223025,0.378042,0.744155,0.007479,0.694699,0.808072,0.272634,0.165245,0.267407,0.305696,0.824697,0.687439,0.789262,0.132897,0.600339,0.896124,0.154356,0.269636,0.778085,0.282042,0.279528,0.026807,0.551540,0.739865,0.919042,0.337322,0.364704,0.584292,0.189993,0.208833,0.851172,0.641294,0.230763,0.434924,0.769204,0.018545,0.683974,0.550596,0.860014,0.247681,0.782087,0.532679,0.737005,0.840709,0.788686,0.442216,0.327220,0.585734,0.433158,0.081367,0.542941,0.214844,0.877319,0.103433,0.395550,0.015999,0.892400,0.567077,0.866300,0.906625,0.643303,0.001124,0.887721,0.931789,0.570687,0.543977,0.615805,0.838570,0.842274,0.091266,0.905247,0.492261,0.437698,0.385032,0.232248,0.395509,0.316165,0.783384,0.339167,0.371504,0.860255,0.312862,0.271542,0.804739,0.247179,0.331932,0.788208,0.403815,0.917320,0.404150,0.551860,0.113959,0.314337,0.065887,0.357541,0.199369,0.799673,0.100226,0.491980,0.707808,0.128154,0.888964,0.812330,0.837933,0.142249,0.784223,0.443270,0.039499,0.857121,0.640505,0.962031,0.847360,0.573219,0.088683,0.502872,0.773605,0.982219,0.153079,0.807188,0.413216};
int CCMathUtils::randCounter = 0;

int CCMathUtils::getRandomInt(int min, int max)
{
	//均匀
	float result=getRandom(min, max+1);
	if(result == (max+1)){
		return max;
	}else{
		return floor(result);
	}
	
	//均匀
    //	if((max - min)==0){
    //		return 0;
    //	}else{
    //		return rand() % (max - min) + min;
    //	}
	
	//不均匀
    //	return floor(getRandom(min, max));
}

int CCMathUtils::getRandomIntWithKey(int min, int max,int key){
    float randF = getRandomWithKey(min, max, key);
    if (randF==max+1) {
        return max;
    }else{
        return floor(randF);
    }
}

/**
 *	@return [min, max]
 */
float CCMathUtils::getRandom(float min, float max)
{
    ++randCounter;
    if (randCounter==MAX_RAND_FLOAT_NUM) {
        randCounter=0;
    }
	return min+(max-min)*RAND_FLOATS[randCounter];
}

float CCMathUtils::getRandomWithKey(float min, float max,int key){
    return min+(max - min)*RAND_FLOATS[key%MAX_RAND_FLOAT_NUM];
}

int CCMathUtils::getRandomPlusOrMinus()
{
    ++randCounter;
    if (randCounter==MAX_RAND_FLOAT_NUM) {
        randCounter=0;
    }
	return RAND_FLOATS[randCounter] >= 0.5 ? 1 : -1;
}

float CCMathUtils::getAngle(CCPoint from, CCPoint to)
{
	float result = atan2(to.y-from.y, to.x-from.x) * 180 / PI;
	return result;
}

int CCMathUtils::getCurrentTime(){
    struct timeval now;
    
    gettimeofday(&now, nullptr);
//    struct cc_timeval now;
//    CCTime::gettimeofdayCocos2d(&now, NULL);
    return (now.tv_sec * 1000 + now.tv_usec / 1000);
}