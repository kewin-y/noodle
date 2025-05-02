#ifndef INCLUDE_SRC_TEXTURE_H_
#define INCLUDE_SRC_TEXTURE_H_

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

void n_init_texture(struct Texture *texture, const char *path);

#endif // INCLUDE_SRC_TEXTURE_H_
