#pragma once

#include <string>
#include <unordered_map>
#include <any>

class GlobalData {
    public:
        GlobalData();
        std::unordered_map<std::string, std::any> data;
};