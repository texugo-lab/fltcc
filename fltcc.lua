print("Currently running FLTCC v" .. fltcc_VERSION)

local fileName = "compiler.c"
local execName = "main"

local fltcc = {
	args = {
		fileName,
		"-o",
		execName,
		-- "-c",
	},
	conf = {
		min_ver = "ALPHA",
	},
}

return fltcc
