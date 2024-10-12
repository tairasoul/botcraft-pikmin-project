#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <IdSplit.hpp>

class GlobalData {
    public:
        GlobalData();
        std::unordered_map<std::string, std::any> data;
};