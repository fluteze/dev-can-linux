#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H

#include <stdlib.h>
#include <wchar.h>
#include <stdint.h>

typedef int8_t __s8;
typedef int16_t __s16;
typedef int32_t __s32;

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;

typedef __s8 s8;
typedef __s16 s16;
typedef __s32 s32;

typedef __u8 u8;
typedef __u16 u16;
typedef __u32 u32;

struct sk_buff {
	unsigned char* data;
    unsigned short dev_id;
};

#define __KERNEL__
#define __UAPI_DEF_IF_IFNAMSIZ
#define __UAPI_DEF_IF_NET_DEVICE_FLAGS
#define __UAPI_DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO

/* Do nothing for the following macros */
#define MODULE_AUTHOR(str)
#define MODULE_DESCRIPTION(str)
#define MODULE_SUPPORTED_DEVICE(str)
#define MODULE_LICENSE(str)
#define MODULE_DEVICE_TABLE(pci, tbl)
#define module_pci_driver(pci_driver)

/* Define mapping to QNX IO */
#define readb(addr)			in8(addr)
#define readw(addr)			in16(addr)
#define readl(addr)			in32(addr)

#define ioread8(addr)       readb(addr)
#define ioread16(addr)      readw(addr)
#define ioread32(addr)      readl(addr)

#define writeb(v, addr)		out8((addr), (v))
#define writew(v, addr)		out16((addr), (v))
#define writel(v, addr)		out32((addr), (v))

#define iowrite8(v, addr)   writeb((v), (addr))
#define iowrite16(v, addr)  writew((v), (addr))
#define iowrite32(v, addr)  writel((v), (addr))

/* Define mapping of dev_*() calls to syslog(*) */
#define dev_err(dev, fmt, arg...) syslog(LOG_ERR, fmt, ##arg)
#define dev_info(dev, fmt, arg...) syslog(LOG_INFO, fmt, ##arg)
#define dev_dbg(dev, fmt, arg...) syslog(LOG_DEBUG, fmt, ##arg)

/* Define mapping of kzalloc to simply malloc */
#define kzalloc(size, gfp) malloc(size)
#define kfree(ptr) free(ptr)

#endif /* _LINUX_TYPES_H */
