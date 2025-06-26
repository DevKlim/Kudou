#pragma once

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#include "core/templates/hash_map.h"
#include "core/variant/typed_array.h"
#include "core/variant/variant.h"

@interface GodotOpenSaveDelegate : NSObject <NSOpenSavePanelDelegate> {
	NSSavePanel *dialog;
	NSMutableArray *allowed_types;

	HashMap<int, String> ctr_ids;
	Dictionary options;
	int cur_index;
	int ctr_id;

	String root;
}

- (void)makeAccessoryView:(NSSavePanel *)p_panel filters:(const Vector<String> &)p_filters options:(const TypedArray<Dictionary> &)p_options;
- (void)setFileTypes:(NSMutableArray *)p_allowed_types;
- (void)popupOptionAction:(id)p_sender;
- (void)popupCheckAction:(id)p_sender;
- (void)popupFileAction:(id)p_sender;
- (int)getIndex;
- (Dictionary)getSelection;
- (int)setDefaultInt:(const String &)p_name value:(int)p_value;
- (int)setDefaultBool:(const String &)p_name value:(bool)p_value;
- (void)setRootPath:(const String &)p_root_path;

@end
