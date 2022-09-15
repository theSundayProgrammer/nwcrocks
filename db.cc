#define LROCKSDB_VERSION "nwc-rocksdb 0.0.1"
#define LROCKSDB_COPYRIGHT "Copyright (C) 2022 Joe Mariadassou"
#define LROCKSDB_DESCRIPTION "RocksDB C++ Lua binding"
#include "db.h"
#include <cstdio>
#include <string>


using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::PinnableSlice;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteBatch;
using ROCKSDB_NAMESPACE::WriteOptions;

namespace {
    int open_db(lua_State *L);
    int close(lua_State *L);
    const struct luaL_Reg  db_reg[] = {
        { "close", close },
        { NULL, NULL }
    };
    const struct luaL_Reg  funcs[] = {
        { "open", open_db },
        { NULL, NULL }
    };


struct rocks_db {
    DB* db;
    bool open;
};

int open_db(lua_State *L) {
    DB* db;
    Options options;
    // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    // create the DB if it's not already present
    options.create_if_missing = true;

    // open DB

    const char *path = luaL_checkstring(L, 1);

    Status s = DB::Open(options, path, &db);
    assert(s.ok());
    if(!s.ok()) {
        luaL_error(L, "failed to open");
        return 0;
    }
        rocks_db *d = (rocks_db *) lua_newuserdata(L, sizeof(rocks_db));
    d->db = db;
    d->open = true;
    lrocks::setmeta(L, "db");

    return 1;
}

    int close(lua_State *L) {
        int index = 1;
        rocks_db *d = (rocks_db*) luaL_checkudata(L, index, "db");
        luaL_argcheck(L, d != NULL && d->db != NULL, index, "db expected");
        d->open = 0;
        delete d->db;
        return 1;
    }



}
#ifdef __cplusplus
extern "C"
#endif
DLL_PUBLIC int luaopen_nwcrocks(lua_State *L) {
    lua_newtable(L);

    /* register classes */

    lrocks::createmeta(L, "db", db_reg);
    lrocks::setfuncs(L, funcs, 0);

    lua_pushliteral(L, LROCKSDB_VERSION);
    lua_setfield(L, -2, "_VERSION");
    lua_pushliteral(L, LROCKSDB_COPYRIGHT);
    lua_setfield(L, -2, "_COPYRIGHT");
    lua_pushliteral(L, LROCKSDB_VERSION);
    lua_setfield(L, -2, "_DESCRIPTION");

    return 1;
}

