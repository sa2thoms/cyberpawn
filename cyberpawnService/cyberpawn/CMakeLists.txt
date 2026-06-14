add_library(cyberpawn STATIC
    BetaTreeEngine.cpp
    ChessGameFunctions.cpp
    ChessPosition.cpp
)

target_include_directories(cyberpawn
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(cyberpawn PUBLIC cxx_std_20)
