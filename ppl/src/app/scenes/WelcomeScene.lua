--
-- Author: Your Name
-- Date: 2015-03-17 13:14:48
--
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
			switchScene("PlayScene")
		end)
		:align(display.CENTER,display.cx,display.cy):addTo(self)

	-- 设置随机种子
	randomInit()

end


function WelcomeScene:onEnter()
end

function WelcomeScene:onExit()
end

return WelcomeScene