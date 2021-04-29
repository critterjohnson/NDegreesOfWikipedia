# found a lot of this at https://github.com/hsf-training/hsf-training-cmake-webpage/blob/gh-pages/_includes/code/03-structure/CMakeLists.txt

cmake_minimum_required(VERSION 3.14...3.18)
project(
    NDegreesOfWikipedia 
    VERSION 0.0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    
    set_property(GLOBAL PROPERTY USE_FOLERS ON)

    include(CTest)

endif()

find_package(OpenSSL REQUIRED)

# set(CPR_FORCE_USE_SYSTEM_CURL ON)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG aac5058a15e9ad5ad393973dc6fe44d7614a7f55) # 1.6.0
FetchContent_MakeAvailable(cpr)

add_subdirectory(src)

add_subdirectory(apps)
