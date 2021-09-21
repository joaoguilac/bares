cmake_minimum_required(VERSION 3.5)
project(EBNF VERSION 1.0.0 LANGUAGES CXX)

#=== SETTING VARIABLES ===#
set( GCC_COMPILE_FLAGS "-Wall -pedantic" )
set( APP_NAME "tinyexp" )

#=== MAIN APP ===
include_directories("src"
                    "lib"
                    "include")
add_executable(bares
               "src/main.cpp"
               "src/parser.cpp"
               "src/bares_manager.cpp")
target_compile_features( bares PUBLIC cxx_std_17 )
