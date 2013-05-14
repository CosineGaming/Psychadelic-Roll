#pragma once
#include <stdlib.h>
#include <stdio.h>

/* Image type (data_type)
 *
 * CMI   - "Color Mapped Image"
 * RGB   - "Red, Green, and Blue"
 * BW    - "Black and White"
 * RLE   - "Run Length Encoded"
 * HDRLE - "Huffman, Delta, and Run Length Encoded"
 * QUAD  - "4 pass quadtree-type process"
 */
#define TGA_NONE 0
#define TGA_UNCOMPRESSED_CMI 1
#define TGA_UNCOMPRESSED_RGB 2
#define TGA_UNCOMPRESSED_BW 3
#define TGA_RLE_CMI 9
#define TGA_RLE_RGB 10
#define TGA_COMPRESSED_BW 11
#define TGA_COMPRESSED_CMI_HDRLE 32
#define TGA_COMPRESSED_CMI_HDRLE_QUAD 33

struct tga_header
{
  unsigned char  id_length;
  unsigned char  colour_map_type;
  unsigned char  data_type;
  unsigned short colour_map_origin;
  unsigned short colour_map_length;
  unsigned char  colour_map_depth;
  unsigned short x_origin;
  unsigned short y_origin;
  unsigned short width;
  unsigned short height;
  unsigned char  bits_per_pixel;
  unsigned char  image_descriptor;
};

unsigned char *rgb_tga(char *filename, int *w, int *h)
{
  FILE *file = fopen(filename, "rb");
  if (file == NULL)
  {
      printf("Could not open the file: %s\n", filename);
      return NULL;
  }

  tga_header header;

  //read all 18 bytes of the header
  fread(&header, sizeof(tga_header), 1, file);

  //should be image type 2 (color) or type 10 (rle compressed color)
  if (header.data_type != TGA_UNCOMPRESSED_RGB && header.data_type != TGA_RLE_RGB)
  {
      fclose(file);
      exit(0);
  }

  if (header.id_length)
  {
      fseek(file, header.id_length, SEEK_CUR);
  }

  // get the size and bitdepth from the header
  int m_width = header.width;
  int m_height = header.height;
  int m_bpp = header.bits_per_pixel / 8;
  *w = m_width;
  *h = m_height;

  if (m_bpp != 3 && m_bpp != 4)
  {
      fclose(file);
      exit(0);
  }

  int imageSize = m_width * m_height * m_bpp;

  //allocate memory for image data
  unsigned char *data = new unsigned char[imageSize];

  //read the uncompressed image data if type 2
  if (header.data_type == TGA_UNCOMPRESSED_RGB)
  {
      fread(data, sizeof(char), imageSize, file);
  }

  long ctpixel = 0,
    ctloop = 0;

  //read the compressed image data if type 10
  if (header.data_type == TGA_RLE_RGB)
  {
      // stores the rle header and the temp color data
      unsigned char rle;
      unsigned char color[4];

      while (ctpixel < imageSize)
      {
	  // reads the the RLE header
	  fread(&rle, 1, 1, file);

	  // if the rle header is below 128 it means that what folows is just raw data with rle+1 pixels
	  if (rle < 128)
	  {
	      fread(&data[ctpixel], m_bpp, rle+1, file);
	      ctpixel += m_bpp*(rle+1);
	  }

	  // if the rle header is equal or above 128 it means that we have a string of rle-127 pixels
	  // that use the folowing pixels color
	  else
	  {
	      // read what color we should use
	      fread(&color[0], 1, m_bpp, file);

	      // insert the color stored in tmp into the folowing rle-127 pixels
	      ctloop = 0;
	      while (ctloop < (rle-127))
	      {
		  data[ctpixel] = color[0];
		  data[ctpixel+1] = color[1];
		  data[ctpixel+2] = color[2];
		  if (m_bpp == 4)
		  {
		      data[ctpixel+3] = color[3];
		  }

		  ctpixel += m_bpp;
		  ctloop++;
	      }
	  }
      }
  }

  ctpixel=0;

  //Because TGA file store their colors in BGRA format we need to swap the red and blue color components
  unsigned char temp;
  while (ctpixel < imageSize)
  {
      temp = data[ctpixel];
      data[ctpixel] = data[ctpixel+2];
      data[ctpixel+2] = temp;
      ctpixel += m_bpp;
  }

  //close file
  fclose(file);

  return data;
}
