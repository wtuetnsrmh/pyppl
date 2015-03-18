#ifndef __FireLayer_H__
#define __FireLayer_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;
using namespace cocos2d;


class FireLayer :
	public Layer
{
public:
	FireLayer(void);
	~FireLayer(void);

	CREATE_FUNC(FireLayer);
	virtual bool init();  
};
#endif