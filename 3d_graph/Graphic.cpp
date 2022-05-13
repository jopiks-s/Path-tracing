#include "Graphic.h"



float Graphic::Length(const Vector3f& vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

Vector3f Graphic::Normalize(const Vector3f& vec)
{
    Vector3f ret = vec;
    float l = Length(ret);
    if(ret.x!=0)
    ret.x /= l;
    if (ret.y != 0)
    ret.y /= l;
    if (ret.z != 0)
    ret.z /= l;
    return ret;
}

Vector3f Graphic::Rotate(const Vector3f& vec, const Vector3f& fi)
{
    Vector3f ret = vec;
    Vector3f buff = ret;
    //rotate on y axis
    ret.x = buff.x * cos(fi.y) + buff.z * sin(fi.y);
    ret.y = buff.y;
    ret.z = -buff.x * sin(fi.y) + buff.z * cos(fi.y);

    buff = ret;

    //rotate on z axis
    ret.x = cos(fi.z) * buff.x - buff.y * sin(fi.z);
    ret.y = sin(fi.z) * buff.x + buff.y * cos(fi.z);

    return ret;
}

Vector3f Graphic::Mix(const Vector3f& v1, const Vector3f& v2, const float& percent)
{
    Vector3f diff = Vector3f(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);

    return Vector3f(v1.x + diff.x * percent, v1.y + diff.y * percent, v1.z + diff.z * percent);;
}

void Graphic::Print(const Vector3f& vec)
{
    std::cout << "x: " + std::to_string(vec.x) + " y: " + std::to_string(vec.y) + " z: " + std::to_string(vec.z)+ "\n";
}
