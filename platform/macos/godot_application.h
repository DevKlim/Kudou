#pragma once

#include "core/os/os.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <IOKit/hidsystem/ev_keymap.h>

@class GodotApplicationDelegate;

@interface GodotApplication : NSApplication

extern "C" GodotApplication *GodotApp;

@property(readonly, nonatomic) GodotApplicationDelegate *godotDelegate;

- (GodotApplication *)init;

- (void)activateApplication;
@end
