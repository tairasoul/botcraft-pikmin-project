#include <global-command-layout.hpp>
#include <PikminClass.hpp>
#include <botcraft/Game/World/World.hpp>
#include <botcraft/Game/Entities/LocalPlayer.hpp>
#include <botcraft/Game/Vector3.hpp>
#include <botcraft/AI/Tasks/AllTasks.hpp>
#include <botcraft/Game/Inventory/InventoryManager.hpp>
#include <botcraft/Game/Inventory/Window.hpp>
#include <botcraft/Game/Inventory/Item.hpp>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include <numeric>
#include <IdSplit.hpp>
#include <BlockMap.hpp>

class GoMineBlock: public Command {
    public:
        GoMineBlock() {
            name = "mine";
            func = std::bind(&GoMineBlock::mineBlock, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        }
    private:
        Vector3<double> returnPos;
        Botcraft::Status EquipBestTool(std::shared_ptr<PikminClient> client, std::string block_id) {
            std::shared_ptr<InventoryManager> inventory = client->GetInventoryManager();
            const std::map<short, ProtocolCraft::Slot> inventory_slots = inventory->GetPlayerInventory()->GetSlots();
            return Botcraft::Status::Success;
        }

        void mineBlock(std::shared_ptr<PikminClient> client, std::shared_ptr<IdSplittedWorld> world, ProtocolCraft::UUID origin, GlobalData global_data, std::vector<std::string> arguments) {
            returnPos = client->GetLocalPlayer()->GetPosition();
            if (arguments.size() == 0) {
                if (client->id == 0) {
                    client->SendChatMessage("mine requires a block id (ex. minecraft:oak_log) and, optionally, amount to mine! amount to mine defaults to 50");
                }
                return;
            }
            std::string block_id = arguments[0];
            int amt = 50;
            if (arguments.size() == 2) {
                amt = std::atoi(arguments[1].c_str());
            }
            std::vector<Botcraft::Position> Blocks = filterClosestToFurthest(FindBlocksWithId(world, block_id, amt), returnPos);
            if (amt > Blocks.size())
                amt = Blocks.size();
            std::vector<std::shared_ptr<Botcraft::BehaviourTree<PikminClient>>> trees;
            int currentBehaviour = 0;
            for (int i = 0; i < amt; i++) {
                Botcraft::Position blockPos = Blocks[i];
                auto tree = Builder<PikminClient>("block mine tree")
                    .sequence()
                        .leaf("dig_block", [&](PikminClient& c) { return Dig(c, blockPos); })
                        .leaf([&](PikminClient& c) { c.SetBehaviourTree(nullptr); c.SetBehaviourTree(trees[++currentBehaviour]); return Status::Success; })
                    .end();
                trees.push_back(tree);
            }
            auto endTree = Builder<PikminClient>("return to start pos")
                .sequence()
                    .leaf("go to start pos", [&](PikminClient& c) { return GoTo(c, returnPos); })
                    .leaf([&](PikminClient& c) { c.SetBehaviourTree(nullptr); return Status::Success; })
                .end();
            trees.push_back(endTree);
            client->SetBehaviourTree(trees[currentBehaviour]);
        }

        double distance(const Botcraft::Position& p1, double x, double y, double z) {
            return std::sqrt(std::pow(p1.x - x, 2) + std::pow(p1.y - y, 2) + std::pow(p1.z - z, 2));
        }

        std::vector<size_t> sortedIndices(const std::vector<double>& distances) {
            std::vector<size_t> indices(distances.size());
            std::iota(indices.begin(), indices.end(), 0);
            std::sort(indices.begin(), indices.end(), [&distances](size_t i, size_t j) {
                return distances[i] < distances[j];
            });
            return indices;
        }

        std::vector<Botcraft::Position> filterClosestToFurthest(const std::vector<Botcraft::Position>& points, Botcraft::Position target) {
            std::vector<double> distances(points.size());
            // Calculate distances of all points from the target point
            for (size_t i = 0; i < points.size(); ++i) {
                distances[i] = distance(points[i], target.x, target.y, target.z);
            }
            // Get sorted indices based on distances
            std::vector<size_t> sortedIdx = sortedIndices(distances);
            // Rearrange points based on sorted indices
            std::vector<Botcraft::Position> sortedPoints(points.size());
            for (size_t i = 0; i < points.size(); ++i) {
                sortedPoints[i] = points[sortedIdx[i]];
            }
            return sortedPoints;
        }

        std::vector<Botcraft::Position> FindBlocksWithId(std::shared_ptr<IdSplittedWorld> world, std::string id, int max) {
            std::vector<Botcraft::Position> Positions = world->blockMap.GetPositions(id);
            
            return Positions;
        }
};

extern "C" void getCommandInstances(std::vector<std::shared_ptr<Command>>& commands) {
    commands.push_back(std::make_shared<GoMineBlock>());
}