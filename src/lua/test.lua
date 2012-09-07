local can = canvas

function setup()
	can:fill(math.random(0, 255),
			 math.random(0, 255),
			 math.random(0, 255), 100)
	println("Line test")
	for i = 1 , 10 do
		can:stroke(math.random(0, 255),
				   math.random(0, 255),
				   math.random(0, 255))
		can:line(math.random(0, width),
				 math.random(0, height),
				 math.random(0, width),
				 math.random(0, height))
	end

	println("Ellipse test")
	can:stroke(255, 0, 0)
	can:ellipse_mode(can.CENTER)
	can:ellipse(50, 30, 100, 60)

	can:stroke(0, 255, 0)
	can:ellipse_mode(can.RADIUS)
	can:ellipse(150, 30, 50, 30)

	can:stroke(0, 0, 255)
	can:ellipse_mode(can.CORNER)
	can:ellipse(200, 0, 100, 60)

	can:stroke(255, 255, 0)
	can:ellipse_mode(can.CORNERS)
	can:ellipse(300, 0, 400, 60)

	println("Rectangle test")
	can:stroke(255, 0, 0)
	can:rect_mode(can.CENTER)
	can:rect(50, 130, 100, 60)

	can:stroke(0, 255, 0)
	can:rect_mode(can.RADIUS)
	can:rect(150, 130, 50, 30)

	can:stroke(0, 0, 255)
	can:rect_mode(can.CORNER)
	can:rect(200, 100, 100, 60)

	can:stroke(255, 255, 0)
	can:rect_mode(can.CORNERS)
	can:rect(300, 100, 400, 160)	

	println("Quad Test")
	can:quad(math.random(0, width),
			 math.random(0, height),
			 math.random(0, width),
			 math.random(0, height),
			 math.random(0, width),
			 math.random(0, height),
			 math.random(0, width),
			 math.random(0, height))
end