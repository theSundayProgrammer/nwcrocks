Case For Lua Extension Modules in C
=
Lua is well known as a programming lanuage that can be embedded quite easily into any application written in C/C++.
There are many applications that use Lua as an embedded language; Openresty, Neovim, Luatex, Redis and Wireshark to name a few.
However if you want extend the application using some existing library then you need to write an interface that helps Lua access that library.
Luajit, a more popular fork of Lua, introduced a Foreign Function Interface, that in theory helps to use any C library without having to write an interface.
There are some caveats of course, but nevertheless it is possible. In practice though, you don't want the Lua developer to know all the nitty-gritty details of the library
and so you provide a wrapper that simplifies the library. However this FFI is available only for Luajit and not the original version of Lua.
Secondly as we shall see, writing a C interface that is source code compatible with both versions of Lua is not too difficult.  

Why Lua
==
Given that Lua is not very popular why use Lua? Why not Python? Admittedly Python is a more poular language. That is mostly due to clever marketting on the part
of Pythonistas. But Lua is a tiny shared object, about 550 KB, and the intrinsically better. Hence it could be ported to Raspberry Pi or smaller devices without any trouble. Besides the language
is richer than Python but not as complex as say Haskell or OCaml. However it does not match Python in its breadth of libraries.
But as we shall see soon writing a C/C++ application to incorporate a  C/C++ library is a lot less simple compare to other languages.

Your First Extension Module
==
Listed below is all the code required to export a local function 'l_sin' by the name 'mysin'
```CPP
///Code modified from Luajit source
#include <math.h>
#include <lua.hpp>
static
int l_sin(lua_State* L){
	double d = luaL_checknumber(L,1);
	lua_pushnumber(L,sin(d));
	return 1;
}

extern "C"
int luaopen_mysin(lua_State* L){
    lua_newtable(L);
    lua_pushcfunction(L,l_sin); 
    lua_setfield(L,-2,"mysin");
	return 1;
}	
```
The code above must be compiled to create a shared object by the name "mysin.so" to use the Lua code isted below.
```lua
ext=require("mysin")
local n = ext.mysin(10.20)
```
The include folder and link library needs to be set appropriately to use the targeted version of Lua. Notice that the name of the shared libray and the suffix of the exported function must match as shown above.

Module Design
==
There are two main ways of writing a wrapper around a C/C++ library. The first is to simply expose all the C functions to Lua. This is the most common approach.
However in most cases the user writes another wrapper in Lua around the exposed functions so as to simplify the interface. 
Hence instead of exposing the whole 'C' library  it would be better if the wrapper functions that would otherwise be written in Lua are exposed as module functions. 
This implies that C++ is better for larger designs although Lua could still be used to glue the main application with the C library.



