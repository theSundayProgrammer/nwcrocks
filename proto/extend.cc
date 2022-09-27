#include <luajit/lua.hpp>
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
int multiplication(lua_State *L) {
    int a = luaL_checkinteger(L, 1);
    int b = lua_tointeger(L, lua_upvalueindex(1));
    lua_Integer c = a * b;
    lua_pushinteger(L, c);
    return 1;
}




#ifdef __cplusplus
extern "C"
#endif
DLL_PUBLIC int luaopen_proto(lua_State *L) {
    lua_newtable(L);


    lua_pushstring(L, "multiply");
    lua_pushinteger(L, 78);
    lua_pushcclosure(L, multiplication, 1);  /* closure with those upvalues */
    lua_settable(L, -3);
    return 1;
}

