include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(cyberpawnTest
    BetaTreeEngineTest.cpp
    ChessGameFunctionsTest.cpp
)

target_link_libraries(cyberpawnTest
    PRIVATE
        cyberpawn
        GTest::gtest_main
)

target_compile_features(cyberpawnTest PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(cyberpawnTest)
