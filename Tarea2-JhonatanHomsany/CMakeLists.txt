# CMakeList.txt : CMake project for Tarea2-JhonatanHomsany, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Tarea2-JhonatanHomsany "Tarea2-JhonatanHomsany.cpp" "Tarea2-JhonatanHomsany.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tarea2-JhonatanHomsany PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
