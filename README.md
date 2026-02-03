# LSB-Image-Stegnography
# Image Steganography using LSB Technique in C.

# Description
  My project is Image Steganography using the LSB technique in C.
I worked on this project to understand how data can be hidden inside an image at bit level.
The basic idea is to use the least significant bit of image bytes to store secret data.
Since only the last bit is changed, there is no visible change in the image.
I used BMP image format because it is uncompressed and suitable for direct bit manipulation.
In the encoding part, I read the source image and the secret text file, convert the secret data into bits, and store each bit in the LSB of the image bytes.
In the decoding part, I read the stego image, extract the LSB bits, and reconstruct the original secret message.
Through this project, I improved my understanding of bitwise operations and binary file handling in C.

