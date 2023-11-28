project(railway-sys)

file(GLOB SOURCE_FILES *.h *.cpp)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
    assets/app.rc
    assets/app.qrc
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${ROOT_INCDIR}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
