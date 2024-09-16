#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adts.h"

static void print_fixed_header(const adts_fixed_header *header)
{
    printf("---- ADTS fixed header details: ----\n");
    printf("syncword:%02X\n", header->syncword);
    printf("id:%d\n", header->id);
    printf("layer:%d\n", header->layer);
    printf("protection_absent:%d\n", header->protection_absent);
    printf("profile:%d\n", header->profile);
    printf("sampling_frequency_index:%d\n", header->sampling_frequency_index);
    printf("private_bit:%d\n", header->private_bit);
    printf("channel_configuration:%d\n", header->channel_configuration);
    printf("original_copy:%d\n", header->original_copy);
    printf("home:%d\n", header->home);
    printf("------------------------------------------\n");
}

static void print_variable_header(const adts_variable_header *header)
{
    printf("---- ADTS variable header details: ----\n");
    printf("copyright_identification_bit:%d\n",
        header->copyright_identification_bit);
    printf("copyright_identification_start:%d\n",
        header->copyright_identification_start);
    printf("aac_frame_length:%d\n", header->aac_frame_length);
    printf("adts_buffer_fullness:%d\n", header->adts_buffer_fullness);
    printf("number_of_raw_data_blocks_in_frame:%d\n",
        header->number_of_raw_data_blocks_in_frame);
    printf("------------------------------------------\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <aac_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    adts_fixed_header fixed_header;
    adts_variable_header variable_header;

    memset(&fixed_header, 0, sizeof(adts_fixed_header));
    memset(&variable_header, 0, sizeof(adts_variable_header));

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    unsigned char buf[7];

    while (fread(buf, 1, sizeof(buf), fp) == sizeof(buf)) {

        memset(&fixed_header, 0, sizeof(adts_fixed_header));
        memset(&variable_header, 0, sizeof(adts_variable_header));
        get_fixed_header(buf, &fixed_header);
        get_variable_header(buf, &variable_header);

        print_fixed_header(&fixed_header);
        print_variable_header(&variable_header);
        char *data = malloc(variable_header.aac_frame_length - 7);
        fread(data, 1, variable_header.aac_frame_length - 7, fp);
        free(data);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}
