#ifndef COMMON_H_
#define COMMON_H_


typedef unsigned char uint8_t;
typedef  char sint8_t;
typedef unsigned int  uint16_t;
typedef  int  sint16_t;
typedef char int8_t;
typedef uint8_t bool_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;


//#define ACCESS_REG_8BIT(REG)         (*(volatile uint8*)REG)

//#define ACCESS_REG_16BIT(REG)         (*(volatile uint16*)REG)

//#define READ_REG_8BIT(REG)           (*(volatile uint8*)(REG))

//#define READ_REG_16BIT(REG)           (*(volatile uint16*)(REG))

//#define WRITE_REG_8BIT(REG,Val)      (ACCESS_REG_8BIT(REG))=(Val);


#define	    READ_REG(REG)				(*(volatile uint32_t*) REG)

#define		WRITE_REG(REG, VAL)			 ((*(volatile uint32_t*) REG) = VAL)

#define     SET_BIT(REG,BIT_NUM)		 ((*(volatile uint32_t*) REG) |= 1<<(BIT_NUM))

#define     CLEAR_BIT(REG,BIT_NUM)       ((*(volatile uint32_t*) REG) &= (~(1<<(BIT_NUM))))

#define     BIT_IS_SET(REG,BIT_NUM)      ((*(volatile uint32_t*) REG) & (1 << BIT_NUM))

#define     BIT_IS_CLEAR(REG,BIT_NUM)    (!(BIT_IS_SET(REG,BIT_NUM)))

#define     TOGGLE_BIT(REG,BIT_NUM)  	 ((*(volatile uint32_t*) REG) = (*(volatile uint32_t*) REG) ^ (1<<BIT_NUM) )



#endif /* COMMON_H_ */
