/*
 * Copyright (c) 2012 cocos2d-x.org
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2008 Radu Gruian
 *
 * Copyright (c) 2011 Vit Valentin
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * Original code by Radu Gruian: http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio.So
 *
 * Adapted to cocos2d-x by Vit Valentin
 *
 * Adapted from cocos2d-x to cocos2d-iphone by Ricardo Quesada
 */


#ifndef __CCACTION_CATMULLROM_H__
#define __CCACTION_CATMULLROM_H__

#include <vector>

#include "CCActionInterval.h"
#include "base_nodes/CCNode.h"
#include "cocoa/CCGeometry.h"

NS_CC_BEGIN;

/**
 * @addtogroup actions
 * @{
 */

/** An Array that contain control points.
 Used by CCCardinalSplineTo and (By) and CCCatmullRomTo (and By) actions.
@ingroup Actions
 */
class CC_DLL CCPointArray : public CCObject
{
public:
    
    /** creates and initializes a Points array with capacity */
    static CCPointArray* create(unsigned int capacity);

    virtual ~CCPointArray();
    CCPointArray();
    
    /** initializes a Catmull Rom config with a capacity hint */
    bool initWithCapacity(unsigned int capacity);
    
    /** appends a control point */
    void addControlPoint(CCPoint controlPoint);
    
    /** inserts a controlPoint at index */
    void insertControlPoint(CCPoint &controlPoint, unsigned int index);
    
    /** replaces an existing controlPoint at index */
    void replaceControlPoint(CCPoint &controlPoint, unsigned int index);
    
    /** get the value of a controlPoint at a given index */
    CCPoint getControlPointAtIndex(unsigned int index);
    
    /** deletes a control point at a given index */
    void removeControlPointAtIndex(unsigned int index);
    
    /** returns the number of objects of the control point array */
    unsigned int count();
    
    /** returns a new copy of the array reversed. User is responsible for releasing this copy */
    CCPointArray* reverse();
    
    /** reverse the current control point array inline, without generating a new one */
    void reverseInline();

    virtual CCObject* copyWithZone(CCZone *zone);
    
    const std::vector<CCPoint*>* getControlPoints();

    void setControlPoints(std::vector<CCPoint*> *controlPoints);
private:
    /** Array that contains the control points */
    std::vector<CCPoint*> *_controlPoints;
};

/** Cardinal Spline path.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
@ingroup Actions
 */
class CC_DLL CCCardinalSplineTo : public CCActionInterval
{
public:

    /** creates an action with a Cardinal Spline array of points and tension */
    static CCCardinalSplineTo* create(float duration, CCPointArray* points, float tension);

    virtual ~CCCardinalSplineTo();
    CCCardinalSplineTo();
    
    /** initializes the action with a duration and an array of points */
    bool initWithDuration(float duration, CCPointArray* points, float tension);
    
    // super virtual functions
	/** returns a new clone of the action */
	virtual CCCardinalSplineTo *clone() const;
    virtual CCCardinalSplineTo* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    virtual CCActionInterval* reverse();
    
    virtual void updatePosition(CCPoint &newPos);
    
    inline CCPointArray* getPoints() { return _points; }
    inline void  setPoints(CCPointArray* points) 
    {
        CC_SAFE_RETAIN(points);
        CC_SAFE_RELEASE(_points);
        _points = points;
    }
    
protected:
    /** Array of control points */
    CCPointArray *_points;
    float _deltaT;
    float _tension;
    CCPoint	_previousPosition;
    CCPoint	_accumulatedDiff;
};

/** Cardinal Spline path.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
 @ingroup Actions
 */
class CC_DLL CCCardinalSplineBy : public CCCardinalSplineTo 
{
public:
    
    /** creates an action with a Cardinal Spline array of points and tension */
    static CCCardinalSplineBy* create(float duration, CCPointArray* points, float tension);

    CCCardinalSplineBy();
    
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCActionInterval* reverse();
    virtual void updatePosition(CCPoint &newPos);

	/** returns a new clone of the action */
	virtual CCCardinalSplineBy *clone() const;

protected:
    CCPoint _startPosition;
};

/** An action that moves the target with a CatmullRom curve to a destination point.
 A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 @ingroup Actions
 */
class CC_DLL CCCatmullRomTo : public CCCardinalSplineTo
{
public:
    
    /** creates an action with a Cardinal Spline array of points and tension */
    static CCCatmullRomTo* create(float dt, CCPointArray* points);

    /** initializes the action with a duration and an array of points */
    bool initWithDuration(float dt, CCPointArray* points);

	/** returns a new clone of the action */
	virtual CCCatmullRomTo *clone() const;
};

/** An action that moves the target with a CatmullRom curve by a certain distance.
 A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 @ingroup Actions
 */
class CC_DLL CCCatmullRomBy : public CCCardinalSplineBy
{
public:
    
    /** creates an action with a Cardinal Spline array of points and tension */
    static CCCatmullRomBy* create(float dt, CCPointArray* points);

    /** initializes the action with a duration and an array of points */
    bool initWithDuration(float dt, CCPointArray* points);

	/** returns a new clone of the action */
	virtual CCCatmullRomBy *clone() const;
};

/** Returns the Cardinal Spline position for a given set of control points, tension and time */
extern CC_DLL CCPoint ccCardinalSplineAt(CCPoint &p0, CCPoint &p1, CCPoint &p2, CCPoint &p3, float tension, float t);

// end of actions group
/// @}

NS_CC_END;

#endif // __CCACTION_CATMULLROM_H__
