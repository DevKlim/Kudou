#pragma once

#import <UIKit/UIKit.h>

class String;

@class GDTView;
@protocol GDTDisplayLayer;
@protocol GDTViewRendererProtocol;

@protocol GDTViewDelegate

- (BOOL)godotViewFinishedSetup:(GDTView *)view;

@end

@interface GDTView : UIView

@property(assign, nonatomic) id<GDTViewRendererProtocol> renderer;
@property(assign, nonatomic) id<GDTViewDelegate> delegate;

@property(assign, readonly, nonatomic) BOOL isActive;

@property(assign, nonatomic) BOOL useCADisplayLink;
@property(strong, readonly, nonatomic) CALayer<GDTDisplayLayer> *renderingLayer;
@property(assign, readonly, nonatomic) BOOL canRender;

@property(assign, nonatomic) NSTimeInterval renderingInterval;

// Can be extended by subclasses
- (void)godot_commonInit;

// Implemented in subclasses
- (CALayer<GDTDisplayLayer> *)initializeRenderingForDriver:(NSString *)driverName;

- (void)stopRendering;
- (void)startRendering;

@end

// Implemented in subclasses
extern GDTView *GDTViewCreate();
