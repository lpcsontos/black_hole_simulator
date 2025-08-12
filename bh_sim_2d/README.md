possible bug on Ubuntu or Linux based system is sdl,
the solution might be to add this to the cmakelists.txt: target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE /usr/include/SDL2)
