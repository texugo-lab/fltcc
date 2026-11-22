#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

extern "C" {
#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>
}

namespace lua {

// TYPES
typedef int														   integer;
typedef std::string												   str;
typedef double													   num;
typedef bool													   boolean;
typedef lua_State												   State;
typedef lua_CFunction											   func;
typedef std::tuple<lua::integer, lua::num, lua::str, lua::boolean> multi;

// START FUNCTIONALITY
// Number of globals
lua::integer globalI = 0;
// Vector of globals' names
std::vector<std::tuple<lua::str, lua::integer>> globalNames;
std::vector<lua::multi>							globalValues;

void setGlobalString(lua::str name, lua::str value) {
	lua::integer type = 1;
	globalValues.push_back({NULL, NULL, value, NULL});
	globalI++;
	globalNames.push_back({name, type});
}
void setGlobalInteger(lua::str name, lua::integer value) {
	lua::integer type = 2;
	globalValues.push_back({value, NULL, "", NULL});
	globalI++;
	globalNames.push_back({name, type});
}
void setGlobalNumber(lua::str name, lua::num value) {
	lua::integer type = 3;
	globalValues.push_back({NULL, value, "", NULL});
	globalI++;
	globalNames.push_back({name, type});
}
void setGlobalBoolean(lua::str name, lua::boolean value) {
	lua::integer type = 4;
	globalValues.push_back({NULL, NULL, "", value});
	globalI++;
	globalNames.push_back({name, type});
}
lua::State* newstate() {
	lua::State* L = luaL_newstate();
	luaL_openlibs(L);
	return L;
}
lua::State* L = lua::newstate();
void		run(lua::str file) {
	for (lua::integer i = 0; i < globalI; i++) {
		auto [name, type] = globalNames.at(i);

		auto [gInt, gNum, gStr, gBool] = globalValues.at(i);
		if (type == 1) lua_pushstring(lua::L, gStr.c_str());
		else if (type == 2) lua_pushinteger(lua::L, gInt);
		else if (type == 3) lua_pushnumber(lua::L, gNum);
		else if (type == 4) lua_pushboolean(lua::L, gBool);

		lua_setglobal(lua::L, name.c_str());
	}
	if (luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 1, 0)) {
		std::cout << "Could not load '" << file << "':\n"
				  << lua_tostring(L, -1) << "\n";
		exit(1);
	}
}

void close() {
	lua_close(lua::L);
}

// USEFUL FUNCTIONS
lua::integer tointeger(lua::integer idx = -1) { return lua_tointeger(lua::L, idx); }
lua::boolean toboolean(lua::integer idx = -1) { return lua_toboolean(lua::L, idx); }
lua::num	 tonumber(lua::integer idx = -1) { return lua_tonumber(lua::L, idx); }
lua::str	 tostring(lua::integer idx = -1) { return lua_tostring(lua::L, idx); }
lua::boolean isstring(lua::integer idx = -1) { return lua_isstring(lua::L, idx); }
lua::boolean isnumber(lua::integer idx = -1) { return lua_isnumber(lua::L, idx); }
lua::boolean isboolean(lua::integer idx = -1) { return lua_isboolean(lua::L, idx); }
lua::boolean isinteger(lua::integer idx = -1) { return lua_isinteger(lua::L, idx); }

void pop(lua::integer n = 1) { lua_pop(lua::L, n); }

// ARRAY AND TABLE
lua::integer getlen(lua::integer idx = -1) {
	lua_len(lua::L, idx);
	lua::integer len = lua::tointeger();
	lua::pop();
	return len;
}
void getfield(lua::str field, lua::integer idx = -1) { lua_getfield(lua::L, idx, field.c_str()); }

lua::multi geti(lua::integer pos, lua::integer idx = -1) {
	lua_geti(lua::L, idx, pos);
	return {lua::tointeger(), lua::tonumber(), lua::tostring(), lua::toboolean()};
}
void seti(lua::integer pos, lua::integer idx = -1) { lua_seti(lua::L, idx, pos); }

// RAW
lua::multi r_geti(lua::integer pos, lua::integer idx = -1) {
	lua_rawgeti(lua::L, idx, pos);
	return {lua::tointeger(), lua::tonumber(), lua::tostring(), lua::toboolean()};
}
void r_seti(lua::integer pos, lua::integer idx = -1) { lua_rawseti(lua::L, idx, pos); }

};	// namespace lua
