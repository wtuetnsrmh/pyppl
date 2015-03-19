--
-- Author: yzm
-- Date: 2015-03-10 13:43:54
--
local json = require("framework.json")

local SelectScene = class("SelectScene", function()
    return display.newScene("SelectScene")
end)

function SelectScene:ctor()
	local rootLayer = display.newLayer()
	local tb = self:createListView()
    -- self:createTableView()
    self.lv:setBounceable(false)
    self.lv:scrollTo(0,0)
	rootLayer:addChild(self.lv)

    rootLayer:addTo(self)
end

function SelectScene:createListView()
	local winSize = cc.Director:getInstance():getWinSize()
    self.lv = cc.ui.UIListView.new {
        -- bgColor = cc.c4b(200, 200, 200, 120),
        -- bg = "sunset.png",
        -- bgScale9 = true,
        viewRect = cc.rect(0, 0, winSize.width, winSize.height),
        direction = cc.ui.UIScrollView.DIRECTION_VERTICAL,
        -- scrollbarImgV = "bar.png"
        }
        -- :onTouch(handler(self, self.touchListener))
        

    -- add items
    for i = 21,1,-1 do
        local item = self.lv:newItem()

        local content = self:createMapCell(i)
        
        item:addContent(content)
        item:setItemSize(512, 512)

        self.lv:addItem(item)
    end
    self.lv:reload()
end

function SelectScene:createMapCell(index)
    local content = cc.Sprite:create(string.format("scenes/sagamap/episode%03d.jpg",index))
    content:setAnchorPoint(cc.p(0,0))
    content:setPosition(cc.p(0, 0))

    for i = 1,10 do
        local passEnterBtn = cc.ui.UIPushButton.new("#pass_enter_btn.png")
        :setButtonSize(67,62)
        :setButtonLabel("normal",cc.ui.UILabel.new({
            UILabelType = 2,
            text = tostring(i),
            size = 30
            }))
        :onButtonClicked(function(event)
            switchScene("PlayScene")
        end)
        :align(display.CENTER,10 + ((i-1)%5) * 70,50 + ((i-1)%2) * 70):addTo(content)
        passEnterBtn:setTouchSwallowEnabled(false)

    end

    return content
end

function SelectScene:touchListener(event)
    -- print("touchListener")
    local listView = event.listView
    -- if "clicked" == event.name then
    --     if 3 == event.itemPos then
    --         listView:removeItem(event.item, true)
    --     else
    --         -- event.item:setItemSize(120, 80)
    --     end
    -- elseif "moved" == event.name then
    --     self.bListViewMove = true
    -- elseif "ended" == event.name then
    --     self.bListViewMove = false
    -- else
    --     print("event name:" .. event.name)
    -- end
end


--[[
function SelectScene.tableCellAtIndex(table, idx)
    local strValue = string.format("%d",idx)
    local cell = table:dequeueCell()
    local label,subMap = nil
    if nil == cell then
        cell = cc.TableViewCell:new()
    else
        cell:removeAllChildren()
    end
    SelectScene.createMapCell(cell,idx)

    return cell
end

function SelectScene.createMapCell(cellNode, index)
    print(string.format("scenes/sagamap/episode%03d.jpg",index))

    local sprite = cc.Sprite:create(string.format("scenes/sagamap/episode%03d.jpg",index))
    sprite:setAnchorPoint(cc.p(0,0))
    sprite:setPosition(cc.p(0, 0))
    cellNode:addChild(sprite)

    label = cc.Label:createWithSystemFont(strValue, "Helvetica", 20.0)
    label:setPosition(cc.p(0,0))
    label:setAnchorPoint(cc.p(0,0))
    label:setTag(123)
    cellNode:addChild(label)

    
end
function SelectScene:createTableView()

    local winSize = cc.Director:getInstance():getWinSize()

    -- local tableView = cc.TableView:create(cc.size(600,60))
    -- tableView:setDirection(cc.SCROLLVIEW_DIRECTION_HORIZONTAL)
    -- tableView:setPosition(cc.p(20, winSize.height / 2 - 150))
    -- tableView:setDelegate()
    -- self:addChild(tableView)
    -- --registerScriptHandler functions must be before the reloadData funtion
    -- tableView:registerScriptHandler(SelectScene.numberOfCellsInTableView,cc.NUMBER_OF_CELLS_IN_TABLEVIEW)  
    -- tableView:registerScriptHandler(SelectScene.scrollViewDidScroll,cc.SCROLLVIEW_SCRIPT_SCROLL)
    -- tableView:registerScriptHandler(SelectScene.scrollViewDidZoom,cc.SCROLLVIEW_SCRIPT_ZOOM)
    -- tableView:registerScriptHandler(SelectScene.tableCellTouched,cc.TABLECELL_TOUCHED)
    -- tableView:registerScriptHandler(SelectScene.cellSizeForTable,cc.TABLECELL_SIZE_FOR_INDEX)
    -- tableView:registerScriptHandler(SelectScene.tableCellAtIndex,cc.TABLECELL_SIZE_AT_INDEX)
    -- tableView:reloadData()

    tableView = cc.TableView:create(winSize)
    tableView:setDirection(cc.SCROLLVIEW_DIRECTION_VERTICAL)
    tableView:setPosition(cc.p(0,0))
    tableView:setDelegate()
    tableView:setVerticalFillOrder(cc.TABLEVIEW_FILL_BOTTOMUP)
    
    -- tableView:registerScriptHandler(SelectScene.scrollViewDidScroll,cc.SCROLLVIEW_SCRIPT_SCROLL)
    -- tableView:registerScriptHandler(SelectScene.scrollViewDidZoom,cc.SCROLLVIEW_SCRIPT_ZOOM)
    -- tableView:registerScriptHandler(SelectScene.tableCellTouched,cc.TABLECELL_TOUCHED)
    tableView:registerScriptHandler(SelectScene.cellSizeForTable,cc.TABLECELL_SIZE_FOR_INDEX)
    tableView:registerScriptHandler(SelectScene.tableCellAtIndex,cc.TABLECELL_SIZE_AT_INDEX)
    tableView:registerScriptHandler(SelectScene.numberOfCellsInTableView,cc.NUMBER_OF_CELLS_IN_TABLEVIEW)
    tableView:reloadData()

    return tableView
end
function SelectScene:test()
   print("test")
end

function SelectScene.numberOfCellsInTableView(table)
   return 21
end

function SelectScene.scrollViewDidScroll(view)
    print("scrollViewDidScroll")
end

function SelectScene.scrollViewDidZoom(view)
    print("scrollViewDidZoom")
end

function SelectScene.tableCellTouched(table,cell)
    print("cell touched at index: " .. cell:getIdx())
end

function SelectScene.cellSizeForTable(table,idx) 
    print("idx",idx)
    if idx == 0 then
        return 64,512 -- height,width
    end
    return 512,512
end
]]


function SelectScene:onEnter()
end

function SelectScene:onExit()
end

return SelectScene