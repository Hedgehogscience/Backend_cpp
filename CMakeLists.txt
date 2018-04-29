cmake_minimum_required(VERSION 3.1)
project(Backend_cpp)

# Use the latest standard at this time.
set(CMAKE_CXX_STANDARD 17)
enable_language(ASM_MASM)

# Export to the a gitignored directory.
set(EXECUTABLE_OUTPUT_PATH ${Backend_cpp_SOURCE_DIR}/Bin)
set(LIBRARY_OUTPUT_PATH ${Backend_cpp_SOURCE_DIR}/Bin)

# Separate release and debugbuilds output and dependencies.
mark_as_advanced(CMAKE_IMPORT_LIBRARY_SUFFIX CMAKE_DEBUG_POSTFIX)
set(CMAKE_DEBUG_POSTFIX "d")

# Extra libraries to be linked.
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(EXTRA_LIBS ${EXTRA_LIBS} dl pthread)
else()
	set(EXTRA_LIBS ${EXTRA_LIBS} Psapi Userenv Iphlpapi)
endif()

# Extra flags for compiling and linking.
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
		set(EXTRA_CMPFLAGS ${EXTRA_CMPFLAGS} "-m32")
		set(EXTRA_LNKFLAGS ${EXTRA_LNKFLAGS} "-m32 -static-libgcc -static-libstdc++")
	else()
		set(EXTRA_CMPFLAGS ${EXTRA_CMPFLAGS} "-m64")
		set(EXTRA_LNKFLAGS ${EXTRA_LNKFLAGS} "-m64 -static-libgcc -static-libstdc++")
    endif()
else()
	set(EXTRA_CMPFLAGS ${EXTRA_CMPFLAGS} "")
	set(EXTRA_LNKFLAGS ${EXTRA_LNKFLAGS} "")
endif()

# Use static VC runtimes when releasing on Windows.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    foreach(flag_var
			CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE 
			CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
			CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()

# Set warningflags per compiler.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
endif()

# Common library includes.
find_package(OPENSSL)
find_library(UWS_LIBRARY uWS)
find_library(UV_LIBRARY libuv)
find_library(ZLIB_LIBRARY zlib)
find_path(UV_INCLUDE_DIR uv.h)
find_path(UWS_INCLUDE_DIR uWS/uWS.h)
include_directories(${UV_INCLUDE_DIR})
include_directories(${UWS_INCLUDE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})

# Registration service.
file(GLOB_RECURSE REGISTRATION_SOURCES "Source/Registration/*.cpp")
add_executable(Registration ${REGISTRATION_SOURCES})
set_target_properties(Registration PROPERTIES COMPILE_FLAGS "${EXTRA_CMPFLAGS}" LINK_FLAGS "${EXTRA_LNKFLAGS}")
target_link_libraries(Registration ${EXTRA_LIBS} ${UWS_LIBRARY} ${OPENSSL_LIBRARIES} ${UV_LIBRARY} ${ZLIB_LIBRARY})
