#ifndef REQUESTS_H
#define REQUESTS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

class IRequest : public QObject
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
    IRequest(QObject * parent = nullptr);
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

class GetHomeCasterIpAddressRequest : public IRequest
{
public:
    explicit GetHomeCasterIpAddressRequest(QObject * parent = nullptr);
    virtual ~GetHomeCasterIpAddressRequest() = default;
    void execute() override;
    void onFinished(QNetworkReply *reply) override;

private:
    void handleResponse(QByteArray response);
};

class GetListObjectsRequest : public IRequest
{
public:
    explicit GetListObjectsRequest(QObject * parent = nullptr);
    virtual ~GetListObjectsRequest() = default;
    void execute() override;
    void onFinished(QNetworkReply *reply) override;

private:
    void handleResponse(QByteArray response);
};

class GetUpgradeJsonRequest : public IRequest
{
public:
    explicit GetUpgradeJsonRequest(QObject * parent = nullptr);
    virtual ~GetUpgradeJsonRequest() = default;
    void execute() override;
    void onFinished(QNetworkReply *reply) override;

private:
    void handleResponse(QByteArray response);
};

class GetBinaryImageRequest : public IRequest
{
public:
    explicit GetBinaryImageRequest(QObject * parent = nullptr);
    virtual ~GetBinaryImageRequest() = default;
    void execute() override;
    void onFinished(QNetworkReply *reply) override;

private:
    void handleResponse(QByteArray response);
};

#endif // REQUESTS_H
