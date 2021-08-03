/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_vec2.h
 * PURPOSE     : Raytracing project.
 *               Mathematics library.
 *               Vec2 handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev
 * LAST UPDATE : 02.08.2021
 * NOTE        : Module namespace 'mth'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec2_h_
#define __mth_vec2_h_

#include <iostream>

#include "mth_def.h"

namespace mth
{
  template<typename Type>
    class vec2
    {
    private:
      Type X, Y;
    public:
      vec2( Type NewX, Type NewY ) : X(NewX), Y(NewY)
      {
      } /* End of 'constructor' function */

      /* Constructor of vec3 class function.
       * ARGUMENTS: None.
       * RETURNS: None.  
       */
      explicit vec2( Type NewV ) : X(NewV), Y(NewV)
      {
      } /* End of 'constructor' function */
      /* Constructor of vec3 class function.
       * ARGUMENTS: None.
       * RETURNS: None.  
       */
      explicit vec2( VOID )
      {
      } /* End of 'constructor' function */

    }; /* End of 'vec2' class */
} /* end of 'mth' namespace */

#endif /* __mth_vec2_h_ */
/* END OF 'mth_vec2.h' FILE */