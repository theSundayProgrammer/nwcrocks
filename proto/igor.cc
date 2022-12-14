#include <luajit/lua.hpp>
#include <iostream>
 
// MyObject as C++ class
class MyObject{
	private:
		double x;
	public:
		MyObject(double x) : x(x){}
		~MyObject()  {}
		void set(double x){this->x = x;}
		double get() const{return this->x;}
};
 
#define LUA_MYOBJECT "MyObject"
 
// Create & return MyObject instance to Lua
static int myobject_new(lua_State* L){
	double x = luaL_checknumber(L, 1);
	 new (lua_newuserdata(L, sizeof(MyObject))) MyObject(x);
	luaL_setmetatable(L, LUA_MYOBJECT);
	return 1;
}
 
 
static int myobject_destroy(lua_State* L){
	(reinterpret_cast<MyObject*>(luaL_checkudata(L, 1, LUA_MYOBJECT)))->~MyObject();
	return 0;
}
// MyObject member functions in Lua
static int myobject_set(lua_State* L){
	(reinterpret_cast<MyObject*>(luaL_checkudata(L, 1, LUA_MYOBJECT)))->set(luaL_checknumber(L, 2));
	return 0;
}
static int myobject_get(lua_State* L){
	lua_pushnumber(L, (reinterpret_cast<MyObject*>(luaL_checkudata(L, 1, LUA_MYOBJECT)))->get());
	return 1;
}
 
// Register MyObject to Lua
static void register_myobject(lua_State* L){
    lua_register(L, LUA_MYOBJECT, myobject_new);
    luaL_newmetatable(L, LUA_MYOBJECT);
    lua_pushcfunction(L, myobject_destroy); lua_setfield(L, -2, "__gc");
    lua_newtable(L);
    lua_pushcfunction(L, myobject_set); lua_setfield(L, -2, "set");
    lua_pushcfunction(L, myobject_get); lua_setfield(L, -2, "get");
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
}
 
// Program entry
int main(int argc, char** argv){
	if(argc > 1){
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		register_myobject(L);
		if(luaL_dofile(L, argv[1]))
			std::cerr << lua_tostring(L, -1);
		lua_close(L);
	}else
		std::cerr << "Expected filename from command line!";
	return 0;
}
