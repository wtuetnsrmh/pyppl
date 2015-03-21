--
-- Author: Your Name
-- Date: 2015-03-19 15:37:09
-- 请求输入文件名窗口 PopInputMapNameLayer
local PopInputMapNameLayer = class("PopInputMapNameLayer", function()
    return display.newLayer("PopInputMapNameLayer")
end)

function PopInputMapNameLayer:ctor(params)
	display.newColorLayer(cc.c4b(255,128,128,200))
		:addTo(self)
	local maskLayer = display.newLayer():addTo(self)
	maskLayer:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
      		if event.name ~= "began" then return end
		end)

	local initX,initY = 200,1000
	self.name = "001"
	display.newTTFLabel({text = "输入要编辑的关卡文件名： "}):pos(initX,initY):addTo(self)
	local editBox2 = cc.ui.UIInput.new({
        image = "#test_bar.png",
        size = cc.size(100, 50),
        x = initX+200,
        y = initY,
        listener = function(event, editbox)
            if event == "began" then
                
            elseif event == "ended" then
                local s = editbox:getText()
                self.name = s
                print("输入的关卡文件名：",s)
            elseif event == "return" then
                -- self:onEditBoxReturn(editbox)
            elseif event == "changed" then
                -- self:onEditBoxChanged(editbox)
            else
                print("EditBox event %s", tostring(event))
            end
        end
    })
    editBox2:setReturnType(cc.KEYBOARD_RETURNTYPE_SEND)
    self:addChild(editBox2)

	PYButton("#PlayButton.png","确 认",function(event)
		if self.name == "" then
			print("不能为空")
		else
			local path = cc.FileUtils:getInstance():getWritablePath().."res/mapData/".. self.name ..".json"
			print("path",path)
			if io.exists(path) then
			    params.callBack(self.name)
			else
				print("输入的文件不存在")
			end
			
		end
	    
	end):pos(display.cx,100):addTo(self)

end


return PopInputMapNameLayer

