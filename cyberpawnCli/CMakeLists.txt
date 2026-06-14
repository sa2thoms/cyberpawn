add_executable(cyberpawnCli
    cyberpawnCli/CyberpawnCli.cpp
    cyberpawnCli/main.cpp
)

target_link_libraries(cyberpawnCli PRIVATE cyberpawn)

target_compile_features(cyberpawnCli PRIVATE cxx_std_20)
