#pragma once
#include <3ds.h>
#include <array>
#include <string>

struct SettingsData {
    char dest_ip[16];
    char frequency[4];
};

namespace Settings {
    extern const std::array<std::string, 2> folders;
    extern const std::string file_name;
    void save();
    void load();
    bool create_directory(const std::string& folder);
};