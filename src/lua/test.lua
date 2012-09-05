local DBL = _ENV

function setup()
	println("Line test")
	for i = 1 , 10 do
		DBL:stroke(math.random(0, 255),
				   math.random(0, 255),
				   math.random(0, 255))
		DBL:line(math.random(0, width),
				 math.random(0, height),
				 math.random(0, width),
				 math.random(0, height))
	end

	println("Ellipse test")
	DBL:stroke(255, 0, 0)
	DBL:ellipse_mode(CENTER)
	DBL:ellipse(50, 30, 100, 60)

	DBL:stroke(0, 255, 0)
	DBL:ellipse_mode(RADIUS)
	DBL:ellipse(150, 30, 50, 30)

	DBL:stroke(0, 0, 255)
	DBL:ellipse_mode(CORNER)
	DBL:ellipse(200, 0, 100, 60)

	DBL:stroke(255, 255, 0)
	DBL:ellipse_mode(CORNERS)
	DBL:ellipse(300, 0, 400, 60)
end