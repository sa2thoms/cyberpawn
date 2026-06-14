add_executable(cyberpawnProfiler
    cyberpawnProfiler/main.cpp
)

target_link_libraries(cyberpawnProfiler PRIVATE cyberpawn)

target_compile_features(cyberpawnProfiler PRIVATE cxx_std_20)
