get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(MOVEIO_INCLUDE_DIRS "${SELF_DIR}/../../include/moveio" ABSOLUTE)
set(MOVEIO_LIBRARY "${SELF_DIR}/libmoveio.so")

