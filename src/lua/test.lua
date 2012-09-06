local dbl = _ENV

function setup()
	dbl:fill(math.random(0, 255),
			 math.random(0, 255),
			 math.random(0, 255), 100)
	println("Line test")
	for i = 1 , 10 do
		dbl:stroke(math.random(0, 255),
				   math.random(0, 255),
				   math.random(0, 255))
		dbl:line(math.random(0, width),
				 math.random(0, height),
				 math.random(0, width),
				 math.random(0, height))
	end

	println("Ellipse test")
	dbl:stroke(255, 0, 0)
	dbl:ellipse_mode(CENTER)
	dbl:ellipse(50, 30, 100, 60)

	dbl:stroke(0, 255, 0)
	dbl:ellipse_mode(RADIUS)
	dbl:ellipse(150, 30, 50, 30)

	dbl:stroke(0, 0, 255)
	dbl:ellipse_mode(CORNER)
	dbl:ellipse(200, 0, 100, 60)

	dbl:stroke(255, 255, 0)
	dbl:ellipse_mode(CORNERS)
	dbl:ellipse(300, 0, 400, 60)

	println("Rectangle test")
	dbl:stroke(255, 0, 0)
	dbl:rect_mode(CENTER)
	dbl:rect(50, 130, 100, 60)

	dbl:stroke(0, 255, 0)
	dbl:rect_mode(RADIUS)
	dbl:rect(150, 130, 50, 30)

	dbl:stroke(0, 0, 255)
	dbl:rect_mode(CORNER)
	dbl:rect(200, 100, 100, 60)

	dbl:stroke(255, 255, 0)
	dbl:rect_mode(CORNERS)
	dbl:rect(300, 100, 400, 160)	

	println("Quad Test")
	dbl:quad(math.random(0, width),
			 math.random(0, height),
			 math.random(0, width),
			 math.random(0, height),
			 math.random(0, width),
			 math.random(0, height),
			 math.random(0, width),
			 math.random(0, height))
end