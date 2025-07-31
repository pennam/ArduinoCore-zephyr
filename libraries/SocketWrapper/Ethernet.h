#include "SocketHelpers.h"

#include <zephyr/net/ethernet.h>

#if DT_HAS_COMPAT_STATUS_OKAY(ethernet_phy)

enum EthernetLinkStatus {
  Unknown,
  LinkON,
  LinkOFF
};

enum EthernetHardwareStatus {
  EthernetNoHardware,
  EthernetOk
};

class EthernetClass: public NetworkInterface
{
public:
	EthernetClass() {}
	virtual ~EthernetClass() {}

    int begin(bool blocking = true, uint32_t additional_event_mask = 0) {
        hardwareStatus();
        return NetworkInterface::begin(blocking, additional_event_mask);
    }

    int begin(uint8_t *mac, unsigned long timeout = 60000, unsigned long responseTimeout = 4000) {
        hardwareStatus();
        if (mac != nullptr) {
            NetworkInterface::setMACAddress(mac);
        }
        return NetworkInterface::begin(true, 0);
    }

	int maintain(); //TODO
	
    EthernetLinkStatus linkStatus() {
        hardwareStatus();
        if (net_if_is_up(netif)) {
            return LinkON;
        } else {
            return LinkOFF;
        }
    }
	
    EthernetHardwareStatus hardwareStatus() {
        const struct device *const dev = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(ethernet_phy));
        if (device_is_ready(dev)) {
            for (int i = 1; i < 3; i++) {
                auto _if = net_if_get_by_index(i);
                if (!net_eth_type_is_wifi(_if)) {
                    netif = _if;
                    break;
                }
            }
            return EthernetOk;
        } else {
            return EthernetNoHardware;
        }
    }    

    int begin(uint8_t *mac, IPAddress ip) {
        return begin(); //TODO
    }
	int begin(uint8_t *mac, IPAddress ip, IPAddress dns) {
        return begin(); //TODO
    }
	int begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway) {
        return begin(); //TODO
    }
	int begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet) {
        return begin(); //TODO
    }  
	void init(uint8_t sspin = 10); //TODO
};
extern EthernetClass Ethernet;

#endif