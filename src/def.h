/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : def.h
 * PURPOSE     : Raytracing project.
 *               Common definitions handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev
 * LAST UPDATE : 02.08.2021
 * NOTE        : Module namespace 'mth'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __def_h_
#define __def_h_

#pragma comment(lib, "opengl32")

#define GLEW_STATIC
#include <glew.h>
#include <wglew.h>

#ifndef WIN32
#define WIN32
#include <commondf.h>
#undef WIN32
#else
#include <commondf.h>
#endif

#include <utility>
#include <vector>
#include <cassert>
#include <string>

#include "mth/mth.h"

/* Useful type declarations */
typedef DOUBLE DBL;
typedef FLOAT FLT;

/* Debug memory allocation support */ 
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy 
{ 
public: 
  /* Class constructor */
  __Dummy( VOID ) 
  { 
    SetDbgMemHooks(); 
  } /* End of '__Dummy' constructor */
} __ooppss;
#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

/* Project namespace */
namespace ivrt
{
  /* Math types definitions */
  typedef mth::vec3<DBL> vec3;
  typedef mth::matr<DBL> matr;
  typedef mth::vec2<DBL> vec2;
  typedef mth::vec4<DBL> vec4;
  typedef mth::camera<DBL> camera;
  typedef mth::ray<DBL> ray;
}

/* Stock class template */
template<typename type>
  class stock
  {
  private:
    INT
      RealSize,  // Real number of array elements
      MaxSize;   // Allocated array elements count
    type *Array; // Data array
 
  public:
    /* Class constructor.
     * ARGUMENTS: None.
     */
    stock( VOID ) : Array(nullptr), RealSize(0), MaxSize(0)
    {
    } /* End of 'stock' function */
 
    /* Class destructor */
    ~stock( VOID )
    {
      if (Array != nullptr)
        delete[] Array;
      Array = nullptr;
      RealSize = 0;
      MaxSize = 0;
    } /* End of '~stock' function */
 
    /* Class copying constructor.
     * ARGUMENTS:
     *   - stock to be copy:
     *       const stock<type> &CopyData;
     */
    stock( const stock<type> &CopyData ) :
      Array(new type[CopyData.RealSize]),
      RealSize(CopyData.RealSize),
      MaxSize(CopyData.RealSize)
    {
      for (INT i = 0; i < RealSize; i++)
        Array[i] = CopyData.Array[i];
    } /* End of 'stock' function */
 
    /* Assignment operator function.
     * ARGUMENTS:
     *   - stock to be assign:
     *       const stock<type> &CopyData;
     * RETURNS:
     *   (stock &) self reference.
     */
    stock & operator=( const stock<type> &CopyData )
    {
      if (Array != nullptr)
        delete[] Array;
      Array = new type[CopyData.RealSize];
      RealSize = CopyData.RealSize;
      MaxSize = CopyData.RealSize;
      for (int i = 0; i < RealSize; i++)
        Array[i] = CopyData.Array[i];
      return *this;
    } /* End of 'operator=' function */
 
    /* Obtain stock length function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) number of stock elements.
     */
    INT Size( VOID ) const
    {
      return RealSize;
    } /* End of 'Size' function */
 
    /* Add new element to stock function.
     * ARGUMENTS:
     *   - value to be add:
     *       const type &NewData;
     * RETURNS:
     *   (stock &) self reference.
     */
    stock & Add( const type &NewData )
    {
      if (RealSize >= MaxSize)
      {
        // increase allocated memory
        if (MaxSize == 0)
          MaxSize = 1;
        else
          MaxSize *= 2;
 
        type *new_bulk = new type[MaxSize];
        if (Array != nullptr)
        {
          // copy old data
          for (int i = 0; i < RealSize; i++)
            new_bulk[i] = Array[i];
          // remove old data
          delete[] Array;
        }
        Array = new_bulk;
      }
      Array[RealSize++] = NewData;
      return *this;
    } /* End of '' function */
 
    /* Add new element to stock (as operator) function.
     * ARGUMENTS:
     *   - value to be add:
     *       const type &NewData;
     * RETURNS:
     *   (stock &) self reference.
     */
    stock & operator<<( const type &NewData )
    {
      return Add(NewData);
    } /* End of '' function */
 
    /* Lvalue element access function.
     * ARGUMENTS:
     *   - element index:
     *       INT Index;
     * RETURNS:
     *   (type &) element reference.
     */
    type & operator[]( INT Index )
    {
      assert(Index >= 0 && Index < RealSize);
      return Array[Index];
    } /* End of 'operator[]' function */
 
    /* Constant element access function.
     * ARGUMENTS:
     *   - element index:
     *       INT Index;
     * RETURNS:
     *   (type &) element reference.
     */
    type & operator[]( INT Index ) const
    {
      assert(Index >= 0 && Index < RealSize);
      return Array[Index];
    } /* End of 'operator[]' function */
 
    /* Clear stock content function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Clear( VOID )
    {
      RealSize = 0;
    } /* End of 'Clear' function */
 
    /* Stock walker function.
     * ARGUMENTS:
     *   - access function-functor reference:
     *       type2 &Walker;
     * RETURNS: None.
     */
    template<typename type2>
      VOID Walk( type2 &Walker )
      {
        for (int i = 0; i < RealSize; i++)
          Walker(Array[i]);
      } /* End of 'Walk' function */
  }; /* End of 'stock' class */

#endif /* __def_h_ */

/* END OF 'def.h' FILE */
