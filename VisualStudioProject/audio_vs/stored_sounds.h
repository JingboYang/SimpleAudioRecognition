#ifndef SOUDNS_H
#define SOUNDS_H

// For .h file
// We must declare this stuff last so that referenced variables
// are defined in the file
#define LEN_AUDIO_LIB_1 36
#define ENTER    0x005
#define OPEN     0x001
#define LIGHT    0x008
#define CLOSE    0x002
#define GUAN     0x004
#define KAI      0x003
#define LEN_AUDIO_LIB_2 29
#define DENG     0x007
#define DOOR     0x006
#define OFF      0x00A
#define ON       0x009



#define OPEN_DOOR 		0x16
#define CLOSE_DOOR 		0x26

#define KAI_DENG 		0x37
#define GUAN_DENG 		0x47

#define LIGHT_ON		0x89
#define LIGHT_OFF		0x8A

#define ENTER_FULL		0x50
#define ENTER_PART		0x05

typedef
struct vartab {
	char const* name;
	int rep;
	float** var;
	int length;
} vartab;

vartab varTable_1[LEN_AUDIO_LIB_1];
vartab varTable_2[LEN_AUDIO_LIB_2];


//--- Start of group 1 -----
float close_1[418];
float close_2[418];
float close_3[594];
float close_4[506];
float close_5[462];
float enter_2[462];
float enter_4[374];
float enter_5[550];
float enter_6[418];
float guan_1[418];
float guan_2[418];
float guan_3[462];
float guan_4[330];
float guan_5[638];
float guan_6[594];
float guan_7[506];
float kai_1[418];
float kai_2[242];
float kai_3[330];
float kai_4[242];
float kai_5[638];
float kai_6[550];
float light_1[286];
float light_2[242];
float light_3[198];
float light_4[550];
float light_5[374];
float light_6[198];
float light_7[286];
float open_1[594];
float open_2[682];
float open_3[902];
float open_4[726];
float open_5[462];
float open_6[638];
float open_7[638];
//--- End of group 1 -----
//--- Start of group 2 -----
float deng_1[374];
float deng_2[330];
float deng_3[330];
float deng_4[374];
float deng_5[330];
float deng_6[286];
float door_1[418];
float door_2[594];
float door_3[594];
float door_4[286];
float door_5[330];
float door_6[330];
float door_7[462];
float off_1[418];
float off_2[330];
float off_3[374];
float off_5[374];
float off_6[286];
float off_7[374];
float off_8[682];
float off_9[550];
float on_1[462];
float on_2[418];
float on_3[506];
float on_4[506];
float on_5[506];
float on_6[550];
float on_7[462];
float on_8[330];
//--- End of group 2 -----




#endif
