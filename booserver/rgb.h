#pragma once

namespace image {
  #pragma pack(push, 1)
  typedef struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
  } RGB;
  #pragma pack(pop)
  
  inline bool operator== (const RGB &lhs, const RGB &rhs) {
    return lhs.r == rhs.r
      && lhs.g == rhs.g
      && lhs.b == rhs.b; 
  }
}
