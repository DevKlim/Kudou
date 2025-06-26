#ifndef _TVG_RAW_LOADER_H_
#define _TVG_RAW_LOADER_H_

class RawLoader : public ImageLoader
{
public:
    bool copy = false;

    RawLoader();
    ~RawLoader();

    using LoadModule::open;
    bool open(const uint32_t* data, uint32_t w, uint32_t h, bool copy);
    bool read() override;
};


#endif //_TVG_RAW_LOADER_H_
