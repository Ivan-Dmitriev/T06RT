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

/* END OF 'rt.cpp' FILE */