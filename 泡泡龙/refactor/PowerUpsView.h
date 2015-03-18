#ifndef __PowerUpsView_H__
#define __PowerUpsView_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;
using namespace cocos2d;

#define POWER_ITEM_TAG 1000
class PowerUpsView :
	public Layer,public TableViewDataSource,public TableViewDelegate
{
public:

	PowerUpsView(void);
	~PowerUpsView(void);
    virtual bool init();
    
public:  
 
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);

	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view);

 
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);

	virtual cocos2d::Size  cellSizeForTable(TableView *table);  

	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);  
 
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	virtual void tableCellHighlight(TableView* table, TableViewCell* cell);  

    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    
    CREATE_FUNC(PowerUpsView);

	void updateItems(int itemIndex);
private:
	TableView *_tableView;
    
};
#endif