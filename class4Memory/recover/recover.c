#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
 
int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: Usage: ./recover IMAGE\n");
        return 1;
    }

    // open memory card
    FILE *memoryCard = fopen(argv[1], "r");
    if (memoryCard == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    
    BYTE buffer[512];
    
    // ###.jpg
    char jpegName[8];
    int counter = 0;
    
    FILE *jpeg = NULL;

    while (fread(&buffer, sizeof(BYTE), 512, memoryCard) == 512)
    {
        // look for beginning of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // first jpeg 
            if (counter == 0)
            {
                // open first jpeg and write to it
                //FILE *jpeg = fopen(jpegName, "w");
                sprintf(jpegName, "%03i.jpg", counter);
                counter ++;
                jpeg = fopen(jpegName, "w");
                fwrite(&buffer, sizeof(BYTE), 512, jpeg);
            }
            else
            {
                // open next jpeg and write to it
                fclose(jpeg);
                sprintf(jpegName, "%03i.jpg", counter);
                counter ++;
                jpeg = fopen(jpegName, "w");
                fwrite(&buffer, sizeof(BYTE), 512, jpeg);
            }
        }
        else if (jpeg != NULL)
        {
            // continue writing to current jpeg
            fwrite(&buffer, sizeof(BYTE), 512, jpeg);
        }
        
    }
    fclose(jpeg);
    fclose(memoryCard); 
}