
--package.cpath = package.cpath .. ';C:/Users/zhengshulin/AppData/Roaming/JetBrains/CLion2022.3/plugins/EmmyLua/debugger/emmy/windows/x64/?.dll'
--local dbg = require('emmy_core')
--dbg.tcpConnect('localhost', 9966)

package.cpath = package.cpath .. ';D:/JetBrains/apps/IDEA-U/ch-0/233.13135.103.plugins/EmmyLua/debugger/emmy/windows/x64/?.dll'
local dbg = require('emmy_core')
dbg.tcpListen('localhost', 9966)


local fw=require("framework.c")
local framework={}

print("lua framework init!")


function framework.start(app)
    print("framework.start")
    fw.game_init=app.game_init
    fw.game_update=app.game_update
    fw.game_touch=app.game_touch
    fw.game_message=app.game_message
    fw.game_pause=app.game_pause

    fw.inject()
end


return framework