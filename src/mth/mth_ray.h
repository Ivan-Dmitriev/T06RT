/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME    : mth_ray.h
 * PURPOSE      : Raytracing project.
 *                Mathematics library.
 *                Space camera handle module.
 * PROGRAMMER   : CGSG-SummerCamp'2021.
 *                Ivan Dmitriev
 * LAST UPDATE  : 02.08.2021
 * NOTE         : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_ray_h_
#define __mth_ray_h_

#include "mth_vec3.h"
#include "mth.h"

#include <iostream>

 /* Math library namespace */
namespace mth
{
  /* 3D ray class declaration */
  template<class type>
    class ray
    {
      //friend class camera;
    public:
      vec3<type>
        /* Ray origin */
        Org, 
        /* Ray normalized direction */
        Dir; 

    public:
      /* Class default constructor */
      ray( VOID )
      {
      } /* End of 'ray' function */

      /* Class copying constructor.
       * ARGUMENTS:
       *   - ray to be copied:
       *       const ray &R;
       */
      ray( const ray &R ) : Org(R.Org), Dir(R.Dir)
      {
      } /* End of 'ray' function */

      /* Class constructor.
       * ARGUMENTS:
       *   - ray new Orgin:
       *       const vec3<type> &Origin, &Direction;
       *   - ray new Direction:
       *       const vec3<type> &Direction;
       */
      ray( const vec3<type> &Origin, const vec3<type> &Direction ) : 
        Org(Origin), Dir(static_cast<vec3<type>>(Direction).Normalizing())
      {
        //Dir.Normalize();
      } /* End of 'ray' function */

      /* Obtain ray point function.
       * ARGUMENTS:
       *   - ray point parameter:
       *       type T;
       * RETURNS:
       *   (vec3<type>) ray point.
       */
      vec3<type> operator()( type T ) const
      {
        return Org + Dir * T;
      } /* End of 'operator()' function */

      /* Obtain ray point function.
       * ARGUMENTS:
       *   - ray point parameter:
       *       type T;
       * RETURNS:
       *   (vec<type>) ray point.
       */
      vec3<type> Apply( type T ) const
      {
        return Org + Dir * T;
      } /* End of 'ApplyPoint' function */

      /* Obtain ray Orgin point function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec<type>) result ray Orgin point.
       */
      vec3<type> GetOrg( VOID ) const
      {
        return Org;
      } /* End of 'GetOrg' function */

      /* Obtain ray Direction vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3<type>) result ray Direction vector.
       */
      vec3<type> GetDir( VOID ) const
      {
        return Dir;
      } /* End of 'GetDir' function */

      /* Check intersection between sphere and ray function.
       * ARGUMENTS:
       *   - sphere center:
       *       vec3<type> C, V2;
       *   - sphere radius:
       *       type R;
       * RETURNS:
       *   (BOOL) TRUE if ok, FALSE otherwise.
       */
      BOOL SphereInter( vec3<type> C, type R )
      {
        vec3<type> a = C - Org;
        FLT OC2, OK, OK2, R2, h2;

        OC2 = VecDotVec(a, a);
        OK = VecDotVec(a, Dir);
        OK2 = VecDotVec(a, Dir) * VecDotVec(a, Dir);
        R2 = R * R;
        h2 = R2 - (OC2 - OK2);
        if (OC2 < R2)
          return TRUE;
        if (OK < 0 || h2 < 0)
          return FALSE;
        return TRUE;
      } /* End of 'SphereInter' function */

      /* Check intersection between box and ray function.
       * ARGUMENTS:
       *   - box borders:
       *       vec3<type> min, max;
       *   - ray:
       *       const ray &r;
       * RETURNS:
       *   (BOOL) TRUE if ok, FALSE otherwise.
       */
      BOOL BoxInter( const ray &r, vec3<type> min, vec3<type> max )
      { 
        FLT tmin = (min[0] - r.Org[0]) / r.Dir[0]; 
        FLT tmax = (max[0] - r.Org[0]) / r.Dir[0]; 
 
        if (tmin > tmax) 
          ;//mth::Swap(&tmin, &tmax); 
 
        FLT tymin = (min[1] - r.Org[1]) / r.Dir[1]; 
        FLT tymax = (max[1] - r.Org[1]) / r.Dir[1]; 
 
        if (tymin > tymax) 
          ;//mth::Swap(&tymin, &tymax); 
 
        if ((tmin > tymax) || (tymin > tmax)) 
          return FALSE; 
 
        if (tymin > tmin) 
            tmin = tymin; 
 
        if (tymax < tmax) 
            tmax = tymax; 
 
        FLT tzmin = (min[2] - r.Org[2]) / r.Dir[2]; 
        FLT tzmax = (max[2] - r.Org[2]) / r.Dir[2]; 
 
        if (tzmin > tzmax) 
          ;//mth::Swap(&tzmin, &tzmax); 
 
        if ((tmin > tzmax) || (tzmin > tmax)) 
            return FALSE; 
 
        if (tzmin > tmin) 
            tmin = tzmin; 
 
        if (tzmax < tmax) 
            tmax = tzmax; 
 
        return TRUE; 
      } /* End of 'BoxInter' function */
    }; /* End of 'ray' class */

} /* end of 'mth' namespace */

#endif /* __mth_ray_h_ */

/* END OF 'mth_ray.h' FILE */
