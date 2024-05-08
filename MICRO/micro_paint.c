#include "micro_paint.h"

int ft_strlen(const char *s) {
    int i;
    for (i = 0; s[i]; i++){};
    return i;
}

int err(const char *s) {
    write(1, s, ft_strlen(s));
    return 1;
}

int check(float x, float y, float xtop, float ytop, float rwidth, float rheight) {
    if (x < xtop || x > xtop + rwidth || y < ytop || y > ytop + rheight)
        return OUT;
    if (x - xtop < 1.0 || xtop + rwidth - x < 1.0 || y - ytop < 1.0 || ytop + rheight - y < 1.0)
        return PERI;
    return IN;
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
    float   xtop, ytop, rwidth, rheight;

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
    while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &ch, &xtop, &ytop, &rwidth, &rheight, &color)) == 6)
    {
        if ((rwidth <= 0.0 || rheight <= 0.0) || (ch != 'r' && ch != 'R'))
            break;
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (check((float)x, (float)y, xtop, ytop, rwidth, rheight) == PERI || (check((float)x, (float)y, xtop, ytop, rwidth, rheight) == IN && ch == 'R'))
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