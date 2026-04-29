set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library(<@ project_name @>
    "${INCLUDE_DIR}/<@ project_name @>/<@ project_name @>.hpp"
    "<@ project_name @>.cpp")
<@ project_name @>_setup_target(<@ project_name @>)

target_include_directories(<@ project_name @> PUBLIC ${INCLUDE_DIR})
