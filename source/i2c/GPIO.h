#pragma once

#include        "Log.h"
#include        "Text.h"

#include        <fcntl.h>
#include        <unistd.h>
#include        <sys/ioctl.h>
#include        <errno.h>
#include        <sys/mman.h>


// http://wiringpi.com/
// http://abyz.me.uk/rpi/pigpio/index.html

// http://www.hertaville.com/introduction-to-accessing-the-raspberry-pis-gpio-in-c.html
// https://elinux.org/RPi_Low-level_peripherals#GPIO_Driving_Example_.28C.29

// http://developer.ridgerun.com/wiki/index.php?title=How_to_use_GPIO_signals

// https://github.com/halherta/RaspberryPi-GPIOClass-v2


// See BCM2835 *or* BCM2837 (UNOFFICIAL), Chapter 6 for GPIO explanations
// From Raspberry Pi Foundation: rpi_DATA_CM_2p0.pdf, Chapter 9


/*
There are 54 general-purpose I/O (GPIO) lines split into two banks.

The GPIO has 41 registers. All accesses are assumed to be 32-bit.

GPIO Pin Output Set Registers (GPSETn)
The output set registers are used to set a GPIO pin. The SET{n} field defines the
respective GPIO pin to set, writing a “0” to the field has no effect. If the GPIO pin is
being used as in input (by default) then the value in the SET{n} field is ignored.
However, if the pin is subsequently defined as an output then the bit will be set
according to the last set/clear operation. Separating the set and clear functions
removes the need for read-modify-write operations.

GPIO Pin Output Clear Registers (GPCLRn)
The output clear registers are used to clear a GPIO pin. The CLR{n} field defines
the respective GPIO pin to clear, writing a “0” to the field has no effect. If the GPIO
pin is being used as in input (by default) then the value in the CLR{n} field is
ignored. However, if the pin is subsequently defined as an output then the bit will
be set according to the last set/clear operation. Separating the set and clear
functions removes the need for read-modify-write operations.

GPIO Pin Level Registers (GPLEVn)
The pin level registers return the actual value of the pin. The LEV{n} field gives the
value of the respective GPIO pin.

Address Field Name Description Size
Read/
Write
0000 GPFSEL0 Function Select 0 32 R/W
0004 GPFSEL1 Function Select 1 32 R/W
0008 GPFSEL2 Function Select 2 32 R/W
000C GPFSEL3 Function Select 3 32 R/W
0010 GPFSEL4 Function Select 4 32 R/W
0014 GPFSEL5 Function Select 5 32 R/W
0018 - Reserved - -
001C GPSET0 Pin Output Set 0 32 W
0020 GPSET1 Pin Output Set 1 32 W
0024 - Reserved - -
0028 GPCLR0 Pin Output Clear 0 32 W
002C GPCLR1 Pin Output Clear 1 32 W
0030 - Reserved - -
0034 GPLEV0 Pin Level 0 32 R
0038 GPLEV1 Pin Level 1 32 R

*/

// this is a memory mapped version of working with GPIO pins

const int GPIO_BLOCK_SIZE = 0xb4;

class GPIO {
    private:
        volatile uint* gpioReg;
    public:
        GPIO () {
            // memory map the gpio registers
            int fd = open("/dev/gpiomem", O_RDWR | O_SYNC) ;
            if (fd >= 0) {
                gpioReg = static_cast<(uint*> (mmap (0, GPIO_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
                close(fd);

                if (gpioReg == MAP_FAILED) {
                    throw RuntimeError (Text("GPIO: ") << "can't map GPIO registers");
                }
            } else {
                throw RuntimeError (Text("GPIO: ") << "can't open /dev/gpiomem");
            }
        }

        ~GPIO () {
            munmap(gpioReg, GPIO_BLOCK_SIZE);
        }

        enum Direction {
            IN, OUT
        };

        int setDirection (int pin, Direction direction) {
        }

        int getDirection (int pin) {
        }

        enum Value {
            OFF, ON
        };

        int setValue (int pin, Value value) {
        }

        int getValue (int pin) {
        }
};





//http://abyz.me.uk/rpi/pigpio/examples.html#Misc_tiny_gpio

/*
   tiny_gpio.c
   2016-04-30
   Public Domain
*/

/*
   gcc -o tiny_gpio tiny_gpio.c
   ./tiny_gpio
*/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define GPSET0 7
#define GPSET1 8

#define GPCLR0 10
#define GPCLR1 11

#define GPLEV0 13
#define GPLEV1 14

#define GPPUD     37
#define GPPUDCLK0 38
#define GPPUDCLK1 39

unsigned piModel;
unsigned piRev;

static volatile uint32_t  *gpioReg = MAP_FAILED;

#define PI_BANK (gpio>>5)
#define PI_BIT  (1<<(gpio&0x1F))

/* gpio modes. */

#define PI_INPUT  0
#define PI_OUTPUT 1
#define PI_ALT0   4
#define PI_ALT1   5
#define PI_ALT2   6
#define PI_ALT3   7
#define PI_ALT4   3
#define PI_ALT5   2

void gpioSetMode(unsigned gpio, unsigned mode)
{
   int reg, shift;

   reg   =  gpio/10;
   shift = (gpio%10) * 3;

   gpioReg[reg] = (gpioReg[reg] & ~(7<<shift)) | (mode<<shift);
}

int gpioGetMode(unsigned gpio)
{
   int reg, shift;

   reg   =  gpio/10;
   shift = (gpio%10) * 3;

   return (*(gpioReg + reg) >> shift) & 7;
}

/* Values for pull-ups/downs off, pull-down and pull-up. */

#define PI_PUD_OFF  0
#define PI_PUD_DOWN 1
#define PI_PUD_UP   2

void gpioSetPullUpDown(unsigned gpio, unsigned pud)
{
   *(gpioReg + GPPUD) = pud;

   usleep(20);

   *(gpioReg + GPPUDCLK0 + PI_BANK) = PI_BIT;

   usleep(20);

   *(gpioReg + GPPUD) = 0;

   *(gpioReg + GPPUDCLK0 + PI_BANK) = 0;
}

int gpioRead(unsigned gpio)
{
   if ((*(gpioReg + GPLEV0 + PI_BANK) & PI_BIT) != 0) return 1;
   else                                         return 0;
}

void gpioWrite(unsigned gpio, unsigned level)
{
   if (level == 0) *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
}

void gpioTrigger(unsigned gpio, unsigned pulseLen, unsigned level)
{
   if (level == 0) *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;

   usleep(pulseLen);

   if (level != 0) *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
}

/* Bit (1<<x) will be set if gpio x is high. */

uint32_t gpioReadBank1(void) { return (*(gpioReg + GPLEV0)); }
uint32_t gpioReadBank2(void) { return (*(gpioReg + GPLEV1)); }

/* To clear gpio x bit or in (1<<x). */

void gpioClearBank1(uint32_t bits) { *(gpioReg + GPCLR0) = bits; }
void gpioClearBank2(uint32_t bits) { *(gpioReg + GPCLR1) = bits; }

/* To set gpio x bit or in (1<<x). */

void gpioSetBank1(uint32_t bits) { *(gpioReg + GPSET0) = bits; }
void gpioSetBank2(uint32_t bits) { *(gpioReg + GPSET1) = bits; }

unsigned gpioHardwareRevision(void)
{
   static unsigned rev = 0;

   FILE * filp;
   char buf[512];
   char term;
   int chars=4; /* number of chars in revision string */

   if (rev) return rev;

   piModel = 0;

   filp = fopen ("/proc/cpuinfo", "r");

   if (filp != NULL)
   {
      while (fgets(buf, sizeof(buf), filp) != NULL)
      {
         if (piModel == 0)
         {
            if (!strncasecmp("model name", buf, 10))
            {
               if (strstr (buf, "ARMv6") != NULL)
               {
                  piModel = 1;
                  chars = 4;
               }
               else if (strstr (buf, "ARMv7") != NULL)
               {
                  piModel = 2;
                  chars = 6;
               }
               else if (strstr (buf, "ARMv8") != NULL)
               {
                  piModel = 2;
                  chars = 6;
               }
            }
         }

         if (!strncasecmp("revision", buf, 8))
         {
            if (sscanf(buf+strlen(buf)-(chars+1),
               "%x%c", &rev, &term) == 2)
            {
               if (term != '\n') rev = 0;
            }
         }
      }

      fclose(filp);
   }
   return rev;
}

int gpioInitialise(void)
{
   int fd;

   piRev = gpioHardwareRevision(); /* sets piModel and piRev */

   fd = open("/dev/gpiomem", O_RDWR | O_SYNC) ;

   if (fd < 0)
   {
      fprintf(stderr, "failed to open /dev/gpiomem\n");
      return -1;
   }

   gpioReg = (uint32_t *)mmap(NULL, 0xB4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

   close(fd);

   if (gpioReg == MAP_FAILED)
   {
      fprintf(stderr, "Bad, mmap failed\n");
      return -1;
   }
   return 0;
}

main()
{
   int i;

   if (gpioInitialise() < 0) return 1;

   printf("Pi model = %d, Pi revision = %d\n", piModel, piRev);

   for (i=0; i<54; i++)
   {
      printf("gpio=%d mode=%d level=%d\n",
         i, gpioGetMode(i), gpioRead(i));
   }
}













#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdint.h>

static volatile uint* gpio;

int main(int argc, char **argv)
{
        int fd ;

        //Obtain handle to physical memory
        if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
                printf("Unable to open /dev/mem: %s\n", strerror(errno));
                return -1;
        }


        //map a page of memory to gpio at offset 0x20200000 which is where GPIO goodnessstarts
        gpio = (uint32_t *)mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x20200000);

        if ((int32_t)gpio < 0){
                printf("Mmap failed: %s\n", strerror(errno));
                return -1;
        }

        //set gpio17 as an output
        //increment the pointer to 0x20200004
        //set the value through a little bit twiddling where we only modify the bits 21-23 in the register
        *(gpio + 1) = (*(gpio + 1) & ~(7 << 21)) | (1 << 21);

        //toggle gpio17 every second
        while(1){
                //set the pin high
                //increment the pointer to 0x2020001C
                *(gpio + 7) = 1 << 17;

                //sleep
                sleep(1);

                //set the pin to low
                //increment the pointer to 0x20200028
                *(gpio + 10) = 1 << 17;

                sleep(1);
        }
}


#include <stdio.h>      // perror
#include <sys/mman.h>   // mmap
#include <sys/types.h>  // fd open
#include <sys/stat.h>   // fd open
#include <fcntl.h>      // fd open
#include <unistd.h>     // fd close

#define BCM2708_PERI_BASE       0x20000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000)  // GPIO controller

#define BLOCK_SIZE              (4 * 1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x)
#define INP_GPIO(g)         *(gpio.addr + ((g) / 10)) &= ~(7 << (((g) % 10) * 3))
#define OUT_GPIO(g)         *(gpio.addr + ((g) / 10)) |=  (1 << (((g) % 10) * 3))
#define SET_GPIO_ALT(g, a)  *(gpio.addr + (((g) / 10))) |= (((a) <= 3 ? (a) + 4 : (a) == 4 ? 3 : 2) << (((g) % 10) * 3))

#define GPIO_SET            *(gpio.addr + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR            *(gpio.addr + 10) // clears bits which are 1 ignores bits which are 0

#define GPIO_READ(g)        *(gpio.addr + 13) &= (1 << (g))

struct bcm2835_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void *map;
    volatile unsigned int *addr;
};

struct bcm2835_peripheral gpio = { GPIO_BASE };

// Exposes the physical address defined in the passed structure using mmap on /dev/mem
int map_peripheral(struct bcm2835_peripheral *p)
{
    // Open /dev/mem
    if ((p->mem_fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
        fprintf(stderr, "Failed to open /dev/mem, try checking permissions.");
        return -1;
    }

    p->map = mmap(
                NULL,
                BLOCK_SIZE,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
                p->addr_p       // Address in physical map that we want this memory block to expose
                );

    if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    p->addr = (volatile unsigned int *)p->map;

    return 0;
}

void unmap_peripheral(struct bcm2835_peripheral *p)
{

    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}

int main()
{
    if(map_peripheral(&gpio) == -1) {
        fprintf(stderr, "Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }

    // Define pin 7 as output
    INP_GPIO(4);
    OUT_GPIO(4);

    while(1) {
        // Toggle pin 7 (blink a led!)
        GPIO_SET = 1 << 4;
        sleep(1);

        GPIO_CLR = 1 << 4;
        sleep(1);
    }

    return 0;
}
