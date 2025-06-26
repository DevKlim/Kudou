#pragma once

#import <UIKit/UIKit.h>

@protocol GDTViewRendererProtocol <NSObject>

@property(assign, readonly, nonatomic) BOOL hasFinishedSetup;

- (BOOL)setupView:(UIView *)view;
- (void)renderOnView:(UIView *)view;

@end

@interface GDTViewRenderer : NSObject <GDTViewRendererProtocol>

@end
