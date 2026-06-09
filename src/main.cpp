#include <iostream>
#include <rtl-sdr.h>
#include <conio.h>
#include <thread>
#include <chrono>


int main() 
{ 
    std:: cout << "SDR Capture Test" << std::endl;

    // check for available device
    uint32_t device_count = rtlsdr_get_device_count();
    if (device_count == 0 ) {
        std::cerr << "ERROR : No devices found :( " << std::endl;
        return 1;
    }

    std::cout << "Found " << device_count << "Supported devices" << std::endl;
    std::cout << "Device name: " << rtlsdr_get_device_name(0) << std::endl;

    // Open the device
    
    //safely create dev variable
    rtlsdr_dev_t *dev = nullptr;
    if (rtlsdr_open(&dev,0) < 0) {
        std::cerr << "ERROR: Failed to open device." << std::endl;
        return 1;
    }
    std::cerr << "Device Opened!" << std::endl;

    // Configure Device
    uint32_t target_freq = 107100000; //107.1 MHz
    int sample_rate = 2048000; //2.048 Mega-samples per second

    if (rtlsdr_set_sample_rate(dev, sample_rate) < 0)
    {
        std::cerr << "ERROR: Failed to set sample rate" << std::endl;

    }else{
        std::cout << "Sample Rate set to: " << sample_rate << " Hz" << std::endl;
    }

    if (rtlsdr_set_center_freq(dev, target_freq) < 0) {
        std::cerr << "Failed to set center frequency" << std::endl;

    } else {
        std::cout << "Successfully tuned to " << target_freq / 1000000.0 << "MHz" << std::endl;

    }


    //clear old buffer in case
    rtlsdr_reset_buffer(dev);
    
    //allocate buffer
    int bytes_to_read = 16*1024;
    uint8_t *buffer = (uint8_t *)malloc(bytes_to_read);
    int n_read = 0;

    if (buffer == NULL)
    {
        std::cout << "Buffer failed, exiting" << std::endl;
        return 1;
    }


    std::cout << "Beginning Sample Reading, press Z to quit" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    while (1)
    {
        
        // Read samples
        if (rtlsdr_read_sync(dev, buffer, bytes_to_read, &n_read) >= 0)
        {
            // Read IQ
            int num_samples = n_read / 2; //For each sample, 1 I 1 Q
            std::cout << "Read: " << num_samples << " IQ Samples" << std::endl;

            // Print first 10 IQ Pairs
            int print_count = std::min(10, num_samples);
            for (int i = 0; i < print_count; i++)
            {
                //subtract 127 to center
                int8_t I = (int8_t)(buffer[2*i]   - 127);
                int8_t Q = (int8_t)(buffer[2*i+1] - 127);
                std::cout << " [" << (i+1) << "] I=" << (int)I << " Q=" << (int)Q << std::endl;
            }
            // Delay for 1ms for readability
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        if (_kbhit()) 
        {
            char key = _getch(); //Read key
            if (key == 'z' || key == 'Z') 
            {
                break;
            }
        }
    }
    

    std::cout << "Sample collection stopped, exiting" << std::endl;
   
    rtlsdr_close(dev);
    // cleanup
    free(buffer);
    return 0;
}