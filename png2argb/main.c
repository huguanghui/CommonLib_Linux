#include <stdio.h>
#include <stdlib.h>
#include <png.h>

void convert_png_to_argb8888(const char *input_file, const char *output_file)
{
    FILE *fp = fopen(input_file, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Unable to open input file\n");
        return;
    }
    png_structp png_ptr
        = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        fprintf(stderr, "Error: Unable to create PNG read struct\n");
        return;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        fprintf(stderr, "Error: Unable to create PNG info struct\n");
        return;
    }
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    png_set_expand(png_ptr);
    png_set_strip_16(png_ptr);
    png_set_gray_to_rgb(png_ptr);
    png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
    png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
    png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);
    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    if (color_type != PNG_COLOR_TYPE_RGBA || bit_depth != 8) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fprintf(stderr, "Error: Input file is not in RGBA8888 format\n");
        return;
    }
    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y]
            = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }
    png_read_image(png_ptr, row_pointers);
    fclose(fp);
    FILE *out_fp = fopen(output_file, "wb");
    if (!out_fp) {
        fprintf(stderr, "Error: Unable to open output file\n");
        return;
    }
    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            fwrite(px + 1, 1, 1, out_fp); // Write red channel
            fwrite(px + 2, 1, 1, out_fp); // Write green channel
            fwrite(px + 3, 1, 1, out_fp); // Write blue channel
            fwrite(px, 1, 1, out_fp); // Write alpha channel
        }
    }
    fclose(out_fp);
    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

int main(int argc, char *argv[])
{
    // const char *input_file = "input.png";
    // const char *output_file = "output.argb8888";
    const char *input_file = argv[1];
    const char *output_file = argv[2];
    convert_png_to_argb8888(input_file, output_file);
    return 0;
}
