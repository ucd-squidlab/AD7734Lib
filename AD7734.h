#ifndef AD7734_H_
#define AD7734_H_

#define ADC_A 0
#define ADC_B 1
#define ADC_C 2
#define ADC_D 3

class AD7734 {
    public:
    AD7734();
    ~AD7734();

    //setup ADC SPI communication and pins used with the AD7734
    //cs: chip select pin, rdy: data ready pin, rst: ADC reset pin 
    void SetupAD7734(int cs, int rdy, int rst);

    //start a single conversion on the passed ADC channel, function does not handle the result of the conversion
    void StartSingleConversion(int adc_channel);

    //start a continous conversion process on the passed ADC channel, function does not handle the result of the conversion
    void StartContinousConversion(int adc_channel);

    //request channel data from the ADC
    unsigned long GetConversionData(int adc_channel);

    private:
    int _cs, _rdy, _rst;
};

#endif