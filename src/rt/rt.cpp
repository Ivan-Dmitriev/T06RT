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
  vec3 Diffuse, Specular, Ambient = vec3(0.1), Color(0);
  DBL att;
  vec3 Result;

  for (auto OneLight : Lights)
  {
    light_info li;
      
    DBL att = OneLight->Shadow(Inter->P, &li);
    vec3 
      L = li.L,
      V = Dir;

    vec3 N = Inter->N * (-V & Inter->N), R = V - N * 2 * (V & N), color = vec3(0);

    DBL nl = mth::Max((Inter->N & L), 0.0);
    Diffuse = li.Color * /* Intersection->Shp->Mtl.Kd */ 0.7 * nl;
    DBL rl = mth::Max(R & L, 0.0);
    Specular = li.Color * 0.5 * pow(rl, 10) * rl;
    
    if (IsIntersected(ray(Inter->P + L * Threshold, L)))
      Color += (Diffuse + Specular) * att * 0.10;
    else
      Color += (Diffuse + Specular) * att;
    Result += color;
  }
  return mth::Clamp((Ambient + Color) * Weight);
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
  //vec3 LDir = vec3(2, 2, 2);
  //DBL Dist = LDir.Distance(vec3(0, 0, 0));
  //light_info L(LDir, vec3(1, 0, 0), Dist);

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
      //color = color * exp(-Intr.T * Media.DecayCoef);
      Weight *= 0.9;
      if (Weight > 0.1)
        color += Trace(R, Media, Weight, ++RecLevel);
    }
  }
  return color;
}

/* END OF 'rt.cpp' FILE */