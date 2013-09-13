project(kdev-cpp-clang-refactor)

find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)

include(KDE4Defaults)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DKDE_DEFAULT_DEBUG_AREA=9043)
add_definitions("-fexceptions")

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

qt4_wrap_ui(GUI_SOURCES ui/extractfunctiondialog.ui)
set(kdevcppclangrefactor_PART_SRCS
    ui/extractfunctiondialog.cpp
    refactorplugin.cpp
)

kde4_add_plugin(kdevcppclangrefactor ${kdevcppclangrefactor_PART_SRCS} ${GUI_SOURCES})

target_link_libraries(kdevcppclangrefactor
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    cppmanip
)

install(TARGETS kdevcppclangrefactor DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kdevcppclangrefactor.desktop DESTINATION ${SERVICES_INSTALL_DIR})