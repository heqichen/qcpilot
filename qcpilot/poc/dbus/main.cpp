#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  qDebug() << "Hello, Qt console application!";

  return QCoreApplication::exec();
}