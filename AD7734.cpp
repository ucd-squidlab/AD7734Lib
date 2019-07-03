#include "Arduino.h"
#include "SPI.h"
#include "AD7734.h"

//ADC symbols

//communications register

//read write bit
#define READ  1 << 6
#define WRITE 0 << 6

//ADC register addresses
//we are using the AD7734, which has 4 channels

#define ADDR_COM                      0x0
#define ADDR_IO                       0x1
#define ADDR_REVISION                 0x2
#define ADDR_TEST                     0x3
#define ADDR_ADCSTATUS                0x4
#define ADDR_CHECKSUM                 0x5
#define ADDR_ADCZEROSCALECAL          0x6
#define ADDR_ADCFULLSCALE             0x7

//Address macro functions, each takes a value betwene 0 and 3 and returns the address of the desired register

#define ADDR_CHANNELDATA(adc_channel)   (0x8 + adc_channel)

/*
#define ADDR_CHANNELDATA_0            0x8  
#define ADDR_CHANNELDATA_1            0x9
#define ADDR_CHANNELDATA_2            0xA
#define ADDR_CHANNELDATA_3            0xB
*/

#define ADDR_CHANNELZEROSCALECAL(adc_channel) (0x10 + adc_channel)

/*
#define ADDR_CHANNELZEROSCALECAL_0    0x10
#define ADDR_CHANNELZEROSCALECAL_1    0x11
#define ADDR_CHANNELZEROSCALECAL_2    0x12
#define ADDR_CHANNELZEROSCALECAL_3    0x13
*/

#define ADDR_CHANNELFULLSCALECAL(adc_channel) (0x18 + adc_channel)

/*
#define ADDR_CHANNELFULLSCALECAL_0    0x18
#define ADDR_CHANNELFULLSCALECAL_1    0x19
#define ADDR_CHANNELFULLSCALECAL_2    0x1A
#define ADDR_CHANNELFULLSCALECAL_3    0x1B
*/

#define ADDR_CHANNELSTATUS(adc_channel)(0x20 + adc_channel)

/*
#define ADDR_CHANNELSTATUS_0          0x20
#define ADDR_CHANNELSTATUS_1          0x21
#define ADDR_CHANNELSTATUS_2          0x22
#define ADDR_CHANNELSTATUS_3          0x23
*/

#define ADDR_CHANNELSETUP(adc_channel)(0x28 + adc_channel)

/*
#define ADDR_CHANNELSETUP_0           0x28 
#define ADDR_CHANNELSETUP_1           0x29
#define ADDR_CHANNELSETUP_2           0x2A
#define ADDR_CHANNELSETUP_3           0x2B
*/

#define ADDR_CHANNELCONVERSIONTIME(adc_channel)(0x30 + adc_channel)

/*
#define ADDR_CHANNELCONVERSIONTIME_0  0X30 
#define ADDR_CHANNELCONVERSIONTIME_1  0X31 
#define ADDR_CHANNELCONVERSIONTIME_2  0X32 
#define ADDR_CHANNELCONVERSIONTIME_3  0X33 
*/

#define ADDR_MODE(adc_channel) (0x38 + adc_channel)

/*
#define ADDR_MODE_0                   0x38
#define ADDR_MODE_1                   0x39 
#define ADDR_MODE_2                   0x3A 
#define ADDR_MODE_3                   0x3B  
*/

//mode register 

//mode bits (MD2, MD1, MD0 bits)
#define IDLE_MODE                   0 << 5
#define CONT_CONV_MODE              1 << 5
#define SINGLE_CONV_MODE            2 << 5
#define PWR_DOWN_MODE               3 << 5
#define ZERO_SCALE_SELF_CAL_MODE    4 << 5
#define CH_ZERO_SCALE_SYS_CAL_MODE  6 << 5
#define CH_FULL_SCALE_SYS_CAL_MODE  7 << 5

//channel setup register

//input range bits (RNG1, RNG0 bits)
#define PN_10 0 //positive/negative 10 volt
#define P_010 1 //positive, 0 to 10 volt
#define PN_5  2 //positive/negative 5 volt
#define P_05  3 //positive 0 to 5 volts

AD7734::AD7734() { }
AD7734::~AD7734() { }

//sets up the AD7734 SPI communication and pins 
void AD7734::SetupAD7734(int cs, int rdy, int rst) {
    cs = cs;
    _rdy = rdy;
    _rst = rst;

    //being SPI communication on the ADC chip select
    SPI.begin(_cs);
    //set bit order for ADC
    SPI.setBitOrder(_cs, MSBFIRST);
    //set clock divider for DAC
    SPI.setClockDivider(_cs, SPI_CLOCK_DIV64);
    //set data mode for dac
    SPI.setDataMode(_cs, SPI_MODE3);

    pinMode(_rdy, INPUT);
    pinMode(_rst, OUTPUT);

    //ADC has power on reset, so keep reset high
    digitalWrite(_rst, HIGH);
}

//tells the ADC to start a single conversion on the passed channel
void AD7734::StartSingleConversion(int adc_channel) {
    byte data_array[2];
  
    //setup communication register for writing operation to the mode register
    data_array[0] = WRITE | ADDR_MODE(adc_channel); 
    
    //setup mode register 
    data_array[1] = SINGLE_CONV_MODE;
    
    //send off command
    SPI.transfer (_cs, data_array, 2);

    //data is ready when rdy goes low
}

//tells the ADC to start a continous conversion on the passed channel 
void AD7734::StartContinousConversion(int adc_channel) {
    byte data_array[2];
  
    //setup communication register for writing operation to the mode register
    data_array[0] = WRITE | ADDR_MODE(adc_channel); 
    
    //setup mode register 
    data_array[1] = CONT_CONV_MODE;
    
    //send off command
    SPI.transfer (_cs, data_array, 2);

    //data is ready when rdy goes low
}

unsigned long AD7734::GetConversionData(int adc_channel) {
    byte data_array;
    
    //setup communication register for reading channel data
    data_array = READ | ADDR_CHANNELDATA(adc_channel);

    //write to the communication register
    SPI.transfer(_cs, data_array);

    //read upper and lower bytes of channel data register
    byte upper, lower;
    upper = SPI.transfer(_cs, 0);
    lower = SPI.transfer(_cs, 0);

    return upper << 8 | lower;
}