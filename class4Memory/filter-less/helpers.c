#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            int roundedRGB = roundf(average);
            image[i][j].rgbtBlue = roundedRGB;
            image[i][j].rgbtGreen = roundedRGB;
            image[i][j].rgbtRed = roundedRGB;
        }   
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = roundf(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepiaGreen = roundf(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaBlue = roundf(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }
            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }
            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
        
    }
    
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            buffer[i][j] = image[i][j];
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[i][width - 1 - j];
        }   
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // make copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = copy[i][j].rgbtRed;
            int green = copy[i][j].rgbtGreen;
            int blue = copy[i][j].rgbtBlue;

            int counter = 1;

            // upper row
            if (i-1 > -1)
            {
                red += copy[i-1][j].rgbtRed;
                green += copy[i-1][j].rgbtGreen;
                blue += copy[i-1][j].rgbtBlue;

                counter ++;

                if (j-1 > -1)
                {
                    red += copy[i-1][j-1].rgbtRed;
                    green += copy[i-1][j-1].rgbtGreen;
                    blue += copy[i-1][j-1].rgbtBlue;

                    counter ++;
                }

                if(j+1 < width)
                {
                    red += copy[i-1][j+1].rgbtRed;
                    green += copy[i-1][j+1].rgbtGreen;
                    blue += copy[i-1][j+1].rgbtBlue;

                    counter ++;
                }
            }

            // middle row
            if (j-1 > -1)
            {
                red += copy[i][j-1].rgbtRed;
                green += copy[i][j-1].rgbtGreen;
                blue += copy[i][j-1].rgbtBlue;

                counter ++;
            }

            if (j+1 < width)
            {
                red += copy[i][j+1].rgbtRed;
                green += copy[i][j+1].rgbtGreen;
                blue += copy[i][j+1].rgbtBlue;

                counter ++;
            }
            
            // lower row
            if (i+1 < height)
            {
                red += copy[i+1][j].rgbtRed;
                green += copy[i+1][j].rgbtGreen;
                blue += copy[i+1][j].rgbtBlue;

                counter ++;

                if (j-1 > -1)
                {
                    red += copy[i+1][j-1].rgbtRed;
                    green += copy[i+1][j-1].rgbtGreen;
                    blue += copy[i+1][j-1].rgbtBlue;

                    counter ++;
                }

                if(j+1 < width)
                {
                    red += copy[i+1][j+1].rgbtRed;
                    green += copy[i+1][j+1].rgbtGreen;
                    blue += copy[i+1][j+1].rgbtBlue;

                    counter ++;
                }
            }

            int avRed = roundf(red / counter);
            int avGreen = roundf(green / counter);
            int avBlue = roundf(blue / counter);

            image[i][j].rgbtRed = avRed;
            image[i][j].rgbtGreen = avGreen;
            image[i][j].rgbtBlue = avBlue;
        }
    }
    return;
}
