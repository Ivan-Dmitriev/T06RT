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

#include "def.h"
#include "win/win.h"
#include "rt/frame/frame.h"
#include "rt/rt.h"
#include "rt/rt_def.h"
#include "rt/lights/point.h"

/* Project namespace */
namespace ivrt
{
  /* Raytracer class */
  class raytracer : public win
  {
  public:
    scene Scene; 
    camera Cam;
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
      Cam.Resize(500, 300);
      Cam.Rotate(vec3(0, 1, 0), 30);
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
      Cam.Resize(W, H);
      Frame.Resize(W, H);
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
      Cam.Rotate(vec3(0, 1, 0), 1);
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
      //Cam.Rotate(vec3(0, 1, 0), 10);
    } /* End of 'Idle' function */

  }; /* End of 'raytracer' class */
} /* end of 'ivrt' namespace */

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

  //ivrt::vec3 p(120, 13, 4);
  //FLT x = p.Distance(p);
  MyNew.Scene << new ivrt::sphere(ivrt::vec3(-1, 1, -1), 1) <<
                 new ivrt::sphere(ivrt::vec3(0, 1, 3), 1) << 
                 new ivrt::sphere(ivrt::vec3(3, 1, 4), 1) << 
                 new ivrt::plane(ivrt::vec3(0, 1, 0), 0) << 
                 new ivrt::point(ivrt::vec3(4, 10, -4), ivrt::vec3(1, 1, 1), 10, 20) <<
                 new ivrt::point(ivrt::vec3(-4, 10, -4), ivrt::vec3(1, 1, 1), 10, 20) <<
                 new ivrt::point(ivrt::vec3(4, 10, -4), ivrt::vec3(1, 1, 1), 10, 20) <<
                 new ivrt::box(ivrt::vec3(0, 0, 0), ivrt::vec3(2, 2, 2)); 

  /*new ivrt::box(ivrt::vec3(0, 1, 0), ivrt::vec3(5, 3, 5)); */

  //MyNew.Scene << new ivrt::plane(ivrt::vec3(0, 0, 0), 0);
  //std::cout << p; 
  //getchar();
  //MyNew.Scene << new ivrt::box(ivrt::vec3(0, 0, 0), ivrt::vec3(1, 1, 1)); 
  //v << "";

  return MyNew.Run();
} /* End of 'WinMain' function */


/* END OF 'main.cpp' FILE */