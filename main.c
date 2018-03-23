#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> red
    // 1 -> green
    // 2 -> blue
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;


int max(int a, int blue) {
    if (a > blue)
        return a;
    return blue;
}

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.red == p2.red &&
        p1.green == p2.green &&
        p1.blue == p2.blue)
        return 1;
    return 0;
}


Image gray_scale(Image image) {
    /*for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
        }
    }*/

    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            int average = image.pixel[i][j][0] +
                        image.pixel[i][j][1] +
                        image.pixel[i][j][2];
            average /= 3;
            image.pixel[i][j][0] = average;
            image.pixel[i][j][1] = average;
            image.pixel[i][j][2] = average;
        }
    }

    return image;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int T, unsigned int width) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel average = {0, 0, 0};

            int smaller_height = (height - 1 > i + T/2) ? i + T/2 : height - 1;
            int min_width = (width - 1 > j + T/2) ? j + T/2 : width - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= smaller_height; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_width; ++y) {
                    average.red += pixel[x][y][0];
                    average.green += pixel[x][y][1];
                    average.blue += pixel[x][y][2];
                }
            }

            // printf("%u", average.red)
            average.red /= T * T;
            average.green /= T * T;
            average.blue /= T * T;

            pixel[i][j][0] = average.red;
            pixel[i][j][1] = average.green;
            pixel[i][j][2] = average.blue;
        }
    }
}

Image rotate_90_right(Image image) {
    Image rotated;

    rotated.width = image.height;
    rotated.height = image.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j][0] = image.pixel[x][y][0];
            rotated.pixel[i][j][1] = image.pixel[x][y][1];
            rotated.pixel[i][j][2] = image.pixel[x][y][2];
        }
    }

    return rotated;
}

void invert_colors(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image cut_image(Image image, int x, int y, int width, int height) {
    Image cutted;

    cutted.width = width;
    cutted.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cutted.pixel[i][j][0] = image.pixel[i + y][j + x][0];
            cutted.pixel[i][j][1] = image.pixel[i + y][j + x][1];
            cutted.pixel[i][j][2] = image.pixel[i + y][j + x][2];
        }
    }

    return cutted;
}


int main() {
    Image image;

    // read type of image
    char type_of_image[4];
    scanf("%s", type_of_image);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &image.width, &image.height, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            scanf("%hu %hu %hu", &image.pixel[i][j][0],
                                 &image.pixel[i][j][1],
                                 &image.pixel[i][j][2]);

        }
    }

    int n_options;
    scanf("%d", &n_options);

    for(int i = 0; i < n_options; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Escala de Cinza
                image = gray_scale(image);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < image.height; ++x) {
                    for (unsigned int j = 0; j < image.width; ++j) {
                        unsigned short int pixel[3];
                        pixel[0] = image.pixel[x][j][0];
                        pixel[1] = image.pixel[x][j][1];
                        pixel[2] = image.pixel[x][j][2];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int smaller_r = (255 >  p) ? p : 255;
                        image.pixel[x][j][0] = smaller_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        smaller_r = (255 >  p) ? p : 255;
                        image.pixel[x][j][1] = smaller_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        smaller_r = (255 >  p) ? p : 255;
                        image.pixel[x][j][2] = smaller_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int size = 0;
                scanf("%d", &size);
                blur(image.height, image.pixel, size, image.width);
                break;
            }
            case 4: { // Rotacao
                int how_many = 0;
                scanf("%d", &how_many);
                how_many %= 4;
                for (int j = 0; j < how_many; ++j) {
                    image = rotate_90_right(image);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int width = image.width, height = image.height;

                if (horizontal == 1) width /= 2;
                else height /= 2;

                for (int i2 = 0; i2 < height; ++i2) {
                    for (int j = 0; j < width; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = image.width - 1 - j;
                        else x = image.height - 1 - i2;

                        Pixel aux1;
                        aux1.red = image.pixel[i2][j][0];
                        aux1.green = image.pixel[i2][j][1];
                        aux1.blue = image.pixel[i2][j][2];

                        image.pixel[i2][j][0] = image.pixel[x][y][0];
                        image.pixel[i2][j][1] = image.pixel[x][y][1];
                        image.pixel[i2][j][2] = image.pixel[x][y][2];

                        image.pixel[x][y][0] = aux1.red;
                        image.pixel[x][y][1] = aux1.green;
                        image.pixel[x][y][2] = aux1.blue;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                invert_colors(image.pixel, image.width, image.height);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                image = cut_image(image, x, y, width, height);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", image.width, image.height);

    // print pixels of image
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            printf("%hu %hu %hu ", image.pixel[i][j][0],
                                   image.pixel[i][j][1],
                                   image.pixel[i][j][2]);

        }
        printf("\n");
    }
    return 0;
}
