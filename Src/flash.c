#include "main.h"
#include "ds1302.h"

#include <stdio.h>
#include <string.h>

extern t_ds1302 ds1302;
/**************************************************
     Flash module organization(STM32F411)
                               512KBytes

Name        Block base address              size
====      ==================               =======
Sector 0    0x8000000-0x8003FFF           16K bytes
Sector 1    0x8004000-0x8007FFF           16K bytes
Sector 2    0x8008000-0x800BFFF           16K bytes
Sector 3    0x800C000-0x800FFFF           16K bytes
Sector 4    0x8010000-0x801FFFF           64K bytes
Sector 5    0x8020000-0x803FFFF          128K bytes
Sector 6    0x8040000-0x805FFFF          128K bytes
Sector 7    0x8060000-0x807FFFF          128K bytes

******************************************************/

// 0x8060000-0x807FFFF 의 빈공간에 사용자 데이터를 flash programming
// 하도록 설정 한다.
#define ADDR_FLASH_SECTOR7      ((uint32_t) 0x8060000)
#define FLASH_USER_START_ADDR   ((uint32_t) 0x8060000)
#define USER_DATA_ADDRESS        0x8060000
#define FLASH_USER_END_ADDR     ((uint32_t) 0x807FFFF)
#define FLASH_INIT_STATUS       0xFFFFFFFF        // flash 초기 상태
#define FLASH_NOT_INIT_STATUS   0xAAAAAAAA        // flash 초기 상태가 아니다
#define DATA_32                 ((uint32_t) 0x00000001)

HAL_StatusTypeDef flash_read(uint32_t *addr32, int size);
HAL_StatusTypeDef flash_write (uint32_t *data32, int size);
HAL_StatusTypeDef flash_erase();

void flash_main();
void flash_set_time(void);
void set_alarm_time(char *time);

uint32_t flash_read_value=0;

typedef struct student
{
	uint32_t magic;
    int num;        // hakbun
    char name[20];  // name
    double grade;   // hakjum
} t_student;

typedef struct
{
  uint32_t magic;   // 4
  uint8_t Hours;
  uint8_t Minutes;
  uint8_t Seconds;
  uint8_t dummy;    // dummy 1 byte : 메모리 얼라인먼트를 위해서 1 word를 맞추기 위해서
} t_set_time;

t_student student;
t_set_time set_time;

void flash_main()
{

	t_student *read_student;

	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS;

	if (flash_read_value == FLASH_INIT_STATUS)  // 초기에 아무런 데이터도 존재 하지 않을 경우
	{
		flash_erase();
printf("flash EMPTY !!!!\n");
		student.magic=0x55555555;
		student.num=1101815;
		strncpy((char *) &student.name,"Hong_Gil_Dong", strlen("Hong_Gil_Dong"));
		student.grade=4.0;
		printf("w grade: %lf\n", student.grade);
		flash_write((uint32_t *) &student, sizeof(student));
	}
	else   // 1번 이상 flash memory에 데이터를 write 한 경우
	{
		flash_read((uint32_t *)&student, sizeof(student));
		printf("magic: %08x\n", student.magic);
		printf("num: %08x\n", 	student.num);
		printf("name: %s\n", student.name);
		printf("r grade: %lf\n", student.grade);
	}
}

void set_alarm_time(char *time)
{
	char hh[4], min[4], ss[4];  // time

	strncpy(hh, time, 2);
	strncpy(min,time+2, 2);
	strncpy(ss, time+4, 2);

	// 1. ascii --> int
	set_time.magic=0x55555555;  // 사용자가 임의로 설정
	set_time.Hours = atoi(hh);
	set_time.Minutes = atoi(min);
	set_time.Seconds = atoi(ss);

	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS;

	flash_erase();

	flash_write((uint32_t *) &set_time, sizeof(set_time));
}
void flash_set_time(void)
{
#if 1
	t_ds1302 *read_set_time;

	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS;

	if (flash_read_value == FLASH_INIT_STATUS)  // 초기에 아무런 데이터도 존재 하지 않을 경우
	{
		flash_erase();
printf("flash EMPTY !!!!\n");
		init_date_time();
		flash_write((uint32_t *) &ds1302, sizeof(ds1302));
	}
	else   // 1번 이상 flash memory에 데이터를 write 한 경우
	{
		flash_read((uint32_t *)&ds1302, sizeof(ds1302));

		printf("year: %02d\n", ds1302.year);
		printf("month: %02d\n", ds1302.month);
		printf("date: %02d\n", ds1302.date);
		printf("Hours: %02d\n", ds1302.hours);
		printf("Minutes: %02d\n", ds1302.minutes);
		printf("Seconds: %02d\n", ds1302.seconds);
	}
#else  // org
	t_set_time *read_set_time;

	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS;

	if (flash_read_value == FLASH_INIT_STATUS)  // 초기에 아무런 데이터도 존재 하지 않을 경우
	{
		flash_erase();
printf("flash EMPTY !!!!\n");
		set_time.magic=0x55555555;  // 사용자가 임의로 설정
		set_time.Hours=11;
		set_time.Minutes=30;
		set_time.Seconds=00;
		flash_write((uint32_t *) &set_time, sizeof(set_time));
	}
	else   // 1번 이상 flash memory에 데이터를 write 한 경우
	{
		flash_read((uint32_t *)&set_time, sizeof(set_time));

		printf("magic: %08x\n", set_time.magic);
		printf("Hours: %02d\n", set_time.Hours);
		printf("Minutes: %02d\n", set_time.Minutes);
		printf("Seconds: %02d\n", set_time.Seconds);
	}
#endif
}
HAL_StatusTypeDef flash_write (uint32_t *data32, int size)
{
	uint32_t *mem32 = data32;

	// mem32 = data32;

  /* Unlock to control */
  HAL_FLASH_Unlock();

  uint32_t Address = FLASH_USER_START_ADDR;

  printf("size: %d\n", size);

  /* Writing data to flash memory */
  for (int i=0; i < size; )
  {
	  if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *mem32) == HAL_OK)
	  {
		  printf("mem32: %0x\n", *mem32);
		  mem32++;
		  Address = Address + 4;
		  i += 4;
	  }
	  else
	  {
		  uint32_t errorcode = HAL_FLASH_GetError();
		  return HAL_ERROR;
	  }
  }
  /* Lock flash control register */
  HAL_FLASH_Lock();

  return HAL_OK;
}

HAL_StatusTypeDef flash_read(uint32_t *addr32, int size)
{
  uint32_t *data32 = addr32;
  uint32_t address = FLASH_USER_START_ADDR;
  uint32_t end_address = FLASH_USER_START_ADDR + size;

  while(address < end_address)
  {
    *data32 = *(uint32_t*) address;
    data32++;
    address = address + 4;
  }

  return HAL_OK;

}


HAL_StatusTypeDef flash_erase()
{
	uint32_t SectorError = 0;

	/* Unlock to control */
	HAL_FLASH_Unlock();

	/* Calculate sector index */
	uint32_t UserSector = 7;     // sector 번호
	uint32_t NbOfSectors = 1;    // sector 수

	/* Erase sectors */
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = UserSector;
	EraseInitStruct.NbSectors = NbOfSectors;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
	{
		uint32_t errorcode = HAL_FLASH_GetError();
		return HAL_ERROR;
	}

	/* Lock flash control register */
	HAL_FLASH_Lock();

	return HAL_OK;
}
