#pragma once

///@TODO this is a near duplicate of CameraIOS, we should find a way to combine those to minimize code duplication!!!!
// If you fix something here, make sure you fix it there as well!

#include "servers/camera_server.h"

class CameraMacOS : public CameraServer {
	GDSOFTCLASS(CameraMacOS, CameraServer);

public:
	CameraMacOS() = default;

	void update_feeds();
	void set_monitoring_feeds(bool p_monitoring_feeds) override;
};
