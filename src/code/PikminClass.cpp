#include <PikminClass.hpp>
#include <GlobalData.hpp>

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    // Add the remaining substring after the last delimiter
    if (start < str.size()) {
        tokens.push_back(str.substr(start));
    }

    return tokens;
}

PikminClient::PikminClient(GlobalData data, int id) : Botcraft::TemplatedBehaviourClient<PikminClient>(false)
{
    global_data = data;
    this->id = id;
    commands = std::vector<std::shared_ptr<Command>>();
}

PikminClient::~PikminClient() {}

void PikminClient::Handle(ProtocolCraft::ClientboundPlayerChatPacket& packet) {
    if (packetIsCommand(packet)) {
        executeCommand(packet);
    }
}

bool PikminClient::hasCommand(std::string name) {
    for (int i = 0; i < commands.size(); i++) {
        if (commands[i]->name == name)
            return true;
    }
    return false;
}

bool PikminClient::packetIsCommand(const ProtocolCraft::ClientboundPlayerChatPacket& msg) {
    std::vector<std::string> arguments = splitString(msg.GetBody().GetContent(), ' ');
    return hasCommand(arguments[0]);
}

bool PikminClient::executeCommand(const ProtocolCraft::ClientboundPlayerChatPacket& msg) {
    std::vector<std::string> arguments = splitString(msg.GetBody().GetContent(), ' ');
    if (hasCommand(arguments[0])) {
        for (int i = 0; i < commands.size(); i++) {
            if (commands[i]->name == arguments[0])
            {
                commands[i]->func(this, msg.GetSender(), global_data, std::vector<std::string>(arguments.begin() + 1, arguments.end()));
                return true;
            }
        }
    }
    return false;
}