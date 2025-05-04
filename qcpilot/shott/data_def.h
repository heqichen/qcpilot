#ifndef __QCPILOT_SHOTT_DATA_DEF_H__
#define __QCPILOT_SHOTT_DATA_DEF_H__

namespace qcpilot {
namespace shott {

struct ConsoleFrame {
    float engineRpm {0.0F};    // RPM
    float speedKph {0.0F};     // km/h
    float brake {0.0F};        // [0, 1]
    float gas {0.0F};          // [0, 1]
    float ax {0.0F};           // longitudinal m/s
    float ay {0.0F};           // lateral m/s


    template<class Archive>
    void serialize(Archive &ar) {
        ar(engineRpm, speedKph, brake, gas, ax, ay);
    }
};
}    // namespace shott
}    // namespace qcpilot

#endif
