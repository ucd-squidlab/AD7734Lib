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

#define ADDR_CHANNELZEROSCALECAL(adc_channel) (0x10 + adc_channel)

#define ADDR_CHANNELFULLSCALECAL(adc_channel) (0x18 + adc_channel)

#define ADDR_CHANNELSTATUS(adc_channel)(0x20 + adc_channel)

#define ADDR_CHANNELSETUP(adc_channel)(0x28 + adc_channel)

#define ADDR_CHANNELCONVERSIONTIME(adc_channel)(0x30 + adc_channel)

#define ADDR_MODE(adc_channel) (0x38 + adc_channel)

//mode register 

//mode bits (MD2, MD1, MD0 bits)
#define IDLE_MODE                   0 << 5
#define CONT_CONV_MODE              1 << 5
#define SINGLE_CONV_MODE            2 << 5
#define PWR_DOWN_MODE               3 << 5
#define ZERO_SCALE_SELF_CAL_MODE    4 << 5
#define CH_ZERO_SCALE_SYS_CAL_MODE  6 << 5
#define CH_FULL_SCALE_SYS_CAL_MODE  7 << 5

#define FSR 20.0
#define ADC2FLOAT(vin) (vin * FSR/(0.5*0xFFFF) - (FSR/2.0)) 

AD7734::AD7734() { }
AD7734::~AD7734() { }

//sets up the AD7734 SPI communication and pins 
void AD7734::SetupAD7734(int cs, int rdy, int rst) {
    _cs = cs;
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

    SPI.usingInterrupt(_rdy);

    pinMode(_rdy, INPUT);
    pinMode(_rst, OUTPUT);

    //ADC has power on reset, so keep reset high
    digitalWrite(_rst, HIGH);
}

byte AD7734::GetADCStatus() {
    byte data_array;

    data_array = READ | ADDR_ADCSTATUS;

    SPI.transfer(_cs, data_array);

    return SPI.transfer(_cs, 0);
}

void AD7734::ChannelSetup(int adc_channel, byte flags) {
    byte data_array[2];

    data_array[0] = WRITE | ADDR_CHANNELSETUP(adc_channel);
    data_array[1] = flags;

    SPI.transfer(_cs, data_array, 2);
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

short AD7734::GetConversionData(int adc_channel) {
    byte data_array;
    
    //setup communication register for reading channel data
    data_array = READ | ADDR_CHANNELDATA(adc_channel);

    //write to the communication register
    SPI.transfer(_cs, data_array);

    //read upper and lower bytes of channel data register (16 bit mode)
    byte upper, lower;
    upper = SPI.transfer(_cs, 0);
    lower = SPI.transfer(_cs, 0);

    short result = upper << 8 | lower;

    return result;
}