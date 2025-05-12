#ifndef __OPENPILOT_QCPILOT_SHOTT_H__
#define __OPENPILOT_QCPILOT_SHOTT_H__

#include <QObject>

class Shott : public QObject {
    Q_OBJECT
  public:
    Shott(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~Shott() {}
    void step();
};

#endif
