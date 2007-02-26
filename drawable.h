//
// C++ Interface: drawable
//
// Description: 
//
//
// Author: Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DRAWABLE_H
#define DRAWABLE_H

/**
	@author Axel Tetzlaff / Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class Drawable{
  public:
    Drawable();

    virtual ~Drawable();

    virtual void drawGL() = 0;
};

#endif
