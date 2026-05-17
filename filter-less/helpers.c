#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

RGBTRIPLE buffer;
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int buff = round(((float) image[i][j].rgbtBlue + (float) image[i][j].rgbtGreen +
                              (float) image[i][j].rgbtRed) /
                             3);
            image[i][j].rgbtBlue = buff;
            image[i][j].rgbtGreen = buff;
            image[i][j].rgbtRed = buff;
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
            float originalBlue = (float) image[i][j].rgbtBlue;
            float originalGreen = (float) image[i][j].rgbtGreen;
            float originalRed = (float) image[i][j].rgbtRed;
            int red = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int green = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int blue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            image[i][j].rgbtRed = red > 255 ? 255 : red;
            image[i][j].rgbtGreen = green > 255 ? 255 : green;
            image[i][j].rgbtBlue = blue > 255 ? 255 : blue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            buffer = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE array[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            array[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int div = 0;
            float red = 0, green = 0, blue = 0;
            for (int col = -1; col < 2; col++)
            {
                for (int row = -1; row < 2; row++)
                {
                    if (!(i + col < 0 || i + col >= height || j + row < 0 || j + row >= width))
                    {
                        red += (float) array[i + col][j + row].rgbtRed;
                        green += (float) array[i + col][j + row].rgbtGreen;
                        blue += (float) array[i + col][j + row].rgbtBlue;
                        div = div + 1;
                    }
                }
            }
            image[i][j].rgbtRed = round(red / div);
            image[i][j].rgbtGreen = round(green / div);
            image[i][j].rgbtBlue = round(blue / div);
        }
    }
    return;
}
