#pragma once
#include <vector>

class Color
{
public:
    Color();
    Color(const std::vector<unsigned char>& initdata);
    Color(const Color& color);
    ~Color();

    void operator= (const std::vector<unsigned char>&initdata);

    int r() const;
    int g() const;
    int b() const;

private:
    std::vector<unsigned char> data;

};