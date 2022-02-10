#pragma once

#include <QTimer>
#include <obs.hpp>
#include <QDockWidget>

#include "../UI/qt-wrappers.hpp"
#include <../UI/obs-frontend-api/obs-frontend-api.h>

#include "ui_KeyMoments.h"

#include "auto-scenes.h"

class KeyMoments : public QDockWidget {
	Q_OBJECT
private:
	static void OBSSignal(void *data, const char *signal,
			      calldata_t *calldata);
	static void AddActiveSource(obs_source_t *parent, obs_source_t *child,
				    void *param);
	static bool AddSource(void *param, obs_source_t *source);
	static void OBSFrontendEvent(enum obs_frontend_event event, void *ptr);
	void ControlContextMenu();
	void ToggleShowTimeRemaining();
	void ToggleAllSources();
	void RefreshMediaControls();

	void addMediaControl(obs_source_t *source, int column);

	bool showTimeDecimals = false;
	bool showTimeRemaining = false;
	bool allSources = false;
	std::unique_ptr<Ui::KeyMoments> ui;

private slots:
	void SignalMediaSource();
	void EditAutoScenes();

public:
	KeyMoments(QWidget *parent = nullptr);
	~KeyMoments();
};
