set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library(<@ project_name @>
    "${INCLUDE_DIR}/<@ project_name @>/<@ project_name @>.hpp"
    "<@ project_name @>.cpp")
<@ lower(namespace) @>_setup_target(<@ project_name @>)

target_include_directories(<@ project_name @> PUBLIC ${INCLUDE_DIR})

include(GNUInstallDirs)
install(DIRECTORY "${INCLUDE_DIR}/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS <@ project_name @>
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
