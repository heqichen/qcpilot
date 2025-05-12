#include <cereal/archives/portable_binary.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

struct ConsoleFrame {
    float engineRpm {0.0F};
    float speedKph {0.0F};
    template<class Archive>
    void serialize(Archive &ar) {
        ar(engineRpm, speedKph);
    }
};

// constexpr std::uint8_t stream[] {0x01, 0x00, 0x40, 0x9a, 0x44, 0xcd, 0xcc, 0x62, 0x42};
// constexpr std::uint8_t stream[] {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
constexpr std::uint8_t stream[] {0x01, 0x00, 0x00, 0x00, 0x00, 0xcd, 0xcc, 0x62, 0x42};


int main(void) {
    ConsoleFrame frame;
    {
        std::istringstream iss(std::string((char *)stream, 9), std::ios::binary);

        // std::ifstream ifs("test.bin", std::ios::binary | std::ios::in);
        cereal::PortableBinaryInputArchive ar(iss);
        ar(frame);
    }

    std::cout << frame.engineRpm << " " << frame.speedKph << std::endl;


    return 0;
}