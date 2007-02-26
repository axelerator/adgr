//
// C++ Interface: kdtreenode
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KDTREENODE_H
#define KDTREENODE_H

#include <kdtree.h>

/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class KdTreeNode : public KdTree
{
public:
    KdTreeNode();

    ~KdTreeNode();

private:
    KdTree *left;
    KdTree *right;
};

#endif
