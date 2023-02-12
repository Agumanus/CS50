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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
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
            int xRed = 0;
            int xGreen = 0;
            int xBlue = 0;

            int yRed = 0;
            int yGreen = 0;
            int yBlue = 0;

            //gx: left and right

            if (j-1 > -1)
            {
                xRed += -2 * copy[i][j-1].rgbtRed;
                xGreen += -2 * copy[i][j-1].rgbtGreen;
                xBlue += -2 * copy[i][j-1].rgbtBlue;

                if (i-1 > -1)
                {
                    xRed += -1 * copy[i-1][j-1].rgbtRed;
                    xGreen += -1 * copy[i-1][j-1].rgbtGreen;
                    xBlue += -1 * copy[i-1][j-1].rgbtBlue;
                }
                
                if (i+1 < height)
                {
                    xRed += -1 * copy[i+1][j-1].rgbtRed;
                    xGreen += -1 *copy[i+1][j-1].rgbtGreen;
                    xBlue += -1 * copy[i+1][j-1].rgbtBlue;
                }    
            }
            
            if (j + 1 < width)
            {
                xRed += 2 * copy[i][j+1].rgbtRed;
                xGreen += 2 * copy[i][j+1].rgbtGreen;
                xBlue += 2 * copy[i][j+1].rgbtBlue;

                if (i - 1 > -1)
                {
                    xRed += copy[i-1][j+1].rgbtRed;
                    xGreen += copy[i-1][j+1].rgbtGreen;
                    xBlue += copy[i-1][j+1].rgbtBlue;
                }

                if (i + 1 < height)
                {
                    xRed += copy[i+1][j+1].rgbtRed;
                    xGreen += copy[i+1][j+1].rgbtGreen;
                    xBlue += copy[i+1][j+1].rgbtBlue;
                }  
            }
            
            //gy: top and bottom

            if (i - 1 > -1)
            {
                yRed += -2 * copy[i-1][j].rgbtRed;
                yGreen += -2 * copy[i-1][j].rgbtGreen;
                yBlue += -2 * copy[i-1][j].rgbtBlue;

                if (j - 1 > -1)
                {
                    yRed += -1 * copy[i-1][j-1].rgbtRed;
                    yGreen += -1 * copy[i-1][j-1].rgbtGreen;
                    yBlue += -1 * copy[i-1][j-1].rgbtBlue;
                }

                if (j + 1 < width)
                {
                    yRed += -1 * copy[i-1][j+1].rgbtRed;
                    yGreen += -1 * copy[i-1][j+1].rgbtGreen;
                    yBlue += -1 * copy[i-1][j+1].rgbtBlue;
                }
            }

            if (i + 1 < height)
            {
                yRed += 2 * copy[i+1][j].rgbtRed;
                yGreen += 2 * copy[i+1][j].rgbtGreen;
                yBlue += 2 * copy[i+1][j].rgbtBlue;

                if (j - 1 > -1)
                {
                    yRed += copy[i+1][j-1].rgbtRed;
                    yGreen += copy[i+1][j-1].rgbtGreen;
                    yBlue += copy[i+1][j-1].rgbtBlue;
                }

                if (j + 1 < width)
                {
                    yRed += copy[i+1][j+1].rgbtRed;
                    yGreen += copy[i+1][j+1].rgbtGreen;
                    yBlue += copy[i+1][j+1].rgbtBlue;
                }
            }
            
            int borderRed = sqrt((xRed * xRed) + (yRed * yRed));
            int borderGreen = sqrt((xGreen * xGreen) + (yGreen * yGreen));
            int borderBlue = sqrt((xBlue * xBlue) + (yBlue * yBlue));

            if (borderRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = borderRed;
            }
            
            if (borderGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = borderGreen;
            }

            if (borderBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = borderBlue;
            }
        }
    }
    return;
}
