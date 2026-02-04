#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

Status open_files_decode(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_decode_args(char* argv[], DecodeInfo *decInfo)
{
    //Step 1: check argv[2] is .bmp file or not,if yes goto step 2,if not goto step 3
    if(strstr(argv[2],".bmp") != NULL)
    {
        //step 2: store the file name into stego_image_fname
        decInfo ->stego_image_fname = argv[2];
    }
    else
    {
        //step 3 : return e_failure
        return e_failure;
    }

    //step 4 : check argv[3] is provided, if yes goto step 5, if no goto step 6
    if(argv[3] != NULL)
    {
        //step 5: store the filename into secret_fname
        strtok(argv[3], ".");
        decInfo->secret_fname = argv[3];
    }
    else
    {
        //step 6: store default filename "decoded.txt" into secret_fname/
        decInfo->secret_fname = malloc(30);
        strcpy(decInfo->secret_fname, "decoded");
    }

    //return e_success
    return e_success;
}

Status decode_byte_from_lsb(char *data,char *image_buffer)
{
    *data= 0;

    for(int i = 0; i < 8; i++)
    {
        // Step 1: Get the LSB bit from image_buffer[i]
        char value = image_buffer[i] & 1;

        // Step 2: Left shift the value to its position
        value = value << i;

        // Step 3: OR with value
        *data = *data | value;
    }
    return e_success;
}

Status decode_size_from_lsb(unsigned int *data, char *image_buffer)
{
    *data=0;

    // Step 1: Loop through 32 bits to decode the size (4 bytes)
    for(int i = 0; i < 32; i++)
    {
        // Step 2: Get the LSB bit from image_buffer[i]
        unsigned int value = image_buffer[i] & 1;

        // Step 3: Left shift the value to its position 
        value = value << i;

        // Step 4: OR with value to build the complete 32-bit size value
        *data = *data | value;
    }
  
    // Step 6: Return success
    return e_success;
}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char buffer[6];
    decode_data_from_image(buffer, strlen(MAGIC_STRING), decInfo->fptr_stego_image);
    //printf("magic string -> %s\n", buffer);
    return e_success;
}

Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
    char image_buffer[8];
  int i = 0;
    for(i=0;i<size;i++)
    {
        //step1 : read 8 bytes from the stego
        fread(image_buffer,1,8,fptr_stego_image);
        //step 2 : call decode_byte_from_lsb(data[i],image_buffer)
        decode_byte_from_lsb(&data[i], image_buffer);
        
    }
    data[i] = '\0';
    return e_success;
}

Status decode_secret_file_ext_size(DecodeInfo *decInfo)
{
    char image_buffer[32];

    //step 1 :  read 32 bytes from the stego_image
    fread(image_buffer,1,32,decInfo -> fptr_stego_image);

    //step 2 : call decode_size_from_lsb(extn,size,image_buffer)
    decode_size_from_lsb( & decInfo->size_secret_file, image_buffer);

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->extn_secret_file, decInfo->size_secret_file, decInfo->fptr_stego_image);
    //strcat(decInfo->secret_fname,decInfo->extn_secret_file);

    strcat(decInfo->secret_fname, decInfo->extn_secret_file);

    // decInfo->secret_fname = output_fname;

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char image_buffer[32];

    //step 1 :read 32 bytes from the stego
    fread(image_buffer,1,32,decInfo -> fptr_stego_image);

    //step 2 : call decode_size_from_lsb(file_size,image_buffer);
    decode_size_from_lsb(&decInfo->size_secret_file, image_buffer);

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char data[decInfo->size_secret_file];
    // : Decode data from stego image
    decode_data_from_image(data, decInfo->size_secret_file, decInfo->fptr_stego_image);

    decInfo->fptr_output = fopen(decInfo->secret_fname, "w");
    if (decInfo->fptr_output == NULL)
    {
        fprintf(stderr, "Error: Unable to open output file %s\n",decInfo->secret_fname);
        return e_failure;
    }

    //  Write decoded data to output file
    fwrite(data, decInfo->size_secret_file, 1, decInfo->fptr_output);

    // Close the file
    fclose(decInfo->fptr_output);


      /*if (strcmp(decInfo->extn_secret_file, ".txt") == 0)
        {
            printf("\nINFO : Decoded file content:");
            for (int i = 0; i < decInfo->size_secret_file; i++)
            {
                putchar(data[i]);
            }
            printf("\n");
        }*/

    return e_success;
}


Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_decode(decInfo)==e_failure)
    {
        //print the error msg
        printf("Error : opening of file is failed");
        //return e_failure
        return e_failure;
    }
    else
    {
        //print the success msg
        printf("\nINFO : File opened successfully\n\n");
    }

    // Skip BMP header (54 bytes)
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    if(decode_magic_string(MAGIC_STRING, decInfo)==e_failure)
    {
        //print the error msg
        printf("Error : Magic string not matched\n");
        // printf("magic string -> %s\n",MAGIC_STRING );

        return e_failure;
    }
    else
    {
        //print the success msg
        printf("\nINFO : Magic string decoded successfully");
        printf(" -> %s\n",MAGIC_STRING );

    }

    if(decode_secret_file_ext_size(decInfo) ==e_failure)
    {
        //print the error msg
        printf("Error : Decoding secret file extension failed");
        //return e_failure
        return e_failure;
    }
    else
    {
        //print the success msg
        printf("\nINFO : Decoded secret file extension size");
        printf(" -> %d Bytes\n",decInfo->size_secret_file);
    }

    if(decode_secret_file_extn(decInfo) == e_failure)
    {
        //print error msg
        printf("Error : decoding secret file extention failed\n");
        //return e_failure
        return e_failure;
    }
    else
    {
        //print success msg
        printf("\nINFO : Decoded file extension successfully");
        printf(" -> %s\n",decInfo->extn_secret_file);
    }

    if(decode_secret_file_size(decInfo)==e_failure)
    {
        //print the error msg
        printf(" Error : Decoding secret file size is failed\n");
        //return e_failure
        return e_failure;
    }
    else
    {
        //print the success msg
        printf("\nINFO : Decoded file size successfully");
         printf(" -> %d Bytes\n",decInfo->size_secret_file);
    }
     Status ret = decode_secret_file_data(decInfo);
     //printf("ret -> %d\n", ret);
    if(ret==e_failure)
    {
        //print the error msg
        printf("Error:decoding file data is failded\n");
        //return e_failure
        return e_failure;
    }
    else
    {
        //print the success msg
        printf("\nINFO : Decoded file data and written successfully\n");
        
    }

    return e_success;
}
