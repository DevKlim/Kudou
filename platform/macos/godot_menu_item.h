#pragma once

#include "servers/display_server.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

#define MENU_TAG_START 0x00004447
#define MENU_TAG_END 0xFFFF4447

enum GlobalMenuCheckType {
	CHECKABLE_TYPE_NONE,
	CHECKABLE_TYPE_CHECK_BOX,
	CHECKABLE_TYPE_RADIO_BUTTON,
};

@interface GodotMenuItem : NSObject {
@public
	Callable callback;
	Callable key_callback;
	Callable hover_callback;
	Variant meta;
	Key accel;
	GlobalMenuCheckType checkable_type;
	bool checked;
	int max_states;
	int state;
	Ref<Image> img;
}

@end
