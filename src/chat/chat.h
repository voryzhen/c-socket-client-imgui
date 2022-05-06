#pragma once

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <string>

namespace rv_chat
{

class RVChat{

public:

    RVChat();

private:

    std::string GLSLversion;
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context;
    ImGuiIO io;

    // Our state
    bool show_demo_window;
    bool show_another_window;
    ImVec4 clear_color;
    bool done = false;

    const char* program_name = "RVChat";

    int init_sdl();
    void setup_shader();
    int init_window_and_context ();
    void imgui_setup ();
    void load_fonts();
    void total_clean_up();
    void do_render();

    void set_program_main_state();

    void handle_events();
    void prepare_windows();
    void create_sidebar();

    int run_chat();

};

};