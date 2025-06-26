#include "ConvectionKernels_Config.h"

#if !defined(CVTT_SINGLE_FILE) || defined(CVTT_SINGLE_FILE_IMPL)

#include "ConvectionKernels_BCCommon.h"

int cvtt::Internal::BCCommon::TweakRoundsForRange(int range)
{
    if (range == 3)
        return 3;
    return 4;
}

#endif
