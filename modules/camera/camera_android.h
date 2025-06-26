#pragma once

#include "servers/camera/camera_feed.h"
#include "servers/camera_server.h"

#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraMetadataTags.h>
#include <media/NdkImageReader.h>

class CameraFeedAndroid : public CameraFeed {
	GDSOFTCLASS(CameraFeedAndroid, CameraFeed);

private:
	String camera_id;
	int32_t orientation;
	Ref<Image> image_y;
	Ref<Image> image_uv;
	Vector<uint8_t> data_y;
	Vector<uint8_t> data_uv;

	ACameraManager *manager = nullptr;
	ACameraMetadata *metadata = nullptr;
	ACameraDevice *device = nullptr;
	AImageReader *reader = nullptr;
	ACameraCaptureSession *session = nullptr;
	ACaptureRequest *request = nullptr;

	void _add_formats();
	void _set_rotation();

	static void onError(void *context, ACameraDevice *p_device, int error);
	static void onDisconnected(void *context, ACameraDevice *p_device);
	static void onImage(void *context, AImageReader *p_reader);
	static void onSessionReady(void *context, ACameraCaptureSession *session);
	static void onSessionActive(void *context, ACameraCaptureSession *session);
	static void onSessionClosed(void *context, ACameraCaptureSession *session);

protected:
public:
	bool activate_feed() override;
	void deactivate_feed() override;
	bool set_format(int p_index, const Dictionary &p_parameters) override;
	Array get_formats() const override;
	FeedFormat get_format() const override;

	CameraFeedAndroid(ACameraManager *manager, ACameraMetadata *metadata, const char *id,
			CameraFeed::FeedPosition position, int32_t orientation);
	~CameraFeedAndroid() override;
};

class CameraAndroid : public CameraServer {
	GDSOFTCLASS(CameraAndroid, CameraServer);

private:
	ACameraManager *cameraManager = nullptr;

	void update_feeds();
	void remove_all_feeds();

public:
	void set_monitoring_feeds(bool p_monitoring_feeds) override;

	~CameraAndroid();
};
