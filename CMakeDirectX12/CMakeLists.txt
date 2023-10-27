# CMakeList.txt : CMake project for CMakeDirectX12, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (CMakeDirectX12 "CMakeDirectX12.cpp" "CMakeDirectX12.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeDirectX12 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
