#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    virtual ~NetworkManager();

    void getHCasterIpAddress();
    void getListObjects();

signals:
    void listOfObjectsAvailable();
};

#endif // NETWORKMANAGER_H
