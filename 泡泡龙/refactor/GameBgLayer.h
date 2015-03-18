#ifndef __GameBgLayer_H__
#define __GameBgLayer_H__

#include "cocos2d.h"
using namespace cocos2d;
class GameBgLayer :
	public Layer
{
public:
	GameBgLayer(void);
	~GameBgLayer(void);
	virtual bool init();  
	CREATE_FUNC(GameBgLayer);  


	void addGameFrame(int x, int y, int width, int height);
};
#endif

