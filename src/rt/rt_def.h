/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : rt_def.h
 * PURPOSE     : Raytracing project.
 *               Raytracing class declaration module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev
 * LAST UPDATE : 02.08.2021
 * NOTE        : Module namespace 'ivrt'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __rt_def_h_
#define __rt_def_h_

#include "../def.h"

#include "lights/light.h"

/* Project namespace */
namespace ivrt
{
  const DBL FogStart = 10;
  const DBL FogEnd = 50;
  const vec3 FogColor(0.1, 0.2, 0.5);

  const static DBL Threshold = 0.0001;
  class shape;
  
  /* Common entry type */
  enum ENTRY_TYPE
  {
    ENTER,  // Enter state 
    STAY,   // Stay state
    LEAVE   // Leave state
  }; /* End of 'ENTRY_TYPE' enum */

  /* Common intersection class */
  class intr
  {
  public:
    DBL T;            // Intersection dist 
    shape *Shp;       // Shape             
    vec3 Color;       // Color 
    vec3 N;           // Normal            
    BOOL IsPos;       // Flag eval pos     
    BOOL IsNorm;      // Flag eval normal  
    vec3 P;           // Position

    BOOL add[5] = {0};
    INT I[5];         // Addon (INT)       
    DBL D[5];         // Addon (DOUBLE)    

    /* Intr class constructor */
    intr( VOID ) : IsNorm(FALSE), IsPos(FALSE)
    {
    } /* End of 'intr' function */
    intr( shape *NShp, DBL NewT ) : IsNorm(FALSE), Shp(NShp), T(NewT) 
    {
    } /* End of 'intr' function */
  }; /* End of 'intr' class */

  /* Surface class */
  class surface
  {
  public:
    std::string Name; // material name
    vec3 Ka, Kd, Ks;  // ambient, diffuse, specular
    DBL Ph;           // Bui Tong Phong coefficient
    DBL Kr, Kt;       // reflected, transmitted
    surface( VOID ) : Ka(vec3(0.23125)), Kd(vec3(0.2775)), Ks(vec3(0.773911)), Kr(0.4), Kt(0.1), Ph(89.6)
    {
    }
    surface( vec3 NKa, vec3 NKd, vec3 NKs, DBL NPh, DBL NKr, DBL NKt ) :
      Ka(NKa), Kd(NKd), Ks(NKs), Kr(NKr), Kt(NKt), Ph(NPh)
    {
    }
  }; /* End of 'surface' class */

  /* All intersections list */
  struct intr_list 
  {
  public:
    std::vector<intr> I_list; 
  }; /* End of 'intr_list ' class */

  /* Environment class */
  class envi
  {
  public:
    DBL RefractionCoef;
    DBL DecayCoef;
    envi( DBL NRefractionCoef, DBL NDecayCoef ) : DecayCoef(NDecayCoef), RefractionCoef(NRefractionCoef)
    {
    }
    envi( VOID ) : DecayCoef(1), RefractionCoef(1)
    {
    }

  }; /* End of 'envi' class */
  
  const envi Air(0.5, 1);
  const envi Glass(1.517, 1);


  /* Shape class */
  class shape
  {
  public:
    surface mtl;

    /* Find intersection function.
     * ARGUMENTS: 
     *   - ray:
     *      const ray &R;
     *   - intersection point on ray:
     *      intr *Intr;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    virtual BOOL Intersection( const ray &R, intr *Intr )
    {
      return TRUE;
    } /* End of 'Intersection' function */
    /* Check if ray intersects object function.
     * ARGUMENTS: 
     *   - input ray:
     *      const ray &R;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    virtual BOOL IsIntersected( const ray &R )
    {
      return TRUE;
    } /* End of 'IsIntersected' function */

    /* Get normal function.
     * ARGUMENTS: 
     *   - intersection point on ray:
     *      intr *Intr;
     * RETURNS: NONE.
     */
    virtual VOID GetNormal( intr *Intr )
    {
    } /* End of 'GetNormal' function */

    /* Check if point is inside of the shape.
     * ARGUMENTS:
     *   - Reference ray to intersect:
     *         ray &R;
     *   - Reference to stock of intersections:
     *         intr_list &IList;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    virtual BOOL IsInside( const vec3 &P )
    {
      return FALSE;
    } /* End of 'IsInside' function */

    /* Find num of intersections with current shape.
     * ARGUMENTS:
     *   - Reference ray to intersect:
     *         ray &R;
     *   - Reference to stock of intersections:
     *         intr_list &IList;
     * RETURNS: None.
     */
    virtual INT AllIntersect( const ray &R, intr_list &IList )
    {
      return 0;
    } /* End of 'AllIntersect' function */

  }; /* End of 'shape' class */


  /* Common shape_info class */
  class shape_info
  {
  public:
    vec3 N;       // Shape normal
    surface surf; // Shape material 
    shape *Shp;   // Reference to shape 
    vec3 P;       // Position of intersection point 
    vec3 Du, Dv;  // Vectores for texture coords
    envi Env;     // Shape environment

   /* Change current environment of shape function.
    * ARGUMENTS: 
    *   - Light to be add:
    *       light *NewShape;
    * RETURNS: (scene & ) link on result scene.
    */
    shape_info & operator<<( envi NewEnvy )
    {
      Env = NewEnvy;

      return *this;
    } /* End of 'operator<<' function */

  }; /* End of 'shape_info' class */

  /* Scene class */
  class scene
  {
  private:
    std::vector<shape *> Shapes;
    std::vector<light *> Lights;
    vec3 AmbientColor, Background = vec3(0.1);
    INT RecLevel = 0, MaxRecLevel = 3;
 
  public:
    /* Scene destructor */
    ~scene( VOID )
    {
      for (auto p : Shapes)
        delete p;
      Shapes.clear();
      for (auto l : Lights)
        delete l;
      Lights.clear();
    }

    /* Find intersection function.
     * ARGUMENTS: 
     *   - ray:
     *      const ray &R;
     *   - intersection point on ray:
     *      intr *Intr;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    BOOL Intersection( const ray &R, intr *Intr );

    /* Find intersection function.
     * ARGUMENTS: 
     *   - ray:
     *      const ray &R;
     *   - intersection point on ray:
     *      intr *Intr;
     * RETURNS: (BOOL) TRUE if success, FALSE otherwise.
     */
    BOOL IsIntersected( const ray &R );

   /* Add new shape of scene to stock function.
    * ARGUMENTS: 
    *   - Shape to be add:
    *       shape *NewShape;
    * RETURNS: (scene & ) link on result scene.
    */
    scene & operator<<( shape *NewShape )
    {
      Shapes.push_back(NewShape);

      return *this;
    } /* End of 'operator<<' function */

   /* Add new light of scene to stock function.
    * ARGUMENTS: 
    *   - Light to be add:
    *       light *NewShape;
    * RETURNS: (scene & ) link on result scene.
    */
    scene & operator<<( light *NewLight )
    {
      Lights.push_back(NewLight);

      return *this;
    } /* End of 'operator<<' function */

    /* Get color of factor function.
     * ARGUMENTS: 
     *   - Ray direction:
     *      const ray &R;
     *   - currrent environment:
     *       const envi &Media;
     *   - intersection point:
     *       intr *Intersection;
     *   - weight of lighting:
     *       DBL Weight;
     * RETURNS: (vec3 ) result color.
     */
    vec3 Shade( vec3 &Dir, const envi &Media, intr *Intersection, DBL Weight );
    
   /* Trace ray function.
    * ARGUMENTS: 
    *   - input ray:
    *       const ray &R;
    *   - currrent environment:
    *       const envi &Media;
    *   - weight of lighting:
    *       DBL Weight;
    * RETURNS: (vec3 ) result color.
    */
    vec3 Trace( ray &R, const envi &Media, DBL Weight, INT RecLevel );
    
   /* Get Ka by position function.
    * ARGUMENTS: 
    *   - input position:
    *       const vec3 &P;
    * RETURNS: (vec3 ) result value.
    */
    vec3 GetKa( const vec3 &P )
    {
      return vec3(((INT)(P[0]) ^ (INT)(P[1]) ^ (INT)(P[2])) & 1);
    }

  }; /* End of 'scene' class */
 

} /* end of 'ivrt' namespace */

#endif /* __rt_def_h_ */

/* END OF 'rt_def.h' FILE */