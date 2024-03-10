#include "Texture.hpp"

Texture::Texture() : height(0), width(0), format("None") {}

Texture::~Texture() {}

void Texture::load_texture(const std::string& path)
{
    std::ifstream tex(path, std::ios::in | std::ios::binary);

    if (tex.is_open())
    {
        tex.seekg(0, tex.end);
        size_t fileSize = tex.tellg();
        tex.seekg(0, tex.beg);
        char* tbuf = new char[fileSize];
        tex.read((char*)tbuf, fileSize);
        std::vector<size_t> buf(fileSize);
        for (size_t i = 0; i < fileSize; i++)
        {
            buf[i] = tbuf[i] & 0xff;
        }
        delete[] tbuf;
        if (buf[0] == 0x89 && buf[1] == 0x50)
        {
            format = ".png";
            width = buf[18] * 256 + buf[19];
            height = buf[22] * 256 + buf[23];
            size_t bitDepth = buf[24], colorType = buf[25], comdivssionMethod = buf[26];
            size_t filterMethod = buf[27], interlaceMethod = buf[28];
            if (colorType == 2)
            {
                std::vector<size_t> dataBuff;
                for (size_t i = 29; i < fileSize - 3; i++)
                {
                    if (buf[i] == (size_t)'I' && buf[i + 1] == (size_t)'D' 
                        && buf[i + 2] == (size_t)'A' && buf[i + 3] == (size_t)'T')
                    {
                        size_t IDATLength = buf[i - 4] * 0x1000000 + buf[i - 3] * 0x10000 + buf[i - 2] * 0x100 + buf[i - 1];
                        dataBuff.insert(dataBuff.end(), buf.begin() + i + 4, buf.begin() + i + 4 + IDATLength);
                    }
                }
                if (comdivssionMethod == 0)
                {
                    ;
                }
                if (filterMethod == 0)
                {
                    ;
                }
                else if (filterMethod == 1)
                {
                    ;
                }
                else if (filterMethod == 2)
                {

                }
                else if (filterMethod == 3)
                {
                    ;
                }
                else if (filterMethod == 4)
                {
                    ;
                }
                if (interlaceMethod == 0)
                {
                    ;
                }
                else if (interlaceMethod == 1)
                {
                    ;
                }
            }
        }
        else if (buf[0] == 0x42 && buf[1] == 0x4d)
        {
            format = ".bmp";
            size_t bfOffBits = buf[13] * 0x1000000 + buf[12] * 0x10000 + buf[11] * 0x100 + buf[10];
            width = buf[19] * 256 + buf[18];
            height = buf[23] * 256 + buf[22];
            size_t infSize = buf[14] + buf[15] * 0x100 + buf[16] * 0x10000 + buf[17] * 0x1000000, bitDepth = buf[28];
            data.resize(height * width);
            size_t point = bfOffBits;
            if (bitDepth == 32)
                alphaData.resize(height * width);
            if (bfOffBits == infSize + 14)
            {
                for (size_t y = 0; y < height; y++)
                    for (size_t x = 0; x < width; x++)
                    {
                        size_t ind = y * width + x;
                        if (bitDepth == 24)
                        {
                            data[ind] = Vector3f(buf[point + 2] * 1.f / 255.f, buf[point + 1] * 1.f / 255.f, buf[point] * 1.f / 255.f);
                            point += 3;
                        }
                        if (bitDepth == 32)
                        {
                            data[ind] = Vector3f(buf[point + 2] * 1.f / 255.f, buf[point + 1] * 1.f / 255.f, buf[point] * 1.f / 255.f);
                            alphaData[ind] = buf[point + 3] * 1.f / 255.f;
                            point += 4;
                        }
                    }
            }
            else
            {
                ;
            }
        }
        tex.close();
    }
    else
    {
        std::cerr << "Not find texture file" << std::endl;
        std::exit(1);
    }

    generate_mipmap();
}

void Texture::generate_mipmap()
{
    size_t depth = std::log2(width);
    size_t width_i = width, height_i = height;
    mipmap.resize(depth + 1);
    mipmap[0] = data;
    for (size_t i = 1; i <= depth; i++)
    {
        width_i /= 2, height_i /= 2;
        mipmap[i].resize(width_i * height_i);
        for (size_t y = 0; y < height_i; y++)
            for (size_t x = 0; x < width_i; x++)
            {
                Vector3f color;
                size_t ind = y * width_i + x;
                for (size_t y0 = 2 * y; y0 < 2 * (y + 1); y0++)
                    for (size_t x0 = 2 * x; x0 < 2 * (x + 1); x0++)
                    {
                        size_t ind0 = y0 * width_i * 2 + x0;
                        color = color + mipmap[i-1][ind0] / 4.f;
                    }
                mipmap[i][ind] = color;
            }
    }
}