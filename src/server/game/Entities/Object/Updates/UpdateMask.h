////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __UPDATEMASK_H
#define __UPDATEMASK_H

#include "UpdateFields.h"
#include "Errors.h"
#include "ByteBuffer.h"

class UpdateMask
{
    public:
        /// Type representing how client reads update mask
        typedef uint32 ClientUpdateMaskType;

        enum UpdateMaskCount
        {
            CLIENT_UPDATE_MASK_BITS = sizeof(ClientUpdateMaskType) * 8,
        };

        UpdateMask() : _fieldCount(0), _blockCount(0), _bits(nullptr) { }

        UpdateMask(UpdateMask const& right) : _bits(nullptr)
        {
            SetCount(right.GetCount());
            if (right._bits)
                memcpy(_bits, right._bits, sizeof (uint8) * _blockCount * 32);
        }

        ~UpdateMask()
        {
            if (_bits != nullptr)
            {
                delete[] _bits;
                _bits = nullptr;
            }
        }

        void SetBit(uint32 index) { _bits[index] = 1; }
        void UnsetBit(uint32 index) { _bits[index] = 0; }
        bool GetBit(uint32 index) const { return _bits[index] != 0; }

        void AppendToPacket(ByteBuffer* data)
        {
            for (uint32 i = 0; i < GetBlockCount(); ++i)
            {
                ClientUpdateMaskType maskPart = 0;
                for (uint32 j = 0; j < CLIENT_UPDATE_MASK_BITS; ++j)
                {
                    if (_bits[CLIENT_UPDATE_MASK_BITS * i + j])
                        maskPart |= 1 << j;
                }

                *data << maskPart;
            }
        }

        uint32 GetBlockCount() const { return _blockCount; }
        uint32 GetCount() const { return _fieldCount; }

        void SetCount(uint32 valuesCount)
        {
            if (_bits != nullptr)
            {
                delete[] _bits;
                _bits = nullptr;
            }

            _fieldCount = valuesCount;
            _blockCount = (valuesCount + CLIENT_UPDATE_MASK_BITS - 1) / CLIENT_UPDATE_MASK_BITS;

            if (!valuesCount)
                return;

            _bits = new uint8[_blockCount * CLIENT_UPDATE_MASK_BITS];
            memset(_bits, 0, sizeof (uint8) * _blockCount * CLIENT_UPDATE_MASK_BITS);
        }

        void AddBlock()
        {
            uint8* curr = _bits;
            _fieldCount += CLIENT_UPDATE_MASK_BITS;
            ++_blockCount;

            _bits = new uint8[_blockCount * CLIENT_UPDATE_MASK_BITS];
            memset(&_bits[(_blockCount - 1) * CLIENT_UPDATE_MASK_BITS], 0, sizeof (uint8) * CLIENT_UPDATE_MASK_BITS);
            if (curr)
            {
                memcpy(_bits, curr, sizeof (uint8) * (_blockCount - 1) * CLIENT_UPDATE_MASK_BITS);
                delete[] curr;
            }
        }

        void Clear()
        {
            if (_bits)
                memset(_bits, 0, sizeof (uint8) * _blockCount * CLIENT_UPDATE_MASK_BITS);
        }

        UpdateMask& operator=(UpdateMask const& right)
        {
            if (this == &right)
                return *this;

            SetCount(right.GetCount());
            memcpy(_bits, right._bits, sizeof (uint8) * _blockCount * CLIENT_UPDATE_MASK_BITS);
            return *this;
        }

        UpdateMask& operator&=(UpdateMask const& right)
        {
            ASSERT(right.GetCount() <= GetCount());
            for (uint32 i = 0; i < _fieldCount; ++i)
                _bits[i] &= right._bits[i];

            return *this;
        }

        UpdateMask& operator|=(UpdateMask const& right)
        {
            ASSERT(right.GetCount() <= GetCount());
            for (uint32 i = 0; i < _fieldCount; ++i)
                _bits[i] |= right._bits[i];

            return *this;
        }

        UpdateMask operator|(UpdateMask const& right)
        {
            UpdateMask ret(*this);
            ret |= right;
            return ret;
        }

    private:
        uint32 _fieldCount;
        uint32 _blockCount;
        uint8* _bits;
};

#endif

