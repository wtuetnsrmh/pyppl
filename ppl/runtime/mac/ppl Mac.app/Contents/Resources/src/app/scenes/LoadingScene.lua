--
-- Author: Your Name
-- Date: 2015-03-16 17:32:37
--
require("framework.cocos2dx.Cocos2dConstants")

local LoadingScene = class("LoadingScene", function()
    return display.newScene("LoadingScene")
end)

function LoadingScene:ctor()
	cc.FileUtils:getInstance():addSearchPath("res/")

	-- 预加载材质
	self.cur_load_index = 1
	self:loadTextureAssetes()

    -- 预加载声音
    for k, v in pairs(AUDIO_ASSETS) do
        audio.preloadSound(v)
    end
end

function LoadingScene:loadTextureAssetes()
	display.setTexturePixelFormat(cc.TEXTURE2D_PIXEL_FORMAT_RGBA4444)
	display.addSpriteFrames(TEXTUER_ASSETS[self.cur_load_index]..".plist",TEXTUER_ASSETS[self.cur_load_index]..".pvr.ccz",function(plist,image)
		self.cur_load_index = self.cur_load_index + 1
		if self.cur_load_index < #TEXTUER_ASSETS then
			self:loadTextureAssetes()
		else
			switchScene("MainScene")
		end
		
	end)
end

function LoadingScene:onEnter()
end

function LoadingScene:onExit()
end

return LoadingScene