/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Suppobox Group of 30 Phys-mth Lyceum
 *************************************************************/

/* FILE NAME   : box.h
 * PURPOSE     : Raytracing project.
 *               Box class declaration module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev
 * LAST UPDATE : 02.08.2021
 * NOTE        : Module namespace 'ivrt'.
 * 
 * No pabox of this file may be changed without agreement of
 * Computer Graphics Suppobox Group of 30 Phys-mth Lyceum
 */

#ifndef __box_h_
#define __box_h_

#include "../rt_def.h"

#define THRESHOLD 0.0001

/* Project namespace */
namespace ivrt
{
  /* Box class */
  class box : public shape
  {
  private:
    vec3 Min, Max; // Maximum and minimum box boreders
  public:
    box( vec3 NewMin, vec3 NewMax ) : Min(NewMin), Max(NewMax) 
    {
    }
     /* Find intersection on box function.
      * ARGUMENTS: 
      *   - input ray:
      *      const ray &R;
      *   - intersection point on ray:
      *      intr *Intr;
      * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
      */
    //BOOL Intersection( const ray &R, intr *Intr ) override
    //{
    //    vec3 norm_arr[] = 
    //    {
    //      vec3(-1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), 
    //      vec3(0.0, -1.0,  0.0), vec3(0.0, 1.0, 0.0), 
    //      vec3(0.0,  0.0, -1.0), vec3(0.0, 0.0, 1.0)
    //    };

    //    FLT txMin = (Min[0] - R.Org[0]) / R.Dir[0]; 
    //    FLT txMax = (Max[0] - R.Org[0]) / R.Dir[0]; 
 
    //    if (txMin > txMax) 
    //      mth::Swap(&txMin, &txMax); 
 
    //    FLT tyMin = (Min[1] - R.Org[1]) / R.Dir[1]; 
    //    FLT tyMax = (Max[1] - R.Org[1]) / R.Dir[1]; 
 
    //    if (tyMin > tyMax) 
    //      mth::Swap(&tyMin, &tyMax); 
 
    //    if ((txMin > tyMax) || (tyMin > txMax)) 
    //      return FALSE;
 
    //    if (tyMin > txMin) 
    //        txMin = tyMin; 
 
    //    if (tyMax < txMax) 
    //        txMax = tyMax; 
 
    //    FLT tzMin = (Min[2] - R.Org[2]) / R.Dir[2]; 
    //    FLT tzMax = (Max[2] - R.Org[2]) / R.Dir[2]; 
 
    //    if (tzMin > tzMax) 
    //      mth::Swap(&tzMin, &tzMax); 
 
    //    if ((txMin > tzMax) || (tzMin > txMax)) 
    //        return FALSE; 
 
    //    if (tzMin > txMin) 
    //        txMin = tzMin; 
 
    //    if (tzMax < txMax) 
    //      txMax = tzMax; 
    //    Intr->T = txMin;
    //    return TRUE; 
    //} /* End of 'Intersection' function */
    
     /* Find intersection on box function.
      * ARGUMENTS: 
      *   - input ray:
      *      const ray &R;
      *   - intersection point on ray:
      *      intr *Intr;
      * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
      */
    BOOL Intersection( const ray &R, intr *Intr ) override
    {
      DBL tnear = 0, tfar = HUGE_VAL;
      vec3 Normals[6] =
      {
        vec3(-1, 0, 0),
        vec3(1, 0, 0),
        vec3(0, -1, 0),
        vec3(0, 1, 0),
        vec3(0, 0, -1),
        vec3(0, 0, 1),
      };
      INT NormNum = -1;

      // X axis
      if (abs(R.Dir[0]) < Threshold)
      {
        if (R.Org[0] < Min[0] || R.Org[0] > Max[0])
          return FALSE;
      }
      else
      {
        DBL t0 = (Min[0] - R.Org[0]) / R.Dir[0];
        DBL t1 = (Max[0] - R.Org[0]) / R.Dir[0];
        DBL tmp;
        INT ind = 0;

        if (t0 > t1)
        {
          COM_SWAP(t0, t1, tmp);
          ind = 1;
        }
        if (t0 > tnear)
        {
          tnear = t0;
          NormNum = ind;
        }
        if (t1 < tfar)
          tfar = t1;
        if (tnear > tfar)
          return FALSE;
        if (tfar < 0)
          return FALSE;
      }

      // Y axis
      if (abs(R.Dir[0]) < Threshold)
      {
        if (R.Org[1] < Min[1] || R.Org[1] > Max[1])
          return FALSE;
      }
      else
      {
        DBL t0 = (Min[1] - R.Org[1]) / R.Dir[1];
        DBL t1 = (Max[1] - R.Org[1]) / R.Dir[1];
        DBL tmp;
        INT ind = 2;

        if (t0 > t1)
        {
          COM_SWAP(t0, t1, tmp);
          ind = 3;
        }
        if (t0 > tnear)
        {
          tnear = t0;
          NormNum = ind;
        }
        if (t1 < tfar)
          tfar = t1;
        if (tnear > tfar)
          return FALSE;
        if (tfar < 0)
          return FALSE;
      }

      // Z axis
      if (abs(R.Dir[2]) < Threshold)
      {
        if (R.Org[2] < Min[2] || R.Org[2] > Max[2])
          return FALSE;
      }
      else
      {
        DBL t0 = (Min[2] - R.Org[2]) / R.Dir[2];
        DBL t1 = (Max[2] - R.Org[2]) / R.Dir[2];
        DBL tmp;
        INT ind = 4;

        if (t0 > t1)
        {
          COM_SWAP(t0, t1, tmp);
          ind = 5;
        }
        if (t0 > tnear)
        {
          tnear = t0;
          NormNum = ind;
        }
        if (t1 < tfar)
          tfar = t1;
        if (tnear > tfar)
          return FALSE;
        if (tfar < 0)
          return FALSE;
      }

      Intr->Shp = this;
      Intr->T = tnear;
      if (NormNum != -1)
        Intr->N = Normals[NormNum];
      for (INT i = 0; i < 5; i++)
        Intr->add[i] = 0;

      return TRUE;
    } /* End of 'Intersection' function */

    /* Check if ray intersects object function.
     * ARGUMENTS: 
     *   - input ray:
     *      const ray &R;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    BOOL IsIntersected( const ray &Ray ) override
    {
      INT Ind = 1, tind = 0;
      DBL tnear = -HUGE_VAL, tfar = HUGE_VAL, t0, t1, tmp;

      // X axis
      if (fabs(Ray.Dir[0]) < Threshold)
        if (Min[0] > Ray.Org[0] || Ray.Org[0] > Max[0])
          return FALSE;
      
      tnear = (Min[0] - Ray.Org[0]) / Ray.Dir[0];
      tfar = (Max[0] - Ray.Org[0]) / Ray.Dir[0];
      
      if (tnear > tfar)
        COM_SWAP(tnear, tfar, tmp), Ind = 0;

      // Y axis
      if (fabs(Ray.Dir[1]) < Threshold)
        if (Min[1] > Ray.Org[1] || Ray.Org[1] > Max[1])
          return FALSE;
      
      t0 = (Min[1] - Ray.Org[1]) / Ray.Dir[1];
      t1 = (Max[1] - Ray.Org[1]) / Ray.Dir[1];
      tind = 3;
      if (t0 > t1)
        COM_SWAP(t0, t1, tmp), tind = 2;
      if (t0 > tnear)
        tnear = t0, Ind = tind;
      if (t1 < tfar)
        tfar = t1;
      if (tnear > tfar || tfar < 0)
        return FALSE;

      // Z axis
      if (fabs(Ray.Dir[2]) < Threshold)
        if (Min[2] > Ray.Org[2] || Ray.Org[2] > Max[2])
          return FALSE;
      
      t0 = (Min[2] - Ray.Org[2]) / Ray.Dir[2];
      t1 = (Max[2] - Ray.Org[2]) / Ray.Dir[2];
      tind = 5;
      if (t0 > t1)
        COM_SWAP(t0, t1, tmp), tind = 4;
      if (t0 > tnear)
        tnear = t0, Ind = tind;
      if (t1 < tfar)
        tfar = t1;
      if (tnear > tfar || tfar < 0)
        return FALSE;

      return TRUE;
    } /* End of 'IsIntersected' function */
  }; /* End of 'box' class */
} /* end of 'ivrt' namespace */

#endif /* __box_h_ */

/* END OF 'box.h' FILE */