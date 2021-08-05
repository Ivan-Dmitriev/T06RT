/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : main.cpp
 * PURPOSE     : Raytracing project.
 *               Entry point.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 02.08.2021.
 * NOTE        : Module namespace 'ivrt'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <cmath>
#include <ctime>
#include <thread>
#include <map>
#include <vector>

#include "def.h"
#include "win/win.h"
#include "rt/frame/frame.h"
#include "rt/rt.h"
#include "rt/rt_def.h"
#include "rt/lights/point.h"
#include "timer.h"

/* Material structure */
struct
{
public:
  std::string Name;
  ivrt::vec3 Ka, Kd, Ks;
  FLT Ph;
} MatLib[] =
{
  {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},               32},
  {"Brass",         {0.329412,0.223529,0.027451}, {0.780392,0.568627,0.113725}, {0.992157,0.941176,0.807843}, 27.8974},
  {"Bronze",        {0.2125,0.1275,0.054},       {0.714,0.4284,0.18144},       {0.393548,0.271906,0.166721},  25.6},
  {"Chrome",        {0.25, 0.25, 0.25},          {0.4, 0.4, 0.4},              {0.774597, 0.774597, 0.774597}, 76.8},
  {"Copper",        {0.19125,0.0735,0.0225},     {0.7038,0.27048,0.0828},      {0.256777,0.137622,0.086014},  12.8},
  {"Gold",          {0.24725,0.1995,0.0745},     {0.75164,0.60648,0.22648},    {0.628281,0.555802,0.366065},  51.2},
  {"Peweter",       {0.10588,0.058824,0.113725}, {0.427451,0.470588,0.541176}, {0.3333,0.3333,0.521569},      9.84615},
  {"Silver",        {0.19225,0.19225,0.19225},   {0.50754,0.50754,0.50754},    {0.508273,0.508273,0.508273},  51.2},
  {"Polished Silver", {0.23125,0.23125,0.23125}, {0.2775,0.2775,0.2775},       {0.773911,0.773911,0.773911},  89.6},
  {"Turquoise",     {0.1, 0.18725, 0.1745},      {0.396, 0.74151, 0.69102},    {0.297254, 0.30829, 0.306678}, 12.8},
  {"Ruby",          {0.1745, 0.01175, 0.01175},  {0.61424, 0.04136, 0.04136},  {0.727811, 0.626959, 0.626959}, 76.8},
  {"Polished Gold", {0.24725, 0.2245, 0.0645},   {0.34615, 0.3143, 0.0903},    {0.797357, 0.723991, 0.208006}, 83.2},
  {"Polished Bronze", {0.25, 0.148, 0.06475},    {0.4, 0.2368, 0.1036},        {0.774597, 0.458561, 0.200621}, 76.8},
  {"Polished Copper", {0.2295, 0.08825, 0.0275}, {0.5508, 0.2118, 0.066},      {0.580594, 0.223257, 0.0695701}, 51.2},
  {"Jade",          {0.135, 0.2225, 0.1575},     {0.135, 0.2225, 0.1575},      {0.316228, 0.316228, 0.316228}, 12.8},
  {"Obsidian",      {0.05375, 0.05, 0.06625},    {0.18275, 0.17, 0.22525},     {0.332741, 0.328634, 0.346435}, 38.4},
  {"Pearl",         {0.25, 0.20725, 0.20725},    {1.0, 0.829, 0.829},          {0.296648, 0.296648, 0.296648}, 11.264},
  {"Emerald",       {0.0215, 0.1745, 0.0215},    {0.07568, 0.61424, 0.07568},  {0.633, 0.727811, 0.633},       76.8},
  {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},                32.0},
  {"Black Rubber",  {0.02, 0.02, 0.02},          {0.01, 0.01, 0.01},           {0.4, 0.4, 0.4},                10.0},
};
#define MAT_N (sizeof(MatLib) / sizeof(MatLib[0]))

/* Project namespace */
namespace ivrt
{
  /* Raytracer class */
  class raytracer : public win
  {
  public:
    scene Scene; 
    camera Cam;
    //timer T;
    frame Frame;  // Window frame
  private:
    std::thread Th[11];
  public:
    raytracer( VOID )
    {
    }
    ~raytracer( VOID )
    {
    }
   /* Render function.
    * ARGUMENTS: None.
    * RETURNS: None
    */
    VOID Render( VOID )
    {
      auto ThreadFunc = []( raytracer *RT, INT i )
      {
        intr I;
        vec3 L = vec3(1, 1, 1).Normalizing();
        vec3 color;
        envi Media(0.7, 0.3);
        for (INT y = i * (RT->Frame.Height / 11); y < (i + 1) * (RT->Frame.Height / 11); y++)
          for (INT x = 0; x < RT->Frame.Width; x++)
          {
            ray R = RT->Cam.FrameRay(x + 0.5, y + 0.5);
            color = RT->Scene.Trace(R, Media, 1.0, 0);

            RT->Frame.PutPixel(x, y, frame::ToRGB(color));
          }
      };
      for (INT i = 0; i < 11; i++)
        Th[i] = std::thread(ThreadFunc, this, i);

      for (INT i = 0; i < 11; i++)
        Th[i].join();

      InvalidateRect(hWnd, nullptr, TRUE);
    } /* End of 'Render' function */

    /* Initialization function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Init( VOID ) override
    {
      Frame.Resize(3840, 2160);
      Cam.Resize(3840, 2160);
      //Cam.Rotate(vec3(0, 1, 0), 30);
    } /* End of 'Init' function */
 
    /* Paint function.
     * ARGUMENTS: 
     *   - Handle device context:
     *       HDC hDC;
     * RETURNS: None.
     */
    VOID Paint( HDC hDC ) override
    {
      //SelectObject(hDC, GetStockObject(NULL_BRUSH));
      //Frame.Draw(hDC, 0, 0, 1);
      Frame.Draw(hDC, (win::W - Cam.FrameW) / 2 , (win::H - Cam.FrameH) / 2 , 1);
      //Rectangle(hDC, (W - Frame.Width) / 2 - 1, (H - Frame.Height) / 2 - 1, (W + Frame.Width) / 2 + 1, (H - Frame.Height) / 2 + 1);
    } /* End of 'Paint' function */

    /* Resizing function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Resize( VOID ) override
    {
      for (INT x = 0; x < W; x++)
        Frame.PutPixel(x, 0, RGB(0, 0, 255));
      for (INT y = 0; y < H; y++)
        Frame.PutPixel(0, y, RGB(0, 0, 255));
      for (INT x = 0; x < W; x++)
        Frame.PutPixel(x, H, RGB(0, 0, 255));
      for (INT y = 0; y < H; y++)
        Frame.PutPixel(W, y, RGB(0, 0, 255));
      //Cam.Resize(W, H);
      //Frame.Resize(W, H);
      Render();
      Frame.SaveTGA();
      //for (INT y = 0; y < H; y++)
      //  for (INT x = 0; x < W; x++)
      //    Frame.PutPixel(x, y, RGB(120 * sin(x), 10 * cos(y), 10));
      //FullScreenSaveRect.left = 0;
      //FullScreenSaveRect.right = W;
      //  FullScreenSaveRect.top = H;
      //FullScreenSaveRect.left = 0;

    } /* End of 'Resize' function */
    /* Timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Timer( VOID ) override
    {
      Render();
      Frame.SaveTGA();

      Cam.Rotate(vec3(0, 1, 0), 3);
    } /* End of 'Timer' function */
    /* Erase function.
     * ARGUMENTS: 
     *   - Handle device context:
     *       HDC hDC;
     * RETURNS: None.
     */
    VOID Erase( HDC hDC ) override
    {
      //Frame.Erase(hDC);
      //SelectObject(hDC, GetStockObject(NULL_PEN));
      //Rectangle(hDC, 0, 0, W, H);
    } /* End of 'Erase' function */
    /* Idle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Idle( VOID ) final
    {
      Render();
      Frame.SaveTGA();
      //Cam.Rotate(vec3(0, 1, 0), 10);
    } /* End of 'Idle' function */

  }; /* End of 'raytracer' class */
  /* WM_KEYDOWN/WM_KEYUP window message handle function.
   * ARGUMENTS:
   *   - virtual key number (see VK_***, 'A', ..., '0', ...):
   *       UINT Key;
   *   - press flag:
   *       BOOL IsPress;
   *   - autorepeat counter:
   *       INT RepeatCount;
   *   - shift/control etc key states bits (see MK_***):
   *       UINT ShiftKeysFlags;
   * RETURNS: None.
   */
  VOID ::ivrt::win::OnKey( UINT Key, BOOL IsPress,
                   INT RepeatCount, UINT ShiftKeysFlags )
  {
    //if (Key == 'W' && IsPress)
    //  raytracer::Cam.Move((((raytracer::Cam.Dir +
    //                         raytracer::Cam.Right * 30 * Ani->GlobalDeltaTime)));

  } /* End of 'win::OnKey' function */

} /* end of 'ivrt' namespace */

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       dg5PRIM *Pr;
 *   - primitive type:
 *       INT Type;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL PrimitiveLoad( std::vector<ivrt::vec3> &V, std::vector<INT> &I, const CHAR *FileName )
{
  INT
    noofv = 0,
    noofi = 0;
  FILE *F;
  CHAR Buf[1000];

  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertex and index quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      noofv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      noofi++;
  }
  I.resize(noofi * 3);
  V.resize(noofv);

  //Ind = (INT *)(V + noofv);

  /* Read vertices and facets data */
  rewind(F);
  noofv = noofi = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      FLT x, y, z;

      sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
      V[noofv++] = ivrt::vec3(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      /* Read one of possible facet references */
      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3);
      n1--;
      n2--;
      n3--;
      I[noofi++] = n1;
      I[noofi++] = n2;
      I[noofi++] = n3;
    }
  }

  fclose(F);

  return TRUE;
} /* End of 'PrimitiveLoad' function */

 /* The main program function.
  * ARGUMENTS:
  *   - handle of application instance:
  *       HINSTANCE hInstance;
  *   - dummy handle of previous application instance (not used):
  *       HINSTANCE hPrevInstance;
  *   - command line string:
  *       CHAR *CmdLine;
  *   - show window command parameter (see SW_***):
  *       INT CmdShow;
  * RETURNS:
  *   (INT) Error level for operation system (0 for success).
  */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  ivrt::raytracer MyNew;
  std::map<std::string, ivrt::surface> MtlTable;
  //ivrt::surface MtlTable[MAT_N];
  DBL Radius = 0.5;
  for (INT i = 0; i < MAT_N; i++)
  {
    MtlTable[MatLib[i].Name].Ka = MatLib[i].Ka;
    MtlTable[MatLib[i].Name].Kd = MatLib[i].Kd;
    MtlTable[MatLib[i].Name].Ks = MatLib[i].Ks;
    MtlTable[MatLib[i].Name].Kr = 0.4;
    MtlTable[MatLib[i].Name].Ph = MatLib[i].Ph;
    MyNew.Scene << new ivrt::sphere(ivrt::vec3(1 * (i % 4), 2 * Radius * (i / 4), 0), Radius, 
                                     MtlTable[MatLib[i].Name]);
  }
  MyNew.Scene << new ivrt::point(ivrt::vec3(5, 10, 5), ivrt::vec3(1, 1, 1), 10, 20) <<
                 new ivrt::plane(ivrt::vec3(0, 1, 0), 0) <<
                 new ivrt::point(ivrt::vec3(-5, 10, -5), ivrt::vec3(1, 1, 1), 10, 20);
  std::vector<ivrt::vec3> Res_V;
  std::vector<INT> Res_I;
  //PrimitiveLoad(Res_V, Res_I, "bin/models/cow.object");

  INT V;
  //for (INT i = 0; i < Res_I.size() - 3; i += 3)
  //  MyNew.Scene << new ivrt::triangle(Res_V[Res_I[i]], Res_V[Res_I[i + 1]], Res_V[Res_I[i + 2]]);

  //ivrt::vec3 p(120, 13, 4);
  //FLT x = p.Distance(p);
  /*
  MyNew.Scene << new ivrt::sphere(ivrt::vec3(-1, 1, -1), 1) <<
                 new ivrt::sphere(ivrt::vec3(0, 1, 3), 1) << 
                 new ivrt::sphere(ivrt::vec3(3, 1, 4), 1) << 
                 new ivrt::plane(ivrt::vec3(0, 1, 0), 0) << 
                 new ivrt::point(ivrt::vec3(4, 10, -4), ivrt::vec3(1, 1, 1), 10, 20) <<
                 new ivrt::point(ivrt::vec3(-4, 10, -4), ivrt::vec3(1, 1, 1), 10, 20) <<
                 new ivrt::point(ivrt::vec3(4, 10, -4), ivrt::vec3(1, 1, 1), 10, 20) <<
                 new ivrt::box(ivrt::vec3(0, 0, 0), ivrt::vec3(2, 2, 2)); 
  */
  /*new ivrt::box(ivrt::vec3(0, 1, 0), ivrt::vec3(5, 3, 5)); */

  //MyNew.Scene << new ivrt::plane(ivrt::vec3(0, 0, 0), 0);
  //std::cout << p; 
  //getchar();
  //MyNew.Scene << new ivrt::box(ivrt::vec3(0, 0, 0), ivrt::vec3(1, 1, 1)); 
  //v << "";

  return MyNew.Run();
} /* End of 'WinMain' function */


/* END OF 'main.cpp' FILE */