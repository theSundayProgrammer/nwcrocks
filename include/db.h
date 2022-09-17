#pragma once
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
extern "C" {
#include <luajit/lauxlib.h>
}
#include <string>
namespace lrocks {
    int createmeta(lua_State *L, const char *name, const luaL_Reg *methods) ;
    void setfuncs (lua_State *L, const luaL_Reg *l, int nup) ;
    void setmeta(lua_State *L, const char *name) ;
    void assert(lua_State *L, int cond, const char *msg) ;
    std::string get_str(lua_State* L, int index);
}
#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
