////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _AUTH_SARC4_H
#define _AUTH_SARC4_H

#include "Define.h"
#include <openssl/evp.h>

class ARC4
{
    public:
        ARC4(uint8 len);
        ARC4(uint8 *seed, uint8 len);
        ~ARC4();
        void Init(uint8 *seed);
        void UpdateData(int len, uint8 *data);
    private:
        EVP_CIPHER_CTX m_ctx;
};

#endif
