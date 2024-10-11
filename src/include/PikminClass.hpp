#pragma once

#include <GlobalData.hpp>
#include "botcraft/AI/SimpleBehaviourClient.hpp"
#include <botcraft/Game/Entities/entities/player/PlayerEntity.hpp>
#include <botcraft/Game/Vector3.hpp>
#include <string>
#include <unordered_map>
#include <functional>
#include <protocolCraft/AllClientboundMessages.hpp>
#include <Loader.hpp>
#include <IdSplit.hpp>
using namespace Botcraft;

class PikminClient : public Botcraft::TemplatedBehaviourClient<PikminClient> {
    public:
        PikminClient(GlobalData data, int id);
        ~PikminClient();
        void addCommand(std::string name, std::function<void(PikminClient* client, ProtocolCraft::UUID origin, std::vector<std::string> arguments)> func);
        bool hasCommand(std::string name);
        bool packetIsCommand(const ProtocolCraft::ClientboundPlayerChatPacket& msg);
        bool executeCommand(const ProtocolCraft::ClientboundPlayerChatPacket& msg);
        std::shared_ptr<IdSplittedWorld> GetWorld();
        std::vector<std::shared_ptr<Command>> commands;
        int id = 0;
    private:
        GlobalData global_data;
    protected:
        virtual void Handle(ProtocolCraft::ClientboundPlayerChatPacket& packet);
};