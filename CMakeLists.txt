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

set(CPACK_PACKAGE_NAME ApplicationRegistration)
set(CPACK_PACKAGE_VERSION 1.0.0.0)
set(CPACK_SYSTEM_NAME linux)

# Where ApplicationRegistration's .h files can be found.
set(app_reg_include_dirs
    include/ApplicationRegistration)

# Heap's .h files
set(app_reg_headers
    include/ApplicationRegistration/ApplicationRegistration.h)

find_package(Qt6 REQUIRED COMPONENTS DBus)
find_package(Qt6 REQUIRED COMPONENTS Core)

set_target_properties(Qt6::Core PROPERTIES
    OUTPUT_NAME "common"
)
set_target_properties(Qt6::DBus PROPERTIES
    OUTPUT_NAME "common"
)




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

add_executable(MAIN
  src/main.cpp
)
target_link_libraries(MAIN PRIVATE ApplicationRegistration)
target_include_directories(MAIN PRIVATE
    ${app_reg_include_dirs}
)


include(GNUInstallDirs)
install(TARGETS ApplicationRegistration MAIN
    LIBRARY         DESTINATION lib/x86_64-linux-gnu
    PUBLIC_HEADER   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ApplicatinRegistration
    RUNTIME         DESTINATION ${CMAKE_INSTALL_BINDIR}/ApplicationRegistration
)
install(DIRECTORY ${CMAKE_PREFIX_PATH}/lib/
    DESTINATION lib/x86_64-linux-gnu
    PATTERN "*.so"
    PATTERN "cmake" EXCLUDE
    PATTERN "objects-RelWithDebInfo" EXCLUDE
    PATTERN "pkgconfig" EXCLUDE
)
#install(IMPORTED_RUNTIME_ARTIFACTS Qt6::Core Qt6::DBus
#    LIBRARY         DESTINATION ${CMAKE_INSTALL_LIBDIR}/ApplicationRegistration)

set(CMAKE_INSTALL_PREFIX "/home/scientist73/projects/ApplicationRegistration/install")



include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_CONTACT "yusupovdanil2807@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "scientist73")
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_VERSION_TWEAK 0)

#set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt6-base-dev (>= 6.6)")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libdbus-1-3, libglib2.0-dev")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "This framework registers the application in the linux OS")
#set(CPACK_PACKAGE_DIRECTORY "/home/scientist73/projects/ApplicationRegistration/install")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")

set(CPACK_DEBIAN_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_DEBIAN_PACKAGE_RELEASE 1)

include(CPack)
