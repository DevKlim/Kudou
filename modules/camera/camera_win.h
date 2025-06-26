#pragma once

#include "servers/camera/camera_feed.h"
#include "servers/camera_server.h"

class CameraWindows : public CameraServer {
private:
	void add_active_cameras();

public:
	CameraWindows();
	~CameraWindows() {}
};
