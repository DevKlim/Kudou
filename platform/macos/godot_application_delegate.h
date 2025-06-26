#pragma once

#include "core/os/os.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

class OS_MacOS_NSApp;

@interface GodotApplicationDelegate : NSObject <NSUserInterfaceItemSearching, NSApplicationDelegate>

- (GodotApplicationDelegate *)initWithOS:(OS_MacOS_NSApp *)os;

- (bool)getHighContrast;
- (bool)getReduceMotion;
- (bool)getReduceTransparency;
- (bool)getVoiceOver;
@end
