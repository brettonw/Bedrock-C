#Raspberry Pi

## GPIO references
* http://wiringpi.com/
* http://abyz.me.uk/rpi/pigpio/index.html
* http://www.hertaville.com/introduction-to-accessing-the-raspberry-pis-gpio-in-c.html
* https://elinux.org/RPi_Low-level_peripherals#GPIO_Driving_Example_.28C.29
* http://developer.ridgerun.com/wiki/index.php?title=How_to_use_GPIO_signals
* https://github.com/halherta/RaspberryPi-GPIOClass-v2

This one in particular was useful:
* http://abyz.me.uk/rpi/pigpio/examples.html#Misc_tiny_gpio

The user running the program should be a member of the "gpio" group.
````
sudo usermod -a -G gpio <username>
````
...and the user should log out and then back in.
