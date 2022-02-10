#pragma once

#include <obs-module.h>

#include <QDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QVBoxLayout>

#include <obs.hpp>
#include "../UI/qt-wrappers.hpp"
#include <../UI/obs-frontend-api/obs-frontend-api.h>
#include "../../item-widget-helpers.hpp"

#include "auto-scene.h"
#include "ui_AutoScenes.h"

#include "../../obs-app.hpp"
#include "jansson.h"

class AutoScenes : public QDialog, public Ui::AutoScenes {
	Q_OBJECT
private:
	std::unique_ptr<Ui::AutoScenes> ui;
	void Save();
private slots:
	void NewAutoScene();
public:
	AutoScenes(QWidget *parent = nullptr);
};
