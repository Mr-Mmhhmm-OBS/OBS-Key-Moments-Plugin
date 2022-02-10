#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

using std::list;
using std::string;
using std::unique_ptr;

class AutoScene : public QWidget {
	Q_OBJECT
private:
	//list<std::shared_ptr<AutoScene>>& auto_scenes;
	string _keyMoment = "";
	string _sceneName = "";

	QLineEdit *keyMoment_LineEdit;
	QComboBox *sceneName_ComboBox;

	void KeyMoment_TextChanged(QString);
	void SceneName_TextChanged(QString);

	void Remove();
public:
	AutoScene(/*list<std::shared_ptr<AutoScene>> &_auto_scenes, */QWidget *parent = nullptr);
	AutoScene(/*list<std::shared_ptr<AutoScene>> &_auto_scenes, */QString keyMoment,
		  QString sceneName, QWidget *parent = nullptr);

	const char *GetKeyMoment();
	const char *GetSceneName();
};
