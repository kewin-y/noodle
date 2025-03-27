#include "texture.h"
#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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

void init_texture(struct Texture *texture, const char *path)
{
        unsigned char *data = NULL;
        GLenum format;

        if (NULL == texture) {
                printf("Error in function init_texture(): "
                       "texture is null");
                exit(1);
        }

        glGenTextures(1, &texture->id);
        glBindTexture(GL_TEXTURE_2D, texture->id);

        /* TEXTURE WRAPPING */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        /* TEXTURE FILTERING */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        /* LOAD TEXTURE */
        data = stbi_load(path, &texture->width, &texture->height,
                         &texture->num_channels, 0);
        if (NULL == data) {
                printf("Error in function init_texture():"
                       "failed to load texture\n");
                exit(1);
        }

        format = get_texture_format(texture->num_channels);

        if (0 == format) {
                printf("Error in function init_texture(): "
                       "Invalid texture format. Are you sure that your texture "
                       "has a valid number of channels?\n");
                exit(1);
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
}
