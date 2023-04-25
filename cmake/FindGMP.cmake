# This module finds the GNU Multiple Precision Arithmetic Library (GMP)

# Find the GMP library
find_path(GMP_INCLUDE_DIR gmp.h PATH_SUFFIXES gmp)
find_library(GMP_LIBRARIES NAMES gmp libgmp)

# Check if GMP was found
if(GMP_INCLUDE_DIR AND GMP_LIBRARIES)
    set(GMP_FOUND TRUE)
endif()

# Provide an imported target for GMP
if(GMP_FOUND)
    add_library(gmp INTERFACE IMPORTED)
    set_property(TARGET gmp PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GMP_INCLUDE_DIR})
    set_property(TARGET gmp PROPERTY INTERFACE_LINK_LIBRARIES ${GMP_LIBRARIES})
endif()

# Export the GMP_INCLUDE_DIR and GMP_LIBRARIES variables
set(GMP_INCLUDE_DIRS ${GMP_INCLUDE_DIR} CACHE PATH "Path to GMP include directory")
set(GMP_LIBRARIES ${GMP_LIBRARIES} CACHE FILEPATH "Path to GMP library")
mark_as_advanced(GMP_INCLUDE_DIR GMP_LIBRARIES)
