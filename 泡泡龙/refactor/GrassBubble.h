#ifndef __Grass_Bubble_H__
#define __Grass_Bubble_H__
#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class GrassBubble :
	public Sprite
{
public:
	GrassBubble(void);
	virtual ~GrassBubble(void);
	CREATE_FUNC(GrassBubble);

	GrassBubble_Type getBubbleType();
	void setBubbleType(GrassBubble_Type type);
	
	static GrassBubble *initWithType(GrassBubble_Type type); 
	static std::string getStringByType(GrassBubble_Type type);


	void playEffect(int type, float delay=0.08f);
	void playEffectOnce(int type, float delay);

private:
	GrassBubble_Type _bubbleType;
    Sprite *_effectSprite;
};

#endif