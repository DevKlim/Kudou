#pragma once

#include "servers/display_server.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

@interface GodotWindow : NSWindow {
	DisplayServer::WindowID window_id;
	NSTimeInterval anim_duration;
}

- (void)setWindowID:(DisplayServer::WindowID)wid;
- (void)setAnimDuration:(NSTimeInterval)duration;

@end
