#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QFile>

#define MAX_BUFSIZE 128

class Frame
{
public:
    /* Constructor */
    Frame(int16_t cmd)

    void PackFrame(void);
    QString Filename;

private:
    int16_t m_cmd;
    int16_t m_length;
    int8_t m_databuf[MAX_BUFSIZE];
    int32_t m_reverse;
    int16_t m_crc;
};





#endif // FILE_H
