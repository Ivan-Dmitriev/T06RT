/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : point.cpp
 * PURPOSE     : Raytracing project.
 *               Point light declaration class.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 02.08.2021.
 * NOTE        : Module namespace 'ivrt'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __point_h_
#define __point_h_

#include "light.h"

/* Project namespace */
namespace ivrt
{
  class point : public light
  {
  private:
    vec3 LgtPos, LgtColor;
    DBL R2, R1, Cr;
  public:  
    /* Create point light function.
     * ARGUMENTS: 
     *   - light color and position:
     *      vec3 NLgtPos, NLgtColor;
     *   - light radiuses:
     *      DBL NR1, NR2;
     * RETURNS: NONE.
     */
    point( vec3 NLgtPos, vec3 NLgtColor, DBL NR1, DBL NR2 ) : 
      LgtPos(NLgtPos), LgtColor(NLgtColor), R1(NR1), R2(NR2), Cr(R2 - R1)
    {
    } /* End of 'point' function */

    /* Get attenuation factor function.
     * ARGUMENTS: 
     *   - input ray:
     *      const ray &R;
     *   - information about light:
     *      light_info *L;
     * RETURNS: (DBL) result value.
     */
    DBL Shadow( const vec3 &P, light_info *L ) override
    {
      vec3 Direction = (LgtPos - P).Normalizing();
      DBL Dist = LgtPos.Distance(P), att = 1;// att = 1 / (Cq * Dist * Dist + Cl * Dist + Cc);

      if (Dist > R1 && Dist <= R2)
        att -= (Dist - R1) / Cr;
      else if (Dist > R2)
        att = 0;

      L->L = Direction;
      L->Color = LgtColor;
      L->Dist = Dist;
      return mth::Min<DBL>(att, 1);
      //return mth::Min(1 / (Cc + Cl * dl + Cq * dl2 * dl2), 1.0)
    } /* End of 'Shadow' function */

  }; /* End of 'point' class */

} /* end of 'ivrt' namespace */

#endif /* __point_h_ */

/* END OF 'point.h' FILE */