#ifndef SPIDATA_H
#define SPIDATA_H

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <QDebug>

class SPIDev{
public:
    friend class SPIData;

    SPIDev()
    {
        device = NULL;
    }

    void selectDevice(char *dev)
    {
        device = dev;
    }

    void openDevice()
    {
        fd = open(device, O_RDWR);
        if(fd < 0){
            qDebug("can't open device");
        }
    }

    void closeDevice(){
        close(fd);
    }

    char *device;

private:
    int fd;
};

class SPIData{
public:
    SPIData();
    ~SPIData();

    int dataSend(SPIDev *dev);
    void setSendSpeed(uint32_t speed);
    void setTXBuff(uint16_t *pTX);

    void fillBuffer(uint32_t val);
private:
    static const uint32_t bufferSize = 256;
    struct spi_ioc_transfer tr;
    uint32_t tx_array[bufferSize];
    int ret;
};


#endif // SPIDATA_H
