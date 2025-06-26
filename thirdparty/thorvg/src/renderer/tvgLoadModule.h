#ifndef _TVG_LOAD_MODULE_H_
#define _TVG_LOAD_MODULE_H_

#include <atomic>
#include "tvgCommon.h"
#include "tvgRender.h"
#include "tvgInlist.h"


struct LoadModule
{
    INLIST_ITEM(LoadModule);

    //Use either hashkey(data) or hashpath(path)
    union {
        uintptr_t hashkey;
        char* hashpath = nullptr;
    };

    FileType type;                                  //current loader file type
    atomic<uint16_t> sharing{};                     //reference count
    bool readied = false;                           //read done already.
    bool pathcache = false;                         //cached by path

    LoadModule(FileType type) : type(type) {}
    virtual ~LoadModule()
    {
        if (pathcache) free(hashpath);
    }

    virtual bool open(const string& path) { return false; }
    virtual bool open(const char* data, uint32_t size, bool copy) { return false; }
    virtual bool resize(Paint* paint, float w, float h) { return false; }
    virtual void sync() {};  //finish immediately if any async update jobs.

    virtual bool read()
    {
        if (readied) return false;
        readied = true;
        return true;
    }

    bool cached()
    {
        if (hashkey) return true;
        return false;
    }

    virtual bool close()
    {
        if (sharing == 0) return true;
        --sharing;
        return false;
    }
};


struct ImageLoader : LoadModule
{
    static atomic<ColorSpace> cs;                   //desired value

    float w = 0, h = 0;                             //default image size
    RenderSurface surface;

    ImageLoader(FileType type) : LoadModule(type) {}

    virtual bool animatable() { return false; }  //true if this loader supports animation.
    virtual Paint* paint() { return nullptr; }

    virtual RenderSurface* bitmap()
    {
        if (surface.data) return &surface;
        return nullptr;
    }
};


struct FontMetrics
{
    //TODO: add necessary metrics
    float minw;
};


struct FontLoader : LoadModule
{
    FontLoader(FileType type) : LoadModule(type) {}

    using LoadModule::read;

    virtual bool read(Shape* shape, char* text, FontMetrics& out) = 0;
    virtual float transform(Paint* paint, FontMetrics& mertrics, float fontSize, bool italic) = 0;
};

#endif //_TVG_LOAD_MODULE_H_
