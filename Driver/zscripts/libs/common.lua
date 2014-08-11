
function hex_dump(buf)
    local hex = ''
    for byte = 1, #buf, 16 do
        local chunk = buf:sub(byte, byte + 15)
        hex = hex .. string.format('%08X  ',byte - 1)
        chunk:gsub('.', function(c)
            hex = hex .. string.format('%02X ',string.byte(c))
        end)
        hex = hex .. string.rep(' ', 3 * (16 - #chunk)) .. ' ' .. chunk:gsub('%c', '.') .. '\n'
    end
    print(hex)
end
