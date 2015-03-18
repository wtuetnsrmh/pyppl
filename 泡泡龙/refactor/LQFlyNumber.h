#ifndef __LQFlyNumber_H__
#define __LQFlyNumber_H__
#include "cocos2d.h"
USING_NS_CC;

//定义飘字类型  
typedef enum{  
    klq_flaytype_normal=0,   //正常  
    klq_flaytype_break =1,   //暴击  放大字体，红色  
    klq_flaytype_resolve=2,  //分解  缩小字体，黄色  
    klq_flaytype_other=3     //???  
} LQFlyNumberTypeEnum;  
class LQFlyNumber :
	public Sprite
{
public:
	static LQFlyNumber* create(int num, bool isSign);  //数字 带符号  
   // static LQFlyNumber* create(const char* str);  
    LQFlyNumber();  
    ~LQFlyNumber();  

      
    bool initWithString(int i, bool isSign);  
    void startNumberAnimation();  
    void removeObject();  
      
    void setCurValueStr(int num, bool isSign);  
      
    //飘字类型  
    CC_SYNTHESIZE(LQFlyNumberTypeEnum, m_numbertype, NumberType);  
      
    //用于允许 "static create()" constructor ( node() deprecated )  
    CREATE_FUNC(LQFlyNumber); 
private:  
    const char* m_curValueStr; //当前串值  
	Sprite *_numberPicture;
	Sprite *_plusPicture;
    //LabelBMFont *m_label;  
};
#endif
