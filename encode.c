#include <stdio.h>
#include <string.h>

#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("INFO : Image Size => width = %u", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf(" , height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char* argv[],EncodeInfo * encInfo)
{
    
        //Step 1: check argv[2] is .bmp file or not,if yes goto step 2,if not goto step 3
    if(strstr(argv[2],".bmp") != NULL)
    
    
        //step 2: store the file name into src_image_fname
        encInfo ->src_image_fname = argv[2];
    
    else
        //step 3 : return e_failure
        return e_failure;
    

        //step 4 : check argv[3] is .txt file or not,if yes goto step 5
    if(strstr(argv[3],".") != NULL)
    {
        //step 5:
            //i)store the file name into the secret_fname
           encInfo -> secret_fname= argv[3];

            //ii)fetch the extn and store into the extn_sec_file(strcpy)
            char * extension;
            extension=strrchr(argv[3],'.');
            if(extension !=NULL)
            {
                strcpy(encInfo ->extn_secret_file,extension);
              }

    }
    else
    {
        // step 6 : return e_failure
        return e_failure;
    }

        //step 7 :check  argv[4] is != NULL,if yes goto step 8,if no goto step 12
    if(argv[4]!= NULL)
    {
        //step 8: if yes ,goto step 9
        if(strstr(argv[4],".bmp") != NULL)
        {
         //step 9 : check the file is .bmp or not,if yes goto step 10
        //step 10 : store the filename into the stego_fname,return e_success
            encInfo ->stego_image_fname = argv[4];
        }
        //step 11 : return e_failure
        else
        {
            return e_failure;

        }
    }
    else
    {
        //step 12 : store the default file name[stego.bmp] into the stego_image_fname
        encInfo -> stego_image_fname = "stego.bmp";
    }

        //return e_success
        return e_success;
    }

uint get_file_size(FILE * fptr_secret)
{
    //step 1 : move the file pointer to the end(fseek)
    fseek(fptr_secret,0,SEEK_END);

    //step 2 : return by using ftell
     return ftell(fptr_secret);

}


Status encode_secret_file_ext_size(long file_size, EncodeInfo *encInfo)
{
        char image_buffer[32];

        //step 1 :  read 32 bytes from the src_image
        fread(image_buffer,1,32,encInfo -> fptr_src_image);

        //step 2 : call a size_to_lsb(extn,size,image_buffer)
        encode_size_to_lsb(file_size,image_buffer);

        //step 3 : write 32 bytes to the dest_image
        fwrite(image_buffer,1,32,encInfo -> fptr_stego_image);

        return e_success;

}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn,strlen(file_extn),encInfo -> fptr_src_image,encInfo -> fptr_stego_image);   //call

}

Status check_capacity(EncodeInfo *encInfo)
{
    //get source image size
    int src_file_Size=get_image_size_for_bmp(encInfo->fptr_src_image);
     //get secret file size
    encInfo -> size_secret_file = get_file_size(encInfo->fptr_secret);

    // check if capacity is enough
    if(src_file_Size > 54 
                        + strlen(MAGIC_STRING) * 8
                        + 32
                        + strlen(encInfo->extn_secret_file) * 8 + 
                        32 + (encInfo -> size_secret_file * 8))
    {
        return e_success;
    }
    else
    {
    return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char image_buffer[54];
    rewind(fptr_src_image);

    //step 1 : read 54 bytes from the src
    fread(image_buffer,1,54,fptr_src_image);
    //step 2 : write 54 bytes to the dest
    fwrite(image_buffer,1,54,fptr_dest_image);

    //step 3 : return e_success
    return e_success;

}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
    //step 1 : get the msb bit in data
   // char val = (data & (1<<(7-i))) ? 1: 0;
    char val = (data >> i) & 1;
 
    //step 2 : clear the lsb bit in image_buffer[i]
    //image_buffer[i] =image_buffer[i] & 0xFE; //1111 1110 
    image_buffer[i] =image_buffer[i] & (~1); //1111 1110 

    //step 3 : step1 | step2
    image_buffer[i] =image_buffer[i] | val;
    }
    return e_success;
}

Status encode_size_to_lsb(int data, char *image_buffer)
{
    for(int i=0;i<32;i++)
    {
    //step 1 : get the msb bit in data
   // char val = (data & (1<<(31-i))) ? 1: 0; //OR
    char val = (data >> i) & 1;

    //step 2 : claer the lsb bit in image_buffer[i]
    image_buffer[i] =image_buffer[i] & (~1); //1111 1110 (FE VALUE)

    //step 3 : step1 | step2
    image_buffer[i] =image_buffer[i] | val;

    }
    return e_success;
}


Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
encode_data_to_image(magic_string,strlen(magic_string),encInfo -> fptr_src_image,encInfo ->fptr_stego_image);

//printf("\npos = %ld\n", ftell(encInfo -> fptr_stego_image));
    //if(ftell(encInfo -> fptr_stego_image) == 70)
    //return e_success;
    //else return e_failure;
}

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)//generic function
{
    char image_buffer[8];

    for(int i=0;i<size;i++)
    {

        //step1 : read 8 bytes from the source
        fread(image_buffer,1,8,fptr_src_image);
        //step 2 : call byte_to_lsb(data[i],image_buffer)
        encode_byte_to_lsb(data[i],image_buffer);

        //step 3 : write 8 bytes to the dest
        fwrite(image_buffer,1,8,fptr_stego_image);
    }
}


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_buffer[32];

    //step 1 :read 32 bytes from the src
    fread(image_buffer,1,32,encInfo -> fptr_src_image);

    //step 2 : call a size_to_lsb(file_size,image_buffer);
    encode_size_to_lsb(file_size,image_buffer);

    //step 3 : write 32 bytes to the dest_image
    fwrite(image_buffer,1,32,encInfo -> fptr_stego_image);

        return e_success;

    
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char data[encInfo -> size_secret_file];

    rewind(encInfo -> fptr_secret);

    //read contents from the file
    fread(data,encInfo -> size_secret_file,1,encInfo -> fptr_secret);
    encode_data_to_image(data,encInfo -> size_secret_file,encInfo ->fptr_src_image,encInfo ->fptr_stego_image);

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    //while(//read byte by byte  from src up to EOF)
    while(fread(&ch,1,1,fptr_src)==1)

    {
        //write byte by by byte to the dest_img
        fwrite(&ch,1,1,fptr_dest);
    }
}


Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_failure)
    {
        //print the error msg
        printf("Error : opening of file is failed");
        //return e_failure
        return e_failure;
    }
    else
    {
        //print the success msg
        printf("\nINFO : Files are opened successfully. \n\n");
    }

    if(check_capacity(encInfo)==e_failure)
    {
        //print the error msg
        printf("Error :Image not possible to match the that capacity\n");
        //return e_failure
        return e_failure;
    }

    else
    {
        //print the success msg
        printf("INFO : Image_file capacity  Check successfully completed ->Suffiecient space available\n\n");
    }

    if(copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_failure)
    {
        //print the error
        printf("Error: Copying BMP header failed\n");
        
        return e_failure;

    }
    else
    {
        //print the success msg
        printf("\nINFO : Copied BMP header successfully ");
        printf(",Stego size = %ld Bytes\n\n", ftell(encInfo -> fptr_stego_image));


    }

    if(encode_magic_string(MAGIC_STRING,encInfo)==e_failure)
    {
        //print the error msg
        printf("Error : Encoding magic string failed\n");
        return e_failure;
    
    }
    else
    {
    //print the success msg
    printf("\nINFO : Magic_String Encoded Successfully -> %ld bytes",strlen(MAGIC_STRING)*8);
    printf(",Stego size = %ld bytes\n", ftell(encInfo -> fptr_stego_image));
        
    }


    if(encode_secret_file_ext_size(strlen(encInfo -> extn_secret_file), encInfo) ==e_failure)
    {
        //print the error msg
        printf("Error : Encoding secret file extension failed");

        //return e_failure
        return e_failure;
    }
    else
    {
        //print the success msg
        printf("\nINFO : Secret file extension size encoded successfully -> %ld bytes",strlen(encInfo -> extn_secret_file)*8);
        printf(",Stego size = %ld bytes\n", ftell(encInfo -> fptr_stego_image));

    }

    if(encode_secret_file_extn(encInfo -> extn_secret_file,encInfo) == e_failure)
    {
        //print error msg
        printf("Error : encoding secret file extention failed\n");

        //return e_failure
        return e_failure;
    }
    else
    {
        //print success msg
        printf("\nINFO : Secret file extension encoded successfully -> %ld bytes",strlen(encInfo -> extn_secret_file)*8);
        printf(",Stego size = %ld bytes\n", ftell(encInfo -> fptr_stego_image));

    }

    if(encode_secret_file_size(encInfo -> size_secret_file,encInfo)==e_failure)
    {
        //print the error msg

        printf(" Error : Encoding secret file size is failed\n");
        //return e_failure
        return e_failure;
    }
    else
    {
        //print the success msg
        printf("\nINFO : Secret file size encoded successfully -> %ld bytes",strlen(encInfo->extn_secret_file) * 8);
        printf(",Stego size = %ld bytes\n", ftell(encInfo -> fptr_stego_image));

    }

    if(encode_secret_file_data(encInfo)==e_failure)
    {
        //print the error msg
        printf("Error:encoding secret file data is failded\n");
        //return e_failure
        return e_failure;

    }
    else
    {
        //print the success msg
        printf("\nINFO : Secret file  data encoded successfully -> %ld bytes",(encInfo -> size_secret_file * 8));
        printf(",Final Stego size = %ld bytes\n\n", ftell(encInfo -> fptr_stego_image));

    }

    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)==e_failure)
    {
        //print the error msg
        printf("Error:Copying remaining image data failed\n");
        //return e_failure
        return e_failure;

    }
    else
    {
        //print the success msg
        printf("INFO : Copied remaining image data successfully\n");
       // printf(",Stego size = %ld bytes\n\n", ftell(encInfo -> fptr_stego_image));

    }

    return e_success;

}






