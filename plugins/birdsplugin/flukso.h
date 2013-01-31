#ifndef FLUKSO_H
#define FLUKSO_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QTimer>

#include <QSignalMapper>

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
    void result(QString sensor);
    void error(QString sensor);
#ifndef QT_NO_OPENSSL
    void sslHandler(const QList<QSslError> &errors);
#endif
    void getRemote();
    
signals:
    void valueChanged(QString, int);
    void errorOccured(QString);

private:
    void readSettings();
    QNetworkAccessManager *_nam;
    QSignalMapper *_resultMapper, *_errorMapper;
    QTimer *_timer;
    QMap<QString, Sensor> *_sensors;
    QString _address, _port, _interval;
    bool _local;
};

#endif // FLUKSO_H
