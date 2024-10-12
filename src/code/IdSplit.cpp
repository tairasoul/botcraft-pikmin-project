#include <IdSplit.hpp>
#include <botcraft/Utilities/Logger.hpp>
#include <botcraft/Game/World/World.hpp>
#include <botcraft/Game/AssetsManager.hpp>

IdSplittedWorld::IdSplittedWorld(bool is_shared) : Botcraft::World(is_shared) {
    blockMap = BlockMap();
}

void IdSplittedWorld::SetBlockImpl(const Botcraft::Position& pos, const Botcraft::BlockstateId id) {
    const int chunk_x = static_cast<int>(std::floor(pos.x / static_cast<double>(16)));
    const int chunk_z = static_cast<int>(std::floor(pos.z / static_cast<double>(16)));

    const Botcraft::Blockstate* state = Botcraft::AssetsManager::getInstance().GetBlockstate(id);
    blockMap.SetPosition(state->GetName(), pos);
    auto it = terrain.find({ chunk_x, chunk_z });

    // Can't set block in unloaded chunk
    if (it == terrain.end())
    {
        return;
    }

    const Botcraft::Position set_pos(
        (pos.x % 16 + 16) % 16,
        pos.y,
        (pos.z % 16 + 16) % 16
    );

    it->second.SetBlock(set_pos, id);

#if USE_GUI
    // If this block is on the edge, update neighbours chunks
    UpdateChunk(chunk_x, chunk_z, pos);
#endif
}