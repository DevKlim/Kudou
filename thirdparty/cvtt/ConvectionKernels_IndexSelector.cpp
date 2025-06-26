#include "ConvectionKernels_Config.h"

#if !defined(CVTT_SINGLE_FILE) || defined(CVTT_SINGLE_FILE_IMPL)

#include "ConvectionKernels_IndexSelector.h"

namespace cvtt
{
    namespace Internal
    {
        const ParallelMath::UInt16 g_weightReciprocals[17] =
        {
            ParallelMath::MakeUInt16(0),        // -1 
            ParallelMath::MakeUInt16(0),        // 0
            ParallelMath::MakeUInt16(32768),    // 1
            ParallelMath::MakeUInt16(16384),    // 2
            ParallelMath::MakeUInt16(10923),    // 3
            ParallelMath::MakeUInt16(8192),     // 4
            ParallelMath::MakeUInt16(6554),     // 5
            ParallelMath::MakeUInt16(5461),     // 6
            ParallelMath::MakeUInt16(4681),     // 7
            ParallelMath::MakeUInt16(4096),     // 8
            ParallelMath::MakeUInt16(3641),     // 9
            ParallelMath::MakeUInt16(3277),     // 10
            ParallelMath::MakeUInt16(2979),     // 11
            ParallelMath::MakeUInt16(2731),     // 12
            ParallelMath::MakeUInt16(2521),     // 13
            ParallelMath::MakeUInt16(2341),     // 14
            ParallelMath::MakeUInt16(2185),     // 15
        };
    }
}

#endif
