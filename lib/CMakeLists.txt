add_library(fp_poly STATIC src/fp_poly.c)
target_include_directories(fp_poly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(fp_poly PRIVATE ${GMP_LIBRARIES})
target_link_libraries(fp_poly PRIVATE list_lib)

add_library(fp_integer STATIC src/fp_integer.c)
target_include_directories(fp_integer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(fp_integer PRIVATE ${GMP_LIBRARIES})
# target_link_libraries(fp_integer PRIVATE list_lib)