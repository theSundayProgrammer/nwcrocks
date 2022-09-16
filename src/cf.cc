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
using ROCKSDB_NAMESPACE::ColumnFamilyHandle;

 ROCKSDB_NAMESPACE::Options options_from_table(lua_State *L, int index) ;
    int open_cf(lua_State *L);
    namespace {
        int close(lua_State *L) ;
        int get(lua_State* L) ;
        int put(lua_State* L) ;
        const struct luaL_Reg  cf_reg[] = {
            { "get", get },
            { "put", put },
            { "close", close },
            { NULL, NULL }
        };



        struct rocks_cf { 
            DB* db=nullptr;
            bool open=false;
            Options options;
            std::vector<std::string> cf_names;
            std::vector<ColumnFamilyHandle*> handles;
            ColumnFamilyHandle* get_handle(const std::string& str){
                ColumnFamilyHandle  *handle = nullptr;
                auto iter_handle = handles.begin();
                for( auto iter_name = cf_names.begin();
                        iter_name != cf_names.end() && *iter_name != str;
                        ++iter_name) 
                    ++iter_handle;
                if (iter_handle != handles.end())
                    handle = *iter_handle;
                return handle;

            }


            ~rocks_cf () {
                for (auto handle : handles) {
                    Status s = db->DestroyColumnFamilyHandle(handle);
                    assert(s.ok());
                }
                delete db;
            }
        };
        std::string get_str(lua_State* L, int index){

            size_t len;
            const char* name = luaL_checklstring(L, index, &len);
            return std::string(name,len);
        }

        int put(lua_State* L) {
            using ROCKSDB_NAMESPACE::Slice;
            int argc=0;
            rocks_cf *d = (rocks_cf*) luaL_checkudata(L, ++argc, "cf");
            std::string cf_name = get_str(L, ++argc);
            std::string key = get_str(L, ++argc);
            std::string value = get_str(L, ++argc);
            Status s = d->db->Put(WriteOptions(), d->get_handle(cf_name), Slice(key), Slice(value));
            if(!s.ok()) {
                fprintf(stderr,"status = %s\n", s.getState());
                luaL_error(L, "failed to put");
                return 0;
            }
            return 1;
        }
        int get(lua_State* L) {
            using ROCKSDB_NAMESPACE::Slice;
            int argc=0;
            rocks_cf *d = (rocks_cf*) luaL_checkudata(L, ++argc, "cf");
            std::string cf_name = get_str(L, ++argc);
            std::string key = get_str(L, ++argc);
            std::string value ;
            Status s = d->db->Get(ReadOptions(), d->get_handle(cf_name), Slice(key), &value);
            if(!s.ok()) {
                fprintf(stderr,"status = %s\n", s.getState());
                luaL_error(L, "failed to get");
                return 0;
            }
            if(!value.empty() ) {
                lua_pushlstring(L, value.data(), value.size());
            }
            else {
                lua_pushnil(L);
            }
            return 1;
        }
        int close(lua_State *L) {
            int index = 1;
            rocks_cf *d = (rocks_cf*) luaL_checkudata(L, index, "cf");
            luaL_argcheck(L, d != NULL && d->db != NULL, index, "cf expected");
            d->~rocks_cf(); ;

            return 1;
        }
    }


int open_cf(lua_State* L){
    static bool init = [=]() {
    lrocks::createmeta(L, "cf", cf_reg);
    return true;
    }();
    DB* db;
    int argc = 0;
    using ROCKSDB_NAMESPACE::ColumnFamilyDescriptor;
    Options options = options_from_table(L, ++argc);
    // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    // create the DB if it's not already present

    //fprintf(stderr," create = %s\n", options.create_if_missing ? "true": "false");
    //fprintf(stderr," create = %s\n", options.create_missing_column_families? "true": "false");
    // open DB

    const char *path = luaL_checkstring(L, ++argc);

    lua_pushvalue(L, ++argc);
    lua_pushnil(L);
    // open DB with two column families
    std::vector<ColumnFamilyDescriptor> column_families;
    // have to open default column family
    std::vector<std::string> cf_names;
    while (lua_next(L, -2))
    {
        lua_pushvalue(L, -2);
        int index = luaL_checkint(L, -1);
        const char *value = lua_tostring(L, -2);
        cf_names.push_back(value);
        column_families.push_back(
                ColumnFamilyDescriptor( value, ROCKSDB_NAMESPACE::ColumnFamilyOptions())
                );
        //fprintf(stderr,"%d=%s\n", index,value);
        lua_pop(L, 2);
    }
    lua_pop(L, 1);
    /* int options */
    std::vector<ColumnFamilyHandle*> handles;
    Status s = DB::Open(options, path, column_families, &handles, &db);
    if(!s.ok()) {
        fprintf(stderr,"status = %s\n", s.getState());
        luaL_error(L, "failed to open");
        return 0;
    }
    rocks_cf *d = new (lua_newuserdata(L, sizeof(rocks_cf))) rocks_cf();
    d->options=options;
    d->db = db;
    d->open = true;
    d->handles.swap(handles);
    d->cf_names.swap(cf_names);
    assert(cf_names.size() == handles.size());
    lrocks::setmeta(L, "cf");

    return 1;
}
