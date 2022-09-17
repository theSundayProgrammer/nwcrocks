
#include "db.h"
namespace lrocks {
     void setfuncs (lua_State *L, const luaL_Reg *l) {
        for (; l->name != NULL; l++) {  /* fill the table with given functions */
            lua_pushstring(L, l->name);
            lua_pushcclosure(L, l->func, 0);  /* closure with those upvalues */
            lua_settable(L, -3);
        }
        lua_pop(L, 0);  /* remove upvalues */
    }

        std::string get_str(lua_State* L, int index){

            size_t len;
            const char* name = luaL_checklstring(L, index, &len);
            return std::string(name,len);
        }
    int createmeta(lua_State *L, const char *name, const luaL_Reg *methods) {
        if (!luaL_newmetatable(L, name)) {
            return 0;
        }

        lua_pushstring(L, "__index");
        lua_newtable(L);
        lua_pushstring(L, "class");
        lua_pushstring(L, name);
        lua_rawset(L, -3);
        for (; methods->name; methods++) {
            lua_pushstring(L, methods->name);
            lua_pushcfunction(L, methods->func);
            lua_rawset(L, methods->name[0] == '_' ? -5: -3);
        }
        lua_rawset(L, -3);
        lua_pop(L, 1);
        return 1;
    }


    void setmeta(lua_State *L, const char *name) {
        luaL_getmetatable(L, name);
        lua_setmetatable(L, -2);
    }


    void lrocksdb_assert(lua_State *L, int cond, const char *msg) {
        if(!cond) {
            luaL_error(L, msg);
        }
    }
}
