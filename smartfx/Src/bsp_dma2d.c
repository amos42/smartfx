#include "bsp_dma2d.h"

#define LCD_CurrentFrameBuffer 	0xc0000000
#define LCD_MAXX									320
#define LCD_MAXY									480


void UB_Graphic2D_CopyImgDMA(UB_Image *img, DMA2D_Coord coord)
{
	uint32_t  dest_address = 0;
	uint32_t  source_address = 0;
	uint32_t offset;
	uint32_t  picture_width;
	uint32_t  picture_height;
	uint32_t w,h;

	// Destination address in the display RAM
	dest_address = LCD_CurrentFrameBuffer + 2 * (LCD_MAXX * coord.dest_yp + coord.dest_xp);

	picture_width = img->width;
	picture_height = img->height;

	// check to Limit
	if (coord.source_w == 0) return;
	if (coord.source_h == 0) return;
	if (coord.source_xp + coord.source_w > picture_width) return;
	if (coord.source_yp + coord.source_h > picture_height) return;
	if (coord.dest_xp + coord.source_w > LCD_MAXX) return;
	if (coord.dest_yp + coord.source_h > LCD_MAXY) return;

	//  Source address from the image
	offset = (picture_width * coord.source_yp + coord.source_xp);
	source_address  = (uint32_t)&img->table[offset];

	w = coord.source_w;
	h = coord.source_h;

	hdma2d.Init.Mode					= DMA2D_M2M;
	hdma2d.Init.ColorMode		= DMA2D_RGB565;
	hdma2d.Init.OutputOffset		= LCD_MAXX - w;

	// bug workaround (layer-0 bug)
	// set layer always to 1
	hdma2d.LayerCfg[1].AlphaMode 				= DMA2D_NO_MODIF_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha 				= 0xFF;
	hdma2d.LayerCfg[1].InputColorMode 		= CM_RGB565;
	hdma2d.LayerCfg[1].InputOffset 				= picture_width-coord.source_w;
	hdma2d.Instance = DMA2D;

	// DMA2D Initialization
	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK) {
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) {
			if (HAL_DMA2D_Start(&hdma2d, source_address, dest_address, w, h) == HAL_OK)
			{        
				HAL_DMA2D_PollForTransfer(&hdma2d, 10);
			}
		}
	}
}


void UB_Graphic2D_CopyImgDMA2(unsigned short *addr, int sx, int sy, int width, int height )
{
	volatile uint32_t	dest_address = 0;
	volatile uint32_t	source_address = 0;

	// Destination address in the display RAM
	dest_address = LCD_CurrentFrameBuffer + 2 * (LCD_MAXX * sy + sx);

	// check to Limit
	if (width == 0) return;
	if (height == 0) return;
	if (sx + width > LCD_MAXX) return;
	if (sy + height > LCD_MAXY) return;

	//  Source address from the image
	source_address  = (uint32_t)&addr[0] + 2 * (LCD_MAXX * sy + sx); //(uint32_t)&addr[0];

	hdma2d.Init.Mode					= DMA2D_M2M;
	hdma2d.Init.ColorMode		= DMA2D_RGB565;
	hdma2d.Init.OutputOffset		= LCD_MAXX - width;

	// bug workaround (layer-0 bug)
	// set layer always to 1
	hdma2d.LayerCfg[1].AlphaMode 				= DMA2D_NO_MODIF_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha 				= 0xFF;
	hdma2d.LayerCfg[1].InputColorMode 		= CM_RGB565;
	hdma2d.LayerCfg[1].InputOffset 				= LCD_MAXX - width;; //0; //picture_width-coord.source_w;
	hdma2d.Instance = DMA2D;

	// DMA2D Initialization
	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK) {
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) {
			if (HAL_DMA2D_Start(&hdma2d, source_address, dest_address, width, height) == HAL_OK)
			{        
				HAL_DMA2D_PollForTransfer(&hdma2d, 10);
			}
		}
	}
}
