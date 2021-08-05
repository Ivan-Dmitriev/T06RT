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
    DBL Radius, Radius2;

  public:
    sphere( vec3 C, DBL R, surface NS ) : Center(C), Radius(R), Radius2(R * R)
    {
      this->mtl = NS;
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
        Intr->P = R(Intr->T);
        Intr->IsPos = TRUE;

        return TRUE;
      }
      if (OK < Threshold || h2 < Threshold)
        return FALSE;
      for (INT i = 0; i < 5; i++)
        Intr->add[i] = 0;

      Intr->T = OK - sqrt(h2);
      Intr->P = R(Intr->T);
      Intr->IsPos = TRUE;

      Intr->Shp = this;
      //Intr->N = (R(Intr->T) - Center).Normalizing();
      return TRUE;
    } /* End of 'SphereInter' function */
 
    /* Get normal function.
     * ARGUMENTS: 
     *   - intersection point on ray:
     *      intr *Intr;
     * RETURNS: NONE.
     */
    VOID GetNormal( intr *Intr ) override
    {
      Intr->N = (Intr->P - Center).Normalizing();
    } /* End of 'GetNormal' function */

    /* Check if ray intersects object function.
     * ARGUMENTS: 
     *   - input ray:
     *      const ray &R;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    BOOL IsIntersected( const ray &R ) override
    {
      vec3 a = Center - R.Org;
      FLT OC2, OK, OK2, R2, h2;

      OC2 = a & a;
      OK = a & R.Dir;
      OK2 = (a & R.Dir) * (a & R.Dir);
      R2 = Radius * Radius;
      h2 = R2 - (OC2 - OK2);
      if (OC2 < R2)
        return TRUE;
      if (OK < Threshold || h2 < Threshold)
        return FALSE;
      return TRUE;
    } /* End of 'IsIntersected' function */

    /* Check if point is inside of the shape.
     * ARGUMENTS:
     *   - Reference ray to intersect:
     *         ray &R;
     *   - Reference to stock of intersections:
     *         intr_list &IList;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    BOOL IsInside( const vec3 &P ) override
    {
      return (P.Distance2(Center) < Radius2);
    } /* End of 'IsInside' function */
  }; /* End of 'sphere' class */
} /* end of 'ivrt' namespace */

#endif /* __sphere_h_ */

/* END OF 'sphere.h' FILE */