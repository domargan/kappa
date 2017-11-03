set(KAPPA_CORE ${CMAKE_SOURCE_DIR}/core)
set(KAPPA_APPS ${CMAKE_SOURCE_DIR}/applications)


add_subdirectory(core)
add_subdirectory(applications)

include_directories(core)