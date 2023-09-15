////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _AUTH_BIGNUMBER_H
#define _AUTH_BIGNUMBER_H

#include "Define.h"
#include <ace/Mutex.h>

struct bignum_st;

class BigNumber
{
    public:
        BigNumber();
        BigNumber(const BigNumber &bn);
        BigNumber(uint32);
        ~BigNumber();

        void SetDword(uint32);
        void SetQword(uint64);
        void SetBinary(const uint8 *bytes, int len);
        void SetHexStr(const char *str);

        void SetRand(int numbits);

        BigNumber& operator=(const BigNumber &bn);

        BigNumber operator+=(const BigNumber &bn);
        BigNumber operator+(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t += bn;
        }
        BigNumber operator-=(const BigNumber &bn);
        BigNumber operator-(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t -= bn;
        }
        BigNumber operator*=(const BigNumber &bn);
        BigNumber operator*(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t *= bn;
        }
        BigNumber operator/=(const BigNumber &bn);
        BigNumber operator/(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t /= bn;
        }
        BigNumber operator%=(const BigNumber &bn);
        BigNumber operator%(const BigNumber &bn)
        {
            BigNumber t(*this);
            return t %= bn;
        }

        bool isZero() const;

        BigNumber ModExp(const BigNumber &bn1, const BigNumber &bn2);
        BigNumber Exp(const BigNumber &);

        int GetNumBytes(void);

        struct bignum_st *BN() { return _bn; }

        uint32 AsDword();
        uint8* AsByteArray(int minSize = 0, bool reverse = true);

        const char *AsHexStr();
        const char *AsDecStr();

    private:
        struct bignum_st *_bn;
        uint8 *_array;

        // This mutex only controls thread-safe access to AsByteArray() and should be replaced with a thread-safe implementation of BigNumber
        ACE_Mutex _lock;

};
#endif

