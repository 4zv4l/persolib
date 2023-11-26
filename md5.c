#include "md5.h"

i32
md5_digest_from_hex(const unsigned char digest_hex[static 32], unsigned char buff[static 16])
{
    for (int i = 0; i < 16; i++)
    {
        sscanf((char*)digest_hex, "%2hhx", &buff[i]);
        digest_hex += 2;
    }

    return 1;
}

void
md5_digest_hex(const unsigned char digest[static 16], unsigned char buff[static 32])
{
    for (int i = 0; i < 16; i++)
        buff += sprintf((char *)buff, "%02x", digest[i]);
}

void
md5_digest(const unsigned char digest[static 16])
{
    for (int i = 0; i < 16; i++)
        printf("%02x", digest[i]);
    printf("\n");
}

void
md5_digest_ex(const unsigned char digest[static 16], const char str[static 1])
{
    for (int i = 0; i < 16; i++)
        printf("%02x", digest[i]);
    printf("  %s\n", str);
}

bool
md5_expect(const char text[static 1], const unsigned char expected[static 16])
{
    unsigned char digest[MD5_DIGEST_LEN+1] = {0};

    md5_from_str(text, digest);
    if (memcmp(digest, expected, MD5_DIGEST_LEN) != 0)
        return false;
    return true;
}

bool
md5_expect_hex(const char text[static 1], const unsigned char expected_hex[static 32])
{
    unsigned char digest[MD5_DIGEST_LEN+1]       = {0};
    unsigned char expected[MD5_DIGEST_LEN+1]     = {0};

    md5_digest_from_hex(expected_hex, expected);
    md5_from_str(text, digest);

    if (memcmp(digest, expected, MD5_DIGEST_LEN) != 0)
        return false;
    return true;
}

i32
md5_from_file(const char filename[static 1], unsigned char digest[static 16])
{
    FILE *file = fopen(filename, "rb");
    if (!file)
        return fprintf(stderr, "%s: %s\n", filename, strerror(errno)), 0;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (EVP_DigestInit_ex(ctx, EVP_md5(), NULL) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    char buff[1024] = {0};
    int length = 0;
    while((length = fread(buff, 1, 1024, file)) > 0)
    {
        if (EVP_DigestUpdate(ctx, buff, length) == 0)
            return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;
    }

    if (EVP_DigestFinal_ex(ctx, digest, NULL) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    fclose(file);
    EVP_MD_CTX_free(ctx);

    return 1;
}

i32
md5_from_str(const char str[static 1], unsigned char digest[static 16])
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (EVP_DigestInit_ex(ctx, EVP_md5(), NULL) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    if (EVP_DigestUpdate(ctx, str, strlen(str)) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    if (EVP_DigestFinal_ex(ctx, digest, NULL) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    EVP_MD_CTX_free(ctx);

    return 1;
}

string
md5_from_str_hex(const char str[static 1])
{
    unsigned char digest[MD5_DIGEST_LEN] = {0};
    string hex_digest = malloc((MD5_DIGEST_LEN*2)+1);
    if (!hex_digest)
        return fprintf(stderr, "couldnt alloc hex_digest\n"), (string)0;

    if (!md5_from_str(str, digest))
        return fprintf(stderr, "couldnt get md5 digest for %s\n", str), (string)0;

    md5_digest_hex(digest, (unsigned char*)hex_digest);
    return hex_digest;
}
