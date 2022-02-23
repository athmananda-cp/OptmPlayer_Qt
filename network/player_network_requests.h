#ifndef PLAYER_NETWORK_REQUESTS_H
#define PLAYER_NETWORK_REQUESTS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

class IPlayerNetworkRequest : public QObject
{
    Q_OBJECT

public:
    enum class Status
    {
        TimedOut,
        Success,
        Failed
    };

public:
    IPlayerNetworkRequest(QObject * parent = nullptr);
    virtual void execute() = 0;

protected slots:
    virtual void onFinished(QNetworkReply *reply) = 0;

signals:
    void requestCompleted(Status);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

protected:
    QNetworkAccessManager *m_nwAccessManager;
    QNetworkRequest        m_nwRequest;
};

class PlayerHcGetParams : public IPlayerNetworkRequest
{
public:
    explicit PlayerHcGetParams(QObject * parent = nullptr);
    virtual ~PlayerHcGetParams() = default;
    void execute() override;
    void onFinished(QNetworkReply *reply) override;

private:
    void handleResponse(QByteArray response);
};

class PlayerHcGetObjectList : public IPlayerNetworkRequest
{
public:
    explicit PlayerHcGetObjectList(QObject * parent = nullptr);
    virtual ~PlayerHcGetObjectList() = default;
    void execute() override;
    void onFinished(QNetworkReply *reply) override;

private:
    void handleResponse(QByteArray response);
};

class PlayerHcGetServiceList : public IPlayerNetworkRequest
{
public:
    explicit PlayerHcGetServiceList(QObject * parent = nullptr);
    virtual ~PlayerHcGetServiceList() = default;
    void execute() override;
    void onFinished(QNetworkReply *reply) override;

private:
    void handleResponse(QByteArray response);
};

class PlayerHcGetUpgradeJson : public IPlayerNetworkRequest
{
public:
    explicit PlayerHcGetUpgradeJson(QObject * parent = nullptr);
    virtual ~PlayerHcGetUpgradeJson() = default;
    void execute() override;
    void onFinished(QNetworkReply *reply) override;

private:
    void handleResponse(QByteArray response);
};

class PlayerHcGetUpgradeImage : public IPlayerNetworkRequest
{
public:
    explicit PlayerHcGetUpgradeImage(QObject * parent = nullptr);
    virtual ~PlayerHcGetUpgradeImage() = default;
    void execute() override;
    void onFinished(QNetworkReply *reply) override;

private:
    void handleResponse(QByteArray response);
};

#endif // PLAYER_NETWORK_REQUESTS_H
