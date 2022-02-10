#include "auto-scene.h"

#include <obs.h>
#include <obs-frontend-api.h>
#include <QHBoxLayout>
#include <QPushButton>
#include "auto-scenes.h"

using std::string;
using std::list;

AutoScene::AutoScene(QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(2);

	keyMoment_LineEdit = new QLineEdit(this);
	keyMoment_LineEdit->setPlaceholderText("Key-Moment Name");
	mainLayout->addWidget(keyMoment_LineEdit);
	connect(keyMoment_LineEdit, &QLineEdit::textChanged, this,
		&AutoScene::KeyMoment_TextChanged);

	sceneName_ComboBox = new QComboBox(this);
	connect(sceneName_ComboBox, &QComboBox::currentTextChanged, this,
		&AutoScene::SceneName_TextChanged);
	char** scenename_ptr = obs_frontend_get_scene_names();
	while (scenename_ptr && *scenename_ptr) {
		sceneName_ComboBox->addItem(*scenename_ptr);
		scenename_ptr++;
	}
	mainLayout->addWidget(sceneName_ComboBox);

	QPushButton *removeButton = new QPushButton();
	removeButton->setText("-");
	connect(removeButton, &QPushButton::clicked, this,
		&AutoScene::deleteLater);
	mainLayout->addWidget(removeButton);

	setLayout(mainLayout);
}

AutoScene::AutoScene(QString keyMoment,
		     QString sceneName, QWidget *parent)
	: AutoScene(parent)
{
	keyMoment_LineEdit->setText(keyMoment);
	sceneName_ComboBox->setCurrentText(sceneName);
}

void AutoScene::KeyMoment_TextChanged(const QString text)
{
	_keyMoment = text.toStdString();
}

const char* AutoScene::GetKeyMoment() {
	return _keyMoment.data();
}

void AutoScene::SceneName_TextChanged(const QString text) {
	_sceneName = text.toStdString();
}

const char* AutoScene::GetSceneName() {
	return _sceneName.data();
}

void AutoScene::Remove() {
	//auto_scenes.remove();
}
