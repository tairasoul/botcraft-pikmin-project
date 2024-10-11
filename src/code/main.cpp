#include <PikminClass.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <rapidjson/document.h>
#include "rapidjson/error/en.h"
#include <Loader.hpp>
#include <botcraft/Game/World/World.hpp>
#include <IdSplit.hpp>

void ShowHelp(const char* argv0)
{
    std::cout << "Usage: " << argv0 << " <options>\n"
        << "Options:\n"
        << "\t-h, --help\tShow this help message\n"
        << "\t--address\tAddress of the server you want to connect to, default: 127.0.0.1:25565\n"
        << "\t--bots\tThe amount of \"pikmin\" you want. Default: 4\n"
        << "\t--logins\t\tPath to logins file relative to current working directory. Default: logins.json\n"
        << std::endl;
}

struct Args
{
    bool help = false;
    std::string address = "127.0.0.1:25565";
    int bot_count = 4;
    std::string logins = "logins.json";

    int return_code = 0;
};

struct Login {
    Botcraft::LoginType LoginType = Botcraft::LoginType::Offline;
    std::string login = "Pikmin";
};

Args ParseCommandLine(int argc, char* argv[]);

std::vector<Login> ParseLogins(std::string logins_path);

namespace fs = std::filesystem;

fs::path resolvePathRelativeToCurrentDirectory(const std::string& inputPath) {
    fs::path fullPath = fs::absolute(inputPath);
    if (!fs::exists(fullPath)) {
        fullPath = fs::absolute(fs::path(fs::current_path()) / inputPath);
    }
    return fullPath.lexically_normal();
}

void loadCommandsIntoClient(std::shared_ptr<PikminClient> client, CommandLoader& loader) {
    for (const auto& command : loader.commands) {
        client->commands.push_back(command);
    }
}

int main(int argc, char* argv[])
{
    try {
        // Init logging, log everything >= Info, only to console, no file
        Botcraft::Logger::GetInstance().SetLogLevel(Botcraft::LogLevel::Info);
        Botcraft::Logger::GetInstance().SetFilename("");
        // Add a name to this thread for logging
        Botcraft::Logger::GetInstance().RegisterThread("main");

        Args args;
        if (argc == 1)
        {
            LOG_WARNING("No command arguments. Using default options.");
            ShowHelp(argv[0]);
            return 0;
        }
        else
        {
            args = ParseCommandLine(argc, argv);
            if (args.help)
            {
                ShowHelp(argv[0]);
                return 0;
            }
            if (args.return_code != 0)
            {
                return args.return_code;
            }
        }
        std::vector<Login> Logins = ParseLogins(args.logins);
        std::vector<std::shared_ptr<PikminClient>> clients(args.bot_count);
        std::vector<std::thread> behaviourThreads(args.bot_count);
        int worlds = 1 + args.bot_count / 8;
        std::vector<std::shared_ptr<IdSplittedWorld>> shared_worlds(worlds);
        for (int i = 0; i < worlds; i++) {
            shared_worlds[i] = std::make_shared<IdSplittedWorld>(true);
        }
        CommandLoader loader = CommandLoader();
        GlobalData global = GlobalData();
        loader.loadCommandsFromDirectory(resolvePathRelativeToCurrentDirectory("commands"));
        for (int i = 0; i < args.bot_count; i++) {
            std::shared_ptr<Botcraft::World> shared_world = shared_worlds[i / 8];
            clients[i] = std::make_shared<PikminClient>(global, i);
            clients[i]->SetAutoRespawn(true);
            clients[i]->SetSharedWorld(shared_world);
            clients[i]->Connect(args.address, Logins[i].login, Logins[i].LoginType);
            clients[i]->StartBehaviour();
            loadCommandsIntoClient(clients[i], loader);
        }
        for (int i = 0; i < args.bot_count; i++) {
            behaviourThreads[i] = std::thread([&, i]() {
                clients[i]->RunBehaviourUntilClosed();
            });
        }
        for (int i = 0; i < args.bot_count; i++) {
            behaviourThreads[i].join();
        }
        return 0;
    }
    catch (std::exception& e)
    {
        LOG_FATAL("Exception: " << e.what());
        return 1;
    }
    catch (...)
    {
        LOG_FATAL("Unknown exception");
        return 2;
    }
}

std::vector<Login> ParseLogins(std::string logins_path) {
    fs::path resolved = resolvePathRelativeToCurrentDirectory(logins_path);
    std::ifstream fileStream(resolved);
    if (fileStream.is_open()) {
        std::string fileContent((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
        rapidjson::Document doc;
        doc.Parse(fileContent.c_str());
        if (doc.HasParseError()) {
            LOG_ERROR("JSON parse error: " << rapidjson::GetParseError_En(doc.GetParseError()));
            return {};
        }
        else if (doc.IsArray()) {
            std::vector<Login> logins(doc.Size());
            for (rapidjson::SizeType i = 0; i < doc.Size(); ++i) {
                const rapidjson::Value& loginObject = doc[i];
                if (!loginObject.IsObject()) {
                    LOG_ERROR("Invalid login object at index " << i);
                    continue;
                }
                Login login;
                std::string loginTypeStr = loginObject["login-type"].GetString();
                if (loginTypeStr == "mojang") {
                    login.LoginType = Botcraft::LoginType::Mojang;
                }
                else if (loginTypeStr == "easymc") {
                    login.LoginType = Botcraft::LoginType::EasyMC;
                }
                else {
                    login.LoginType = Botcraft::LoginType::Offline;
                }
                login.login = loginObject["login"].GetString();
                logins[i] = login;
            }
            return logins;
        }
        else {
            LOG_ERROR("Logins json is not an array at root!");
        }
    }
    return {};
}

Args ParseCommandLine(int argc, char* argv[])
{
    Args args;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help")
        {
            ShowHelp(argv[0]);
            args.help = true;
            return args;
        }
        else if (arg == "--address")
        {
            if (i + 1 < argc)
            {
                args.address = argv[++i];
            }
            else
            {
                LOG_FATAL("--address requires an argument");
                args.return_code = 1;
                return args;
            }
        }
        else if (arg == "--bots")
        {
            if (i + 1 < argc)
            {
                args.bot_count = std::stoi(argv[++i]);
            }
            else
            {
                LOG_FATAL("--bots requires an argument");
                args.return_code = 1;
                return args;
            }
        }
        else if (arg == "--logins") 
        {
            if (i + 1 < argc) {
                args.logins = argv[++i];
            }
            else
            {
                LOG_FATAL("--logins requires an argument");
                args.return_code = 1;
                return args;
            }
        }
    }
    return args;
}