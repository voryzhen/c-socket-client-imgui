project(hello)

include_directories( ./vendor/SDL2/SDL2_win/include )

link_directories( ./vendor/SDL2/SDL2_win/lib/x64 )

add_executable( hello main.cpp )
target_link_libraries( hello SDL2 SDL2main )
