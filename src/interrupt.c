/*
 * \file    interrupt.c
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

#include <unistd.h>
#include <atomic.h>
#include <sys/netmgr.h>
#include <pci/cap_msi.h>
#include <pci/cap_msix.h>

#include "interrupt.h"

//#define MSI_DEBUG

#if CONFIG_QNX_INTERRUPT_ATTACH_EVENT == 1 || \
    CONFIG_QNX_INTERRUPT_ATTACH == 1
int chid = -1;
struct sigevent terminate_event;
#endif

/* atomic shutdown check */
volatile unsigned shutdown_program = 0x0; // 0x0 = running, 0x1 = shutdown

void terminate_run_wait () {
    atomic_set_value(&shutdown_program, 0x01);

#if CONFIG_QNX_INTERRUPT_ATTACH == 1 || \
    CONFIG_QNX_INTERRUPT_ATTACH_EVENT == 1

    if (MsgDeliverEvent(0, &terminate_event) == -1) {
        log_err("irq_trigger_event error; %s\n", strerror(errno));
    }
#else
    sleep(2);
#endif
}

#if CONFIG_QNX_INTERRUPT_ATTACH == 1
/*
 * Interrupt Service Routine (ISR)
 *
 * The actual ISR actually does no work except returning a QNX signal event.
 */
const struct sigevent *irq_handler (void *area, int id) {
    return &event;
}
#elif CONFIG_QNX_INTERRUPT_ATTACH_EVENT != 1
/*
 * Interrupt Service Routine (ISR)
 *
 * The actual ISR performs the work.
 */
const struct sigevent *irq_handler (void *area, int id) {
    log_enabled = false;

    // TODO: If raw ISR is ever needed it needs to be implemented here

    log_enabled = true;
    return NULL;
}
#endif

int request_irq (unsigned int irq, irq_handler_t handler, unsigned long flags,
        const char *name, void *dev)
{
    log_trace("request_irq; irq: %d, name: %s\n", irq, name);

    unsigned attach_flags =
        /*
         * Put the new event at the end of the list of existing events
         * instead of the start.
         */
          _NTO_INTR_FLAGS_END
        /*
         * Associate the interrupt event with the process instead of the
         * attaching thread. The interrupt event is removed when the process
         * exits, instead of when the attaching thread exits.
         */
        | _NTO_INTR_FLAGS_PROCESS
        /*
         * Track calls to InterruptMask() and InterruptUnmask() to make
         * detaching the interrupt handler safer.
         */
        | _NTO_INTR_FLAGS_TRK_MSK;

    struct net_device *ndev = (struct net_device *)dev;

#if CONFIG_QNX_INTERRUPT_ATTACH_EVENT == 1 || \
    CONFIG_QNX_INTERRUPT_ATTACH == 1
    if (chid == -1) {
        unsigned flags = _NTO_CHF_PRIVATE;

        chid = ChannelCreate(flags);
        int coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL, 0);

        SIGEV_SET_TYPE(&terminate_event, SIGEV_PULSE);
        terminate_event.sigev_coid = coid;
        terminate_event.sigev_code = 0; // not needed for termination
    }
#endif

    irq_group_t* group = irq_to_group_map[irq];

    uint_t i;
    for (i = 0; i < group->num_irq; ++i) {
        if (irq_attach_size == MAX_IRQ_ATTACH_COUNT) {
            log_err( "error request_irq reached max IRQ attach count: %lu\n",
                    irq_attach_size );

            return -1;
        }

        int k = irq_attach_size++;

#if CONFIG_QNX_INTERRUPT_ATTACH_EVENT == 1 || \
    CONFIG_QNX_INTERRUPT_ATTACH == 1
        SIGEV_SET_TYPE(&irq_attach[k].event, SIGEV_PULSE);
#endif

        int coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL, 0);

        irq_attach[k].event.sigev_coid = coid;
        irq_attach[k].event.sigev_code = k;
        irq_attach[k].irq = group->irq[i];
        irq_attach[k].irq_entry = i;
        irq_attach[k].handler = handler;
        irq_attach[k].dev = ndev;
        irq_attach[k].hdl = group->hdl;
        irq_attach[k].msi_cap = group->msi_cap;
        irq_attach[k].is_msix = group->is_msix;

        int id;
#if CONFIG_QNX_INTERRUPT_ATTACH_EVENT == 1
        if ((id = InterruptAttachEvent( group->irq[i],
                        &irq_attach[k].event, attach_flags )) == -1)
        {
            log_err("internal error; interrupt attach event failure\n");
        }
#else
        if ((id = InterruptAttach( group->irq[i],
                        &irq_handler, NULL, 0, attach_flags )) == -1)
        {
            log_err("internal error; interrupt attach failure\n");
        }
#endif
        irq_attach[k].id = id;
    }

    return 0;
}

void free_irq (unsigned int irq, void *dev) {
    log_trace("free_irq; irq: %d\n", irq);

    uint_t i, k;
    for (i = 0; i < irq_to_group_map[irq]->num_irq; ++i) {
        for (k = 0; k < irq_attach_size; ++k) {
            if (irq_attach[k].id != -1 &&
                irq_to_group_map[irq]->irq[i] == irq_attach[k].irq &&
                irq_attach[k].dev == dev)
            {
                InterruptDetach(irq_attach[k].id);

                irq_attach[k].id = -1;
                break;
            }
        }
    }
}

void run_wait() {
#if CONFIG_QNX_INTERRUPT_ATTACH == 1 || \
    CONFIG_QNX_INTERRUPT_ATTACH_EVENT == 1

    int rcvid;

    for(;;) {
        struct _pulse pulse;
        rcvid = MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL);

        if (shutdown_program) {
            log_err("MsgReceivePulse error; %s\n", strerror(errno));

            return;
        }

        if (rcvid == -1 ) {
            // handle error (errno)
            continue;
        }

        int_t k = pulse.code;

        if (k < 0) {
            continue;
        }

# if CONFIG_QNX_INTERRUPT_ATTACH_EVENT == 1
        InterruptUnmask(irq_attach[k].irq, irq_attach[k].id);
# endif

#ifdef MSI_DEBUG
        log_dbg("IRQ: %d\n", irq_attach[k].irq);
#endif

        if (irq_attach[k].msi_cap
            && pci_device_cfg_cap_isenabled(
                    irq_attach[k].hdl, irq_attach[k].msi_cap ))
        {
            if (irq_attach[k].is_msix) {
                pci_err_t err =
                    cap_msix_unmask_irq_entry(
                            irq_attach[k].hdl,
                            irq_attach[k].msi_cap,
                            irq_attach[k].irq_entry );
#ifdef MSI_DEBUG
                log_dbg("IRQ: MSI-X enabled\n");
#endif

                switch (err) {
#ifdef MSI_DEBUG
                    case PCI_ERR_EALREADY:
                        log_info("IRQ: PCI_ERR_EALREADY\n");
                        break;
#endif
                    case PCI_ERR_OK:
                        break;
                    default:
                        log_err("cap_msix_unmask_irq_entry error: %s\n",
                                pci_strerror(err));
                        break;
                };
            }
            else {
                pci_err_t err =
                    cap_msi_unmask_irq_entry(
                            irq_attach[k].hdl,
                            irq_attach[k].msi_cap,
                            irq_attach[k].irq_entry );
#ifdef MSI_DEBUG
                log_dbg("IRQ: MSI enabled\n");
#endif

                switch (err) {
                    case PCI_ERR_ENOTSUP:
#ifdef MSI_DEBUG
                        log_info("IRQ: Per Vector Masking (PVM) not "
                                 "supported\n");
                        break;
#endif
                    case PCI_ERR_OK:
                        break;
                    default:
                        log_err("cap_msi_unmask_irq_entry error: %s\n",
                                pci_strerror(err));
                        break;
                };
            }
        }
        else {
#ifdef MSI_DEBUG
            log_dbg("IRQ: non-MSI\n");
#endif
        }

        // handle pulse
        irq_attach[k].handler(irq_attach[k].irq, irq_attach[k].dev);
    }
#else /* CONFIG_QNX_INTERRUPT_ATTACH_EVENT != 1 &&
         CONFIG_QNX_INTERRUPT_ATTACH != 1 */
    while (1) {

        if (shutdown_program) {
            return;
        }

        /* Just spend some time doing nothing so the loop doesn't hard-lock */
        sleep(1);
    }
#endif
}
