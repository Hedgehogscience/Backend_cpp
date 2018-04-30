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

// Keep the common strings on the same cacheline.
enum class ResponseID { Sucess, Invalid, Failure, Duplicate, Usage, Max};
std::string Responses[static_cast<size_t>(ResponseID::Max)] =
{
    "{ \"Result\": \"Sucess\" }",
    "{ \"Result\": \"Invalid\" }",
    "{ \"Result\": \"Failure\" }",
    "{ \"Result\": \"Duplicate\" }",
    "{ \"Endpoints\": [ \"/Account/Login\", \"/Account/Verify\", \"/Account/Register\" ] }"
};

// Prerendered HTML pages.
enum class PageID { Login, Max};
std::unique_ptr<std::string> Renderedpages[static_cast<size_t>(PageID::Max)];

// Unhandled request return usage info.
void Unhandledrequest(evpp::EventLoop *Loop, const evpp::http::ContextPtr &Context, const evpp::http::HTTPSendResponseCallback &Callback)
{
    Context->AddResponseHeader("content-type", "application/json");
    Callback(Responses[static_cast<size_t>(ResponseID::Usage)]);
}

// Return the authentication page.
void Loginrequest(evpp::EventLoop *Loop, const evpp::http::ContextPtr &Context, const evpp::http::HTTPSendResponseCallback &Callback)
{
    Context->AddResponseHeader("content-type", "text/html");
    Callback((*Renderedpages[static_cast<size_t>(PageID::Login)]));
}

// Verify that a key exists in the database.
void Verificationrequest(evpp::EventLoop *Loop, const evpp::http::ContextPtr &Context, const evpp::http::HTTPSendResponseCallback &Callback)
{
    Context->AddResponseHeader("content-type", "application/json");

    auto Publickey = Context->GetQuery("key");
    if(!Base64::Validate(Publickey)) return Callback(Responses[static_cast<size_t>(ResponseID::Invalid)]);

    bool Databaseresult{};
    /* TODO(Convery): Query the database */

    if(Databaseresult) return Callback(Responses[static_cast<size_t>(ResponseID::Sucess)]);
    else return Callback(Responses[static_cast<size_t>(ResponseID::Failure)]);
}

// Register the new key.
void Registrationrequest(evpp::EventLoop *Loop, const evpp::http::ContextPtr &Context, const evpp::http::HTTPSendResponseCallback &Callback)
{
    


    Context->AddResponseHeader("content-type", "application/json");
    Callback(Responses[static_cast<size_t>(ResponseID::Invalid)]);
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
    WSAData WD; WSAStartup(MAKEWORD(2, 2), &WD);
    #endif

    // Create the new server.
    evpp::http::Server Server(Threadcount);
    Server.RegisterDefaultHandler(&Unhandledrequest);    
    Server.RegisterHandler("/Account/Login", Loginrequest);
    Server.RegisterHandler("/Account/Verify", Verificationrequest);
    Server.RegisterHandler("/Account/Register", Registrationrequest);
    Server.SetThreadDispatchPolicy(evpp::ThreadDispatchPolicy::kRoundRobin);

    // Start the server.
    Server.Init(static_cast<int>(Common::Serviceports::Account));
    Server.Start();

    // Loop until terminated by the dev.
    while (true) 
    {
        static auto Readfile = [](std::string Filename) -> std::string 
        {
            if (auto Filehandle = std::fopen(Filename.c_str(), "rb"))
            {
                std::fseek(Filehandle, 0, SEEK_END);
                auto Length = std::ftell(Filehandle);
                std::fseek(Filehandle, 0, SEEK_SET);

                auto Buffer = std::make_unique<char[]>(Length + 1);
                std::fread(Buffer.get(), Length, 1, Filehandle);
                std::fclose(Filehandle);

                return std::string(std::move(Buffer.get()));
            }

            return "";
        };

        // Update the pages periodically.
       auto Locallogin = std::make_unique<std::string>(Readfile("Loginpage.html"));
       Renderedpages[static_cast<size_t>(PageID::Login)].swap(Locallogin);

        std::this_thread::sleep_for(std::chrono::minutes(5));
        /* TODO(Convery): Do some logging here. */
    }

    return 0;
}

