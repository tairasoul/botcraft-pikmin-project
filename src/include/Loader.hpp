#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <filesystem>
#include <functional>
#include <dlfcn.h>
#include <GlobalData.hpp>
#include <protocolCraft/AllClientboundMessages.hpp>
class PikminClient;

using CommandFunction = std::function<void(PikminClient*, ProtocolCraft::UUID, GlobalData, std::vector<std::string>)>;

class Command {
    public:
        std::string name;
        CommandFunction func;
};

class CommandLoader {
    public:
        CommandLoader();
        void loadCommandsFromDirectory(const std::string& directory);
        CommandLoader(CommandLoader const&) = delete;
        void operator=(CommandLoader const&) = delete;
        std::vector<std::shared_ptr<Command>> commands;
    private:
        void loadCommandsFromLibrary(const std::string& libraryPath);
};