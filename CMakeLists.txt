project(Starfield)

add_executable(starfield main.cc)
target_link_libraries(starfield user32.lib gdi32.lib)