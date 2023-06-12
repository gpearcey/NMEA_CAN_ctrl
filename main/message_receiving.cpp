#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "NMEA_msg.h"
#include "esp_log.h"
#include <N2kMsg.h>
#include <NMEA2000_esp32.h> 
#include <NMEA2000.h>
#include "ESP32N2kStream.h"
//#define ESP32_CAN_TX_PIN (gpio_num_t) 32 // from sdkconfig (idf menuconfig)
//#define ESP32_CAN_RX_PIN (gpio_num_t) 34

static const char* TAG = "message_sending.cpp";
tNMEA2000_esp32 NMEA2000;
static TaskHandle_t N2K_task_handle = NULL;

// This is a FreeRTOS task
void N2K_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Starting task");
    
    NMEA2000.SetN2kCANMsgBufSize(8);
    NMEA2000.SetN2kCANReceiveFrameBufSize(223);// not sure what this should be set to

    NMEA2000.SetForwardStream(new ESP32N2kStream()); // will need to change to WASM
    NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);   // Show in clear text
    // NMEA2000.EnableForward(false);                 // Disable all msg forwarding to USB (=Serial)

    //NMEA2000.SetMsgHandler(HandleNMEA2000Msg);

    //do I need to do anything for group functions?

    // If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
    NMEA2000.SetMode(tNMEA2000::N2km_ListenOnly, 22);//does 22 do anything in ListenOnly mode?

    // Here we tell, which PGNs we transmit and receive we want to receive all!
    //NMEA2000.ExtendTransmitMessages(TransmitMessages, 0);
    //NMEA2000.ExtendReceiveMessages(ReceiveMessages, 0);

    // Define OnOpen call back. This will be called, when CAN is open and system starts address claiming.
    NMEA2000.Open();
    for (;;)
    {
        // put your main code here, to run repeatedly:
        //TaskN2kBinStatus();
        NMEA2000.ParseMessages();
    }
    vTaskDelete(NULL); // should never get here...
}

extern "C" int app_main(void)
{
    esp_err_t result = ESP_OK;
    ESP_LOGV(TAG, "create task");
    xTaskCreate(
        &N2K_task,            // Pointer to the task entry function.
        "Reading_task",           // A descriptive name for the task for debugging.
        3072,                 // size of the task stack in bytes.
        NULL,                 // Optional pointer to pvParameters
        tskIDLE_PRIORITY + 3, // priority at which the task should run
        &N2K_task_handle      // Optional pass back task handle
    );
    if (N2K_task_handle == NULL)
    {
        ESP_LOGE(TAG, "Unable to create task.");
        result = ESP_ERR_NO_MEM;
        goto err_out;
    }

err_out:
    if (result != ESP_OK)
    {
        if (N2K_task_handle != NULL)
        {
            vTaskDelete(N2K_task_handle);
            N2K_task_handle = NULL;
        }
    }

    return result;
};