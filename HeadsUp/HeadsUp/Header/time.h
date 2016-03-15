/* ---------- T I M E . H --------- */


#ifndef _TIME_H_
#define _TIME_H_


#define PER_AM 0			/* TIME DEFINITIONS */
#define PER_PM 1
#define DAY_SUN 0
#define DAY_MON 1
#define DAY_TUE 2
#define DAY_WED 3
#define DAY_THU 4
#define DAY_FRI 5
#define DAY_SAT 6
#define MON_JAN 0
#define MON_FEB 1
#define MON_MAR 2
#define MON_APR 3
#define MON_MAY 4
#define MON_JUN 5
#define MON_JUL 6
#define MON_AUG 7
#define MON_SEP 8
#define MON_OCT 9
#define MON_NOV 10
#define MON_DEC 11


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
