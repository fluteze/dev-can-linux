/*
 * \file    config.h
 *
 * Copyright (C) 2022 Deniz Eren <deniz.eren@outlook.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>

#include <linux/irqreturn.h>

#include <sys/neutrino.h> /* QNX header */
#include <pci/pci.h>      /* QNX header */

#include "timer.h"

#define DEFAULT_NUM_RX_CHANNELS 1
#define DEFAULT_NUM_TX_CHANNELS 1
#define DEFAULT_RESTART_MS      50 // Default bus-off restart delay


/*
 * Program options
 *
 * See print_help() or dev-can-linux -h for details
 */

extern int optr;
extern int optr_restart_ms;
extern int optv;
extern int optl;
extern int opti;
extern int optq;
extern int optd;
extern int opte;
extern int opts;
extern int optt;
extern int optu;
extern int optx;

typedef struct {
    int id;
    int num_rx_channels;
    int num_tx_channels;
    int is_extended_mid;
    int bitrate;            /* Bit-rate in bits/second */
    int bprm;               /* Bit-rate prescaler */
    int phase_seg1;         /* Phase buffer segment 1 in TQs */
    int phase_seg2;         /* Phase buffer segment 2 in TQs */
    int sjw;                /* Synchronisation jump width in TQs */
    int btr0;               /* SJA1000 BTR0 register */
    int btr1;               /* SJA1000 BTR1 register */
} channel_config_t;

extern size_t num_optu_configs;
extern channel_config_t* optu_config;

typedef struct {
    int vid;
    int did;
    int cap; // cap=-1 to disable entire device
} device_config_t;

extern size_t num_disable_device_configs;
extern device_config_t* disable_device_config;
extern size_t num_enable_device_cap_configs;
extern device_config_t* enable_device_cap_config;


/*
 * Linux Kernel platform and architecture macros
 */

#define __KERNEL__
#define __iomem volatile

#if defined(__X86_64__) || defined(__aarch64__)
#define BITS_PER_LONG (64)
#else /* 32-bit system */
#define BITS_PER_LONG (32)
#endif

/* Defines the need for macro IFNAMSIZ */
#define __UAPI_DEF_IF_IFNAMSIZ (1)

/* Defines the need for macros
 *  - IFF_UP        interface is up
 *  - IFF_NOARP     no address resolution protocol */
#define __UAPI_DEF_IF_NET_DEVICE_FLAGS (1)

/* Defines the need for macro IFF_ECHO; this is a unique network device flags in
 * that no equivalent exists in QNX (e.g. /usr/include/net/if.h). */
#define __UAPI_DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO (1)

/*
 * Linux Kernel compiler definition macros
 */

#define __always_inline     inline // Impact: reduce binary size
#define __must_check
#define __attribute_const__

/*
 * Mapping stdint.h types to Linux Kernel needed type names
 */

typedef int8_t      __s8;
typedef int16_t     __s16;
typedef int32_t     __s32;
typedef int64_t     __s64;

typedef uint8_t     __u8;
typedef uint16_t    __u16;
typedef uint32_t    __u32;
typedef uint64_t    __u64;

typedef __s8        s8;
typedef __s16       s16;
typedef __s32       s32;
typedef __s64       s64;

typedef __u8        u8;
typedef __u16       u16;
typedef __u32       u32;
typedef __u64       u64;

/*
 * IRQ Management
 */

/* MAX_IRQ_ATTACH_COUNT
 * We currently use sigevent.sigev_code to generate struct _pulse (SIGEV_PULSE)
 * with code (int16_t) payload to store the IRQ attach array index for entry.
 * Only positive values can be used; values less than zero conflict with kernel
 * or pulse codes generated by a QNX Neutrino manager. */
#define MAX_IRQ_ATTACH_COUNT    (127)

typedef struct {
    pci_irq_t*          irq;
    size_t              num_irq;
    pci_devhdl_t        hdl;
    pci_cap_t           msi_cap;
    bool                is_msix;
} irq_group_t;

typedef struct {
    struct sigevent     event;
    int                 id;
    pci_irq_t           irq;
    struct net_device*  dev;
    irqreturn_t         (*handler)(int, void*);
    pci_devhdl_t        hdl;
    pci_cap_t           msi_cap;
    uint_t              irq_entry;
    bool                is_msix;

    void                (*unmask)(uint_t);
    void                (*mask)(uint_t);
} irq_attach_t;

extern irq_group_t*     irq_group;
extern size_t           irq_group_size;
extern irq_group_t**    irq_to_group_map;
extern size_t           irq_to_group_map_size;
extern irq_attach_t     irq_attach[MAX_IRQ_ATTACH_COUNT];
extern size_t           irq_attach_size;

void irq_group_add (pci_irq_t* irq, size_t nirq,
        pci_devhdl_t hdl, pci_cap_t msi_cap, bool is_msix);

void irq_group_cleanup (void);

#endif /* SRC_CONFIG_H_ */
