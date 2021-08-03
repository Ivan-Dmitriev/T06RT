/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Supposphere Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : sphere.h
 * PURPOSE     : Raytracing project.
 *               Sphere class declaration module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev
 * LAST UPDATE : 02.08.2021
 * NOTE        : Module namespace 'ivrt'.
 * 
 * No pasphere of this file may be changed without agreement of
 * Computer Graphics Supposphere Group of 30 Phys-Math Lyceum
 */

#ifndef __sphere_h_
#define __sphere_h_

#include "../rt_def.h"

/* Project namespace */
namespace ivrt
{
  class sphere : public shape
  {
  private:
    vec3 Center;
    DBL Radius;
  public:
    sphere( vec3 C, DBL R ) : Center(C), Radius(R) 
    {
    }
    /* Find intersection on sphere function.
      * ARGUMENTS: 
      *   - ray:
      *      const ray &R;
      *   - intersection point on ray:
      *      intr *Intr;
      * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
      */
    BOOL Intersection( const ray &R, intr *Intr ) override
    {
      vec3 a = Center - R.Org;
      FLT OC2, OK, OK2, R2, h2;

      OC2 = a & a;
      OK = a & R.Dir;
      OK2 = (a & R.Dir) * (a & R.Dir);
      R2 = Radius * Radius;
      h2 = R2 - (OC2 - OK2);
      if (OC2 < R2)
      {
        Intr->Shp = this;
        Intr->T = OK + sqrt(h2);
        Intr->N = (R(Intr->T) - Center).Normalizing();
        return TRUE;
      }
      if (OK < Threshold || h2 < Threshold)
        return FALSE;
      Intr->T = OK - sqrt(h2);
      Intr->Shp = this;
      Intr->N = (R(Intr->T) - Center).Normalizing();
      return TRUE;
    } /* End of 'SphereInter' function */

  };
} /* end of 'ivrt' namespace */

#endif /* __sphere_h_ */

/* END OF 'sphere.h' FILE */