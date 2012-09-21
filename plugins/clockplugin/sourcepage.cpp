#include "sourcepage.h"
#include "ui_sourcepage.h"

#include "musiccontrol.h"

#include <QtCore/QDebug>

SourcePage::SourcePage(Msg::Alarm *alarm, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::SourcePage),
    _alarm(alarm),
    _plugin(NULL),
    _source(QStringList())
{
    _ui->setupUi(this);

    initSourceList();

    connect(_ui->sourceList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectSource(QListWidgetItem*)));
    connect(_ui->backButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
    connect(_ui->setButton, SIGNAL(clicked()), this, SLOT(setSource()));
}

SourcePage::~SourcePage()
{
    delete _ui;
}

void SourcePage::initSourceList()
{
    _source.append("");
    _ui->sourceList->clear();
    if ( _source.size() == 1 )
    {
        if ( !Msg::MusicControl::getInstance().getAudioPlugins().isEmpty() )
        {
            _ui->sourceList->addItems(Msg::MusicControl::getInstance().getAudioPlugins());
            //load source from wizard, if available
            if ( !_alarm->getSource().isEmpty() )
            {
                QString src = _alarm->getSource();
                int index = src.indexOf("/");
                src.remove(index, src.length());
                QList<QListWidgetItem*> items = _ui->sourceList->findItems(src, Qt::MatchExactly);
                if ( items.size() != 1 )
                    return;

                _ui->sourceList->clearSelection();
                //items.first()->setSelected(true);
                _ui->sourceList->setCurrentItem(items.first());
                selectSource(items.first());
            }
        }
    } else {
        Msg::AudioPlugin* plugin = getPlugin();
        if ( !plugin->getSourceList().isEmpty() )
        {
            _ui->sourceList->addItems(plugin->getSourceList());
            //load source from wizard, if available
            if ( !_alarm->getSource().isEmpty() )
            {
                QString src = _alarm->getSource();
                int index = src.indexOf("/");
                src.remove(0, index + 1);
                QList<QListWidgetItem*> items = _ui->sourceList->findItems(src, Qt::MatchExactly);
                if ( items.size() != 1 )
                    return;

                _ui->sourceList->clearSelection();
                items.first()->setSelected(true);
                selectSource(items.first());
            }
        }
    }

    if ( _ui->sourceList->selectedItems().size() == 0 && _ui->sourceList->itemAt(1,1) )
    {
        _ui->sourceList->setCurrentItem(_ui->sourceList->itemAt(1,1));
        selectSource(_ui->sourceList->selectedItems().first());
    }
}

void SourcePage::selectSource(QListWidgetItem *item)
{
    if ( item )
    {
        qDebug() << item->text();
        _source.last() = item->text();
        getPlugin();
    }
}

void SourcePage::setSource()
{
    if ( _plugin != NULL && _plugin->isFinal(getPath()))
    {
        _alarm->setSource(_source.join("/"));
        deleteLater();
    } else {
        initSourceList();
    }
}

Msg::AudioPlugin* SourcePage::getPlugin()
{
    if ( _plugin == NULL || _plugin->getName().compare(_source.first().toStdString()) != 0 )
        _plugin = Msg::MusicControl::getInstance().getAudioPlugin(_source.first());
    return _plugin;
}

QString SourcePage::getPath() const
{
    if ( _source.size() > 1 )
    {
        QStringList sources = _source;
        sources.removeFirst();
        return sources.join("/");
    }
    return "";
}
