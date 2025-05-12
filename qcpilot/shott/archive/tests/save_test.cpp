#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

struct ConsoleFrame {
    float engineRpm {0.0F};
    float speedKph {0.0F};
    template<class Archive>
    void serialize(Archive &ar) {
        ar(engineRpm, speedKph);
    }
};


int main(int argc, char *argv[]) {
    ConsoleFrame frame;
    frame.engineRpm = 1234.0F;
    frame.speedKph = 56.7F;
    frame.engineRpm = 0.0F;
    // frame.speedKph = 0.0F;

    std::ostringstream oss(std::ios::binary);
    // oss << ;
    std::cout << "size: " << oss.str().size() << std::endl;

    {
        cereal::PortableBinaryOutputArchive archive(oss);
        archive(frame);
    }

    std::ofstream ofs("test.bin", std::ios::out | std::ios::binary);
    std::cout << "is opened? " << ofs.is_open() << std::endl;
    std::cout << "size: " << oss.str().size() << std::endl;
    ofs.write(oss.str().data(), oss.str().size());
    ofs.flush();
    ofs.close();

    return 0;
}