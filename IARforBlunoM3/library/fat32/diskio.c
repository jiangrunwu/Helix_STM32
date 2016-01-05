/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
#include "sd.h"		/* Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/





int ff_sd_ioctl(uint8_t cmd, void *vbuf)
{
   
    switch (cmd)
    {
    case CTRL_SYNC :

            return 0;
         
    case GET_SECTOR_COUNT:
                      *(DWORD *)vbuf = 4194304;

           return 0;
           
     case GET_SECTOR_SIZE :
                       
                       *(DWORD *)vbuf = 512;
            return 0;
           
       
    case GET_BLOCK_SIZE :
                       *(DWORD *)vbuf = 4194304;
            return 0;
           
    }
   
  return 0;
}
DSTATUS disk_status (BYTE pdrv)	/* Physical drive nmuber to identify the drive */
{
  
	DSTATUS stat;
	int result;

	switch (pdrv) {
          
	case ATA :
		//result = ATA_disk_status();

		// translate the reslut code here

		return RES_OK;

	case MMC :
		result = RES_OK;

		// translate the reslut code here

		return RES_OK;

	case USB :
		//result = USB_disk_status();

		// translate the reslut code here

		return RES_OK;
	}
        
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
		//result = ATA_disk_initialize();

		// translate the reslut code here

		return RES_OK;

	case MMC :
		SD_Initialize();

		// translate the reslut code here

		return RES_OK;

	case USB :
		//result = USB_disk_initialize();

		// translate the reslut code here

		return RES_OK;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		//result = ATA_disk_read(buff, sector, count);

		// translate the reslut code here

		return RES_OK;

	case MMC :
		// translate the arguments here

		result = SD_ReadDisk(buff, sector, count);

		// translate the reslut code here

		return RES_OK;

	case USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return RES_OK;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		//result = ATA_disk_write(buff, sector, count);

		// translate the reslut code here

		return RES_OK;

	case MMC :
		// translate the arguments here

		result = SD_WriteDisk((const BYTE *)buff, sector, count);

		// translate the reslut code here

		return RES_OK;

	case USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return RES_OK;
	}

	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :

		// Process of the command for the ATA drive

		return RES_OK;

	case MMC :
          
                 res = ff_sd_ioctl(cmd, buff);
          

		// Process of the command for the MMC/SD card

		return res;

	case USB :

		// Process of the command the USB drive

		return RES_OK;
	}

	return RES_PARERR;
}
#endif



int get_fattime(void)
{

  
  return 0;

}