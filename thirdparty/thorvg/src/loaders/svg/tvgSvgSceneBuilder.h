#ifndef _TVG_SVG_SCENE_BUILDER_H_
#define _TVG_SVG_SCENE_BUILDER_H_

#include "tvgCommon.h"

Scene* svgSceneBuild(SvgLoaderData& loaderData, Box vBox, float w, float h, AspectRatioAlign align, AspectRatioMeetOrSlice meetOrSlice, const string& svgPath, SvgViewFlag viewFlag);

#endif //_TVG_SVG_SCENE_BUILDER_H_
