#ifndef INCLUDE_SRC_TEXTURE_H
#define INCLUDE_SRC_TEXTURE_H

#include <stdbool.h>

/*
 * TEXTURE STRUCT
 * We'll need to store the width, height, and number of channels
 * We'll need to store the id
 *
 * FUNCTIONS
 * Loading in texture
 * changing texture parameters
 *      - bind texture
 *      - unbind after (!)
 * Being fat
 */

struct Texture {
  unsigned int id;
  int width;
  int height;
  int num_channels;
};

bool n_init_texture(struct Texture *texture, const char *path);
void n_texture_bind(const struct Texture *texture, unsigned int unit);
void n_texture_destroy(struct Texture *texture);

#endif
