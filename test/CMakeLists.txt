# (uncomment these lines for an external project)
#
# cmake_minimum_required(VERSION 2.6)
# project(Bar)
# find_package(Foo REQUIRED)
# include_directories(${FOO_INCLUDE_DIRS})

add_executable(clog-test test.cpp)

target_link_libraries(clog-test ${LIBRARY_NAME})

find_package(GTest REQUIRED)
target_link_libraries(clog-test pthread ${GTEST_BOTH_LIBRARIES})

#install(TARGETS test-runningstats
# In order to export target, uncomment next line
#   EXPORT ${PROJECT_EXPORT}
#  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
