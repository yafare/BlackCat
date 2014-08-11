
package.path = './?.lua;./zscripts/?.lua;./zscripts/libs/?.lua'

math.randomseed(os.time())



require('test.test')

function game_main()
    print('game_main')
end

print('load game_main.lua')
