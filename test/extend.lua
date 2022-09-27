#!/usr/bin/env luajit
ext=require("mysin")
local n = ext.mysin(10.20)
exe=require("mysin")
local m = exe.mysin(20.20)
print("n=", n, " m=", m)
