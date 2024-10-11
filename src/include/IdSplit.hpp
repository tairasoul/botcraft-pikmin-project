#pragma once
#include <botcraft/Game/World/World.hpp>
#include <protocolCraft/AllClientboundMessages.hpp>

class IdSplittedWorld : public Botcraft::World 
{
    public:
        IdSplittedWorld(bool is_shared);
    protected:
        void Handle(const ProtocolCraft::ClientboundLevelChunkPacketData& packet);
        void Handle(const ProtocolCraft::ClientboundBlockUpdatePacket& packet);
};