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

	int begin(uint8_t *mac = nullptr, unsigned long timeout = 60000, unsigned long responseTimeout = 4000);
	int maintain();
	EthernetLinkStatus linkStatus();
	EthernetHardwareStatus hardwareStatus();

	// Manual configuration
	int begin(uint8_t *mac, IPAddress ip);
	int begin(uint8_t *mac, IPAddress ip, IPAddress dns);
	int begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway);
	int begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet, unsigned long timeout = 60000, unsigned long responseTimeout = 4000);

	int begin(IPAddress ip) {
		return begin(nullptr, ip);
	}
	int begin(IPAddress ip, IPAddress dns) {
		return begin(nullptr, ip, dns);
	}
	int begin(IPAddress ip, IPAddress dns, IPAddress gateway) {
		return begin(nullptr, ip, dns, gateway);
	}
	int begin(IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet) {
		return begin(nullptr, ip, dns, gateway, subnet);
	}
	void init(uint8_t sspin = 10);

	int disconnect(void);
	void end(void);

	void MACAddress(uint8_t *mac_address);
	IPAddress localIP();
	IPAddress subnetMask();
	IPAddress gatewayIP();
	IPAddress dnsServerIP();

	void setMACAddress(const uint8_t *mac_address);
	void setLocalIP(const IPAddress local_ip);
	void setSubnetMask(const IPAddress subnet);
	void setGatewayIP(const IPAddress gateway);
	void setDnsServerIP(const IPAddress dns_server);

	void setRetransmissionTimeout(uint16_t milliseconds);
	void setRetransmissionCount(uint8_t num);
};

extern EthernetClass Ethernet;

#endif
