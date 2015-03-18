#ifndef __AlertDialog_H__
#define __AlertDialog_H__

#include "cocos2d.h"
using namespace cocos2d;
class AlertDialog :
	public Layer
{
public:
	AlertDialog(void);
	~AlertDialog(void);
	virtual bool init();  
	CREATE_FUNC(AlertDialog);  

	void setText(int type);

	void removeMe(Node *obj);
private:
	Sprite *_background;
};
#endif // !__AlertDialog_H__
