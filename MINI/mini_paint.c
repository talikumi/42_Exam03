#include "mini_paint.h"

int ft_strlen(const char *s) {
    int i;
    for (i = 0; s[i]; i++){};
    return i;
}

int err(const char *s) {
    write(1, s, ft_strlen(s));
    return 1;
}

int check(float x, float y, float xtop, float ytop, float radius) {
    float dist = sqrtf(powf(x - xtop, 2.0) + powf(y - ytop, 2.0));
    if (dist <= radius) {
        if ((radius - dist) < 1.0)
            return PERI;
        return IN;
    }
    return OUT;
}

void canvas(char *canva, int width, int height) {
    for (int y = 0; y < height; y++)
	{
		write(1, canva + (y * width), width);
		write(1, "\n", 1);
	}
}

int main(int argc, char **argv) {
    FILE    *file;
    int     width, height, ret;
    char    background, ch, color;
    float   xtop, ytop, radius;

    if (argc != 2)
        return err(ARG);
    if (!(file = fopen(argv[1], "r")))
        return err(CORRUPT);

    if (fscanf(file, "%d %d %c\n", &width, &height, &background) != 3 || !(width > 0 && width <= 300 && height > 0 && height <= 300))
    {
        fclose(file);
        return err(CORRUPT);
    }
    char canva[width * height];
    memset(canva, background, sizeof(canva));
    while ((ret = fscanf(file, "%c %f %f %f %c\n", &ch, &xtop, &ytop, &radius, &color)) == 5) {
        if (radius <= 0.0 || (ch != 'c' && ch != 'C'))
            break;
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (check((float)x, (float)y, xtop, ytop, radius) == PERI || (check((float)x, (float)y, xtop, ytop, radius) == IN && ch == 'C'))
                    canva[x + y * width] = color;
    }
    if (ret != EOF) {
        fclose(file);
        return err(CORRUPT);
    }
    canvas(canva, width, height);
    fclose(file);
    return 0;
}