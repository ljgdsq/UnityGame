local cSprite= require("game.sprite")
local sprite={}

local sprite_meta={}

function sprite_meta.__index(spr,key,...)
    return cSprite[key]
end

function sprite_meta.__newindex(spr,key,v)
    print("__newindex "..key)
end

function sprite.new(name)
    local cIns=cSprite.new(name)
    return debug.setmetatable(cIns,sprite_meta)
end



return sprite