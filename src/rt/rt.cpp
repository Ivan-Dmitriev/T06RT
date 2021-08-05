/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : rt.cpp
 * PURPOSE     : Raytracing project.
 *               Raytracing class implementation module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev
 * LAST UPDATE : 02.08.2021
 * NOTE        : Module namespace 'ivrt'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "rt.h"

/* Find intersection function.
 * ARGUMENTS: 
 *   - input ray:
 *      const ray &R;
 *   - intersection point on ray:
 *      intr *Intr;
 * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL ivrt::scene::Intersection( const ray &R, intr *Intr )
{
  intr intersection, closest_intersection;

  for (INT i = 0; i < Shapes.size(); i++)
  {
    if ((Shapes[i]->Intersection(R, &intersection)) && 
         ((closest_intersection.Shp == nullptr) || (closest_intersection.T > intersection.T)))
      closest_intersection = intersection;
  }
  *Intr = closest_intersection;

  return closest_intersection.Shp != nullptr;
} /* End of 'ivrt::scene::Intersection' function */

/* Find intersection function.
 * ARGUMENTS: 
 *   - input ray:
 *      const ray &R;
 *   - intersection point on ray:
 *      intr *Intr;
 * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL ivrt::scene::IsIntersected( const ray &R )
{
  intr intersection, closest_intersection;

  for (INT i = 0; i < Shapes.size(); i++)
  {
    if (Shapes[i]->IsIntersected(R)) 
      return TRUE;
  }

  return closest_intersection.Shp != nullptr;
} /* End of 'ivrt::scene::IsIntersected' function */

/* Get color of factor function.
 * ARGUMENTS: 
 *   - input ray:
 *      const vec3 &Dir;
 *   - currrent environment:
 *       const envi &Media;
 *   - intersection point:
 *       intr *Intersection;
 *   - info about light:
 *       light_info *L;
 *   - weight of lighting:
 *       DBL Weight;
 * RETURNS: (vec3 ) result color.
 */
ivrt::vec3 ivrt::scene::Shade( vec3 &Dir, const envi &Media, intr *Inter, DBL Weight )
{
  DBL vn = Inter->N & Dir;
  if (vn > 0)
    vn = -vn, Inter->N = -Inter->N;

  vec3 Diffuse = vec3(0), Specular = vec3(0), Ambient = vec3(0.1), Color(0);
  //vec3 R = Dir - Inter->N * (2 * (Dir & Inter->N));
  vec3 R = Inter->N.Reflect(Dir);
  for (auto OneLight : Lights)
  {
    intr I;

    light_info li;
      
    DBL att = OneLight->Shadow(Inter->P, &li);
    vec3 
      L = li.L,
      V = Dir;
    Ambient = Inter->Shp->mtl.Ka;
    //vec3 N = Inter->N * (-V & Inter->N);
    //vec3 R = V - N * 2 * (V & N);
    //vec3 R = N.Reflect(V);
    DBL nl = Inter->N & L;
    if (nl > Threshold)
      Diffuse = Diffuse + li.Color * Inter->Shp->mtl.Kd * att * nl;
    DBL rl = R & L;

    if (rl > Threshold)
      Specular = Specular + li.Color * pow(rl, Inter->Shp->mtl.Ph);

    if (Intersection(ray(Inter->P + L * Threshold, L), &I) && (I.T + Threshold > li.Dist))
      Color += (Diffuse + Specular) * att * 0.10;
    else
      Color += (Diffuse + Specular) * att;
    /*
    if (!Inter->add[4])
      Color = Inter->Shp->mtl.Ka + Inter->Shp->mtl.Kd * Diffuse + Inter->Shp->mtl.Ks * Specular;
    else
      Color  = GetKa(Inter->P + R * Threshold) + Inter->Shp->mtl.Kd * Diffuse + Inter->Shp->mtl.Ks * Specular;
    */
  }
  return mth::vec3<DBL>::ClampV((Ambient + Color) * Weight);
} /* End of 'ivrt::scene::Shade' function */

/* Trace ray function.
 * ARGUMENTS: 
 *   - input ray:
 *       const ray &R;
 *   - currrent environment:
 *       const envi &Media;
 *   - weight of lighting:
 *       DBL Weight;
 *   - Current recursion level:
 *       INT RecLevel; 
 * RETURNS: (vec3 ) result color.
 */
ivrt::vec3 ivrt::scene::Trace( ray &R, const envi &Media, DBL Weight, INT RecLevel )
{
  vec3 color = Background;

  if (RecLevel < MaxRecLevel)
  {
    intr Intr;
    if (Intersection(R, &Intr))
    {
      if (!Intr.IsNorm)
        Intr.Shp->GetNormal(&Intr);
      //if (!Intr.IsPos)
      //  Intr.P = R(Intr.T);
      color = Shade(R.Dir, Media, &Intr, Weight);
      /*
      DBL fogcoef = exp(-0.007 * Intr.T);
      DBL interpfog = 0;
      
      if (Intr.T < FogStart)
        interpfog = 1;
      else if (Intr.T > FogEnd)
        interpfog = 0;
      else 
        interpfog = (Intr.T - FogStart) / (FogEnd - FogStart);

      color = color * fogcoef + FogColor * (1 - fogcoef);
      */
      vec3 reflraydir = Intr.N.Reflect(R.Dir);

      //DBL wt = Weight * Intr.Shp->mtl.Kr;
      //if (wt > Threshold)
        //color += Trace(ray(Intr.Shd.P + R.oooo
        // Dir * Threshold, R.Dir), Media, wr) * Shd.mtl.Krefl;
      ray NewR(Intr.P + reflraydir * Threshold, reflraydir);

      Weight *= Intr.Shp->mtl.Kr;
      if (Weight > 0.1)
        color += Trace(NewR, Media, Weight, ++RecLevel);
      
      //DBL rc = Weight * Intr.Shp->mtl.Kt;
      
      //if (rc > Threshold)
      //{
      //  vec3 v = R.Dir;

      //  DBL cosa = -v & Intr.N;
      //  DBL ETAratio = /* Intr.Shp-> */Media.RefractionCoef / Glass.RefractionCoef;
      //  
      //  //vec3 T = ETAratio * (v - Intr.N);
      //}
    }
  }
  return color;
}

/* END OF 'rt.cpp' FILE */