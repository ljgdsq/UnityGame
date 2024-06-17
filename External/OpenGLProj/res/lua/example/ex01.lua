local framework = require "framework"
--local Sprite= require("game.sprite")
local game={}

local sprite=require("framework.Sprite")

local sp1=sprite.new("face")
local x=100
local y=200
function game.game_init()
    print("game_init")

end

function game.game_update()
    --print("game_update")
    x=x+1
    y=y+2
    sp1:pos(x,y)
    sp1:draw()
    --Sprite.pos(sp1,x,y)
    --Sprite.draw(sp1)
end

framework.start(game)
