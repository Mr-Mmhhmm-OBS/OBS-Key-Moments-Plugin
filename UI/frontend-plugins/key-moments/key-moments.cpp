
#include "key-moments.h"

#include <obs-module.h>
#include <QMainWindow>
#include <QMenuBar>
#include <QVBoxLayout>
#include "ui_KeyMoments.h"

#include "../../item-widget-helpers.hpp"
#include "../../obs-app.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_AUTHOR("Andrew Carbert");
OBS_MODULE_USE_DEFAULT_LOCALE("key-moments", "en-US")

bool obs_module_load()
{
	const auto main_window =
		static_cast<QMainWindow *>(obs_frontend_get_main_window());
	obs_frontend_push_ui_translation(obs_module_get_string);
	auto *tmp = new KeyMoments(main_window);
	obs_frontend_add_dock(tmp);
	obs_frontend_pop_ui_translation();
	return true;
}

void obs_module_unload() {}

MODULE_EXPORT const char *obs_module_description(void)
{
	return obs_module_text("Description");
}

MODULE_EXPORT const char *obs_module_name(void)
{
	return obs_module_text("Key Moments");
}

void KeyMoments::OBSSignal(void *data, const char *signal,
			   calldata_t *call_data)
{
	obs_source_t *source =
		static_cast<obs_source_t *>(calldata_ptr(call_data, "source"));
	if (!source)
		return;
	uint32_t flags = obs_source_get_output_flags(source);
	if ((flags & OBS_SOURCE_CONTROLLABLE_MEDIA) == 0 ||
	    strcmp(signal, "source_destroy") != 0 &&
		    strcmp(signal, "source_remove") != 0)
		return;

	KeyMoments *controls = static_cast<KeyMoments *>(data);
	QMetaObject::invokeMethod(controls, "SignalMediaSource",
				  Qt::QueuedConnection);
}

void KeyMoments::OBSFrontendEvent(enum obs_frontend_event event, void *ptr)
{
	KeyMoments *controls = reinterpret_cast<KeyMoments *>(ptr);

	switch (event) {
	case OBS_FRONTEND_EVENT_SCENE_CHANGED:
	case OBS_FRONTEND_EVENT_PREVIEW_SCENE_CHANGED:
		controls->RefreshMediaControls();
		break;
	default:
		break;
	}
}

void KeyMoments::AddActiveSource(obs_source_t *parent, obs_source_t *child,
				 void *param)
{
	//if (obs_source_removed(child))
	//	return;
	//auto *controls = static_cast<KeyMoments *>(param);
	//const uint32_t flags = obs_source_get_output_flags(child);
	//if ((flags & OBS_SOURCE_CONTROLLABLE_MEDIA) == 0) {
	//	obs_source_enum_active_sources(child, AddActiveSource, param);
	//	return;
	//}

	//QString source_name = QT_UTF8(obs_source_get_name(child));
	//const auto count = controls->ui->verticalLayout->count();
	//for (int i = count - 1; i >= 0; i--) {
	//	QLayoutItem *item = controls->ui->verticalLayout->itemAt(i);
	//	if (!item)
	//		continue;
	//	auto *w = dynamic_cast<MediaControl *>(item->widget());
	//	if (w->objectName() == source_name ||
	//	    w->GetSource() == OBSGetWeakRef(child)) {
	//		return;
	//	}
	//}

	//for (int i = count - 1; i >= 0; i--) {
	//	QLayoutItem *item = controls->ui->verticalLayout->itemAt(i);
	//	if (item) {
	//		QWidget *w = item->widget();
	//		if (source_name.localeAwareCompare(w->objectName()) >=
	//		    0) {
	//			controls->addMediaControl(child, i + 1);
	//			return;
	//		}
	//	}
	//}
	//controls->addMediaControl(child, 0);
}

void KeyMoments::addMediaControl(obs_source_t *source, int column)
{
	//MediaControl *c = new MediaControl(OBSGetWeakRef(source),
	//				   showTimeDecimals, showTimeRemaining);
	//ui->verticalLayout->insertWidget(column, c);
}

bool KeyMoments::AddSource(void *param, obs_source_t *source)
{
	//if (obs_source_removed(source))
	//	return true;
	//KeyMoments *controls = static_cast<KeyMoments *>(param);
	//uint32_t flags = obs_source_get_output_flags(source);
	//if ((flags & OBS_SOURCE_CONTROLLABLE_MEDIA) == 0)
	//	return true;

	//QString source_name = QT_UTF8(obs_source_get_name(source));
	//const auto count = controls->ui->verticalLayout->count();
	//for (int i = count - 1; i >= 0; i--) {
	//	QLayoutItem *item = controls->ui->verticalLayout->itemAt(i);
	//	if (!item)
	//		continue;
	//	auto *w = dynamic_cast<MediaControl *>(item->widget());
	//	if (w->objectName() == source_name ||
	//	    w->GetSource() == OBSGetWeakRef(source)) {
	//		return true;
	//	}
	//}
	//for (int i = count - 1; i >= 0; i--) {
	//	QLayoutItem *item = controls->ui->verticalLayout->itemAt(i);
	//	if (item) {
	//		QWidget *w = item->widget();
	//		if (source_name.localeAwareCompare(w->objectName()) >=
	//		    0) {
	//			controls->addMediaControl(source, i + 1);
	//			return true;
	//		}
	//	}
	//}
	//controls->addMediaControl(source, 0);
	return true;
}

KeyMoments::KeyMoments(QWidget *parent)
	: QDockWidget(parent), ui(new Ui::KeyMoments)
{
	ui->setupUi(this);

	char *file = obs_module_config_path("config.json");
	obs_data_t *data = nullptr;
	if (file) {
		data = obs_data_create_from_json_file(file);
		bfree(file);
	}
	if (data) {
		showTimeDecimals = obs_data_get_bool(data, "showTimeDecimals");
		showTimeRemaining =
			obs_data_get_bool(data, "showTimeRemaining");
		allSources = obs_data_get_bool(data, "showAllSources");
		obs_data_release(data);
	}

	connect(ui->dockWidgetContents, &QWidget::customContextMenuRequested,
		this, &KeyMoments::ControlContextMenu);

	connect(ui->editAutoScenes, &QPushButton::clicked, this, &KeyMoments::EditAutoScenes);

	signal_handler_connect_global(obs_get_signal_handler(), OBSSignal,
				      this);
	obs_frontend_add_event_callback(OBSFrontendEvent, this);

	hide();
}

KeyMoments::~KeyMoments()
{
	signal_handler_disconnect_global(obs_get_signal_handler(), OBSSignal,
					 this);
	//char *file = obs_module_config_path("config.json");
	//if (!file) {
	//	deleteLater();
	//	return;
	//}
	//obs_data_t *data = obs_data_create_from_json_file(file);
	//if (!data)
	//	data = obs_data_create();
	//obs_data_set_bool(data, "showTimeDecimals", showTimeDecimals);
	//obs_data_set_bool(data, "showTimeRemaining", showTimeRemaining);
	//obs_data_set_bool(data, "showAllSources", allSources);
	//if (!obs_data_save_json(data, file)) {
	//	char *path = obs_module_config_path("");
	//	if (path) {
	//		os_mkdirs(path);
	//		bfree(path);
	//	}
	//	obs_data_save_json(data, file);
	//}
	//obs_data_release(data);
	//bfree(file);
	//deleteLater();
}

void KeyMoments::SignalMediaSource()
{
	RefreshMediaControls();
}

void KeyMoments::ControlContextMenu()
{
	QAction showTimeRemainingAction(obs_module_text("ShowTimeRemaining"),
					this);
	showTimeRemainingAction.setCheckable(true);
	showTimeRemainingAction.setChecked(showTimeRemaining);

	QAction allSourcesAction(obs_module_text("AllSources"), this);
	allSourcesAction.setCheckable(true);
	allSourcesAction.setChecked(allSources);

	connect(&showTimeRemainingAction, &QAction::toggled, this,
		&KeyMoments::ToggleShowTimeRemaining, Qt::DirectConnection);

	connect(&allSourcesAction, &QAction::toggled, this,
		&KeyMoments::ToggleAllSources, Qt::DirectConnection);

	QMenu popup;
	popup.addAction(&showTimeRemainingAction);
	popup.addSeparator();
	popup.addAction(&allSourcesAction);
	popup.exec(QCursor::pos());
}

void KeyMoments::EditAutoScenes()
{
	AutoScenes autoScenes = new AutoScenes(this);
	autoScenes.exec();
}

void KeyMoments::ToggleShowTimeRemaining()
{
	showTimeRemaining = !showTimeRemaining;
	RefreshMediaControls();
}

void KeyMoments::ToggleAllSources()
{
	allSources = !allSources;
	RefreshMediaControls();
}

void KeyMoments::RefreshMediaControls()
{
	/*const auto count = ui->verticalLayout->count();
	for (int i = count - 1; i >= 0; i--) {
		QLayoutItem *item = ui->verticalLayout->itemAt(i);
		if (!item)
			continue;
		QWidget *w = item->widget();
		obs_source_t *source =
			obs_get_source_by_name(QT_TO_UTF8(w->objectName()));
		if (!source) {
			ui->verticalLayout->removeItem(item);
			delete w;
		} else {
			if (obs_source_removed(source) ||
			    (!allSources && !obs_source_active(source))) {
				ui->verticalLayout->removeItem(item);
				delete w;
			}
			obs_source_release(source);
		}
	}

	if (allSources) {
		obs_enum_sources(AddSource, this);
	} else {
		obs_source_t *scene = obs_frontend_get_current_scene();
		if (scene) {
			obs_source_enum_active_sources(scene, AddActiveSource,
						       this);
			obs_source_release(scene);
		}
		scene = obs_frontend_get_current_preview_scene();
		if (scene) {
			obs_source_enum_active_sources(scene, AddActiveSource,
						       this);
			obs_source_release(scene);
		}
	}*/
}
