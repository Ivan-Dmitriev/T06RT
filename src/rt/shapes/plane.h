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
      Intr->T = -(Norm & v + D) / nd;
      //Intr->N = Norm;
      if (Intr->T < 0)
        return FALSE;
      Intr->P = R(Intr->T);

      for (INT i = 0; i < 5; i++)
        Intr->add[i] = 0;

      Intr->add[4] = 1;

      Intr->Shp = this;
      return TRUE;
    } /* End of 'Intersection' function */
    /* Get normal function.
     * ARGUMENTS: 
     *   - intersection point on ray:
     *      intr *Intr;
     * RETURNS: NONE.
     */
    VOID GetNormal( intr *Intr ) override
    {
      Intr->N = Norm;
    } /* End of 'GetNormal' function */
    /* Check if ray intersects object function.
     * ARGUMENTS: 
     *   - input ray:
     *      const ray &R;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    BOOL IsIntersected( const ray &R ) override
    {
      DBL nd = Norm & R.Dir, res = 0;

      if (fabs(nd) < Threshold)
        return FALSE;
      vec3 v = R.Org;
      res = -(Norm & v + D) / nd;
      if (res < 0)
        return FALSE;
      return TRUE;
    } /* End of 'IsIntersected' function */
    /* Find all intersections function.
     * ARGUMENTS:
     *   - ray to find intersections:
     *       const ray &R;
     *   - intersection list (for output):
     *       intr_list &IList;
     * RETURNS:
     *   (INT) number of intersections.
     */
    INT AllIntersect( const ray &R, intr_list &IList )
    {
      DBL Treshold = 1e-4;
      DBL divider = Norm & R.Dir;
      if (COM_ABS(divider) <= Treshold)
        return 0;
      DBL T = (D - (Norm & R.Org)) / divider;
      if (T < Treshold)
        return 0;
      IList.I_list.push_back(intr(this, (D - (Norm & R.Org)) / divider));
      return 1;
    } /* End of 'AllIntersect' function */
  }; /* End of 'plane' class */
} /* end of 'ivrt' namespace */

#endif /* __plane_h_ */

/* END OF 'plane.h' FILE */