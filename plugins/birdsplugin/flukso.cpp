#include "flukso.h"

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslError>
#include <QUrl>

#include <qjson/parser.h>

Flukso::Flukso(QObject *parent)
	: QObject(parent)
	, _nam(new QNetworkAccessManager)
	, _timer(new QTimer)
{
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
    QNetworkRequest req(QUrl("https://api.mysmartgrid.de:8443/sensor/4850219fefb8d4aa0c8b4e5da38916d8?interval=hour&unit=watt"));
    req.setRawHeader("X-Token", "359c91d87359cac7a887c25abcbeb8c3");
    req.setRawHeader("X-Version", "1.0");
    _nam->get(req);
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
