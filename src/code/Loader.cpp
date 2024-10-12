#include <Loader.hpp>
#include <PikminClass.hpp>

CommandLoader::CommandLoader() {}

void CommandLoader::loadCommandsFromDirectory(const std::string& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".so") {
            loadCommandsFromLibrary(entry.path().string());
        }
    }
}

void CommandLoader::loadCommandsFromLibrary(const std::string& libraryPath) {
    void* handle = dlopen(libraryPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!handle) {
        LOG_ERROR("Error loading library: " << dlerror());
        return;
    }
    using GetCommandInstancesFunc = void(*)(std::vector<std::shared_ptr<Command>>& commands);
    auto getCommandInstances = reinterpret_cast<GetCommandInstancesFunc>(dlsym(handle, "getCommandInstances"));
    if (!getCommandInstances) {
        LOG_ERROR("Error getting symbol: " << dlerror());
        dlclose(handle);
        return;
    }
    getCommandInstances(commands);
    // libraries remain loaded, as the functions need to remain loaded too. no dlclose()
}