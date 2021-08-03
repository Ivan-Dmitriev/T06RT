/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : frame.h
 * PURPOSE     : Raytracing project.
 *               Frame class declaration module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Ivan Dmitriev
 * LAST UPDATE : 02.08.2021
 * NOTE        : Module namespace 'ivrt'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __frame_h_
#define __frame_h_

#include <fstream>

#pragma pack(push, 1)
#include <TGAHEAD.H>
#pragma pack(pop)

#include "../../def.h"

/* Useful macro function */
#define Clamp(A) (A) > 1 ? 255 : (A) < 0 ? 0 : ((A) * 255)

/* Useful macro function */
//#define Clamp(A) ((A) > 1 ? 1.0f : (A) < 0 ? 0.0f : (A));

/* Project namespace */
namespace ivrt
{
  /* Frame class */
  class frame
  {
  protected:
    DWORD *Pixels;             // Frame buffer pixels
  public:
    INT Width = 0, Height = 0; // Frame size

  public:
    frame( VOID )
    {
      Pixels = nullptr;
    } /* End of 'frame' function */

    ~frame( VOID )
    {
      if (Pixels != nullptr)
        delete [] Pixels;
    } /* End of '~frame' function */

    /* Resize frame function.
     * ARGUMENTS: 
     *   - new frame size:
     *       INT W, H;
     * RETURNS: None.
     */
    VOID Resize( INT W, INT H )
    {
      if (Pixels != nullptr)
        delete [] Pixels;
      Pixels = new DWORD[W * H];
      //memset(Pixels, 0, sizeof(DWORD) * W * H);
      Width = W;
      Height = H;
    } /* End of 'Resize' function */

    /* Put pixel on frame function.
     * ARGUMENTS: 
     *   - coords on screen:
     *       INT X, Y;
     *   - color of pixel:
     *       DWORD Color;
     * RETURNS: None.
     */
    VOID PutPixel( INT X, INT Y, DWORD Color)
    {
      if (X < 0 || Y < 0 || X >= Width || Y >= Height)
        return;
      Pixels[Y * Width + X] = Color;
    } /* End of 'PutPixel' function */

    /* Draw frame function.
     * ARGUMENTS: 
     *   - handle device context:
     *       HDC hDC;
     *   - start point of drawing:
     *       INT X, INT Y;
     *   - streching parameter:
     *       INT Stretch;
     * RETURNS: None.
     */
    VOID Draw( HDC hDC, INT X, INT Y, INT Stretch )
    {
      BITMAPINFOHEADER bih;
      bih.biSize = sizeof(BITMAPINFOHEADER);
      bih.biBitCount = 32;
      bih.biPlanes = 1;
      bih.biWidth = Width;
      bih.biHeight = -Height;
      bih.biSizeImage = Width * Height * 4;
      bih.biCompression = BI_RGB;
      bih.biClrUsed = 0;
      bih.biXPelsPerMeter = 30;
      bih.biYPelsPerMeter = 30;

      //SelectObject(hDC, GetStockObject(BLACK_PEN));
      //Rectangle(hDC, 0, 0, Width, Height);

      SetStretchBltMode(hDC, COLORONCOLOR);
      StretchDIBits(hDC, X, Y, Width * Stretch, Height * Stretch, 0, 0, Width, Height, Pixels,
                     (BITMAPINFO *)&bih, DIB_RGB_COLORS, SRCCOPY);
    } /* End of 'Draw' function */

    /* Convert from floats to dword rgb function.
     * ARGUMENTS: 
     *   - color components:
     *       FLT R, G, B;
     * RETURNS: (DWORD) result value.
     */
    static DWORD ToRGB( FLT R, FLT G, FLT B )
    {
      //return Clamp(R) << 16 | Clamp(G) << 8 | Clamp(B);
      //return ((COLORREF)(((BYTE)(Clamp(R)) | ((WORD)((BYTE)(Clamp(G)))<<8)) | (((DWORD)(BYTE)(Clamp(B)))<<16)));
      //return (BYTE)((Clamp(R)) << 16))| (BYTE)(Clamp(G)))<< 8 ))|((BYTE)(Clamp(B)))))
      //return ((COLORREF)((BYTE)(Clamp(R)) << 16) | Clamp(G) << 8 | Clamp(B)));
      DWORD Color = RGB(Clamp(B), Clamp(G), Clamp(R));
      return Color;
    } /* End of 'ToRGB' function */

    /* Convert from floats to dword rgb function.
     * ARGUMENTS: 
     *   - color components:
     *       vec3 Color;
     * RETURNS: (DWORD) result value.
     */
    static DWORD ToRGB( vec3 Color )
    {
      //return Clamp(R) << 16 | Clamp(G) << 8 | Clamp(B);
      //return ((COLORREF)(((BYTE)(Clamp(R)) | ((WORD)((BYTE)(Clamp(G)))<<8)) | (((DWORD)(BYTE)(Clamp(B)))<<16)));
      //return (BYTE)((Clamp(R)) << 16))| (BYTE)(Clamp(G)))<< 8 ))|((BYTE)(Clamp(B)))))
      //return ((COLORREF)((BYTE)(Clamp(R)) << 16) | Clamp(G) << 8 | Clamp(B)));
      //DWORD Color = RGB(Clamp(Color[0]), Clamp(Color[1]), Clamp(Color[2]));
      //return Color;
    } /* End of 'ToRGB' function */

    /* Erase function.
     * ARGUMENTS: 
     *   - Handle device context:
     *       HDC hDC;
     * RETURNS: None.
     */
    VOID Erase( HDC hDC )
    {
      SelectObject(hDC, GetStockObject(NULL_PEN));
      Rectangle(hDC, 0, 0, Width, Height);
    } /* End of 'Erase' function */

    /* Save image to TGA file function.
     * ARGUMENTS: NONE.
     * RETURNS:
     *   (BOOL) TRUE if ok, FALSE otherwise.
     */
    BOOL SaveTGA( VOID )
    {
      std::string Addon = "CGSG_FOREVER";

      std::string FileName; 
      SYSTEMTIME st;
      GetLocalTime(&st);
      FileName = "bin/shots/ID3_RES_RT_" +
        std::to_string(st.wYear) + "_" +
        std::to_string(st.wMonth) + "_" +
        std::to_string(st.wDay) + "_" +
        std::to_string(st.wHour) + "_" +
        std::to_string(st.wMinute) + "_" +
        std::to_string(st.wSecond) + "_" +
        std::to_string(st.wMilliseconds) + ".tga";

      std::fstream f(FileName, std::fstream::out /*| std::fstream::binary*/);
      tgaFILEHEADER fh;
      tgaFILEFOOTER ff;
      tgaEXTHEADER eh;
      fh.IDLength = 0;
      fh.ColorMapType = 0;
      fh.ImageType = 2;
      fh.X = 0;
      fh.Y = 0;
      fh.Width = Width;
      fh.Height = Height;
      fh.BitsPerPixel = 24;
      fh.ImageDescr = 32;
      
      ff.DeveloperOffset = 0;
      ff.ExtensionOffset = 0;
      
      strcpy(ff.Signature, TGA_EXT_SIGNATURE);
      strcpy(eh.AuthorName, "Ivan Dmitriev");
      strcpy(eh.AuthorComment, "Ray Tracing project");

      eh.VersionLetter = 'A';
      eh.VersionNumber = 2;
      strcpy(eh.SoftwareID, "Cgsg foreve");

      if (!f.is_open())
        return FALSE;

      f.write((CHAR *)&fh, sizeof(tgaFILEHEADER));
      //f.write((CHAR *)&Addon, sizeof(Addon));
      for (INT y = 0; y < Height; y++)
        for (INT x = 0; x < Width; x++)
          f.write((CHAR *)&Pixels[y * Width + x], 3);
      //f.write((CHAR *)&eh, sizeof(tgaEXTHEADER));

      f.write((CHAR *)&ff, sizeof(tgaFILEFOOTER));
     return TRUE;
    } /* End of 'SaveTGA' function */
  }; /* End of 'Erase' function */
}
#endif /* __frame_h_ */

/* END OF 'frame.h' FILE */