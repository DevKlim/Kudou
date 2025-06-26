#pragma once

#import <QuartzCore/CAMetalLayer.h>

@protocol GDTDisplayLayer <NSObject>

- (void)startRenderDisplayLayer;
- (void)stopRenderDisplayLayer;
- (void)initializeDisplayLayer;
- (void)layoutDisplayLayer;

@end
