--
-- Author: yzm
-- Date: 2015-03-09 16:57:52
--
local json = require("framework.json")
local BublleSprite = import(".BublleSprite")
local BublleBasic = import("..modle.BublleBasic")

local PlayScene = class("PlayScene", function()
    return display.newScene("PlayScene")
end)

function PlayScene:ctor()
	local rootLayer = display.newLayer()
	-- rootLayer:setAnchorPoint(cc.p(0.5,0.5))
	-- 
    local jsonStr = cc.HelperFunc:getFileData("level-data/064.json")
    local jsonObj = json.decode(jsonStr)
    -- dump(jsonObj.level.graph)
    print("jsonObj.level.graph",#jsonObj.level.graph)
    local graph = jsonObj.level.graph
    for i,bublleData in ipairs(graph) do 
    	local bublle = BublleSprite.new(BublleBasic.new(bublleData))
    		:pos(bublleData.x * BUBLLE_RADIS + (bublleData.y%2 ~= 0 and BUBLLE_RADIS or BUBLLE_RADIS/2) , display.top - bublleData.y * BUBLLE_RADIS - BUBLLE_RADIS/2):addTo(rootLayer)
    end

    rootLayer:addTo(self)
end

function PlayScene:onEnter()
end

function PlayScene:onExit()
end

return PlayScene