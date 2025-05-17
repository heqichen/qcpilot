#ifndef __OPENPILOT_QCPILOT_SHOTT_NETWORK_H__
#define __OPENPILOT_QCPILOT_SHOTT_NETWORK_H__

#include <dbus/dbus.h>

namespace cooboc {
namespace shott {

class NetworkState {
  public:
    NetworkState();
    void step();
    bool isDirty() const {
        return isDirty_;
    }
    void clear() {
        isDirty_ = false;
    };

  private:
    DBusConnection* dbusSystemConn_ {nullptr};
    bool isDirty_ {false};
};

}    // namespace shott
}    // namespace cooboc

#endif
