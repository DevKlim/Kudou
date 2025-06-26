#pragma once

#include "scene/resources/texture.h"

class CameraTexture : public Texture2D {
	GDCLASS(CameraTexture, Texture2D);

private:
	mutable RID _texture;
	int camera_feed_id = 0;
	CameraServer::FeedImage which_feed = CameraServer::FEED_RGBA_IMAGE;

protected:
	static void _bind_methods();
	void _on_format_changed();

public:
	virtual int get_width() const override;
	virtual int get_height() const override;
	virtual RID get_rid() const override;
	virtual bool has_alpha() const override;

	virtual Ref<Image> get_image() const override;

	void set_camera_feed_id(int p_new_id);
	int get_camera_feed_id() const;

	void set_which_feed(CameraServer::FeedImage p_which);
	CameraServer::FeedImage get_which_feed() const;

	void set_camera_active(bool p_active);
	bool get_camera_active() const;

	CameraTexture();
	~CameraTexture();
};
