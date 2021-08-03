/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Suppoplane Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : plane.h
 * PURPOSE     : Raytracing project.
 *               Plane class declaration module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev
 * LAST UPDATE : 02.08.2021
 * NOTE        : Module namespace 'ivrt'.
 * 
 * No paplane of this file may be changed without agreement of
 * Computer Graphics Suppoplane Group of 30 Phys-Math Lyceum
 */

#ifndef __plane_h_
#define __plane_h_

#include "../rt_def.h"

/* Project namespace */
namespace ivrt
{
  class plane : public shape
  {
  private:
    vec3 Norm;
    DBL D;
  public:
    plane( vec3 NewN, DBL NewD ) : Norm(NewN.Normalizing()), D(NewD) 
    {
    }
    /* Find intersection on plane function.
      * ARGUMENTS: 
      *   - ray:
      *      const ray &R;
      *   - intersection point on ray:
      *      intr *Intr;
      * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
      */
    BOOL Intersection( const ray &R, intr *Intr ) override
    {
      DBL nd = Norm & R.Dir;

      if (fabs(nd) < Threshold)
        return FALSE;
      vec3 v = R.Org;
      Intr->T = -(Norm & v + D) / (Norm & R.Dir); 
      Intr->N = Norm;
      if (Intr->T < 0)
        return FALSE;
      Intr->Shp = this;
      return TRUE;
    } /* End of 'Intersection' function */

  };
} /* end of 'ivrt' namespace */

#endif /* __plane_h_ */

/* END OF 'plane.h' FILE */