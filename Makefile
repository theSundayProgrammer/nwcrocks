
LIB_PATH= $(HOME)/.local/lib
INC_PATH= $(HOME)/.local/include
LUA_LIB= -L$(LIB_PATH) -lluajit-5.1
LUA_INC= -I$(INC_PATH)
ROCKSDB_LIB= -lrocksdb
EXTRACFLAGS= -fpic

INC= $(LUA_INC) $(ROCKSDB_INC)
LIB= $(LUA_LIB) $(ROCKSDB_LIB)
WARN= -Wall
CFLAGS= -O2 -std=c++17 $(WARN) $(INC)  -fvisibility=hidden

MYNAME= nwcrocks
MYLIB= 
T= $(MYNAME).so
OBJS= src/db.o \
	  src/cf.o \
	  src/writebatch_cf.o \
	  src/writebatch.o \
	  src/backup_engine.o \
	  src/iter.o \
	  src/utils.o \
	  src/options.o


%.o: %.cc 
	$(CC) $(CFLAGS) -I./include -fPIC -c -o $@ $<

$T:	$(OBJS)
	$(CC) $(CFLAGS)  $(EXTRACFLAGS) -shared -o $@  $(LIB)  $(OBJS) -lrocksdb  

clean:
	rm -f $T $(OBJS)



