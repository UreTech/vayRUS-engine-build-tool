//vayRUS-engine-build-tool version 1.0.0
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <filesystem>

std::string CONFIG_PATH = "/UreTechEngine/config.h";

std::string generateUUID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 15);

    std::ostringstream uuid;
    const char* hex_chars = "0123456789abcdef";

    for (int i = 0; i < 32; i++) {
        if (i == 8 || i == 12 || i == 16 || i == 20) {
            uuid << "-";
        }
        uuid << hex_chars[dist(gen)];
    }
    return uuid.str();
}

void updateConfigFile() {
    std::ifstream fileIn(CONFIG_PATH);
    if (!fileIn) {
        std::cerr << "ERROR: Can't open " << CONFIG_PATH << "\n";
        return;
    }

    std::ostringstream buffer;
    std::string line;
    std::string newUUID = generateUUID();

    while (std::getline(fileIn, line)) {
        if (line.find("#define engineBuildID") != std::string::npos) {
            buffer << "#define engineBuildID \"" << newUUID << "\"\n";
        }
        else {
            buffer << line << "\n";
        }
    }
    fileIn.close();

    std::ofstream fileOut(CONFIG_PATH);
    if (!fileOut) {
        std::cerr << "ERROR: Can't write " << CONFIG_PATH << "\n";
        return;
    }
    fileOut << buffer.str();
    fileOut.close();

    std::cout << "New vayRUS Engine BuildID: " << newUUID << "\n";
}

int main() {
    std::cout << "vayRUS-engine-build-tool v1.0.0\nBuild begins...";
    std::cout << "Creating new Engine BuildID...\n";
    std::filesystem::path currentPath = std::filesystem::current_path();
    CONFIG_PATH = currentPath.generic_string().append(CONFIG_PATH);
    updateConfigFile();
    std::cout << "vayRUS-engine-build-tool ended with no errors.\n";
    return 0;
}
