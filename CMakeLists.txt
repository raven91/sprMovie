cmake_minimum_required(VERSION 3.8)
project(sprMovie)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        main.cpp
        ScreenshotHandling.cpp
        Rendering/Renderer.cpp Rendering/Renderer.hpp
        Definitions.hpp
        Parameters/ParameterContainer.cpp
        Parameters/ParameterContainer.hpp
        Rendering/ScreenshotHandler.cpp Rendering/ScreenshotHandler.hpp
        Rendering/RendererExperimental.cpp Rendering/RendererExperimental.hpp
        Rendering/RendererSynthetic.cpp Rendering/RendererSynthetic.hpp
        Models/Model.cpp Models/Model.hpp
        Models/SimulationModel.cpp Models/SimulationModel.hpp
        Models/SimulationModelWithMisdetections.cpp Models/SimulationModelWithMisdetections.hpp
        Parameters/ParameterContainerWithMisdetections.cpp Parameters/ParameterContainerWithMisdetections.hpp
        Models/SimulationModelAfterTracking.cpp Models/SimulationModelAfterTracking.hpp
        Parameters/ParameterContainerExperimental.cpp Parameters/ParameterContainerExperimental.hpp
        Models/ModelBasedOnExperiment.cpp Models/ModelBasedOnExperiment.hpp)
add_executable(sprMovie ${SOURCE_FILES})

include_directories(/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/ /usr/local/include/ /usr/local/include/freetype2/)
link_directories(/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib/ /usr/local/lib/)
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)

file(GLOB LIBRARIES "/usr/local/lib/libGLEW.1.13.0.dylib"
        "/usr/local/lib/libglfw.3.1.dylib"
        "/usr/local/lib/libpng16.16.dylib"
        "/usr/local/lib/libfreetype.a")
message("LIBRARIES = ${LIBRARIES}")
target_link_libraries(sprMovie ${LIBRARIES})

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-framework OpenGL" COMPILER_SUPPORTS_FRAMEWORK_OPENGL)
if (COMPILER_SUPPORTS_FRAMEWORK_OPENGL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL")
endif ()
#[[CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()
CHECK_CXX_COMPILER_FLAG("-O3" COMPILER_SUPPORTS_O3)
if(COMPILER_SUPPORTS_O3)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
CHECK_CXX_COMPILER_FLAG("-flto" COMPILER_SUPPORTS_FLTO)
if(COMPILER_SUPPORTS_FLTO)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
endif()
]]