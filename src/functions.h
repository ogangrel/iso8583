#ifndef FUNCTIONS_H
#define FUNCTIONS_H

         int   strlen_sec (const unsigned char *str, int max_len);
         int   atoi_sec   (const unsigned char *str, int max_len, int  num_len);
    long int   atol_sec   (const unsigned char *str, int max_len, int  num_len);
         int   hex2dec_sec(const unsigned char *str, int str_len, int  length );
unsigned char *hex2bin_sec(const unsigned char *str, int str_len, int *length );

         int   hexdec     (unsigned char c1, unsigned char c2);

#endif // FUNCTIONS_H
