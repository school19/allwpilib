project(wpilibc)

find_package( OpenCV REQUIRED )

configure_file(src/generate/WPILibVersion.cpp.in WPILibVersion.cpp)

file(GLOB_RECURSE
    wpilibc_native_src src/main/native/cpp/*.cpp)

add_library(wpilibc ${wpilibc_native_src} ${CMAKE_CURRENT_BINARY_DIR}/WPILibVersion.cpp)

target_include_directories(wpilibc PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/native/include>
                            $<INSTALL_INTERFACE:${include_dest}/wpilibc>)
target_link_libraries(wpilibc PUBLIC cameraserver hal ntcore cscore wpiutil ${OpenCV_LIBS})

set_property(TARGET wpilibc PROPERTY FOLDER "libraries")

install(TARGETS wpilibc EXPORT wpilibc DESTINATION "${main_lib_dest}")
install(DIRECTORY src/main/native/include/ DESTINATION "${include_dest}/wpilibc")

if (MSVC)
    set (wpilibc_config_dir ${wpilib_dest})
else()
    set (wpilibc_config_dir share/wpilibc)
endif()

install(FILES wpilibc-config.cmake DESTINATION ${wpilibc_config_dir})
install(EXPORT wpilibc DESTINATION ${wpilibc_config_dir})
