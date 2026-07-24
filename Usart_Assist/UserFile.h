#ifndef User_FILE_H
#define User_FILE_H

#include <QWidget>

#define MAX_BUFSIZE 128

class UserFrame
{
public:

    UserFrame(int16_t cmd, uint16_t length, uint8_t* payload);

    uint8_t* frame();
    uint16_t length();

    ~UserFrame();


private:

    uint8_t m_frame[MAX_BUFSIZE + 32];

    uint16_t prCRC_Cal(uint8_t parityframe[], uint16_t length);
};







#endif // User_FILE_H
