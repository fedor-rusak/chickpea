#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace freetype {

	void testFTloadCharBitmap(char letter, int *resultWidth, int *resultHeight, unsigned char** resultData) {
		// Initialize FreeType.
		FT_Library library;
		FT_Face face;
		char* fontName = "resources/OpenSans-Bold.ttf";

		FT_Init_FreeType(&library);

		if(FT_New_Face(library, fontName, 0, &face)) {
			printf("Could not load font: ");
			printf("%s", fontName);
		}

		int err = FT_Set_Char_Size(
			face,    /* handle to face object           */
			0,       /* char_width in 1/64th of points  */
			16*64,   /* char_height in 1/64th of points */
			300,     /* horizontal device resolution    */
			300 );   /* vertical device resolution      */

		if(err){printf("FAIL1");}
		err = FT_Set_Pixel_Sizes(
			face,   /* handle to face object */
			0,      /* pixel_width           */
			250 );   /* pixel_height          */
		if(err){printf("FAIL2");}

		FT_GlyphSlot  slot;

		slot = face->glyph;
		FT_Load_Char(face, letter, FT_LOAD_RENDER);

		FT_Bitmap bitmap2 = face->glyph->bitmap;

		int size = bitmap2.width*bitmap2.rows;

		unsigned char* someCharData = (unsigned char*) malloc(size*4);
		for (int i = 0; i < size; i++) {
			someCharData[i*4] = bitmap2.buffer[i];
			someCharData[i*4+1] = bitmap2.buffer[i];
			someCharData[i*4+2] = bitmap2.buffer[i];
			someCharData[i*4+3] = bitmap2.buffer[i];
		}


		*resultWidth = bitmap2.width;
		*resultHeight = bitmap2.rows;

		*resultData = someCharData;

	  	FT_Done_FreeType(library);
	}

}
