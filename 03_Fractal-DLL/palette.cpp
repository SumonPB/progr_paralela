#include "palette.h"

/**
#FEE5D9
#FDD6C6
#FDC8B3
#FCB99F
#FCA98C
#FC9779
#FB8566
#FB7353
#F76245
#EF523C
#E84132
#E03128
#D32723
#C31F1E
#B4171A
#A50F15
*/

uint32_t bswap32(uint32_t a){
    return
    ((a & 0x000000FF) <<24)|
    ((a & 0x0000FF00) <<8)|
    ((a & 0x00FF0000) >>8)|
    ((a & 0xFF000000) >>24);
        
}

std::vector<uint32_t> color_ramp = {
    bswap32(0xFF00FF00),
    bswap32(0xFF00F200),
    bswap32(0xFF00E600),
    bswap32(0xFF00D900),
    bswap32(0xFF00CC00),
    bswap32(0xFF00BF00),
    bswap32(0xFF00B300),
    bswap32(0xFF00A600),
    bswap32(0xFF009900),
    bswap32(0xFF008C00),
    bswap32(0xFF008000),
    bswap32(0xFF007300),
    bswap32(0xFF006600),
    bswap32(0xFF005900),
    bswap32(0xFF004D00),
    bswap32(0xFF004000),
    bswap32(0xFF003300),
    bswap32(0xFF002600),
    bswap32(0xFF001900),
    bswap32(0xFF000D00),
    bswap32(0xFF000000)
};

