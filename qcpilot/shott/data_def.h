#ifndef __OPENPILOT_QCPILOT_SHOTT_DATA_DEF_H__
#define __OPENPILOT_QCPILOT_SHOTT_DATA_DEF_H__
#include <cstdint>

namespace qcpilot {


struct ConsoleFrame {
    std::uint16_t engineRpm {0U};    // RPM
    float speedKph {0.0F};           // km/h
    float brake {0.0F};              // [0, 1]
    float gas {0.0F};                // [0, 1]
    float ax {0.0F};                 // longitudinal m/s
    float ay {0.0F};                 // lateral m/s

    template<class Archive>
    void serialize(Archive &ar) {
        ar(engineRpm, speedKph, brake, gas, ax, ay);
    }
};

}    // namespace qcpilot

#endif
