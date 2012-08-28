function setup()
	print('inside setup')
end

function draw()
	put_pixel(math.floor(math.random()*640), math.floor(math.random()*480), 255, 0, 0)
end