// (c) Mathias Panzenböck
// http://github.com/panzi/mathfun/blob/master/examples/portable_endian.h
//

#pragma once
 
#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)
  
#   define __WINDOWS__
   
#endif
    
#if defined(__linux__) || defined(__CYGWIN__) || defined(__GNU__)
     
#   include <endian.h>
      
#elif defined(__APPLE__)
       
#   include <machine/endian.h>
#   include <libkern/OSByteOrder.h>
        
#   define htobe16 OSSwapHostToBigInt16
#   define htole16 OSSwapHostToLittleInt16
#   define be16toh OSSwapBigToHostInt16
#   define le16toh OSSwapLittleToHostInt16
         
#   define htobe32 OSSwapHostToBigInt32
#   define htole32 OSSwapHostToLittleInt32
#   define be32toh OSSwapBigToHostInt32
#   define le32toh OSSwapLittleToHostInt32
          
#   define htobe64 OSSwapHostToBigInt64
#   define htole64 OSSwapHostToLittleInt64
#   define be64toh OSSwapBigToHostInt64
#   define le64toh OSSwapLittleToHostInt64
           
/**
#   define __BYTE_ORDER    BYTE_ORDER
#   define __BIG_ENDIAN    BIG_ENDIAN
#   define __LITTLE_ENDIAN LITTLE_ENDIAN
#   define __PDP_ENDIAN    PDP_ENDIAN
**/
            
#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#   include <sys/endian.h>

#elif defined(__FreeBSD_kernel__)
     
#   include <endian.h>
      
#elif defined(__WINDOWS__)
                    
#   if BYTE_ORDER == LITTLE_ENDIAN

        inline static uint16_t EndianSwap16(uint16_t u)
        {
            return (u << 8) |
                   (u >> 8);
        }

#       define htobe16 EndianSwap16
#       define htole16(x) (x)
#       define be16toh EndianSwap16
#       define le16toh(x) (x)

        inline static uint32_t EndianSwap32(uint32_t u)
        {
            return ((u & 0x000000FF) << 24) |
                   ((u & 0xFF000000) >> 24) |
                   ((u & 0x0000FF00) << 8) |
                   ((u & 0x00FF0000) >> 8);
        }

#       define htobe32 EndianSwap32
#       define htole32(x) (x)
#       define be32toh EndianSwap32
#       define le32toh(x) (x)

        inline static uint64_t EndianSwap64(uint64_t u)
        {
            return ((u & 0x00000000000000FF) << 56) |
                   ((u & 0xFF00000000000000) >> 56) |
                   ((u & 0x000000000000FF00) << 40) |
                   ((u & 0x00FF000000000000) >> 40) |
                   ((u & 0x0000000000FF0000) << 24) |
                   ((u & 0x0000FF0000000000) >> 24) |
                   ((u & 0x00000000FF000000) << 8) |
                   ((u & 0x000000FF00000000) >> 8);
        }

#       define htobe64 EndianSwap64
#       define htole64(x) (x)
#       define be64toh EndianSwap64
#       define le64toh(x) (x)
                         
#   elif BYTE_ORDER == BIG_ENDIAN
                          
                                /* that would be xbox 360 */
#       define htobe16(x) (x)
#       define htole16(x) __builtin_bswap16(x)
#       define be16toh(x) (x)
#       define le16toh(x) __builtin_bswap16(x)
                                 
#       define htobe32(x) (x)
#       define htole32(x) __builtin_bswap32(x)
#       define be32toh(x) (x)
#       define le32toh(x) __builtin_bswap32(x)
                                  
#       define htobe64(x) (x)
#       define htole64(x) __builtin_bswap64(x)
#       define be64toh(x) (x)
#       define le64toh(x) __builtin_bswap64(x)
                                   
#   else
                                    
#       error byte order not supported
                                     
#   endif
                                      
#   define __BYTE_ORDER    BYTE_ORDER
#   define __BIG_ENDIAN    BIG_ENDIAN
#   define __LITTLE_ENDIAN LITTLE_ENDIAN
#   define __PDP_ENDIAN    PDP_ENDIAN
                                       
#else
                                        
#   error platform not supported
                                         
#endif
