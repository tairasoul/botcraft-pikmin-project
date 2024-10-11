#include <IdSplit.hpp>
#include <botcraft/Utilities/Logger.hpp>
#include <botcraft/Game/World/World.hpp>

IdSplittedWorld::IdSplittedWorld(bool is_shared) : Botcraft::World(is_shared) {}

void IdSplittedWorld::Handle(const ProtocolCraft::ClientboundLevelChunkPacketData& packet) {
    std::vector<ProtocolCraft::BlockEntityInfo> entities = packet.GetBlockEntitiesData();
    LOG_INFO(entities[0].GetPackedXZ());
    LOG_INFO(entities[0].GetType());
}

void IdSplittedWorld::Handle(const ProtocolCraft::ClientboundBlockUpdatePacket& packet) {
    LOG_INFO(packet.GetName());
    LOG_INFO(packet.GetPos().GetX());
    LOG_INFO(packet.GetPos().GetY());
    LOG_INFO(packet.GetPos().GetZ());
}