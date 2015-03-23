--
-- Author: bbjxl
-- Date: 2015-03-18 21:25:53
--
local json = require("framework.json")
local BublleSprite = import(".BublleSprite")
local BublleBasic = import("..modle.BublleBasic")
local EditBubble = import(".EditBubble")

local EditMapLayer = class("EditMapLayer", function()
    return display.newLayer("EditMapLayer")
end)

function EditMapLayer:ctor(params)
	self.mapEditScene = params.mapEditScene
	self.name = params.name or ""
	self.op = params.op
	self.maxRow = tonumber(params.maxRow) or 10
	self.maxColor = tonumber(params.maxColor) or 3
	self.starlevel = params.starlevel or "100,200,300"
	self.queue = params.queue or 25

	if self.op == 2 then
		-- 编辑地图
		if not self:initData() then return end
	else
		-- 新建地图
		self.m_board = {}
	end
	
    self:initUI()
end

-- 找到当前最大行数
function EditMapLayer:findMaxRow()
	local maxRow = 0
	for _,bubble in pairs(self.m_listBubble) do
		if bubble:getRow() > maxRow then
			maxRow = bubble:getRow()
		end
	end
	return maxRow
end

function EditMapLayer:onEnterFrame(dt)
    
end

-- 编辑地图时要先加载地图数据
function EditMapLayer:initData()
    self.m_board = {} --整个面板所有能放的泡泡 -- pairs

    self.m_listBubble = {}       -- 当前图上所有的球 -- pairs
    print("map name:",string.format("mapData/%s.json",self.name))
    local path = string.format("mapData/%s.json",self.name)
    if device.platform == "windows" then
        -- win下可写入路径跟MAC不同
        path = cc.FileUtils:getInstance():getWritablePath() .."res/" .. path
    end

    local jsonStr = cc.HelperFunc:getFileData(path)
    local jsonObj = json.decode(jsonStr)
    if not jsonObj or table.nums(jsonObj) == 0 then
    	print("地图加载出错，确认输入的名字是否正确")
    	return false
    end

    self.graph = jsonObj.level.graph
    print("球的个数：",table.nums(self.graph))
    -- 重载当前关卡的最大行列数(暂时未用)
    self.maxRow = tonumber(jsonObj.level.maxRow)
    self.maxColor = tonumber(jsonObj.level.maxRandomColors)
    self:initBoard()

    return true
end

-- 如果是编辑地图，则根据加载的地图信息先生成之前地图
function EditMapLayer:initBoard()
    for i,bublleData in ipairs(self.graph) do 
    	bublleData.randomFlag = true -- 不随机显示颜色为6的球
    	self:creatOneEditBublle(bublleData)
    end

end

-- 测试用的格子
function EditMapLayer:initTestBar()
    for i = 0,MAX_COLS-1 do
        for j = 0,self.maxRow - 1 do
            if not (j%2 == 1 and i == MAX_COLS-1) then
                local bar = display.newSprite("#test_bar.png")
                local rcLable =cc.ui.UILabel.new({text = j.."_"..i,size = 20 }):align(display.CENTER, 32, 32):addTo(bar)
                local tempPoint = getPosByRowAndCol(j,i)
                bar:pos(tempPoint.x,tempPoint.y):addTo(self)

                bar:setTouchEnabled(true)
                bar:setTouchSwallowEnabled(false)
                local prevY
                bar:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
                	if event.name == "began" then
                		prevY = event.y
                		return true
                	elseif event.name == "ended" then
                		if math.abs(event.y - prevY) < BUBBLE_RADIUS then
                			self:popEditBubblePanel(j,i)
                		end
                	end
                	return true
                end)
            end
        end
    end
end

-- 弹出要编辑的球属性面板
function EditMapLayer:popEditBubblePanel(row,col)
	local bublleData = {}
	bublleData.row = row
	bublleData.col = col

	--如果点击的球已经有，则先取出数据
	local existBubble = self.m_board[row .. col]
	if existBubble then
		bublleData.color = existBubble:getColor()
		bublleData.type = existBubble:getType()
	else
		bublleData.color = 1
		bublleData.type = 1
	end

	if self.popPropertyLayer then
		self.popPropertyLayer:removeAllChildren()
		self.popPropertyLayer:removeSelf()
		self.popPropertyLayer = nil
	end
	self.popPropertyLayer = display.newLayer()
		:addTo(self,3)
	local lZeroP = self:convertToNodeSpace(cc.p(0,0))
	self.popPropertyLayer:pos(lZeroP.x,lZeroP.y)

	local maskSp = display.newSprite():pos(0,0):addTo(self.popPropertyLayer)
	local shape4 = display.newRect(cc.rect(0, 0, display.width, display.height),
        {fillColor = cc.c4f(0.5,0,0,0.5), borderColor = cc.c4f(0,1,0,0), borderWidth = 1})
		:addTo(maskSp)
	maskSp:setTouchEnabled(true)
	maskSp:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
    	return true
    end)

	-- UI 颜色
	local initX,initY = 200,1000
	display.newTTFLabel({text = string.format("颜色(1-%d)： ",self.maxColor)})
		:pos(initX,initY):addTo(self.popPropertyLayer)
	local editBox2 = cc.ui.UIInput.new({
        image = "#test_bar.png",
        size = cc.size(100, 50),
        x = initX+150,
        y = initY,
        listener = function(event, editbox)
            if event == "began" then
                
            elseif event == "ended" then
                local s = editbox:getText()
                if tonumber(s) <= self.maxColor then
                	bublleData.color = tonumber(s)
                else
                	print("出错：输入的颜色值比设置的最大颜色值大")
                end
                
                print("输入的颜色：",s)
            elseif event == "return" then
                -- self:onEditBoxReturn(editbox)
            elseif event == "changed" then
                -- self:onEditBoxChanged(editbox)
            else
                print("EditBox event %s", tostring(event))
            end
        end
    })
    editBox2:setText(bublleData.color)
    editBox2:setReturnType(cc.KEYBOARD_RETURNTYPE_SEND)
    self.popPropertyLayer:addChild(editBox2)

    -- 类型
    display.newTTFLabel({text = string.format("类型(1-%d)： ",table.nums(BUBBLE_TYPE))})
    	:pos(initX,initY-200):addTo(self.popPropertyLayer)
	local editBox2 = cc.ui.UIInput.new({
        image = "#test_bar.png",
        size = cc.size(100, 50),
        x = initX+150,
        y = initY-200,
        listener = function(event, editbox)
            if event == "began" then
                
            elseif event == "ended" then
                local s = editbox:getText()
                if tonumber(s) <= table.nums(BUBBLE_TYPE) then
                	bublleData.type = tonumber(s)
                else
                	print("出错：输入的类型值比设置的最大颜色值大")
                end
                
                print("输入的类型：",s)
            elseif event == "return" then
                -- self:onEditBoxReturn(editbox)
            elseif event == "changed" then
                -- self:onEditBoxChanged(editbox)
            else
                print("EditBox event %s", tostring(event))
            end
        end
    })
    editBox2:setText(bublleData.type)
    editBox2:setReturnType(cc.KEYBOARD_RETURNTYPE_SEND)
    self.popPropertyLayer:addChild(editBox2)

    PYButton("#PlayButton.png","确 认",function(event)
    		self.popPropertyLayer:removeSelf()
    		self.popPropertyLayer = nil
    		-- 删除之前的球
    		if existBubble then
				existBubble:removeSelf()
				self.m_board[bublleData.row .. bublleData.col] = nil
			end

	        self:creatOneEditBublle(bublleData)
		end):pos(display.cx+100,300):addTo(self.popPropertyLayer)
    PYButton("#PlayButton.png","删 除",function(event)
    		self.popPropertyLayer:removeSelf()
    		self.popPropertyLayer = nil
    		local existB = self.m_board[bublleData.row .. bublleData.col]
    		if existB then
    			existB:removeSelf()
    			self.m_board[bublleData.row .. bublleData.col] = nil
    		end
		end):pos(display.cx-100,300):addTo(self.popPropertyLayer)
end


function EditMapLayer:creatOneEditBublle(bublleData)
	bublleData.randomFlag = true -- 不随机显示颜色为6的球
	local editBubble = EditBubble.new(BublleBasic.new(bublleData))
	editBubble:setTouchEnabled(false)
	local tempPoint = getPosByRowAndCol(editBubble:getRow(),editBubble:getCol())
    editBubble:pos(tempPoint.x,tempPoint.y):addTo(self)
    self.m_board[editBubble:getRow() .. editBubble:getCol()] = editBubble

    print("当前球的数量：",table.nums(self.m_board))
end

function EditMapLayer:initUI()
    -- self:initBoard()
    self:initTestBar()
end

function EditMapLayer:hasBall(row, col)
    if self.m_board[ row .. col ] then return true end
    return false
end

--保存地图数据
function EditMapLayer:saveMap(name)
	local mapData = {}
	mapData.level = {} 
	mapData.level.maxCol = 11
	mapData.level.maxRow = self.maxRow
	mapData.level.maxRandomColors = self.maxColor
	mapData.level.mode = "classic"
	mapData.level.starlevel = self.starlevel
	mapData.level.queue = self.queue
	mapData.level.graph = {}

	for k,editBubble in pairs(self.m_board) do
		local bD = {}
		bD.color = editBubble:getColor()
		bD.col = editBubble:getCol()
		bD.row = editBubble:getRow()
		bD.type = editBubble:getType()
		bD.attributes = {}

		table.insert(mapData.level.graph,bD)
	end
	local path = cc.FileUtils:getInstance():getWritablePath() .."res/mapData/"
	print("path",path)
	io.writefile(path.. name .. ".json", json.encode(mapData))

	dump(mapData)
end



return EditMapLayer