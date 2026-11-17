#ifndef NV_GL_COMPAT_H
#define NV_GL_COMPAT_H

  #include <GLES2/gl2.h>
  #define NV_GLSL_PREFIXO "precision mediump float;\n"

#include "perftv.h"
// Count submitted pixel uploads (including text/mip levels), not allocations.
// Inline wrappers leave only a flag branch when measurement is off.
static inline void nvPerfTexImage2D(GLenum target, GLint level, GLint internal,
    GLsizei w, GLsizei h, GLint border, GLenum format, GLenum type, const void *pixels) {
  if (perftv_enabled && pixels) perftv_uploads++;
  glTexImage2D(target, level, internal, w, h, border, format, type, pixels);
}
static inline void nvPerfTexSubImage2D(GLenum target, GLint level, GLint x, GLint y,
    GLsizei w, GLsizei h, GLenum format, GLenum type, const void *pixels) {
  if (perftv_enabled && pixels) perftv_uploads++;
  glTexSubImage2D(target, level, x, y, w, h, format, type, pixels);
}
#define glTexImage2D nvPerfTexImage2D
#define glTexSubImage2D nvPerfTexSubImage2D

#endif
