#include "Ethernet.h"

#if DT_HAS_COMPAT_STATUS_OKAY(ethernet_phy)

int EthernetClass::begin(uint8_t *mac, unsigned long timeout, unsigned long responseTimeout) {
    setMACAddress(mac);
    return NetworkInterface::begin(true, 0);
}

int EthernetClass::begin(uint8_t *mac, IPAddress ip) {
    IPAddress dns = ip;
    dns[3] = 1;

    auto ret = begin(mac, ip, dns);
    return ret;
}

int EthernetClass::begin(uint8_t *mac, IPAddress ip, IPAddress dns) {
    IPAddress gateway = ip;
    gateway[3] = 1;

    auto ret = begin(mac, ip, dns, gateway);
    return ret;
}

int EthernetClass::begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway) {
    IPAddress subnet(255, 255, 255, 0);
    auto ret = begin(mac, ip, dns, gateway, subnet);
    return ret;
}

int EthernetClass::begin(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet, unsigned long timeout, unsigned long responseTimeout) {
    // TODO: Config the network interface with the provided IP, DNS, gateway, and subnet

    return begin(mac, timeout, responseTimeout);
}

EthernetLinkStatus EthernetClass::linkStatus() {
    if ((hardwareStatus() == EthernetOk) && net_if_is_up(netif)) {
        return LinkON;
    }
    return LinkOFF;
}

EthernetHardwareStatus EthernetClass::hardwareStatus() {
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

void EthernetClass::setMACAddress(const uint8_t *mac_address) {
    if (mac_address != nullptr) {
        NetworkInterface::setMACAddress(mac_address);
    }
}

IPAddress EthernetClass::localIP() {
    return NetworkInterface::localIP();
}

EthernetClass Ethernet;
#endif
