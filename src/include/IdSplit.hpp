#pragma once
#include <botcraft/Game/World/World.hpp>
#include <protocolCraft/AllClientboundMessages.hpp>
#include <botcraft/Game/Vector3.hpp>
#include <botcraft/Game/World/Chunk.hpp>
#include <BlockMap.hpp>

class IdSplittedWorld : public Botcraft::World 
{
    public:
        IdSplittedWorld(bool is_shared);
        BlockMap blockMap;
    private:
        void SetBlockImpl(const Botcraft::Position& pos, const Botcraft::BlockstateId id);
};