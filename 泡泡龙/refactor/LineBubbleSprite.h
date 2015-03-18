#ifndef __Line_Bubble_Sprite__
#define __Line_Bubble_Sprite__
#include "cocos2d.h"
using namespace cocos2d;

class LineBubbleSprite :public Sprite
{
public:
	LineBubbleSprite();
	~LineBubbleSprite();
	CREATE_FUNC(LineBubbleSprite);
	static LineBubbleSprite *initWithPic(std::string pic);

	CC_SYNTHESIZE(int, _xRefraction, XRefraction);
	CC_SYNTHESIZE(int, _yRefraction, YRefraction);

	CC_SYNTHESIZE(cocos2d::Point, _originalPoint, OriginalPoint);

	void setOriginalPointY(float y);
private:
	
};

#endif // !__Line_Bubble_Sprite__
