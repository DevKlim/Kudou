#ifndef _TVG_SIMPLE_XML_PARSER_H_
#define _TVG_SIMPLE_XML_PARSER_H_

#include "tvgSvgLoaderCommon.h"

#define NUMBER_OF_XML_ENTITIES 9
const char* const xmlEntity[] = {"&#10;", "&quot;", "&nbsp;", "&apos;", "&amp;", "&lt;", "&gt;", "&#035;", "&#039;"};
const int xmlEntityLength[] = {5, 6, 6, 6, 5, 4, 4, 6, 6};

enum class SimpleXMLType
{
    Open = 0,     //!< \<tag attribute="value"\>
    OpenEmpty,    //!< \<tag attribute="value" /\>
    Close,        //!< \</tag\>
    Data,         //!< tag text data
    CData,        //!< \<![cdata[something]]\>
    Error,        //!< error contents
    Processing,   //!< \<?xml ... ?\> \<?php .. ?\>
    Doctype,      //!< \<!doctype html
    Comment,      //!< \<!-- something --\>
    Ignored,      //!< whatever is ignored by parser, like whitespace
    DoctypeChild  //!< \<!doctype_child
};

typedef bool (*simpleXMLCb)(void* data, SimpleXMLType type, const char* content, unsigned int length);
typedef bool (*simpleXMLAttributeCb)(void* data, const char* key, const char* value);

bool simpleXmlParseAttributes(const char* buf, unsigned bufLength, simpleXMLAttributeCb func, const void* data);
bool simpleXmlParse(const char* buf, unsigned bufLength, bool strip, simpleXMLCb func, const void* data);
bool simpleXmlParseW3CAttribute(const char* buf, unsigned bufLength, simpleXMLAttributeCb func, const void* data);
const char* simpleXmlParseCSSAttribute(const char* buf, unsigned bufLength, char** tag, char** name, const char** attrs, unsigned* attrsLength);
const char* simpleXmlFindAttributesTag(const char* buf, unsigned bufLength);
bool isIgnoreUnsupportedLogElements(const char* tagName);
const char* simpleXmlNodeTypeToString(SvgNodeType type);

#endif //_TVG_SIMPLE_XML_PARSER_H_
