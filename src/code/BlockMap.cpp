#include <BlockMap.hpp>

BlockMap::BlockMap() {}
BlockMap::~BlockMap() {}

void BlockMap::SetPosition(std::string blockId, Botcraft::Position pos) {
    if (internal_map.contains(blockId)) {
        std::vector<Botcraft::Position> positions = internal_map[blockId];
        for (const auto& pair : internal_map) {
            for (int i = 0; i < pair.second.capacity(); i++) {
                if (pair.second[i].x == pos.x && pair.second[i].y == pos.y && pair.second[i].z == pos.z)
                    RemovePosition(pair.first, pair.second[i]);
            }
        }
        positions.emplace_back(pos);
        internal_map[blockId] = positions;
    }
    else {
        std::vector<Botcraft::Position> positions;
        positions.emplace_back(pos);
        internal_map[blockId] = positions;
    }
}

void BlockMap::RemovePosition(std::string blockId, Botcraft::Position pos) {
    if (internal_map.contains(blockId)) {
        std::vector<Botcraft::Position> positions = internal_map[blockId];
        std::vector<Botcraft::Position> filtered;
        for (int i = 0; i < positions.size(); i++) {
            if (positions[i].x == pos.x && positions[i].y == pos.y && positions[i].z == pos.z)
                continue;
            filtered.emplace_back(positions[i]);
        }
        internal_map[blockId] = filtered;
    }
}

std::vector<Botcraft::Position> BlockMap::GetPositions(std::string blockId) {
    if (internal_map.contains(blockId))
        return internal_map[blockId];
    return std::vector<Botcraft::Position>();
}

std::vector<Botcraft::Position> BlockMap::GetPositionsCloseTo(std::string blockId, Botcraft::Position pos) {
    if (internal_map.contains(blockId)) {
        std::vector<Botcraft::Position> positions = internal_map[blockId];
        std::sort(positions.begin(), positions.end(), [&pos](const Botcraft::Position& a, const Botcraft::Position& b) {
            return a.SqrDist(pos) < b.SqrDist(pos);
        });
        return positions;
    }
    return std::vector<Botcraft::Position>();
}