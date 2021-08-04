/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : light.h
 * PURPOSE     : Raytracing project.
 *               light declaration class.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 02.08.2021.
 * NOTE        : Module namespace 'ivrt'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __light_h_
#define __light_h_

#include "../../def.h"

/* Project namespace */
namespace ivrt
{
  class intr;
  /* Light info class */
  class light_info 
  {
  public:
    vec3 L;     // light source direction
    vec3 Color; // light source color
    DBL Dist;   // distance to light source
    /* Light info default constructor */
    light_info( VOID ) : Dist(0)
    {
    } /* End of 'light_info' function */

    /* Light info constructor */
    light_info( vec3 NL, vec3 NColor, DBL NDist ) : L(NL), Color(NColor), Dist(NDist)
    {
    } /* End of 'light_info' function */
 
  }; /* End of 'light_info ' class */

  /* Light class */
  class light 
  {
  public:
    DBL Cc, Cl, Cq;
 
    light( VOID ) : Cc(1.0), Cl(0.01), Cq(0.01)
    {
    } /* End of 'light' function */

    /* Get attenuation factor function.
     * ARGUMENTS: 
     *   - input ray:
     *      const ray &R;
     *   - information about light:
     *      light_info *L;
     * RETURNS: (DBL) result value.
     */
    virtual DBL Shadow( vec3 &P, light_info *L )
    {
      return 0.0;
    } /* End of 'Shadow' function */

    /* Get color function.
     * ARGUMENTS: 
     *   - intersection info:
     *      intr *Intr;
     *   - direction of view:
     *      vec3 &Dir;
     *   - information about light:
     *      light_info *L;
     * RETURNS: (vec3) result color.
     */
    //vec3 Shade( intr *Intr, vec3 &Dir, light_info *L )
    //{

    //  return color;
    //} /* End of 'Shade' function */
  }; /* End of 'light' class */

} /* end of 'ivrt' namespace */

#endif /* __light_h_ */

/* END OF 'light.h' FILE */