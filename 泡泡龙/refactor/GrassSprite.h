#ifndef __Grass_Sprite_H__
#define __Grass_Sprite_H__
#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

#define GRASS_TAG_BASE 5000
class GrassBubble;
class GrassSprite :
	public Sprite
{
public:
	GrassSprite(void);
	virtual ~GrassSprite(void);
	CREATE_FUNC(GrassSprite);
	
	void setGrassType(int type);//树叶类型 
	int getGrassType();

	static GrassSprite *initWithType(int type);
	static std::string getStringByType(int type);

	void addABubble();
	void removeABubble();

	GrassBubble * getBubble();
	void showEffect(GrassBubble_Type type);
	void changeHp(Node* node);
	void addShizhong(Node *node);
	void restoreBarrier(Node *node);
	void addGem(int count);
	void addGold(int count);

	void removeBubble(Node*node);
	void removeBubbleAction(Node*node);

private:
	GrassBubble *_bubble;
	int _grassType;
	int _hpCount;
};

#endif