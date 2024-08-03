
cmake_minimum_required (VERSION 3.8)

#Define o projeto 
project(srvOuve VERSION 1.0)

#Diretorios de Include
include_directories(	   "/usr/local/include/pocketsphinx"
						   "/usr/local/include/sphinxbase"
                          )						   

set(CMAKE_CXX_STANDARD 14)

add_executable(srvOuve ouve.cpp)

