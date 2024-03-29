########################################################################
#
# CMake build script for ApplicationRegistration.
#

cmake_minimum_required(VERSION 3.21)

project(ApplicationRegistration LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER clang++)



set(CPACK_PACKAGE_NAME ApplicationRegistration)
set(CPACK_PACKAGE_VERSION 1.0.0.0)
set(CPACK_SYSTEM_NAME linux)

set(QT6_VERSION 6.2.4)

# Where ApplicationRegistration's .h files can be found.
set(app_reg_include_dirs
    include/ApplicationRegistration)

# Heap's .h files
set(app_reg_headers
    include/ApplicationRegistration/ApplicationRegistration.h)

find_package(Qt6 ${QT6_VERSION} EXACT REQUIRED COMPONENTS DBus)
find_package(Qt6 ${QT6_VERSION} EXACT REQUIRED COMPONENTS Core)


add_library(ApplicationRegistration STATIC
    ${app_reg_headers}
    src/ApplicationRegistration.cpp
)
target_link_libraries(ApplicationRegistration PUBLIC
    Qt6::Core
    Qt6::DBus
)
target_include_directories(ApplicationRegistration PRIVATE
    ${app_reg_include_dirs}
)
set_target_properties(ApplicationRegistration PROPERTIES PUBLIC_HEADER  ${app_reg_headers})

add_executable(DemoAppReg
    src/DemoAppReg.cpp
)
target_link_libraries(DemoAppReg PRIVATE ApplicationRegistration)
target_include_directories(DemoAppReg PRIVATE
    ${app_reg_include_dirs}
)

add_executable(DBusService
    src/DBusService.cpp
)
target_link_libraries(DBusService PRIVATE ApplicationRegistration)
target_include_directories(DBusService PRIVATE
    ${app_reg_include_dirs}
)

add_executable(TestProg
    src/TestProg.cpp)

include(GNUInstallDirs)
install(TARGETS ApplicationRegistration DemoAppReg DBusService TestProg
    LIBRARY         DESTINATION lib/x86_64-linux-gnu
    PUBLIC_HEADER   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ApplicatinRegistration
    RUNTIME         DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#set(CMAKE_INSTALL_PREFIX "/home/scientist73/projects/ApplicationRegistration/install")
set(CMAKE_INSTALL_PREFIX "/usr")


include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_CONTACT "yusupovdanil2807@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "scientist73")
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_VERSION_TWEAK 0)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt6-base-dev (>= ${QT6_VERSION}), libdbus-1-3, libglib2.0-dev")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "This framework registers the application in the linux OS")
#set(CPACK_PACKAGE_DIRECTORY "/home/scientist73/projects/ApplicationRegistration/install")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")

set(CPACK_DEBIAN_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_DEBIAN_PACKAGE_RELEASE 1)

include(CPack)
