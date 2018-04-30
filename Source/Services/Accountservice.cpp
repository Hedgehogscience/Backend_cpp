/*
    Initial author: Convery (tcn@ayria.se)
    Started: 30-04-2018
    License: MIT
    Notes:
        Provides account services.
*/

#define MODULENAME "Accountservice"

#include "_Common.hpp"
#include "../Stdinclude.hpp"
#include <evpp/http/http_server.h>

// Readable registrationpage.
std::string DefaultHTML = "<HTML><body><h1>Error =(</h1></body></HTML>";
std::unique_ptr<std::string> CurrentHTMLPage{ &DefaultHTML };
std::unique_ptr<std::string> PreviousHTMLPage;

// Landingpage request.
void Servelandingpage(evpp::EventLoop *Loop, const evpp::http::ContextPtr &Context, const evpp::http::HTTPSendResponseCallback &Callback)
{
    Context->AddResponseHeader("content-type", "text/html; charset=UTF-8");
    Callback(*CurrentHTMLPage);
}

// Start the server.
int main(int argc, const char **argv)
{
    // Clear the previous session.
    Clearlog();

    // See how many threads we should use.
    uint32_t Threadcount = 2;
    if (argc == 2)
    {
        Threadcount = std::atoi(argv[1]);
    }

    // Initialize winsock if we are on Windows.
    #if defined(_WIN32)
    WSAData WD;
    WSAStartup(1, &WD);
    #endif

    // Create the new server.
    evpp::http::Server Server(Threadcount);
    Server.SetThreadDispatchPolicy(evpp::ThreadDispatchPolicy::kIPAddressHashing);
    Server.RegisterDefaultHandler(&Servelandingpage);

    // Start the server.
    Server.Init(static_cast<int>(Common::Serviceports::Account));
    Server.Start();

    // Loop until terminated by the dev.
    while (true) 
    {
        // Reload the HTML from disk.
        if (auto Filehandle = std::fopen("Accountservice.html", "rb"))
        {
            std::fseek(Filehandle, 0, SEEK_END);
            auto Length = std::ftell(Filehandle);
            std::fseek(Filehandle, 0, SEEK_SET);

            auto Buffer = std::make_unique<char[]>(Length);
            std::fread(Buffer.get(), Length, 1, Filehandle);
            std::fclose(Filehandle);

            PreviousHTMLPage = std::make_unique<std::string>(Buffer.get());
            CurrentHTMLPage.swap(PreviousHTMLPage);
        }

        std::this_thread::sleep_for(std::chrono::minutes(5));
        /* TODO(Convery): Do some logging here. */
    }

    return 0;
}

