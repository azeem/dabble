local Dabble = {
	loaded = {},
	path = '?.lua'
	math = math
}

local function setfenv(func, env)
	local name
	local upv_id = 0
	
	repeat
		upv_id = upv_id + 1
		name = debug.getupvalue(func, upv_id)
	until name == '_ENV' or name == nil
	
	if name then
		debug.upvaluejoin(func, upv_id, function() return name end, 1)
		debug.setupvalue(func, upv_id, env)
	end
end

function Dabble:create(script_name, init)
	local func = self.loaded[script_name]
	if func == nil then
		local err, path
		path = package.searchpath(script_name, self.path)
		if path == nil then
			return nil, "Unable to find script " .. script_name .. " in path " .. self.path
		end
		func, err = loadfile(path)
		if err ~= nil then
			error(err)
		end
		self.loaded[script_name] = func
	end
	
	-- create new dabble object
	local new_dbl
	if init then
		new_dbl = init
	else
		new_dbl = {}
	end
	new_dbl['DBL'] = new_dbl -- set the DBL self reference
	self.__index = self
	setmetatable(new_dbl, self)
	setfenv(func, new_dbl)
	-- setfenv(func, {DBL = new_dbl, print = print})
	func()
	return new_dbl
end

function Dabble:put_pixel(x, y, r, g, b)
	gfx.put_pixel(self.screen, x, y, r, g, b)
end

function Dabble.print(...)
	print("heasdllo")
	print(...)
end

return Dabble