cmake_minimum_required(VERSION 2.8)
project(kdevcpprefactor)

find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)

include(UseQt4)
include(KDE4Defaults)
add_subdirectory(Test)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DKDE_DEFAULT_DEBUG_AREA=9043)
add_definitions("-fexceptions")

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

qt4_wrap_ui(GUI_SOURCES ui/extractfunctiondialog.ui)
set(kdevcpprefactor_PART_SRCS
    ui/extractfunctiondialog.cpp
    refactorplugin.cpp
)

kde4_add_plugin(kdevcpprefactor ${kdevcpprefactor_PART_SRCS} ${GUI_SOURCES})

target_link_libraries(kdevcpprefactor
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    cppmanip
)

configure_file(kdevcpprefactor.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/kdevcpprefactor.desktop)

install(TARGETS kdevcpprefactor DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kdevcpprefactor.desktop DESTINATION ${SERVICES_INSTALL_DIR})
