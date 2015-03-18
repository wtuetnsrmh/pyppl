
require("config")
require("framework.init")
require("cocos.init")
require("app.scenes.Constants")
require("utils.init")

local MyApp = class("MyApp", cc.mvc.AppBase)

function MyApp:ctor()
    MyApp.super.ctor(self)
end

function MyApp:run()
    cc.FileUtils:getInstance():addSearchPath("res/")

-- test --
 --    local t = {
	-- 			class = {r =1,c=2},
	-- 			save  ={r =1,c=3},
	-- 			wheel = {r =1,c=4},
	-- 			clear  = {r =1,c=5},
	-- 			time = {r =1,c=6},
	-- 		}
	-- dump(t)
	-- -- local tempI=1
	-- -- for _,v in pairs(t) do
	-- -- 	tempI = tempI+1
	-- -- 	if tempI == 10 then
	-- -- 		break
	-- -- 	else
	-- -- 		t[5] = tostring(tempI)
	-- -- 	end
	-- -- end
	-- local k = table.keyof(t,{r=1,c=2})
	-- print(k)
	-- dump(t)

-- end test --

    self:enterScene("LoadingScene")
end

return MyApp
