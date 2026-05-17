#include "helpers.h"
#include <math.h>
#include <stdlib.h>

#define MAX_RGB 255
int check_edge(int col_id, int row_id, int height, int width);
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

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer;
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
                    if (check_edge(i + col, j + row, width, height))
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE array[height][width];
    int gxarray[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gyarray[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
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
            float xred = 0, xgreen = 0, xblue = 0, yred = 0, ygreen = 0, yblue = 0;
            for (int col = -1; col < 2; col++)
            {
                for (int row = -1; row < 2; row++)
                {
                    if (check_edge(i + col, j + row, width, height))
                    {
                        xred += (float) array[i + col][j + row].rgbtRed * gxarray[row + 1][col + 1];
                        xgreen +=
                            (float) array[i + col][j + row].rgbtGreen * gxarray[row + 1][col + 1];
                        xblue +=
                            (float) array[i + col][j + row].rgbtBlue * gxarray[row + 1][col + 1];

                        yred += (float) array[i + col][j + row].rgbtRed * gyarray[row + 1][col + 1];
                        ygreen +=
                            (float) array[i + col][j + row].rgbtGreen * gyarray[row + 1][col + 1];
                        yblue +=
                            (float) array[i + col][j + row].rgbtBlue * gyarray[row + 1][col + 1];
                    }
                }
            }
            int red = round(sqrt(xred * xred + yred * yred));
            int green = round(sqrt(xgreen * xgreen + ygreen * ygreen));
            int blue = round(sqrt(xblue * xblue + yblue * yblue));
            red = red > MAX_RGB ? MAX_RGB : red;
            green = green > MAX_RGB ? MAX_RGB : green;
            blue = blue > MAX_RGB ? MAX_RGB : blue;
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}

int check_edge(int col_id, int row_id, int height, int width)
{
    if (col_id < 0 || col_id >= width || row_id < 0 || row_id >= height)
        return 0;
    return 1;
}
