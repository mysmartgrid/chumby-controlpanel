#ifndef SOURCEPAGE_H
#define SOURCEPAGE_H

#include <QWizardPage>
#include <QListWidgetItem>

#include "../../controlpanel/audioplugin.h"

//TODO: reselection must be handled

namespace Ui {
class SourcePage;
}

class SourcePage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit SourcePage(QWidget *parent = 0, QStringList source = QStringList());
    ~SourcePage();
	
protected slots:
	void selectSource(QListWidgetItem *item);
    
private:
		Msg::AudioPlugin* getPlugin();
		QString getPath() const;
    Ui::SourcePage *_ui;
		QStringList _source;
		Msg::AudioPlugin* _plugin;
};

#endif // SOURCEPAGE_H
