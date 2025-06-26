#pragma once

#include "core/input/input_enums.h"
#include "core/variant/callable.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

@interface GodotStatusItemDelegate : NSObject {
	Callable cb;
}

- (IBAction)click:(id)sender;

- (void)setCallback:(const Callable &)callback;

@end
