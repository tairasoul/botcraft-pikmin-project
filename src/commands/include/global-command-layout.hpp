#pragma once
#include <Loader.hpp>
#include <iostream>

extern "C" void getCommandInstances(std::vector<std::shared_ptr<Command>>& commands);