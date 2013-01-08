#include "flukso.h"

#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QSettings>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslError>

#include <QStringList>

#include <qjson/parser.h>

Flukso::Flukso(QObject *parent)
	: QObject(parent)
	, _nam(new QNetworkAccessManager)
	, _timer(new QTimer)
    , _sensors(new QMap<QString, Sensor>)
{
    readSettings();

    _timer->setInterval(1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(getRemote()));
    _timer->setSingleShot(true);
    _timer->start();

    connect(_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(result(QNetworkReply*)));
    connect(_nam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslHandler(QNetworkReply*,QList<QSslError>)));
}

Flukso::~Flukso()
{
}

void Flukso::getRemote()
{
    for(QMap<QString, Sensor>::Iterator it = _sensors->begin(); it != _sensors->end(); it++)
    {
        Sensor s = it.value();
        if ( s.enabled )
        {
            QNetworkRequest req(QUrl(QString("https://api.mysmartgrid.de:8443/sensor/%1?interval=hour&unit=watt").arg(s.id)));
            req.setRawHeader("X-Token", s.token.toAscii());
            req.setRawHeader("X-Version", "1.0");
            _nam->get(req);
        }
    }
}

void Flukso::result(QNetworkReply* reply)
{
    if ( !reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).isValid() )
    {
        reply->deleteLater();
        return;
    }

    qDebug() << "result:" << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString() << "(" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << ")";
    switch ( reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() )
    {
    case 200:
    {
        QJson::Parser parser;
        QVariantList values = parser.parse(reply->readAll()).toList();
        qDebug() << "Success:" << values;
        int lvalue = -1;
        foreach( QVariant pair, values)
        {
            QVariantList l = pair.toList();
            qDebug() << l.first().toULongLong() << l.at(1).toULongLong();
            if ( l.at(1).toULongLong() > 0 )
                lvalue = l.at(1).toInt();
        }
        if ( lvalue >= 0 )
            emit valueChanged(lvalue);
    }
        break;
    case 302:
        _nam->get(QNetworkRequest(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl()));
        break;
    }
    reply->deleteLater();
    _timer->start();
}

void Flukso::sslHandler(QNetworkReply *reply, const QList<QSslError> &errors)
{
    foreach( QSslError error, errors)
    {
        qDebug() << error;
    }
    reply->ignoreSslErrors();
}

void Flukso::readSettings()
{
    QSettings settings("/mnt/usb/flukso.conf", QSettings::NativeFormat);
    settings.setIniCodec("UTF-8");

    // Reading general setting

    // Reading sensors
    settings.beginGroup("sensors");
    foreach(QString sensor, settings.childGroups())
    {
        settings.beginGroup(sensor);
        Sensor s;
        s.id = settings.value("id").toString();
        s.token = settings.value("token").toString();
        s.enabled = settings.value("enabled").toBool();
        _sensors->insert(sensor, s);
        settings.endGroup();
    }
}