////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_TYPECONTAINERVISITOR_H
#define TRINITY_TYPECONTAINERVISITOR_H

/*
 * @class TypeContainerVisitor is implemented as a visitor pattern.  It is
 * a visitor to the TypeContainerList or TypeContainerMapList.  The visitor has
 * to overload its types as a visit method is called.
 */

#include "Define.h"
#include "Dynamic/TypeContainer.h"

// forward declaration
template<class T, class Y> class TypeContainerVisitor;

// visitor helper
template<class VISITOR, class TYPE_CONTAINER> void VisitorHelper(VISITOR &v, TYPE_CONTAINER &c)
{
    v.Visit(c);
}

// terminate condition for container list
template<class VISITOR> void VisitorHelper(VISITOR &/*v*/, ContainerList<TypeNull> &/*c*/)
{
}

template<class VISITOR, class T> void VisitorHelper(VISITOR &v, ContainerList<T> &c)
{
    v.Visit(c._element);
}

// recursion for container list
template<class VISITOR, class H, class T> void VisitorHelper(VISITOR &v, ContainerList<TypeList<H, T> > &c)
{
    VisitorHelper(v, c._elements);
    VisitorHelper(v, c._TailElements);
}

// terminate condition container map list
template<class VISITOR> void VisitorHelper(VISITOR &/*v*/, ContainerMapList<TypeNull> &/*c*/)
{
}

template<class VISITOR, class T> void VisitorHelper(VISITOR &v, ContainerMapList<T> &c)
{
    v.Visit(c._element);
}

// recursion container map list
template<class VISITOR, class H, class T> void VisitorHelper(VISITOR &v, ContainerMapList<TypeList<H, T> > &c)
{
    VisitorHelper(v, c._elements);
    VisitorHelper(v, c._TailElements);
}

// array list
template<class VISITOR, class T> void VisitorHelper(VISITOR &v, ContainerArrayList<T> &c)
{
    v.Visit(c._element);
}

template<class VISITOR> void VisitorHelper(VISITOR &/*v*/, ContainerArrayList<TypeNull> &/*c*/)
{
}

// recursion
template<class VISITOR, class H, class T> void VisitorHelper(VISITOR &v, ContainerArrayList<TypeList<H, T> > &c)
{
    VisitorHelper(v, c._elements);
    VisitorHelper(v, c._TailElements);
}

// for TypeMapContainer
template<class VISITOR, class OBJECT_TYPES> void VisitorHelper(VISITOR &v, TypeMapContainer<OBJECT_TYPES> &c)
{
    VisitorHelper(v, c.GetElements());
}

template<class VISITOR, class TYPE_CONTAINER>
class TypeContainerVisitor
{
    public:
        TypeContainerVisitor(VISITOR &v) : i_visitor(v) {}

        void Visit(TYPE_CONTAINER &c)
        {
            VisitorHelper(i_visitor, c);
        }

        void Visit(const TYPE_CONTAINER &c) const
        {
            VisitorHelper(i_visitor, c);
        }

    private:
        VISITOR &i_visitor;
};
#endif

