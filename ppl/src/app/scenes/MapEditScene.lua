--
-- Author: Your Name
-- Date: 2015-03-19 10:15:08
--MapEditScene


local json = require("framework.json")
local BublleSprite = import(".BublleSprite")
local BublleBasic = import("..modle.BublleBasic")
local EditMapLayer = import(".EditMapLayer")
local PopInputMapNameLayer = import(".PopInputMapNameLayer")

local MapEditScene = class("MapEditScene", function()
    return display.newScene("MapEditScene")
end)

function MapEditScene:ctor(params)
	self.op = params.op or 1

    display.newSprite("bg.jpg"):pos(display.cx,display.cy):addTo(self)
    self.popLayer = display.newLayer():addTo(self)

    if self.op == 1 then
    	-- new map edit
    	self:popCreateNewMapProLayer()
    	
    else
    	-- edit map
    	self:popInputEditMapNameLayer()
    end

end

--公有的一些操作UI
function MapEditScene:initCommonUI()
	-- 保存文件名输入框
	local saveNameInput = cc.ui.UIInput.new({
        image = "#test_bar.png",
        size = cc.size(250, 50),
        x = 300,
        y = 100,
        listener = function(event, editbox)
            if event == "began" then
                
            elseif event == "ended" then
                local s = editbox:getText()
                print("要保存的文件名：",s)
                
            elseif event == "return" then
                -- self:onEditBoxReturn(editbox)
            elseif event == "changed" then
                -- self:onEditBoxChanged(editbox)
            else
                print("EditBox event %s", tostring(event))
            end
        end
    })
    saveNameInput:setText(self.name)
    saveNameInput:setReturnType(cc.KEYBOARD_RETURNTYPE_SEND)
    self:addChild(saveNameInput)

	local saveBtn = PYButton("#PlayButton.png","保 存",function(event)
		local saveName = saveNameInput:getText()
		if string.len(saveName) > 0 then
			self.editMapLayer:saveMap(saveName)
		else
			print("请输入要保存的文件名")
		end
		
	end):pos(display.width-100,100):addTo(self,2)
	saveBtn:setTouchSwallowEnabled(true)

	-- 返回按钮
	local returnBtn = PYButton("#PlayButton.png","返回主场景",function(event)
		clearMemory()
		switchScene("WelcomeScene")
	end):pos(100,100):addTo(self,2)
	returnBtn:setTouchSwallowEnabled(true)

	-- 放大缩小预览场景
	local SLIDER_IMAGES = {
	    bar = "SliderBar.png",
	    button = "SliderButton.png",
	}
    self:performWithDelay(function()
        cc.ui.UISlider.new(display.LEFT_TO_RIGHT, SLIDER_IMAGES, {scale9 = true, min = 50 })
            :onSliderValueChanged(function(event)
                self.editMapLayer:scale((event.value/100))
            end)
            :setSliderSize(200, 10)
            :setSliderValue(100)
            :align(display.BOTTOM_LEFT, 200,20)
            :addTo(self,2)
        end, 0.1)
	
end

function MapEditScene:perFun()
    self.editMapLayer:retain()
    self.editMapLayer:removeSelf()
    local sv = cc.ui.UIScrollView.new({viewRect = cc.rect(0,150,display.width,display.height - 150 )})
    sv:addScrollNode(self.editMapLayer)
        :onScroll(handler(self, self.scrollListener))
        :setDirection(1) --只支持纵向滑动
        :addTo(self,3)
     self.editMapLayer:release()
    self:initCommonUI()
end

function MapEditScene:initDebugUI()
	self.popLayer:removeAllChildren()

	self.editMapLayer = EditMapLayer.new({mapEditScene = self, op = self.op,maxRow = self.maxRow,maxColor = self.maxColor,
		starlevel = self.starlevel, queue = self.queue, name = self.name }):pos(10,0):addTo(self)

    self:performWithDelay(function()
            self:perFun()
        end, 0.3)
   
end

function MapEditScene:scrollListener(event)
	-- print("TestUIScrollViewScene - scrollListener:" .. event.name)
end

-- 弹出要编辑的地图名字
function MapEditScene:popInputEditMapNameLayer()
	self.popLayer:removeAllChildren()

	local popLayer = PopInputMapNameLayer.new({callBack = function(name)
			self.name = name
			self:initDebugUI()
		end}):addTo(self.popLayer)
end

-- 设置新地图的属性
function MapEditScene:popCreateNewMapProLayer()
	self.popLayer:removeAllChildren()

	display.newColorLayer(cc.c4b(128,128,128,128)):addTo(self.popLayer)
	local maskLayer = display.newLayer():addTo(self.popLayer)
    maskLayer:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self,self.onTouch))

    self.maxColor = 3
	self.maxRow = 10
	self.model = "classic"

	local initX,initY = 200,1000
	display.newTTFLabel({text = "输入行数(50)： "})
		:pos(initX,initY):addTo(self.popLayer)
    local editBox2 = cc.ui.UIInput.new({
        image = "#test_bar.png",
        size = cc.size(100, 50),
        x = initX+150,
        y = initY,
        listener = function(event, editbox)
            if event == "began" then
                
            elseif event == "ended" then
                local s = editbox:getText()
                print("输入的行数：",tonumber(s))
                self.maxRow = tonumber(s) > 50 and 50 or tonumber(s)
            elseif event == "return" then
                -- self:onEditBoxReturn(editbox)
            elseif event == "changed" then
                -- self:onEditBoxChanged(editbox)
            else
                print("EditBox event %s", tostring(event))
            end
        end
    })
    editBox2:setText(self.maxRow)
    editBox2:setReturnType(cc.KEYBOARD_RETURNTYPE_SEND)
    self.popLayer:addChild(editBox2)

    --几种颜色
    display.newTTFLabel({text = "几种颜色(6)： "})
		:pos(initX,initY-100):addTo(self.popLayer)
    local editBoxColor = cc.ui.UIInput.new({
        image = "#test_bar.png",
        size = cc.size(100, 50),
        x = initX+150,
        y = initY-100,
        listener = function(event, editbox)
            if event == "began" then
                
            elseif event == "ended" then
                local s = editbox:getText()
                print("输入的几种颜色：",tonumber(s))
                self.maxColor = tonumber(s) > MAX_COLOR and MAX_COLOR or tonumber(s)
            elseif event == "return" then
                -- self:onEditBoxReturn(editbox)
            elseif event == "changed" then
                -- self:onEditBoxChanged(editbox)
            else
                print("EditBox event %s", tostring(event))
            end
        end
    })
    editBoxColor:setReturnType(cc.KEYBOARD_RETURNTYPE_SEND)
    editBoxColor:setText(self.maxColor)
    self.popLayer:addChild(editBoxColor)

   	-- 总共有多少个发射球
    display.newTTFLabel({text = "共有多少个发射球： "})
		:pos(initX,initY-200):addTo(self.popLayer)
    local editBubbleNum = cc.ui.UIInput.new({
        image = "#test_bar.png",
        size = cc.size(100, 50),
        x = initX+150,
        y = initY-200,
        listener = function(event, editbox)
            if event == "began" then
                
            elseif event == "ended" then
                local s = editbox:getText()
                print("输入的发射球：",tonumber(s))
                self.queue = tonumber(s) > 150 and 150 or tonumber(s)
            elseif event == "return" then
                -- self:onEditBoxReturn(editbox)
            elseif event == "changed" then
                -- self:onEditBoxChanged(editbox)
            else
                print("EditBox event %s", tostring(event))
            end
        end
    })
    editBubbleNum:setReturnType(cc.KEYBOARD_RETURNTYPE_SEND)
    editBubbleNum:setText(25)
    self.popLayer:addChild(editBubbleNum)

    -- 星数所需的分数
    display.newTTFLabel({text = "星数所需的\n(100,200,300)： "})
		:pos(initX,initY-300):addTo(self.popLayer)
    local editBubbleNum = cc.ui.UIInput.new({
        image = "#test_bar.png",
        size = cc.size(400, 50),
        x = initX+300,
        y = initY-300,
        listener = function(event, editbox)
            if event == "began" then
                
            elseif event == "ended" then
                local s = editbox:getText()
                print("输入的星数所需的：",s)
                self.starlevel = string.trim(s)
                if #string.split(self.starlevel,",") < 3 then 
                	print("输入的格式不对，以,分割,要三个")
                end
            elseif event == "return" then
                -- self:onEditBoxReturn(editbox)
            elseif event == "changed" then
                -- self:onEditBoxChanged(editbox)
            else
                print("EditBox event %s", tostring(event))
            end
        end
    })
    editBubbleNum:setReturnType(cc.KEYBOARD_RETURNTYPE_SEND)
    editBubbleNum:setText("100,200,300")
    self.popLayer:addChild(editBubbleNum)

    -- 模式 "save" ，"classic" ，"wheel"
    local model = {"classic","save" ,"wheel"}
    local RADIO_BUTTON_IMAGES = {
	    off = "#PlayButton.png",
	    off_pressed = "#PlayButton.png",
	    off_disabled = "#PlayButton.png",
	    on = "#ExitButton.png",
	    on_pressed = "#ExitButton.png",
	    on_disabled = "#PlayButton.png",
	}
    display.newTTFLabel({text = "关卡模式： "})
		:pos(initX,initY-400):addTo(self.popLayer)
	local curSelected = display.newTTFLabel({text = ""})
		:pos(initX+100,initY-400):addTo(self.popLayer)
    local group = cc.ui.UICheckBoxButtonGroup.new(display.TOP_TO_BOTTOM)
        :addButton(cc.ui.UICheckBoxButton.new(RADIO_BUTTON_IMAGES)
            :setButtonLabel(cc.ui.UILabel.new({text = "classic", color = display.COLOR_BLACK}))
            :setButtonLabelOffset(20, 0)
            :align(display.CENTER))
        :addButton(cc.ui.UICheckBoxButton.new(RADIO_BUTTON_IMAGES)
            :setButtonLabel(cc.ui.UILabel.new({text = "save", color = display.COLOR_BLACK}))
            :setButtonLabelOffset(20, 0)
            :align(display.CENTER))
        :addButton(cc.ui.UICheckBoxButton.new(RADIO_BUTTON_IMAGES)
            :setButtonLabel(cc.ui.UILabel.new({text = "wheel", color = display.COLOR_BLACK}))
            :setButtonLabelOffset(20, 0)
            :align(display.CENTER))
        
        :setButtonsLayoutMargin(10, 10, 10, 10)
        :onButtonSelectChanged(function(event)
            print("当前选的模式：",model[event.selected])
            curSelected:setString(model[event.selected])
        end)
        :align(display.LEFT_TOP, initX+100, initY-700)
        :addTo(self.popLayer)
    group:getButtonAtIndex(1):setButtonSelected(true)

    PYButton("#PlayButton.png","确 认",function(event)
	        self:initDebugUI()
	    end):pos(display.cx,100):addTo(self.popLayer)

end

function MapEditScene:onTouch(event)
    print("onTouch")
	if event.name ~= "began" then return true end
end


function MapEditScene:onEnter()

end


function MapEditScene:onExit()
end


return MapEditScene
