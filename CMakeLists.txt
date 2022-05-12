cmake_minimum_required(VERSION 3.8)

#set(PATCH_VERSION 3)
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
project(allocator VERSION 0.0.${PATCH_VERSION})

#option(WITH_BOOST_TEST "Boost tests" ON)

#install(TARGETS helloworld RUNTIME DESTINATION bin)
#install(TARGETS version RUNTIME DESTINATION lib)

add_executable(allocator allocator.cpp)

set_target_properties(allocator PROPERTIES
					CXX_STANDARD 17
					CXX_STANDARD_REQUIRED ON
)

target_compile_options(allocator PUBLIC -Wall -Wextra -pedantic -Werror -Wno-deprecated)

# Создание пакета deb
# cmake --build . --target package
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT darek@bk.ru)
include(CPack)