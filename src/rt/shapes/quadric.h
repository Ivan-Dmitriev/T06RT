/***************************************************************
 * Copyright (C) 2015
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/
/* FILE NAME   : quadric.H
 * PURPOSE     : Ray tracing project.
 *               Ray  tracing quadric module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 04.08.2021.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __QUADRIC_H_
#define __QUADRIC_H_

#include "def.h"

/* Ray tracing work namespace */
namespace ivrt
{
  /* Quadruc surface representation form */
  class quadric : public shape
  {
  private:
    /* Quadric surface coefficent */
    DBL A, B, C, D, E, F, G, H, I, J;

  public:
    /* Quadric surface constructor.
     * ARGUMENTS:
     *   - coefficents:
     *       DBL A, B, C, D, E, F, G, H, I, J;
     *   - color:
     *       const color &Color = color(1);
     */
    quadric( DBL A, DBL B, DBL C, DBL D, DBL E, DBL F, DBL G, DBL H, DBL I, DBL J,
             const color &Color = color(1) ) :
             shape(Color), A(A), B(B), C(C), D(D), E(E), F(F), G(G), H(H), I(I), J(J)
    {
    } /* End of 'quadric' function */

    /* Intersect find function.
     * ARGUMENTS:
     *   - ray to find intersect:
     *       const ray &R;
     *   - intersect pointer (for output):
     *       intr *Intr;
     * RETURNS:
     *   (BOOL) has intersection.
     */
    BOOL Intersect( const ray &R, intr *Intr )
    {
      DBL Treshold = 1e-4;
      DBL Dx = R.Dir[0], Dy = R.Dir[1], Dz = R.Dir[2],
          Ox = R.Org[0], Oy = R.Org[1], Oz = R.Org[2];

      DBL
        a = A * Dx * Dx + 2 * B * Dx * Dy + 2 * C * Dx * Dz + E * Dy * Dy +
            2 * F * Dy * Dz + H * Dz * Dz,
        b = A * Ox * Dx + B * (Ox * Dy + Dx * Oy) + C * (Ox * Dz + Dx * Oz) +
            D * Dx + E * Oy * Dy + F * (Oy * Dz + Dy * Oz) + G * Dy + H * Oz * Dz + I * Dz,
        c = A * Ox * Ox + 2 * B * Ox * Oy + 2 * C * Ox * Oz + 2 * D * Ox + E * Oy * Oy +
            2 * F * Oy * Oz + 2 * G * Oy + H * Oz * Oz + 2 * I * Oz + J,
        d = b * b - a * c;

      if (COM_ABS(a) < Treshold)
        return FALSE;
      if (d < Treshold)
        return FALSE;
      d = sqrt(d);

      DBL t1 = (-b + d) / a, t2 = (-b - d) / a;
      if (a < 0)
      {
        DBL tmp;
        COM_SWAP(t1, t2, tmp);
      }
      if (t2 > 0)
      {
        Intr->Set(this, t2, TRUE);
        return TRUE;
      }
      if (t1 > 0)
      {
        Intr->Set(this, t1, FALSE);
        return TRUE;
      }
      return FALSE;
    } /* End of 'Intersect' function */

    /* Check for intersection function.
     * ARGUMENTS:
     *   - ray to check:
     *       const ray &R;
     * RETURNS:
     *   (BOOL) has intersection.
     */
    BOOL IsIntersect( const ray &R )
    {
      DBL Treshold = 1e-4;
      DBL Dx = R.Dir[0], Dy = R.Dir[1], Dz = R.Dir[2],
          Ox = R.Org[0], Oy = R.Org[1], Oz = R.Org[2];

      DBL
        a = A * Dx * Dx + 2 * B * Dx * Dy + 2 * C * Dx * Dz + E * Dy * Dy +
            2 * F * Dy * Dz + H * Dz * Dz,
        b = A * Ox * Dx + B * (Ox * Dy + Dx * Oy) + C * (Ox * Dz + Dx * Oz) +
            D * Dx + E * Oy * Dy + F * (Oy * Dz + Dy * Oz) + G * Dy + H * Oz * Dz + I * Dz,
        c = A * Ox * Ox + 2 * B * Ox * Oy + 2 * C * Ox * Oz + 2 * D * Ox + E * Oy * Oy +
            2 * F * Oy * Oz + 2 * G * Oy + H * Oz * Oz + 2 * I * Oz + J,
        d = b * b - a * c;

      if (COM_ABS(a) < Treshold)
        return FALSE;
      if (d < Treshold)
        return FALSE;
      d = sqrt(d);

      DBL t1 = (-b + d) / a, t2 = (-b - d) / a;
      if (a < 0)
      {
        DBL tmp;
        COM_SWAP(t1, t2, tmp);
      }
      return t2 > 0 || t1 > 0;
    } /* End of 'IsIntersect' function */

    /* Check if point is inside function.
     * ARGUMENTS:
     *   - point to check:
     *       const vec &P;
     * RETURNS:
     *   (BOOL) is inside.
     */
    BOOL IsInside( const vec &P )
    {
      DBL Treshold = 1e-4, X = P[0], Y = P[1], Z = P[2];
      return A * X * X + E * Y * Y + H * Z * Z + 2 * B * X * Y + 2 * C * X * Z +
             2 * D * X + 2 * F * Y * Z + 2 * G * Y + 2 * I * Z + J > Treshold;
    } /* End of 'IsInside' function */

    /* Find all intersections function.
     * ARGUMENTS:
     *   - ray to find intersections:
     *       const ray &R;
     *   - intersection list (for output):
     *       intr_list &IList;
     * RETURNS:
     *  (INT) number of intersections.
     */
    INT AllIntersect( const ray &R, intr_list &IList )
    {
      DBL Treshold = 1e-4;
      DBL Dx = R.Dir[0], Dy = R.Dir[1], Dz = R.Dir[2],
          Ox = R.Org[0], Oy = R.Org[1], Oz = R.Org[2];
      INT count = 0;

      DBL
        a = A * Dx * Dx + 2 * B * Dx * Dy + 2 * C * Dx * Dz + E * Dy * Dy +
            2 * F * Dy * Dz + H * Dz * Dz,
        b = A * Ox * Dx + B * (Ox * Dy + Dx * Oy) + C * (Ox * Dz + Dx * Oz) +
            D * Dx + E * Oy * Dy + F * (Oy * Dz + Dy * Oz) + G * Dy + H * Oz * Dz + I * Dz,
        c = A * Ox * Ox + 2 * B * Ox * Oy + 2 * C * Ox * Oz + 2 * D * Ox + E * Oy * Oy +
            2 * F * Oy * Oz + 2 * G * Oy + H * Oz * Oz + 2 * I * Oz + J,
        d = b * b - a * c;

      if (COM_ABS(a) < Treshold)
        return 0;
      if (d < Treshold)
        return 0;
      d = sqrt(d);

      DBL t1 = (-b + d) / a, t2 = (-b - d) / a;
      if (a < 0)
      {
        DBL tmp;
        COM_SWAP(t1, t2, tmp);
      }
      if (t2 > 0)
      {
        IList.push_back(intr(this, t2, TRUE)), count++;
      }
      if (t1 > 0)
      {
        IList.push_back(intr(this, t1, FALSE)), count++;
      }
      return count;
    } /* End of 'AllIntersect' function */

    /* Normal get function.
     * ARGUMENTS:
     *   - intersection structure:
     *       intr *Intr;
     * RETURNS: None.
     */
    VOID GetNormal( intr *Intr )
    {
      if (!Intr->IsP || Intr->IsN)
        return;
      DBL X = Intr->P[0], Y = Intr->P[1], Z = Intr->P[2];
      Intr->IsN = TRUE;
      Intr->N = vec(2 * A * X + 2 * B * Y + 2 * C * Z + 2 * D,
                    2 * B * X + 2 * C * Y + 2 * F * Z + 2 * G,
                    2 * C * X + 2 * F * Y + 2 * H * Z + 2 * I);
      Intr->N.Normalize();
    } /* End of 'GetNormal' function */
  }; /* End of 'quadric' class */
} /* end of 'ivrt' namespace */

#endif /* __QUADRIC_H_ */

/* END OF '' FILE */
