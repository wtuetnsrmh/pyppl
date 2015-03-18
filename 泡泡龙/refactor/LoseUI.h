#ifndef __LoseUI_H__
#define __LoseUI_H__

#include "cocos2d.h"

#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;
class LoseUI :
	public Layer
{
public:
	LoseUI(void);
	~LoseUI(void);
	virtual bool init();  
	CREATE_FUNC(LoseUI);  
	void touchEvent(Sprite*psender,TouchEventType type);
	void setLoseData();
    const char* cutScreen();
    void gotoShare();
    void gotoShareDelay();
private:
	Text *_score;
	Text *_bubbleEliminate;
	Text *_bubbleDrop;
	Text *_baseScore;
	Text *_coin;
	Text *_stage;
    const char *_sharePicPath;
    LayerColor *lc ;
};

#endif

