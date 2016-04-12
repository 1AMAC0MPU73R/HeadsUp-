/* ---------- T I M E . H --------- */


#ifndef _TIME_H_
#define _TIME_H_


#define RTC_12AM 0x40			/* TIME DEFINITIONS */
#define RTC_12PM 0x60
#define RTC_SUN 0x01
#define RTC_MON 0x02
#define RTC_TUE 0x03
#define RTC_WED 0x04
#define RTC_THU 0x05
#define RTC_FRI 0x06
#define RTC_SAT 0x07
#define RTC_JAN 0x01
#define RTC_FEB 0x02
#define RTC_MAR 0x03
#define RTC_APR 0x04
#define RTC_MAY 0x05
#define RTC_JUN 0x06
#define RTC_JUL 0x07
#define RTC_AUG 0x08
#define RTC_SEP 0x09
#define RTC_OCT 0x10
#define RTC_NOV 0x11
#define RTC_DEC 0x12


typedef uint16_t year;		/* TIME TYPES */
typedef uint8_t month;
typedef uint16_t day;
typedef uint16_t hour;
typedef uint16_t minute;
typedef uint16_t second;
typedef bool period;


class time {
	public:
		time time();							/* TIME DEFAULT CONSTRUCTOR */
		
		year Get_Year(){ return yrYears; }		/* TIME DEFAULT CONSTRUCTOR */
		month Get_Month(){ return monMonths; }	
		day Get_Day(){ return dayDays; }	
		hour Get_Hour(){ return hrHours; }	
		minute Get_Minute(){ return mntMinutes; }	
		second Get_Second(){ return scdSeconds; }	
		period Get_Period(){ return prdAMPM; }	

		void Set_Time(year yrToBeSet, month monToBeSet, day dayToBeSet, hour hrToBeSet, minute mntToBeSet, second scdToBeSet, period prdToBeSet);	/* TIME MUTATOR FUNCTIONS */
		void Set_Year(year yrToBeSet);
		void Set_Month(month monToBeSet);
		void Set_Day(day dayToBeSet);
		void Set_Hour(hour hrToBeSet);
		void Set_Minute(minute mntToBeSet);
		void Set_Second(second scdToBeSet);
		void Set_Period(period prdToBeSet);

	private:
		year yrYears;							/* DATA MEMBERS */
		month monMonths;
		day dayDays;
		hour hrHours;
		minute mntMinutes;
		second scdSeconds;
		period prdAMPM;
};


#endif
