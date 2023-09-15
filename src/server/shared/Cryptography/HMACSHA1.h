////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _AUTH_HMAC_H
#define _AUTH_HMAC_H

#include "Define.h"
#include <string>
#include <openssl/hmac.h>
#include <openssl/sha.h>

class BigNumber;

#define SEED_KEY_SIZE 16

class HmacHash
{
    public:
        HmacHash(uint32 len, uint8 *seed);
        ~HmacHash();
        void UpdateData(const std::string &str);
        void UpdateData(const uint8* data, size_t len);
        void Finalize();
        uint8 *ComputeHash(BigNumber* bn);
        uint8 *GetDigest() { return (uint8*)m_digest; }
        int GetLength() const { return SHA_DIGEST_LENGTH; }
    private:
        HMAC_CTX m_ctx;
        uint8 m_digest[SHA_DIGEST_LENGTH];
};

class HmacHash256
{
    public:
    HmacHash256(uint32 len, uint8 *seed);
    ~HmacHash256();
    void UpdateData(const std::string &str);
    void UpdateData(const uint8* data, size_t len);
    void Finalize();
    uint8 *ComputeHash(BigNumber* bn);
    uint8 *ComputeHash(uint8 *seed, uint32 len);
    uint8 *GetDigest() {
        return (uint8*)m_digest;
    }
    int GetLength() const {
        return SHA256_DIGEST_LENGTH;
    }
    private:
    HMAC_CTX m_ctx;
    uint8 m_digest[SHA256_DIGEST_LENGTH];
};

#endif

