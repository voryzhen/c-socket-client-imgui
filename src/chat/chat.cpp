#include "chat.h"
//#include "chat_backend.h"

namespace rv_chat
{

SOCKET rv_chat::RVChat::Connection;

RVChat::RVChat()
{
    run_chat();
}

int RVChat::init_sdl ()
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    return 0;    
}

void RVChat::setup_shader ()
{

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    GLSLversion = glsl_version;

}

int RVChat::init_window_and_context ()
{
    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    window = SDL_CreateWindow(

        program_name,

        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags

    );

    if (window != nullptr)
    {
        gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync
        return 0;
    }
    else
    {
        return -1;
    }
}

void RVChat::imgui_setup ()
{
// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(GLSLversion.c_str());
}

void RVChat::total_clean_up()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void RVChat::load_fonts()
{

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

}

void RVChat::do_render()
{
    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void RVChat::set_program_main_state()
{
    // Our state
    show_demo_window = true;
    show_another_window = false;
    clear_color = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
}

void RVChat::handle_events ()
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            done = true;
    }    
}

int RVChat::run_chat()
{

    if ( init_sdl () != 0 ) return -1;
    setup_shader ();
    if ( init_window_and_context () != 0 ) return -1;
    imgui_setup();
    load_fonts();

    set_program_main_state();

    init_socet_lib();

    // Main loop
    while (!done)
    {
        handle_events();

        prepare_windows();

        do_render();

    }

    total_clean_up();

    return 0;
}

void RVChat::prepare_windows ()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    create_sidebar();

    create_chat_history();

    create_msg_field();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    // if (show_demo_window)
        // ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    // {
        // static float f = 0.0f;
        // static int counter = 0;

        // ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            // counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        // ImGui::End();
    // }

    // 3. Show another simple window.
    // if (show_another_window)
    // {
        // ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        // ImGui::Text("Hello from another window!");
        // if (ImGui::Button("Close Me"))
            // show_another_window = false;
        // ImGui::End();
    // }
}

void RVChat::create_chat_history()
{
    ImGui::SetNextWindowPos(ImVec2(440, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(820, 540), ImGuiCond_Once );
    ImGui::Begin("Chat History", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
    ImGui::Text("Server status: %s", this_session_history.c_str());
    ImGui::End();    
}

void RVChat::create_msg_field()
{
    ImGui::SetNextWindowPos(ImVec2(440, 580), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(820, 120), ImGuiCond_Once );
    
    ImGui::Begin("Message", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
    
    ImGui::InputText( "Enter Your Msg", my_msg, sizeof(my_msg) );
    ImGui::SameLine();
    if (ImGui::Button("Send"))
    {
        send_msg(std::string{my_msg});
    }
    ImGui::End();       
}

void RVChat::send_msg(std::string msg1)
{
    std::cout << "Want to send: " << msg1 << std::endl;
    int msg_size = (int)msg1.size();
    send(rv_chat::RVChat::Connection, (char*)&msg_size, sizeof(int), NULL);
    send(rv_chat::RVChat::Connection, msg1.c_str(), msg_size, NULL);
}

void RVChat::create_sidebar()
{

    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 680), ImGuiCond_Once );
    
    ImGui::Begin("Sidebar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

    int counter = 0;

    ImGui::InputText( "ServerIP", server_addr_buf, sizeof(server_addr_buf) );

    ImGui::SameLine();
    if (ImGui::Button("Connect"))
    {
        std::string addr{server_addr_buf};
        int is_connected = connect_to_server(addr);
        if (is_connected == 0)
        {
            server_connect_status = "connect";
        }
        else
        {
            server_connect_status = "some error";
        }
    }

    ImGui::Text("Server status: %s", server_connect_status.c_str());

    ImGui::End();

}






int RVChat::init_socet_lib()
{
    //WSAStartup
    WORD DLLVersion = MAKEWORD(2, 1);
    if(WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error" << std::endl;
        return -1;
    }
    return 0;
}

void RVChat::ClientHandler() {
    int msg_size;
    while(true) {
        recv(rv_chat::RVChat::Connection, (char*)&msg_size, sizeof(int), NULL);
        char *msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(rv_chat::RVChat::Connection, msg, msg_size, NULL);
        this_session_history += std::string{msg};
        this_session_history += '\n';
        this_session_history += '\n';
        std::cout << msg << std::endl;
        delete[] msg;
    }
}

int RVChat::connect_to_server(std::string input_addr)
{
    int sizeofaddr = sizeof(addr);
    // addr.sin_addr.s_addr = inet_addr("192.168.0.101");
    addr.sin_addr.s_addr = inet_addr(input_addr.c_str());
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    rv_chat::RVChat::Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if(connect(rv_chat::RVChat::Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
        std::cout << "Error: failed connect to server.\n";
        return -1;
    }
    std::cout << "Connected!\n";
    // startMyThread();
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)(&RVChat::ClientHandler), NULL, NULL, NULL);
    return 0;
}





};