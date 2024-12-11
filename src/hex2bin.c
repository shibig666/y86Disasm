/*
 * 作者: Shibig666
 * 日期: 2024年12月11日
 * 说明: 文本转二进制。
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
    return 1;
  }

  FILE *inputFile = fopen(argv[1], "r");
  if (inputFile == NULL) {
    perror("Error opening input file");
    return 1;
  }

  FILE *outputFile = fopen(argv[2], "wb");
  if (outputFile == NULL) {
    perror("Error opening output file");
    fclose(inputFile);
    return 1;
  }

  char hex[3];
  int ch1, ch2;
  while ((ch1 = fgetc(inputFile)) != EOF) {
    if (ch1 == '\n')
      continue;
    ch2 = fgetc(inputFile);
    if (ch2 == '\n')
      ch2 = fgetc(inputFile);
    if (ch2 == EOF)
      break;

    hex[0] = ch1;
    hex[1] = ch2;
    hex[2] = '\0';

    unsigned char byte = (unsigned char)strtol(hex, NULL, 16);
    fwrite(&byte, sizeof(byte), 1, outputFile);
  }

  fclose(inputFile);
  fclose(outputFile);
}