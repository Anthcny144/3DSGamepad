#include <3ds.h>
#include "App.hpp"
#include "Console.hpp"
#include "Keyboard.hpp"
#include "Network.hpp"
#include "Settings.hpp"
#include "Utils.hpp"

namespace Settings {
    const std::array<std::string, 2> folders = {"/3ds", "/3ds/3DSGamepad"};
    const std::string full_path = "/3ds/3DSGamepad/settings.bin";

    void save() {
        for (const std::string& folder : folders)
            create_directory(folder);

        SettingsData data;
        memset(&data, 0, sizeof(data));

        std::string frequency_str = std::to_string(Network::frequency);

        strncpy(data.dest_ip, Network::dest_ip.c_str(), sizeof(data.dest_ip) - 1);
        strncpy(data.frequency, frequency_str.c_str(), sizeof(data.frequency) - 1);

        FILE* file = fopen(full_path.c_str(), "wb");
        if (file == nullptr)
            return;

        fwrite(&data, sizeof(data), 1, file);
        fclose(file);
    }

    void load() {
        FILE* file = fopen(full_path.c_str(), "rb");
        if (file == nullptr)
            return;

        SettingsData data;
        fread(&data, sizeof(data), 1, file);

        // Dest IP
        std::string dest_ip_str = std::string(data.dest_ip);
        if (Network::is_valid_ip(dest_ip_str)) {
            snprintf(Keyboard::written_dest_ip, sizeof(Keyboard::written_dest_ip), "%s", dest_ip_str.c_str());
            Network::dest_ip = dest_ip_str;
            Network::valid_dest_ip = true;
            Network::set_dest();
        }

        // Frequency
        SafeStoiRes frequency_stoi = Utils::safe_stoi(data.frequency);
        if (frequency_stoi.error == 0 && frequency_stoi.val > 0 && frequency_stoi.val <= 999) {
            strcpy(Keyboard::written_frequency, data.frequency);
            Network::frequency = frequency_stoi.val;
            App::sleep_ns = 1000000000LL / Network::frequency;
            Console::print_frequency();
        }

        fclose(file);
    }

    bool create_directory(const std::string& folder) {
        Result ret = fsInit();
        if (ret < 0)
            return false;

        FS_Archive sdmc_archive;
        ret = FSUSER_OpenArchive(&sdmc_archive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
        if (ret < 0) {
            fsExit();
            return false;
        }

        FS_Path dir_path = fsMakePath(PATH_ASCII, folder.c_str());
        FSUSER_CreateDirectory(sdmc_archive, dir_path, 0);
        FSUSER_CloseArchive(sdmc_archive);
        fsExit();

        return true;
    }
};