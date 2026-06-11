#include "Arduino.h"
#include <ZephyrSSLClient.h>
#include "mbedtls/memory_buffer_alloc.h"

static unsigned char arduino_mbedtls_heap[CONFIG_MBEDTLS_HEAP_SIZE];

static int arduino_mbedtls_init() {
    printk("allocating mbedtls budffer\n\r");
    mbedtls_memory_buffer_alloc_init(arduino_mbedtls_heap, sizeof(arduino_mbedtls_heap));

#if defined(CONFIG_MBEDTLS_DEBUG_LEVEL)
	mbedtls_debug_set_threshold(CONFIG_MBEDTLS_DEBUG_LEVEL);
#endif

#if defined(CONFIG_MBEDTLS_PSA_CRYPTO_CLIENT)
	if (psa_crypto_init() != PSA_SUCCESS) {
		return -EIO;
	}
#endif

	return 0;
}

ZephyrSSLClient::ZephyrSSLClient() {
    static int mbedtls_init = arduino_mbedtls_init();
}

ZephyrSSLClient::~ZephyrSSLClient() {
}