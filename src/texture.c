#include "texture.h"
#include "glad/glad.h"
#include <stdio.h>

#include <stb_image.h>

static GLenum get_texture_format(int num_channels)
{
  switch (num_channels) {
  case 1:
    return GL_RED;
  case 2:
    return GL_RG;
  case 3:
    return GL_RGB;
  case 4:
    return GL_RGBA;
  default:
    return 0;
  }
}

bool n_init_texture(struct Texture *texture, const char *path)
{
  unsigned char *data = NULL;
  GLenum format;

  if (NULL == texture) {
    printf("Error in function n_init_texture(): "
           "texture is null\n");
    return false;
  }

  texture->id = 0;

  glGenTextures(1, &texture->id);
  glBindTexture(GL_TEXTURE_2D, texture->id);

  /* TEXTURE WRAPPING */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  /* TEXTURE FILTERING */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  /* LOAD TEXTURE */
  data = stbi_load(path, &texture->width, &texture->height, &texture->num_channels, 0);
  if (NULL == data) {
    printf("n_Error in function init_texture(): "
           "Failed to load texture.\n");
    n_texture_destroy(texture);
    return false;
  }

  format = get_texture_format(texture->num_channels);

  if (0 == format) {
    printf("Error in function init_texture(): "
           "Invalid texture format. Are you sure that your texture "
           "has a valid number of channels?\n");
    stbi_image_free(data);
    n_texture_destroy(texture);
    return false;
  }

  /* GENERATE MIPMAPS */
  glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height,
               0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  /* FREE */
  stbi_image_free(data);

  /* UNBIND */
  glBindTexture(GL_TEXTURE_2D, 0);
  printf("Finished initializing texture with path: %s\n", path);
  printf("Texture: id: %d, width: %d, height: %d, num_channels: %d\n",
         texture->id, texture->width, texture->height,
         texture->num_channels);
  return true;
}

void n_texture_bind(const struct Texture *texture, unsigned int unit)
{
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

void n_texture_destroy(struct Texture *texture)
{
  if (texture != NULL && texture->id != 0) {
    glDeleteTextures(1, &texture->id);
    texture->id = 0;
  }
}
