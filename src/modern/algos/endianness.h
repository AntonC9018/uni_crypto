#include <mydefines.h>

#define u32_toggle_endianness(x)	\
	(((x) << 24) | (((x) << 8) & 0xFF0000) | (((x) >> 8) & 0xFF00) | ((x) >> 24))

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define u32_to_little_endian(x) u32_toggle_endianness((x)) 
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define u32_to_little_endian(x) (x) 
#else
    #error unsupported byte order
#endif

inline u32 to_u32_little_endian(u8* bytes)
{
    return (u32)bytes[0]      |
        ((u32)bytes[1] << 8)  | 
        ((u32)bytes[2] << 16) |
        ((u32)bytes[3] << 24);
}

inline u32 to_u32_little_endian(const u8* bytes)
{
    return (u32)bytes[0]      |
        ((u32)bytes[1] << 8)  | 
        ((u32)bytes[2] << 16) |
        ((u32)bytes[3] << 24);
}