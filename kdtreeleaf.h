//
// C++ Interface: kdtreeleaf
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KDTREELEAF_H
#define KDTREELEAF_H

#include <kdtree.h>
#include <QList>
class Geometry;
/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class KdTreeLeaf : public KdTree
{
public:
    KdTreeLeaf();

    ~KdTreeLeaf();
private:
    QList<Geometry*> primitives;
};

#endif
