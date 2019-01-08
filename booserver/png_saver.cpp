#include "png_saver.h"
#include <png.h>
#include <memory>

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

} // namespace png
