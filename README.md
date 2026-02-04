# LSB-Image-Stegnography🔐🖼️
# Image Steganography using LSB Technique in C.

# Description
This project implements image steganography using the Least Significant Bit (LSB) technique in C.
The objective is to hide secret text inside a BMP image without causing any visible change in the image.

I worked on this project to understand how data can be hidden inside an image at bit level.
The basic idea is to use the least significant bit of image bytes to store secret data.
Since only the last bit is changed, there is no visible change in the image.
I used BMP image format because it is uncompressed and suitable for direct bit manipulation.
In the encoding part, I read the source image and the secret text file, convert the secret data into bits, and store each bit in the LSB of the image bytes.
In the decoding part, I read the stego image, extract the LSB bits, and reconstruct the original secret message.
Through this project, I improved my understanding of bitwise operations and binary file handling in C.

# 🛠 Concepts Used

- C Programming
- Bitwise Operations (LSB manipulation)
- File Handling in C (fopen, fread, fwrite, fseek)
- Binary File Processing
- Structures and Pointers
- Command Line Arguments
- Modular Programming using Header Files
- BMP Image Format Handling
- Encoding and Decoding Logic

#  Working Principle

The working principle of this project is based on Least Significant Bit (LSB) steganography.

During encoding, the source BMP image and the secret text file are read in binary mode.
The BMP header is copied directly to the output image to preserve the image format.
Each bit of the secret data is stored in the least significant bit of consecutive image bytes using bitwise operations.
Since only the last bit of each byte is modified, the visual quality of the image remains unchanged.
Additional information such as magic string, secret file extension, and secret file size is also encoded to support proper decoding.

During decoding, the stego image is read and the BMP header is skipped.
The least significant bits of image bytes are extracted to reconstruct the hidden data.
Using the decoded file size and extension, the original secret message is correctly retrieved and written to an output file.



🔹 Overall Encoding Flow

In the encoding part, my program hides secret data into a BMP image using the LSB technique.
First, I validate input arguments and open required files.
Then I check whether the image has enough capacity to store the secret data.
After copying the BMP header, I encode a magic string, secret file extension, secret file size, and finally the secret file data into the image.
At the end, I copy the remaining image data to generate the final stego image.

🔹 get_image_size_for_bmp()

This function calculates the image capacity.
In BMP format, width and height are stored at offset 18.
I read width and height from the header and return width × height × 3 since BMP uses 3 bytes per pixel.

🔹 open_files()

This function opens the source image, secret file, and stego image file.
I perform error handling for each file to ensure safe execution.

🔹 read_and_validate_encode_args()

This function validates command-line arguments.
It checks whether the source image is .bmp, secret file has an extension, and stego image name is valid.
If stego image name is not provided, I use a default name stego.bmp.

🔹 check_capacity()

Before encoding, I check whether the image has sufficient capacity.
I calculate required size for magic string, file extension, file size, and secret data in bits.
If the image size is sufficient, encoding continues.

🔹 copy_bmp_header()

BMP header is 54 bytes.
I copy these bytes directly to the stego image without modification to preserve image structure.

🔹 encode_byte_to_lsb()

This is the core LSB logic.
For each bit of secret data, I clear the LSB of the image byte and set it using bitwise operations.
Only the least significant bit is modified, so image quality is not affected.

🔹 encode_size_to_lsb()

This function encodes an integer value, like file size or extension size, into 32 image bytes using LSB technique.

🔹 encode_data_to_image()

This is a generic function used to encode any data.
For each character, I read 8 bytes from the image and store each bit of the character into the LSB of those bytes.

🔹 encode_magic_string()

A predefined magic string is encoded to identify that the image contains hidden data.
This helps during decoding.

🔹 encode_secret_file_ext_size() and encode_secret_file_extn()

First, I encode the length of the secret file extension.
Then I encode the actual extension so that the decoder knows what type of file is hidden.

🔹 encode_secret_file_size() and encode_secret_file_data()

I encode the secret file size so the decoder knows how many bytes to extract.
After that, I encode the complete secret file data into the image using LSB.

🔹 copy_remaining_img_data()

After encoding is complete, I copy the remaining image bytes directly to the stego image.

🔹 do_encoding() (MAIN CONTROLLER)

This function controls the complete encoding process.
It calls all helper functions step by step and ensures encoding is done correctly with proper error handling.


 ## 🔁 Overall Decoding Flow

In the decoding part, my program extracts the hidden secret data from the stego BMP image using the LSB technique.
First, I validate the input arguments and open the stego image file. Then I skip the BMP header to reach the encoded data.
The program decodes a predefined magic string to confirm that the image contains hidden data.
After that, it decodes the secret file extension size, secret file extension, and secret file size.
Finally, it extracts the secret file data bit by bit from the LSBs of the image bytes and writes the decoded data into an output file with the correct file extension.

🔹 open_files_decode()

This function opens the stego image file in read mode.
Proper error handling is done to ensure that decoding does not continue if the file cannot be opened.

🔹 read_and_validate_decode_args()

This function validates the command-line arguments for decoding.
It checks whether the provided input file is a .bmp file.
If the output file name is provided, it uses that name; otherwise, it assigns a default name (decoded) for the output file.

🔹 decode_byte_from_lsb()

This function extracts one byte of secret data from 8 image bytes.
It reads the least significant bit of each image byte, shifts it to the correct position, and reconstructs the original character using bitwise operations.

🔹 decode_size_from_lsb()

This function decodes an integer value (such as file size or extension size) from the LSBs of 32 image bytes.
Each LSB bit is extracted and combined to reconstruct the original 32-bit size value.

🔹 decode_magic_string()

A predefined magic string is decoded from the stego image.
This step confirms that the image actually contains hidden data encoded by the same steganography algorithm.

🔹 decode_data_from_image()

This is a generic function used to decode any data from the stego image.
For each character, it reads 8 bytes from the image and calls decode_byte_from_lsb() to reconstruct the original data.

🔹 decode_secret_file_ext_size()

This function decodes the size of the secret file extension.
It reads 32 bytes from the stego image and extracts the extension size using LSB decoding.

🔹 decode_secret_file_extn()

This function decodes the actual secret file extension (for example, .txt).
The decoded extension is appended to the output file name so that the original file type is restored.

🔹 decode_secret_file_size()

This function decodes the size of the secret file data.
Knowing the file size helps the decoder determine how many bytes of secret data need to be extracted.

🔹 decode_secret_file_data()

This function extracts the complete secret file data from the stego image.
After decoding, the data is written into the output file using the decoded file name and extension.

🔹 do_decoding() (MAIN CONTROLLER)

This function controls the entire decoding process.
It opens files, skips the BMP header, decodes the magic string, secret file extension, file size, and secret data step by step.
Proper status checking and error handling ensure that decoding is completed safely and correctly.


# Compilation 
<img width="940" height="151" alt="image" src="https://github.com/user-attachments/assets/5a0e4e3f-30bc-4257-8c1d-7b1a5fbe1536" />



1.gcc *.c

2.encoding => ./a.out -e stego1.bmp secret.txt st.bmp

3.decoding => ./a.out -d st.bmp


# 📘 Learning Outcomes

- Understood LSB-based image steganography
- Gained hands-on experience with bitwise operations
- Learned binary file handling using C
- Improved understanding of command-line arguments
- Practiced modular programming using multiple source files

# Sample Output

1. Encoding
    <img width="923" height="766" alt="image" src="https://github.com/user-attachments/assets/b5a91a66-e36a-4d50-8db0-c146178fe111" />

2. Decoding


<img width="729" height="513" alt="image" src="https://github.com/user-attachments/assets/335a0d7e-d839-455f-85f7-2588169a6921" />



<img width="1457" height="729" alt="image" src="https://github.com/user-attachments/assets/3c32d46a-0b39-4ceb-974d-6bff0f63379d" />

<img width="1405" height="721" alt="Screenshot 2026-02-04 091254" src="https://github.com/user-attachments/assets/7f233445-8329-46e0-9331-8f6a948e61c1" />








