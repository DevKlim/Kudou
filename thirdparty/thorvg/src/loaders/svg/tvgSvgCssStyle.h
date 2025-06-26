#ifndef _TVG_SVG_CSS_STYLE_H_
#define _TVG_SVG_CSS_STYLE_H_

#include "tvgSvgLoaderCommon.h"

void cssCopyStyleAttr(SvgNode* to, const SvgNode* from);
SvgNode* cssFindStyleNode(const SvgNode* style, const char* title, SvgNodeType type);
SvgNode* cssFindStyleNode(const SvgNode* style, const char* title);
void cssUpdateStyle(SvgNode* doc, SvgNode* style);
void cssApplyStyleToPostponeds(Array<SvgNodeIdPair>& postponeds, SvgNode* style);

#endif //_TVG_SVG_CSS_STYLE_H_
