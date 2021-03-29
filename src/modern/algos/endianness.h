#define u32_toggle_endianness(x)	\
	(((x) << 24) | (((x) << 8) & 0xFF0000) | (((x) >> 8) & 0xFF00) | ((x) >> 24))

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define u32_to_little_endian(x) u32_toggle_endianness((x)) 
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define u32_to_little_endian(x) (x) 
#else
    #error unsupported byte order
#endif