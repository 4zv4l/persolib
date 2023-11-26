#ifndef MD5
#define MD5
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "types.h"

#define MD5_DIGEST_LEN 16

/*
 * convert digest from hex and store it in buff (must be at least 16 char long)
 */
i32 md5_digest_from_hex(const unsigned char digest[static 32], unsigned char buff[static 16]);
/*
 * convert digest to hex and store it in buff (must be at least 32 char long)
 */
void md5_digest_hex(const unsigned char digest[static 16], unsigned char buff[static 32]);
/*
 * show the md5 digest in hex
 */
void md5_digest(const unsigned char digest[static 16]);
/*
 * show the md5 digest in hex followed by the string
 */
void md5_digest_ex(const unsigned char digest[static 16], const char str[static 1]);
/*
 * return if digest for string match or not
 */
bool md5_expect(const char text[static 1], const unsigned char expected[static 16]);
/*
 * return if digest (in hex format) for string match or not
 */
bool md5_expect_hex(const char text[static 1], const unsigned char expected_hex[static 32]);
/*
 * compute md5 from given filename
 * return 1 if ok
 * print error to stderr and return error code otherwise
 */
i32 md5_from_file(const char filename[static 1], unsigned char[static 16]);
/*
 * compute md5 of string (MUST be 0 terminated)
 * return 1 if ok
 * print error to stderr and return error code otherwise
 */
i32 md5_from_str(const char str[static 1], unsigned char[static 16]);
/*
 * compute md5 of string (MUST be 0 terminated)
 * return hex digest if ok
 * print error to stderr and return error code otherwise
 */
string md5_from_str_hex(const char str[static 1]);
#endif
