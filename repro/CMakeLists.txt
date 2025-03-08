enable_language(CXX)

add_executable(kodi_repro kodi_repro.cpp)
target_link_libraries(kodi_repro nfs)

add_executable(mini_repro mini_repro.cpp)
target_link_libraries(mini_repro nfs)
