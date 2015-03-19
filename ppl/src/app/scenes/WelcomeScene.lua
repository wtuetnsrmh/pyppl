--
-- Author: Your Name
-- Date: 2015-03-17 13:14:48
--
local PopInputMapNameLayer = import(".PopInputMapNameLayer")

local WelcomeScene = class("WelcomeScene", function()
    return display.newScene("WelcomeScene")
end)

function WelcomeScene:ctor()
	display.newSprite("bg.jpg"):pos(display.cx,display.cy):addTo(self)
	cc.ui.UIPushButton.new("#PlayButton.png",{scale9 = true})
		:setButtonSize(250,104)
		:setButtonLabel("normal",cc.ui.UILabel.new({
			UILabelType = 2,
			text = "PLAY",
			size = 30
			}))
		:onButtonClicked(function(event)
			-- switchScene("SelectScene")
			clearMemory()
			local popLayer = PopInputMapNameLayer.new({callBack = function(name)
				print("name =",name)
				switchScene("PlayScene",{level = name})
			end}):addTo(self)
		end)
		:align(display.CENTER,display.cx,display.cy+300):addTo(self)

	-- 新建地图
	cc.ui.UIPushButton.new("#PlayButton.png",{scale9 = true})
		:setButtonSize(300,104)
		:setButtonLabel("normal",cc.ui.UILabel.new({
			UILabelType = 2,
			text = "新建地图",
			size = 30
			}))
		:onButtonClicked(function(event)
			clearMemory()
			switchScene("MapEditScene",{op = 1})
		end)
		:align(display.CENTER,display.cx,display.cy):addTo(self)

	-- 编辑地图
	cc.ui.UIPushButton.new("#PlayButton.png",{scale9 = true})
		:setButtonSize(300,104)
		:setButtonLabel("normal",cc.ui.UILabel.new({
			UILabelType = 2,
			text = "编辑地图",
			size = 30
			}))
		:onButtonClicked(function(event)
			clearMemory()
			switchScene("MapEditScene",{op = 2})
		end)
		:align(display.CENTER,display.cx,display.cy-200):addTo(self)

	-- 设置随机种子
	randomInit()

end


function WelcomeScene:onEnter()
end

function WelcomeScene:onExit()
end

return WelcomeScene