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

#include "session_history.h"




#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#pragma warning(disable: 4996)






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

    char server_addr_buf[20] = "";
    char my_msg[256] = "";
    std::string server_connect_status = "waiting to connection";

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
    void create_chat_history();
    void create_msg_field();

    int run_chat();

    // backend

    WSAData wsaData;
    SOCKADDR_IN addr;

    int init_socet_lib();
    int connect_to_server(std::string);

public:

    static SOCKET Connection;
    static void ClientHandler();
    void send_msg(std::string msg1);

};

};