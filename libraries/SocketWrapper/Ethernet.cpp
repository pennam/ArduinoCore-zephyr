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
    setMACAddress(mac);

    if (!NetworkInterface::setLocalIPFull(ip, subnet, gateway)) {
        return 0;
    }

    if (!net_if_is_up(netif)) {
        net_if_up(netif);
    }

    return 1;
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

int EthernetClass::disconnect() {
    return NetworkInterface::disconnect();
}

void EthernetClass::end() {
    disconnect();
}

void EthernetClass::setMACAddress(const uint8_t *mac_address) {
    if (mac_address != nullptr) {
        NetworkInterface::setMACAddress(mac_address);
    }
}

void EthernetClass::MACAddress(uint8_t *mac_address) {
    setMACAddress(mac_address);
}

IPAddress EthernetClass::localIP() {
    return NetworkInterface::localIP();
}

IPAddress EthernetClass::subnetMask() {
    return NetworkInterface::subnetMask();
}

IPAddress EthernetClass::gatewayIP() {
    return NetworkInterface::gatewayIP();
}

IPAddress EthernetClass::dnsServerIP() {
    return NetworkInterface::dnsServerIP();
}

void EthernetClass::setLocalIP(const IPAddress local_ip) {
    NetworkInterface::setLocalIP(local_ip);
}

void EthernetClass::setSubnetMask(const IPAddress subnet) {
    NetworkInterface::setSubnetMask(subnet);
}

void EthernetClass::setGatewayIP(const IPAddress gateway) {
    NetworkInterface::setGatewayIP(gateway);
}

void EthernetClass::setDnsServerIP(const IPAddress dns_server) {
    NetworkInterface::setDnsServerIP(dns_server);
}

EthernetClass Ethernet;
#endif
