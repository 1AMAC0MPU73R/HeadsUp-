/* ---------- A L A R M . H --------- */


#ifndef __ALARM_H_INCLUDED__
#define __ALARM_H_INCLUDED__ 


#define ALARM_TONE_0 0			/* ALARM DEFINITIONS */
#define ALARM_TONE_0_NAME 'None'
#define ALARM_TONE_1 1
#define ALARM_TONE_1_NAME 'Default'
#define ALARM_TONE_2 2
#define ALARM_TONE_2_NAME 'Take it Easy'
#define ALARM_TONE_3 1
#define ALARM_TONE_3_NAME 'Get Hyped'
#define ALARM_TONE_0 0			/* ALARM DEFINITIONS */
#define ALARM_LIGHT_0_NAME 'None'
#define ALARM_LIGHT_1 1
#define ALARM_LIGHT_1_NAME 'Gradual'
#define ALARM_LIGHT_2 2
#define ALARM_LIGHT_2_NAME 'Fast'
#define ALARM_LIGHT_3 1
#define ALARM_LIGHT_3_NAME 'Pulse'



typedef uint8_t tone;		/* ALARM TYPES */



class time {
	public:
		time time();							/* TIME DEFAULT CONSTRUCTOR */

	private:
		time timActivation;							/* DATA MEMBERS */
		tone tonAlarm
};


#endif 