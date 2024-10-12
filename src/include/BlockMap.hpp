#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <botcraft/Game/Vector3.hpp>

class BlockMap {
    public:
        BlockMap();
        ~BlockMap();
        std::vector<Botcraft::Position> GetPositions(std::string blockId);
        std::vector<Botcraft::Position> GetPositionsCloseTo(std::string blockId, Botcraft::Position pos);
        void SetPosition(std::string blockId, Botcraft::Position pos);
        void RemovePosition(std::string blockId, Botcraft::Position pos);
    
    private:
        std::unordered_map<std::string, std::vector<Botcraft::Position>> internal_map;
};