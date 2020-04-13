#ifndef _IMAGE_
#define _IMAGE_

enum result
{
    OK,
    STBI_ERROR,
    MALLOC_FAULT,
    UNDEFINED,
    WRONG_DATA_TYPE
};

class image
{
private:
    result last_result;
    int width;
    int height;
    unsigned char data[];

public:
    image(/* args */);
    ~image();

    int hello_world ();
};

#endif // _IMAGE_