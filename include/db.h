#pragma once
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#include <luajit/lua.hpp>
#include <string>
namespace rocksdb{
    class DB;
}
namespace lrocks {
    int createmeta(lua_State *L, const char *name, const luaL_Reg *methods) ;
    void setfuncs (lua_State *L, const luaL_Reg *l) ;
    void setmeta(lua_State *L, const char *name) ;
    void assert(lua_State *L, int cond, const char *msg) ;
    std::string get_str(lua_State* L, int index);
    int writebatch_create(lua_State *L, rocksdb::DB*);
}

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
