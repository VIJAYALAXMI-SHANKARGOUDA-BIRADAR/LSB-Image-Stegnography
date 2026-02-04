/*
NAME : VIJAYALAXMI SHANKARGOUDA BIRADAR.
DATE : 24-09-2025
PROJECT : LSB IMAGE STEGNOGRAPHY.
*/

#include <stdio.h>
#include <string.h>

#include "encode.h"
#include "types.h"
#include "decode.h"

OperationType check_operation_type(char *argv)
{
        //STEP1 1: Check the argv is -e or not ,if yes Goto step2
    if(strcmp(argv,"-e")==0)
    {
        //step2 2:return e_encode
        return e_encode;
    }

        //step3: check if argv is -d or not ,if yes goto step 4,if not goto step 5
    else if(strcmp(argv,"-d") ==0)
    {    
        //step 4: return e_decode
        return e_decode;
    }
        //step 5 :return e_supported
    else
    {
        return e_unsupported;

    }
}

int main(int argc,char *argv[])
{
    if(argc<2)
    {
        printf("INFO : Please pass valid arguments.\n");
        printf("INFO : Encoding - minimum 4 arguments.\n");
        printf("INFO : Decoding - minimum 3 arguments\n");
        return 0;
    }

    OperationType res=check_operation_type(argv[1]); //call the check_operation_type function
    if(res==e_encode)
    {

        if(argc <4)
        {
            printf("INFO : For encoding please pass minimum 4 argumnets like\n");
            printf("./a.out -e source_image_file secret_data_file\n");
            return e_failure;
        }
        else if(argc ==5)
        {
        printf("INFO : Selected Encoding,Encoding Started.\n");
       printf("\nINFO : Output file  mentioned => %s\n",argv[4]);
        }
        else
        {

        printf("INFO : Selected Encoding,Encoding Started.\n");
        printf("\nINFO : Output file not mentioned,default file name is added => <stego.bmp>.\n");
        
        }
        EncodeInfo encInfo;

        if(read_and_validate_encode_args(argv,&encInfo)==e_success)  // call read_and_validate_encode_args function
        {
            printf("\nINFO : Read and validate executed successfully.\n");

            if(do_encoding(&encInfo)==e_success)
            {
                printf("-------------------------------------------------------------------------------------------\n");
                printf("INFO : Encoding Completed Succesfully.     \n");
                printf("-------------------------------------------------------------------------------------------\n");

            }
            else
            {
            //print the error msg
            printf("Error : encoding failed.\n");

            //return e_failure
            return e_failure;
            }
        }
        else
        {
            // print error msg
            printf("Read and validate encode arg is a failed.\n");
             return 1;

        }    
    }
    else if(res==e_decode)
    {
        if(argc <3)
        {
            printf("INFO : For decoding please pass minimum 3 argumnets like\n");
            printf("./a.out -d stego.bmp <optional file>\n");
            return e_failure;
        }
        else if(argc == 4)
        {
        printf("INFO : Selected Decoding,Decoding Started.\n");
        printf("\nINFO : Output file  mentioned => %s\n",argv[3]);
        }
        else
        {
        printf("INFO : Selected Decoding,Decoding Started.\n");
        printf("\nINFO : Output file not mentioned,default file name is added => <decoded.txt>.\n");
        
        }


       DecodeInfo decinfo;

        if(read_and_validate_decode_args(argv,&decinfo)==e_success)  // call read_and_validate_decode_args function
        {
            printf("\nINFO : Read and Validation is completed for Decoding successfully.\n");

            if(do_decoding(&decinfo)==e_success)
            {
                printf("----------------------------------------------------------------------------\n");
                printf("INFO : Decoding Completed Successfully.\n");
                printf("----------------------------------------------------------------------------\n");
            }
            else
            {
            //print the error msg
            printf("Error : decoding failed\n");

            //return e_failure
            return e_failure;
            }
        }
        else
        {
            // print error msg
            printf("INFO : Read and validate decode arg is a failed\n");
             return 1;
        }    
    }

    else
    {
        //print the error msg
        printf("Invalid option selected\nFor Decoding: ./a.out -e beautiful.bmp secret.txt [stego.bmp]\nFor decoding : ./a.out -d stego.bmp\n");
        //return e_failure
    }

    return 0;
}
