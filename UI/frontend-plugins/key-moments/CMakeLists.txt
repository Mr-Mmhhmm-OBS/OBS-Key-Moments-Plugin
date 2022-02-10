project(key-moments)
set(PROJECT_FULL_NAME "Key Moments")

set(key-moments_HEADERS
	key-moments.h
	auto-scenes.h
	auto-scene.h)

set(key-moments_SOURCES
	key-moments.cpp
	auto-scenes.cpp
	auto-scene.cpp)

qt5_wrap_ui(key-moments_UI_HEADERS 
	KeyMoments.ui
	AutoScenes.ui)

add_library(key-moments MODULE
	${key-moments_HEADERS}
	${key-moments_SOURCES}
	${key-moments_UI_HEADERS})

target_link_libraries(key-moments
	obs-frontend-api
	libobs
	jansson
	Qt5::Widgets)

set_target_properties(key-moments PROPERTIES FOLDER "plugins")

install_obs_plugin_with_data(key-moments data)
