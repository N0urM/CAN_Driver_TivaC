/*
 * common.h
 *
 * Created: 8/23/2019 1:29:45 PM
 *  Author: ebrah
 */ 


#ifndef COMMON_H_
#define COMMON_H_


typedef unsigned char uint8;
typedef  char sint8;
typedef unsigned int  uint16;
typedef  int  sint16;
typedef char int8;
typedef uint8 bool_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64;


#define ACCESS_REG_8BIT(REG)         (*(volatile uint8*)REG)

#define ACCESS_REG_16BIT(REG)         (*(volatile uint16*)REG)

#define READ_REG_8BIT(REG)           (*(volatile uint8*)(REG))

#define READ_REG_16BIT(REG)           (*(volatile uint16*)(REG))

#define WRITE_REG_8BIT(REG,Val)      (ACCESS_REG_8BIT(REG))=(Val);

#define SET_BIT(REG,BIT_NUM)		 (ACCESS_REG_8BIT(REG)|=1<<(BIT_NUM))

#define CLEAR_BIT(REG,BIT_NUM)       (ACCESS_REG_8BIT(REG) &=(~(1<<(BIT_NUM))))

#define BIT_IS_SET(REG,BIT_NUM)      ((ACCESS_REG_8BIT(REG) & (1 << BIT_NUM)))

#define BIT_IS_CLEAR(REG,BIT_NUM)    (!(BIT_IS_SET(REG,BIT_NUM)))

#define TOGGLE_BIT(REG,BIT_NUM)  	 (ACCESS_REG_8BIT(REG) = ACCESS_REG_8BIT(REG) ^ (1<<BIT_NUM) )




#endif /* COMMON_H_ */
