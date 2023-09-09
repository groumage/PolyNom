add_library(fp_poly STATIC src/fp_poly.c)
target_include_directories(fp_poly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(fp_poly PRIVATE ${GMP_LIBRARIES})
target_link_libraries(fp_poly PRIVATE list_lib)