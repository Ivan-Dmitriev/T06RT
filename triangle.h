/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : triangle.h
 * PURPOSE     : Ray tracing project.
 *               Ray tracing module.
 *               Objects shapes module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.08.2021.
 * NOTE        : Module namespace 'bort'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __triangle_h_
#define __triangle_h_

#include "../../def.h"
#include "../rt.h"

/* Project name space */
namespace bort
{
  /* Triangle intersection class */
  class triangle : public shape
  {
  public:
    vec3 N;      // Plane normal
    DBL D;       // Plane coefficient
    vec3 U1, V1; // triangle
    DBL u0, v0;  // Triangle coefficients


    /* Class constructor */
    triangle( VOID ) : N(0, 1, 0), D(1), U1(0), V1(0), u0(0), v0(0)
    {
    } /* End of 'triangle' function */

    /* Class constructor */
    triangle( vec3 P0, vec3 P1, vec3 P2 )
    {
      N = ((P1 - P0) % (P2 - P0)).Normalized();
      vec3 M0 = P0, P;
      D = N[0] * M0[0] + N[1] * M0[1] + N[2] * M0[2];
      P = vec3(D / N[0], D / N[1], D / N[2]);

      vec3 r = P - P0, s1 = P1 - P0, s2 = P2 - P0;
      DBL s12 = s1 & s1, s22 = s2 & s2;

      U1 = ((s1 * s22) - (s2 * (s1 & s2))) / ((s12 * s22) - (s1 & s2) * (s1 & s2));
      u0 = P0 & U1; 

      V1 = ((s2 * s12) - (s1 * (s1 & s2))) / ((s12 * s22) - (s1 & s2) * (s1 & s2));
      v0 = P0 & V1; 
    } /* End of 'triangle' function */

    /* Is intersection exist function.
     * ARGUMENTS:
     *   - ray:
     *       const ray &R;
     * RETURNS: (TRUE) if intersected, FALSE otherwise.
     */
    BOOL IsIntersected( const ray &R ) override
    {
      DBL t = (D - (N & R.Org)) / (N & R.Dir);
      if (t < Threshold)
        return FALSE;

      vec3 P = R(t);
      
      DBL u = (P & U1) - u0;
      DBL v = (P & V1) - v0;
      if (u >= Threshold && u <= 1 && v >= Threshold && v <= 1 && (u + v) <= 1)
        return TRUE;
      return FALSE;
    } // End of 'IsIntersected' function

    /* Find intersection between ray and plane function.
     * ARGUMENTS:
     *   - ray:
     *       const ray &R;
     *   - pointer to intersection type:
     *       intr *Intr;
     * RETURNS:
     *   (BOOL) TRUE if intersection exist, FALSE otherwise.
     */
    BOOL Intersection( const ray &R, intr *Intr ) override
    {
      DBL Treshold = 0.00001; 
      Intr->Shp = this;

      Intr->T = (D - (N & R.Org)) / (N & R.Dir);
      if (Intr->T < Treshold)
        return FALSE;

      vec3 P = R(Intr->T);
      
      DBL u = (P & U1) - u0;
      DBL v = (P & V1) - v0;
      if (u >= Treshold && v >= Treshold && (u + v) <= 1)
      {
        Intr->Shp = this;
        return TRUE;
      }
      return FALSE;
    } /* End of 'Intersection' function */

    /* Get noramal function.
     * ARGUMENTS:
     *   - pointer to intersection results class:
     *       intr *I;
     * RETURNS: None.
     */
    VOID GetNormal( intr *I ) override
    {
      I->N = N;
    } /* End of 'GetNormal' function */
  }; /* End of 'triangle' class */
} /* End of 'bort' namespace */

#endif // __triangle_h_

/* END OF 'triangle.h' FILE */
