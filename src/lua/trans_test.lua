local can = canvas
local dbl = dbl

function mov_func(x, y)
	return x+10, y+10
end

function setup()
	can:stroke(255, 255, 255)
	can:line(10, 10, 124, 60)
	local mov = dbl:create {type="Movement", func=mov_func}
	mov:draw()
end