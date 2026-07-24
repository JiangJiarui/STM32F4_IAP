#include "./UserFile.h"

uint8_t* UserFrame::frame()
{
    return m_frame;
}

uint16_t UserFrame::length()
{
    return (uint16_t)m_frame[2] | ((uint16_t)m_frame[3] << 8);;
}


UserFrame::UserFrame(int16_t cmd, uint16_t length, uint8_t* payload)
{
    uint8_t header_len = sizeof(cmd) + sizeof(length);
    uint8_t *ptr = m_frame;
    uint16_t crc = 0;

    /* Header */
    m_frame[0] = (uint8_t)(cmd & 0xffu);
    m_frame[1] = (uint8_t)((cmd & 0xff00u)>>8);
    m_frame[2] = (uint8_t)(length & 0xffu);
    m_frame[3] = (uint8_t)((length & 0xff00u)>>8);

    /* Payload */
    if((payload != nullptr) && (length > 0) && (length <= MAX_BUFSIZE))
    {
        for(int i =0; i < length; i++)
        {
            m_frame[i+ header_len] = *(payload + i);
        }
    }
    else
    {
        qDebug("frame input invalid");
        return;
    }

    for(int k = 0; k < 4; k++)
    {
        m_frame[length + header_len + k] = 0xffu;   //reserved
    }

    /* CRC */
    crc = prCRC_Cal(ptr, header_len + length);
    m_frame[length + header_len + 4] = (uint8_t)(crc & 0xffu);
    m_frame[length + header_len + 5] = (uint8_t)((crc & 0xff00u)>>8);
}


uint16_t UserFrame::prCRC_Cal(uint8_t parityframe[], uint16_t length)
{
    uint16_t polynom = 0x8005;   //crc-16-ibm
    uint16_t crc = 0;
    uint8_t i;
    uint16_t len = length;
    uint8_t* ptr = parityframe;

    while(len--)
    {
        for(i = 0x80; i; i = i>>1)
        {
            if((crc & 0x8000) != 0)
            {
                crc = crc << 1;
                crc = crc ^ polynom;
            }
            else
            {
                crc = crc << 1;
            }

            if((*ptr&i) != 0)
            {
                crc = crc ^ polynom;
            }
        }
        ptr++;
    }

    return crc;
}

