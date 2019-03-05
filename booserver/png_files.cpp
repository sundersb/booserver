#include "png_files.h"
#include <png.h>
#include <memory>
#include <cstring>

namespace png {

const int BPP = 3;

void fcloser(FILE* file) {
    if (file) fclose(file);
}

void pngDestructor(png_structp png) {
  if (png) png_destroy_write_struct(&png, nullptr);
}

bool toPNG (const std::string& fileName, image::Canvas &canvas) {
  FILE* fp = fopen(fileName.c_str(), "wb");
  if (!fp) return false;
  std::shared_ptr<FILE> file (fp, fcloser);

  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr) return false;
  std::shared_ptr<png_struct> pguard (png_ptr, pngDestructor);
    
  png_infop pinfo = png_create_info_struct(png_ptr);
  if (!pinfo) return false;

  if (setjmp(png_jmpbuf(png_ptr))) return false;
  
  png_init_io(png_ptr, file.get());
    
  png_set_IHDR(png_ptr,
    pinfo,
    canvas.getWidth(),
    canvas.getHeight(),
    8,
    PNG_COLOR_TYPE_RGB,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT);

  unsigned char *ptr = canvas.getPixels();
  unsigned char *rows[canvas.getHeight()];
  int stride = canvas.getWidth() * BPP;

  for (int i = 0; i < canvas.getHeight(); ++i) {
    rows[i] = ptr;
    ptr += stride;
  }

  png_set_rows(png_ptr, pinfo, rows);
  png_write_png(png_ptr, pinfo, PNG_TRANSFORM_IDENTITY, nullptr);
  png_write_end(png_ptr, pinfo);
  
  return true;
}

bool drawPNG (const std::string& fileName, image::Canvas &canvas) {
  FILE* fp = fopen(fileName.c_str(), "rb");
  if (!fp) return false;
  std::shared_ptr<FILE> file (fp, fcloser);
  
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr) return false;
  std::shared_ptr<png_struct> pguard (png_ptr, pngDestructor);
    
  png_infop pinfo = png_create_info_struct(png_ptr);
  if (!pinfo) return false;
  
  if (setjmp(png_jmpbuf(png_ptr))) return false;
  png_init_io(png_ptr, file.get());

  png_read_png(png_ptr, pinfo, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_SWAP_ALPHA | PNG_TRANSFORM_EXPAND, NULL);

  png_uint_32 width = png_get_image_width(png_ptr, pinfo);
  png_uint_32 height = png_get_image_height(png_ptr, pinfo);
    
  if (png_get_bit_depth(png_ptr, pinfo) != 8) return false;
  if (png_get_channels(png_ptr, pinfo) != 3) return false;
  if (png_get_color_type(png_ptr, pinfo) != 2) return false;
  
  if (canvas.getHeight() != (int) height
    || canvas.getWidth() != (int) width) return false;
  
  png_bytepp row_pointers = png_get_rows(png_ptr, pinfo);
  
  unsigned char *pix = const_cast<unsigned char*>(canvas.getPixels());
  int stride = width * 3;
  
  for (unsigned int i=0; i < height; ++i) {
    memcpy(pix, row_pointers[i], stride);
    pix += stride;
  }
  
  return true;
}

} // namespace png
