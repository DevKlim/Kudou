#pragma once

#include "servers/display_server.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

class DisplayServerMacOS;

@interface GodotWindowDelegate : NSObject <NSWindowDelegate>

- (void)setWindowID:(DisplayServer::WindowID)wid;

- (instancetype)initWithDisplayServer:(DisplayServerMacOS *)p_ds;

@end
