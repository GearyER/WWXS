#ifndef __SSDS_ENCRYPTION
#define __SSDS_ENCRYPTION

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"
#include "mbedtls/aes.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/pk.h"

esp_err_t init_encryption(unsigned char* rsa_pub, uint16_t key_len);
esp_err_t end_encryption();
esp_err_t encrypt_block(unsigned char* input, unsigned char* output, uint16_t buf_len);
esp_err_t decrypt_block(unsigned char* input, unsigned char* output, uint16_t buf_len);

int get_encrypted_key_length();
uint8_t* get_encrypted_key();
unsigned char* get_aes_iv();


/* The following comment and function prototype were found here, as is,
 * incomplete, with no implementation or indication of what it's
 * supposed to be used for. I'm leaving it for future investigation
 * but it's unclear how much we actually need it, so the prototype
 * is commented out.
 */

/* Return an encrypted version of the AES key, using the public key
 * provided in init_encryption
 * Requires a 256 byte input array to store the result of this
 * operation in
 */
// esp_err_t encrypt_aes ();

#endif // __SSDS_ENCRYPTION
