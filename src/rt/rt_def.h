/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : rt_def.h
 * PURPOSE     : Raytracing project.
 *               Raytracing class declaration module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev
 * LAST UPDATE : 02.08.2021
 * NOTE        : Module namespace 'ivrt'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __rt_def_h_
#define __rt_def_h_

#include "../def.h"

/* Project namespace */
namespace ivrt
{
  const static DBL Threshold = 0.0001;
  class shape;

  /* Common intersection class */
  class intr
  {
  public:
    DBL T;
    shape *Shp;
    vec3 Color;
    vec3 N;
    BOOL IsPos;
    BOOL IsNorm;
    vec3 P;
    INT I[5];
    DBL D[5];

    /* Intr class constructor */
    intr( VOID ) : IsNorm(FALSE), IsPos(FALSE)
    {
    }

  }; /* End of 'intr' class */

  /* Shape class */
  class shape
  {
  public:
    /* Find intersection function.
     * ARGUMENTS: 
     *   - ray:
     *      const ray &R;
     *   - intersection point on ray:
     *      intr *Intr;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    virtual BOOL Intersection( const ray &R, intr *Intr )
    {
      return TRUE;
    } /* End of 'Intersection' function */
    /* Check if ray intersects object function.
     * ARGUMENTS: 
     *   - input ray:
     *      const ray &R;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    virtual BOOL IsIntersected( const ray &R )
    {
      return TRUE;
    } /* End of 'IsIntersected' function */

    /* Get normal function.
     * ARGUMENTS: 
     *   - intersection point on ray:
     *      intr *Intr;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    virtual VOID GetNormal( intr *Intr )
    {
    }
  }; /* End of 'shape' class */

  /* Scene class */
  class scene
  {
  private:
    std::vector<shape *> Shapes;
    vec3 AmbientColor, Background;
    INT RecLevel, MaxRecLevel;
 
  public:
    /* Scene destructor */
    ~scene( VOID )
    {
      for (auto p : Shapes)
      {
        delete p;
      } 
      Shapes.clear();
    }

    /* Find intersection function.
     * ARGUMENTS: 
     *   - ray:
     *      const ray &R;
     *   - intersection point on ray:
     *      intr *Intr;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    BOOL Intersection( const ray &R, intr *Intr );

   /* Add unit of scene to stock function.
    * ARGUMENTS: 
    *   - Shape to be add:
    *       shape *NewShape;
    * RETURNS: (scene & ) link on result scene.
    */
    scene & operator<<( shape *NewShape )
    {
      Shapes.push_back(NewShape);

      return *this;
    } /* End of 'operator<<' function */
    
    /*
    vec3 Trace( const ray &R, const evni &Media, DBL Weight )
    {
      color = Background;
      if (RecLevel < MaxRecLevel)
      {
        RecLevel++;
        color = Shade(R.Dir, Media, &intersection, Weight);
        //color *= exp(-intersection.T * Media.DecayCoef);
        RecLevel--;
      }
    }
    */
  }; /* End of 'scene' class */
} /* end of 'ivrt' namespace */

#endif /* __rt_def_h_ */

/* END OF 'rt_def.h' FILE */