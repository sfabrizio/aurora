# Aurora

This is an ever ongoing project to control ws2812, ws2811, ws2813 led light strips
having some pre programmed effects and animations while providing a simple interface both via mqtt and http.

Devices work with and without connection to a wireless network, when not connected to a wireless network commands can
be issued to the device by connecting to it via it's own ssid (Aurora_{chip_id})

Once the device is configured its configuration is stored in eeeprom memory and can be changed via the `/setup` endpoint

## firmware upload ##
Firmware must be upload with 


## SPIFSS upload ##
Spifss data contains the web interface.
use platformio command `pio run -t uploadfs` to upload contents of data directory to flash memory

## setup ##

In order to configure a device send a post to ::ip::/setup with the following body:

```
pass: the network password (max 32 chars)
ssid: the network ssid (max 32 chars)
topic: the mqtt topic to listen for commands (max 32 chars)
broker: the address of the mqtt broker (max 32 chars)
announce_topic: the topic where to anounce once connected (max 12 chars)
human_name: the human readable name of the device (max 32 chars)
ap_ssid: the ssid of the access point mode
```


## available endpoints ##

These endpoints are reachable via http requests:

`/setup` via HTTP POST and expecting a body with configuration values.
`/cmd` via HTTP POST request, expecting a body containing `cmd` and `payload` with the desired command and parameters
`/clear` via HTTP POST endpoint for clearing eeprom config
`/info` via any HTTP request responds with a json containing the current config (minus network password) plus the device_name
`"/"`  via any HTTP request presents the control interface