set_kind("binary")
add_cxxflags("-std=c++11")
set_optimize("faster")
set_warnings("all")
set_targetdir("./target")
add_includedirs("./include")
add_linkdirs("./lib")
add_cxflags("-g3")
add_syslinks("pthread", "dl")

after_build(function ()
    os.rm("build")
end)

target("gdb_1")
    add_files("gdb_1.cc")
    
