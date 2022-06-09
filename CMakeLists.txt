cmake_minimum_required(VERSION 3.8)

#set(PATCH_VERSION 3)
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
project(allocator VERSION 0.0.${PATCH_VERSION})

find_package(GTest)

add_executable(allocator allocator.cpp my_allocator.h my_list.h)
set_target_properties(allocator PROPERTIES
					CXX_STANDARD 17
					CXX_STANDARD_REQUIRED ON
)
target_compile_options(allocator PUBLIC -Wall -Wextra -pedantic -Werror -Wno-deprecated)

add_executable(my_allocator_test my_allocator_test.cpp)
set_target_properties(my_allocator_test PROPERTIES
					CXX_STANDARD 17
					CXX_STANDARD_REQUIRED ON
)
target_include_directories(my_allocator_test PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(my_allocator_test PRIVATE ${GTEST_BOTH_LIBRARIES} pthread)

add_executable(my_list_test my_list_test.cpp)
set_target_properties(my_list_test PROPERTIES
					CXX_STANDARD 17
					CXX_STANDARD_REQUIRED ON
)
target_include_directories(my_list_test PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(my_list_test PRIVATE ${GTEST_BOTH_LIBRARIES} pthread)
target_compile_options(my_list_test PUBLIC -Wall -Wextra -pedantic -Werror -Wno-deprecated)

# Создание пакета deb
# cmake --build . --target package
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT darek@bk.ru)
include(CPack)