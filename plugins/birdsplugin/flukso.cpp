#include "flukso.h"

#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QSettings>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#ifndef QT_NO_OPENSSL
#include <QtNetwork/QSslError>
#endif

#include <QStringList>

#include <qjson/parser.h>

//#define FLUKSO_DEBUG

Flukso::Flukso(QObject *parent)
	: QObject(parent)
	, _nam(new QNetworkAccessManager)
    , _resultMapper(new QSignalMapper)
    , _errorMapper(new QSignalMapper)
	, _timer(new QTimer)
    , _sensors(new QMap<QString, Sensor>)
    , _address("api.mysmartgrid.de")
    , _port("8443")
    , _interval("hour")
    , _local(false)
{
    readSettings();

    _timer->setInterval(1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(getRemote()));
    _timer->setSingleShot(true);
    _timer->start();

    connect(_resultMapper, SIGNAL(mapped(QString)), this, SLOT(result(QString)));
    connect(_errorMapper, SIGNAL(mapped(QString)), this, SLOT(error(QString)));
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
            //TODO: support for local flukso api
            QString protocol;
            QString version;
#ifndef QT_NO_OPENSSL
            if ( _local ) {
                protocol = "http";
                version = "&version=1.0";
            } else {
                protocol = "https";
                version = "";
            }
#else
            protocol = "http";
#endif

            QNetworkRequest req(QUrl(protocol+"://"+_address+":"+_port+"/sensor/"+s.id+"?interval="+_interval+"&unit=watt"+version));
            if ( !_local )
                req.setRawHeader("X-Token", s.token.toAscii());
            req.setRawHeader("X-Version", "1.0");
            QNetworkReply *r = _nam->get(req);
            _resultMapper->setMapping(r, it.key());
            _errorMapper->setMapping(r, it.key());
            connect(r, SIGNAL(finished()), _resultMapper, SLOT(map()));
            connect(r, SIGNAL(error(QNetworkReply::NetworkError)), _errorMapper, SLOT(map()));
#ifndef QT_NO_OPENSSL
            connect(r, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslHandler(QList<QSslError>)));
#endif
        }
    }
}

void Flukso::result(QString sensor)
{
    QNetworkReply *reply = (QNetworkReply*) _resultMapper->mapping(sensor);
    if ( !reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).isValid() )
    {
        reply->deleteLater();
        return;
    }

#ifdef FLUKSO_DEBUG
    qDebug() << "result (" << sensor << "):" << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString() << "(" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << ")";
#endif
    switch ( reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() )
    {
    case 200:
    {
        QJson::Parser parser;
        QVariantList values = parser.parse(reply->readAll()).toList();
#ifdef FLUKSO_DEBUG
        qDebug() << "Success:" << values;
#endif
        int lvalue = -1;
        foreach( QVariant pair, values)
        {
            QVariantList l = pair.toList();
#ifdef FLUKSO_DEBUG
            qDebug() << l.first().toULongLong() << l.at(1).toULongLong();
#endif
            if ( l.at(1).toULongLong() > 0 )
                lvalue = l.at(1).toInt();
        }
        //lvalue = qrand()%3000; //activate for random debug values
        if ( lvalue >= 0 )
            emit valueChanged(sensor, lvalue);
        else
            qDebug() << "no valid data found. lvalue:" << lvalue;
    }
        break;
    case 302:
        //TODO: construct new request from old one and change the url
        _nam->get(QNetworkRequest(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl()));
        break;
    default:
    {
        emit errorOccured("Unknown return code: " + reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString() + "(" + reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() + ")");
    }
    }
    reply->deleteLater();
    _timer->start(1000);
}

void Flukso::error(QString sensor)
{
#ifdef FLUKSO_DEBUG
    qDebug() << "An error occured while fetching" << sensor << ":" << ((QNetworkReply*) _errorMapper->mapping(sensor))->errorString();
#endif FLUKSO_DEBUG
    emit errorOccured("Error while fetching data: " + ((QNetworkReply*) _errorMapper->mapping(sensor))->errorString());

    _timer->start(5000);
}

#ifndef QT_NO_OPENSSL
void Flukso::sslHandler(const QList<QSslError> &errors)
{
    QNetworkReply *reply = (QNetworkReply*) sender();
    foreach( QSslError error, errors)
    {
        qDebug() << error;
    }
    reply->ignoreSslErrors();
}
#endif

QString Flukso::displaySensor()
{
    return _display;
}

void Flukso::readSettings()
{
    QString configdir = getenv("CHUMBY_CONFIG_DIR");
    if ( configdir.isEmpty() )
        configdir = "/mnt/usb";
    QSettings settings(configdir + "/flukso.conf", QSettings::NativeFormat);
    settings.setIniCodec("UTF-8");

    // Reading general setting
#ifndef QT_NO_OPENSSL
    if ( !settings.contains("local") || settings.value("local").toBool() )
#endif
    {
        _local = true;
        _address = "192.168.1.1";
        _port = "8080";
        _interval = "minute";
    }

    if ( settings.contains("ip") )
        _address = settings.value("ip").toString();

    if ( settings.contains("port") )
        _port = settings.value("port").toString();

    if ( settings.contains("interval") )
        _interval = settings.value("interval").toString();

    if ( settings.contains("display") )
        _display = settings.value("display").toString();

#ifdef FLUKSO_DEBUG
    qDebug() << "General:" << _local << "," << _address << "," << _port << "," << _interval;
#endif FLUKSO_DEBUG

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
    settings.endGroup();

    if ( _sensors->empty() ) //legacy parser for old config style
    {
        if ( settings.contains("sensor1") )
        {
            Sensor s;
            s.id = settings.value("sensor1").toString();
            s.enabled = settings.value("sen1ena").toBool();
            _sensors->insert("1", s);
        }

        if ( settings.contains("sensor2") )
        {
            Sensor s;
            s.id = settings.value("sensor2").toString();
            s.enabled = settings.value("sen2ena").toBool();
            _sensors->insert("2", s);
        }

        if ( settings.contains("sensor3") )
        {
            Sensor s;
            s.id = settings.value("sensor3").toString();
            s.enabled = settings.value("sen3ena").toBool();
            _sensors->insert("3", s);
        }
    }

    if ( _display.isEmpty() )
        _display = _sensors->keys().first();

#ifdef FLUKSO_DEBUG
    qDebug() << "Displayed sensor:" << _display;
    foreach(QString key, _sensors->keys())
    {
        Sensor s = _sensors->value(key);
        qDebug() << "Sensor configured:" << key << "(" << s.id << "," << s.token << "," << s.enabled << ")";
    }
#endif
}
