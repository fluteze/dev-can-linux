#include <stdlib.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <hw/inout.h>

#include "kernel/drivers/net/can/sja1000/sja1000.h"
#include "kernel/include/linux/pci.h"

extern struct pci_driver adv_pci_driver;
extern struct pci_driver kvaser_pci_driver;
extern struct net_device_ops sja1000_netdev_ops;

struct pci_driver *detected_driver = NULL;

struct net_device* device[16];
netdev_tx_t (*dev_xmit[16]) (struct sk_buff *skb, struct net_device *dev);

/**
 * Command:
 *
 * 		pci-tool -vvvvv
 *
 * Output:
 *
 * B000:D05:F00 @ idx 7
 *         vid/did: 13fe/c302
 *                 <vendor id - unknown>, <device id - unknown>
 *         class/subclass/reg: 0c/09/00
 *                 CANbus Serial Bus Controller
 *         revid: 0
 *         cmd/status registers: 103/0
 *         Capabilities list (0):
 *
 *         Address Space list - 2 assigned
 *             [0] I/O, addr=c000, size=400, align: 400, attr: 32bit CONTIG ENABLED
 *             [1] I/O, addr=c400, size=400, align: 400, attr: 32bit CONTIG ENABLED
 *         Interrupt list - 0 assigned
 *         hdrType: 0
 *                 ssvid: 13fe  ?
 *                 ssid:  c302
 *
 *         Device Dependent Registers
 *                 [40] 00000000  00000000  00000000  00000000
 *                   :
 *                 [f0] 00000000  00000000  00000000  00000000
 *
 */

// NETDEVICE
#include "linux/netdevice.h"

void netif_wake_queue(struct net_device *dev)
{
	syslog(LOG_INFO, "netif_wake_queue");
}

int netif_rx(struct sk_buff *skb)
{
	struct can_frame* msg = (struct can_frame*)skb->data;

	syslog(LOG_INFO, "netif_rx; can%d: %x#%2x%2x%2x%2x%2x%2x%2x%2x",
			skb->dev_id,
			msg->can_id,
			msg->data[0],
			msg->data[1],
			msg->data[2],
			msg->data[3],
			msg->data[4],
			msg->data[5],
			msg->data[6],
			msg->data[7]);

	return 0;
}

void netif_start_queue(struct net_device *dev)
{
	syslog(LOG_INFO, "netif_start_queue");
}

void netif_carrier_on(struct net_device *dev)
{
	syslog(LOG_INFO, "netif_carrier_on");
	// Not called from SJA1000 driver
}

void netif_carrier_off(struct net_device *dev)
{
	syslog(LOG_INFO, "netif_carrier_off");
}

int netif_queue_stopped(const struct net_device *dev)
{
	syslog(LOG_INFO, "netif_queue_stopped");
	return false;
}

void netif_stop_queue(struct net_device *dev)
{
	syslog(LOG_INFO, "netif_stop_queue");
}
// NETDEVICE

// DEV
#include "linux/can/dev.h"

int register_candev(struct net_device *dev) {
	snprintf(dev->name, IFNAMSIZ, "can%d", dev->dev_id);

	if (dev->dev_id >= 16) {
		syslog(LOG_ERR, "device id (%d) exceeds max (%d)", dev->dev_id, 16);
	}

	dev_xmit[dev->dev_id] = dev->netdev_ops->ndo_start_xmit;
	device[dev->dev_id] = dev;

	syslog(LOG_INFO, "register_candev: %s", dev->name);

	if (sja1000_netdev_ops.ndo_open(dev)) {
		return -1;
	}

	return 0;
}

void unregister_candev(struct net_device *dev) {
	syslog(LOG_INFO, "unregister_candev: %s", dev->name);

	if (sja1000_netdev_ops.ndo_stop(dev)) {
		syslog(LOG_ERR, "internal error; ndo_stop failure");
	}
}

int open_candev(struct net_device *dev)
{
	syslog(LOG_INFO, "open_candev: %s", dev->name);
	return 0;
}

void close_candev(struct net_device *dev)
{
	syslog(LOG_INFO, "close_candev: %s", dev->name);
}
// DEV

// INTERRUPT
#include "linux/interrupt.h"

struct sigevent event;
int id = -1;

struct func_t {
	irq_handler_t handler;
	struct net_device *dev;
	unsigned int irq;
};
struct func_t funcs[16];
int funcs_size = 0;

const struct sigevent *irq_handler( void *area, int id ) {
	return( &event );
}

extern int
request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
	    const char *name, void *dev)
{
	syslog(LOG_INFO, "request_irq; irq: %d, name: %s", irq, name);

	struct net_device *ndev = (struct net_device *)dev;

	struct func_t f = {
			.handler = handler,
			.dev = ndev,
			.irq = irq
	};

	funcs[funcs_size++] = f;

	if (id == -1) {
		SIGEV_SET_TYPE(&event, SIGEV_INTR);

	    if ((id = InterruptAttach(irq, &irq_handler, NULL, 0, 0)) == -1) {
	    	syslog(LOG_ERR, "internal error; interrupt attach failure");
	    }
	}

	return 0;
}

void free_irq(unsigned int irq, void *dev)
{
	syslog(LOG_INFO, "free_irq; irq: %d", irq);

	// Disconnect the ISR handler
    InterruptDetach(id);
}
// INTERRUPT

// PCI
int pci_enable_device(struct pci_dev *dev) {
	syslog(LOG_INFO, "pci_enable_device: %x:%x",
			dev->vendor, dev->device);

	uint_t idx = 0;
	pci_bdf_t bdf = 0;

	while ((bdf = pci_device_find(idx, dev->vendor, dev->device, PCI_CCODE_ANY)) != PCI_BDF_NONE)
	{
	    pci_err_t r;

		pci_devhdl_t hdl = pci_device_attach(bdf, pci_attachFlags_EXCLUSIVE_OWNER, &r);
		dev->hdl = hdl;

        if (hdl == NULL) {
			switch (r) {
			case PCI_ERR_EINVAL:
				syslog(LOG_ERR, "pci device attach failed; Invalid flags.");
				break;
			case PCI_ERR_ENODEV:
				syslog(LOG_ERR, "pci device attach failed; The bdf argument doesn't refer to a valid device.");
				break;
			case PCI_ERR_ATTACH_EXCLUSIVE:
				syslog(LOG_ERR, "pci device attach failed; The device identified by bdf is already exclusively owned.");
				break;
			case PCI_ERR_ATTACH_SHARED:
				syslog(LOG_ERR, "pci device attach failed; The request was for exclusive attachment, but the device identified by bdf has already been successfully attached to.");
				break;
			case PCI_ERR_ATTACH_OWNED:
				syslog(LOG_ERR, "pci device attach failed; The request was for ownership of the device, but the device is already owned.");
				break;
			case PCI_ERR_ENOMEM:
				syslog(LOG_ERR, "pci device attach failed; Memory for internal resources couldn't be obtained. This may be a temporary condition.");
				break;
			case PCI_ERR_LOCK_FAILURE:
				syslog(LOG_ERR, "pci device attach failed; There was an error related to the creation, acquisition, or use of a synchronization object.");
				break;
			case PCI_ERR_ATTACH_LIMIT:
				syslog(LOG_ERR, "pci device attach failed; There have been too many attachments to the device identified by bdf.");
				break;
			default:
				syslog(LOG_ERR, "pci device attach failed; Unknown error: %d", (int)r);
				break;
			}

			return -1;
        }

        /* optionally determine capabilities of device */
        uint_t capid_idx = 0;
        pci_capid_t capid;

        /* instead of looping could use pci_device_find_capid() to select which capabilities to use */
        while ((r = pci_device_read_capid(bdf, &capid, capid_idx)) == PCI_ERR_OK)
        {
			syslog(LOG_INFO, "read capability[%d]: %x", capid_idx, capid);

            /* get next capability ID */
            ++capid_idx;
        }

        pci_ba_t ba[7];    // the maximum number of entries that can be returned
        int_t nba = NELEMENTS(ba);

        /* read the address space information */
        r = pci_device_read_ba(hdl, &nba, ba, pci_reqType_e_UNSPECIFIED);
        if ((r == PCI_ERR_OK) && (nba > 0))
        {
			dev->nba = nba;
        	dev->ba = (pci_ba_t*)malloc(nba*sizeof(pci_ba_t));

            for (int_t i=0; i<nba; i++)
            {
            	dev->ba[i] = ba[i];

    			syslog(LOG_INFO, "read ba[%d] { addr: %x, size: %x }",
    					i, ba[i].addr, ba[i].size);
            }
        }

        pci_irq_t irq[10];
        int_t nirq = NELEMENTS(irq);

        /* read the irq information */
        r = pci_device_read_irq(hdl, &nirq, irq);
        if ((r == PCI_ERR_OK) && (nirq > 0))
        {
        	dev->irq = irq[0];

            for (int_t i=0; i<nirq; i++) {
    			syslog(LOG_INFO, "read irq[%d]: %d", i, irq[i]);
            }

        	if (nirq > 1) {
        		syslog(LOG_ERR, "read multiple (%d) IRQs", nirq);

        		return -1;
        	}
        }

	    /* get next device instance */
	    ++idx;
	}

	return 0;
}

void pci_disable_device(struct pci_dev *dev) {
	syslog(LOG_INFO, "pci_disable_device");

	if (dev != NULL) {
		if (dev->hdl != NULL) {
			pci_device_detach(dev->hdl);
		}
	}
}

uintptr_t pci_iomap(struct pci_dev *dev, int bar, unsigned long max) {
	syslog(LOG_INFO, "pci_iomap; bar: %d, max: %d", bar, max);

	if (bar >= dev->nba) {
		syslog(LOG_ERR, "internal error; bar: %d, nba: %d", bar, dev->nba);
	}

	if (dev->ba[bar].size > max) {
		dev->ba[bar].size = max;
	}

	/* mmap() the address space(s) */

	if (mmap_device_io(dev->ba[bar].size, dev->ba[bar].addr) == MAP_DEVICE_FAILED) {
		switch (errno) {
		case EINVAL:
			syslog(LOG_ERR, "pci device address mapping failed; Invalid flags type, or len is 0.");
			break;
		case ENOMEM:
			syslog(LOG_ERR, "pci device address mapping failed; The address range requested is outside of the allowed process address range, or there wasn't enough memory to satisfy the request.");
			break;
		case ENXIO:
			syslog(LOG_ERR, "pci device address mapping failed; The address from io for len bytes is invalid.");
			break;
		case EPERM:
			syslog(LOG_ERR, "pci device address mapping failed; The calling process doesn't have the required permission; see procmgr_ability().");
			break;
		default:
			syslog(LOG_ERR, "pci device address mapping failed; Unknown error: %d", errno);
			break;
		}
	}
	else {
		syslog(LOG_INFO, "ba[%d] mapping successful", bar);
	}

	return dev->ba[bar].addr;
}

void pci_iounmap(struct pci_dev *dev, uintptr_t p) {
	int bar = -1;
	uint64_t size = 0u;

	for (int i = 0; i < dev->nba; ++i) {
		if (dev->ba[i].addr == p) {
			bar = i;
			size = dev->ba[i].size;
			break;
		}
	}

	syslog(LOG_INFO, "pci_iounmap; bar: %d, size: %d", bar, size);

	if (bar == -1 || !size) {
		syslog(LOG_ERR, "internal error; bar size failure during pci_iounmap");
	}

	if (munmap_device_io(p, size) == -1) {
		syslog(LOG_ERR, "internal error; munmap_device_io failure");
	}
}

int pci_request_regions(struct pci_dev *dev, const char *res_name) {
	syslog(LOG_INFO, "pci_request_regions");

	return 0;
}

void pci_release_regions(struct pci_dev *dev) {
	syslog(LOG_INFO, "pci_release_regions");
}
// PCI

int check_driver_support (const struct pci_driver* driver, pci_vid_t vid, pci_did_t did) {
	if (driver->id_table != NULL) {
		const struct pci_device_id *id_table = driver->id_table;

		while (id_table->vendor != 0) {
			if (id_table->vendor == vid &&
				id_table->device == did)
			{
				return 1;
			}

			++id_table;
		}
	}

	return 0;
}

void print_card (const struct pci_driver* driver) {
	printf("  Name of driver: %s\n", driver->name);
	printf("  Supported cards:\n");

	if (driver->id_table != NULL) {
		const struct pci_device_id *id_table = driver->id_table;

		while (id_table->vendor != 0) {
			printf("    { vendor: %x, device: %x, subvendor: %x, subdevice: %x, class: %x, class_mask: %x\n",
				id_table->vendor,
				id_table->device,
				id_table->subvendor,
				id_table->subdevice,
				id_table->class,
				id_table->class_mask);
			++id_table;
		}
	}
}

int main(void) {
	pci_vid_t vid;
    pci_did_t did;

	uint_t idx = 0;
	pci_bdf_t bdf = 0;

	syslog(LOG_INFO, "start");

	ThreadCtl(_NTO_TCTL_IO, 0);

	printf("Advantech CAN cards:\n");
	print_card(&adv_pci_driver);

	printf("Kvaser CAN cards:\n");
	print_card(&kvaser_pci_driver);

	while ((bdf = pci_device_find(idx, PCI_VID_ANY, PCI_DID_ANY, PCI_CCODE_ANY)) != PCI_BDF_NONE)
	{
	    pci_err_t r = pci_device_read_vid(bdf, &vid);

	    if (r != PCI_ERR_OK) {
	    	continue;
	    }

	    r = pci_device_read_did(bdf, &did);

	    if (r == PCI_ERR_OK)
	    {
	    	/* does this driver handle this device ? */

		    if (check_driver_support(&adv_pci_driver, vid, did)) {
		    	detected_driver = &adv_pci_driver;
		    }
		    else if (check_driver_support(&kvaser_pci_driver, vid, did)) {
		    	detected_driver = &kvaser_pci_driver;
		    }

		    if (detected_driver) {
		    	syslog(LOG_INFO, "checking device: %x:%x <- detected", vid, did);
		    }
		    else {
		    	syslog(LOG_INFO, "checking device: %x:%x", vid, did);
		    }
	    }

	    /* get next device instance */
	    ++idx;
	}

	if (!detected_driver) {
		return -1;
	}

	struct pci_dev pdev = {
			.ba = NULL,
			.vendor = vid,
			.device = did,
			.dev = { .driver_data = NULL },
			.irq = 10
	};

	if (detected_driver->probe(&pdev, NULL)) {
		return -1;
	}

	struct sk_buff *skb;
	struct can_frame *cf;

	/* create zero'ed CAN frame buffer */
	skb = alloc_can_skb(device[1], &cf);

	if (skb == NULL) {
		adv_pci_driver.remove(&pdev);

		return -1;
	}

	int g = 0;
	while (1) {
		int i;
		InterruptWait( 0, NULL );

		for (i = 0; i < funcs_size; ++i) {
			funcs[i].handler(funcs[i].irq, funcs[i].dev);
		}

		if (++g % 5 == 0)
		dev_xmit[1](skb, device[1]);
	}

	adv_pci_driver.remove(&pdev);

	return EXIT_SUCCESS;
}
