#ifndef __bsp_dma2d_H
#define __bsp_dma2d_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "dma2d.h"


typedef struct UB_Image_t
{
  const uint16_t *table; // Tabelle mit den Daten
  uint16_t width;        // Breite des Bildes (in Pixel)
  uint16_t height;       // Hoehe des Bildes  (in Pixel)
}UB_Image;



//--------------------------------------------------------------
// Struktur von einer Kopie-Koordinate
//--------------------------------------------------------------
typedef struct DMA2D_Koord_t {
  uint32_t source_xp; // Quelle X-Start
  uint32_t source_yp; // Quelle Y-Start
  uint32_t source_w;  // Quelle Breite
  uint32_t source_h;  // Quelle H?e
  uint32_t dest_xp;   // Ziel X-Start
  uint32_t dest_yp;   // Ziel Y-Start
}DMA2D_Coord;

void UB_Graphic2D_CopyImgDMA(UB_Image *img, DMA2D_Coord coord);
void UB_Graphic2D_CopyImgDMA2(unsigned short *addr, int sx, int sy, int width, int height );

#ifdef __cplusplus
}
#endif
#endif /*__ bsp_dma2d_H */
