#include "ssds_global.h"

/* Encryption is done on a seperate thread from other processes, primarily taking in unencrypted
 * datapoints from a datapoint iterator, encrypting the data, then pushing it into our ringbuffer.
 * This data is later pulled from the ringbuffer in ssds_bluetooth for transmission.
 */

#define ENC_TAG "ENCRY"

/* Storing initial IV for use in decryption testing */
uint8_t decrypt_iv[16];

/* Temporary workaround for the app, keeping the IV constant over all encryption */
uint8_t const_iv[16];

/* Encryption engine variables */
static mbedtls_ctr_drbg_context ctr_drbg;
static mbedtls_entropy_context entropy;

/* AES */
#define ENCRYPTED_KEY_LEN 256

static esp_aes_context ctx;
static char *seed_str = "SSDS floating weather station!";

unsigned char aes_key[32];
unsigned char aes_iv[16];
uint8_t encrypted_key[ENCRYPTED_KEY_LEN];

// Initialized as a copy of the initial IV, but modified as encryption occurs.
static unsigned char aes_curr_iv[16];

/* RSA */
static mbedtls_pk_context pk;

static uint8_t encryption_active = 0;

esp_err_t encrypt_block(unsigned char* input, unsigned char* output, uint16_t buf_len) {	
	int encrypt_length = 16;
	uint8_t iv[16];
	memcpy(iv, const_iv, 16);
  
	/* This is a temporary workaround for the IV apparently not updating on the app, eventually remove please */
	//int ret = esp_aes_crypt_cbc ( &ctx, ESP_AES_ENCRYPT, encrypt_length, aes_curr_iv, input, output );
	int ret = esp_aes_crypt_cbc ( &ctx, ESP_AES_ENCRYPT, encrypt_length, iv, input, output );
  
	if (ret){
		ESP_LOGE(ENC_TAG,"AES Encryption error");
		return ESP_FAIL;
	} else {
		return ESP_OK;
	}
}

esp_err_t decrypt_block(unsigned char* input, unsigned char* output, uint16_t buf_len) {
	uint16_t encrypt_length = buf_len + (16 - (buf_len%16));
	encrypt_length = 16;
	uint8_t iv[16];
	memcpy(iv, const_iv, 16);
  
	//int ret = esp_aes_crypt_cbc ( &ctx, ESP_AES_DECRYPT, encrypt_length, decrypt_iv, input, output );
	int ret = esp_aes_crypt_cbc ( &ctx, ESP_AES_DECRYPT, encrypt_length, iv, input, output );
  
  
	if (ret){
		ESP_LOGE(ENC_TAG,"AES Decryption error");
		return ESP_FAIL;
	}
	else{
		return ESP_OK;
	}
}

esp_err_t init_encryption(unsigned char* rsa_pub, uint16_t key_len) {
	ESP_LOGI(ENC_TAG, "Encryption initializing");
	int ret = 0;
	size_t olen = 0;
  
	if (encryption_active) {
		ESP_LOGW(ENC_TAG, "Attempt to double initialize encryption");
		end_encryption();
		
	}
  
	/* Initializing random number generators */
	mbedtls_entropy_init( &entropy );
	mbedtls_ctr_drbg_init( &ctr_drbg );

	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (unsigned char*)seed_str, strlen(seed_str));
  
	if(ret != 0) {
		ESP_LOGE(ENC_TAG, "mbedtls_ctr_drbg_init returned -0x%04x\n", -ret );
		return ESP_FAIL;
	}
  
  
	/* Generating the 32 byte AES key */
	ret = mbedtls_ctr_drbg_random(&ctr_drbg, aes_key, 32);
	if(ret != 0) {
		ESP_LOGE(ENC_TAG, "mbedtls_ctr_drbg_random returned -0x%04x\n", -ret );
		return ESP_FAIL;
	}
  
	/* Generating the 16 byte IV */
	ret = mbedtls_ctr_drbg_random(&ctr_drbg, aes_iv, 16);
	if(ret != 0) {
		ESP_LOGE(ENC_TAG, "mbedtls_ctr_drbg_random returned -0x%04x\n", -ret );
		return ESP_FAIL;
	}

	/* Copying the initial IV for use */
	memcpy(aes_curr_iv, aes_iv, 16);
  
	/* Copying the initial IV for use in decryption if we want later */
	memcpy(decrypt_iv, aes_iv, 16);
  
	memcpy(const_iv, aes_iv, 16);
  
	ESP_LOGE(ENC_TAG, "AES key and IV generated");
  
	esp_aes_init ( &ctx );
  
	esp_err_t err = esp_aes_setkey(&ctx, aes_key, (unsigned)256);
  
	ESP_ERROR_CHECK(err);
  
	/* Loading public key */
	mbedtls_pk_init (&pk);
	ret = mbedtls_pk_parse_public_key(&pk, rsa_pub, key_len);
	if(ret != 0) {
		ESP_LOGE(ENC_TAG, "mbedtls_pk_parse_public_keyfile returned -0x%04x\n", -ret );
		return ESP_FAIL;
	}
  
	/* Encrypting AES key */
	ret = mbedtls_pk_encrypt(&pk, (unsigned char*)aes_key, 32, encrypted_key, 
	                         &olen, 256, mbedtls_ctr_drbg_random, &ctr_drbg);
	
	if(ret != 0) {
		ESP_LOGE(ENC_TAG, "mbedtls_pk_encrypt returned -0x%04x\n", -ret );
		return ESP_FAIL;
	}

	encryption_active = 1;
	return ESP_OK;
}


esp_err_t end_encryption() {
	if (!encryption_active) {
		ESP_LOGW(ENC_TAG, "Encryption never initialized, cannot end");
		return ESP_FAIL;
	}
  
	esp_aes_free ( &ctx );
  
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	mbedtls_pk_free(&pk);
  
	encryption_active = 0;
	return ESP_OK;
}

int get_encrypted_key_length() {
	return ENCRYPTED_KEY_LEN;
}

uint8_t* get_encrypted_key() {
	return encrypted_key;
}

unsigned char* get_aes_iv() {
	return aes_iv;
}
