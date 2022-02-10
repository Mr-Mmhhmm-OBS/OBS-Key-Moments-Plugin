
#include "auto-scenes.h"
#include "../../obs-app.hpp"

AutoScenes::AutoScenes(QWidget *parent)
	: QDialog(parent), ui(new Ui::AutoScenes)
{
	ui->setupUi(this);

	connect(ui->newAutoScene, &QPushButton::clicked, this,
		&AutoScenes::NewAutoScene);

	connect(this, &QDialog::rejected, this, &AutoScenes::Save);

	char *file = obs_module_config_path("config.json");
	obs_data_t *data = nullptr;
	if (file) {
		data = obs_data_create_from_json_file(file);
		bfree(file);
	}
	if (data) {
		json_error_t error;
		json_t *json_root = json_loads(
			obs_data_get_string(data, "auto-scenes"), 0, &error);
		obs_data_release(data);
		if (json_root) {
			const char *key;
			json_t *value;
			json_object_foreach (json_root, key, value) {
				AutoScene *auto_scene = new AutoScene(
						QString::fromStdString(key),
						QString::fromStdString(
							json_string_value(
								value)),
						this);
				ui->autoScenes->addWidget(auto_scene);
			}
		}
	}
}

void AutoScenes::NewAutoScene()
{
	AutoScene *auto_scene = new AutoScene(this);
	ui->autoScenes->addWidget(auto_scene);
}

void AutoScenes::Save()
{
	char *file = obs_module_config_path("config.json");
	if (file) {
		obs_data_t *data = obs_data_create_from_json_file(file);
		if (!data)
			data = obs_data_create();

		json_t *json_root = json_object();
		for (auto child : findChildren<AutoScene *>())
		{
			const char *key = child->GetKeyMoment();
			const char *scene = child->GetSceneName();
			if (strlen(key) && strlen(scene)) {
				json_object_set_new(json_root, key,
						json_string(scene));
			}
		}
		obs_data_set_string(data, "auto-scenes",
				    json_dumps(json_root, 0));
		if (!obs_data_save_json(data, file)) {
			char *path = obs_module_config_path("");
			if (path) {
				os_mkdirs(path);
				bfree(path);
			}
			obs_data_save_json(data, file);
		}
		obs_data_release(data);
		bfree(file);
	}
}
