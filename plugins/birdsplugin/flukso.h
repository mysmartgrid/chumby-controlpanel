#ifndef FLUKSO_H
#define FLUKSO_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QTimer>

class Flukso : public QObject
{
	Q_OBJECT

    struct Sensor {
        QString id;
        QString token;
        bool enabled;
    };

public:
	explicit Flukso(QObject *parent = 0);
	~Flukso();

public slots:
    void result(QNetworkReply* reply);
    void sslHandler(QNetworkReply * reply, const QList<QSslError> & errors);
    void getRemote();
    
signals:
    void valueChanged(int);

private:
    void readSettings();
    QNetworkAccessManager *_nam;
    QTimer *_timer;
    QMap<QString, Sensor> *_sensors;
};

#endif // FLUKSO_H
