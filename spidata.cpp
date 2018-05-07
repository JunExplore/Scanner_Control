#include "spidata.h"
#include <algorithm>
SPIData::SPIData()
{
    tr.bits_per_word = 8;
    tr.speed_hz = 500000;
    tr.len = bufferSize;
    tr.tx_buf = NULL;
    tr.rx_buf = NULL;
    std::fill_n(tx_array, bufferSize, 0);
}

SPIData::~SPIData()
{

}

int SPIData::dataSend(SPIDev *dev)
{
    ret = ioctl(dev->fd, SPI_IOC_MESSAGE(1), &tr);
    if(ret < 1){
        qDebug("can't send spi message");
    }
    return ret;
}

void SPIData::setSendSpeed(uint32_t speed)
{
    tr.speed_hz = speed;
}

void SPIData::setTXBuff(uint16_t *pTX)
{
    tr.tx_buf = (unsigned long)pTX;
}

void SPIData::fillBuffer(uint32_t val)
{
    std::fill_n(tx_array, bufferSize, val);
}
