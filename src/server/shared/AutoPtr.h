////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _TRINITY_AUTO_PTR_H
#define _TRINITY_AUTO_PTR_H

#include <ace/Bound_Ptr.h>

namespace JadeCore
{
    template <class Pointer, class Lock>
    class AutoPtr : public ACE_Strong_Bound_Ptr<Pointer, Lock>
    {
        public:
            AutoPtr() : ACE_Strong_Bound_Ptr<Pointer, Lock>() {}

            AutoPtr(Pointer* x)
            {
                ACE_Strong_Bound_Ptr<Pointer, Lock>::reset(x);
            }

            operator bool() const
            {
                return ACE_Strong_Bound_Ptr<Pointer, Lock>::get() != NULL;
            }

            bool operator !() const
            {
                return ACE_Strong_Bound_Ptr<Pointer, Lock>::get() == NULL;
            }

            bool operator !=(Pointer* x) const
            {
                return ACE_Strong_Bound_Ptr<Pointer, Lock>::get() != x;
            }
    };
}

#endif
