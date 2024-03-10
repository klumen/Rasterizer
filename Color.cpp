#include "Color.hpp"

Color::Color() { data.resize(3); }

Color::Color(const std::vector<unsigned char>& initdata) : data(initdata) {}

Color::Color(const Color& color) : data(color.data) {}

Color::~Color() {}

void Color::operator= (const std::vector<unsigned char>& initdata)
{
    data = initdata;
}

int Color::r() const { return data[0]; }

int Color::g() const { return data[1]; }

int Color::b() const { return data[2]; }