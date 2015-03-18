#ifndef __Shake_H__
#define __Shake_H__
#include "cocos2d.h"  
USING_NS_CC;  

class Shake : public cocos2d::ActionInterval  
{  
public:  
	Shake();  

	//下面给出了两个创建该抖动动作实例的类方法：  

	//①参数指定动作执行时间和抖动范围（x和y相同）  
	// Create the action with a time and a strength (same in x and y)  
	static Shake *create(float d, float strength );  

	//②参数指定动作执行时间和x，y抖动范围  
	// Create the action with a time and strengths (different in x and y)  
	static Shake *createWithStrength(float d, float strength_x, float strength_y );  

	bool initWithDuration(float d, float strength_x, float strength_y );  


	virtual Shake* clone() const override;
	virtual Shake* reverse(void) const  override;

protected:  

	void startWithTarget(Node *pTarget);  
	void update(float time);  
	void stop(void);
cocos2d::Point  m_StartPosition;  

	// Strength of the action  
	float m_strength_x, m_strength_y;  
};  

#endif