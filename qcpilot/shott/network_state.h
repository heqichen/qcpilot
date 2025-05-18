#ifndef __OPENPILOT_QCPILOT_SHOTT_NETWORK_H__
#define __OPENPILOT_QCPILOT_SHOTT_NETWORK_H__

#include <dbus/dbus.h>

namespace cooboc {
namespace shott {

class NetworkState {
  public:
    NetworkState(bool disabled = false);
    void step();
    bool isDirty() const {
        return isDirty_;
    }
    void clear() {
        isDirty_ = false;
    };

  private:
    bool isDisabled_ {false};
    bool isDirty_ {false};
    DBusConnection* dbusSystemConn_ {nullptr};
};

}    // namespace shott
}    // namespace cooboc

#endif
